/*************************************************************************
 * SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
 * All rights reserved.
 * Email: yamokosk at gmail dot com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version. The text of the GNU Lesser General
 * Public License is included with this library in the file LICENSE.TXT.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for
 * more details.
 *
 *************************************************************************/

#include "HardwareBufferManager.h"


namespace TinySG
{

//-----------------------------------------------------------------------
template<> HardwareBufferManager* Singleton<HardwareBufferManager>::ms_Singleton = 0;
HardwareBufferManager* HardwareBufferManager::getSingletonPtr(void)
{
	return ms_Singleton;
}
HardwareBufferManager& HardwareBufferManager::getSingleton(void)
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

//-----------------------------------------------------------------------
HardwareBufferManager::HardwareBufferManager()
	: mUnderUsedFrameCount(0)
{
}
//-----------------------------------------------------------------------
HardwareBufferManager::~HardwareBufferManager()
{
	// Clear vertex/index buffer list first, avoid destroyed notify do
	// unnecessary work, and we'll destroy everything here.
	mVertexBuffers.clear();
	mIndexBuffers.clear();

	// Destroy everything
	destroyAllDeclarations();
	destroyAllBindings();
	// No need to destroy main buffers - they will be destroyed by removal of bindings

	// No need to destroy temp buffers - they will be destroyed automatically.
}
//-----------------------------------------------------------------------
VertexDeclaration* HardwareBufferManager::createVertexDeclaration(void)
{
	VertexDeclaration* decl = createVertexDeclarationImpl();
	OGRE_LOCK_MUTEX(mVertexDeclarationsMutex)
	mVertexDeclarations.insert(decl);
	return decl;
}
//-----------------------------------------------------------------------
void HardwareBufferManager::destroyVertexDeclaration(VertexDeclaration* decl)
{
	OGRE_LOCK_MUTEX(mVertexDeclarationsMutex)
	mVertexDeclarations.erase(decl);
	destroyVertexDeclarationImpl(decl);
}
//-----------------------------------------------------------------------
VertexBufferBinding* HardwareBufferManager::createVertexBufferBinding(void)
{
	VertexBufferBinding* ret = createVertexBufferBindingImpl();
	OGRE_LOCK_MUTEX(mVertexBufferBindingsMutex)
	mVertexBufferBindings.insert(ret);
	return ret;
}
//-----------------------------------------------------------------------
void HardwareBufferManager::destroyVertexBufferBinding(VertexBufferBinding* binding)
{
	OGRE_LOCK_MUTEX(mVertexBufferBindingsMutex)
	mVertexBufferBindings.erase(binding);
	destroyVertexBufferBindingImpl(binding);
}
//-----------------------------------------------------------------------
VertexDeclaration* HardwareBufferManager::createVertexDeclarationImpl(void)
{
	return new VertexDeclaration();
}
//-----------------------------------------------------------------------
void HardwareBufferManager::destroyVertexDeclarationImpl(VertexDeclaration* decl)
{
	delete decl;
}
//-----------------------------------------------------------------------
VertexBufferBinding* HardwareBufferManager::createVertexBufferBindingImpl(void)
{
	return new VertexBufferBinding();
}
//-----------------------------------------------------------------------
void HardwareBufferManager::destroyVertexBufferBindingImpl(VertexBufferBinding* binding)
{
	delete binding;
}
//-----------------------------------------------------------------------
void HardwareBufferManager::destroyAllDeclarations(void)
{
	OGRE_LOCK_MUTEX(mVertexDeclarationsMutex)
	VertexDeclarationList::iterator decl;
	for (decl = mVertexDeclarations.begin(); decl != mVertexDeclarations.end(); ++decl)
	{
		destroyVertexDeclarationImpl(*decl);
	}
	mVertexDeclarations.clear();
}
//-----------------------------------------------------------------------
void HardwareBufferManager::destroyAllBindings(void)
{
	OGRE_LOCK_MUTEX(mVertexBufferBindingsMutex)
	VertexBufferBindingList::iterator bind;
	for (bind = mVertexBufferBindings.begin(); bind != mVertexBufferBindings.end(); ++bind)
	{
		destroyVertexBufferBindingImpl(*bind);
	}
	mVertexBufferBindings.clear();
}
//-----------------------------------------------------------------------
void HardwareBufferManager::registerVertexBufferSourceAndCopy(
		const HardwareVertexBufferSharedPtr& sourceBuffer,
		const HardwareVertexBufferSharedPtr& copy)
{
	OGRE_LOCK_MUTEX(mTempBuffersMutex)
	// Add copy to free temporary vertex buffers
	mFreeTempVertexBufferMap.insert(
		FreeTemporaryVertexBufferMap::value_type(sourceBuffer.get(), copy));
}
//-----------------------------------------------------------------------
HardwareVertexBufferSharedPtr
HardwareBufferManager::allocateVertexBufferCopy(
	const HardwareVertexBufferSharedPtr& sourceBuffer,
	BufferLicenseType licenseType, HardwareBufferLicensee* licensee,
	bool copyData)
{
	// pre-lock the mVertexBuffers mutex, which would usually get locked in
	//  makeBufferCopy / createVertexBuffer
	// this prevents a deadlock in _notifyVertexBufferDestroyed
	// which locks the same mutexes (via other methods) but in reverse order
	OGRE_LOCK_MUTEX(mVertexBuffersMutex)
	{
		OGRE_LOCK_MUTEX(mTempBuffersMutex)
		HardwareVertexBufferSharedPtr vbuf;

		// Locate existing buffer copy in temporary vertex buffers
		FreeTemporaryVertexBufferMap::iterator i =
			mFreeTempVertexBufferMap.find(sourceBuffer.get());
		if (i == mFreeTempVertexBufferMap.end())
		{
			// copy buffer, use shadow buffer and make dynamic
			vbuf = makeBufferCopy(
				sourceBuffer,
				HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
				true);
		}
		else
		{
			// Allocate existing copy
			vbuf = i->second;
			mFreeTempVertexBufferMap.erase(i);
		}

		// Copy data?
		if (copyData)
		{
			vbuf->copyData(*(sourceBuffer.get()), 0, 0, sourceBuffer->getSizeInBytes(), true);
		}

		// Insert copy into licensee list
		mTempVertexBufferLicenses.insert(
			TemporaryVertexBufferLicenseMap::value_type(
				vbuf.get(),
				VertexBufferLicense(sourceBuffer.get(), licenseType, EXPIRED_DELAY_FRAME_THRESHOLD, vbuf, licensee)));
		return vbuf;
	}

}
//-----------------------------------------------------------------------
void HardwareBufferManager::releaseVertexBufferCopy(
	const HardwareVertexBufferSharedPtr& bufferCopy)
{
	OGRE_LOCK_MUTEX(mTempBuffersMutex)

	TemporaryVertexBufferLicenseMap::iterator i =
		mTempVertexBufferLicenses.find(bufferCopy.get());
	if (i != mTempVertexBufferLicenses.end())
	{
		const VertexBufferLicense& vbl = i->second;

		vbl.licensee->licenseExpired(vbl.buffer.get());

		mFreeTempVertexBufferMap.insert(
			FreeTemporaryVertexBufferMap::value_type(vbl.originalBufferPtr, vbl.buffer));
		mTempVertexBufferLicenses.erase(i);
	}
}
//-----------------------------------------------------------------------
void HardwareBufferManager::touchVertexBufferCopy(
		const HardwareVertexBufferSharedPtr& bufferCopy)
{
	OGRE_LOCK_MUTEX(mTempBuffersMutex)
	TemporaryVertexBufferLicenseMap::iterator i =
		mTempVertexBufferLicenses.find(bufferCopy.get());
	if (i != mTempVertexBufferLicenses.end())
	{
		VertexBufferLicense& vbl = i->second;
		assert(vbl.licenseType == BLT_AUTOMATIC_RELEASE);

		vbl.expiredDelay = EXPIRED_DELAY_FRAME_THRESHOLD;
	}
}
//-----------------------------------------------------------------------
void HardwareBufferManager::_freeUnusedBufferCopies(void)
{
	OGRE_LOCK_MUTEX(mTempBuffersMutex)
	size_t numFreed = 0;

	// Free unused temporary buffers
	FreeTemporaryVertexBufferMap::iterator i;
	i = mFreeTempVertexBufferMap.begin();
	while (i != mFreeTempVertexBufferMap.end())
	{
		FreeTemporaryVertexBufferMap::iterator icur = i++;
		// Free the temporary buffer that referenced by ourself only.
		// TODO: Some temporary buffers are bound to vertex buffer bindings
		// but not checked out, need to sort out method to unbind them.
		if (icur->second.useCount() <= 1)
		{
			++numFreed;
			mFreeTempVertexBufferMap.erase(icur);
		}
	}

	StringUtil::StrStreamType str;
	if (numFreed)
	{
		str << "HardwareBufferManager: Freed " << numFreed << " unused temporary vertex buffers.";
	}
	else
	{
		str << "HardwareBufferManager: No unused temporary vertex buffers found.";
	}
	LogManager::getSingleton().logMessage(str.str(), LML_TRIVIAL);
}
//-----------------------------------------------------------------------
void HardwareBufferManager::_releaseBufferCopies(bool forceFreeUnused)
{
	OGRE_LOCK_MUTEX(mTempBuffersMutex)
	size_t numUnused = mFreeTempVertexBufferMap.size();
	size_t numUsed = mTempVertexBufferLicenses.size();

	// Erase the copies which are automatic licensed out
	TemporaryVertexBufferLicenseMap::iterator i;
	i = mTempVertexBufferLicenses.begin();
	while (i != mTempVertexBufferLicenses.end())
	{
		TemporaryVertexBufferLicenseMap::iterator icur = i++;
		VertexBufferLicense& vbl = icur->second;
		if (vbl.licenseType == BLT_AUTOMATIC_RELEASE &&
			(forceFreeUnused || --vbl.expiredDelay <= 0))
		{
			vbl.licensee->licenseExpired(vbl.buffer.get());

			mFreeTempVertexBufferMap.insert(
				FreeTemporaryVertexBufferMap::value_type(vbl.originalBufferPtr, vbl.buffer));
			mTempVertexBufferLicenses.erase(icur);
		}
	}

	// Check whether or not free unused temporary vertex buffers.
	if (forceFreeUnused)
	{
		_freeUnusedBufferCopies();
		mUnderUsedFrameCount = 0;
	}
	else
	{
		if (numUsed < numUnused)
		{
			// Free temporary vertex buffers if too many unused for a long time.
			// Do overall temporary vertex buffers instead of per source buffer
			// to avoid overhead.
			++mUnderUsedFrameCount;
			if (mUnderUsedFrameCount >= UNDER_USED_FRAME_THRESHOLD)
			{
				_freeUnusedBufferCopies();
				mUnderUsedFrameCount = 0;
			}
		}
		else
		{
			mUnderUsedFrameCount = 0;
		}
	}
}
//-----------------------------------------------------------------------
void HardwareBufferManager::_forceReleaseBufferCopies(
	const HardwareVertexBufferSharedPtr& sourceBuffer)
{
	_forceReleaseBufferCopies(sourceBuffer.get());
}
//-----------------------------------------------------------------------
void HardwareBufferManager::_forceReleaseBufferCopies(
	HardwareVertexBuffer* sourceBuffer)
{
	OGRE_LOCK_MUTEX(mTempBuffersMutex)
	// Erase the copies which are licensed out
	TemporaryVertexBufferLicenseMap::iterator i;
	i = mTempVertexBufferLicenses.begin();
	while (i != mTempVertexBufferLicenses.end())
	{
		TemporaryVertexBufferLicenseMap::iterator icur = i++;
		const VertexBufferLicense& vbl = icur->second;
		if (vbl.originalBufferPtr == sourceBuffer)
		{
			// Just tell the owner that this is being released
			vbl.licensee->licenseExpired(vbl.buffer.get());

			mTempVertexBufferLicenses.erase(icur);
		}
	}

	// Erase the free copies
	//
	// Why we need this unusual code? It's for resolve reenter problem.
	//
	// Using mFreeTempVertexBufferMap.erase(sourceBuffer) directly will
	// cause reenter into here because vertex buffer destroyed notify.
	// In most time there are no problem. But when sourceBuffer is the
	// last item of the mFreeTempVertexBufferMap, some STL multimap
	// implementation (VC and STLport) will call to clear(), which will
	// causing intermediate state of mFreeTempVertexBufferMap, in that
	// time destroyed notify back to here cause illegal accessing in
	// the end.
	//
	// For safely reason, use following code to resolve reenter problem.
	//
	typedef FreeTemporaryVertexBufferMap::iterator _Iter;
	std::pair<_Iter, _Iter> range = mFreeTempVertexBufferMap.equal_range(sourceBuffer);
	if (range.first != range.second)
	{
		std::list<HardwareVertexBufferSharedPtr> holdForDelayDestroy;
		for (_Iter it = range.first; it != range.second; ++it)
		{
			if (it->second.useCount() <= 1)
			{
				holdForDelayDestroy.push_back(it->second);
			}
		}

		mFreeTempVertexBufferMap.erase(range.first, range.second);

		// holdForDelayDestroy will destroy auto.
	}
}
//-----------------------------------------------------------------------
void HardwareBufferManager::_notifyVertexBufferDestroyed(HardwareVertexBuffer* buf)
{
	OGRE_LOCK_MUTEX(mVertexBuffersMutex)

	VertexBufferList::iterator i = mVertexBuffers.find(buf);
	if (i != mVertexBuffers.end())
	{
		// release vertex buffer copies
		mVertexBuffers.erase(i);
		_forceReleaseBufferCopies(buf);
	}
}
//-----------------------------------------------------------------------
void HardwareBufferManager::_notifyIndexBufferDestroyed(HardwareIndexBuffer* buf)
{
	OGRE_LOCK_MUTEX(mIndexBuffersMutex)

	IndexBufferList::iterator i = mIndexBuffers.find(buf);
	if (i != mIndexBuffers.end())
	{
		mIndexBuffers.erase(i);
	}
}
//-----------------------------------------------------------------------
HardwareVertexBufferSharedPtr
HardwareBufferManager::makeBufferCopy(
	const HardwareVertexBufferSharedPtr& source,
	HardwareBuffer::Usage usage, bool useShadowBuffer)
{
	return this->createVertexBuffer(
		source->getVertexSize(),
		source->getNumVertices(),
		usage, useShadowBuffer);
}

} // End TinySG namespace
