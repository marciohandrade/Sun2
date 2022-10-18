#pragma once

#include "timeobject.h"

class ConnectionTimeout : public TimeObject
{
public:
	ConnectionTimeout();
	virtual ~ConnectionTimeout();

	virtual VOID	OnStart();
	virtual VOID	OnExpired();

	VOID			SetUserGUID( DWORD dwGUID ) { m_dwUserGUID = dwGUID;	}
	DWORD			GetUserGUID()	{ return m_dwUserGUID; }

	VOID			SetAuthUserID( DWORD dwAuthUserID ) { m_dwAuthUserID = dwAuthUserID; }
	DWORD			GetAuthUserID() { return m_dwAuthUserID; }

	VOID			SetUserID( const char * ps ) { memcpy( m_pszID, ps, ID_MAX_LEN ); m_pszID[ID_MAX_LEN]='\0'; }
	const char*		GetUserID() { return m_pszID;	}

	VOID			SetClientIP( const char * ps ) { memcpy( m_pszClientIP, ps, IP_MAX_LEN ); m_pszClientIP[IP_MAX_LEN]='\0';	}
	const char*		GetClientIP() { return m_pszClientIP;	}

    // reference : CHANGE_PCROOM_STATUS
	VOID			SetBillingType(const CHANGE_PCROOM_STATUS& byBillingType ) { m_BillingType = byBillingType; }
	CHANGE_PCROOM_STATUS GetBillingType() { return m_BillingType; }

	VOID			SetBillingInfo( const char *ps ) { memcpy( m_szBillingInfo, ps, INFO_MAX_LEN); m_szBillingInfo[INFO_MAX_LEN]='\0';	}
	const char*		GetBillingInfo() { return m_szBillingInfo; }

	VOID			SetReservedValue( DWORD dwReservedValue ) { m_dwReservedValue = dwReservedValue; }
	DWORD			GetReservedValue() { return m_dwReservedValue; }
    void            SetAuthUserType(BYTE authUserType) { m_AuthUserType = authUserType; }
    BYTE            GetAuthUserType() const { return m_AuthUserType; }

	VOID			SetSerialKey(const BYTE *ps) { memcpy(m_szSerialKey, ps, AUTH_SERIAL_KEY_LEN); }
	BYTE*			GetSerialKey() { return m_szSerialKey; }

	BYTE*			GetLastSSN_Key() { return m_LastSSN_Key; }
    VOID			SetLastSSNKey( BYTE LastSSNKey[kCryptStringSize] ) 
    {
        memcpy(m_LastSSN_Key, LastSSNKey, kCryptStringSize);	
    }

	BYTE			GetSts() { return m_bySts; }
	VOID			SetSts( BYTE bySts ) { m_bySts = bySts; }

	VOID			SetPCRoomSts( BYTE bySts )	{ m_PCRoomSts = bySts; }
	BYTE 			GetPCRoomSts()				{ return m_PCRoomSts; } 

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	VOID			SetAge( BYTE byAge ){ m_byAge = byAge; }
	BYTE			GetAge(){ return m_byAge; }
	VOID			SetRealNameAuth( BYTE byRealNameAuth ){ m_byRealNameAuth = byRealNameAuth; }
	BYTE			GetRealNameAuth(){ return m_byRealNameAuth; }
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN      GetGameChuUSN(){return gamechu_usn_;};
    VOID            SetGameChuUSN(GAMECHUUSN gamechu_usn){gamechu_usn_ = gamechu_usn;};
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BETAKEY         GetBetaKey(){return betakey_;};
    VOID            SetBetaKey(BETAKEY betakey){betakey_ = betakey;};
#endif

private:

	DWORD	m_dwAuthUserID;						// 인증서버에서 발급한 아이디
	DWORD	m_dwUserGUID;						// DB GUID
	char	m_pszID[ID_MAX_LEN+1];				// 계정
	char	m_pszClientIP[IP_MAX_LEN+1];		// 클라이언트 아이피
	CHANGE_PCROOM_STATUS m_BillingType;					// 빌링 타입
	char	m_szBillingInfo[INFO_MAX_LEN+1];	// 빌링 정보
	DWORD	m_dwReservedValue;					// 예약 { [1, ~) PC방 }
    BYTE    m_AuthUserType;                     // eAUTH_USER_TYPE
	BYTE	m_szSerialKey[AUTH_SERIAL_KEY_LEN]; // 접속 시리얼키
    BYTE	m_LastSSN_Key[kCryptStringSize];
	BYTE	m_bySts;							// DB에 저장되는 사용자 Sts값. 100 이상은 GM 등급
	BYTE	m_PCRoomSts;						// PC방 블락상태
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	BYTE	m_byAge;							// 사용자 연령
	BYTE	m_byRealNameAuth;					// 실명인증 ( 0 : 비실명, 1 : 실명 )
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_    
    GAMECHUUSN  gamechu_usn_;
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BETAKEY betakey_;
#endif
};















