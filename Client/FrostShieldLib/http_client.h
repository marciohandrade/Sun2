#ifndef HTTP_CLIENT_CLIENT_H
#define HTTP_CLIENT_CLIENT_H

#include "http_socket.h"
#include <string>

namespace http_client {

class client {
public:
    static std::wstring get_server(const std::wstring& url);

    static std::wstring get_path(const std::wstring&);

public:
    client(const std::wstring& ipport);

    ~client();

    int download(const std::wstring& src,
                 const std::wstring& dest,
                 const std::wstring& crc,
                 unsigned __int64 size,
                 unsigned __int64* actualSize,
                 std::string& errors,
                 bool firstTime = true);

private:
    client(const client &); 
    client& operator= (const client &); 
private:
    http_socket m_socket;
    int         m_error;
    std::wstring m_currentFile;
public:
    static const std::wstring m_downloadInfoExt;
};

}

#endif //HTTP_CLIENT_CLIENT_H
