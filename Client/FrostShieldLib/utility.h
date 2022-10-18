#ifndef GAME_SHIELD_LIB_UTILITY_H
#define GAME_SHIELD_LIB_UTILITY_H

#include <io.h>
#include <direct.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <Wininet.h>
#include <shlwapi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

struct update_by_crc_param
{
    update_by_crc_param(const char* rootURL,
                        const char* destDir):
        m_rootURL(new char[(rootURL ? strlen(rootURL) : 0) + 1]),
        m_destDir(new char[(destDir ? strlen(destDir) : 0) + 1])
    {
        memcpy(m_rootURL, rootURL, (rootURL ? strlen(rootURL) : 0));
        m_rootURL[rootURL ? strlen(rootURL) : 0] = 0;
        memcpy(m_destDir, destDir, (destDir ? strlen(destDir) : 0));
        m_destDir[destDir ? strlen(destDir) : 0] = 0;
    }

    ~update_by_crc_param()
    {
        delete[] m_rootURL;
        delete[] m_destDir;
    }

    char*  m_rootURL;
    char*  m_destDir;
};

struct utility
{
    static bool download_url_to_directory(const std::string& url,
                                          const std::string& dst_file);

    static bool update_by_crc(update_by_crc_param* up);

    static void run_command(std::string cmd);

    enum {
        buff_len = 4096
    };
};

#endif //GAME_SHIELD_LIB_UTILITY_H

