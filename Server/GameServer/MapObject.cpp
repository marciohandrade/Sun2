// MapObject.cpp: implementation of the MapObject class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MapObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MapObject::MapObject()
: m_dwNpcObjectKey(0)
{
	SetObjectType( MAP_OBJECT );
}

MapObject::~MapObject()
{

}
BOOL MapObject::Init()
{
	return NonCharacter::Init();
}

VOID MapObject::Release()
{
	NonCharacter::Release();
}

BOOL MapObject::Update( DWORD dwDeltaTick )
{
	return NonCharacter::Update( dwDeltaTick );
}
