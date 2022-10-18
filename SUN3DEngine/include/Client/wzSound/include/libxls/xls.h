#ifndef __XLS_H__
#define __XLS_H__
#include "xlstool.h"

#include <string>
#include <vector>

using namespace std;

//#include "xlstypes.h"

//#ifdef __cplusplus
//extern "C" {
//#endif
typedef	struct ST_CELLS
{
	std::vector<string> vec;
}
ST_CELLS;

typedef	struct ST_ROWS
{
	std::vector<ST_CELLS> vec;
}
ST_ROWS;

static char* libxls_version="1.0.0";

int xls(void);
void xls_addSST(xlsWorkBook* pWB,SST* sst,DWORD size);
void xls_appendSST(xlsWorkBook* pWB,BYTE* buf,DWORD size);

void xls_parseWorkBook(xlsWorkBook* pWB);
void xls_parseWorkSheet(xlsWorkSheet* pWS);
xlsWorkSheet * xls_getWorkSheet(xlsWorkBook* pWB,int num);
xlsWorkBook* xls_open(char *file,char* charset);
void xls_close(xlsWorkBook* pWB);


BOOL GetTxtFile(char* szFilePath, ST_ROWS* stRows);		
void SetRows(string& strData, ST_ROWS* stRows);
BOOL IsRows(string strData);
void SetRow(string strData, ST_ROWS* stRows);

//#ifdef __cplusplus
//}
//#endif

#endif //__XLS_H__