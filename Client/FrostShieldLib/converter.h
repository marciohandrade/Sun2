#ifndef UTILITY_CONVERTER_H
#define UTILITY_CONVERTER_H

#include <string>
#include <vector>

namespace utils {

std::string int2str(__int64);

__int64 str2int(const std::string&, bool&);

__int64 str2int(const std::string&);

void utf16_to_ascii(const wchar_t*, std::string&);

std::string utf16_to_ascii(const wchar_t*);

void ascii_to_utf16(const char*, std::wstring&);

std::wstring ascii_to_utf16(const char*);

std::string trim(const std::string& s);

void to_words(const std::string&,
              std::vector<std::string>&);

std::string int2hexstr(unsigned __int64);


}

#endif // UTILITY_CONVERTER_H

