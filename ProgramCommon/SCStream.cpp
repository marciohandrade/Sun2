#include "stdafx.h"
#include "./SCStream.h"

//==================================================================================================

SCStream::~SCStream()
{
    if (stream_) {
        ::free(stream_);
        stream_ = NULL;
    };
}
/*
// (f100804.5L) full stream serialize
bool SCStream::Serialize(uint8_t* stream, int size, eSERIALIZE serialize_type)
{
    const size_t total_size = size_of_item_ * number_of_items_;
    if (FlowControl::FCAssert(total_size > 0 &&
                              total_size == static_cast<size_t>(size)) == false)
    {
        return false;
    };
    //
    if (serialize_type == SERIALIZE_STORE) {
        CopyMemory(stream_, stream, total_size);
        return true;
    }
    else if (serialize_type == SERIALIZE_LOAD) {
        CopyMemory(stream, stream_, total_size);
        return true;
    }
    return false;
}

// (f100804.5L) token stream serialize
bool SCStream::Serialize(int position, uint8_t* stream, eSERIALIZE serialize_type)
{
    if (FlowControl::FCAssert(position >= 0 && position < number_of_items_) == false) {
        return false;
    }
    //
    const size_t index_position = size_of_item_ * position;
    //
    if (serialize_type == SERIALIZE_STORE) {
        CopyMemory(&stream_[index_position], stream, size_of_item_);
        return true;
    }
    else if (serialize_type == SERIALIZE_LOAD) {
        CopyMemory(stream, &stream_[index_position], size_of_item_);
        return true;
    }
    return false;
}
*/
//==================================================================================================
