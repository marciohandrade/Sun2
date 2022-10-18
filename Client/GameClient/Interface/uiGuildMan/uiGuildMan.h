#ifndef     __USER_INTERFACE_MANAGER_GUILD_H__
#define     __USER_INTERFACE_MANAGER_GUILD_H__
//------------------------------------------------------------------------------
/**
    @class uiGuildMan

    interface manager for a party

    (C) 2005 ckbang
*/
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMan_def.h"
#include "hero.h"
#include "uiBaseMan.h"
#include "uiChangeGuildName/uiChangeGuildName.h"

typedef	WORD                    QCODE;
typedef GUILD_INFO				stGUILD_INFO;
typedef GUILD_MEMBER_INFO		stGUILD_MEMBER_INFO;

typedef std::vector<stGUILD_MEMBER_INFO>		VEC_GUILD_MEMBER;
typedef VEC_GUILD_MEMBER::iterator				VEC_GUILD_MEMBER_ITR;

enum eMEMBER_SORT_TYPE
{
	eSORT_NAME,
	eSORT_LV,
	eSORT_CLASS,
	eSORT_DUTY,
	eSORT_CONNETION,
	eSORT_MAX,
};


class uiGuildMain;				
class uiGuildCreate;				
class uiGuildMenu;				
class uiGuildWindow;				
class uiGuildConvertGP;			
class uiGuildDonateUP;			
class uiGuildRightDelegation;		
class uiGuildDutyVest;			
class uiGuildMemberRightView;		
class uiGuildCorpsChange;			
class uiGuildCorpsChange;			
class uiGuildMarkCombination;		
class uiGuildMarkRegisteConfirm;	
class uiGuildNotice;				
class uiGuildRankup;				
class uiGuildAlliance;			
class uiGuildHostility;			
class uiGuildOneSideHostility;	
class uiGuildRelationAsk;			
class uiGuildAllianceRequest;		
class uiGuildWareHouse;			
class uiGuildWareHouseLog;		
class uiChangeGuildName;		    


class uiGuildMan :
    public uiBaseMan
{
public:
    uiGuildMan(InterfaceManager *pUIMan);
    virtual ~uiGuildMan();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnRelease();
    virtual void            OnReset();
	virtual void            OnInitialize();
    virtual void            InitPKBooleans();
    void                    Clear();
	void					ClearNotice();

private:
    BOOL                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    bool                    CheckCreatCondition();
    void                    SEND_CG_GUILD_DESTROY_SYN();
	void                    SEND_CG_GUILD_INVITE_SYN( LPCTSTR szName );

	void					SEND_CG_GUILD_TOTALINFO_REQ_SYN(uiBase* base = NULL);
	void					ShowGuildWindow();
	
	void					SEND_CG_GUILD_WAREHOUSE_OPEN_SYN();
	void					SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN();
	BOOL					IsGuildWareHouseWindow();
	BOOL					IsGuildRankUpWindow();
	void					ShowGuildRankUpWindow(BOOL bVal);
    void                    SEND_CG_GUILD_WITHDRAW_SYN();
	BOOL                    IsGuildMemberInfo();
	void					SEND_CG_GUILD_RANKUP_SYN();
	void					SEND_CG_GUILD_CONVERT_UP2GP_SYN();
	void					SEND_CG_GUILD_DONATE_UP_SYN();

    bool                    IsGuildInfo();
    const stGUILD_INFO&		GetGuildInfo();
    void                    SetGuildInfo(const stGUILD_INFO& guildInfo);

	
    void                    SetSilentGuildInfo();
    void                    SetShowGuildMain();

    const VEC_GUILD_MEMBER&	GetGuildMembers();

	bool					_isEnableInvite();
	bool					_isEnableKick();
	bool					_isEnableNotice();
	

    bool                    Invite(const TCHAR* pname);
	bool					Kick(const TCHAR* pname);
	bool					Withdraw();	//탈퇴
	bool					Notice(TCHAR* pmsg);

	bool					IsShowWindow();
	void					CloseWin();

	TCHAR*					GetGuildName();
	void					SetGuildName(std::string & pszGuildName);
	void					SetGuildName(char* ptszGuildName);

	TCHAR*					GetGuildMasterName();
	void					SetGuildMasterName(char* ptszName);
	void					SetGuildMasterName(std::string & pszName);
	BOOL                    IsGuildMember(TCHAR* ptszGuildName); //jjumbo 케릭터명 넣어서 검사하던것 길드명 넣어서 검사하도록 수정

	TCHAR*					GetKickUser();
	TCHAR*					GetGuildNotice();

	// 친구 관련 창 중에서 열려 있는 창에 포커스를 준다
	void					SetWindowFocus();
	void					SortMember(eMEMBER_SORT_TYPE byType);
	const GUILD_CORPS_TOTAL_INFO & GetCorpsTotalInfo();
	void					SEND_CG_GUILD_DESTROY_REAR_CAMP_SYN();
    void                    Send_CW_FRIEND_STATUS_REQ(char* character_name, CHARGUID char_guid);
	void					ShowGuildDestoryWindow();
	int						GetCorpMemberNum(int iCorps);
	void					CloseAllOpenedGuildWindow();
	void					SetRelationNoneByGuildMember();

	stGUILD_MEMBER_INFO*	GetGuildMemberInfo(const TCHAR * pszName);
    stGUILD_MEMBER_INFO* const GetGuildMemberInfo(int index);

	inline HANDLE			GetGuildMarkTexture(){ return m_hGuildMarkTexture;}
	void					SEND_CG_GUILD_GUILDMARK_DELETE_SYN();
	void					SetGuildMarkBackGround(int iBackGround);
	void					SetGuildMarkPattern(int iPattern);

    static void             ProcessErrorCode(BYTE byErrorCode);

private:
	void					_addGuildMember(GUILD_MEMBER_INFO member);

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );
    virtual void            NetworkProcWorld(MSG_BASE * pMsg );

public:

    BOOL GetbForSort(){return m_bForSort;}
    void SetbForSort(BOOL bVal){m_bForSort = bVal;}

    void SortAllianceCon(BOOL bOrder);
    void SortHostilityCon(BOOL bOrder, BOOL bState); //1:소팅 순서, 2:소팅에 사용될 멤버값(true == GuildName, false == State)
    void SortOneSideHostilityCon(BOOL bOrder);

    void DelRelationCon();

    std::list<GUILD_RELATION*>& GetAllianceCon(){ return m_listGuildRelation_Alliance; }
    std::list<GUILD_RELATION*>& GetHostilityCon(){ return m_listGuildRelation_Hostility; }
    std::list<GUILD_RELATION*>& GetOneSideHostilityCon(){ return m_listGuildRelation_OneSideHostility; }

    uiBase* active_tab() const { return active_tab_; }
    void set_active_tab(uiBase* const val) { active_tab_ = val; }

    void ChangeTab(uiBase* ui_base);
    void ShowTab();


public:
    //길드 해산 대기중인지 확인 할때 사용.
    eGUILD_DELCHK GetGuildDelChk(){return m_GuildInfo.m_GuildDestroyWaitFlag;}



    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiGuildMain*				m_pUIGuildMain;
    uiGuildCreate*				m_pUIGuildCreate;
    uiGuildMenu*				m_pUIGuildMenu;
    uiGuildWindow*				m_pUIGuildWindow;
	uiGuildConvertGP*			m_pUIGuildConvertGP;
	uiGuildDonateUP*			m_pUIGuildDonateUP;
	uiGuildRightDelegation*		m_pUIGuildRightDelegation;
	uiGuildDutyVest*			m_pUIGuildDutyVest;
	uiGuildMemberRightView*		m_pUIGuildMemberRightView;
	uiGuildCorpsChange*			m_pUiGuildCorpsChange;
	uiGuildCorpsChange*			m_pUIGuildCorpsDestory;
	uiGuildMarkCombination*		m_pUIGuildMarkCombination;
	uiGuildMarkRegisteConfirm*	m_pUIGuildMakrRegisteConfirm;
	uiGuildNotice*				m_pUIGuildNotice;
	uiGuildRankup*				m_pUIGuildRankup;
	uiGuildAlliance*			m_pUIGuildAlliance;
	uiGuildHostility*			m_pUIGuildHostility;
	uiGuildOneSideHostility*	m_pUIGuildOneSideHostility;
	uiGuildRelationAsk*			m_pUIGuildRelationAsk;
	uiGuildAllianceRequest*		m_pUIGuildAllianceRequest;
	uiGuildWareHouse*			m_pUIGuildWareHouse;
	uiGuildWareHouseLog*		m_pUIGuildWareHouseLog;
	uiChangeGuildName*		    m_pUIChangeGuildName;

    stGUILD_INFO			m_GuildInfo;
    VEC_GUILD_MEMBER		m_vMembers;
    bool                    m_bGuildInfo;
    bool                    m_bSilentGuildInfo;
    bool                    m_bShowGuildMain;

	bool					m_bGuildMemberInfo;
    
    CTimerSimple            m_timerGuildMemberInfoGet;

	util::_tstring			m_strGuildName;
	util::_tstring			m_strGuildMasterName;

	TCHAR					m_tszKickMember[MAX_CHARNAME_LENGTH+1];
	TCHAR					m_tszNotice[MAX_GUILDNOTICE_LENGTH+1];

	GUILD_CORPS_TOTAL_INFO	m_GuildCorpsTotalInfo;

	HANDLE					m_hGuildMarkTexture;

	BOOL					m_bForSort; //소팅관련 버튼 누를때마다 On/Off

	std::list<GUILD_RELATION*> m_listGuildRelation_Alliance;
	std::list<GUILD_RELATION*> m_listGuildRelation_Hostility;
	std::list<GUILD_RELATION*> m_listGuildRelation_OneSideHostility;

    uiBase* active_tab_;

    uiBase* request_uibase_guild_totalinfo_;
};

//------------------------------------------------------------------------------
/**
*/
inline 
TCHAR*	
uiGuildMan::GetKickUser()
{
	return m_tszKickMember;
}

//------------------------------------------------------------------------------
/**
*/
inline
TCHAR*
uiGuildMan::GetGuildNotice()
{
	return m_tszNotice;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiGuildMan::ClearNotice()
{
	ZeroMemory(m_tszNotice, sizeof(m_tszNotice));
}

//------------------------------------------------------------------------------
/**
*/
inline 
TCHAR*
uiGuildMan::GetGuildMasterName()
{
	return const_cast<TCHAR *>(m_strGuildMasterName.c_str());
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiGuildMan::SetGuildMasterName(std::string & pszName)
{
	m_strGuildMasterName = pszName;
}


//------------------------------------------------------------------------------
/**
*/
inline
void
uiGuildMan::SetGuildMasterName(char* ptszName)
{
	m_strGuildMasterName = ptszName;
}

//------------------------------------------------------------------------------
/**
*/
inline 
TCHAR*  uiGuildMan::GetGuildName()
{
	return const_cast<TCHAR *>(m_strGuildName.c_str());

}

//------------------------------------------------------------------------------
inline
void uiGuildMan::SetGuildName(std::string & pszGuildName)
{
	m_strGuildName = pszGuildName;
}

//------------------------------------------------------------------------------
inline
void
uiGuildMan::SetGuildName(char* ptszGuildName)
{
	TCHAR szTemp[256] = {0,};
	StrnCopy(szTemp, ptszGuildName, MAX_GUILDNAME_LENGTH);
	szTemp[MAX_GUILDNAME_LENGTH] = 0;

	m_strGuildName = util::_tstring(szTemp);
}

//------------------------------------------------------------------------------
/**
*/
inline
const stGUILD_INFO&
uiGuildMan::GetGuildInfo()
{
    return this->m_GuildInfo;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiGuildMan::SetGuildInfo(const stGUILD_INFO& guildInfo)
{
    memcpy ((stGUILD_INFO*)&m_GuildInfo,
            (stGUILD_INFO*)&guildInfo,
            sizeof(stGUILD_INFO));

    this->m_bGuildInfo = true;

	SetGuildName((char*)guildInfo.m_tszGuildName);

	if (g_pHero)
	{
		g_pHero->SetGuildName((char*) guildInfo.m_tszGuildName, MAX_GUILDNAME_LENGTH);
		g_pHero->GetGuildRenderInfo().m_BackGroundIndex = guildInfo.m_BackGroundIdx;
		g_pHero->GetGuildRenderInfo().m_PatternIndex = guildInfo.m_PatternIdx;
		g_pHero->GetGuildRenderInfo().m_GuildGuid = guildInfo.m_GuildGuid;
		g_pHero->GetGuildRenderInfo().m_GuildGrade = guildInfo.m_GuildGrade;


	}
}
//------------------------------------------------------------------------------
/**
*/
inline
const GUILD_CORPS_TOTAL_INFO &
uiGuildMan::GetCorpsTotalInfo()
{
	return this->m_GuildCorpsTotalInfo;
}


//------------------------------------------------------------------------------
/**
*/
inline
bool
uiGuildMan::IsGuildInfo()
{
    return this->m_bGuildInfo;
}

//------------------------------------------------------------------------------
/**
*/
inline
BOOL
uiGuildMan::IsGuildMemberInfo()
{
    return this->m_bGuildMemberInfo;
}

//------------------------------------------------------------------------------
/**
*/
inline
const VEC_GUILD_MEMBER&
uiGuildMan::GetGuildMembers()
{
    return this->m_vMembers;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiGuildMan::SetSilentGuildInfo()
{
    this->m_bSilentGuildInfo = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiGuildMan::SetShowGuildMain()
{
    this->m_bShowGuildMain = true;
}

//------------------------------------------------------------------------------
/**
*/
BOOL                    
inline 
uiGuildMan::IsGuildMember(TCHAR* ptszGuildName)
{
	if( ! IsGuildInfo() )
		return FALSE;

	if (!StrnCmp (GetGuildInfo().m_tszGuildName, ptszGuildName, MAX_CHARNAME_LENGTH))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_MANAGER_GUILD_H__
