#pragma once

// <HEADERs>
#include "UserInfo.h"

#ifdef _NA_20100307_BETAKEY_SYSTEM
#include <BetaKey.h>
#endif

// <PROTOTYPEs>
class ServerSessionEx;

// <DEFINITIONs>
class UserSession : public NetworkObject
{
	friend class UserManager;
public:
	UserSession();
	virtual ~UserSession();

//////////////////////////////////////////////////////////////////////////
// <OBJECT CONTROL>
public:
	virtual VOID		Init();
	virtual VOID		Release();
    virtual eUSER_TYPE GetUserType() const = 0;
	//virtual eUSER_TYPE	GetUserTypeBySecureControl()	{ return eUSER_CONTROL_NONE; }
protected:

// <NETWORK OBJECT CONTROL>
public:
	BOOL				SendToDBPServer( MSG_BASE* pMsg, WORD wSize );
	BOOL				SendToWorldServer( MSG_BASE* pMsg, WORD wSize );
	BOOL				SendToGuildServer( MSG_BASE* pMsg, WORD wSize );
	inline DWORD		GetSessionIndex()					{ return m_dwSessionIndex; }
	inline VOID			SetSessionIndex( DWORD dwIndex )	{ m_dwSessionIndex = dwIndex; }

//////////////////////////////////////////////////////////////////////////
// <NETWORK OBJECT><INHERITANCE>
protected:
	virtual VOID		OnDisconnect();
	virtual VOID		OnAccept( DWORD dwNetworkIndex );
	virtual VOID		OnRecv( BYTE* pMsg, WORD wSize ) = 0;
	virtual VOID		OnLogString( CHAR* pszLog );

	VOID				OnDisconnectInfoToAuth();
public:
	virtual	BOOL		SendPacket( MSG_BASE* pMsg, WORD wSize, const BOOLEAN bUsePacking = TRUE );

//////////////////////////////////////////////////////////////////////////
// <NETWORK OBJECT DISPOSING CONTROL>
public:
	inline VOID			SetDisconnectCode( RC::eCONNECTION_RESULT eDisconnectReason ) { m_eDisconnectReason = eDisconnectReason; }

	virtual VOID		DisconnectUser( BOOL bGracefulDisconnect = TRUE );

	eAUTH_LOGOUT_REASON				GetAuthLogoutType();
	eLOG_SESSION_LOGOUT_TYPE		GetLogoutType();
#ifdef _NA_008765_20160405_DISCONNECTION_REASON_LOG
	RC::eCONNECTION_RESULT			GetLogoutType2();
#endif //(_NA_008765_20160405_DISCONNECTION_REASON_LOG)
protected:
	inline RC::eCONNECTION_RESULT	GetDisconnectCode() { return m_eDisconnectReason; }

//////////////////////////////////////////////////////////////////////////
// <PROPERTIES>
public:
// <PROPERTIES> <NETWORK SESSION CONTROL>
	inline BOOL&		DisconnectProcessing()	{ return m_bDisconnectProcessing; }

    //----------------------------------------------------------------------------------------------
    const ns_object::UserAccountInfo& GetUserAccountInfo() const;
    //
    void  SetUserKey(DWORD dwKey);
    DWORD GetUserKey() const;
    void  SetUserGUID(DWORD dwGUID);
    DWORD GetUserGUID() const;
    void  SetAuthID(DWORD dwID);
    DWORD GetAuthID() const;
    void  SetPlayerKey(DWORD object_key); // for 'User', not 'TempUser'
    DWORD GetPlayerKey() const;
    void  SetUserID(const TCHAR* psUserID);
    const TCHAR* GetUserID() const;

    void  SetLastSSNKey(const BYTE* pbyLastSSNKey);
    const BYTE* GetLastSSNKey() const;
    bool  IsEqualLastSSN(const BYTE* pbyLastSSNKey);

    //----------------------------------------------------------------------------------------------
    // <USER LOGIN TIME INFO>
    void SetLoginTime(const TCHAR* pStrTM);
    void SetCharLoginTime(const TCHAR* pStrTM);
    void SetLoginTime_t(time_t t);
    void SetCharLoginTime_t(time_t t);

    const TCHAR* GetLoginTime() const;
    const TCHAR* GetCharLoginTime() const;
    time_t GetLoginTime_t() const;
    time_t GetCharLoginTime_t() const;

    // <USER LOGIN TIME INFO><LIFETIME TRANSACTION>
    void WriteLoginTime();
    void BeginCharLoginTime();
    void EndCharLoginTime();
    //----------------------------------------------------------------------------------------------

	// (WAVERIX)(WARNING)
	// 현재 TCHAR와 CHAR의 구분이 전혀 없다. 이렇게 하자.
	inline const TCHAR*	GetClientIP()						{ return (TCHAR*)GetIP(); }

    // Added by _PCBANG_POLICY_CHANGE_POST_EXPIRED
    inline void         SetBillingType(const CHANGE_PCROOM_STATUS& status) { m_BillingType = status; }
    inline CHANGE_PCROOM_STATUS GetBillingType() const { return m_BillingType; };
	inline VOID			SetReservedValue( DWORD dwReservedValue )	{ m_dwReservedValue = dwReservedValue; }
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    inline DWORD        GetReservedValue(BOOLEAN raw_value = 0) const;
#else
	inline DWORD		GetReservedValue(BOOLEAN raw_value = 0) { return m_dwReservedValue; } 
#endif
    //!~ _NA_006413_20130201_PREMIUMSERVICE_ITEM
    void                SetPremiumService(DWORD premiumservice);
    DWORD               GetPremiumService();
    //~! _NA_006413_20130201_PREMIUMSERVICE_ITEM
    inline VOID         SetAuthUserType(BYTE authUserType) { m_AuthUserType = authUserType; }
    inline BYTE         GetAuthUserType() const { return m_AuthUserType; }
	BOOL				CanApplyPCBangBenefit( BOOL bNationCheck );
	inline VOID			SetSts( BYTE bySts )		{ m_bySts = bySts; }
	inline BYTE			GetSts()					{ return m_bySts; } 
    inline BOOL         IsGM() const { return (m_bySts > eGM_GRADE_BASE) && (m_bySts < eGM_GRADE_MAX); }

	inline VOID			SetPCRoomSts( BYTE bySts )	{ m_PCRoomSts = bySts; }
	inline BYTE 		GetPCRoomSts()				{ return m_PCRoomSts; }
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN          GetGameChuUSN() const           {return gamechu_usn_;};
    VOID                SetGameChuUSN(GAMECHUUSN gamechu_usn){gamechu_usn_ = gamechu_usn;};
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BetaKeyInfo*        const GetBetaKeyInfo()       {return &betakey_info_;};
    VOID                SetBetaKey(BETAKEY betakey)  {betakey_info_ = betakey;};
#endif
//////////////////////////////////////////////////////////////////////////
// <CHARACTER INFO>
	// TempUser시는 Null, User일 때 : 선택된 캐릭터
	virtual BYTE			GetCharClass() const	{ return 0; }
	virtual LEVELTYPE		GetCharLV() const		{ return 0; }
	virtual const TCHAR*	GetCharName() const	{ return NULL; }

private:
	DWORD					m_dwSessionIndex;
	BOOL					m_bDisconnectProcessing;
	RC::eCONNECTION_RESULT	m_eDisconnectReason;
	ns_object::UserAccountInfo user_account_info_; //m_USERINFO;
	// Addin-INFO
	DWORD					m_dwReservedValue;
    DWORD                   m_dwPremiumService;
    CHANGE_PCROOM_STATUS    m_BillingType;  //CHANGE_PCROOM_STATUS::eStatus, _PCBANG_POLICY_CHANGE_POST_EXPIRED
    BYTE                    m_AuthUserType;
	BYTE					m_PCRoomSts;				// PC방 블락상태
	BYTE					m_bySts;					// DB 저장되는 유저 상태값(100 이상은 GM)
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_    
    GAMECHUUSN              gamechu_usn_;
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BetaKeyInfo             betakey_info_;
#endif
};	//class UserSession

//==================================================================================================
//==================================================================================================
//==================================================================================================

inline const ns_object::UserAccountInfo& UserSession::GetUserAccountInfo() const
{
    return user_account_info_;
}

inline void UserSession::SetUserKey(DWORD user_key)
{
    user_account_info_.KEY_INFO.user_guid = user_key;
};

inline DWORD UserSession::GetUserKey() const
{
    return user_account_info_.KEY_INFO.user_guid;
};

inline void UserSession::SetUserGUID(DWORD user_guid)
{
    user_account_info_.KEY_INFO.user_guid = user_guid;
};

inline DWORD UserSession::GetUserGUID() const
{
    return user_account_info_.KEY_INFO.user_guid;
};

inline void UserSession::SetAuthID(DWORD auth_id)
{
    user_account_info_.KEY_INFO.auth_id = auth_id;
};

inline DWORD UserSession::GetAuthID() const
{
    return user_account_info_.KEY_INFO.auth_id;
};

inline void UserSession::SetPlayerKey(DWORD object_key)
{
    user_account_info_.KEY_INFO.object_key = object_key;
}

inline DWORD UserSession::GetPlayerKey() const
{
    return user_account_info_.KEY_INFO.object_key;
};

inline void UserSession::SetUserID(const TCHAR* user_id)
{
    _tcsncpy(user_account_info_.ID_INFO.user_id, user_id,
             _countof(user_account_info_.ID_INFO.user_id));
    user_account_info_.ID_INFO.user_id[_countof(user_account_info_.ID_INFO.user_id) - 1] = _T('\0');
};

inline const TCHAR* UserSession::GetUserID() const
{
    return user_account_info_.ID_INFO.user_id;
};


inline void UserSession::SetLastSSNKey(const BYTE* ssn_string)
{
    CopyMemory(user_account_info_.ID_INFO.ssn_string, ssn_string,
               sizeof(user_account_info_.ID_INFO.ssn_string));
};

inline const BYTE* UserSession::GetLastSSNKey() const
{
    return user_account_info_.ID_INFO.ssn_string;
};

inline bool  UserSession::IsEqualLastSSN(const BYTE* ssn_string)
{
    return user_account_info_.IsEqualLastSSN(ssn_string);
};


//=================================================================================================
// <USER LOGIN TIME INFO>
inline void UserSession::SetLoginTime(const TCHAR* login_time)
{
    _tcsncpy(user_account_info_.LOGIN_INFO.login_time_string, login_time,
             _countof(user_account_info_.LOGIN_INFO.login_time_string));
    user_account_info_.LOGIN_INFO.login_time_string[
        _countof(user_account_info_.LOGIN_INFO.login_time_string) - 1] = _T('\0');
};

inline void UserSession::SetCharLoginTime(const TCHAR* char_login_time)
{
    _tcsncpy(user_account_info_.LOGIN_INFO.char_login_time_string, char_login_time,
             _countof(user_account_info_.LOGIN_INFO.char_login_time_string));
    user_account_info_.LOGIN_INFO.char_login_time_string[
        _countof(user_account_info_.LOGIN_INFO.char_login_time_string) - 1] = _T('\0');
};

inline void UserSession::SetLoginTime_t(time_t login_time)
{
    user_account_info_.LOGIN_INFO.login_time = login_time;
};

inline void UserSession::SetCharLoginTime_t(time_t char_login_time)
{
    user_account_info_.LOGIN_INFO.char_login_time = char_login_time;
};


inline const TCHAR* UserSession::GetLoginTime() const
{
    return user_account_info_.LOGIN_INFO.login_time_string;
};

inline const TCHAR* UserSession::GetCharLoginTime() const
{
    return user_account_info_.LOGIN_INFO.char_login_time_string;
};

inline time_t UserSession::GetLoginTime_t() const
{
    return user_account_info_.LOGIN_INFO.login_time;
};

inline time_t UserSession::GetCharLoginTime_t() const
{
    return user_account_info_.LOGIN_INFO.char_login_time;
};


// <USER LOGIN TIME INFO><LIFETIME TRANSACTION>
inline void UserSession::WriteLoginTime()
{
    user_account_info_.WriteLoginTime(true);
};

inline void UserSession::BeginCharLoginTime()
{
    user_account_info_.WriteLoginTime(false);
};

inline void UserSession::EndCharLoginTime()
{
    user_account_info_.LOGIN_INFO.char_login_time = 0;
    ZeroMemory(user_account_info_.LOGIN_INFO.char_login_time_string,
               sizeof(user_account_info_.LOGIN_INFO.char_login_time_string));
};


#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
// (Arguments)
//  raw_value == false : complex operation with billing_type
//  raw_value != false : get m_dwReservedValue
inline DWORD
UserSession::GetReservedValue(BOOLEAN raw_value) const
{
    // (Status Table)
    //                    status.eStatus_None | status.eStatus_PcRoom | status.eStatus_PcRoomExpired
    //GetReservedValue()          0                      id                           0

    if(raw_value == 0) {
        return (m_BillingType.status_ == m_BillingType.eStatus_PcRoom)
               ?    m_dwReservedValue
               :    0;
    }

    return m_dwReservedValue;
}
#endif

