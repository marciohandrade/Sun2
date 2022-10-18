#include "converter.h"
#include <string.h>

extern std::ofstream frostLogFile;
extern bool frostLogging;

namespace utils {

void int2str(__int64 n, std::string& str)
{
    str = "";
    if(!n) {
        str = "0";
        return;
    }
    bool b = n < 0;
    if(b) {
        n *= -1;
    }
    while(n) {
        str = char('0' + (n % 10)) + str;
        n /= 10;
    }
    if(b) {
        str = char('-') + str;
    }
}

std::string int2str(__int64 u)
{
    std::string s;
    int2str(u, s);
    return s;
}

void trim(std::string& s)
{
    std::string::size_type i = 0, j = 0;
    while((i < s.size()) && (isspace((unsigned char)s[i]) || !s[i] || ('\n'==s[i]))) ++i;
    j = s.size();
    while((0 < j) && (isspace((unsigned char)s[j-1]) || !s[j-1] || ('\n'==s[j-1]))) --j;
    if((j-i) != s.size()) {
        s = (i == j ? "": s.substr(i, j - i));
    }
}

void trim(std::wstring& s)
{
    std::string::size_type i = 0, j = 0;
    while((i < s.size()) && (isspace((unsigned short)s[i]) || !s[i] || (L'\n'==s[i]))) ++i;
    j = s.size();
    while((0 < j) && (isspace((unsigned short)s[j-1]) || !s[j-1] || (L'\n'==s[j-1]))) --j;
    if((j-i) != s.size()) {
        s = (i == j ? L"": s.substr(i, j - i));
    }
}

std::string trim(const std::string& s)
{
    std::string t(s);
    trim(t);
    return t;
}

__int64 str2int(const std::string& n_str, bool& err)
{
    std::string str(n_str);
    trim(str);
    __int64 n = 0;
    err = false;
    if(str.empty() || str.size() > 21) {
        err = true;
        return n;
    }
    size_t i=0;
    if('-' == str[0]) {
        ++i;
    }
    for(; i<str.size(); ++i) {
        if(!isdigit((unsigned char)str[i])) {
            err = true;
            return n;
        }
        n *= 10;
        n += (str[i] - '0');
    }
    if('-' == str[0]) {
        n *= -1;
    }
    return n;
}

__int64 str2int(const std::string& str)
{
    bool err = false;
    __int64 n = str2int(str, err);
    if (err) {
        throw err;
    }
    return n;
}

inline wchar_t ascii_to_utf16(unsigned char c)
{
    if (0xbf < c) {
        return (wchar_t)(0x350 + c);
    } else if(0xa8 == c) {
        return (wchar_t)0x401;
    } else if (0xb8 == c) {
        return (wchar_t)0x451;
    }
    return (unsigned short)c;
}

inline unsigned char utf16_to_ascii(wchar_t w)
{
    if (0x47f == w) {
        w = 0x448;
    }
    if ((w < 0x450) && (0x40f < w)) {
        return (unsigned char)(w - 0x350);
    } else if(0x401 == w) {
        return (unsigned char)0xa8;
    } else if (0x451 == w) {
        return (unsigned char)0xb8;
    }
    return (unsigned char)w;
}

void ascii_to_utf16(const char* str, std::wstring& w)
{
    w = L"";
    size_t len = str ? strlen((const char*)str) : 0;
    w.reserve(len);
    for(size_t i=0; i<len; ++i) {
        w += ascii_to_utf16((unsigned char)str[i]);
    }
}

std::wstring ascii_to_utf16(const char* str)
{
    std::wstring wstr;
    ascii_to_utf16(str, wstr);
    return wstr;
}

void utf16_to_ascii(const wchar_t* wstr, std::string& str)
{
    str = "";
    size_t len = 0;
    if(wstr) {
        const wchar_t* u = wstr;
        while(*u) {
            ++len;
            ++u;
        }
    }
    str.reserve(len);
    for(size_t i=0; i<len; ++i) {
        str += (char)utf16_to_ascii(wstr[i]);
    }
}

std::string utf16_to_ascii(const wchar_t* wstr)
{
    std::string str;
    utf16_to_ascii(wstr, str);
    return str;
}

std::string int2hexstr(unsigned __int64 u)
{
    std::string s;
    unsigned char x[16] = 
                  {(unsigned char)(0xf & (u >> 60)),
                   (unsigned char)(0xf & (u >> 56)),
                   (unsigned char)(0xf & (u >> 52)),
                   (unsigned char)(0xf & (u >> 48)),
                   (unsigned char)(0xf & (u >> 44)),
                   (unsigned char)(0xf & (u >> 40)), 
                   (unsigned char)(0xf & (u >> 36)),
                   (unsigned char)(0xf & (u >> 32)),
                   (unsigned char)(0xf & (u >> 28)),
                   (unsigned char)(0xf & (u >> 24)),
                   (unsigned char)(0xf & (u >> 20)), 
                   (unsigned char)(0xf & (u >> 16)),
                   (unsigned char)(0xf & (u >> 12)), 
                   (unsigned char)(0xf & (u >> 8)),
                   (unsigned char)(0xf & (u >> 4)),  
                   (unsigned char)(0xf & u)};
    size_t i = 0;
    while(i < 15 && !x[i]) ++i;
    for(; i<16; ++i) {
        switch(x[i]) {
            case 0x0:
                s += '0';
            break;
            case 0x1:
                s += '1';
            break;
            case 0x2:
                s += '2';
            break;
            case 0x3:
                s += '3';
            break;
            case 0x4:
                s += '4';
            break;
            case 0x5:
                s += '5';
            break;
            case 0x6:
                s += '6';
            break;
            case 0x7:
                s += '7';
            break;
            case 0x8:
                s += '8';
            break;
            case 0x9:
                s += '9';
            break;
            case 0xa:
                s += 'a';
            break;
            case 0xb:
                s += 'b';
            break;
            case 0xc:
                s += 'c';
            break;
            case 0xd:
                s += 'c';
            break;
            case 0xe:
                s += 'e';
            break;
            case 0xf:
                s += 'f';
            break;
        }
    }
    return s;
}

void to_words(const std::string& s,
              std::vector<std::string>& v)
{
    v.clear();
    std::string::size_type i = 0, j = 0;
    while(i < s.size()) {
        while(i < s.size() && isspace(s[i])) ++i;
        if(i < s.size()) {
            j = i + 1;
            while(j < s.size() && !isspace(s[j])) ++j;
            v.push_back(s.substr(i, j-i));
            i = j;
        }
    }
}

}

