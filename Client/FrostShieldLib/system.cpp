#include "system.h"
#include "converter.h"
#include <windows.h>
#include <shlwapi.h>
#include <io.h>
#include <fstream>

extern std::ofstream frostLogFile;
extern bool frostLogging;

namespace utils {

bool get_host_ip(const std::string& host, std::string& ip_str)
{
    if(frostLogging) {
        frostLogFile << "get_host_ip(" << host << ", " << ip_str << ")" << std::endl;
    }
    ip_str = "";
    unsigned ip = ::inet_addr(host.c_str());
    if(INADDR_NONE == ip) {
        hostent* phostent = ::gethostbyname(host.c_str());
        if(!phostent) {
            if(frostLogging) {
                frostLogFile << "get_host_ip() == false, SE = " << GetLastError() << std::endl;
            }
            return false;
        }
        ip = *((unsigned*)phostent->h_addr_list[0]);
        in_addr inaddr;
        inaddr.s_addr = ip;
        ip_str = ::inet_ntoa(inaddr);
    } else {
        ip_str = host;
    }
    if(frostLogging) {
        frostLogFile << "get_host_ip() = ok, host = " << ip_str << std::endl;
    }
    return true;
}

unsigned __int64 get_file_size(const std::wstring& p)
{
    struct _stati64 bufferint;
    if(!PathFileExistsW(p.c_str()) ||  _wstati64(p.c_str(), &bufferint)) {
        if(frostLogging) {
            frostLogFile << "get_file_size(" << utils::utf16_to_ascii(p.c_str()) << ") = null" << std::endl;
        }
        return 0;
    }
    if(frostLogging) {
        frostLogFile << "get_file_size(" << utils::utf16_to_ascii(p.c_str()) << ") = " << bufferint.st_size << std::endl;
    }
    return bufferint.st_size;
}

void delete_file(const std::wstring& f)
{
    if(!PathFileExistsW(f.c_str())) {
        if(frostLogging) {
            frostLogFile << "delete_file(" << utils::utf16_to_ascii(f.c_str()) << ") not found" << std::endl;
        }
        return;
    }
    SetFileAttributesW(f.c_str(),
                       FILE_ATTRIBUTE_NORMAL);
    DeleteFileW(f.c_str());
    if(frostLogging) {
        frostLogFile << "delete_file(" << utils::utf16_to_ascii(f.c_str())  << ")  = " << (PathFileExistsW(f.c_str()) ? "fail" : "ok" ) << std::endl;
    }
}

std::wstring get_parent_directory(std::wstring p)
{
    if(frostLogging) {
        frostLogFile << "get_parent_directory(" << utils::utf16_to_ascii(p.c_str()) << ") begin" << std::endl;
    }
    std::wstring::value_type sep[] = {L'/', L'\\', 0};
    std::wstring::size_type k = p.size();
    for(; 1 <= k && ((sep[0] == p[k - 1]) || (sep[1] == p[k - 1])); --k);
    if(!p.empty()) {
        p = p.substr(0, k);
        p = p.substr(0, p.find_last_of(sep) + 1);
    }
    if(frostLogging) {
        frostLogFile << "get_parent_directory() = " << utils::utf16_to_ascii(p.c_str()) << std::endl;
    }
    return p;
}

void create_directory(const std::wstring& d)
{
    if(frostLogging) {
        frostLogFile << "create_directory(" << utils::utf16_to_ascii(d.c_str()) << ") begin" << std::endl;
    }
    delete_file(d);
    _wmkdir(d.c_str());
    if(frostLogging) {
        frostLogFile << "create_directory() end" << std::endl;
    }
}

void create_parent_directories(std::wstring p)
{
    if(frostLogging) {
        frostLogFile << "create_parent_directories(" << utils::utf16_to_ascii(p.c_str()) << ") begin" << std::endl;
    }
    std::wstring::value_type sep[] = {L'/', L'\\', 0};
    std::wstring::size_type k = p.size();
    for(; 1 <= k && ((sep[0] == p[k - 1]) || (sep[1] == p[k - 1])); --k);
    if(!p.empty()) {
        p = p.substr(0, k);
        p = p.substr(0, p.find_last_of(sep) + 1);
    }
    if(!p.empty()) {
        for(std::wstring::size_type i = 0; i < p.size();){
            std::wstring::size_type j = p.find_first_of(sep, i);
            i = (std::wstring::npos != i) ? (j + 1) : j;
            std::wstring tmpS = p.substr(0, i);
            if(!tmpS.empty()){
                std::wstring::size_type x = tmpS.size();
                while((1 < x) && ((sep[0] == tmpS[x-1]) || (sep[1] == tmpS[x-1]))) --x;
                if(x < tmpS.size()) {
                    tmpS = tmpS.substr(0, x);
                }
                create_directory(tmpS);
            }
        }
    }
    if(frostLogging) {
        frostLogFile << "create_parent_directories() end" << std::endl;
    }
}

}
