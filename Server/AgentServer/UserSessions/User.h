#pragma once

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
#include "UserInfo.Secure.h"

//
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <夥莞 User>
class User : public UserSession
{
	friend class UserManager;
	friend class SecureManager;
public:
	User();
	virtual ~User();

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <OBJECT CONTROL>
protected:
	virtual	VOID			Init();
	virtual	VOID			Release();
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <NETWORK OBJECT><INHERITANCE>
protected:
	virtual	VOID			OnRecv( BYTE* pMsg, WORD wSize );
	virtual	VOID			OnAccept( DWORD dwNetworkIndex );
	virtual	VOID			OnDisconnect();

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <NETWORK OBJECT CONTROL>
public:
	virtual	BOOL			SendPacket( MSG_BASE* pMsg, WORD wSize, const BOOLEAN bUsePacking = TRUE );

	// TempUser朝 Forwarding 腎雖 彊朝棻.
	inline	BOOL			SendToLinkedServer( MSG_BASE_FORWARD* pMsg, WORD wSize ) { return LinkedServer.SendToServer( pMsg, wSize ); }
	//

	inline	VOID			LinkServer( DWORD dwSessionIndex ) { LinkedServer.LinkUserWithServer( this, dwSessionIndex ); }
	//
	inline	VOID			UnlinkServer() { LinkedServer = LinkedServerSession(); }
	inline	ServerSessionEx* GetLinkedServer() { return LinkedServer.GetLinkedServer(); }

public:
	inline	DWORD			GetServerSessionIndex()		{ return LinkedServer.GetLinkedServerSessionIndex(); }

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <OBJECT STATUS>
public:
	inline	VOID			SetState( eUSER_STATE state ) { m_State = state; }
	inline	eUSER_STATE		GetState() { return m_State; }

	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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

	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
	inline	KEYTYPE			GetZoneKey() { return m_RoomKey;	}
	inline	KEYTYPE			GetTRRoomKey() { return m_TRRoomKey;	}
	inline	VOID			SetZoneKey( KEYTYPE key )	{ m_RoomKey = key;	}
	inline	VOID			SetTRRoomKey( KEYTYPE key )	{ m_TRRoomKey = key;	}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <ROOM& PARTY>
	// 寞婦溼 籀葬
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
	// だじ 婦溼
	inline	PartyState&		GetPartyState()				{ return m_PartyState; }
	VOID					SerializePartyInfo( _PARTY_MEMBER_INFO & MemberInfo );

	///
	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
protected:	VOID			LeaveRoom();

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <SECURE ROUTINEs>
//   Cryptography& Sequence Number& Heartbeat& ...
public:
	inline	SecurePackNode4User&	GetSecureNode() { return SECUREINFO; }
	inline	CryptPack4Packet&	GetActiveCryptPacketNode() { return SECUREINFO.ACTIVE_CRYPT_PACKET_INFO(); }
	inline	HeartbeatChecker&	GetHeartbeatChecker() { return SECUREINFO.HEARTBEAT(); }
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    inline  TickSpeedChecker&   GetTickSpeedChecker() { return SECUREINFO.TICKSPEED(); }
#endif

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <PROPERTY><CHARACTER CONTROL>
public:
	//CharacterOperator		CHARINFO;
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <COMPATIBLE INTERFACEs for Prev-Version>><CHARACTER CONTROL>
    bool SelectChar(BYTE slot_index);
    BYTE GetSelectedCharIndex() const;
    void SetNumberOfCreatedChars(BYTE number_of_chars);
    BYTE GetNumberOfCreatedChars() const;
    DWORD GetCharGUID(BYTE slot_index) const;
    bool IsCharDelete(BYTE slot_index) const;

    // TempUser衛朝 Null, User橾 陽 : 摹鷗脹 議葛攪
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
    // 嶸?褲? 蝸煜檣雖 罹睡 籀葬
    void SetOccupiedSlot(BYTE slot_index, bool set_value);
    bool IsValidSlot(BYTE slot_index) const;
	//
	//
	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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
	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
	// 唳爵ьた 瞪瞳
	inline	VOID			SetSelectedCharCHuntingVCount( DWORD dwVCount )		{ m_dwCHuntingVCount = dwVCount; }
	inline	DWORD			GetSelectedCharCHuntingVCount()						{ return m_dwCHuntingVCount; }

	inline	VOID			SetSelectedCharCHuntingTotCount( DWORD dwTCount )	{ m_dwCHuntingTotCount = dwTCount; }
	inline	DWORD			GetSelectedCharCHuntingTotCount()					{ return m_dwCHuntingTotCount; }
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <FIELDs>
//
	virtual VOID			DisconnectUser( BOOL bGracefulDisconnect = TRUE );

private:
	VOID					_DisconnectUser();

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <SECURITY>
private:
    struct
    {
        SecurePackNode4User		SECUREINFO;				// 懍???? 婦溼 薑爾 掘褻羹
    };

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <SESSION CONTROL>
private:
    struct
    {
        bool m_bLogOutWait;             // 贗塭陛 煎斜嬴醒 ж啊棻堅 曖紫и 鼻鷓
        bool m_bExecuteDisconnectUser;  // _DisconnectUser()蒂 籀葬ц朝雖 罹睡
        bool world_service_enabled_state_; // NOTE: f110916.1L, _NA002676_WORLD_SERVER_RENEWAL
    };
//	<PROPERTY>
public:
    LinkedServerSession         LinkedServer;
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
//	<STATE>
private:
    struct
    {
        eUSER_STATE             m_State;
        eBEHAVE_TRANSACTION     m_eTransactionState;
        ePLAYER_BEHAVE_STATE    m_eBehaveState;
    };

//	<PROPERTY><顫塊и 餌錳>
public:
    ISSQHandlerD    SSQHandler;
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
//	<CHARACTER INFO>
private:
    ns_object::CharacterList character_list_;
    //sUSER_CHAR_INFO m_CHARINFO;
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
//	<ZONE>
private:
    struct
    {
        eZONETYPE   m_eZoneType;
        eZONE_STATE m_eZoneState;
        eZONETYPE   m_eFromZoneType;	// 1. 襄 除曖 檜翕衛 橫替襄縑憮 橫替襄戲煎 陛朝勒雖
        eZONETYPE   m_eToZoneType;		// 2. 襄 除曖 檜翕衛 橫替襄縑憮 橫替襄戲煎 陛朝勒雖
        DWORD       m_ToZoneLinkServerIndex;
        KEYTYPE     m_RoomKey;
        KEYTYPE     m_TRRoomKey;
        PortalUserInfo m_PortalUserInfo;
    #ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
        nsPacket::ReAuthInfo  m_ReAuthInfo;
    #endif
        //__NA001336_090514_PARTY_NAVI_SUPPORT__
        DWORD       m_ReservedHP;       // 檜 高擎 翕橾ж啪 餌辨ж晦煎 и棻.
        BYTE        m_PlayerStateEvent; // PlayerStatEvent
        // NOTE: f110622.2L, the below field can be used in case of need without no session links.
        util::ITimerBase  he_is_in_char_select_timer_;
    };
//	綠薑鼻瞳 謙猿煎 檣п 營殮濰衛 晦襄 寞戲煎 給嬴陛朝 唳辦
public:
    ClosingUserTransController closing_user_trans_controller_;

private:
    ClosingUserTransInfo closing_user_trans_info_;

//	綠薑鼻瞳 謙猿衛 嶸盪 薑爾 盪濰
public:
    PartyReturnInfo&    GetPartyReturnInfo() { return m_PartyReturnInfo; }
private:
    PartyReturnInfo     m_PartyReturnInfo;

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <ROOM& PARTY>
private:
    struct
    {
        BOOL    m_bLobbyReady;
        BOOL    m_bMaster;
        BYTE    m_Team;

        PartyState  m_PartyState;
        ItemOwnFlag m_ItemOwnFlag;			// 嬴檜蠱 模嶸鼻鷓(蓬ぷ寞濰...)

        util::Timer m_TimeoutTimer;
    };

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <MISSION>
private:
    struct
    {
        MissionManager m_MissionManager;
    };
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <INSTANCE DUNGEON>
private:
    struct
    {
        POSTYPE m_LockItemPosForInstanceDungeon;		//檣湍菟橫陞 陽 Lock勘 嬴檜蠱(в蹂 橈擊 唳辦紫 氈擠)
    };
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <COMPETITION HUNTING>
private:
    struct
    {
        // 唳爵ьた 瞪瞳
        DWORD   m_dwCHuntingVCount;		// 唳爵ьた 蝓葬?蝦?
        DWORD   m_dwCHuntingTotCount;	// 唳爵ьた 霤罹?蝦?
    };
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// <APPENDIX INFO>
private:
    TCHAR   m_pszClientIP[IP_MAX_LEN+1];		// 贗塭檜樹お 嬴檜Я
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



