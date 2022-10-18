#ifndef INN_UPDATER_ERRORS_H
#define INN_UPDATER_ERRORS_H

#include <string>

enum inn_errors {
    INN_UPDATER_UNKNOWN_ERROR          = -1,
    INN_UPDATER_OK                     = 0,
    INN_UPDATER_IO_ERROR               = 6,
    INN_UPDATER_NULL_PTR               = 8,
    INN_UPDATER_FAILED_FILE_OPEN       = 9,
    INN_UPDATER_UNKNOWN_DOWNLOAD_TYPE  = 14,
    INN_UPDATER_RESOLVE_HOST_ERROR     = 15,
    INN_UPDATER_CREATE_SOCKET_ERROR    = 16,
    INN_UPDATER_CONNECT_ERROR          = 17,
    INN_UPDATER_SEND_ERROR             = 18,
    INN_UPDATER_RECV_ERROR             = 19,
    INN_UPDATER_HTTP_ERROR             = 20,
    INN_UPDATER_HTTP_SERVER_CLOSED     = 21,
    INN_UPDATER_RECV_TIMEOUTED_ERROR   = 22,
    INN_UPDATER_STD_ERROR              = 23,
    INN_UPDATER_SELECT_ERROR           = 25,
    INN_UPDATER_HTTP_ERROR_404         = 26
};

#endif //INN_UPDATER_ERRORS_H