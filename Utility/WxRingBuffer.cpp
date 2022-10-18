#include "UtilityCommon.h"
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
namespace util {
;

struct WxRingBufferAutoLock 
{
    WxRingBufferAutoLock(CRITICAL_SECTION& critical_section)
        : critical_section_(critical_section)
    {
        EnterCriticalSection(&critical_section_);
    }

    ~WxRingBufferAutoLock() {
        LeaveCriticalSection(&critical_section_);
    }

    CRITICAL_SECTION& critical_section_;
    __DISABLE_COPY(WxRingBufferAutoLock);
};

#define AutoLockTransaction(_critical_section_, _code_block_)  \
    { \
        util::WxRingBufferAutoLock _lock_(_critical_section_);  \
        _code_block_  \
    }

}; //end of namespace

//------------------------------------------------------------------------------
//()
//  @name  
//  @code  
util::WxRingBuffer::WxRingBuffer(DWORD flag)
    : flag_(flag)
    , data_(0)
    , buffer_size_(0)
    , writePos_(0)
    , readPos_(0)
{}

util::WxRingBuffer::WxRingBuffer(DWORD flag, DWORD buffer_size)
    : flag_(flag)
    , data_(new BYTE[buffer_size])
    , buffer_size_(buffer_size)
    , writePos_(0)
    , readPos_(0)
{
}

//util::WxRingBuffer::WxRingBuffer(DWORD flag, BYTE* data, DWORD buffer_size)
//    : flag_(flag)
//    , data_(data)
//    , buffer_size_(buffer_size)
//    , writePos_(0)
//    , readPos_(0)
//{
//}

util::WxRingBuffer::~WxRingBuffer()
{
    if(data_)
        delete[] data_;
    data_ = 0;
}

void util::WxRingBuffer::Reset()
{
    writePos_ = 0;
    readPos_  = 0;
}

//@WxNote if(writePos_ && readPos_) == buffer_size_ then() = data_
bool util::WxRingBuffer::Enqueue(const BYTE* data, DWORD size)
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
        DWORD writableLen1 = \
            DWORD(readPos == 0  ? --writableLen0, 0 : readPos - 1);
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

bool util::WxRingBuffer::Dequeue(BYTE* OUT data, DWORD IN size)
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

bool util::WxRingBuffer::Peek(BYTE* OUT data, DWORD size)
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

bool util::WxRingBuffer::WritableInfo(RingBufferInfo* buffer_info) const
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
        buffer_info->first.buffer = \
            reinterpret_cast<uint8_t*>(writableLen ? data_ + writePos : 0);
        buffer_info->first.size = writableLen;
    } else /*if(readPos <= writePos)*/ {
        //  ....RooW...E
        //  0 1 2 3 4 5 6 7 8 9 [10
        //      R       W
        DWORD writableLen0 = DWORD(buffer_size_ - writePos);
        DWORD writableLen1 = \
            DWORD(readPos == 0 ? --writableLen0, 0 : readPos - 1);

        buffer_info->first.buffer = \
            reinterpret_cast<uint8_t*>(writableLen0 ? data_ + writePos : 0);
        buffer_info->first.size = writableLen0;
        buffer_info->second.buffer = \
            reinterpret_cast<uint8_t*>(writableLen1 ? data_ : 0);
        buffer_info->second.size = writableLen1;
    }
    return !!(buffer_info->first.size + buffer_info->second.size);
}

bool util::WxRingBuffer::ReadableInfo(RingBufferInfo* buffer_info) const
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
        buffer_info->first.buffer = \
            reinterpret_cast<uint8_t*>(readableLen ? data_ + readPos : 0);
        buffer_info->first.size = readableLen;
    } else if(writePos < readPos) {
        //  ....WooR...E
        //  0 1 2 3 4 5 6 7 8 9 [10
        //      W       R
        DWORD readableLen0 = DWORD(buffer_size_ - readPos_);
        DWORD readableLen1 = DWORD(writePos);

        buffer_info->first.buffer = \
            reinterpret_cast<uint8_t*>(readableLen0 ? data_ + readPos : 0);
        buffer_info->first.size = readableLen0;
        buffer_info->second.buffer = \
            reinterpret_cast<uint8_t*>(readableLen1 ? data_ : 0);
        buffer_info->second.size = readableLen1;
    }
    return !!(buffer_info->first.size + buffer_info->second.size);
}
//
//
//==============================================================================


util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::
WxRingBufferEx(DWORD flag)
    : util::WxRingBuffer(flag | kFlagDefault)
{
    ::InitializeCriticalSectionAndSpinCount(&cs_, 4000);
}

util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::
WxRingBufferEx(DWORD flag, DWORD buffer_size)
    : util::WxRingBuffer(flag | kFlagDefault, buffer_size)
{
    ::InitializeCriticalSectionAndSpinCount(&cs_, 4000);
}

//util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::
//WxRingBufferEx(DWORD flag, BYTE* data, DWORD buffer_size)
//    : util::WxRingBuffer(flag | kFlagDefault, data, buffer_size)
//{
//}

util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::~WxRingBufferEx()
{
    ::DeleteCriticalSection(&cs_);
}

void util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::Reset()
{
    AutoLockTransaction(cs_, {
        return util::WxRingBuffer::Reset();
    });
}

// if(data == 0) only offset control
bool util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::Enqueue(
    const BYTE* data, DWORD size)
{
    AutoLockTransaction(cs_, {
        return util::WxRingBuffer::Enqueue(data, size);
    });
}

// if(data == 0) only offset control
bool util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::Dequeue(
    BYTE* OUT data, DWORD size)
{
    return util::WxRingBuffer::Dequeue(data, size);
}

bool util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::Peek(
    BYTE* OUT data, DWORD size)
{
    return util::WxRingBuffer::Peek(data, size);
}

//bool
//util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::
//WritableInfo(RingBufferInfo* buffer_info) const
//{
//  WxNote("Useful : SR_SW | MR_SW ");
//  return util::WxRingBuffer::WritableInfo(buffer_info);
//}

bool util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::ReadableInfo(
    util::RingBufferInfo* buffer_info) const
{
    //"Useful : SR_SW | SR_MW "
    return util::WxRingBuffer::ReadableInfo(buffer_info);
}

//==============================================================================
