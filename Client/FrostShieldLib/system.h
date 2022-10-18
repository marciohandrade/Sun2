#ifndef UTILITY_SYSTEM_H
#define UTILITY_SYSTEM_H

#include <string>

namespace utils {

unsigned __int64 get_file_size(const std::wstring&);

void delete_file(const std::wstring&);

std::wstring get_parent_directory(std::wstring);

void create_directory(const std::wstring&);

void create_parent_directories(const std::wstring);

bool get_host_ip(const std::string&, std::string&);

}

#endif //UTILITY_SYSTEM_H
