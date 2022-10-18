#include "UtilityCommon.h"
#include "./BitStream_v2.hpp"

//==================================================================================================

BitStream::BitStream(void* IN buffer, size_t buffer_length, bool writing)
    : buffer_ptr_(reinterpret_cast<uint8_t*>(buffer))
    , buffer_end_(buffer_ptr_ + buffer_length)
    , pos_(buffer_ptr_)
    , offset_(0)
{
    mode_ = (buffer_ptr_ && buffer_ptr_ < buffer_end_) ? eNone : eError;
    if (mode_ == eNone && writing) {
        ZeroMemory(buffer_ptr_, static_cast<size_t>(buffer_end_ - buffer_ptr_));
    };
}


template<class ConvT_>
__forceinline void
BitStream::_WriteBitsUnit(const ConvT_* value, int write_bits)
{
    const ulong write_bit_unit = sizeof(*value) * 8;
    BOOST_STATIC_ASSERT(write_bit_unit < (32 + 1));
    assert(write_bits < write_bit_unit + 1);
    // value == 3, write_bits == 2
    //
    // if ConvT_ == byte and offset_ == 9
    // status = { pos == { 1, 0 } }
    //    used   = 9 & (8 - 1) --> 1
    //    remain = 8 - used(1) --> 7
    //
    //        1       3        1
    // pos = pos | (value << used) --> 00000001 | 00000110
    //
    int used = offset_ & (write_bit_unit - 1); //mod
    int remain = write_bit_unit - used;

    //assume clear stream in 'not set region'
    ConvT_ filtered = *value;
    if (write_bit_unit != write_bits) {
        int offset = write_bit_unit - write_bits;
        if (offset != 0) // enable remove
            filtered = ConvT_(ConvT_(filtered << offset) >> offset);
    }

    ConvT_* pos = reinterpret_cast<ConvT_*>(buffer_ptr_);
    const int kTypeArrayOffset =
        boost::mpl::if_c<sizeof(char)  == sizeof(*value), boost::mpl::integral_c<int, 3>,
        boost::mpl::if_c<sizeof(short) == sizeof(*value), boost::mpl::integral_c<int, 4>,
        boost::mpl::if_c<sizeof(long)  == sizeof(*value), boost::mpl::integral_c<int, 5>,
        boost::mpl::void_>::type>::type>::type::value;
    //
    pos = &pos[offset_ >> kTypeArrayOffset];
    // write
    *pos = *pos | ConvT_(filtered << used);
    int next_op = write_bits - remain;
    if (next_op > 0) {
        ++pos;
        *pos = *pos | ConvT_(filtered >> remain);
    }
    offset_ += write_bits;
}

//------------------------------------------------------------------------------

bool BitStream::WriteBits(const void* input, size_t write_bits, size_t input_size)
{
    __TOUCHED(input_size);
    // warning check boundary in usage case
    if (write_bits == 0) {
        return true; //TODO more thinking!
    };
    const int total_bits = static_cast<int>(buffer_end_ - buffer_ptr_) << 3;
    int remains = total_bits - offset_;
    if (remains <= 0 || (ulong)remains < write_bits) {
        assert(!"index out of ranges");
        return false;
    };
    //8, 16, 32, 64?
    const uint16_t* offset_d2 = reinterpret_cast<const uint16_t*>(input);
    while (16 <= write_bits) {
        _WriteBitsUnit(offset_d2, 16);
        ++offset_d2; write_bits -= 16;
    }

    const uint8_t* offset_d1 = reinterpret_cast<const uint8_t*>(offset_d2);
    while (8 <= write_bits) {
        _WriteBitsUnit(offset_d1, 8);
        ++offset_d1; write_bits -= 8;
    }

    if (write_bits != 0) {
        _WriteBitsUnit(offset_d1, static_cast<int>(write_bits));
    };
    return true;
}

//------------------------------------------------------------------------------

template<class ConvT_>
__forceinline void
BitStream::_ReadBitsUnit(ConvT_* value, int read_bits) const
{
    const ulong read_bit_unit = sizeof(*value) * 8;
    BOOST_STATIC_ASSERT(read_bit_unit < (32 + 1));
    assert(read_bits < read_bit_unit + 1);

    int skip = offset_ & (read_bit_unit - 1); //mod
    int remain = read_bit_unit - skip;

    ConvT_* pos = reinterpret_cast<ConvT_*>(buffer_ptr_);
    const int kTypeArrayOffset =
        boost::mpl::if_c<sizeof(char)  == sizeof(*value), boost::mpl::integral_c<int, 3>,
        boost::mpl::if_c<sizeof(short) == sizeof(*value), boost::mpl::integral_c<int, 4>,
        boost::mpl::if_c<sizeof(long)  == sizeof(*value), boost::mpl::integral_c<int, 5>,
        boost::mpl::void_>::type>::type>::type::value;
    //
    pos = &pos[offset_ >> kTypeArrayOffset];

    ConvT_ filtered = *pos;
    int offset = remain - read_bits;
    int next_op = 0;
    if (offset > 0) {
        filtered = ConvT_(ConvT_(filtered << offset) >> (offset + skip));
    }
    else {
        filtered = ConvT_(filtered >> skip);
        next_op = -offset;
    }

    if (next_op != 0) { // more read
        ++pos;
        next_op = read_bit_unit - next_op; // ex) 8 - 5(remain read bits)
        ConvT_ temp = ConvT_(ConvT_(*pos << next_op) >> next_op); // clear unused bits
        temp = ConvT_(temp << remain);
        filtered = filtered | temp;
    }
    offset_ += read_bits;
    *value = filtered;
}

//------------------------------------------------------------------------------

bool BitStream::ReadBits(void* output, size_t read_bits, size_t output_size) const
{
    // warning check boundary in usage case
    ZeroMemory(output, output_size);
    if (read_bits == 0) {
        return true; //TODO more thinking!
    };
    const int total_bits = static_cast<int>(buffer_end_ - buffer_ptr_) << 3;
    int remains = total_bits - offset_;
    if (remains <= 0 || (ulong)remains < read_bits) {
        assert(!"index out of ranges");
        return false;
    }
    uint16_t* offset_d2 = reinterpret_cast<uint16_t*>(output);
    while (16 <= read_bits) {
        _ReadBitsUnit(offset_d2, 16);
        ++offset_d2; read_bits -= 16;
    }

    uint8_t* offset_d1 = reinterpret_cast<uint8_t*>(offset_d2);
    while (8 <= read_bits) {
        _ReadBitsUnit(offset_d1, 8);
        ++offset_d1; read_bits -= 8;
    }

    if (read_bits != 0) {
        _ReadBitsUnit(offset_d1, static_cast<int>(read_bits));
    };
    return true;
}

//==================================================================================================
