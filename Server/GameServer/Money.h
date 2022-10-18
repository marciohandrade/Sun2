// Money.h: interface for the Money class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONEY_H__4C8DFAEB_2B5B_4F1B_AABC_B60C558029BF__INCLUDED_)
#define AFX_MONEY_H__4C8DFAEB_2B5B_4F1B_AABC_B60C558029BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonCharacter.h"

//-------------------------------------------------------------------------------
// 사용하지 않는 클래스
// 지울 예정 : taiyo
//-------------------------------------------------------------------------------
class Money : public NonCharacter  
{
public:
	Money();
	virtual ~Money();

	virtual BOOL			Init();
	virtual BOOL			Update( DWORD dwDeltaTick );
	virtual VOID			Reuse()		{ Release(); }

protected:
	virtual VOID			Release();
};

#endif // !defined(AFX_MONEY_H__4C8DFAEB_2B5B_4F1B_AABC_B60C558029BF__INCLUDED_)
