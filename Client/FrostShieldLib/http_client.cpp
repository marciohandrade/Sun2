#include "http_client.h"
#include "system.h"
#include "md5.h"
#include "converter.h"
#include <fstream>
#include <algorithm>

extern std::ofstream frostLogFile;
extern bool frostLogging;

namespace http_client {

std::wstring client::get_server(const std::wstring& url)
{
    if(frostLogging) {
        frostLogFile << "get_server(" << utils::utf16_to_ascii(url.c_str()) << ")" << std::endl;
    }
    if((url.size() > 7) && 
       (0 == _wcsicmp(url.substr(0, 7).c_str(), L"http://"))) {
        size_t i = 7;
        while(((i+1) < url.size()) && (L'/' != url[i])) ++i;
        if(frostLogging) {
            frostLogFile << "get_server() ok" << std::endl;
        }
        return url.substr(7, i - 7);
    }
    if(frostLogging) {
        frostLogFile << "get_server() fail" << std::endl;
    }
    return L"";
}

std::wstring client::get_path(const std::wstring& url)
{
    if(frostLogging) {
        frostLogFile << "get_path(" << utils::utf16_to_ascii(url.c_str()) << ")" << std::endl;
    }
    if((url.size() > 7) && 
       (0 == _wcsicmp(url.substr(0, 7).c_str(), L"http://"))) {
        size_t i = 7;
        while(((i+1) < url.size()) && (L'/' != url[i])) ++i;
        while((i < url.size()) && (L'/' == url[i])) ++i;
        if(frostLogging) {
            frostLogFile << "get_path() ok" << std::endl;
        }
        return url.substr(i);
    }
    if(frostLogging) {
        frostLogFile << "get_path() fail" << std::endl;
    }
    return L"";
}

struct download_info
{
    download_info(const std::wstring& src,
                  const std::wstring& dest,
                  const std::wstring& crc,
                  unsigned __int64 size):
        m_src(src),
        m_dest(L""),
        m_crc(crc),
        m_size(size)
    {
        if(frostLogging) {
            frostLogFile << "download_info::download_info(" 
                         << utils::utf16_to_ascii(src.c_str()) << ", " 
                         << utils::utf16_to_ascii(dest.c_str()) << ", "
                         << utils::utf16_to_ascii(crc.c_str()) << ", "
                         << size << ")" << std::endl;
        }
        bool isSep = false;
        for(size_t i=0; i<dest.size(); ++i) {
            wchar_t w = dest[i];
            if(w == L'\\' || w == L'/') {
                if(!isSep) {
                    m_dest += L'\\';
                    isSep = true;
                }
            } else {
                m_dest += w;
                isSep = false;
            }
        }
        if(frostLogging) {
            frostLogFile << "download_info::download_info() end" << std::endl;
        }
    }

    std::wstring get_info_file() const
    {
        if(frostLogging) {
            frostLogFile << "download_info::get_info_file()" << std::endl;
        }
        return m_dest + client::m_downloadInfoExt;
    }

    bool load(std::wstring& crc,
              unsigned __int64& size) const
    {
        if(frostLogging) {
            frostLogFile << "download_info::load()" << std::endl;
        }
        FILE* in = _wfopen((m_dest + client::m_downloadInfoExt).c_str(), L"rb");
        if(!in) {
            if(frostLogging) {
                frostLogFile << "download_info::load() end1" << std::endl;
            }
            return false;
        }
        char header[29] = {0};
        fread(header, 1, 28, in);
        if(m_infoHeader != std::string(header)) {
            fclose(in);
            if(frostLogging) {
                frostLogFile << "download_info::load() end2" << std::endl;
            }
            return false;
        }
        size_t len = 0;
        len = fread(&size, 1, sizeof(size), in);
        if(len != sizeof(m_size)) {
            fclose(in);
            if(frostLogging) {
                frostLogFile << "download_info::load() end3" << std::endl;
            }
            return false;
        }
        wchar_t crcBuff[33] = {0};
        fread(crcBuff, sizeof(wchar_t), 32, in);
        fclose(in);
        crc.assign(crcBuff);
        if(frostLogging) {
            frostLogFile << "download_info::load() end4" << std::endl;
        }
        return true;
    }

    bool save() const
    {
        if(frostLogging) {
            frostLogFile << "download_info::save()" << std::endl;
        }
        const std::wstring destFile = m_dest + client::m_downloadInfoExt;
        utils::create_parent_directories(destFile);
        SetFileAttributesW(destFile.c_str(), FILE_ATTRIBUTE_NORMAL);
        FILE* out = _wfopen(destFile.c_str(), L"wb");
        if(!out) {
            if(frostLogging) {
                frostLogFile << "download_info::save() end1" << std::endl;
            }
            return false;
        }
        char header[29] = {0};
        memcpy(header, m_infoHeader.c_str(), m_infoHeader.size());
        fwrite(header, 1, 28, out);
        fwrite(&m_size, 1, sizeof(m_size), out);
        std::wstring crc = m_crc;
        if(crc.find(L"?hash=") == 0) {
            crc = crc.substr(6);
        }
        fwrite(crc.c_str(), sizeof(wchar_t), crc.size(), out);
        fclose(out);
        SetFileAttributesW(destFile.c_str(), FILE_ATTRIBUTE_HIDDEN);
        if(frostLogging) {
            frostLogFile << "download_info::save() end2" << std::endl;
        }
        return true;
    }

    unsigned __int64 get_downloaded_size() const
    {
        if(frostLogging) {
            frostLogFile << "download_info::get_downloaded_size()" << std::endl;
        }
        std::wstring crc;
        unsigned __int64 size = 0;
        if(load(crc, size) &&
           (m_size == size) &&
           (m_crc.find(crc) != std::string::npos)) {
            size = utils::get_file_size(m_dest);
            if(frostLogging) {
                frostLogFile << "download_info::get_downloaded_size() = " << size << std::endl;
            }
            return size;
        }
        if(frostLogging) {
            frostLogFile << "download_info::get_downloaded_size() = " << 0 << std::endl;
        }
        return 0;
    }

    std::string get_url() const
    {
        if(frostLogging) {
            frostLogFile << "download_info::get_url()" << std::endl;
        }
        std::string url, crc;
        url = utils::utf16_to_ascii(m_src.c_str());
        std::replace(url.begin(), url.end(), '\\', '/');
        std::string::size_type i = url.find("//");
        while(i != std::string::npos) {
            url = url.substr(0, i) + url.substr(i+1);
            i = url.find("//");
        }
        if(!m_crc.empty()) {
            utils::utf16_to_ascii(m_crc.c_str(), crc);
        } else {
            utils::utf16_to_ascii(utils::md5Hash::get_random_crc().c_str(), crc);
            crc = "?hash=" + crc;
        }
        url += crc;
        if(frostLogging) {
            frostLogFile << "download_info::get_url() = " << url << std::endl;
        }
        return url;
    }

    std::wstring m_src;
    std::wstring m_dest;
    std::wstring m_crc;
    unsigned __int64       m_size;
    static const std::string  m_infoHeader;
};

const std::wstring client::m_downloadInfoExt = L".downloading";
const std::string download_info::m_infoHeader = "innDownloaderInfo";

client::client(const std::wstring& ipport):
    m_socket(ipport, 1, 1000),
    m_error(0),
    m_currentFile()
{
    if(frostLogging) {
        frostLogFile << "client::client()" << std::endl;
    }
}

client::~client()
{
    if(frostLogging) {
        frostLogFile << "client::~client()" << std::endl;
    }
}

int client::download(const std::wstring& src,
                     const std::wstring& dest,
                     const std::wstring& crc,
                     unsigned __int64 size,
                     unsigned __int64* actualSize,
                     std::string& errors,
                     bool firstTime)
{
    if(frostLogging) {
        frostLogFile << "client::download(" 
            << utils::utf16_to_ascii(src.c_str()) << ", "
            << utils::utf16_to_ascii(dest.c_str()) << ", "
            << utils::utf16_to_ascii(crc.c_str()) << ", "
            << size << ", "
            << (actualSize ? "actualSize" : "null") << ", "
            << errors << ", "
            << firstTime << ") " << std::endl;
    }
    utils::create_parent_directories(dest);
    m_currentFile = dest;
    download_info di(src, dest, crc, size);
    unsigned __int64 oldSize = di.get_downloaded_size();
    if(size && (oldSize == size)) {
        return 0;
    } else if(oldSize > size) {
        oldSize = 0;
    }
    if(0 == oldSize) {
        utils::delete_file(dest);
    }
    if(oldSize && firstTime) {
        unsigned __int64 allSize = 0;
        while(allSize < oldSize) {
            unsigned delta = (oldSize < (allSize + 10000000)) ? (oldSize - allSize) : 10000000;
            allSize += delta;
        }
    }
    unsigned triesCount = 0;
    m_error = 0;
    if(actualSize) *actualSize = 0;
    if(!m_socket.connect()) {
        while (true) {
            std::string range;
            if(size) {
                if(oldSize == size) {
                    break;
                }
                range += "Range: bytes=";
                range += utils::int2str(oldSize);
                range += "-";
                range += utils::int2str(size);
                range += "\r\n";
            }
            m_error = m_socket.send(di.get_url(), range);
            unsigned __int64 newSize = 0;
            if(!m_error) {
                di.save();
                m_error = m_socket.recv(size - oldSize, dest, actualSize);
                if(!m_error) {
                    newSize = di.get_downloaded_size();
                }
            }
            if(!errors.empty()) {
                errors += "; ";
            }
            errors += m_socket.get_errors();
            if(((newSize < size) || m_error) && (triesCount < 10)) {
                ++triesCount;
                oldSize = di.get_downloaded_size();
                if(!size) {
                    size = m_socket.get_conntent_size();
                }
                Sleep(500);
            } else {
                utils::delete_file(di.get_info_file());
                break;
            }
        }
    } else {
        m_error = m_socket.m_error;
        errors += m_socket.get_errors();
    }
    m_currentFile = L"";
    if(frostLogging) {
        frostLogFile << "client::download() = " << m_error << std::endl; 
    }
    return m_error;
}

}
