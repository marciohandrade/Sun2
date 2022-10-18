// MapObject.h: interface for the MapObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPOBJECT_H__5ACBB04B_B1A3_4716_86DE_ED7FAF1D96B7__INCLUDED_)
#define AFX_MAPOBJECT_H__5ACBB04B_B1A3_4716_86DE_ED7FAF1D96B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonCharacter.h"

class MapObject  : public NonCharacter
{
public:
	MapObject();
	virtual ~MapObject();

    virtual BOOL Init();
    virtual VOID Release();
    virtual BOOL Update( DWORD dwDeltaTick );

    DWORD GetLinkedNpcObjectKey() const
    { 
        return m_dwNpcObjectKey; 
    }
    VOID  SetLinkedNpcObjectKey( DWORD dwObjectKey ) { m_dwNpcObjectKey = dwObjectKey; }

protected:
    DWORD m_dwNpcObjectKey;
};

#endif // !defined(AFX_MAPOBJECT_H__5ACBB04B_B1A3_4716_86DE_ED7FAF1D96B7__INCLUDED_)
