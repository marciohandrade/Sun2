#pragma once

#include <hash_map>

// 채팅방 키 관리용
#include <ObjKeyGenerator.h>

class ChatZone;
class VillageZone;
class User;
class PartyManager;

typedef STLX_HASH_MAP<DWORD, ChatZone*>		ZONE_MAP;
typedef ZONE_MAP::iterator					ZONE_MAP_ITER;
typedef std::pair<DWORD, ChatZone*>			ZONE_MAP_PAIR;

typedef STLX_HASH_MAP<DWORD, VillageZone*>	VILLAGE_MAP;
typedef VILLAGE_MAP::iterator				VILLAGE_MAP_ITER;
typedef std::pair< DWORD, VillageZone * >	VILLAGE_MAP_PAIR;

typedef STLX_HASH_MAP<DWORD, User*>			USER_MAP;
typedef USER_MAP::iterator					USER_MAP_ITER;
typedef std::pair<DWORD, User*>				USER_MAP_PAIR;

class Channel
{
public:
	Channel(void);
	~Channel(void);

	VOID					Init();
	VOID					Release();
	VOID					Update();

	BOOL					LoadMap();
	BOOL					AddShoutLinkedZone( DWORD dwMapCode, ChatZone *pLinkZone );
	BOOL					AddBattleShoutLinkedZone( DWORD dwMapCode, ChatZone *pLinkZone );
	VOID					RemoveBattleShoutLinkedZone( ChatZone *pLinkZone );

	ChatZone*				AddChatZone( eZONETYPE eZoneType, DWORD dwZoneKey, DWORD dwMapCode );
	VOID					AddVillage( DWORD dwZoneKey, ChatZone *pVillage );
    VOID					AddField( DWORD dwZoneKey, ChatZone *pField );
    VOID					AddDominationField( DWORD dwZoneKey, ChatZone *pField );
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    VOID                    AddSpaField(DWORD zone_key, ChatZone* field);
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    ChatZone*				AddFriendChat( DWORD dwZoneKey );

	VOID					RemoveChatZone( DWORD dwZoneKey );
	VOID					RemoveFriendChat( DWORD dwZoneKey );

	ChatZone*				FindZone( DWORD dwZoneKey );
	VillageZone*			FindVillage( DWORD dwMapCode );
	ChatZone*				FindFriendChat( DWORD dwZoneKey );

	VOID					AddUser( User *pUser, eZONETYPE userStatus, DWORD dwZoneKey );
	VOID					RemoveUser( User *pUser, BOOL bIterErase = TRUE );
	BOOL					UserEnterZone( User *pUser, eZONETYPE eZoneType, DWORD dwZoneKey = 0, DWORD dwMapCode = 0 );

	VOID					SendToAll( BYTE *pMsg, WORD wSize );
	VOID					SendToVillages( BYTE *pMsg, WORD wSize );
	VOID					SendToBattleZones( BYTE *pMsg, WORD wSize );

	inline VOID				SetChannelID( DWORD dwChannelID ) { m_dwChannelID = dwChannelID; }
	inline DWORD			GetChannelID() { return m_dwChannelID; }

	VOID					DisplayInfo();

	VOID					UserLeavePrevZone( User *pUser );

	inline VOID				ClearZoneUserNum()
	{
		for( USHORT i = eZONETYPE_LOBBY; i < eZONETYPE_MAX; i++ )
		{
			m_dwZoneUserNum[i] = 0;
			m_dwZonePCRoomUserNum[i] = 0;
		}
	}
	inline VOID				ClearZoneNum()
	{
		for( USHORT i = eZONETYPE_LOBBY; i < eZONETYPE_MAX; i++ )
		{
			m_dwZoneNum[i] = 0;
		}
	}

/*
	inline VOID				IncZoneUserNum( eZONETYPE eZoneType, DWORD dwPCRoomID )
	{
		if( dwPCRoomID )
		{
			m_dwZonePCRoomUserNum[eZoneType]++;
		}
		else
		{
			m_dwZoneUserNum[eZoneType]++;
		}
	}

	inline VOID				DecZoneUserNum( eZONETYPE eZoneType, DWORD dwPCRoomID )
	{
		if( dwPCRoomID )
		{
			if( m_dwZonePCRoomUserNum[eZoneType] > 0 )
			{
				m_dwZonePCRoomUserNum[eZoneType]--;
			}
		}
		else
		{
			if( m_dwZoneUserNum[eZoneType] > 0 )
			{
				m_dwZoneUserNum[eZoneType]--;
			}
		}
	}
*/
	inline VOID				IncZoneNum( eZONETYPE eZoneType ) { m_dwZoneNum[eZoneType]++; }
	inline VOID				DecZoneNum( eZONETYPE eZoneType ) { if( m_dwZoneNum[eZoneType] > 0 ) m_dwZoneNum[eZoneType]--; }

	inline DWORD			GetTotalUserNum() { return (DWORD)m_mapUsers.size(); }
	inline DWORD			GetZoneUserNum( eZONETYPE eZoneType ) { return m_dwZoneUserNum[eZoneType]; }
	inline DWORD			GetZonePCRoomUserNum( eZONETYPE eZoneType ) { return m_dwZonePCRoomUserNum[eZoneType]; }
	inline DWORD			GetZoneCount( eZONETYPE eZoneType ) { return m_dwZoneNum[eZoneType]; }

	inline KEYTYPE			AllocKey()						{ return m_ChatRoomKeyGenerator.GetKey(); }
	inline VOID				FreeKey( KEYTYPE Key )			{ m_ChatRoomKeyGenerator.RestoreKey(Key); }

	VOID					CreateParty( WORD wPartyKey );
	BOOL					JoinParty( WORD wPartyKey, User* pJoinUser );
	BOOL					WithdrawParty( User* pWithdrawUser );
	VOID					DestroyParty( WORD wPartyKey );
	VOID					RestoreParty( WORD wPartyKey, User *pUser );
#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
    VOID                    SetRoomKey(const WORD party_key, const WORD room_kek);
#endif  // _NA_004694_20120412_1202_PARTY_ROOM_KEY

	// 유저 숫자 통계 관련...아따 귀찮다...-_-;
	VOID					RefreshUserNum();
	DWORD					GetNumerOfNormalUsers() { return m_dwNormalUserNum; }
	VOID					IncNumberOfNormalUsers() { m_dwNormalUserNum++; }
	VOID					DecNumberOfNormalUsers() { if( m_dwNormalUserNum > 0 ) m_dwNormalUserNum--; }
	DWORD					GetNumerOfPCRoomUsers() { return m_dwPCRoomUserNum; }
	VOID					IncNumberOfPCRoomUsers() { m_dwPCRoomUserNum++; }
	VOID					DecNumberOfPCRoomUsers() { if( m_dwPCRoomUserNum > 0 ) m_dwPCRoomUserNum--; }

private:

	// 
	ZONE_MAP				m_mapZones;


	util::CObjKeyGenerator<KEYTYPE>	m_ChatRoomKeyGenerator;	// 채팅방 키 제조기
	ZONE_MAP				m_mapFriendChat;		// 친구 채팅용
	USER_MAP				m_mapUsers;
	DWORD					m_dwChannelID;

	DWORD					m_dwZonePCRoomUserNum[eZONETYPE_MAX];	// 각 지역의 PC방 유저 넘버
	DWORD					m_dwZoneUserNum[eZONETYPE_MAX];
	DWORD					m_dwZoneNum[eZONETYPE_MAX];

	DWORD					m_dwNormalUserNum;						// 접속중인 전체 일반 유저 숫자
	DWORD					m_dwPCRoomUserNum;						// 접속중인 전체 PC방 유저 숫자

	PartyManager			*m_pPartyManager;
	VILLAGE_MAP				m_mapVillages;	// 마을/필드 지역 임시 저장용 맵
};
