#ifndef _SERVER_GAMESERVER_VENDORSEARCHSYSTEM_VENDORSEARCHPACKETSTRUCT_H
#define _SERVER_GAMESERVER_VENDORSEARCHSYSTEM_VENDORSEARCHPACKETSTRUCT_H
#include "StdAfx.h"

typedef struct _VENDOR_SEARCH_BUFFER
{
    BYTE message_buffer[1024];
} VENDOR_SEARCH_BUFFER;

enum eVendorSearchPacket
{
    eVendorSearchInsert = 1,
    eVendorSearchDelete,
    eVendorSearchSelect,
    eVendorSearchSelectAck,
};

struct VENDOR_SEARCH_SYN
{
    BYTE packet_type_;
};

struct VENDOR_SEARCH_ACK
{
    BYTE packet_type_;
};

typedef struct {
    DWORD guid;
    //CHAR szName[(MAX_ITEMNAME_LENGTH)*2+1];
    POSTYPE pos;
} SEARCH_RESULT_ROW;

struct SEARCH_RESULT 
{
    int row_count;
    SEARCH_RESULT_ROW result_row[VENDOR_ITEMS_PER_PAGE];
    int current_page;
    int total_page;

    SEARCH_RESULT()
    {
        row_count = 0;
        memset(&result_row[0], 0, sizeof(result_row));
        current_page = 0, total_page = 0;
    }
};

//////////////////////////////////////////////////////////////////////////
// VENDOR_SEARCH_SYN
struct VENDOR_SEARCH_INSERT : VENDOR_SEARCH_SYN
{
    DWORD guid;
    CHAR item_name[(MAX_ITEMNAME_LENGTH)*2+1];
    POSTYPE pos;
    MONEY price;

    VENDOR_SEARCH_INSERT(DWORD _guid, const CHAR* _item_name, POSTYPE _pos, MONEY _price)
    {
        packet_type_ = eVendorSearchInsert;
        guid = _guid;
        memset(item_name, 0, sizeof(item_name));
        memcpy(item_name, _item_name, strlen(_item_name));
        pos = _pos, price = _price;
    };
};

struct VENDOR_SEARCH_DELETE : VENDOR_SEARCH_SYN
{
    DWORD guid;
    POSTYPE pos;

    VENDOR_SEARCH_DELETE(DWORD _guid, POSTYPE _pos)
    {
        packet_type_ = eVendorSearchDelete;
        guid = _guid, pos = _pos;
    };
};

struct VENDOR_SEARCH_SELECT : VENDOR_SEARCH_SYN
{
    DWORD guid;
    CHAR item_name[(MAX_ITEMNAME_LENGTH)*2+1];
    int page;

    VENDOR_SEARCH_SELECT(DWORD _guid, const CHAR* _search_name, int _page)
    {
        packet_type_ = eVendorSearchSelect;
        guid = _guid;
        memset(item_name, 0, sizeof(item_name));
        memcpy(item_name, _search_name, strlen(_search_name));
        page = _page;
    };
};

//////////////////////////////////////////////////////////////////////////
// VENDOR_SEARCH_ACK
struct VENDOR_SEARCH_SELECT_ACK : VENDOR_SEARCH_ACK
{
    DWORD guid;
    SEARCH_RESULT result;

    VENDOR_SEARCH_SELECT_ACK(DWORD _guid, SEARCH_RESULT& _result)
    {
        packet_type_ = eVendorSearchSelectAck;
        guid = _guid;
        memset(&result, 0, sizeof(result));
        memcpy(&result, &_result, sizeof(SEARCH_RESULT));
    };
};
#endif //_SERVER_GAMESERVER_VENDORSEARCHSYSTEM_VENDORSEARCHPACKETSTRUCT_H