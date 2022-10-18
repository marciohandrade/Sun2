// Item.h: interface for the Item class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__65ED0FFC_8AE9_4595_8223_07D6A5E1B55F__INCLUDED_)
#define AFX_ITEM_H__65ED0FFC_8AE9_4595_8223_07D6A5E1B55F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SCItem.h>
#include <Const.h>
#include "NonCharacter.h"
#include <SCItemSlot.h>

class Item : public NonCharacter, public SCItem
{
	friend class ObjectFactory;
public:
	Item();
	virtual ~Item();

	VOID					GetItemRenderInfo( ITEM_RENDER_INFO & OUT RenderInfo );

	virtual BOOL			Init();
	virtual VOID			Release();
	virtual BOOL			Update( DWORD dwDeltaTick );

	// 아이템 타입 용
	VOID					CreateItem( DWORD OwnerKey, DWORD MonsterKey, SCItemSlot & IN slot )
	{
		SetOwnerPlayerKey( OwnerKey );
		SetFromMonsterKey( MonsterKey );
		m_ItemSlot.Copy( slot );
		m_Money = 0;
	}
	SCItemSlot &			GetItemSlot() { return m_ItemSlot;	}
	VOID					GetItem( ITEMSTREAMEX & OUT ItemStreamOut )
	{
		m_ItemSlot.CopyOut( ItemStreamOut );
	}

    // 돈 타입 용
    void CreateMoney(DWORD owner_key, DWORD monster_key, const MONEY& money)
    {
        m_ItemSlot.Clear();
        SetOwnerPlayerKey(owner_key);
        SetFromMonsterKey(monster_key);
        SetMoney(money);
    }
    void SetMoney(const MONEY& money) { m_Money = money; }
	inline MONEY			GetMoney()						{ return m_Money;	}

	virtual VOID			OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle=-1 );
	virtual VOID			OnLeaveField();

public :
	virtual VOID			Reuse()		{ Release(); }
	virtual VOID			OnEnterObject( const Sector & IN rSector );
	virtual VOID			OnLeaveObject( const Sector & IN rSector );

private:
	enum { _MAX_FREE_OWNERSHIP_TIME_DELAY = 1000*60, };
	enum { _MAX_DESTROY_TIME_DELAY = 1000*180, };
	util::SimpleTimer		m_OwnershipTimer;		// 30초간 소유하지 않으면 소유권이 사라진다.
	util::SimpleTimer		m_DestroyTimer;

	SCItemSlot				m_ItemSlot;				//< 최적화 필요
	MONEY					m_Money;				//< 최적화 필요
	__PROPERTY( DWORD, OwnerPlayerKey );
	__PROPERTY( DWORD, FromMonsterKey );
};

#endif // !defined(AFX_ITEM_H__65ED0FFC_8AE9_4595_8223_07D6A5E1B55F__INCLUDED_)












