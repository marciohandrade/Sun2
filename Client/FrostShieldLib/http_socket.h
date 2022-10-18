#ifndef HTTP_CLIENT_HTTP_SOCKET_H
#define HTTP_CLIENT_HTTP_SOCKET_H

#include <windows.h>
#include <vector>

namespace http_client {

class http_socket
{
public:
    http_socket(const std::wstring& host,
                unsigned connectRetryCount,
                unsigned connectRetryTimeout);

    ~http_socket();

    bool send(const std::string&, const std::string& range);

    int recv(unsigned __int64 size,
             const std::wstring&,
             unsigned __int64* actualSize = 0);

    bool connect();

    void abort();

    float get_current_speed() const;

    float get_current_progress() const;

    unsigned __int64 get_current_file_size() const;

    unsigned __int64 get_current_downloaded_size() const;

    std::string get_errors();

    unsigned __int64 get_conntent_size() const;

    int       m_error;

    void set_max_speed(float);

    float get_max_speed() const;

    static void set_total_max_speed(float);

    static float get_total_max_speed();

    static float get_current_total_speed();

private:
    void recv_header(std::vector<char>& fileBuff);
    void recv_file(unsigned __int64 size,
                   const std::wstring&,
                   const std::vector<char>& fileBuff);
    
    bool pars_html_errors(const std::string& header);
    void pars_header();

    static void calt_current_total_speed(unsigned);
    static void init_speed_mutex();

private:
    std::string                           m_host;
    unsigned short                        m_port;
    std::string                           m_ip;
    unsigned                              m_connectRetryCount;
    unsigned                              m_connectRetryTimeout;
    SOCKET                                m_socket;
    std::string                           m_lastMsg;
    unsigned                              m_lastStatusCode;
    char*                                 m_buff;
    unsigned                              m_bytesInBuff;
    unsigned __int64                    m_dataLen;
    unsigned __int64                    m_currentDataLen;
    std::string                           m_errStr;
    std::string                           m_query;
    std::string                           m_header;

    static const unsigned                 m_buffLen;// = 0xfff;
public:
    static unsigned                       m_connect_timeout; // milisecond
    static unsigned                       m_response_timeout; // milisecond
};

}

#endif //HTTP_CLIENT_HTTP_SOCKET_H
