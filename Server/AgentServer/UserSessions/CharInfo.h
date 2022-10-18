#pragma once
#ifndef AGENTSERVER_CHAR_INFO_H
#define AGENTSERVER_CHAR_INFO_H

#include "UserInfo.Structures.h"

//==================================================================================================
namespace ns_object {
;

struct CharacterInfoList
{
    DWORD cur_hp, max_hp, cur_mp, max_mp, cur_sd, max_sd;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    PLAYERFP cur_fp,max_fp;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    uint8_t chao_state;
    // last order...
    DWORD char_name_hashes[MAX_CHARACTER_SLOT_NUM];
    SERVER_CHARACTER_PART nodes[MAX_CHARACTER_SLOT_NUM];
};

struct CharacterList
{
    struct PoolInstance;
    //
    CharacterList();
    ~CharacterList();
    //
    void Init(); // called by User::Init
    void Release(); // called by User::Release
    //
    bool SelectCharacter(int selecting_char_slot_index);
    //
    void SerializeCharacterPart(BYTE slot_index, SERVER_CHARACTER_PART& INOUT parts, eSERIALIZE se);
    void LoadClientCharacterPart(BYTE slot_index,
        CLIENT_CHARACTER_PART* const result_parts) const;
    // data fields...
    SERVER_CHARACTER_PART* selected_char;
    uint8_t selected_char_slot_index;
    uint8_t number_of_characters;
    bool occupied_slots[MAX_CHARACTER_SLOT_NUM];
    CharacterInfoList* character_infos; // managed by pool control, User::Init, User::Release
    //
    static PoolInstance* pool_instance_s;
    //
    __DISABLE_COPY(CharacterList);
};

}; //end of namespace
//==================================================================================================

#if SUN_CODE_BACKUP
//
struct sUSER_CHAR_INFO
{
	// 캐릭터 정보
	BYTE					m_bySelectedCharIndex;
	BYTE					m_byCharNum;
	BOOL					m_bOccupiedSlot[MAX_CHARACTER_SLOT_NUM];
	// 캐릭터 정보
	DWORD					m_dwCurHP;
	DWORD					m_dwMaxHP;
	DWORD					m_dwCurMP;
	DWORD					m_dwMaxMP;
	eCHAO_STATE				m_ChaoState;
	SERVER_CHARACTER_PART	m_CharacterPart[MAX_CHARACTER_LIST_NUM+1];	//< 0~5까지 사용, 6는 초기값(사용하지 않음)

	struct
	{
		BYTE				byPlayingTutorial;
		MAPCODE				mcLastVillageMapCode;
	} m_Tutorials[MAX_CHARACTER_LIST_NUM];

    DWORD					m_dwCurSD;
    DWORD					m_dwMaxSD;
};

//////////////////////////////////////////////////////////////////////////
// <CHARACTER OPERATOR>
//
class CharacterOperator
{
	sUSER_CHAR_INFO&		m_rCHAR_INFO;
public:
	CharacterOperator( sUSER_CHAR_INFO& rINFO ) : m_rCHAR_INFO(rINFO) { Clear(); }

	VOID Clear() {
		memset( &m_rCHAR_INFO, 0, sizeof(m_rCHAR_INFO) );
		m_rCHAR_INFO.m_bySelectedCharIndex  = MAX_CHARACTER_LIST_NUM;
		m_rCHAR_INFO.m_ChaoState            = eCHAO_STATE_NORMAL;
	}

	VOID SelectChar( BYTE byIndex );

    // TempUser시는 Null, User일 때 : 선택된 캐릭터
    virtual BYTE GetCharClass() {
        return m_rCHAR_INFO.m_bySelectedCharIndex<MAX_CHARACTER_SLOT_NUM ? 
            m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_byClass : 0;
    }

    virtual LEVELTYPE GetCharLV() {
        return m_rCHAR_INFO.m_bySelectedCharIndex<MAX_CHARACTER_SLOT_NUM ?
            m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_LV : 0;
    }
    inline LEVELTYPE GetCharLV( BYTE bySlotIndex ) {
        return bySlotIndex<MAX_CHARACTER_SLOT_NUM ? m_rCHAR_INFO.m_CharacterPart[bySlotIndex].m_LV : 0;
    }

    //
    virtual const TCHAR* GetCharName() {
        return m_rCHAR_INFO.m_bySelectedCharIndex<MAX_CHARACTER_SLOT_NUM ? 
            m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_tszCharName : NULL;
    }
    inline TCHAR* GetCharName( BYTE bySlotIndex )  { return m_rCHAR_INFO.m_CharacterPart[bySlotIndex].m_tszCharName; }

    inline void SetCharName( BYTE bySlotIndex, TCHAR* char_name )  { 
        strncpy(m_rCHAR_INFO.m_CharacterPart[bySlotIndex].m_tszCharName, char_name, MAX_CHARNAME_LENGTH+1);
        m_rCHAR_INFO.m_CharacterPart[bySlotIndex].m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';        
    }

	//
	inline BYTE GetSelectedCharIndex()                      { return m_rCHAR_INFO.m_bySelectedCharIndex; }

    inline VOID SetNumberOfCreatedChars( BYTE byCharNum )   { m_rCHAR_INFO.m_byCharNum = byCharNum; }
	inline BYTE	GetNumberOfCreatedChars()                   { return m_rCHAR_INFO.m_byCharNum; }

    inline DWORD  GetCharGUID( BYTE bySlotIndex )  { return m_rCHAR_INFO.m_CharacterPart[bySlotIndex].m_CharGuid; }
	inline BOOL	  IsCharDelete( BYTE bySlotIndex ) { return (m_rCHAR_INFO.m_CharacterPart[bySlotIndex].m_DelChk != 0); }


	//
    inline CHARGUID GetSelectedCharGuid() {
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		return m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_CharGuid;
	}

	inline TCHAR* GetSelectedCharName() {
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		return (TCHAR *)&m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_tszCharName;
	}

	inline LEVELTYPE GetSelectedCharLV() {
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		return m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_LV;
	}

	inline BYTE	GetSelectedCharClass()	{
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		return m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_byClass;
	}

	inline MAPCODE GetSelectedCharMapCode()	{
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		return (MAPCODE)m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_iRegion;
	}

	inline VOID SetSelectedCharMapCode(MAPCODE mCode) {
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_iRegion=mCode;
	}

	inline GUILDGUID GetSelectedCharGuildGuid()	{
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		return m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_GuildGuid;
	}

	inline VOID SetSelectedCharGuildGuid( GUILDGUID guid )	{
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_GuildGuid=guid;
	}

	inline BYTE GetSelectedCharState() {
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		return m_rCHAR_INFO.m_CharacterPart[m_rCHAR_INFO.m_bySelectedCharIndex].m_CharState;
	}

	//
	inline VOID	 SetSelectedCharMaxHP( DWORD dwMaxHP ) { ASSERT( dwMaxHP ); m_rCHAR_INFO.m_dwMaxHP = dwMaxHP; }
    inline VOID  SetSelectedCharMaxMP( DWORD dwMaxMP ) { ASSERT( dwMaxMP ); m_rCHAR_INFO.m_dwMaxMP = dwMaxMP; }
    inline VOID  SetSelectedCharCurHP( DWORD dwCurHP ) { m_rCHAR_INFO.m_dwCurHP = dwCurHP; }
    inline VOID  SetSelectedCharCurMP( DWORD dwCurMP ) { m_rCHAR_INFO.m_dwCurMP = dwCurMP; }

    inline DWORD GetSelectedCharMaxHP()	{ ASSERT( m_rCHAR_INFO.m_dwMaxHP );	return m_rCHAR_INFO.m_dwMaxHP; }
    inline DWORD GetSelectedCharMaxMP() { ASSERT( m_rCHAR_INFO.m_dwMaxMP );	return m_rCHAR_INFO.m_dwMaxMP; }
    inline DWORD GetSelectedCharCurMP()	{ return m_rCHAR_INFO.m_dwCurMP; }
    inline DWORD GetSelectedCharCurHP() { return m_rCHAR_INFO.m_dwCurHP; }

    // AS 에서 사용하는 SD 용도는 AG_STATUS_LEVEL_UP_CMD, AG_STATUS_CHAR_STAT_CHANGE_CMD 뿐이나 정보는 유지한다.
    // 현재는 정보 유지 필요 없음. (다이렉트로 가능)
    inline VOID  SetSelectedCharMaxSD( DWORD dwMaxSD ) { ASSERT( dwMaxSD ); m_rCHAR_INFO.m_dwMaxSD = dwMaxSD; }
    inline VOID  SetSelectedCharCurSD( DWORD dwCurSD ) { m_rCHAR_INFO.m_dwCurSD = dwCurSD; }
    inline DWORD GetSelectedCharMaxSD() { ASSERT( m_rCHAR_INFO.m_dwMaxSD ); return m_rCHAR_INFO.m_dwMaxSD; }
    inline DWORD GetSelectedCharCurSD() { return m_rCHAR_INFO.m_dwCurSD; }

	inline VOID SetSelectedCharChaoState( BYTE byChaoState ) { m_rCHAR_INFO.m_ChaoState = (eCHAO_STATE)byChaoState;	}
	inline BYTE GetSelectedCharChaoState() { return (BYTE)m_rCHAR_INFO.m_ChaoState; }

	inline BOOL IsChaoState() { return m_rCHAR_INFO.m_ChaoState == eCHAO_STATE_CHAO; }

    inline VOID SetCharLevel( BYTE bySlotIndex, LEVELTYPE LV ) { 
        ASSERT( bySlotIndex < MAX_CHARACTER_SLOT_NUM ); 
		m_rCHAR_INFO.m_CharacterPart[bySlotIndex].m_LV = LV;
	}

	inline VOID SetCharDelChk( BYTE bySlotIndex, BYTE chk )	{
        ASSERT( bySlotIndex < MAX_CHARACTER_SLOT_NUM ); 
		m_rCHAR_INFO.m_CharacterPart[bySlotIndex].m_DelChk = chk;
	}

	inline VOID SerializeCharacterPart( BYTE bySlotIndex, SERVER_CHARACTER_PART & INOUT CharacterPart, eSERIALIZE se )
	{ 
		ASSERT( bySlotIndex < MAX_CHARACTER_SLOT_NUM ); 
		if( bySlotIndex >= MAX_CHARACTER_SLOT_NUM ) return;

		if( se == SERIALIZE_STORE )
			memcpy( &m_rCHAR_INFO.m_CharacterPart[bySlotIndex], &CharacterPart, sizeof(SERVER_CHARACTER_PART) );
		else if( se == SERIALIZE_LOAD )
			memcpy( &CharacterPart, &m_rCHAR_INFO.m_CharacterPart[bySlotIndex], sizeof(SERVER_CHARACTER_PART) );
	}

	//
	VOID SerializeCharacterPart( BYTE bySlotIndex, CLIENT_CHARACTER_PART & INOUT CharacterPart, eSERIALIZE se );



	// 유효한 슬롯인지 여부 처리
	inline VOID SetOccupiedSlot( BYTE bySlotIndex, BOOL val ) {	m_rCHAR_INFO.m_bOccupiedSlot[bySlotIndex] = val; }
	inline BOOL IsValidSlot( BYTE bySlotIndex )               { return m_rCHAR_INFO.m_bOccupiedSlot[bySlotIndex]; }


	inline VOID SetSelectedCharPlayingTutorial( BOOL bIsPlaying ) {
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		m_rCHAR_INFO.m_Tutorials[m_rCHAR_INFO.m_bySelectedCharIndex].byPlayingTutorial = (BYTE)bIsPlaying;
	}

	inline BOOL IsSelectedCharPlayingTutorial() {
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		return (BOOL)(m_rCHAR_INFO.m_Tutorials[m_rCHAR_INFO.m_bySelectedCharIndex].byPlayingTutorial ? TRUE : FALSE);
	}

	inline VOID SetSelectedCharLastVillageMapCode( MAPCODE mCode ) {
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		m_rCHAR_INFO.m_Tutorials[m_rCHAR_INFO.m_bySelectedCharIndex].mcLastVillageMapCode = mCode;
	}

	inline MAPCODE GetSelectedCharLastVillageMapCode() {
		ASSERT( m_rCHAR_INFO.m_bySelectedCharIndex <= MAX_CHARACTER_SLOT_NUM );
		return m_rCHAR_INFO.m_Tutorials[m_rCHAR_INFO.m_bySelectedCharIndex].mcLastVillageMapCode;
	}

};

#endif //SUN_CODE_BACKUP

#endif //AGENTSERVER_CHAR_INFO_H