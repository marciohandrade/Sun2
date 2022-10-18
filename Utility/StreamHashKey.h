#pragma once
#ifndef UTILITY_STREAM_HASH_KEY_H
#define UTILITY_STREAM_HASH_KEY_H

namespace util {
;
//==================================================================================================

typedef ulong HashKey;

//==================================================================================================

class StreamHashKey
{
public:
    static HashKey GetStringHashKey(const char* input_str);
    static HashKey GetStringHashKey(const wchar_t* input_wstr);
    static HashKey GetMemoryHashKey(const void* input_ptr, size_t length);
    //static HashKey GetMemoryHashKeyEnDecode(const void* input_ptr, size_t length, HashKey endecode);
private:
    static const HashKey crc_table_static_[256];
    __DISABLE_COPY(StreamHashKey);
};

}; //end of namespace

#endif //UTILITY_STREAM_HASH_KEY_H