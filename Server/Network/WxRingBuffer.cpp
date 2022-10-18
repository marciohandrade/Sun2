#include "stdafx.h"
#include "./WxRingBuffer.h"

//==============================================================================
//
//
//
//
//==============================================================================


//==============================================================================
//  @WxNote(util::)
//
namespace module {

//------------------------------------------------------------------------------
//()
//  @name  
//  @code  
WxRingBuffer::WxRingBuffer(DWORD flag)
    : flag_(flag)
    , data_(0)
    , buffer_size_(0)
    , writePos_(0)
    , readPos_(0)
{}

WxRingBuffer::WxRingBuffer(DWORD flag, DWORD buffer_size)
    : flag_(flag)
    , data_(new BYTE[buffer_size])
    , buffer_size_(buffer_size)
    , writePos_(0)
    , readPos_(0)
{
}

WxRingBuffer::WxRingBuffer(DWORD flag, BYTE* data, DWORD buffer_size)
    : flag_(flag | eSync_ExternalBuffer)
    , data_(data)
    , buffer_size_(buffer_size)
    , writePos_(0)
    , readPos_(0)
{
}

WxRingBuffer::~WxRingBuffer()
{
    if (data_ && ((flag_ & eSync_ExternalBuffer)) == 0) {
        delete[] data_;
    };
    data_ = 0;
}

void
WxRingBuffer::Reset()
{
    writePos_ = 0;
    readPos_  = 0;
}

//@WxNote if(writePos_ && readPos_) == buffer_size_ then() = data_
BOOLEAN
WxRingBuffer::Enqueue(const BYTE* data, DWORD size)
{
    DWORD writePos  = writePos_;
    DWORD readPos   = readPos_;

    if((writePos + 1) % buffer_size_ == readPos)
        return false; // full
    if(writePos < readPos) {
        //  ....WooR....
        //  0 1 2 3 4 5 6 7 8 9 [10
        //              W     R
        DWORD writableLen = DWORD(readPos - 1 - writePos);
        if(writableLen < size)
            return false;

        if(data)
            CopyMemory(data_ + writePos, data, size);
        if(flag_ & eSync_Write) {
            writePos = (writePos + size) % buffer_size_;
            InterlockedExchange((long*)&writePos_, writePos);
        } else {
            writePos_ = (writePos + size) % buffer_size_;
        }
        return true;
    } else /*if(readPos <= writePos)*/ {
        //  ....RooW...E
        //  0 1 2 3 4 5 6 7 8 9 [10
        //      R       W
        DWORD writableLen0 = DWORD(buffer_size_ - writePos);
        DWORD writableLen1 = DWORD(readPos == 0  ? --writableLen0, 0 : readPos - 1);
        if(writableLen0 + writableLen1 < size)
            return false;

        DWORD firstWrite = min(writableLen0, size);
        DWORD continueWrite = (writableLen0 < size) ? size - writableLen0 : 0;

        if(firstWrite)
            if(data)
                CopyMemory(data_ + writePos, data, firstWrite);
        if(continueWrite) {
            if(data)
                CopyMemory(data_, data + firstWrite, continueWrite);
            writePos = continueWrite;
        } else if(firstWrite) {
            writePos = (writePos + firstWrite) % buffer_size_;
        }
        if(flag_ & eSync_Write) {
            InterlockedExchange((long*)&writePos_, writePos);
        } else {
            writePos_ = writePos;
        }
    }
    return true;
}

BOOLEAN
WxRingBuffer::Dequeue(BYTE* OUT data, DWORD IN size)
{
    DWORD writePos  = writePos_;
    DWORD readPos  = readPos_;

    if(readPos == writePos) {
        return false; // empty
    } else if(readPos < writePos) {
        //  ....RooW....
        //  0 1 2 3 4 5 6 7 8 9 [10
        //            R       W
        DWORD readableLen = DWORD(writePos - readPos);
        if(readableLen < size)
            return false;

        if(data)
            CopyMemory(data, data_ + readPos, size);
        if(flag_ & eSync_Read) {
            readPos = (readPos + size) % buffer_size_;
            InterlockedExchange((long*)&readPos_, readPos);
        } else {
            readPos_ = (readPos + size) % buffer_size_;
        }
    } else if(writePos < readPos) {
        //  ....WooR...E
        //  0 1 2 3 4 5 6 7 8 9 [10
        //      W       R
        DWORD readableLen0 = DWORD(buffer_size_ - readPos_);
        DWORD readableLen1 = DWORD(writePos);
        if(readableLen0 + readableLen1 < size)
            return false;

        DWORD firstRead = min(readableLen0, size);
        DWORD continueRead = (readableLen0 < size) ? size - readableLen0 : 0;

        if(firstRead)
            if(data)
                CopyMemory(data, data_ + readPos, firstRead);
        if(continueRead) {
            if(data)
                CopyMemory(data + firstRead, data_, continueRead);
            readPos = continueRead;
        } else if(firstRead) {
            readPos = (readPos + firstRead) % buffer_size_;
        }

        if(flag_ & eSync_Read) {
            InterlockedExchange((long*)&readPos_, readPos);
        } else {
            readPos_ = readPos;
        }

    }
    return true;
}

BOOLEAN
WxRingBuffer::Peek(BYTE* OUT data, DWORD size)
{
    DWORD writePos  = writePos_;
    DWORD readPos  = readPos_;

    if(readPos == writePos) {
        return false; // empty
    } else if(readPos < writePos) {
        //  ....RooW....
        //  0 1 2 3 4 5 6 7 8 9 [10
        //            R       W
        DWORD readableLen = DWORD(writePos - readPos);
        if(readableLen < size)
            return false;
        if(data)
            CopyMemory(data, data_ + readPos, size);
    } else if(writePos < readPos) {
        //  ....WooR...E
        //  0 1 2 3 4 5 6 7 8 9 [10
        //      W       R
        DWORD readableLen0 = DWORD(buffer_size_ - readPos_);
        DWORD readableLen1 = DWORD(writePos);
        if(readableLen0 + readableLen1 < size)
            return false;

        DWORD firstRead = min(readableLen0, size);
        DWORD continueRead = (readableLen0 < size) ? size - readableLen0 : 0;

        if(firstRead)
            if(data)
                CopyMemory(data, data_ + readPos, firstRead);
        if(continueRead) {
            if(data)
                CopyMemory(data + firstRead, data_, continueRead);
        } else if(firstRead) {
            ;
        }
    }
    return true;
}

BOOLEAN
WxRingBuffer::WritableInfo(RingBufferInfo* buffer_info) const
{
    DWORD writePos  = writePos_;
    DWORD readPos   = readPos_;

    ZeroMemory(buffer_info, sizeof(*buffer_info));
    if((writePos + 1) % buffer_size_ == readPos)
        return false; // full
    if(writePos < readPos) {
        //  ....WooR....
        //  0 1 2 3 4 5 6 7 8 9 [10
        //              W     R
        DWORD writableLen = DWORD(readPos - 1 - writePos);
        buffer_info->first.buf = reinterpret_cast<char*>(writableLen ? data_ + writePos : 0);
        buffer_info->first.len = writableLen;
    } else /*if(readPos <= writePos)*/ {
        //  ....RooW...E
        //  0 1 2 3 4 5 6 7 8 9 [10
        //      R       W
        DWORD writableLen0 = DWORD(buffer_size_ - writePos);
        DWORD writableLen1 = DWORD(readPos == 0  ? --writableLen0, 0 : readPos - 1);

        buffer_info->first.buf = reinterpret_cast<char*>(writableLen0 ? data_ + writePos : 0);
        buffer_info->first.len = writableLen0;
        buffer_info->second.buf = reinterpret_cast<char*>(writableLen1 ? data_ : 0);
        buffer_info->second.len = writableLen1;
    }
    return !!(buffer_info->first.len + buffer_info->second.len);
}

BOOLEAN
WxRingBuffer::ReadableInfo(RingBufferInfo* buffer_info) const
{
    DWORD writePos  = writePos_;
    DWORD readPos  = readPos_;

    ZeroMemory(buffer_info, sizeof(*buffer_info));
    if(readPos == writePos) {
        return false; // empty
    } else if(readPos < writePos) {
        //  ....RooW....
        //  0 1 2 3 4 5 6 7 8 9 [10
        //            R       W
        DWORD readableLen = DWORD(writePos - readPos);
        buffer_info->first.buf = reinterpret_cast<char*>(readableLen ? data_ + readPos : 0);
        buffer_info->first.len = readableLen;
    } else if(writePos < readPos) {
        //  ....WooR...E
        //  0 1 2 3 4 5 6 7 8 9 [10
        //      W       R
        DWORD readableLen0 = DWORD(buffer_size_ - readPos_);
        DWORD readableLen1 = DWORD(writePos);

        buffer_info->first.buf = reinterpret_cast<char*>(readableLen0 ? data_ + readPos : 0);
        buffer_info->first.len = readableLen0;
        buffer_info->second.buf = reinterpret_cast<char*>(readableLen1 ? data_ : 0);
        buffer_info->second.len = readableLen1;
    }
    return !!(buffer_info->first.len + buffer_info->second.len);
}

}; //end of namespace
//
//
//==============================================================================
