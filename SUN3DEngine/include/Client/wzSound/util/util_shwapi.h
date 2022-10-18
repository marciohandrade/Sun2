#ifndef __UTIL_SHWAPI_H__
#define __UTIL_SHWAPI_H__
// util_shwapi.h

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL    path_is_directory(char *filename);
DWORD  get_program_path(HINSTANCE hInst, LPTSTR pszBuffer,
                              DWORD dwSize);
void    path_add_backslash(char *path);

BOOL    path_is_relative(const char *path);
BOOL    path_remove_filespec(LPTSTR path);
void    path_unquote(LPTSTR path);

char *replace_filename(char *dest, char *path, char *filename, int size);
char *replace_extension(char *dest, char *filename, char *ext, int size);
char *append_filename(char *dest, char *path, char *filename, int size);
char *get_filename(char *path);
char *get_extension(char *filename);

#ifdef __cplusplus
}
#endif

#endif //__UTIL_SHWAPI_H__