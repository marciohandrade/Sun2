#include "stdafx.h"
#include "VendorSearchSystem.h"
#include "SQLite/sqlite3.h"

BOOST_STATIC_ASSERT(sizeof(VENDOR_SEARCH_SELECT_ACK) < sizeof(VENDOR_SEARCH_BUFFER));

VendorSearchSystem::VendorSearchSystem()
{
    destroy_thread_ = false;
}

VendorSearchSystem::~VendorSearchSystem()
{

}

UINT WINAPI VendorSearchSystem::SearchThread(PVOID pvParam)
{
    if (!pvParam)
        return FALSE;

    VendorSearchSystem* this_ = (VendorSearchSystem*)pvParam;
    while(this_->destroy_thread_ == false)
    {
        static VENDOR_SEARCH_BUFFER recv_buffer;

        if (this_->RecvFromVendorManager(&recv_buffer) == false) 
        {
            Sleep(100);
            continue;
        }

        VENDOR_SEARCH_SYN* recv_packet = reinterpret_cast<VENDOR_SEARCH_SYN*>(&recv_buffer);

        switch(recv_packet->packet_type_)
        {
        case eVendorSearchInsert:
            {
                VENDOR_SEARCH_INSERT* packet = static_cast<VENDOR_SEARCH_INSERT*>(recv_packet);
                this_->insert(packet->guid, packet->item_name, packet->pos, packet->price);
            } break;
        case eVendorSearchDelete:
            {
                VENDOR_SEARCH_DELETE* packet = static_cast<VENDOR_SEARCH_DELETE*>(recv_packet);
                this_->erase(packet->guid, packet->pos);
            } break;
        case eVendorSearchSelect:
            {
                VENDOR_SEARCH_SELECT* packet = static_cast<VENDOR_SEARCH_SELECT*>(recv_packet);

                if (strlen(packet->item_name) < 4 ||
                    strncmp(packet->item_name, "SUN", 3) == 0) 
                {
                    // show into page.
                    int page_type = 0; /* 0 : random select */
                    this_->select_intro_page(packet->guid, page_type);
                }
                else {
                    this_->select(packet->guid, packet->item_name, packet->page);
                }
            } break;
        }
    }

    return 0;
}

bool VendorSearchSystem::initialize()
{
    int rc;

    /* create database in memory */
    rc = sqlite3_open16(L":memory:", &db);
    /* create database in file for testing with sqlite3.exe console */
    //rc = sqlite3_open16(L"vendor_search.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    /*;{
        wchar_t* query_1 = L"drop table if exists vendor_items;";
        sqlite3_exec16(db, query_1, NULL, NULL, NULL);
    };*/
    wchar_t* query = \
        L"create table if not exists vendor_items"
        L"(player_guid INTEGER, item_name TEXT(512), pos SMALLINT, price UNSIGNED BIG INT);";
    if (execute(query) != SQLITE_OK)
        return false;

    /*;{
        wchar_t* query_2 = L"drop table if exists search_result;";
        sqlite3_exec16(db, query_2, NULL, NULL, NULL);
    };*/
    wchar_t* query_result_table = \
        L"create table if not exists search_result"
        L"(player_guid INTEGER, item_name TEXT(512), pos SMALLINT, price UNSIGNED BIG INT);";
    if (execute(query_result_table) != SQLITE_OK)
        return false;

    //////////////////////////////////////////////////////////////////////////
    //
    message_buffer_.Initialize(100);

    search_thread_ = (HANDLE)_beginthreadex( \
        NULL, 0, VendorSearchSystem::SearchThread, this, 0, &search_thread_id_);

    return true;
}

void VendorSearchSystem::close()
{
    sqlite3_close(db);

    destroy_thread_ = true;
    WaitForSingleObject(search_thread_, INFINITE);
}

void VendorSearchSystem::SendToVendorSearchSystem(PVOID packet, size_t size)
{
    if(size >= sizeof(VENDOR_SEARCH_BUFFER))
        return;

    VENDOR_SEARCH_BUFFER* buffer = (VENDOR_SEARCH_BUFFER*)message_buffer_.Alloc();
    memset(buffer, 0, sizeof(VENDOR_SEARCH_BUFFER));
    memcpy(buffer, packet, size);

    ;{
        AutoLock lock(recv_lock_);
        recv_queue_.push_back(buffer);
    };
}

bool VendorSearchSystem::RecvFromVendorManager(PVOID buffer)
{
    if (recv_queue_.empty())
        return false;

    ;{
        AutoLock lock(recv_lock_);
        VENDOR_SEARCH_BUFFER* packet = recv_queue_.front();
        memcpy(buffer, packet, sizeof(VENDOR_SEARCH_BUFFER));
        recv_queue_.pop_front();
        message_buffer_.Free(packet);
    };

    return true;
}

void VendorSearchSystem::SendToVendorManager(PVOID packet, size_t size)
{
    if(size >= sizeof(VENDOR_SEARCH_BUFFER))
        return;

    VENDOR_SEARCH_BUFFER* buffer = (VENDOR_SEARCH_BUFFER*)message_buffer_.Alloc();
    memset(buffer, 0, sizeof(VENDOR_SEARCH_BUFFER));
    memcpy(buffer, packet, size);

    ;{
        AutoLock lock(send_lock_);
        send_queue_.push_back(buffer);
    };
}

bool VendorSearchSystem::RecvFromVendorSearchSystem(PVOID buffer)
{
    if (send_queue_.empty())
        return false;

    ;{
        AutoLock lock(send_lock_);
        VENDOR_SEARCH_BUFFER* packet = send_queue_.front();
        memcpy(buffer, packet, sizeof(VENDOR_SEARCH_BUFFER));
        send_queue_.pop_front();
        message_buffer_.Free(packet);
    };

    return true;
}

void VendorSearchSystem::insert( DWORD guid, const char* item_name, /*POSTYPE*/BYTE pos, /*MONEY*/ULONGLONG price )
{
    wchar_t unicode[255] = { 0, };
    mbstowcs(unicode, item_name, strlen(item_name)+1);

    wchar_t query[MAX_QUERY_SIZE+1] = { 0, };
    _snwprintf(query, MAX_QUERY_SIZE,
        L"insert into vendor_items(player_guid, item_name, pos, price) values(%d, \'%s\', %d, %llu);",
        guid, unicode, pos, price);
    
    wchar_t* err;
    int rc;
    rc = sqlite3_exec16(db, query, NULL, NULL, &err);

    if(rc != SQLITE_OK) {
        if (err != NULL) {
            fprintf(stderr, "execute() Error: %s\n", err);
            sqlite3_free(err);
        }
    }
}

void VendorSearchSystem::erase(DWORD guid, BYTE pos)
{
    wchar_t query[MAX_QUERY_SIZE+1] = { 0, };
    _snwprintf(query, MAX_QUERY_SIZE,
        L"delete from vendor_items where player_guid = %d and pos = %d;",
        guid, pos);
    
    wchar_t* err;
    int rc;
    rc = sqlite3_exec16(db, query, NULL, NULL, &err);

    if(rc != SQLITE_OK) {
        if (err != NULL) {
            fprintf(stderr, "execute() Error: %s\n", err);
            sqlite3_free(err);
        }
    }
}

bool VendorSearchSystem::select( DWORD guid, const char* search_name, int page)
{
    wchar_t unicode[255] = { 0, };
    mbstowcs(unicode, search_name, strlen(search_name)+1);

    int insert_count = 0;

    //////////////////////////////////////////////////////////////////////////
    //
    ;{
        wchar_t* query_clear = L"delete from search_result;";
        sqlite3_exec16(db, query_clear, NULL, NULL, NULL);

        // 검색 결과 저장
        wchar_t query[MAX_QUERY_SIZE+1] = { 0, };
        _snwprintf(query, MAX_QUERY_SIZE,
            L"insert into search_result"
            L"   select player_guid, item_name, pos, price from vendor_items where item_name like '%%%s%%'",
                unicode);
        sqlite3_exec16(db, query, NULL, NULL, NULL);

        insert_count = sqlite3_changes(db);
        //printf("changes returns %d\n", insert_count);
    };

    //////////////////////////////////////////////////////////////////////////
    //
    SEARCH_RESULT result;
    if (insert_count > 0)
    {
        int total_page   = (insert_count > 0) ? (insert_count-1)/VENDOR_ITEMS_PER_PAGE : 0;
        int current_page = min(max(page, 0), total_page);

        result.row_count = 0;
        result.current_page = current_page;
        result.total_page = total_page;

        wchar_t query[MAX_QUERY_SIZE+1] = { 0, };
        _snwprintf(query, MAX_QUERY_SIZE,
            L"select player_guid, item_name, pos from search_result"
            L"   order by item_name, price"
            L"   limit %d offset %d;",
                VENDOR_ITEMS_PER_PAGE, current_page*VENDOR_ITEMS_PER_PAGE);

        sqlite3_stmt* stmt = NULL;
        sqlite3_prepare16_v2(db, query, -1, &stmt, NULL);

        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            SEARCH_RESULT_ROW& query_result = result.result_row[result.row_count++];
            query_result.guid   = sqlite3_column_int(stmt, 0);
            /*;{
                const wchar_t* char_value = NULL;
                int char_len = 0;

                char_value = (wchar_t*)sqlite3_column_text16(stmt, 1);
                char_len = sqlite3_column_bytes16(stmt, 1);

                wcstombs(query_result.szName, char_value, char_len);
            };*/
            query_result.pos    = sqlite3_column_int(stmt, 2);
            if (result.row_count >= VENDOR_ITEMS_PER_PAGE)
                break;
        }

        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
    };

    VENDOR_SEARCH_SELECT_ACK msg(guid, result);
    SendToVendorManager(&msg, sizeof(msg));

    return true;
}

bool VendorSearchSystem::select_intro_page(DWORD guid, int page_type)
{
    int insert_count = 0;
    //////////////////////////////////////////////////////////////////////////
    //
    ;{
        wchar_t* query_clear = L"delete from search_result;";
        sqlite3_exec16(db, query_clear, NULL, NULL, NULL);

        // 검색 결과 저장
        wchar_t query[MAX_QUERY_SIZE+1] = { 0, };
        if (page_type == 0) 
        {
            /* random select */
            _snwprintf(query, MAX_QUERY_SIZE,
                L"insert into search_result"
                L"   select player_guid, item_name, pos, price from vendor_items"
                L"   order by random() limit %d", \
                    VENDOR_ITEMS_PER_PAGE);
        }
        sqlite3_exec16(db, query, NULL, NULL, NULL);

        insert_count = sqlite3_changes(db);
        //printf("changes returns %d\n", insert_count);
    };

    //////////////////////////////////////////////////////////////////////////
    //
    SEARCH_RESULT result;
    ;{
        int total_page   = 0;
        int current_page = 0;

        result.row_count = 0;
        result.current_page = current_page;
        result.total_page = total_page;

        wchar_t query[MAX_QUERY_SIZE+1] = { 0, };
        _snwprintf(query, MAX_QUERY_SIZE,
            L"select player_guid, item_name, pos from search_result"
            L" limit %d", \
                VENDOR_ITEMS_PER_PAGE);

        sqlite3_stmt* stmt = NULL;
        sqlite3_prepare16_v2(db, query, -1, &stmt, NULL);

        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            SEARCH_RESULT_ROW& query_result = result.result_row[result.row_count++];
            query_result.guid   = sqlite3_column_int(stmt, 0);
            query_result.pos    = sqlite3_column_int(stmt, 2);
            if (result.row_count >= VENDOR_ITEMS_PER_PAGE)
                break;
        }

        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
    };

    VENDOR_SEARCH_SELECT_ACK msg(guid, result);
    SendToVendorManager(&msg, sizeof(msg));

    return true;
}

int VendorSearchSystem::execute( const wchar_t* query )
{
    wchar_t* err;
    int rc;
    rc = sqlite3_exec16(db, query, NULL, NULL, &err);

    if(rc != SQLITE_OK) {
        if (err != NULL) {
            fwprintf(stderr, L"execute() Error: %s\n", err);
            sqlite3_free(err);
        }
    }

    return rc;
}

void VendorSearchSystem::print_sql_result(const char* query)
{
    char **result, *err;
    int i, j, k, l, nrows, ncols, width, *widths; 

    sqlite3_get_table(db,query,&result,&nrows,&ncols,&err);

    /* Determine column widths*/
    widths = (int*)malloc(ncols*sizeof(int));
    memset(widths,0,ncols*sizeof(int));
    for(i=0; i <= nrows; i++) {
        for(j=0; j < ncols; j++) {
            if(result[i*ncols+j] == NULL)
                continue;
            width = strlen(result[i*ncols+j]);
            if(width > widths[j]) {
                widths[j] = width;
            }
        }
    }

    for(i=0; i <= nrows; i++) {
        if(i==1) {
            for(k=0; k < ncols; k++) {
                for(l=0; l < widths[k]; l++) {
                    fprintf(stdout, "-");
                }
                fprintf(stdout, " ");
            }
            fprintf(stdout, "\n");
        }

        for(j=0; j < ncols; j++) {
            fprintf(stdout, "%-*s", widths[j]+1, result[i*ncols+j]);
        }
        fprintf(stdout, "\n");
    }

    free(widths);
    sqlite3_free_table(result);
}
