#ifndef		_PARTY_H
#define		_PARTY_H
//------------------------------------------------------------------------------
/**
    @class Party

    파티 클래스
    파티 초대, 탈퇴, 강제 탈퇴 등을 처리하는 클래스

    (C) 2005 ckbang
*/
#include "PacketStruct_CG.h"
#include "ResultCode.h"
#include "InterfaceManager.h"

using namespace RC;

class Party
{
public:
    Party();
    ~Party();

    void                    Init ();                    /// 파티 초기화
    void                    ShowPartyInfoDlgs (BOOL bFlag); /// 파티 정보 다이얼로그들 띄우기
    void                    SetInfos (MSG_CG_PARTY_MEMBER_INFO_CMD *pAck);  /// 파티 정보들 채우기
    void                    AddInfo (MSG_CG_PARTY_MEMBER_JOIN_BRD *pAck);   /// 추가 정보 채우기
    void                    DelInfo (DWORD objKey);
    void					ClearInfos ();              /// 파티 구성 초기화
    void                    SetMaster (DWORD objKey);   /// 파티장 새로 설정
    void                    LeaveParty ();              /// 파티 떠나기
    
    PARTY_MEMBER_INFO*      GetInfo (DWORD objKey);     /// 파티원 정보 얻기
    DWORD                   GetPartyMasterObjKey ();    /// 파티장 키값 얻기
    BYTE                    GetDistributionType ();     /// 파티 분배방식 얻기
    BYTE                    GetPartyNum ();             /// 파티 구성원 수 얻기
    InterfaceManager::eDIALOG_ID GetStartPartyDialog ();     /// 파티창 첫번째 값 얻기
    const char *            GetInviterName ();          /// 초대한 사람의 이름
    DWORD                   GetInviterKey ();           /// 초대한 사람의 키값
    
    BOOL                    IsMaster ();                /// 파티장인가
    BOOL                    IsVisable ();               /// 파티 인터페이스가 활성화 되었는가
    BOOL                    VisableSwap ();             /// 파티 인터페이스의 활성화 상태 자동변환
    DWORD                   name2key (char *pname);     /// 파티원의 이름을 키값으로 컨버팅
    char *                  key2name (DWORD key);       /// 파티원의 키값을 이름으로 컨버팅
    
    /// 알림
    void                    Notice (TCHAR *pnotice, WzColor Color=WzColor_RGBA( 255, 255, 0, 255 ));
    /// 이유
    void                    Reason (ePARTY_RESULT eReason, WzColor Color=WzColor_RGBA(   0,  0,  255, 255 ));

    //
    // PARTY ACTION
    //
    bool                    actForcedExplusion (DWORD objKey); /// 강제 탈퇴
    bool                    actLeave ();                       /// 탈퇴
    void                    actInvite (char *pname);           /// 초대
    void                    actAskInviting(char *strInviterName, DWORD dwInviterKey); /// 초대 수락여부
    void                    actInvitedYes (); /// 초대에 응함
    void                    actInvitedNo ();                    /// 초대에 불응
    bool                    actMigration (DWORD objKey);       /// 방장 위임
    bool                    actToggleShowBuffInfo ();          /// 버프 정보 보이기/숨기기
    bool                    actDestroyParty ();                /// 파티 해체

    //
    // PARTY PARSE PACKET
    //
    void                    Parse (MSG_BASE * pMsg);

protected:
    BOOL                    m_bVisable;             /// 파티 인터페이스 활성화 플렉
    DWORD				    m_dwMasterObjKey;		/// 파티장 오브젝트 키
    BYTE				    m_byDistributionType;	/// 아이템 분배방식
    BYTE				    m_byMemberNum;			/// 파티원 수(파티장 포함)
    vector<PARTY_MEMBER_INFO>  m_MemberInfo;        /// 파티 구성원들의 목록
    BOOL                    m_bShowBuffInfo;        /// 파티 버프창 보이기
    std::string             m_strInviterName;       /// 초대한 사람 이름
    DWORD                   m_dwInviterKey;         /// 초대한 사람 키
};

//------------------------------------------------------------------------------
/**
*/
inline
BOOL
Party::VisableSwap()
{
    if (m_bVisable)
        m_bVisable = FALSE;
    else
        m_bVisable = TRUE;

    return m_bVisable;
}

//------------------------------------------------------------------------------
/**
*/
inline
BOOL
Party::IsVisable()
{
    return m_bVisable;
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
Party::GetPartyMasterObjKey()
{
    return m_dwMasterObjKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
const char *
Party::GetInviterName()
{
    return m_strInviterName.c_str();
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
Party::GetInviterKey()
{
    return m_dwInviterKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
BYTE
Party::GetDistributionType()
{
    return m_byDistributionType;
}

//------------------------------------------------------------------------------
/**
*/
inline
BYTE
Party::GetPartyNum()
{
    return m_MemberInfo.size();
}

//------------------------------------------------------------------------------
#endif	//	_PARTY_H
