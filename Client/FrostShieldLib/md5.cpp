#include "md5.h"
#include "converter.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

extern std::ofstream frostLogFile;
extern bool frostLogging;

namespace utils {

const char md5Hash::m_oneAndZeros = (char)0x80;

const char md5Hash::m_zeros = (char)0x0;

const unsigned md5Hash::m_bufLen = 8192;

const unsigned __int32 md5Hash::m_k[64] = {
    3614090360, 3905402710, 606105819,  3250441966,
    4118548399, 1200080426, 2821735955, 4249261313, 
    1770035416, 2336552879, 4294925233, 2304563134, 
    1804603682, 4254626195, 2792965006, 1236535329, 
    4129170786, 3225465664, 643717713,  3921069994, 
    3593408605, 38016083,   3634488961, 3889429448, 
    568446438,  3275163606, 4107603335, 1163531501, 
    2850285829, 4243563512, 1735328473, 2368359562, 
    4294588738, 2272392833, 1839030562, 4259657740, 
    2763975236, 1272893353, 4139469664, 3200236656, 
    681279174,  3936430074, 3572445317, 76029189,   
    3654602809, 3873151461, 530742520,  3299628645, 
    4096336452, 1126891415, 2878612391, 4237533241, 
    1700485571, 2399980690, 4293915773, 2240044497, 
    1873313359, 4264355552, 2734768916, 1309151649, 
    4149444226, 3174756917, 718787259,  3951481745
};
const unsigned __int32 md5Hash::m_r[64] = {
    7, 12, 17, 22, 7, 12, 17, 22,
    7, 12, 17, 22, 7, 12, 17, 22,
    5, 9,  14, 20, 5, 9,  14, 20,
    5, 9,  14, 20, 5, 9,  14, 20,
    4, 11, 16, 23, 4, 11, 16, 23,
    4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21,
    6, 10, 15, 21, 6, 10, 15, 21
};

md5Hash::md5Hash()
{
    m_buff = new char[m_bufLen];
}

md5Hash::~md5Hash()
{
    delete[] m_buff;
}

std::wstring md5Hash::get_random_crc()
{
    if(frostLogging) {
        frostLogFile << "md5Hash::get_random_crc()" << std::endl;
    }
    static bool b = false;
    if(!b) {
        b = true;
        srand((unsigned)time(0));
    }
    std::string crc = int2hexstr(rand());
    crc += int2hexstr(time(0));
    crc += int2hexstr(rand());
    while(crc.size() < 32) {
        crc += (char)((rand() % 26) + 'a');
    }
    if(frostLogging) {
        frostLogFile << "md5Hash::get_random_crc() = " << crc << std::endl;
    }
    return ascii_to_utf16(crc.c_str());
}

std::wstring md5Hash::getMD5FromFile(const wchar_t* fileName,
                                     OnMD5Progress_t clb,
                                     void* userData,
                                     double alpha,
                                     bool* running)
{
    if(frostLogging) {
        frostLogFile << "md5Hash::getMD5FromFile(" << utf16_to_ascii(fileName ? fileName : L"NULL") << ")" << std::endl;
    }
    m_h[0] = 0x67452301;
    m_h[1] = 0xEFCDAB89;
    m_h[2] = 0x98BADCFE;
    m_h[3] = 0x10325476;
    __int64 bitLen = 0;
    unsigned char res[16] = {0};
    char* part;//[0x40] = {0};
    __int64 i = 0;
    unsigned __int64 pIteration = 0, pIterationDelta = 10000000;
    FILE* myFile = _wfopen(fileName, L"rb");
    if(myFile) {
        size_t fromFile = 0;
        while (true) {
            if(running && !(*running)) break;
            if(!fromFile) {
                fromFile = fread(m_buff, 1, m_bufLen, myFile);
                part = m_buff;
            }
            size_t gcount = fromFile > 0x40 ? 0x40 : fromFile;
            pIteration += gcount;
            if(pIterationDelta <= pIteration) {
                if(clb) {
                    clb((unsigned)pIteration, userData, alpha);
                }
                pIteration = 0;
            }
            if (gcount == 64) {
                bitLen += gcount * 8;
                processChunk(part);
            } else {
                bitLen += gcount * 8;
                i = gcount;
                part[i] = m_oneAndZeros;
                ++i;
                if (i > 0x38) {
                    for (int j = (int)i; j < 0x40; ++j){
                        part[j] = m_zeros;
                    }
                    i = 0;
                    processChunk(part);
                }
                for (int j = (int)i; j < 0x40; ++j) {
                    if (j < 0x38) {
                        part[j] = m_zeros;
                    } else {
                        part[j] = (char)(bitLen >> (8 * (j - 0x38)));
                    }
                }
                processChunk(part);
                break;
            }
            part += gcount;
            fromFile -= gcount;
        }

        fclose(myFile);
        for (int i = 0; i < 16; i++) {
            res[i] = ((char*)&m_h[i / 4])[(i % 4)];
        }
        if(pIteration) {
            if(clb) {
                clb((unsigned)pIteration, userData, alpha);
            }
        }
        if(frostLogging) {
            frostLogFile << "md5Hash::getMD5FromFile() = ok" << std::endl;
        }
        return convToString(res);
    }
    if(frostLogging) {
        frostLogFile << "md5Hash::getMD5FromFile() = fail" << std::endl;
    }
    return L"";
}

void md5Hash::processChunk(char input[64])
{
    unsigned __int32 a = m_h[0];
    unsigned __int32 b = m_h[1];
    unsigned __int32 c = m_h[2];
    unsigned __int32 d = m_h[3];
    unsigned __int32 t = 0;
    unsigned __int32 f = 0;
    unsigned __int32 g = 0;
    unsigned __int32 w0 = 0;
    unsigned __int32 w1 = 0;
    unsigned __int32 w2 = 0;
    unsigned __int32 w3 = 0;
    unsigned __int32 w[16] = {0};

    for (int i = 0; i < 64; ++i) {
        if (i < 16) {
            w0 = (int)input[4 * i + 0];
            w1 = (int)input[4 * i + 1];
            w2 = (int)input[4 * i + 2];
            w3 = (int)input[4 * i + 3];
            w[i] = (w0 & 0x000000FF) | ((w1 << 8) & 0x0000FF00) | ((w2 << 16) & 0x00FF0000) | ((w3 << 24) & 0xFF000000);
            f = F(b, c, d);
            g = i;
        } else if (i < 32) {
            f = G(b, c, d);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            f = H(b, c, d);
            g = (3 * i + 5) % 16;
        } else {
            f = I(b, c, d);
            g = (7 * i) % 16;
        }
        t = d;
        d = c;
        c = b;
        b = b + ROTATE_LEFT((a + f + m_k[i] + w[g]) , m_r[i]);
        a = t;
    }
    m_h[0] += a;
    m_h[1] += b;
    m_h[2] += c;
    m_h[3] += d;
}

std::wstring md5Hash::convToString(unsigned char* bytes)
{
    std::ostringstream os;
    for(int i = 0; i < 16; i++) {
        os.width(2);
        os.fill('0');
        os << std::hex << (0xff & static_cast<unsigned int>(bytes[i]));
    }
    return ascii_to_utf16(os.str().c_str());
}

void md5Hash::convToString(unsigned char* bytes, char* md5Buff)
{
    std::stringstream os;
    for(int i = 0; i < 16; i++) {
        os.width(2);
        os.fill('0');
        os << std::hex << (0xff & static_cast<unsigned int>(bytes[i]));
    }
    os >> md5Buff;
}
}

