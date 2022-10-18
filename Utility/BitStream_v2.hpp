#pragma once
#ifndef UTILITY_BITSTREAM_V2_H
#define UTILITY_BITSTREAM_V2_H

#pragma pack(push)
#pragma pack()
//==================================================================================================
// CHANGES: f110425.4L, this class is migrated and stripped version
// from WxFramework::BitSerializer (v0120)
class BitStream
{
public:
    static const int kVersion = 0x0200;
    enum eIoType { eError, eNone, eReading, eWriting, eIoCounts };
    //
    BitStream(void* buffer, size_t number_of_bytes, bool writing);
    //
    template<class InT_>
    bool Write(const InT_* writing_ptr, size_t number_of_bits);
    template<class OutT_>
    bool Read(OutT_* const reading_ptr, size_t number_of_bits) const;
    //
    int GetNumberOfBytesUsed() const;
    //
    // NOTE: low level io interface #1
    //                                        for check 'input' type size overflow
    bool WriteBits(const void* input, size_t write_bits, size_t input_size);
    // NOTE: low level io interface #1
    //                                        for 'output' clear stream
    bool ReadBits(void* ouput, size_t read_bits, size_t output_size) const;
private:
    template<class ConvT_>
    void _WriteBitsUnit(const ConvT_* value, int write_bits);
    template<class ConvT_>
    void _ReadBitsUnit(ConvT_* value, int read_bits) const;
    // fields...
    eIoType mode_; // ref: eIoType
    uint8_t* buffer_ptr_;
    uint8_t* buffer_end_;
    mutable uint8_t* pos_;
    mutable int offset_;
};

//==================================================================================================
#pragma pack(pop)
//==================================================================================================


template<class InT_>
inline bool
BitStream::Write(const InT_* input, size_t write_bits) {
    return WriteBits(input, write_bits, sizeof(*input));
}

template<class OutT_>
inline bool
BitStream::Read(OutT_* const output, size_t read_bits) const {
    return ReadBits(output, read_bits, sizeof(*output));
}

inline int
BitStream::GetNumberOfBytesUsed() const {
    size_t aligned = (((offset_)+((8)-1))&(~((8)-1)));
    aligned >>= 3;
    return static_cast<int>(aligned);
};

#endif //UTILITY_BITSTREAM_V2_H