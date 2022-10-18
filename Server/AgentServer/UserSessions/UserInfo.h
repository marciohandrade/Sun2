#pragma once
#ifndef AGENTSERVER_USER_INFO_H
#define AGENTSERVER_USER_INFO_H

#include "UserInfo.Structures.h"
//
//==================================================================================================
//
//struct sUSERINFO_NODE
//{
//    //--------------------------------------------
//    // <CONTROL LIST FIELDs>
//    sUSERINFO_NODE*     next;
//    sUSERINFO_NODE*     prev;
//};
namespace ns_object {
;

struct UserAccountInfo
{
    //--------------------------------------------
    // <USER KEY INFO>
    //
    struct
    {
        DWORD user_guid; // User의 GUID
        DWORD auth_id; // Auth Server로부터 받은 사용자 키
        DWORD object_key; // 플레이시 사용되는 Sequential issued key (this lifetime equals it of session)
    } KEY_INFO;

    //--------------------------------------------
    // <USER ACCOUNT INFO>
    struct
    {
        TCHAR user_id[ID_MAX_LEN + 1]; // 계정
        BYTE  ssn_string[kCryptStringSize]; // 주민등록번호 뒷자리
    } ID_INFO;

    //--------------------------------------------
    // <USER LOGIN INFO>
    struct
    {
        time_t login_time;
        time_t char_login_time;
        TCHAR login_time_string[MAX_TIMEDATA_SIZE + 1];
        TCHAR char_login_time_string[MAX_TIMEDATA_SIZE + 1];
    } LOGIN_INFO;

    void WriteLoginTime(bool is_account);
    bool IsEqualLastSSN(const BYTE* pLastSSNKey);
};

}; //end of namespace
#if SUN_CODE_BACKUP
class UserInfoOperator
{
private:
    sUSERINFO&  m_rUSERINFO;

    // IP정보는 SessionIndex로 해결해야 하나 말아야 하나?
public:
    UserInfoOperator( sUSERINFO& rUSERINFO ) : m_rUSERINFO(rUSERINFO)
    {
        Init();
    }

    //============================================
    // <METHODs>
    //

    // UserGUID와 UserKey는 동일한 것이다.
    //inline VOID           SetUserKey( DWORD dwKey )           { m_rUSERINFO.KEY_INFO.dwUserGUID = dwKey; }
    //inline DWORD      GetUserKey() const                  { return m_rUSERINFO.KEY_INFO.dwUserGUID; }
    inline VOID         SetUserGUID( DWORD dwGUID )         { m_rUSERINFO.KEY_INFO.dwUserGUID = dwGUID; }
    inline DWORD        GetUserGUID() const                 { return m_rUSERINFO.KEY_INFO.dwUserGUID; }
    inline VOID         SetAuthID( DWORD dwID )             { m_rUSERINFO.KEY_INFO.dwAuthID = dwID; }
    inline DWORD        GetAuthID() const                   { return m_rUSERINFO.KEY_INFO.dwAuthID; }
    // PlayerKey와 ObjectKey는 동일하다.
    inline DWORD        SetObjectKey( DWORD dwKey )         { return m_rUSERINFO.KEY_INFO.dwObjectKey = dwKey;  }
    inline DWORD        GetObjectKey() const                { return m_rUSERINFO.KEY_INFO.dwObjectKey;  }

    //
    //
    inline VOID         SetUserID( const TCHAR* pszID )     { _tcsncpy( m_rUSERINFO.ID_INFO.pszUserID, pszID, ID_MAX_LEN ); m_rUSERINFO.ID_INFO.pszUserID[ID_MAX_LEN]='\0'; }
    inline const TCHAR* GetUserID()                         { return m_rUSERINFO.ID_INFO.pszUserID; }
    inline VOID         SetLastSSNKey( const BYTE* pbyLastSSNKey ) { memcpy(m_rUSERINFO.ID_INFO.LastSSN, pbyLastSSNKey, sizeof(m_rUSERINFO.ID_INFO.LastSSN) ); }
    inline const BYTE*  GetLastSSNKey()                     { return m_rUSERINFO.ID_INFO.LastSSN; }
    BOOL                IsEqualLastSSN( const BYTE* LastSSNKey );

    // 아래 것은 해보고 강구할 것
    //virtual VOID      SetClientIP( const TCHAR * ps )     {}
    //virtual const TCHAR *GetClientIP()                        { return GetIP();   }

    //--------------------------------------------
    // <USER LOGIN TIME METHODs>
    VOID                WriteLoginTime( BOOL bIsAccount = TRUE );
    inline VOID         BeginCharLoginTime() { WriteLoginTime( FALSE ); }
    inline VOID         EndCharLoginTime() { m_rUSERINFO.LOGIN_INFO.tmCharLoginTime = 0; m_rUSERINFO.LOGIN_INFO.tszCharLoginTime[0] = '\0'; }

    inline const TCHAR* GetLoginTime( BOOL bIsAccount )
    {
        TCHAR* ptszLoginTime = bIsAccount ? m_rUSERINFO.LOGIN_INFO.tszLoginTime : m_rUSERINFO.LOGIN_INFO.tszCharLoginTime;
        return ptszLoginTime;
    }

    inline time_t       GetLoginTime_T( BOOL bIsAccount )
    {
        time_t& rTime = bIsAccount ? m_rUSERINFO.LOGIN_INFO.tmLoginTime : m_rUSERINFO.LOGIN_INFO.tmCharLoginTime;
        return rTime;
    }

    inline VOID         SetLoginTime( BOOL bIsAccount, const TCHAR* pStrTM )
    {
        TCHAR* ptszLoginTime = bIsAccount ? m_rUSERINFO.LOGIN_INFO.tszLoginTime : m_rUSERINFO.LOGIN_INFO.tszCharLoginTime;

        memcpy( ptszLoginTime, pStrTM, MAX_TIMEDATA_SIZE );
        ptszLoginTime[ MAX_TIMEDATA_SIZE ] = '\0';
    }

    inline VOID         SetLoginTime_T( BOOL bIsAccount, time_t t )
    {
        time_t& rTime = bIsAccount ? m_rUSERINFO.LOGIN_INFO.tmLoginTime : m_rUSERINFO.LOGIN_INFO.tmCharLoginTime;
        rTime = t;
    }

    // <CONSTRUCTOR><DESTRUCTOR>
    // <STRUCTURE CONTROL>
    inline VOID         Init()
    {
        memset( &m_rUSERINFO.KEY_INFO, 0, sizeof(m_rUSERINFO.KEY_INFO) );
        memset( &m_rUSERINFO.ID_INFO, 0, sizeof(m_rUSERINFO.ID_INFO) );
        memset( &m_rUSERINFO.LOGIN_INFO, 0, sizeof(m_rUSERINFO.LOGIN_INFO) );
    }

    inline VOID         CopyInfo( const sUSERINFO& rINFO )
    {
        // 일괄 복사는 계속 유지해야 하는 값 때문에 안된다. 좀더 방법을 강구할 것
        //m_rUSERINFO.KEY_INFO = rINFO.KEY_INFO;
        //m_rUSERINFO.ID_INFO = rINFO.ID_INFO;
        //m_rUSERINFO.LOGIN_INFO = rINFO.LOGIN_INFO;
    }
};
#endif SUN_CODE_BACKUP
//==================================================================================================

#endif //AGENTSERVER_USER_INFO_H