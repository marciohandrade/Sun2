#pragma once

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <SHARED>
#include <Version.h>
#include <LinkedListCS.h>
#include <Timer.h>

// <COMMON>
#include <ServerStruct.h>
#include <ItemOwnFlag.h>
#include <MissionManager.h>

// <AGENT SERVER>
#include "UserSession.h"
#include "CharInfo.h"

#include <PartyState.h>

// <SECURE>
#include <CryptManager.h>
#include <TimeCheck/HeartbeatChecker.h>

class TempUser;

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
#include "UserInfo.Secure.h"

//
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <¹Ù²ð User>
class User : public UserSession
{
	friend class UserManager;
	friend class SecureManager;
public:
	User();
	virtual ~User();

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <OBJECT CONTROL>
protected:
	virtual	VOID			Init();
	virtual	VOID			Release();
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <OBJECT CONTROL>
public:
    virtual eUSER_TYPE GetUserType() const { return UT_USER; };
	virtual const TCHAR*	GetClientIP()					{ return m_pszClientIP;	}
	virtual	VOID			SetClientIP( const TCHAR* ps )	{ memcpy( m_pszClientIP, ps, IP_MAX_LEN ); m_pszClientIP[IP_MAX_LEN]='\0';	}

	VOID					CopyInfo( TempUser* pUser );

	///
	MissionManager*			GetMissionManager()			{ return &m_MissionManager; }
	VOID					SetLogOutWait()				{ m_bLogOutWait = true; }
	ItemOwnFlag &			GetItemOwnFlag()			{ return m_ItemOwnFlag; }

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <NETWORK OBJECT><INHERITANCE>
protected:
	virtual	VOID			OnRecv( BYTE* pMsg, WORD wSize );
	virtual	VOID			OnAccept( DWORD dwNetworkIndex );
	virtual	VOID			OnDisconnect();

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <NETWORK OBJECT CONTROL>
public:
	virtual	BOOL			SendPacket( MSG_BASE* pMsg, WORD wSize, const BOOLEAN bUsePacking = TRUE );

	// TempUser´Â Forwarding µÇÁö ¾Ê´Â´Ù.
	inline	BOOL			SendToLinkedServer( MSG_BASE_FORWARD* pMsg, WORD wSize ) { return LinkedServer.SendToServer( pMsg, wSize ); }
	//

	inline	VOID			LinkServer( DWORD dwSessionIndex ) { LinkedServer.LinkUserWithServer( this, dwSessionIndex ); }
	//
	inline	VOID			UnlinkServer() { LinkedServer = LinkedServerSession(); }
	inline	ServerSessionEx* GetLinkedServer() { return LinkedServer.GetLinkedServer(); }

public:
	inline	DWORD			GetServerSessionIndex()		{ return LinkedServer.GetLinkedServerSessionIndex(); }

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <OBJECT STATUS>
public:
	inline	VOID			SetState( eUSER_STATE state ) { m_State = state; }
	inline	eUSER_STATE		GetState() { return m_State; }

	//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
    inline eBEHAVE_TRANSACTION GetTransactionState() const { return m_eTransactionState; }
    inline BOOL IsBeginTransaction() { return !(m_eTransactionState&TR_END); }
    inline VOID BeginTransaction(eBEHAVE_TRANSACTION tr) { ASSERT( m_eTransactionState&TR_END ); m_eTransactionState = tr;	}
    inline VOID EndTransaction() { ASSERT(m_eTransactionState & TR_BEGIN); m_eTransactionState = TR_END;	}
public:
    VOID SetBehaveState(ePLAYER_BEHAVE_STATE eState) { m_eBehaveState = eState; }
    inline ePLAYER_BEHAVE_STATE GetBehaveState() const { return m_eBehaveState; }
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    const nsPacket::ReAuthInfo& GetReAuthInfo() const { return m_ReAuthInfo; }
    BOOLEAN OnServerSelectQueryResult(nsPacket::ReAuthInfo::eReAuthSlot reAuthSlot,
                                      nsPacket::ReAuthInfoBase::eReAuthStep reAuthStep,
                                      const nsPacket::ReAuthTokenStream* arg);
    BOOLEAN OnServerSelectQueryUpdate();
#endif
//#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    // need settled info (billing type) (reserved value : PCRoomID)
    void NotifyPcBangBillingStatus(BOOLEAN send_to_dbproxy, BOOLEAN send_to_world);
//#endif

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <ZONE>
	inline	eZONETYPE		GetZoneType()		{ return m_eZoneType;	}
	inline	VOID			SetZoneType( eZONETYPE type )	{ m_eZoneType = type; }

	inline	eZONE_STATE		GetZoneState()		{ return m_eZoneState;	}
	inline	VOID			SetZoneState( eZONE_STATE eState ) { m_eZoneState = eState;	}
public:		BOOL			IsMovingBetweenServer();
public:
	inline	VOID			GetMoveZoneType( eZONETYPE& OUT FromZone, eZONETYPE& OUT ToZone, DWORD& OUT ToLinkServerIndex )	
							{ FromZone = m_eFromZoneType; ToZone = m_eToZoneType; ToLinkServerIndex = m_ToZoneLinkServerIndex; }
	inline	VOID			SetMoveZoneType( eZONETYPE FromZone, eZONETYPE ToZone, DWORD ToLinkServerIndex )		
							{ m_eFromZoneType = FromZone; m_eToZoneType = ToZone; m_ToZoneLinkServerIndex = ToLinkServerIndex; }

	//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
	inline	KEYTYPE			GetZoneKey() { return m_RoomKey;	}
	inline	KEYTYPE			GetTRRoomKey() { return m_TRRoomKey;	}
	inline	VOID			SetZoneKey( KEYTYPE key )	{ m_RoomKey = key;	}
	inline	VOID			SetTRRoomKey( KEYTYPE key )	{ m_TRRoomKey = key;	}

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <ROOM& PARTY>
	// ¹æ°ü·Ã Ã³¸®
	inline	VOID			SetReady( BOOL bReady )		{ m_bLobbyReady = bReady; }
	inline	BOOL			IsReady()					{ return m_bLobbyReady; }
	inline	VOID			SetMaster( BOOL bMaster )	{ m_bMaster = bMaster; }
	inline	BOOL			IsMaster()					{ return m_bMaster;	}
	inline	VOID			SetTeam( BYTE Team )		{ m_Team = Team; }
	inline	BYTE			GetTeam()					{ return m_Team; }
	inline	BOOL			IsExpired()					{ return m_TimeoutTimer.IsExpiredManual(FALSE);	}
	inline	VOID			StartTimer( DWORD val )		{ m_TimeoutTimer.SetTimer(val);	}
    // NOTE: f110622.2L
    bool NotifyHeIsHereIfSatisfied();
    void ChangeHeIsHereNotificationTimer(bool started);
	// ÆÄÆ¼ °ü·Ã
	inline	PartyState&		GetPartyState()				{ return m_PartyState; }
	VOID					SerializePartyInfo( _PARTY_MEMBER_INFO & MemberInfo );

	///
	//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
protected:	VOID			LeaveRoom();

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <SECURE ROUTINEs>
//   Cryptography& Sequence Number& Heartbeat& ...
public:
	inline	SecurePackNode4User&	GetSecureNode() { return SECUREINFO; }
	inline	CryptPack4Packet&	GetActiveCryptPacketNode() { return SECUREINFO.ACTIVE_CRYPT_PACKET_INFO(); }
	inline	HeartbeatChecker&	GetHeartbeatChecker() { return SECUREINFO.HEARTBEAT(); }
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    inline  TickSpeedChecker&   GetTickSpeedChecker() { return SECUREINFO.TICKSPEED(); }
#endif

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <PROPERTY><CHARACTER CONTROL>
public:
	//CharacterOperator		CHARINFO;
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <COMPATIBLE INTERFACEs for Prev-Version>><CHARACTER CONTROL>
    bool SelectChar(BYTE slot_index);
    BYTE GetSelectedCharIndex() const;
    void SetNumberOfCreatedChars(BYTE number_of_chars);
    BYTE GetNumberOfCreatedChars() const;
    DWORD GetCharGUID(BYTE slot_index) const;
    bool IsCharDelete(BYTE slot_index) const;

    // TempUser½Ã´Â Null, UserÀÏ ¶§ : ¼±ÅÃµÈ Ä³¸¯ÅÍ
    virtual BYTE GetCharClass() const;
    virtual LEVELTYPE GetCharLV() const;
    virtual const TCHAR* GetCharName() const;

    LEVELTYPE GetCharLV(BYTE slot_index) const;

    const TCHAR* GetCharName(BYTE slot_index) const;
    void SetCharName(BYTE slot_index, const TCHAR* char_name);
    // CHANGES: f110517.3L, added hash finding utility for char_name
    DWORD GetCharNameHash(BYTE slot_index) const;
    DWORD GetSelectedCharNameHash() const;
    const TCHAR* GetSelectedCharName() const;
    CHARGUID GetSelectedCharGuid() const;
    LEVELTYPE GetSelectedCharLV() const;
    BYTE GetSelectedCharClass() const;
    MAPCODE GetSelectedCharMapCode() const;
    void SetSelectedCharMapCode(MAPCODE map_code);
    GUILDGUID GetSelectedCharGuildGuid() const;
    void SetSelectedCharGuildGuid(GUILDGUID guild_guid);
    BYTE GetSelectedCharState() const;

    void SetSelectedCharMaxHP(DWORD map_hp);
    DWORD GetSelectedCharMaxHP() const;
    void SetSelectedCharCurHP(DWORD cur_hp);
    DWORD GetSelectedCharCurHP() const;
    bool IsDead() const;

    void SetSelectedCharMaxMP(DWORD max_mp);
    DWORD GetSelectedCharMaxMP() const;
    void SetSelectedCharCurMP(DWORD cur_mp);
    DWORD GetSelectedCharCurMP() const;

    void SetSelectedCharMaxSD(DWORD max_sd);
    DWORD GetSelectedCharMaxSD() const;
    void SetSelectedCharCurSD(DWORD cur_sd);
    DWORD GetSelectedCharCurSD() const;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    void SetSelectedCharMaxFP(PLAYERFP max_fp);
    PLAYERFP GetSelectedCharMaxFP() const;
    void SetSelectedCharCurFP(PLAYERFP cur_fp);
    PLAYERFP GetSelectedCharCurFP() const;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    void SetSelectedCharChaoState(BYTE chao_state);
    BYTE GetSelectedCharChaoState() const;
    bool IsChaoState() const;

    void SetCharLevel(BYTE slot_index, LEVELTYPE level);

    void SetCharDelChk(BYTE slot_index, BYTE del_check);
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE GetSelectedCharGender() const;
#endif

    void SerializeCharacterPart(BYTE slot_index, SERVER_CHARACTER_PART& INOUT parts, eSERIALIZE se);
    void LoadClientCharacterPart(BYTE slot_index, CLIENT_CHARACTER_PART* const result_parts) const;
    // À¯È¿ÇÑ ½½·ÔÀÎÁö ¿©ºÎ Ã³¸®
    void SetOccupiedSlot(BYTE slot_index, bool set_value);
    bool IsValidSlot(BYTE slot_index) const;
	//
	//
	//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
	//
	//
	//
public:		VOID			SetLockItemPosForInstanceDungeon( POSTYPE Pos ){ m_LockItemPosForInstanceDungeon = Pos; }
			POSTYPE			GetLockItemPosForInstanceDungeon(){ return m_LockItemPosForInstanceDungeon; }
    inline  VOID            SetReservedHP(DWORD reservedHP) { m_ReservedHP = reservedHP; };
    inline  DWORD           GetReservedHP() const { return m_ReservedHP; };
    inline  VOID            SetNeedChangedStatus(PlayerStatEvent::eFlag statEvent);
    inline  BYTE            GetNeedChangedStatus() const;
    inline  PortalUserInfo& GetPortalUserInfo() { return m_PortalUserInfo; }
public:
	//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
	// °æÀïÇåÆÃ ÀüÀû
	inline	VOID			SetSelectedCharCHuntingVCount( DWORD dwVCount )		{ m_dwCHuntingVCount = dwVCount; }
	inline	DWORD			GetSelectedCharCHuntingVCount()						{ return m_dwCHuntingVCount; }

	inline	VOID			SetSelectedCharCHuntingTotCount( DWORD dwTCount )	{ m_dwCHuntingTotCount = dwTCount; }
	inline	DWORD			GetSelectedCharCHuntingTotCount()					{ return m_dwCHuntingTotCount; }
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
			VOID			DisconnectLinkedServerUser();
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    inline VOID             SetIsRecvInvitePartyRoom(BOOL is_recv_invite){m_isInvaitPartyRoom = is_recv_invite;}
    inline BOOL             GetIsRecvInvitePartyRoom()const{return m_isInvaitPartyRoom;}

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    inline void SetGroupKey(const WORD group_key) { group_key_ = group_key; }
    inline WORD GetGroupKey() { return group_key_; }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	inline	VOID			SetAge( BYTE byAge ){ m_byAge = byAge; }
	inline	BYTE			GetAge(){ return m_byAge; }
	inline	VOID			SetRealNameAuth( BYTE byRealNameAuth ){ m_byRealNameAuth = byRealNameAuth; }
	inline	BYTE			GetRealNameAuth(){ return m_byRealNameAuth; }
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE  

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    ShopUserQuerySlot&      GetShopUserQuerySlot() { return m_ShopUserQuerySlot; }
#endif
#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
    // NOTE: f110916.1L, added by _NA002676_WORLD_SERVER_RENEWAL
    void ChangeWorldServiceEnabledState(bool enabled);
    bool EnabledWorldService() const;

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <FIELDs>
//
	virtual VOID			DisconnectUser( BOOL bGracefulDisconnect = TRUE );

private:
	VOID					_DisconnectUser();

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <SECURITY>
private:
    struct
    {
        SecurePackNode4User		SECUREINFO;				// ¾ÏÈ£È­ °ü·Ã Á¤º¸ ±¸Á¶Ã¼
    };

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <SESSION CONTROL>
private:
    struct
    {
        bool m_bLogOutWait;             // Å¬¶ó°¡ ·Î±×¾Æ¿ô ÇÏ°Ú´Ù°í ÀÇµµÇÑ »óÅÂ
        bool m_bExecuteDisconnectUser;  // _DisconnectUser()¸¦ Ã³¸®Çß´ÂÁö ¿©ºÎ
        bool world_service_enabled_state_; // NOTE: f110916.1L, _NA002676_WORLD_SERVER_RENEWAL
    };
//	<PROPERTY>
public:
    LinkedServerSession         LinkedServer;
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
//	<STATE>
private:
    struct
    {
        eUSER_STATE             m_State;
        eBEHAVE_TRANSACTION     m_eTransactionState;
        ePLAYER_BEHAVE_STATE    m_eBehaveState;
    };

//	<PROPERTY><Å¸¶ôÇÑ »ç¿ø>
public:
    ISSQHandlerD    SSQHandler;
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
//	<CHARACTER INFO>
private:
    ns_object::CharacterList character_list_;
    //sUSER_CHAR_INFO m_CHARINFO;
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
//	<ZONE>
private:
    struct
    {
        eZONETYPE   m_eZoneType;
        eZONE_STATE m_eZoneState;
        eZONETYPE   m_eFromZoneType;	// 1. Á¸ °£ÀÇ ÀÌµ¿½Ã ¾î´ÀÁ¸¿¡¼­ ¾î´ÀÁ¸À¸·Î °¡´Â°ÇÁö
        eZONETYPE   m_eToZoneType;		// 2. Á¸ °£ÀÇ ÀÌµ¿½Ã ¾î´ÀÁ¸¿¡¼­ ¾î´ÀÁ¸À¸·Î °¡´Â°ÇÁö
        DWORD       m_ToZoneLinkServerIndex;
        KEYTYPE     m_RoomKey;
        KEYTYPE     m_TRRoomKey;
        PortalUserInfo m_PortalUserInfo;
    #ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
        nsPacket::ReAuthInfo  m_ReAuthInfo;
    #endif
        //__NA001336_090514_PARTY_NAVI_SUPPORT__
        DWORD       m_ReservedHP;       // ÀÌ °ªÀº µ¿ÀÏÇÏ°Ô »ç¿ëÇÏ±â·Î ÇÑ´Ù.
        BYTE        m_PlayerStateEvent; // PlayerStatEvent
        // NOTE: f110622.2L, the below field can be used in case of need without no session links.
        util::ITimerBase  he_is_in_char_select_timer_;
    };
//	ºñÁ¤»óÀû Á¾·á·Î ÀÎÇØ ÀçÀÔÀå½Ã ±âÁ¸ ¹æÀ¸·Î µ¹¾Æ°¡´Â °æ¿ì
public:
    ClosingUserTransController closing_user_trans_controller_;

private:
    ClosingUserTransInfo closing_user_trans_info_;

//	ºñÁ¤»óÀû Á¾·á½Ã À¯Àú Á¤º¸ ÀúÀå
public:
    PartyReturnInfo&    GetPartyReturnInfo() { return m_PartyReturnInfo; }
private:
    PartyReturnInfo     m_PartyReturnInfo;

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <ROOM& PARTY>
private:
    struct
    {
        BOOL    m_bLobbyReady;
        BOOL    m_bMaster;
        BYTE    m_Team;

        PartyState  m_PartyState;
        ItemOwnFlag m_ItemOwnFlag;			// ¾ÆÀÌÅÛ ¼ÒÀ¯»óÅÂ(½´ÆÛ¹æÀå...)

        util::Timer m_TimeoutTimer;
    };

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <MISSION>
private:
    struct
    {
        MissionManager m_MissionManager;
    };
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <INSTANCE DUNGEON>
private:
    struct
    {
        POSTYPE m_LockItemPosForInstanceDungeon;		//ÀÎ´øµé¾î°¥ ¶§ Lock°É ¾ÆÀÌÅÛ(ÇÊ¿ä ¾øÀ» °æ¿ìµµ ÀÖÀ½)
    };
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <COMPETITION HUNTING>
private:
    struct
    {
        // °æÀïÇåÆÃ ÀüÀû
        DWORD   m_dwCHuntingVCount;		// °æÀïÇåÆÃ ½Â¸®È½¼ö
        DWORD   m_dwCHuntingTotCount;	// °æÀïÇåÆÃ Âü¿©È½¼ö
    };
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// <APPENDIX INFO>
private:
    TCHAR   m_pszClientIP[IP_MAX_LEN+1];		// Å¬¶óÀÌ¾ðÆ® ¾ÆÀÌÇÇ
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    BYTE    m_byAge;
    BYTE    m_byRealNameAuth;
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        ShopUserQuerySlot m_ShopUserQuerySlot;
    #endif
#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    BOOL                    m_isInvaitPartyRoom;

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        WORD group_key_;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND


}; //end class User



#include "User.Addin.h"



