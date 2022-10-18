#ifndef     __USER_INTERFACE_PARTYLEADER_H__
#define     __USER_INTERFACE_PARTYLEADER_H__
//------------------------------------------------------------------------------
/**
    @class uiPartyLeader

    interface for a party's leader.

    (C) 2005 ckbang
*/
#include "uiBase.h"
#include "uiPartyLeader_def.h"

#include "ResultCode.h"
using namespace RC;

class uiPartyLeader :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_HPBAR,
        DLGPOS_SLIDE,
        DLGPOS_OPTION,
        DLGPOS_BUFF01,
        DLGPOS_BUFF02,
        DLGPOS_BUFF03,
        DLGPOS_BUFF04,
        DLGPOS_BUFF05,
        DLGPOS_BUFF06,
        DLGPOS_BUFF07,
        DLGPOS_BUFF08,
        DLGPOS_BUFF09,
        DLGPOS_BUFF10,

        DLGPOS_FACE00,
        DLGPOS_FACE01,
        DLGPOS_FACE02,
        DLGPOS_FACE03,
        DLGPOS_FACE04,

        DLGPOS_TXT,

        DLGPOS_MINE,
        DLGPOS_TARGETSHARE,

        DIALOG_MAX,
    };

    const int MAX_SKILLEFFECT_AREA;

public:
    uiPartyLeader(InterfaceManager *pUIMan);
    virtual ~uiPartyLeader();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    void                    Reason(ePARTY_RESULT eReason, WzColor Color=WzColor_RGBA(   0,  0,  255, 255 ));
    void                    PrintInterfaceMessage(int msgIdx);
    void                    PrintInterfaceMessage(TCHAR *pstr);
    const PARTY_MEMBER_INFO* GetMemberInfo();
    void                    OnChangeInfo();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    const MSG_CG_PARTY_MEMBER_INFO_CMD* GetPartyInfos();
    int                     GetMemberInfoNum();
    bool                    IsLeader();

    void                    ClearMemberKeys();
    void                    AddMemberKey (DWORD dwKey);
    const std::vector<DWORD>&   GetMemberKeys();
    
    void                    InitMemberInfos(MSG_CG_PARTY_MEMBER_INFO_CMD *pMsg);
#ifdef _CK_RENEW_PARTY
    void                    AddMemberInfo(MSG_CG_PARTY_JOIN_BRD* pMsg);
#else
    void                    AddMemberInfo(MSG_CG_PARTY_MEMBER_JOIN_BRD* pMsg);
#endif
    void                    RemoveMemberInfo(DWORD dwObjKey);
    void                    Migration(DWORD dwObjKey);

    void                    Invite(TCHAR *pname);
    void                    AskInviting(const TCHAR *str, DWORD dwObjKey);

    void                    SetTargeting();
    void                    GetSkillEffectArea(int iIndex,RECT *pRect);

    bool                    Assist_SetTargetToLeader();

    bool                    Assist_SetPrimaryTarget(DWORD dwTarget=0);
    void                    NET_RECV_CG_PARTY_SELECT_TARGET_BRD(MSG_CG_PARTY_SELECT_TARGET_BRD *msg);

    void                    Set_AssistPartyLeader(bool on);
    bool                    Is_AssistPartyLeader();
    void                    Set_PartyLeaderTargetKey(DWORD dwKey);
    DWORD                   Get_PartyLeaderTargetKey();

    void                    Set_NextTarget();

    void                    SetSkillEffectInfo(Character *);
    void                    ClearSkillEffectInfo();

private:
    void                    ComebackToVillage();
    void                    SetLeader();
    void                    SetMemberInfo(PARTY_MEMBER_INFO *memInfo, BYTE byMyPos);

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
    void                    SetObjectInfo();
    void                    ResetHPGauge();
    bool                    IsShowBuffInfos();

protected:
    virtual void	        showWindow( BOOL val );
    virtual void            render();
    virtual void            update();

    void                    updateInfos();
    void                    updateHP();
    void                    updateBuff();

    void                    SetHP( float fHPPercent );
    CCtrlButtonWZ*          GetCtrlBtnOption();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

	DWORD                   GetObjectKey();

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    MSG_CG_PARTY_MEMBER_INFO_CMD*   m_PartyInfos;
    bool                            m_bLeader;
    CCtrlButtonWZ*                  m_ctrlBtnOption;
    uicPartyLeaderInfo              m_memInfo;

    float			                m_fGaugeSize;
    RECT                            m_rcInit;
    short                           m_idxCurrentFreeTargetAssistMember;
    
    bool                            m_bAssistPartyLeader;
    DWORD                           m_dwPartyLeaderTargetKey;

    SKILL_EFFECT_IMAGE              m_arEffectImage[MAX_PARTYDLG_SKILL_EFFECT];

    bool                            m_bShowBuffInfos;
    bool                            m_bAssistHelpOnce;

    // 주의: 아래의 변수 2개는 m_memInfo 가 있을경우에만 사용
    DWORD                           m_dwObjectKey;  // 이 다이얼로그의 주인의 오브젝트키
    BYTE                            m_byMyPos;      // 나의 다이얼로그 위치 (위에서 아래로)
    std::vector<DWORD>              m_vdwMemberKeys;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiPartyLeader::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiPartyLeader::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
const
MSG_CG_PARTY_MEMBER_INFO_CMD*
uiPartyLeader::GetPartyInfos()
{
    //assert(this->m_PartyInfos && "파티 정보를 갱신하십시요.");
    return this->m_PartyInfos;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiPartyLeader::IsLeader()
{
    return this->m_bLeader;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiPartyLeader::SetLeader()
{
    this->m_bLeader = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
CCtrlButtonWZ*
uiPartyLeader::GetCtrlBtnOption()
{
    assert(this->m_ctrlBtnOption && "옵션 버튼 컨트롤 정보가 비었습니다.");
    return this->m_ctrlBtnOption;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiPartyLeader::SetMemberInfo(PARTY_MEMBER_INFO *memInfo, BYTE byMyPos)
{
    if (!memInfo)
    {
        this->m_memInfo.Clear();
        this->m_byMyPos = byMyPos;
        this->m_dwObjectKey = 0;
        return;
    }

    uicPartyLeaderInfo pBakMemInfo;

    if (this->m_memInfo.IsFill())
        pBakMemInfo = this->m_memInfo;

    if (memInfo)
    {
        this->m_memInfo = memInfo;

		m_byMyPos = byMyPos;
        m_dwObjectKey = memInfo->m_dwObjKey;

        if (!pBakMemInfo.IsFill())
        {
            // 초기 세팅
            this->OnChangeInfo();
        }
        else if (pBakMemInfo.GetInfo().m_dwObjKey != memInfo->m_dwObjKey)
        {
            // 이후 변동사항에 따른 세팅
            this->OnChangeInfo();
        }

        
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
const PARTY_MEMBER_INFO *
uiPartyLeader::GetMemberInfo()
{
    return this->m_memInfo.GetInfoPtr();
}

//------------------------------------------------------------------------------
/**
*/
inline
int
uiPartyLeader::GetMemberInfoNum()
{
    if (!this->m_PartyInfos)
        return 0;
    return this->m_PartyInfos->m_byMemberNum;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiPartyLeader::ResetHPGauge()
{
    this->m_fGaugeSize = -1;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiPartyLeader::Set_AssistPartyLeader(bool on)
{
    this->m_bAssistPartyLeader = on;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiPartyLeader::Set_PartyLeaderTargetKey(DWORD dwKey)
{
    this->m_dwPartyLeaderTargetKey = dwKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiPartyLeader::Is_AssistPartyLeader()
{
    return this->m_bAssistPartyLeader;
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
uiPartyLeader::Get_PartyLeaderTargetKey()
{
    return this->m_dwPartyLeaderTargetKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiPartyLeader::IsShowBuffInfos()
{
    return this->m_bShowBuffInfos;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_PARTYLEADER_H__
