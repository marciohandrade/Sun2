#pragma once

//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiCommunityMan class
//------------------------------------------------------------------------------

#include "uiBaseMan.h"

#include "Nstring.h"
#include "timer.h"

#define MAX_DATE_SIZE		10

#ifdef _INTERNATIONAL_UI
	const WzID c_wiCommunityFont = StrToWzID("st10");
#else
	const WzID c_wiCommunityFont = StrToWzID("m212");
#endif//_INTERNATIONAL_UI

#define GROUP_MESSAGE_TIMER	600000	// 10분, 레벨차이 공지시간

enum eCOMMUNITY_MODE
{
	eCM_FRIEND = 0,
	eCM_BLOCK,
	eCM_REVENGE,
	eCM_MAIL,
	eCM_GROUP,
	eCM_MODEMAX,
};

enum eCOMMUNITY_SORT
{
	eCOMSORT_NAME = 0,
	eCOMSORT_LV,
	eCOMSORT_CLASS,
	eCOMSORT_CHANNEL,
	eCOMSORT_ONLINE,
	eCOMSORT_SENDER,
	eCOMSORT_DATE,
	eCOMSORT_MEMO,
};

enum eCOMMUNITY_SORT_BASE
{
	eCOMSORT_BNONE = 0,	// 그냥 탭이 바뀌거나, 화면 갱신 된 경우
	eCOMSORT_BSAMETAB_SAMEBTN, // 버튼을 누른 상태이며, 탭과 버튼이 모두 같을 때, 반전
	eCOMSORT_BSAMETAB_OTHERBTN, // 버튼을 누른 상태이며, 탭은 같지만 버튼은 바뀐 경우, true로 OrderBy(오름차순)
};

class uiCommunityDlg;			// 커뮤니티 메인창
class uiCommunityInfo;			// 상세 정보창
class uiCommunityReadMail;		// 쪽지 읽기 창
class uiCommunityWriteMail;		// 쪽지 쓰기 창
class uiCommunityChat;			// 1:1 대화창

struct FRIEND_INFO_DATA
{
	TCHAR		m_ptszCharName[MAX_CHARNAME_LENGTH+1];
	BYTE		m_byChannel;
	BYTE		m_byZoneType;							// 현재 위치(마을/필드/로비...), eZONETYPE 참고
	MAPCODE		m_MapCode;								// 현재 맵 코드
	DWORD		m_dwZoneKey;							// 현재 지역 인덱스(배틀존 번호로 사용 가능)
	TCHAR		m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	// 소속된 길드명(없으면 0)
	BYTE		m_byDuty;

    //! _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    TCHAR		m_ptszMemo[MAX_FRIEND_MEMO_LENGHT+1];
    CHARGUID    m_dwCharGuid;
};

struct FRIEND_DATA
{
	nString     name;
	LEVELTYPE	lv;
	BYTE		bClassCode;
	BOOL        bOnline;
	BYTE		byChannel;								// 현재 접속중인 채널(온라인일 때만 사용)
	BYTE		byMaster;								// 상/하위 요소
	INT64		lastDate;	                            // 마지막 접속 시간 	
	DWORD		dwFriendGuid;
	BYTE		byFriendSts;
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    BYTE        gender; // 성별
#endif  // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
};

struct FRIEND_CHAT_DATA
{
	nString     name;
	DWORD       dwDlgId;
	std::vector<util::_tstring>		vstrChatMsg;
};

struct MAIL_DATA
{
	STRUCT_SUBJECT_PACKETINFO	m_subject;
	bool						m_bRecvMemo;	// 서버로부터 메모 내용을 받아왔는가
    SLOTCODE                    m_recvItemCode; //보상 받을 아이템 코드(0:하임)
    ULONGLONG                   m_amountCost;   //보상 받을 갯수/금액
	TCHAR						m_ptszMemo[MAX_MEMO_STRLENGTH];
};

class uiCommunityMan : public uiBaseMan
{
	typedef	std::vector<MAIL_DATA>				MAIL_DATA_VECTOR_TYPE;
	typedef std::vector<MAIL_DATA>::iterator	MAIL_DATA_VECTOR_ITRTYPE;

	enum ePacketPending
	{
		ePKBOOL_COMMUNITY_BASE = 0,
		ePKBOOL_COMMUNITY_CW_MEMO_VIEW_REQ,
		ePKBOOL_COMMUNITY_MAX,
	};
	const static DWORD TIMEOUT_PACKET_PENDING = 3;

public:
    uiCommunityMan(InterfaceManager *pUIMan);

    void OnCallBackMailDeleteInReadDialog( bool bYes );

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnRelease();
    virtual void            Process(DWORD dwTick);
    virtual void            OnInitialize();
    virtual void			InitPKBooleans();

private:
    void                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:
	void                    Clear();
	void					ClearFriendData();
	void					ClearSelection();
	void					Reset();

    void					ShowCommunityDlg();
    uiCommunityInfo*        GetCommunityinfo_Dialog() const { return m_pUICommunityInfo; }

	// 패킷 처리
	void					SEND_CW_FRIEND_NEWLIST_SYN();
	void					Send_CW_FRIEND_STATUS_REQ(CHAR *ptszCharName, CHARGUID char_guid);
	void					Send_CW_FRIEND_STATUS_JOIN_SYN();

	void					Send_CW_GROUP_STATUS_REQ(CHAR *ptszCharName);
	void					SEND_CW_GROUP_RESTORELIST_SYN();
	void					SEND_CW_GROUP_NEWLIST_SYN();
	void					OnRecvCW_Group(MSG_BASE *pMsg);	
    void		            OnRecvCG_Group(MSG_BASE *pMsg);

	void					OnRecvCW_Friend(MSG_BASE *pMsg);
    void					OnRecvCW_Friend_STATUS_ANS(MSG_BASE *pMsg);
	void					OnRecvCW_Revenge(MSG_BASE *pMsg);
	void					OnRecvCG_Event(MSG_BASE *pMsg);
	void					SetRqstJoinGame(bool bRqst) { m_bRqstJoinGame = bRqst; }
	bool					IsRqstJoinGame()			{ return m_bRqstJoinGame;	}

    void                    SEND_CW_FRIEND_MEMO_UPDATE_SYN(CHARGUID game_user_id, TCHAR* memo);
	void					SEND_CW_MEMO_SEND_SYN(TCHAR *pszName, TCHAR *pszMemo);
	void					SEND_CW_MEMO_LIST_REQ();
	bool					SEND_CW_MEMO_VIEW_REQ(MEMOGUID guid);
	void					SEND_CW_MEMO_DELETE_SYN(MEMOGUID guid);
	void					OnRecvCW_Memo(MSG_BASE *pMsg);	
	void					ProcessMemoErrCode(BYTE byErrCode);


	// 친구
    void					AddFriend( const STRUCT_FRIEND_PACKET_INFO& friend_info, DWORD guild_id );
	BOOL					DeleteFriend(TCHAR *pName);	

	void					SetFriendOnline(TCHAR *pName, BOOL bOn, BYTE byChannel);

	void					SetFriendLevel(TCHAR *pName, LEVELTYPE lv);
	int						FindFriend(TCHAR *pName);
	TCHAR *					GetFriendSelectString();

	// 차단 
	void					AddBlockFriend(TCHAR *pName);
	BOOL					DeleteBlockFriend(TCHAR *pName);
	int						FindBlockFriend(TCHAR *pName);
	TCHAR *					GetBlockFriendSelectString();

	// 리벤지 
	void					AddRevenge(TCHAR *pName, LEVELTYPE lv, BYTE bClassCode, BOOL bOnline, 
                                        BYTE byChannel, CHARGUID character_guid);
	BOOL					DeleteRevenge(TCHAR *pName);

	void					SetRevengeOnline(TCHAR *pName, BOOL bOn, BYTE byChannel);

	void					SetRevengeLevel(TCHAR *pName, LEVELTYPE lv);
	int						FindRevenge(TCHAR *pName);
	TCHAR *					GetRevengeSelectString();
	void					ShowRevengeResultCode(BYTE bError);

	TCHAR *					GetSelectString();
	BOOL					IsOnline(TCHAR *pName);
	bool					IsMyChannel();
	BYTE					GetMyChannel();
	LPCTSTR					GetMyServer();

	LEVELTYPE				GetLevel();
	BYTE					GetClass();	
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    GenderType::Value       GetGender();
#endif //_NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    CHARGUID                GetGUID(TCHAR* character_name);
    FRIEND_DATA*            GetFriendData(TCHAR* character_name);

	// 친구초대
    FRIEND_DATA*            GetFriendData(CHARGUID character_guid);
	BYTE					IsFriendSts(TCHAR *pName);

	// 그룹
	void					AddGroup(TCHAR *pName, LEVELTYPE lv,BYTE bClassCode, BOOL bOnline, 
                                        BYTE byChannel, BYTE byMaster, INT64 i64LastDate,
                                        CHARGUID character_guid);
	BOOL					DeleteGroup(TCHAR *pName);	
	void					SetGroupOnline(TCHAR *pName, BOOL bOn, BYTE byChannel);
	void					SetGroupLevel(TCHAR *pName, LEVELTYPE lv);
	int						FindGroup(TCHAR *pName);
	TCHAR *					GetGroupSelectString();
	void					SortbyList(nArray<FRIEND_DATA> &Array, int iSortBy, bool OrderBy);
	void					SortbyListMail(std::vector<MAIL_DATA> &Array, int iSortBy, bool OrderBy);
	void					ResponseJoinGroup(char *pszMasterName);
	void					SetGroupRejection(BOOL bRejection);

	inline void				SetOpenGroupJoinWindow(bool bWin){ m_bIsOpenGroupJoinWinodw = bWin; }
	inline void				SetOrderBy(int eCM_Id, bool bOrderBy){ m_bOrderBy[eCM_Id] = bOrderBy; }
	inline bool				GetOrderBy(int eCM_Id){ return m_bOrderBy[eCM_Id]; }
	inline void				SetSortBy(int eCM_Id, int bSortBy){ m_iSortBy[eCM_Id] = bSortBy; }
	inline int				GetSortBy(int eCM_Id){ return m_iSortBy[eCM_Id]; }
	inline void				SetSortReady(int iReady){ m_iSortReady = iReady; }
	inline int				GetSortReady(){ return m_iSortReady; }
	inline BOOL				IsRecvGroup();
	bool					GetRejectGroup()			{ return m_bRejectGroup;	}
	void					SetGroupName(char *pName);
	const char*				GetGroupName();
	bool					CheckGroupLevel();
	void					SetGroupTimer(DWORD dwTime);
	void					ClearGroupTimer();
	void					CheckGroupLevelfromHero();
	void					SetGroupInfo(FRIEND_INFO_DATA info);
	INT64					GetNowTimeINT64();

	INT64					ParseMemoInfo(char *pStr,int nIndex,int nBufSize, unsigned int uBufSize = 0, char *pDest = NULL);

	// 친구 상세정보
	void					SetFriendInfo(FRIEND_INFO_DATA info);
	void					ShowFriendInfoDlg(BOOL bShow = TRUE);
	void					SetWaitJoinGame(bool bWait);
	bool					IsWaitJoinGame()				{ return m_bWaitJoinGame;	}
	
	void					RqstRefreshFriendInfo();



	// 1:1 채팅 
	BOOL					DeleteChatDlgId(DWORD dwId);
	void					AddFriendChatData(FRIEND_CHAT_DATA & data);
	int						FindChatDialog(TCHAR *pName);
	void					RemoveAllChatDialog();
	int						FindChatData(TCHAR *pName);
	void					ShowFriendChatDlg(TCHAR *pName);
	bool					NewFriendChatDlg(int iIndex);
	void					AddChatMsg(TCHAR * ptszFriendNameFrom ,TCHAR *ptszChatMsg ,COLORREF TextColor);
	//void					ShowTooltipForNewChatMsg();
	int						GetNewFriendChatCount()			{ return m_iNewFriendChat; }
	TCHAR*					GetFriendNameForTooltip();
	void					RequestFriendChat(bool bInfoDlg);
	
	BOOL					IsRecvMessage();
	void					SetMessageRejection(BOOL bRejection);
	bool					GetRejectChat()			{ return m_bRejectChat;	}
	bool					GetNotDisplayOffLine()	{ return m_bNotDisplayOffLine; }
	bool					GetDisplayNewMail()		{ return m_bDisplayNewMail;		}
	void					ToggleNotDisplayOffLine()		{ m_bNotDisplayOffLine = !m_bNotDisplayOffLine; }
	void					ToggleDisplayNewMail()			{ m_bDisplayNewMail = !m_bDisplayNewMail;	}



	// 쪽지 관련
	void					ShowWriteMailDlg(TCHAR* pszName = NULL);
	void					ShowReadMailDlg(MAIL_DATA *pData);
	void					ShowReadMailDlg(MEMOGUID guid);

	int						GetCurMailNum()		{ return m_vecMailData.size();	}

	void					AddMailData(STRUCT_SUBJECT_PACKETINFO *pSubject);		// 뒤에 추가
	void					AddMailDataBegin(STRUCT_SUBJECT_PACKETINFO *pSubject);	// 앞에 추가
	void					AddMailView(const MSG_CW_MEMO_VIEW_ANS* memo_view);
	void					DelMail(MEMOGUID guid, bool bMsgShow = true);
	void					DelAllMailData(bool bMsgShow = true);
	void					DelOldMail();

	MEMOGUID				GetCurMailGuid();


	// 기타
	void					SetMode(int iMode);
	int						GetMode()				{ return m_iCommunityMode;	}

	// 친구 관련 창 중에서 열려 있는 창에 포커스를 준다
	void					SetWindowFocus();

	bool					IsShowSystemDialog()		{ return m_bShowSystemDialog;	}
	void					SetSystemDialog(bool bShow)	{ m_bShowSystemDialog = bShow;	}

	void					SetPendingDelFriend(TCHAR* pName);
	void					SetPendingDelBlock(TCHAR* pName);
	void					SetPendingDelRevenge(TCHAR* pName);
	void					SetPendingDelGroup(TCHAR* pName);
	void					SetPendingInfoMode(int iMode)		{ m_iPendingInfoMode	= iMode;	}


	void					SetPendingDelMemoGuid(MEMOGUID guid)	{ m_PendingDelMemoGuid = guid;	}

	TCHAR*					GetPendingDelFriend()		{ return m_szPendingDelFriend;	}
	TCHAR*					GetPendingDelBlock()		{ return m_szPendingDelBlock;	}
	TCHAR*					GetPendingDelRevenge()		{ return m_szPendingDelRevenge;	}
	TCHAR*					GetPendingDelGroup()		{ return m_szPendingDelGroup;	}
	int						GetPendingInfoMode()		{ return m_iPendingInfoMode;	}
	MEMOGUID				GetPendingDelMemoGuid()		{ return m_PendingDelMemoGuid;	}

	void					ClearPendingData();

	BOOL					HaveNewNote();				// 읽지않은 쪽지가 있는가?

    int last_scrollbar_position(const eCOMMUNITY_MODE community_mode) const { return last_scrollbar_position_[community_mode]; }
    eCOMMUNITY_MODE last_community_mode() const { return last_community_mode_; }
    eCOMMUNITY_MODE last_community_friend_subject_mode() const { return last_community_friend_subject_mode_; }
    void set_last_scrollbar_position(const eCOMMUNITY_MODE community_mode, int val) { last_scrollbar_position_[community_mode] = val; }
    void set_last_community_mode(const eCOMMUNITY_MODE community_mode) { last_community_mode_ = community_mode; }
    void set_last_community_friend_subject_mode(const eCOMMUNITY_MODE friend_subject_mode) { last_community_friend_subject_mode_ = friend_subject_mode; }
    unsigned int onlined_friend_count(){ return onlined_friend_count_; }
    unsigned int onlined_group_count(){ return onlined_group_count_; }
    void SEND_CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ(const MAIL_DATA* mail_data);
    STRUCT_SUBJECT_PACKETINFO::eMEMOTYPE GetMemoType(MEMOGUID memo_guid);

protected:
	void					_processResultCode(BYTE byErrorCode);

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );
	virtual void            NetworkProcWorld( MSG_BASE * pMsg );
    
    //--------------------------------------------------------------------------
    // member vars
    //
public:
	nArray<FRIEND_DATA>			m_FriendArray;
	nArray<FRIEND_DATA>			m_BlockFriendArray;
	nArray<FRIEND_DATA>			m_RevengeArray;
	nArray<FRIEND_CHAT_DATA>	m_chatDlgArray;
	nArray<FRIEND_DATA>			m_GroupArray;
	
	std::vector<MAIL_DATA>		m_vecMailData;

	int							m_iCurFriendSelect;
	int							m_iCurBlockSelect;
	int							m_iCurRevengeSelect;
	int							m_iCurMailSelect;
	char						m_GroupName[MAX_CHARNAME_LENGTH+1];
	int							m_iCurGroupSelect;
	int							m_iSortBy[eCM_MODEMAX];
	bool						m_bOrderBy[eCM_MODEMAX];
	bool						m_bIsOpenGroupJoinWinodw;
	bool						m_bRecvGroupMemberInfo;
	bool						m_bRejectGroup;
	FRIEND_INFO_DATA			m_GroupInfo;

	bool						m_bRejectChat;
	bool						m_bRecvMemberInfo;
	bool						m_bRecvMailInfo;

	BYTE						m_byRevengePoint;

	FRIEND_INFO_DATA			m_FriendInfo;
	bool						m_bWaitJoinGame;

	TCHAR						m_szRecvNameForMail[MAX_CHARNAME_LENGTH+1];

private:
	uiCommunityDlg*				m_pUICommunityDlg;
	uiCommunityInfo*			m_pUICommunityInfo;
	uiCommunityReadMail*		m_pUICommunityReadMail;
	uiCommunityWriteMail*		m_pUICommunityWriteMail;
	
	int							m_iNewFriendChat;
	int							m_iCommunityMode;
	int							m_iSortReady;
	util::Timer					m_GroupMessageTimer;

	bool						m_bNotDisplayOffLine;
	bool						m_bDisplayNewMail;

	bool						m_bShowSystemDialog;

	util::Timer					m_PacketTimer;

	TCHAR						m_szPendingDelFriend[MAX_CHARNAME_LENGTH+1];
	TCHAR						m_szPendingDelBlock[MAX_CHARNAME_LENGTH+1];
	TCHAR						m_szPendingDelRevenge[MAX_CHARNAME_LENGTH+1];
	TCHAR						m_szPendingDelGroup[MAX_CHARNAME_LENGTH+1];

	int							m_iPendingInfoMode;

	MEMOGUID					m_PendingDelMemoGuid;

	bool						m_bRqstJoinGame;	// 게임같이하기 요청중

    int last_scrollbar_position_[eCM_MODEMAX];  //!< 각탭에 대한 스크롤을 따로 저장한다.
    eCOMMUNITY_MODE last_community_mode_;
    eCOMMUNITY_MODE last_community_friend_subject_mode_;    //!< 친구 탭일때는 3가지 서브 탭이 있다.
    unsigned int onlined_friend_count_;
    unsigned int onlined_group_count_;
};

//------------------------------------------------------------------------------
/**
*/
inline 	BOOL uiCommunityMan::IsRecvMessage()
{
	return m_bRejectChat;
}

//------------------------------------------------------------------------------
/**
*/
inline 	BOOL uiCommunityMan::IsRecvGroup()
{
	return m_bRejectGroup;
}
