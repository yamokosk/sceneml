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

#include "HardwareVertexBuffer.h"

namespace TinySG
{

//-----------------------------------------------------------------------------
HardwareVertexBuffer::HardwareVertexBuffer(size_t vertexSize,
	size_t numVertices, HardwareBuffer::Usage usage,
	bool useSystemMemory, bool useShadowBuffer)
	: HardwareBuffer(usage, useSystemMemory, useShadowBuffer),
	  mNumVertices(numVertices),
	  mVertexSize(vertexSize)
{
	// Calculate the size of the vertices
	mSizeInBytes = mVertexSize * numVertices;

	// Create a shadow buffer if required
	if (mUseShadowBuffer)
	{
		mpShadowBuffer = new DefaultHardwareVertexBuffer(mVertexSize,
				mNumVertices, HardwareBuffer::HBU_DYNAMIC);
	}

}
//-----------------------------------------------------------------------------
HardwareVertexBuffer::~HardwareVertexBuffer()
{
	HardwareBufferManager* mgr = HardwareBufferManager::getSingletonPtr();
	if (mgr)
	{
		mgr->_notifyVertexBufferDestroyed(this);
	}
	if (mpShadowBuffer)
	{
		delete mpShadowBuffer;
	}
}

//-----------------------------------------------------------------------------
HardwareVertexBufferSharedPtr::HardwareVertexBufferSharedPtr(HardwareVertexBuffer* buf)
	: SharedPtr<HardwareVertexBuffer>(buf)
{

}

DefaultHardwareVertexBuffer::DefaultHardwareVertexBuffer(size_t vertexSize, size_t numVertices,
	HardwareBuffer::Usage usage)
	: HardwareVertexBuffer(vertexSize, numVertices, usage, true, false) // always software, never shadowed
{
	// Allocate aligned memory for better SIMD processing friendly.
	mpData = static_cast<unsigned char*>(AlignedMemory::allocate(mSizeInBytes));
}
//-----------------------------------------------------------------------
DefaultHardwareVertexBuffer::~DefaultHardwareVertexBuffer()
{
	AlignedMemory::deallocate(mpData);
}
//-----------------------------------------------------------------------
void* DefaultHardwareVertexBuffer::lockImpl(size_t offset, size_t length, LockOptions options)
{
	// Only for use internally, no 'locking' as such
	return mpData + offset;
}
//-----------------------------------------------------------------------
void DefaultHardwareVertexBuffer::unlockImpl(void)
{
	// Nothing to do
}
//-----------------------------------------------------------------------
void* DefaultHardwareVertexBuffer::lock(size_t offset, size_t length, LockOptions options)
{
	mIsLocked = true;
	return mpData + offset;
}
//-----------------------------------------------------------------------
void DefaultHardwareVertexBuffer::unlock(void)
{
	mIsLocked = false;
	// Nothing to do
}
//-----------------------------------------------------------------------
void DefaultHardwareVertexBuffer::readData(size_t offset, size_t length, void* pDest)
{
	assert((offset + length) <= mSizeInBytes);
	memcpy(pDest, mpData + offset, length);
}
//-----------------------------------------------------------------------
void DefaultHardwareVertexBuffer::writeData(size_t offset, size_t length, const void* pSource,
		bool discardWholeBuffer)
{
	assert((offset + length) <= mSizeInBytes);
	// ignore discard, memory is not guaranteed to be zeroised
	memcpy(mpData + offset, pSource, length);
}




}
