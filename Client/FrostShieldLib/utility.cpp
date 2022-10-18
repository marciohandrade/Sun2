#include "http_client.h"
#include "converter.h"
#include "system.h"
#include "utility.h"
#include "metafile.h"
#include "md5.h"
#include <shlwapi.h>
#include <direct.h>
#include <errno.h>
#include <string>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "shlwapi.lib")

extern std::ofstream frostLogFile;
extern bool frostLogging;

bool utility::
download_url_to_directory(const std::string& url,
                          const std::string& dst_file)
{
    if(frostLogging) {
        frostLogFile << "download_url_to_directory(" << url << ", " << dst_file << ")" << std::endl;
    }
    std::wstring wurl;
    utils::ascii_to_utf16(url.c_str(), wurl);
    std::wstring wdst_file;
    utils::ascii_to_utf16(dst_file.c_str(), wdst_file);
    http_client::client c(http_client::client::get_server(wurl));
    unsigned __int64 asize = 0;
    std::string errors;
    DeleteFileW(wdst_file.c_str());
    int err = c.download(http_client::client::get_path(wurl), 
                         wdst_file,
                         L"", 
                         0, 
                         &asize, 
                         errors);
    if(frostLogging) {
        frostLogFile << "download_url_to_directory = " << err << ", SE = " << GetLastError() << std::endl;
    }
    if(0 == err) {
        return true;
    }
    return false;
}

static bool update_file(metafile& mtf,
                        update_by_crc_param* up,
                        const file_descriptor& f)
{
    if(frostLogging) {
        frostLogFile << "update_file() begin" << std::endl;
    }
    char crc32[33] = {0};
    std::string fn = std::string(up->m_destDir) + "\\" + f.m_path;
    if(PathFileExistsA(fn.c_str())) {
        std::wstring ncrc = utils::md5Hash().getMD5FromFile(utils::ascii_to_utf16(fn.c_str()).c_str());
        if(0 == _wcsicmp(ncrc.c_str(), utils::ascii_to_utf16(f.m_crc.c_str()).c_str())) {
            if(frostLogging) {
                frostLogFile << "update_file() end1" << std::endl;
            }
            return true;
        }
    }

    if(!utility::download_url_to_directory(
        std::string(up->m_rootURL) + "/" + f.m_path,
        fn)) {
        if(frostLogging) {
            frostLogFile << "update_file() end2" << std::endl;
        }
        return false;
    }
    if(f.m_size != utils::get_file_size(utils::ascii_to_utf16(fn.c_str()))) {
        DeleteFileA(fn.c_str());
        if(frostLogging) {
            frostLogFile << "update_file() end3" << std::endl;
        }
        return false;
    }
    if(frostLogging) {
        frostLogFile << "update_file() end4" << std::endl;
    }
    return true;
}

static bool download_files(metafile& mtf,
                           update_by_crc_param* up)
{
    if(frostLogging) {
        frostLogFile << "download_files() begin" << std::endl;
    }
    std::set<file_descriptor>::const_iterator
        i = mtf.m_files.begin(), e = mtf.m_files.end();
    for(; i != e; ++i) {
        if(!update_file(mtf, up, *i)) {
            if(frostLogging) {
                frostLogFile << "download_files() end1" << std::endl;
            }
            return false;
        }
    }
    if(frostLogging) {
        frostLogFile << "download_files() end2" << std::endl;
    }
    return true;
}

static void remove_local_crc(const std::string& dir)
{
    if(PathFileExistsA((dir + "\\update.crc").c_str())) {
        SetFileAttributesA((dir + "\\update.crc").c_str(), FILE_ATTRIBUTE_NORMAL);
        DeleteFileA((dir + "\\update.crc").c_str());
    }
}

bool utility::update_by_crc(update_by_crc_param* up)
{
    if(frostLogging) {
        frostLogFile << "update_by_crc() begin" << std::endl;
    }
    if(!up) {
        if(frostLogging) {
            frostLogFile << "update_by_crc() end1" << std::endl;
        }
        return false;
    }
    metafile mtf;
    if(!utility::download_url_to_directory(std::string(up->m_rootURL) + "/update.crc",
        std::string(up->m_destDir) + "\\update.crc")){
        remove_local_crc(up->m_destDir);
        if(frostLogging) {
            frostLogFile << "update_by_crc() end2" << std::endl;
        }
        return false;
    }
    if(!mtf.load(std::string(up->m_destDir) + "\\update.crc")){
        remove_local_crc(up->m_destDir);
        if(frostLogging) {
            frostLogFile << "update_by_crc() end3" << std::endl;
        }
        return false;
    }
    remove_local_crc(up->m_destDir);
    bool r = download_files(mtf, up);
    if(frostLogging) {
        frostLogFile << "update_by_crc() end4" << std::endl;
    }
    return r;
}

