#ifndef INN_UPDATERDLL_MD5_H
#define INN_UPDATERDLL_MD5_H

#include <string>

namespace utils {

/*
 * ROTATE_LEFT rotates x left n bits. 
 * cast to unsigned int to guarantee support for 64Bit System
 */
#define ROTATE_LEFT(x, c) ((x << c) | (x >> (32-c)))

/* F, G, H and I are basic MD5 functions. */
#define F(x, y, z) ((x & y) | ((~x) & z))
#define G(x, y, z) ((z & x) | ((~z) & y))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | (~z)))

typedef void (*OnMD5Progress_t)(unsigned, void*, double);

class md5Hash
{   
public:
    md5Hash();
    ~md5Hash();
    std::wstring getMD5FromFile(const wchar_t* fileName,
                                OnMD5Progress_t clb = 0,
                                void* userData = 0,
                                double alpha = 0,
                                bool* running = 0);
    static std::wstring get_random_crc();
private:
    void processChunk(char[64]);
    void convToString(unsigned char* bytes, char* md5Buff/*32 byte*/);
    std::wstring convToString(unsigned char* bytes);

private:
    unsigned __int32 m_h[4];
    char* m_buff;
    static const unsigned m_bufLen;

private:
    static const unsigned __int32 m_k[64];
    static const unsigned __int32 m_r[64];
    static const char m_oneAndZeros;
    static const char m_zeros;
};

}

#endif //INN_UPDATERDLL_MD5_H

