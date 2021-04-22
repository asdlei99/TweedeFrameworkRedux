#pragma once

#include "TeCorePrerequisites.h"

namespace te
{
    class TE_CORE_EXPORT HardwareBuffer
    {
    public:
        virtual ~HardwareBuffer() = default;

        /**
         * Locks a portion of the buffer and returns pointer to the locked area. You must call unlock() when done.
         *
         * @param[in]	offset		Offset in bytes from which to lock the buffer.
         * @param[in]	length		Length of the area you want to lock, in bytes.
         * @param[in]	options		Signifies what you want to do with the returned pointer. Caller must ensure not to do
         *							anything he hasn't requested (for example don't try to read from the buffer unless you
         *							requested it here).
         * @param[in]	deviceIdx	Index of the device whose memory to map. If the buffer doesn't exist on this device,
         *							the method returns null.
         * @param[in]	queueIdx	Device queue to perform any read/write operations on. See @ref queuesDoc.
         */
        virtual void* Lock(UINT32 offset, UINT32 length, GpuLockOptions options, UINT32 deviceIdx = 0, UINT32 queueIdx = 0)
        {
            assert(!IsLocked() && "Cannot lock this buffer, it is already locked!");
            void* ret = Map(offset, length, options, deviceIdx, queueIdx);
            _isLocked = true;

            return ret;
        }

        /**
         * Locks the entire buffer and returns pointer to the locked area. You must call unlock() when done.
         *
         * @param[in]	options		Signifies what you want to do with the returned pointer. Caller must ensure not to do
         *							anything he hasn't requested (for example don't try to read from the buffer unless you
         *							requested it here).
         * @param[in]	deviceIdx	Index of the device whose memory to map. If the buffer doesn't exist on this device,
         *							the method returns null.
         * @param[in]	queueIdx	Device queue to perform any read/write operations on. See @ref queuesDoc.
         */
        void* Lock(GpuLockOptions options, UINT32 deviceIdx = 0, UINT32 queueIdx = 0)
        {
            return this->Lock(0, _size, options, deviceIdx, queueIdx);
        }

        /**	Releases the lock on this buffer. */
        virtual void Unlock()
        {
            assert(IsLocked() && "Cannot unlock this buffer, it is not locked!");

            Unmap();
            _isLocked = false;
        }

        /**
         * Reads data from a portion of the buffer and copies it to the destination buffer. Caller must ensure destination
         * buffer is large enough.
         *
         * @param[in]	offset		Offset in bytes from which to copy the data.
         * @param[in]	length		Length of the area you want to copy, in bytes.
         * @param[in]	dest		Destination buffer large enough to store the read data. Data is written from the start
         *							of the buffer (@p offset is only applied to the source).
         * @param[in]	deviceIdx	Index of the device whose memory to read. If the buffer doesn't exist on this device,
         *							no data will be read.
         * @param[in]	queueIdx	Device queue to perform the read operation on. See @ref queuesDoc.
         */
        virtual void ReadData(UINT32 offset, UINT32 length, void* dest, UINT32 deviceIdx = 0, UINT32 queueIdx = 0) = 0;

        /**
         * Writes data into a portion of the buffer from the source memory.
         *
         * @param[in]	offset		Offset in bytes from which to copy the data.
         * @param[in]	length		Length of the area you want to copy, in bytes.
         * @param[in]	source		Source buffer containing the data to write. Data is read from the start of the buffer
         *							(@p offset is only applied to the destination).
         * @param[in]	writeFlags	Optional write flags that may affect performance.
         * @param[in]	queueIdx	Device queue to perform the write operation on. See @ref queuesDoc.
         */
        virtual void WriteData(UINT32 offset, UINT32 length, const void* source,
            BufferWriteType writeFlags = BWT_NORMAL, UINT32 queueIdx = 0) = 0;

        /**
         * Copies data from a specific portion of the source buffer into a specific portion of this buffer.
         *
         * @param[in]	srcBuffer			Buffer to copy from.
         * @param[in]	srcOffset			Offset into the source buffer to start copying from, in bytes.
         * @param[in]	dstOffset			Offset into this buffer to start copying to, in bytes.
         * @param[in]	length				Size of the data to copy, in bytes.
         * @param[in]	discardWholeBuffer	Specify true if the data in the current buffer can be entirely discarded. This
         *									may improve performance.
         */
        virtual void CopyData(HardwareBuffer& srcBuffer, UINT32 srcOffset, UINT32 dstOffset, UINT32 length,
            bool discardWholeBuffer = false) = 0;

        /**
         * Copy data from the provided buffer into this buffer. If buffers are not the same size, smaller size will be used.
         *
         * @param[in]	srcBuffer		Hardware buffer to copy from.
         */
        virtual void CopyData(HardwareBuffer& srcBuffer)
        {
            UINT32 sz = std::min(GetSize(), srcBuffer.GetSize());
            CopyData(srcBuffer, 0, 0, sz, true);
        }

        /** Returns the size of this buffer in bytes. */
        UINT32 GetSize() const { return _size; }

        /**	Returns whether or not this buffer is currently locked. */
        bool IsLocked() const { return _isLocked; }

        /** Returns a mask signifying on which devices has been this buffer created on. */
        GpuDeviceFlags GetDeviceMask() const { return _deviceMask; }

    protected:
        friend class HardwareBufferManager;

        /**
         * Constructs a new buffer.
         *
         * @param[in]	size			Size of the buffer, in bytes.
         * @param[in]	usage			Hint on how the buffer is intended to be used.
         * @param[in]	deviceMask		Mask that determines on which GPU devices should the object be created on.
         */
        HardwareBuffer(UINT32 size, GpuBufferUsage usage, GpuDeviceFlags deviceMask)
            : _size(size), _usage(usage), _deviceMask(deviceMask)
        { }

        /** @copydoc Lock */
        virtual void* Map(UINT32 offset, UINT32 length, GpuLockOptions options, UINT32 deviceIdx,
            UINT32 queueIdx) {
            return nullptr;
        }

        /** @copydoc Unlock */
        virtual void Unmap() { }

    protected:
        UINT32 _size;
        GpuBufferUsage _usage;
        GpuDeviceFlags _deviceMask;

        bool _isLocked = false;
    };
}
