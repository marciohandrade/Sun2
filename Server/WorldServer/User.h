#pragma once

#include <NetworkObject.h>
#include <string>

// 친구 관리 위해 include
#include <map>

// 암호화 처리 위해 include
#include <CryptManager.h>
#include<ItemOwnFlag.h>
#include <StructInPacket.h>
#include "GroupManager.h"

// <Cryptography>
typedef struct {
	ServerSNKey			m_SNKey;
	CryptManager		m_CryptManager;

	BYTE					GetCryptAlgorithmInfo( BYTE bySNKey, BYTE* cryptInfo, DWORD _dwUserKey )
	{	return m_CryptManager.GetCryptAlgorithmInfo( bySNKey, cryptInfo, _dwUserKey );	}

	VOID					InitCryptInfo( DWORD dwUserKey, BYTE byProtoMajorVer, BYTE byProtoMinorVer ) {
		m_SNKey.Initialize( dwUserKey, byProtoMajorVer, byProtoMinorVer );
		/*C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO*/
		m_CryptManager.Initialize( CryptManager::bServerSession );
	};

	// About SNKey
	VOID					NextRoundSNKey4Send() { m_SNKey.NextRoundSNKey4Send(); };
	VOID					NextRoundSNKey4Recv() { m_SNKey.NextRoundSNKey4Recv(); };
	BYTE					GetSendSNKey() { return m_SNKey.GetSendSNKey( RandomSNKey::c_dwBaseKey ); };
	BYTE					GetSendPairSNKey() { return m_SNKey.GetSendSNKey( RandomSNKey::c_dwPairKey ); };
	BYTE					GetRecvSNKey() { return m_SNKey.GetRecvSNKey( RandomSNKey::c_dwBaseKey ); };
	BYTE					GetRecvPairSNKey() { return m_SNKey.GetRecvSNKey( RandomSNKey::c_dwPairKey ); };
}STRUCT_CRYPT;

struct STRUCT_FRIEND_INFO;
struct STRUCT_BLOCK_INFO;
struct STRUCT_REVENGE_INFO;
struct STRUCT_GROUP_MEMBER_INFO;

//#define _WHATTHEHELL_CHANGE_FRIENDCHECK_20060802

class User;
//class Party;
class ChatZone;
class CommChatZone;

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
#include "FriendMap.h"
#else
typedef STLX_MAP<DWORD, STRUCT_FRIEND_INFO*>		FRIEND_INFO_MAP;
typedef FRIEND_INFO_MAP::iterator					FRIEND_INFO_MAP_ITER;
typedef std::pair< DWORD, STRUCT_FRIEND_INFO* >		FRIEND_INFO_MAP_PAIR;
#endif 

typedef STLX_MAP<DWORD, STRUCT_BLOCK_INFO*>			BLOCK_INFO_MAP;
typedef BLOCK_INFO_MAP::iterator					BLOCK_INFO_MAP_ITER;
typedef std::pair< DWORD, STRUCT_BLOCK_INFO* >		BLOCK_INFO_MAP_PAIR;

typedef STLX_MAP<DWORD, STRUCT_REVENGE_INFO*>		REVENGE_INFO_MAP;
typedef REVENGE_INFO_MAP::iterator					REVENGE_INFO_MAP_ITER;
typedef std::pair< DWORD, STRUCT_REVENGE_INFO* >	REVENGE_INFO_MAP_PAIR;

typedef STLX_MAP<DWORD, User*>						FRIEND_ONLINE_MAP;
typedef FRIEND_ONLINE_MAP::iterator					FRIEND_ONLINE_MAP_ITER;
typedef std::pair< DWORD, User* >					FRIEND_ONLINE_MAP_PAIR;

enum eUSER_TYPE { INVALID_USER, TEMP_USER, ACTIVE_USER };

class BaseZone;

class User : public NetworkObject
{
	friend class UserManager;
    friend class GroupManager;

	enum { WAITING_TICK = 1000 * 100 };

	enum { FRIEND_STAT_LOGIN = 0, FRIEND_STAT_LOGOUT = 1, REVENGE_STAT_LOGIN = 2, REVENGE_STAT_LOGOUT = 3, };


#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
public:
	enum FRIEND_TYPE {FRIEND_NORMAL = 1, FRIEND_INVIATE = 5, FRIEND_BEINVIATED = 6};
#endif

protected:
	// 클라이언트와 통신할 때는 반드시 암호화 여부를 체크하는 SendPacket()을 써야 하므로 protected 멤버로 뺐다.
	BOOL Send( BYTE *pMsg, WORD wSize ) { return NetworkObject::Send( pMsg, wSize ); }

public:
	User();
	virtual ~User();

	virtual VOID			Init();
	virtual VOID			Release();
	virtual VOID			Update();
	virtual VOID			OnRedirect() {}

	VOID					Logout();
	VOID					OnEnterZone( BaseZone *pZone );
	VOID					OnLeaveZone( BaseZone *pZone );
	VOID					OnEnterSector( BaseZone *pZone );
	VOID					OnLeaveSector( BaseZone *pZone );
	VOID					ProcessLogout();
	VOID					ProcessCharLogout();

	inline VOID				SetNetworkIndex( DWORD index ) { m_dwNetworkIndex = index; }
	inline DWORD			GetNetworkIndex() { return m_dwNetworkIndex; }
	inline VOID				SetGUID( DWORD dwGUID ) { m_dwGUID = dwGUID; }
	inline DWORD			GetGUID() const { return m_dwGUID; }
	inline VOID				SetCharGuid( DWORD CharGuid ) { m_CharGuid = CharGuid; }
	inline DWORD			GetCharGuid() { return m_CharGuid; }
	VOID					SetAccountID( const TCHAR* ptszAccountID );
	inline TCHAR *			GetAccountID() { return m_ptszAccountID; }
	inline VOID				SetCharName( std::string charName ) { m_strCharName = charName; }
	inline std::string&		GetCharName() { return m_strCharName; }
    // CHANGES: f110524.2L, fixed invalid freed problem to pool
    virtual eUSER_TYPE GetUserType() const = 0;
	inline VOID				StartTimeCheck() { m_dwTimeoutTick = GetTickCount() + WAITING_TICK; }
	inline BOOL				IsTimeout()
	{
		return GetTickCount() > m_dwTimeoutTick;
	}
	inline VOID				SetAuthIP( const char *pszIP ) { strncpy(m_szAuthIP, pszIP, _countof(m_szAuthIP)); m_szAuthIP[_countof(m_szAuthIP) - 1] = '\0'; }
	inline char*			GetAuthIP() { return m_szAuthIP; }
	inline VOID				SetAuthID( DWORD dwAuthID ) { m_dwAuthID = dwAuthID; }
	inline DWORD			GetAuthID() { return m_dwAuthID; }
	inline VOID				SetChannelID( DWORD channelID ) { m_dwChannelID = channelID; }
	inline DWORD			GetChannelID() { return m_dwChannelID; }
	inline VOID				SetZoneKey( DWORD dwZoneKey ) { m_dwZoneKey = dwZoneKey; }
	inline DWORD			GetZoneKey() { return m_dwZoneKey; }
	inline VOID				SetStatus( eZONETYPE status ) { m_status = status; }
	inline eZONETYPE		GetStatus() const { return m_status; }

	inline VOID				SetWhisper(BOOL bWhisper)	{ m_bWhisper = bWhisper; }	// 귓말 받을 것인지 세트. 기본 값은 TRUE(허용)
	inline BOOL				CanWhisper()	{	return m_bWhisper;	}				// 귓말 받는 상태인지(TRUE), 아닌지(FALSE)
	void					OnLogString(char *pszLog);								// 상속

	inline BaseZone*		GetNormalChatZone() { return m_pNormalChatZone; }
	inline VOID				SetNormalChatZone( BaseZone *pZone ) { m_pNormalChatZone = pZone; }
	VOID					SendNormalChat( BYTE *pMsg, WORD nSize );
	VOID					SendNomalChatToAll( BYTE *pMsg, WORD wSize );
	inline VOID				SetMapCode(MAPCODE wMapCode) { m_MapCode = wMapCode; }
	inline MAPCODE			GetMapCode() { return m_MapCode; }
	inline VOID				SetFieldCode(DWORD dwFieldCode)	{ m_dwFieldCode = dwFieldCode; }
	inline DWORD			GetFieldCode()	{ return m_dwFieldCode; }
	inline VOID				SetSectorIndex(int dwSectorIndex)	{ m_nSectorIndex = dwSectorIndex; }
	inline int				GetSectorIndex()	{ return m_nSectorIndex; }
	VOID					SendWorldChatToAll(BYTE *pMsg, WORD wSize, BOOL bNow = TRUE);
	// 친구 정보 관리
	BOOL					CheckFriendList();
	VOID					InitFriendListInit();
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	FriendMap*				GetFriendMap(FRIEND_TYPE type = FRIEND_NORMAL);
#else		
	inline BYTE				GetFriendNum() { return (BYTE)m_mapFriend.size(); }
	STRUCT_FRIEND_INFO*		GetFirstFriend();
	STRUCT_FRIEND_INFO*		GetNextFriend();
	BOOL					AddFriendInfo( STRUCT_FRIEND_INFO* pFriend );	
	STRUCT_FRIEND_INFO*		FindFriend( DWORD dwCharGuid );
	STRUCT_FRIEND_INFO*		FindFriend( TCHAR* ptszFriendName );
	VOID					RemoveFriend( DWORD dwGuid );
#endif
	
	inline BOOL				GetFriendChatBlock() { return m_bFriendChatBlock; }
	inline VOID				SetFriendChatBlock(BOOL bBlock) { m_bFriendChatBlock = bBlock; }
	// 친구 관련
	VOID					AddOnlineFriend( User *pUser, BOOL bLoggedIn = TRUE );
	VOID					RemoveOnlineFriend( User *pUser );
	User*					FindOnlineFriend( DWORD CharGuid );
	VOID					AlarmFriendStatChange( int nFlag, User *pUser );
	inline VOID				SetFriendTransaction( BOOL bTrans ) { m_bFriendTrans = bTrans; }
	inline BOOL				IsFriendTransaction() { return m_bFriendTrans; }
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    RC::eFRIEND_RESULT      CheckConditionAccessFriendInfo(CHARGUID friendguid);

    // implemented by __KR_001355_20090608_GROUPINGSYSTEM_DATA = {
    //유저 DB작업 트랜젝션
    inline BOOL             IsGroupTransaction() { return m_bGroupTrans; }                                      
    //트랜젝션 세팅
    inline VOID				SetGroupTransaction( BOOL bTrans ) { m_bGroupTrans = bTrans; }                      
    BYTE                    GetGroupMemberCount();
    //해당 케릭터 클레스 세팅
    inline VOID             SetCharClass( BYTE charClass ){ m_CharClass = charClass; }                          
    //해당 케릭터 클레스 가져오기
    inline BYTE             GetCharClass( ){ return m_CharClass; }                                              
    //최근에 받은 보너스 하임세팅
    inline VOID             SetCurBonusMoney( MONEY curBonusMoney ){ m_curBonusMoney = curBonusMoney; }        
    //최근에 받은 보너스 하임 얻기
    inline MONEY            GetCurBonusMoney(){ return m_curBonusMoney; }                                      
    //최근에 받은 보너스 경험치 세팅
    inline VOID             SetCurBonusEXP( EXPTYPE curBonusEXP ){ m_curBonusEXP = curBonusEXP; }               
    //최근에 받은 보너스 경험치 얻기
    inline EXPTYPE          GetCurBonusEXP(){ return m_curBonusEXP; }                                           
    //맴버 추가 거부 토글 세팅
    inline VOID             SetAddBlock( BOOL bAddBlock ) { m_bAddBlock = bAddBlock; }                          
    //맴버 추가 거부 토글 정보 가져오기
    inline BOOL             GetAddBlockInfo() { return m_bAddBlock; }                                           
    //축적 저장 경험치 세팅
    inline VOID             SetCurAccumEXP( EXPTYPE curAccumEXP ) { m_curAccumEXP = curAccumEXP; }              
    //축적 저장 경험치 가져오기 최대 축적 경험치는 500만이다.
    inline EXPTYPE          GetCurAccumEXP( ) { return ( m_curAccumEXP > MAX_GROUP_EXP)? 
                                                MAX_GROUP_EXP : m_curAccumEXP; }   
    inline VOID             AddCurAccumEXP( EXPTYPE curAccumEXP ) { m_curAccumEXP += curAccumEXP; }
    //그룹 리스트를 요청한적 있나?
    BOOL					CheckGroupList();                                                                   
    VOID                    InitGroupInfo();
    //임시 맴버 정보 저장소에 해당하는 유저 정보 세팅
    VOID                    SetStructMemberInfo( User* pUser, eGROUP_STAT eMemberStat );                        
    //임시 멤버 정보 저장소 가져오기
    inline STRUCT_GROUP_MEMBER_INFO* GetStructMemberInfo() { return &m_tempMemberInfo; }                        
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    inline VOID             SetCharGender( BYTE gender ){ gender_ = gender; }                          
    //해당 케릭터 클레스 가져오기
    inline BYTE             GetCharGender( ){ return gender_; }                                              
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_DATA'
	VOID					SendOnlineFriendAll( BYTE* pMsg, WORD wSize );

	// 리벤지 정보 관리
	inline BYTE				GetRevengeNum() { return (BYTE)m_mapRevenge.size(); }
	STRUCT_REVENGE_INFO*	GetFirstRevenge();
	STRUCT_REVENGE_INFO*	GetNextRevenge();
	BOOL					AddRevengeInfo( STRUCT_REVENGE_INFO* pRevenge );
	STRUCT_REVENGE_INFO*	FindRevenge( DWORD dwCharGuid );
	STRUCT_REVENGE_INFO*	FindRevenge( TCHAR* ptszRevengeName );
	VOID					RemoveRevenge( DWORD dwGuid );
	// 리벤지 리스트 관련
	inline VOID				SetRevengePoint( BYTE byPoint ) { m_byRevengePoint = byPoint; }
	inline BYTE				GetRevengePoint() { return m_byRevengePoint; }
	inline VOID				IncRevengePoint() { if( m_byRevengePoint == UCHAR_MAX ) m_byRevengePoint++; }
	inline BOOL				DecRevengePoint() { if( m_byRevengePoint == 0 ) return FALSE; m_byRevengePoint--; return TRUE; }
	VOID					AddOnlineRevenge( User *pUser, BOOL bLoggedIn = TRUE );
	VOID					RemoveOnlineRevenge( User *pUser );
	User*					FindOnlineRevenge( DWORD CharGuid );
	VOID					AlarmRevengeStatChange( int nFlag, User *pUser );
//	inline VOID				SetRevengeTransaction( BOOL bTrans ) { m_bRevengeTrans = bTrans; }
//	inline BOOL				IsRevengeTransaction() { return m_bRevengeTrans; }
	VOID					SendOnlineRevengeAll( BYTE* pMsg, WORD wSize );

	inline VOID				SetOnlineLevel( LEVELTYPE &CharLevel )
	{
		m_OnlineLevel = CharLevel;
	}
	inline VOID				CheckOnlineLevel( LEVELTYPE &CharLevel )
	{
		if( m_OnlineLevel != 0 && CharLevel != m_OnlineLevel )
		{
			CharLevel = m_OnlineLevel;
		}
	}

	// 차단 정보 관리
	inline BYTE				GetBlockNum() { return (BYTE)m_mapBlock.size(); }
	STRUCT_BLOCK_INFO*		GetFirstBlock();
	STRUCT_BLOCK_INFO*		GetNextBlock();
	BOOL					AddBlockInfo( STRUCT_BLOCK_INFO* pBlock );
	STRUCT_BLOCK_INFO*		FindBlock( DWORD dwGuid );
	STRUCT_BLOCK_INFO*		FindBlock( TCHAR* ptszBlockName );
	VOID					RemoveBlock( DWORD dwGuid );
	VOID					RemoveBlock( TCHAR* ptszBlockedName );

	// 길드 정보 관리
//	inline BOOL				CheckGuildList() { return m_bGuildList; }
	BOOL					CheckGuildList();
	inline VOID				SetGuildList( BOOL bGuildList = TRUE ) { m_bGuildList = bGuildList; }
	inline VOID				SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid; }
	inline GUILDGUID		GetGuildGuid() { return m_GuildGuid; }

	// GM정보 관리
	inline VOID				SetGMGrade(BYTE byGMGrade) { m_byGMGrade = byGMGrade; }
	inline BYTE				GetGMGrade() { return m_byGMGrade; }
    //{__CN_000001_20090513_ADD_FRIEND_CHECK_GM
	inline BOOL		        IsGM() const { return (m_byGMGrade > eGM_GRADE_BASE) && (m_byGMGrade < eGM_GRADE_MAX); }
    INT                     CopyBlockInfo( STRUCT_BLOCK_INFO* pDstBlockInfo, const int size );
    INT                     CopyFriendInfo( STRUCT_FRIEND_INFO* pDstFriendInfo, const int size );
    INT                     CopyRevengeInfo( STRUCT_REVENGE_INFO* pDstRevengeInfo, const int size );
    // 삭제 _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    INT                     CopyRevengeInfo( STRUCT_REVENGE_PACKET_INFO* pDstRevengeInfo, const int size );
    //}

	BOOL					CheckChatTime();

	// 파티 관련
	inline VOID				JoinParty( CommChatZone *pParty ) { m_pParty = pParty; }
	inline VOID				RemoveParty() { m_pParty = NULL; }
	inline CommChatZone*	GetParty() { return m_pParty; }
	VOID					SendParty( BYTE* pMsg, WORD wSize );

	// 음성채팅
	inline DWORD			GetVoiceChatRequestGUID() { return m_dwRequestGUID; }
	inline VOID				RequestVoiceChat(DWORD dwGUID) { m_dwRequestGUID = dwGUID; }
	inline VOID				ClearVoiceChatRequest() { m_dwRequestGUID = 0; }
	inline BOOL				IsVoiceChatOn() { return m_bVoiceOn; }
	inline VOID				SetVoiceChat( BOOL bOn ) { m_bVoiceOn = bOn; }

	// PC방 정보
	inline DWORD			GetPCRoomID() { return m_dwPCRoomID; }
	VOID					SetPCRoomID( DWORD dwPCRoomID ) { m_dwPCRoomID = dwPCRoomID; }
    //!~ _NA_006413_20130201_PREMIUMSERVICE_ITEM
    bool                    CanApplyPCBangBenefit();
    void                    SetPremiumService(DWORD premiumservice);
    DWORD                   GetPremiumService();
    //~! _NA_006413_20130201_PREMIUMSERVICE_ITEM

	// 채팅금지 정보
	inline BOOL				IsChatBlocked() { return m_bChatBlock; }
	VOID					SetChatBlock( DWORD dwTime );
	VOID					FreeChatBlock();
	inline DWORD			GetBlockTime() { return m_dwBlockTime; }
	
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	BOOL					UpdateBlockTime();

	// 채널채팅 정보
	inline CommChatZone*	GetChannelChatRoom() { return m_pChannelChatRoom; }
	inline VOID				JoinChannelChatRoom( CommChatZone *pZone ) { m_pChannelChatRoom = pZone; }
	inline VOID				OutChannelChatRoom() { m_pChannelChatRoom = NULL; }
	BOOL					SendChannelChat( BYTE* pMsg, WORD wSize );

	BOOL					CheckMemoList();
	inline VOID				SetMemoTransaction( BOOL bTrans ) { m_bMemoTrans = bTrans; }
	inline BOOL				IsMemoTransaction() { return m_bMemoTrans; }

	ItemOwnFlag&			GetItemOwnFlag(){ return m_ItemOwnFlag; }
	VOID					SendChatBlock();
	VOID SendChatBlock(RC::eCHAT_RESULT result);

	inline VOID				SetCharLevel(LEVELTYPE Level){ m_CharLevel = Level;} 
	inline LEVELTYPE		GetCharLevel(){ return m_CharLevel; }

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK

	VOID			SendUseDecreeItem();
	inline VOID		SetTmpDecreeMsg(TCHAR* ptszMsg){ _tcsncpy(m_tszTmpDecreeMsg, ptszMsg, sizeof(TCHAR) * MAX_SHOUT_LEN); }
	inline TCHAR*	GetTmpDecreeMsg(){ return m_tszTmpDecreeMsg; }

	BOOL			SetInterceptionOption(eINTERCEPTION_TYPE eCode, BOOL bFlag);

protected:
	virtual VOID			OnAccept( DWORD dwNetworkIndex );
	virtual VOID			OnDisconnect();
	virtual VOID			OnRecv( BYTE *pMsg, WORD wSize );


	// <Cryptography>
	//   Cryptography & Sequence Number
public:
	BOOL					GetCryptography() { return m_bUseCryptography; }
	BOOL					SetCryptography( BOOL bInited ) { return m_sCrypt.m_CryptManager.StartLogic( m_bUseCryptography = bInited ); }

	VOID					NextRoundSNKey4Send() { m_sCryptBack.NextRoundSNKey4Send(); };
	// Cryptography
	BOOL SendPacket( BYTE *pMsg, WORD wSize );

	BYTE					InitCrypt( BYTE byProtoMajorVer, BYTE byProtoMinorVer )
	{
		m_sCryptBack.InitCryptInfo( GetGUID(), byProtoMajorVer, byProtoMinorVer );

		return m_sCryptBack.GetSendSNKey();
	};

	BYTE					GetCryptAlgorithmInfo( BYTE *pCryptInfo )
	{
		return m_sCryptBack.GetCryptAlgorithmInfo( m_sCryptBack.GetSendPairSNKey(), pCryptInfo, GetGUID() );
	}

	inline VOID				SetCrypt() { m_sCrypt = m_sCryptBack; SetCryptography(TRUE); m_bCrypting = FALSE; }
	inline BOOL				IsCrypting() { return m_bCrypting; }
	inline DWORD			ISCryptTimeOut( DWORD dwNowTime ) { __UNUSED(dwNowTime); return FALSE; }	//return dwNowTime > m_dwCryptTime; }
	inline VOID				SetCryptTime( DWORD dwTime ) { m_bCrypting = TRUE; m_dwCryptTime = dwTime + 100000; }
	inline VOID				EndCrypt() { m_bCrypting = FALSE; }
	inline VOID				CopyCrypt( const STRUCT_CRYPT &sCrypt ) { m_sCrypt.m_CryptManager.Release(); m_sCryptBack = sCrypt; }

	VOID					SetChatTimer( DWORD dwTimer ){ m_ChatTimer.SetTimer( dwTimer ); }
	BOOL					IsExpiredChatTimer(){ return m_ChatTimer.IsExpired(); }
	char*					GetLastChatMsg(){ return m_szLastChatMsg; }
	VOID					SetLastChatMsg( const char* szMsg );

protected:
	BOOL					Encrypt( BYTE* IN OUT pEncryptMsg, const int size, BYTE& OUT crc )
	{	return m_sCrypt.m_CryptManager.Encrypt( pEncryptMsg, size, crc );	}
	BOOL					Decrypt( BYTE* IN OUT pEncryptMsg, const int size, BYTE IN crc )
	{	return m_sCrypt.m_CryptManager.Decrypt( pEncryptMsg, size, crc );	}
protected:

	STRUCT_CRYPT			m_sCrypt;			// ACK 떨어지면 세팅
	STRUCT_CRYPT			m_sCryptBack;		// CMD 보낼 때 세팅

	BOOL					m_bUseCryptography;	// FALSE로 설정할 것
	BOOL					m_bCrypting;		// 암호화 키 변경 중인가?
	DWORD					m_dwCryptTime;		// 암호화 결정한 시각(KEY 보낸 시각)


private:

	BOOL					m_bLogoutProcessed;
	eUSER_TYPE				m_userType;
	DWORD					m_dwNetworkIndex;
	DWORD					m_dwGUID;
	DWORD					m_CharGuid;
	std::string				m_strCharName;
	DWORD					m_dwTimeoutTick;
	TCHAR					m_ptszAccountID[ID_MAX_LEN+1];	// 계정 ID
	char					m_szAuthIP[MAX_IP_STRING_LEN + 1];
	DWORD					m_dwChannelID;
	DWORD					m_dwZoneKey;
	eZONETYPE				m_status;
	DWORD					m_dwAuthID;

	//귓말 켬/끔 상태
	BOOL					m_bWhisper;			// TRUE면 귓말 켬, FALSE면 귓말 끔

	MAPCODE					m_MapCode;
	DWORD					m_dwFieldCode;		// 현재 속해 있는 필드 코드
	DWORD					m_nSectorIndex;	// 현재 속해 있는 섹터 인덱스
	BaseZone				*m_pNormalChatZone;

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	FriendMap				m_mapFriend;	
    // used by _KR_0_20091021_FIRENDINVITATION_EXTEND = {
	FriendMap				m_mapInvitateFriend;
	FriendMap				m_mapBeInvitatedFriend;
    // }
#else
	// 친구 정보 관리
	FRIEND_INFO_MAP			m_mapFriend;				// 친구 관리용 맵
	FRIEND_INFO_MAP_ITER	m_iterNowFriend;	// GetFirst...(), GetNext...() 에서 사용.
#endif
	BOOL					m_bFriendChatBlock;	// 친구 채팅 차단 여부(0: 허용 1: 거부)
	BOOL					m_bFriendTrans;		// 친구 관련 DB 작업중인가?(0: 아니오 1: 예)
	LEVELTYPE				m_OnlineLevel;		// 접속 중 레벨업 했을 때 친구 정보에 갱신 안 되는 문제 해결용(레벨업 없으면 0)
#ifndef _WHATTHEHELL_CHANGE_FRIENDCHECK_20060802
	BOOL					m_bFriendList;		// 친구 리스트 받았었나?(TRUE: 받았음, FALSE: 안 받았음)
#else
	DWORD					m_dwLastFriendTick;	// 마지막으로 친구 리스트 요청한 시각 - 확인 필수
#endif

	FRIEND_ONLINE_MAP		m_mapOnlineFriend;	// 온라인 친구 관리용 맵

	// 차단 정보 관리
	BLOCK_INFO_MAP			m_mapBlock;		// 차단 관리용 맵
	BLOCK_INFO_MAP_ITER		m_iterNowBlock;	// GetFirst...(), GetNext...() 에서 사용.

	//리벤지 정보 관리
	REVENGE_INFO_MAP		m_mapRevenge;		// 빚 받아야 할 유저들 관리용 맵
	REVENGE_INFO_MAP_ITER	m_iterNowRevenge;	// GetFirst...(), GetNext...() 에서 사용.
	BYTE					m_byRevengePoint;	// 리벤지 포인트

	FRIEND_ONLINE_MAP		m_mapOnlineRevenge;	// 온라인인 빚쟁이들 관리용 맵

	// 길드 관련
	BOOL					m_bGuildList;	// 길드 리스트 받았었나?(TRUE:받았음, FALSE: 안 받았음)
	GUILDGUID				m_GuildGuid;	// 소속 길드 guid
	DWORD					m_dwLastGuildTick;	// 마지막으로 길드 리스트 요청한 시각

	// 어뷰즈 체크
	BYTE					m_byChatCount;		// 일정 시간 안에 채팅을 몇 번 날렸냐?
	DWORD					m_dwLastChatTime;	// 마지막으로 채팅 날린 시각

	// GM Check
	BYTE					m_byGMGrade;		// GM맞냐?

	// 파티 관련
	CommChatZone*			m_pParty;

	// 음성채팅 정보
	DWORD					m_dwRequestGUID;	// 채팅을 요청한 상대의 GUID
	BOOL					m_bVoiceOn;			// 음성 채팅 허용 여부(TRUE: 허용 FALSE: 금지)

	// PC방 사용자 정보
	DWORD					m_dwPCRoomID;		// 0이면 일반 사용자. 1 이상이면 PC방 ID
    DWORD                   m_dwPremiumService; // _NA_006413_20130201_PREMIUMSERVICE_ITEM

	// 채팅금지 여부
	BOOL					m_bChatBlock;		// 채팅금지 여부(TRUE: 금지 FALSE: 허용)
	DWORD					m_dwBlockTime;		// 채팅금지 남은 시간(ms 단위 - DB 저장할 때나 요청 받을 때는 분단위이므로 주의)
	DWORD					m_dwBlockLastTime;	// 시간 차감의 기준 시각

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK

	DWORD					m_dwLastShoutTime;	// 마지막 외친 시각

	// 채널 채팅 정보
	CommChatZone*			m_pChannelChatRoom;	// 현재 들어가 있는 채널 채팅방(NULL이면 없음)

	DWORD					m_dwLastMemoTick;	// 쪽지 목록 마지막으로 요청한 시각
	BOOL					m_bMemoTrans;		// 메모 트랜잭션중

	ItemOwnFlag				m_ItemOwnFlag;

	LEVELTYPE				m_CharLevel;

	util::Timer				m_ChatTimer;
	char					m_szLastChatMsg[MAX_CHATMSG_SIZE+1];
	TCHAR					m_tszTmpDecreeMsg[MAX_SHOUT_LEN + 1];

	//그룹 시스템 정보 __KR_001355_20090608_GROUPINGSYSTEM_DATA = {
private :
	eGROUP_STAT					m_eHighMember;              //상위 맴버의 상태
    eGROUP_STAT                 m_eLowMember;               //하위 맴버의 상태
    GROUP_MEMBER_MAP            m_GroupMemberMap;           //그룹원 Map
    GROUP_MEMBER_MAP_ITER       m_curGroupMemberiter;       //그룹 맴버를 가리키는 iter
    STRUCT_GROUP_MEMBER_INFO    m_tempMemberInfo;           //맴버 정보를 임시로 저장하는 변수
    BYTE                        m_CharClass;                //해당 케릭터 클레스(직업)
    BOOL                        m_bGroupTrans;              //DB작업 트렌 젝션 용
    BOOL                        m_bAddBlock;                //현재 그룹 맴버 추가 토글이 켜있는가?
    MONEY                       m_curBonusMoney;            //가장 최근에 받은 보너스 Money
    EXPTYPE                     m_curBonusEXP;              //가장 최근에 받은 보너스 EXP
    EXPTYPE                     m_curAccumEXP;              //최근에 받은 보너스 AccumEXP
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    BYTE                        gender_;                    //해당 캐릭터의 성별
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_DATA'
};
