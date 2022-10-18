// Money.cpp: implementation of the Money class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Money.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Money::Money()
{

}

Money::~Money()
{

}

BOOL Money::Init()
{
	return NonCharacter::Init();
}

VOID Money::Release()
{
	NonCharacter::Release();
}

BOOL Money::Update( DWORD dwDeltaTick )
{
	return NonCharacter::Update( dwDeltaTick );
}