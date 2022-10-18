#pragma once
#ifndef PROGRAMCOMMON_SCSTREAM_H
#define PROGRAMCOMMON_SCSTREAM_H

//==================================================================================================
/// SCStream class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2006. 2. 13
    @purpose
    - SCSlot을 이용하지 않고 간단히 Stream의 데이터를 쉽게 처리하기 위해서 만든 최상위 인터페이스
    - 현재 DBProxy에서 사용중
    @note
    - 
    @history 
    - 10/08/04|waverix|change to no template to support easy debugging
                       the use case of a SCStream has a SerializeStream only based on a case study
                       (f100804.5L) change to one souece centralized code detached from template.
*/
//==================================================================================================

#include <SCSlotStruct.h>

#pragma pack(push)
#pragma pack()

//==================================================================================================
//
class SCStream
{
public:
    //
    template<class _ContainerType>
    SCStream(int number_of_items, const _ContainerType*);
    virtual ~SCStream();
    //                  0 <= position < number_of_items
    virtual bool IsEmpty(int position) const = 0;
    //
    void Clear();
    POSTYPE GetMaxSlotNum() const;
    uint8_t* GetStreamPtr();
    const uint8_t* GetStreamPtr() const;
    uint8_t* GetStreamPtr(int position);
    const uint8_t* GetStreamPtr(int position) const;
    //// full stream serialize
    //bool Serialize(uint8_t* stream, int size, eSERIALIZE serialize_type);
    //// token stream serialize
    //bool Serialize(int position, uint8_t* stream, eSERIALIZE serialize_type);
    // changes, f100818.1L, add a new interfaces
    // to solve an ambigous call stack is caused by same name methods and
    // to substitute 'Serialize' usage cases
    bool LoadStream(uint8_t* stream, int size) const;
    // changes, f100818.1L
    bool StoreStream(const uint8_t* stream, int size);
    // changes, f100818.1L
    bool LoadStreamToken(int position, uint8_t* stream, int token_size_for_debug) const;
    // changes, f100818.1L
    bool StoreStreamToken(int position, const uint8_t* stream, int token_size_for_debug);

protected:
    //
    const int number_of_items_;
    const int size_of_item_;
    const size_t total_size_; // changes, f100818.1L
    uint8_t* stream_;
private:
    //
    __DISABLE_COPY(SCStream);
};

//==================================================================================================
// implements
template<class _ContainerType>
SCStream::SCStream(int number_of_items, const _ContainerType*)
    : number_of_items_(number_of_items)
    , size_of_item_(sizeof(_ContainerType))
    , total_size_(size_of_item_ * number_of_items_)
    , stream_(NULL)
{
    BOOST_STATIC_ASSERT(sizeof(POSTYPE) == 1);
    if (number_of_items_ <= 0 || number_of_items_ >= UCHAR_MAX) {
        __debugbreak();
        return;
    }
    stream_ = reinterpret_cast<uint8_t*>(::malloc(total_size_));
}

inline void SCStream::Clear()
{
    ZeroMemory(stream_, total_size_);
}

inline POSTYPE SCStream::GetMaxSlotNum() const
{
    return static_cast<POSTYPE>(number_of_items_);
}

inline uint8_t* SCStream::GetStreamPtr()
{
    return stream_;
}

inline const uint8_t* SCStream::GetStreamPtr() const
{
    return stream_;
}

inline uint8_t* SCStream::GetStreamPtr(int position)
{
    assert(position >= 0 && position < number_of_items_);
    const size_t index_position = size_of_item_ * position;
    return &stream_[index_position];
}

inline const uint8_t* SCStream::GetStreamPtr(int position) const
{
    assert(position >= 0 && position < number_of_items_);
    const size_t index_position = size_of_item_ * position;
    return &stream_[index_position];
}

// changes, f100818.1L, add a new interfaces
// to solve an ambigous call stack is caused by same name methods and
// to substitute 'Serialize' usage cases
inline bool SCStream::LoadStream(uint8_t* stream, int size) const
{
    if (total_size_ != static_cast<size_t>(size)) {
        assert(!"check valid size and a version control");
        return false;
    }
    CopyMemory(stream, stream_, total_size_);
    return true;
}

// changes, f100818.1L, add a new interfaces
// to solve an ambigous call stack is caused by same name methods and
// to substitute 'Serialize' usage cases
inline bool SCStream::StoreStream(const uint8_t* stream, int size)
{
    if (total_size_ != static_cast<size_t>(size)) {
        assert(!"check valid size and a version control");
        return false;
    }
    CopyMemory(stream_, stream, total_size_);
    return true;
}

// changes, f100818.1L, add a new interfaces
// to solve an ambigous call stack is caused by same name methods and
// to substitute 'Serialize' usage cases
inline bool SCStream::LoadStreamToken(int position, uint8_t* stream, int token_size_for_debug) const
{
    if (position < 0 || position >= number_of_items_) {
        assert(!"you should be check using a GetMaxSlotNum first");
        return false;
    }
    __UNUSED(token_size_for_debug);
    assert(size_of_item_ == token_size_for_debug);
    const size_t index_position = size_of_item_ * position;
    CopyMemory(stream, &stream_[index_position], size_of_item_);
    return true;
}

// changes, f100818.1L, add a new interfaces
// to solve an ambigous call stack is caused by same name methods and
// to substitute 'Serialize' usage cases
inline bool SCStream::StoreStreamToken(int position, const uint8_t* stream, int token_size_for_debug)
{
    if (position < 0 || position >= number_of_items_) {
        assert(!"you should be check using a GetMaxSlotNum first");
        return false;
    }
    __UNUSED(token_size_for_debug);
    assert(size_of_item_ == token_size_for_debug);
    const size_t index_position = size_of_item_ * position;
    CopyMemory(&stream_[index_position], stream, size_of_item_);
    return true;
}

//==================================================================================================
// extra interface ... hummm...                  add const - (f100804.5L)
inline void CopyStream(RENDER_ITEMSTREAMEX& rDest, const ITEMSTREAMEX& rSrc)
{
    rDest.Part.wCode    = rSrc.Part.wCode;
    rDest.Part.byDura   = rSrc.Part.byDura;
    rDest.OptionPart    = rSrc.OptionPart;
}

//inline CopyStream(ITEMSTREAMEX & rDest, RENDER_ITEMSTREAMEX & rSrc)
//{
//  rDest.Part.wCode    = rSrc.Part.wCode;
//  rDest.Part.byDura   = rSrc.Part.byDura;
//}

//==================================================================================================

#pragma pack(pop)

#endif //PROGRAMCOMMON_SCSTREAM_H