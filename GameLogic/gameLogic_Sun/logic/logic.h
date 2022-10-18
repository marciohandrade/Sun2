#ifndef __LOGIC_H__
#define __LOGIC_H__
// logic.h

//#define CHECK(x) {if(!x) {return E_FAIL;} }
#define CHECK(x, err) {if(!x) {_LOG(err);return E_FAIL;} }


void GetStringByCode(DWORD dwCode, char* dest, int destLen);

#endif //__LOGIC_H__