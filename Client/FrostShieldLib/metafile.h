#ifndef GAME_SHIELD_LIB_METAFILE_H
#define GAME_SHIELD_LIB_METAFILE_H

#include "utility.h"
#include <set>

#define max_one_line_len 1100

struct file_descriptor
{
    file_descriptor(const std::string& = "");
    bool operator< (const file_descriptor& fd) const;
    bool pars_line(const char* buff, unsigned len);

    std::string m_path;
    std::string m_crc;
    unsigned m_size;
};

class metafile
{
public:
    metafile();
    bool load(const std::string& fname);

    std::string m_crcPath;
    std::set<file_descriptor> m_files;
    unsigned __int64 m_totalSize;
};

#endif //GAME_SHIELD_LIB_METAFILE_H

