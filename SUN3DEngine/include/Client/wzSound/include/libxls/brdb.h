#ifndef __BRDB_H__
#define __BRDB_H__


//#ifdef __cplusplus
//extern "C" {
//#endif
	
struct str_brdb
{
    WORD opcode;
    char * name;			/* printable name */
    char * desc;			/* printable description */
};
typedef struct str_brdb record_brdb;

record_brdb brdb[] =
    {
#include "brdb.c.h"
    };

static int get_brbdnum(int id)
{

    int i;
    i=0;
    do
    {
        if (brdb[i].opcode==id)
            return i;
        i++;
    }
    while (brdb[i].opcode!=0xFFF);
    return 0;
}

//#ifdef __cplusplus
//}
//#endif

#endif //__BRDB_H__