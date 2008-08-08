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



/// Specialisation of HardwareIndexBuffer for emulation
class _OgreExport DefaultHardwareIndexBuffer : public HardwareIndexBuffer
{
protected:
	unsigned char* mpData;
	/** See HardwareBuffer. */
	void* lockImpl(size_t offset, size_t length, LockOptions options);
	/** See HardwareBuffer. */
	void unlockImpl(void);
public:
	DefaultHardwareIndexBuffer(IndexType idxType, size_t numIndexes, HardwareBuffer::Usage usage);
	~DefaultHardwareIndexBuffer();
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

/** Specialisation of HardwareBufferManager to emulate hardware buffers.
@remarks
	You might want to instantiate this class if you want to utilise
	classes like MeshSerializer without having initialised the
	rendering system (which is required to create a 'real' hardware
	buffer manager.
*/
class _OgreExport DefaultHardwareBufferManager : public HardwareBufferManager
{
public:
	DefaultHardwareBufferManager();
	~DefaultHardwareBufferManager();
	/// Creates a vertex buffer
	HardwareVertexBufferSharedPtr
		createVertexBuffer(size_t vertexSize, size_t numVerts,
			HardwareBuffer::Usage usage, bool useShadowBuffer = false);
	/// Create a hardware vertex buffer
	HardwareIndexBufferSharedPtr
		createIndexBuffer(HardwareIndexBuffer::IndexType itype, size_t numIndexes,
			HardwareBuffer::Usage usage, bool useShadowBuffer = false);

};
