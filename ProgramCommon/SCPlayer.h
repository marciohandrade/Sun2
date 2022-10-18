#ifndef __SCPLAYER_H__
#define __SCPLAYER_H__
#pragma once

//==================================================================================================
//  @history
//  100827, waverix - r100827.4L, a server object is not derived this class anymore
//                    because of a server and client logic is definitely different system
//                    that don't have shared process using this.
//==================================================================================================


//==================================================================================================
//  PLAYER ����
enum ePLAYER_STATE
{
    PLAYER_CREATED,     // �÷��̾� ������ �޾Ƶ���
    PLAYER_CHARINFO_FIRST_REQUESTED,
    PLAYER_CHARINFO_REQUESTED,
    PLAYER_LOADING,
    PLAYER_MAP_ENTER,
    PLAYER_DESTROYED,
};
//
const BYTE KEYBOARDMOVE_FOWARD    = 0x00000001;
const BYTE KEYBOARDMOVE_BACKWARD  = 0x00000002;
//
const BYTE KEYBOARDMOVE_LEFTTURN  = 0x00000004;
const BYTE KEYBOARDMOVE_RIGHTTURN = 0x00000008;
const BYTE KEYBOARDMOVE_LEFTSTEP  = 0x00000010;
const BYTE KEYBOARDMOVE_RIGHTSTEP = 0x00000020;
//
const BYTE KEYBOARDMOVE_ISMOVING  = 0x00000033;
//
//
//
struct BASE_PLAYERINFO;


//==================================================================================================

// CLIENT
#if !defined(_SERVER)

#include <SolarHashTable.h>
#include <Timer.h>

//	<SCPlayer>
class SCPlayer
{
public:		SCPlayer();
			~SCPlayer();

	//inline	BYTE						GetState()					{ return m_byState; }
	//inline	VOID						SetState(BYTE byState)		{ m_byState = byState;}

	inline
	virtual	const BASE_PLAYERINFO*		GetCharInfo() const			{ return m_pCharInfo;	}
	inline
	virtual	BASE_PLAYERINFO*			GetCharInfo()				{ return m_pCharInfo;	}
	VOID								SetCharInfo( const BASE_PLAYERINFO* pCharInfo );

	eCHAR_TYPE							GetCharType() const;
	LEVELTYPE							GetLevel() const;

	//inline virtual VOID					SetWeaponKind( DWORD dwWeaponKind )	{ m_dwWeaponKind = dwWeaponKind; }
	//inline virtual DWORD				GetWeaponKind() const { return m_dwWeaponKind; }

	virtual	VOID						OnSetCharInfo() {}

	// Item Cool Timer
	VOID								SetItemCoolTime( BOOL bPoolUse, DWORD dwCoolTimeMagin = 0 );
	VOID								FreeItemCoolTime( BOOL bPoolUse );
	util::Timer *						GetItemCoolTimer( WORD wType ) { return m_pItemCoolTimeHash->GetData( wType ); };
	VOID								InitializeItemCooltime();

protected:	// taiyo : �����غ��� ���� Ŭ���̾�Ʈ���� BASE_PLAYERINFO ��θ� ���� �ʿ�� ����.!!! �ٲ��ʿ� ����
	BASE_PLAYERINFO *					m_pCharInfo;	

private:	
	//BYTE								m_byState;			// �÷��̾��� ����
	//DWORD								m_dwWeaponKind;

	// ������ ��Ÿ�� Ÿ�̸� �ؽ����̺�(�Ҹ� ������ ���� Ÿ�̸Ӹ� ����) 
	typedef util::SolarHashTable<util::Timer *>		PLAYER_ITEM_COOL_TIMER_HASH;
	PLAYER_ITEM_COOL_TIMER_HASH*	m_pItemCoolTimeHash;
};

#endif //!defined(_SERVER)

//#ifdef _SERVER

struct IPlayerAttributeLinker
{
    virtual BASE_PLAYERINFO* GetCharacterInfo() const = 0;
    virtual eCHAR_TYPE GetCharType() const = 0;
    virtual LEVELTYPE GetLevel() const = 0;
    virtual DWORD GetWeaponKind() const = 0;
#ifdef _SERVER
    virtual void SetCharInfo(BASE_PLAYERINFO* char_info) = 0;
#else
    virtual void SetCharInfo(const BASE_PLAYERINFO* char_info) = 0;
#endif
};

//#endif

#endif // __SCPLAYER_H__









