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

#ifndef _HARDWAREVERTEXBUFFER_H_
#define _HARDWAREVERTEXBUFFER_H_

namespace TinySG {

/** Specialisation of HardwareBuffer for a vertex buffer. */
class HardwareVertexBuffer : public HardwareBuffer
{
protected:
	size_t mNumVertices;
	size_t mVertexSize;

public:
	/// Should be called by HardwareBufferManager
	HardwareVertexBuffer(size_t vertexSize, size_t numVertices,
	HardwareBuffer::Usage usage, bool useSystemMemory, bool useShadowBuffer);
	~HardwareVertexBuffer();
	/// Gets the size in bytes of a single vertex in this buffer
	size_t getVertexSize(void) const { return mVertexSize; }
	/// Get the number of vertices in this buffer
	size_t getNumVertices(void) const { return mNumVertices; }
	// NB subclasses should override lock, unlock, readData, writeData
};

/** Shared pointer implementation used to share index buffers. */
class HardwareVertexBufferSharedPtr : public SharedPtr<HardwareVertexBuffer>
{
public:
	HardwareVertexBufferSharedPtr() : SharedPtr<HardwareVertexBuffer>() {}
	explicit HardwareVertexBufferSharedPtr(HardwareVertexBuffer* buf);
};


/// Specialisation of HardwareVertexBuffer for emulation
class DefaultHardwareVertexBuffer : public HardwareVertexBuffer
{
protected:
	unsigned char* mpData;
	/** See HardwareBuffer. */
	void* lockImpl(size_t offset, size_t length, LockOptions options);
	/** See HardwareBuffer. */
	void unlockImpl(void);
public:
	DefaultHardwareVertexBuffer(size_t vertexSize, size_t numVertices,
		HardwareBuffer::Usage usage);
	~DefaultHardwareVertexBuffer();
	/** See HardwareBuffer. */
	void readData(size_t offset, size_t length, void* pDest);
	/** See HardwareBuffer. */
	void writeData(size_t offset, size_t length, const void* pSource,
			bool discardWholeBuffer = false);
	/** Override HardwareBuffer to turn off all shadowing. */
	void* lock(size_t offset, size_t length, LockOptions options);
	/** Override HardwareBuffer to turn off all shadowing. */
	void unlock(void);


};


}
#endif

