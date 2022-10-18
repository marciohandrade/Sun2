// myAPI.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYAPI_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
#define AFX_MYAPI_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_

#include "SQLAPI.h"

// API header(s)
#ifdef LINUX
	#include "./my_linux/mysql.h"
#endif
#ifdef WIN32
	#include "./my_win32/mysql.h"
#endif

extern void AddMySQLSupport();
extern void ReleaseMySQLSupport();

/* Functions to get information from the MYSQL and MYSQL_RES structures */
/* Should definitely be used if one uses shared libraries */

typedef my_ulonglong (STDCALL *mysql_num_rows_t)(MYSQL_RES *res);
typedef unsigned int (STDCALL *mysql_num_fields_t)(MYSQL_RES *res);
typedef my_bool (STDCALL *mysql_eof_t)(MYSQL_RES *res);
typedef MYSQL_FIELD * (STDCALL *mysql_fetch_field_direct_t)(MYSQL_RES *res,
					      unsigned int fieldnr);
typedef MYSQL_FIELD * (STDCALL *mysql_fetch_fields_t)(MYSQL_RES *res);
typedef MYSQL_ROWS * (STDCALL *mysql_row_tell_t)(MYSQL_RES *res);
typedef unsigned int (STDCALL *mysql_field_tell_t)(MYSQL_RES *res);

typedef unsigned int (STDCALL *mysql_field_count_t)(MYSQL *mysql);
typedef my_ulonglong (STDCALL *mysql_affected_rows_t)(MYSQL *mysql);
typedef my_ulonglong (STDCALL *mysql_insert_id_t)(MYSQL *mysql);
typedef unsigned int (STDCALL *mysql_errno_t)(MYSQL *mysql);
typedef char * (STDCALL *mysql_error_t)(MYSQL *mysql);
typedef char * (STDCALL *mysql_info_t)(MYSQL *mysql);
typedef unsigned long (STDCALL *mysql_thread_id_t)(MYSQL *mysql);
typedef const char * (STDCALL *mysql_character_set_name_t)(MYSQL *mysql);

typedef MYSQL *		(STDCALL *mysql_init_t)(MYSQL *mysql);
//#ifdef HAVE_OPENSSL
typedef int		(STDCALL *mysql_ssl_set_t)(MYSQL *mysql, const char *key,
				      const char *cert, const char *ca,
				      const char *capath);
typedef char *		(STDCALL *mysql_ssl_cipher_t)(MYSQL *mysql);
typedef int		(STDCALL *mysql_ssl_clear_t)(MYSQL *mysql);
//#endif /* HAVE_OPENSSL */
typedef MYSQL *		(STDCALL *mysql_connect_t)(MYSQL *mysql, const char *host,
				      const char *user, const char *passwd);
typedef my_bool		(STDCALL *mysql_change_user_t)(MYSQL *mysql, const char *user, 
					  const char *passwd, const char *db);
//#if MYSQL_VERSION_ID >= 32200
typedef MYSQL *		(STDCALL *mysql_real_connect2_t)(MYSQL *mysql, const char *host,
					   const char *user,
					   const char *passwd,
					   const char *db,
					   unsigned int port,
					   const char *unix_socket,
					   unsigned int clientflag);
//#else
typedef MYSQL *		(STDCALL *mysql_real_connect1_t)(MYSQL *mysql, const char *host,
					   const char *user,
					   const char *passwd,
					   unsigned int port,
					   const char *unix_socket,
					   unsigned int clientflag);
//#endif
typedef void		(STDCALL *mysql_close_t)(MYSQL *sock);
typedef int		(STDCALL *mysql_select_db_t)(MYSQL *mysql, const char *db);
typedef int		(STDCALL *mysql_query_t)(MYSQL *mysql, const char *q);
typedef int		(STDCALL *mysql_send_query_t)(MYSQL *mysql, const char *q,
					 unsigned int length);
typedef int		(STDCALL *mysql_read_query_result_t)(MYSQL *mysql);
typedef int		(STDCALL *mysql_real_query_t)(MYSQL *mysql, const char *q,
					unsigned int length);
typedef int		(STDCALL *mysql_create_db_t)(MYSQL *mysql, const char *DB);
typedef int		(STDCALL *mysql_drop_db_t)(MYSQL *mysql, const char *DB);
typedef int		(STDCALL *mysql_shutdown_t)(MYSQL *mysql);
typedef int		(STDCALL *mysql_dump_debug_info_t)(MYSQL *mysql);
typedef int		(STDCALL *mysql_refresh_t)(MYSQL *mysql,
				     unsigned int refresh_options);
typedef int		(STDCALL *mysql_kill_t)(MYSQL *mysql,unsigned long pid);
typedef int		(STDCALL *mysql_ping_t)(MYSQL *mysql);
typedef char *		(STDCALL *mysql_stat_t)(MYSQL *mysql);
typedef char *		(STDCALL *mysql_get_server_info_t)(MYSQL *mysql);
typedef char *		(STDCALL *mysql_get_client_info_t)(void);
typedef char *		(STDCALL *mysql_get_host_info_t)(MYSQL *mysql);
typedef unsigned int	(STDCALL *mysql_get_proto_info_t)(MYSQL *mysql);
typedef MYSQL_RES *	(STDCALL *mysql_list_dbs_t)(MYSQL *mysql,const char *wild);
typedef MYSQL_RES *	(STDCALL *mysql_list_tables_t)(MYSQL *mysql,const char *wild);
typedef MYSQL_RES *	(STDCALL *mysql_list_fields_t)(MYSQL *mysql, const char *table,
					 const char *wild);
typedef MYSQL_RES *	(STDCALL *mysql_list_processes_t)(MYSQL *mysql);
typedef MYSQL_RES *	(STDCALL *mysql_store_result_t)(MYSQL *mysql);
typedef MYSQL_RES *	(STDCALL *mysql_use_result_t)(MYSQL *mysql);
typedef int		(STDCALL *mysql_options_t)(MYSQL *mysql,enum mysql_option option,
				      const char *arg);
typedef void		(STDCALL *mysql_free_result_t)(MYSQL_RES *result);
typedef void		(STDCALL *mysql_data_seek_t)(MYSQL_RES *result,
					my_ulonglong offset);
typedef MYSQL_ROW_OFFSET (STDCALL *mysql_row_seek_t)(MYSQL_RES *result, MYSQL_ROW_OFFSET);
typedef MYSQL_FIELD_OFFSET (STDCALL *mysql_field_seek_t)(MYSQL_RES *result,
					   MYSQL_FIELD_OFFSET offset);
typedef MYSQL_ROW	(STDCALL *mysql_fetch_row_t)(MYSQL_RES *result);
typedef unsigned long * (STDCALL *mysql_fetch_lengths_t)(MYSQL_RES *result);
typedef MYSQL_FIELD *	(STDCALL *mysql_fetch_field_t)(MYSQL_RES *result);
typedef unsigned long	(STDCALL *mysql_escape_string_t)(char *to,const char *from,
					    unsigned long from_length);
typedef unsigned long (STDCALL *mysql_real_escape_string_t)(MYSQL *mysql,
					       char *to,const char *from,
					       unsigned long length);
typedef void		(STDCALL *mysql_debug_t)(const char *debug);
typedef char *		(STDCALL *mysql_odbc_escape_string_t)(MYSQL *mysql,
						 char *to,
						 unsigned long to_length,
						 const char *from,
						 unsigned long from_length,
						 void *param,
						 char *
						 (*extend_buffer)
						 (void *, char *to,
						  unsigned long *length));
typedef void 		(STDCALL *myodbc_remove_escape_t)(MYSQL *mysql,char *name);
typedef unsigned int	(STDCALL *mysql_thread_safe_t)(void);

  
//#define mysql_reload(mysql) mysql_refresh((mysql),REFRESH_GRANT)


// API declarations
class SQLAPI_API myAPI : public saAPI
{
public:
	myAPI();

	mysql_num_rows_t mysql_num_rows;
	mysql_num_fields_t mysql_num_fields;
	mysql_eof_t mysql_eof;
	mysql_fetch_field_direct_t mysql_fetch_field_direct;
	mysql_fetch_fields_t mysql_fetch_fields;
	mysql_row_tell_t mysql_row_tell;
	mysql_field_tell_t mysql_field_tell;
	mysql_field_count_t mysql_field_count;
	mysql_affected_rows_t mysql_affected_rows;
	mysql_insert_id_t mysql_insert_id;
	mysql_errno_t mysql_errno;
	mysql_error_t mysql_error;
	mysql_info_t mysql_info;
	mysql_thread_id_t mysql_thread_id;
	mysql_character_set_name_t mysql_character_set_name;
	mysql_init_t mysql_init;
	mysql_ssl_set_t mysql_ssl_set;
	mysql_ssl_cipher_t mysql_ssl_cipher;
	mysql_ssl_clear_t mysql_ssl_clear;
	mysql_connect_t mysql_connect;
	mysql_change_user_t mysql_change_user;
	mysql_real_connect1_t mysql_real_connect1;
	mysql_real_connect2_t mysql_real_connect2;
	mysql_close_t mysql_close;
	mysql_select_db_t mysql_select_db;
	mysql_query_t mysql_query;
	mysql_send_query_t mysql_send_query;
	mysql_read_query_result_t mysql_read_query_result;
	mysql_real_query_t mysql_real_query;
	mysql_create_db_t mysql_create_db;
	mysql_drop_db_t mysql_drop_db;
	mysql_shutdown_t mysql_shutdown;
	mysql_dump_debug_info_t mysql_dump_debug_info;
	mysql_refresh_t mysql_refresh;
	mysql_kill_t mysql_kill;
	mysql_ping_t mysql_ping;
	mysql_stat_t mysql_stat;
	mysql_get_server_info_t mysql_get_server_info;
	mysql_get_client_info_t mysql_get_client_info;
	mysql_get_host_info_t mysql_get_host_info;
	mysql_get_proto_info_t mysql_get_proto_info;
	mysql_list_dbs_t mysql_list_dbs;
	mysql_list_tables_t mysql_list_tables;
	mysql_list_fields_t mysql_list_fields;
	mysql_list_processes_t mysql_list_processes;
	mysql_store_result_t mysql_store_result;
	mysql_use_result_t mysql_use_result;
	mysql_options_t mysql_options;
	mysql_free_result_t mysql_free_result;
	mysql_data_seek_t mysql_data_seek;
	mysql_row_seek_t mysql_row_seek;
	mysql_field_seek_t mysql_field_seek;
	mysql_fetch_row_t mysql_fetch_row;
	mysql_fetch_lengths_t mysql_fetch_lengths;
	mysql_fetch_field_t mysql_fetch_field;
	mysql_escape_string_t mysql_escape_string;
	mysql_real_escape_string_t mysql_real_escape_string;
	mysql_debug_t mysql_debug;
	mysql_odbc_escape_string_t mysql_odbc_escape_string;
	myodbc_remove_escape_t myodbc_remove_escape;
	mysql_thread_safe_t mysql_thread_safe;
};

class SQLAPI_API myConnectionHandles : public saConnectionHandles
{
public:
	myConnectionHandles();

	MYSQL *mysql;	// MySQL connection struct
};

class SQLAPI_API myCommandHandles : public saCommandHandles
{
public:
	myCommandHandles();

	MYSQL_RES *result; // MySQL result struct
};

extern myAPI g_myAPI;

#endif // !defined(AFX_MYSQLAPI_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
