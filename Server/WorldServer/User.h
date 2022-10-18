#pragma once

#include <NetworkObject.h>
#include <string>

// ģ�� ���� ���� include
#include <map>

// ��ȣȭ ó�� ���� include
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
	// Ŭ���̾�Ʈ�� ����� ���� �ݵ�� ��ȣȭ ���θ� üũ�ϴ� SendPacket()�� ��� �ϹǷ� protected ����� ����.
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

	inline VOID				SetWhisper(BOOL bWhisper)	{ m_bWhisper = bWhisper; }	// �Ӹ� ���� ������ ��Ʈ. �⺻ ���� TRUE(���)
	inline BOOL				CanWhisper()	{	return m_bWhisper;	}				// �Ӹ� �޴� ��������(TRUE), �ƴ���(FALSE)
	void					OnLogString(char *pszLog);								// ���

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
	// ģ�� ���� ����
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
	// ģ�� ����
	VOID					AddOnlineFriend( User *pUser, BOOL bLoggedIn = TRUE );
	VOID					RemoveOnlineFriend( User *pUser );
	User*					FindOnlineFriend( DWORD CharGuid );
	VOID					AlarmFriendStatChange( int nFlag, User *pUser );
	inline VOID				SetFriendTransaction( BOOL bTrans ) { m_bFriendTrans = bTrans; }
	inline BOOL				IsFriendTransaction() { return m_bFriendTrans; }
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    RC::eFRIEND_RESULT      CheckConditionAccessFriendInfo(CHARGUID friendguid);

    // implemented by __KR_001355_20090608_GROUPINGSYSTEM_DATA = {
    //���� DB�۾� Ʈ������
    inline BOOL             IsGroupTransaction() { return m_bGroupTrans; }                                      
    //Ʈ������ ����
    inline VOID				SetGroupTransaction( BOOL bTrans ) { m_bGroupTrans = bTrans; }                      
    BYTE                    GetGroupMemberCount();
    //�ش� �ɸ��� Ŭ���� ����
    inline VOID             SetCharClass( BYTE charClass ){ m_CharClass = charClass; }                          
    //�ش� �ɸ��� Ŭ���� ��������
    inline BYTE             GetCharClass( ){ return m_CharClass; }                                              
    //�ֱٿ� ���� ���ʽ� ���Ӽ���
    inline VOID             SetCurBonusMoney( MONEY curBonusMoney ){ m_curBonusMoney = curBonusMoney; }        
    //�ֱٿ� ���� ���ʽ� ���� ���
    inline MONEY            GetCurBonusMoney(){ return m_curBonusMoney; }                                      
    //�ֱٿ� ���� ���ʽ� ����ġ ����
    inline VOID             SetCurBonusEXP( EXPTYPE curBonusEXP ){ m_curBonusEXP = curBonusEXP; }               
    //�ֱٿ� ���� ���ʽ� ����ġ ���
    inline EXPTYPE          GetCurBonusEXP(){ return m_curBonusEXP; }                                           
    //�ɹ� �߰� �ź� ��� ����
    inline VOID             SetAddBlock( BOOL bAddBlock ) { m_bAddBlock = bAddBlock; }                          
    //�ɹ� �߰� �ź� ��� ���� ��������
    inline BOOL             GetAddBlockInfo() { return m_bAddBlock; }                                           
    //���� ���� ����ġ ����
    inline VOID             SetCurAccumEXP( EXPTYPE curAccumEXP ) { m_curAccumEXP = curAccumEXP; }              
    //���� ���� ����ġ �������� �ִ� ���� ����ġ�� 500���̴�.
    inline EXPTYPE          GetCurAccumEXP( ) { return ( m_curAccumEXP > MAX_GROUP_EXP)? 
                                                MAX_GROUP_EXP : m_curAccumEXP; }   
    inline VOID             AddCurAccumEXP( EXPTYPE curAccumEXP ) { m_curAccumEXP += curAccumEXP; }
    //�׷� ����Ʈ�� ��û���� �ֳ�?
    BOOL					CheckGroupList();                                                                   
    VOID                    InitGroupInfo();
    //�ӽ� �ɹ� ���� ����ҿ� �ش��ϴ� ���� ���� ����
    VOID                    SetStructMemberInfo( User* pUser, eGROUP_STAT eMemberStat );                        
    //�ӽ� ��� ���� ����� ��������
    inline STRUCT_GROUP_MEMBER_INFO* GetStructMemberInfo() { return &m_tempMemberInfo; }                        
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    inline VOID             SetCharGender( BYTE gender ){ gender_ = gender; }                          
    //�ش� �ɸ��� Ŭ���� ��������
    inline BYTE             GetCharGender( ){ return gender_; }                                              
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_DATA'
	VOID					SendOnlineFriendAll( BYTE* pMsg, WORD wSize );

	// ������ ���� ����
	inline BYTE				GetRevengeNum() { return (BYTE)m_mapRevenge.size(); }
	STRUCT_REVENGE_INFO*	GetFirstRevenge();
	STRUCT_REVENGE_INFO*	GetNextRevenge();
	BOOL					AddRevengeInfo( STRUCT_REVENGE_INFO* pRevenge );
	STRUCT_REVENGE_INFO*	FindRevenge( DWORD dwCharGuid );
	STRUCT_REVENGE_INFO*	FindRevenge( TCHAR* ptszRevengeName );
	VOID					RemoveRevenge( DWORD dwGuid );
	// ������ ����Ʈ ����
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

	// ���� ���� ����
	inline BYTE				GetBlockNum() { return (BYTE)m_mapBlock.size(); }
	STRUCT_BLOCK_INFO*		GetFirstBlock();
	STRUCT_BLOCK_INFO*		GetNextBlock();
	BOOL					AddBlockInfo( STRUCT_BLOCK_INFO* pBlock );
	STRUCT_BLOCK_INFO*		FindBlock( DWORD dwGuid );
	STRUCT_BLOCK_INFO*		FindBlock( TCHAR* ptszBlockName );
	VOID					RemoveBlock( DWORD dwGuid );
	VOID					RemoveBlock( TCHAR* ptszBlockedName );

	// ��� ���� ����
//	inline BOOL				CheckGuildList() { return m_bGuildList; }
	BOOL					CheckGuildList();
	inline VOID				SetGuildList( BOOL bGuildList = TRUE ) { m_bGuildList = bGuildList; }
	inline VOID				SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid; }
	inline GUILDGUID		GetGuildGuid() { return m_GuildGuid; }

	// GM���� ����
	inline VOID				SetGMGrade(BYTE byGMGrade) { m_byGMGrade = byGMGrade; }
	inline BYTE				GetGMGrade() { return m_byGMGrade; }
    //{__CN_000001_20090513_ADD_FRIEND_CHECK_GM
	inline BOOL		        IsGM() const { return (m_byGMGrade > eGM_GRADE_BASE) && (m_byGMGrade < eGM_GRADE_MAX); }
    INT                     CopyBlockInfo( STRUCT_BLOCK_INFO* pDstBlockInfo, const int size );
    INT                     CopyFriendInfo( STRUCT_FRIEND_INFO* pDstFriendInfo, const int size );
    INT                     CopyRevengeInfo( STRUCT_REVENGE_INFO* pDstRevengeInfo, const int size );
    // ���� _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    INT                     CopyRevengeInfo( STRUCT_REVENGE_PACKET_INFO* pDstRevengeInfo, const int size );
    //}

	BOOL					CheckChatTime();

	// ��Ƽ ����
	inline VOID				JoinParty( CommChatZone *pParty ) { m_pParty = pParty; }
	inline VOID				RemoveParty() { m_pParty = NULL; }
	inline CommChatZone*	GetParty() { return m_pParty; }
	VOID					SendParty( BYTE* pMsg, WORD wSize );

	// ����ä��
	inline DWORD			GetVoiceChatRequestGUID() { return m_dwRequestGUID; }
	inline VOID				RequestVoiceChat(DWORD dwGUID) { m_dwRequestGUID = dwGUID; }
	inline VOID				ClearVoiceChatRequest() { m_dwRequestGUID = 0; }
	inline BOOL				IsVoiceChatOn() { return m_bVoiceOn; }
	inline VOID				SetVoiceChat( BOOL bOn ) { m_bVoiceOn = bOn; }

	// PC�� ����
	inline DWORD			GetPCRoomID() { return m_dwPCRoomID; }
	VOID					SetPCRoomID( DWORD dwPCRoomID ) { m_dwPCRoomID = dwPCRoomID; }
    //!~ _NA_006413_20130201_PREMIUMSERVICE_ITEM
    bool                    CanApplyPCBangBenefit();
    void                    SetPremiumService(DWORD premiumservice);
    DWORD                   GetPremiumService();
    //~! _NA_006413_20130201_PREMIUMSERVICE_ITEM

	// ä�ñ��� ����
	inline BOOL				IsChatBlocked() { return m_bChatBlock; }
	VOID					SetChatBlock( DWORD dwTime );
	VOID					FreeChatBlock();
	inline DWORD			GetBlockTime() { return m_dwBlockTime; }
	
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	BOOL					UpdateBlockTime();

	// ä��ä�� ����
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

	STRUCT_CRYPT			m_sCrypt;			// ACK �������� ����
	STRUCT_CRYPT			m_sCryptBack;		// CMD ���� �� ����

	BOOL					m_bUseCryptography;	// FALSE�� ������ ��
	BOOL					m_bCrypting;		// ��ȣȭ Ű ���� ���ΰ�?
	DWORD					m_dwCryptTime;		// ��ȣȭ ������ �ð�(KEY ���� �ð�)


private:

	BOOL					m_bLogoutProcessed;
	eUSER_TYPE				m_userType;
	DWORD					m_dwNetworkIndex;
	DWORD					m_dwGUID;
	DWORD					m_CharGuid;
	std::string				m_strCharName;
	DWORD					m_dwTimeoutTick;
	TCHAR					m_ptszAccountID[ID_MAX_LEN+1];	// ���� ID
	char					m_szAuthIP[MAX_IP_STRING_LEN + 1];
	DWORD					m_dwChannelID;
	DWORD					m_dwZoneKey;
	eZONETYPE				m_status;
	DWORD					m_dwAuthID;

	//�Ӹ� ��/�� ����
	BOOL					m_bWhisper;			// TRUE�� �Ӹ� ��, FALSE�� �Ӹ� ��

	MAPCODE					m_MapCode;
	DWORD					m_dwFieldCode;		// ���� ���� �ִ� �ʵ� �ڵ�
	DWORD					m_nSectorIndex;	// ���� ���� �ִ� ���� �ε���
	BaseZone				*m_pNormalChatZone;

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	FriendMap				m_mapFriend;	
    // used by _KR_0_20091021_FIRENDINVITATION_EXTEND = {
	FriendMap				m_mapInvitateFriend;
	FriendMap				m_mapBeInvitatedFriend;
    // }
#else
	// ģ�� ���� ����
	FRIEND_INFO_MAP			m_mapFriend;				// ģ�� ������ ��
	FRIEND_INFO_MAP_ITER	m_iterNowFriend;	// GetFirst...(), GetNext...() ���� ���.
#endif
	BOOL					m_bFriendChatBlock;	// ģ�� ä�� ���� ����(0: ��� 1: �ź�)
	BOOL					m_bFriendTrans;		// ģ�� ���� DB �۾����ΰ�?(0: �ƴϿ� 1: ��)
	LEVELTYPE				m_OnlineLevel;		// ���� �� ������ ���� �� ģ�� ������ ���� �� �Ǵ� ���� �ذ��(������ ������ 0)
#ifndef _WHATTHEHELL_CHANGE_FRIENDCHECK_20060802
	BOOL					m_bFriendList;		// ģ�� ����Ʈ �޾Ҿ���?(TRUE: �޾���, FALSE: �� �޾���)
#else
	DWORD					m_dwLastFriendTick;	// ���������� ģ�� ����Ʈ ��û�� �ð� - Ȯ�� �ʼ�
#endif

	FRIEND_ONLINE_MAP		m_mapOnlineFriend;	// �¶��� ģ�� ������ ��

	// ���� ���� ����
	BLOCK_INFO_MAP			m_mapBlock;		// ���� ������ ��
	BLOCK_INFO_MAP_ITER		m_iterNowBlock;	// GetFirst...(), GetNext...() ���� ���.

	//������ ���� ����
	REVENGE_INFO_MAP		m_mapRevenge;		// �� �޾ƾ� �� ������ ������ ��
	REVENGE_INFO_MAP_ITER	m_iterNowRevenge;	// GetFirst...(), GetNext...() ���� ���.
	BYTE					m_byRevengePoint;	// ������ ����Ʈ

	FRIEND_ONLINE_MAP		m_mapOnlineRevenge;	// �¶����� �����̵� ������ ��

	// ��� ����
	BOOL					m_bGuildList;	// ��� ����Ʈ �޾Ҿ���?(TRUE:�޾���, FALSE: �� �޾���)
	GUILDGUID				m_GuildGuid;	// �Ҽ� ��� guid
	DWORD					m_dwLastGuildTick;	// ���������� ��� ����Ʈ ��û�� �ð�

	// ����� üũ
	BYTE					m_byChatCount;		// ���� �ð� �ȿ� ä���� �� �� ���ȳ�?
	DWORD					m_dwLastChatTime;	// ���������� ä�� ���� �ð�

	// GM Check
	BYTE					m_byGMGrade;		// GM�³�?

	// ��Ƽ ����
	CommChatZone*			m_pParty;

	// ����ä�� ����
	DWORD					m_dwRequestGUID;	// ä���� ��û�� ����� GUID
	BOOL					m_bVoiceOn;			// ���� ä�� ��� ����(TRUE: ��� FALSE: ����)

	// PC�� ����� ����
	DWORD					m_dwPCRoomID;		// 0�̸� �Ϲ� �����. 1 �̻��̸� PC�� ID
    DWORD                   m_dwPremiumService; // _NA_006413_20130201_PREMIUMSERVICE_ITEM

	// ä�ñ��� ����
	BOOL					m_bChatBlock;		// ä�ñ��� ����(TRUE: ���� FALSE: ���)
	DWORD					m_dwBlockTime;		// ä�ñ��� ���� �ð�(ms ���� - DB ������ ���� ��û ���� ���� �д����̹Ƿ� ����)
	DWORD					m_dwBlockLastTime;	// �ð� ������ ���� �ð�

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK

	DWORD					m_dwLastShoutTime;	// ������ ��ģ �ð�

	// ä�� ä�� ����
	CommChatZone*			m_pChannelChatRoom;	// ���� �� �ִ� ä�� ä�ù�(NULL�̸� ����)

	DWORD					m_dwLastMemoTick;	// ���� ��� ���������� ��û�� �ð�
	BOOL					m_bMemoTrans;		// �޸� Ʈ�������

	ItemOwnFlag				m_ItemOwnFlag;

	LEVELTYPE				m_CharLevel;

	util::Timer				m_ChatTimer;
	char					m_szLastChatMsg[MAX_CHATMSG_SIZE+1];
	TCHAR					m_tszTmpDecreeMsg[MAX_SHOUT_LEN + 1];

	//�׷� �ý��� ���� __KR_001355_20090608_GROUPINGSYSTEM_DATA = {
private :
	eGROUP_STAT					m_eHighMember;              //���� �ɹ��� ����
    eGROUP_STAT                 m_eLowMember;               //���� �ɹ��� ����
    GROUP_MEMBER_MAP            m_GroupMemberMap;           //�׷�� Map
    GROUP_MEMBER_MAP_ITER       m_curGroupMemberiter;       //�׷� �ɹ��� ����Ű�� iter
    STRUCT_GROUP_MEMBER_INFO    m_tempMemberInfo;           //�ɹ� ������ �ӽ÷� �����ϴ� ����
    BYTE                        m_CharClass;                //�ش� �ɸ��� Ŭ����(����)
    BOOL                        m_bGroupTrans;              //DB�۾� Ʈ�� ���� ��
    BOOL                        m_bAddBlock;                //���� �׷� �ɹ� �߰� ����� ���ִ°�?
    MONEY                       m_curBonusMoney;            //���� �ֱٿ� ���� ���ʽ� Money
    EXPTYPE                     m_curBonusEXP;              //���� �ֱٿ� ���� ���ʽ� EXP
    EXPTYPE                     m_curAccumEXP;              //�ֱٿ� ���� ���ʽ� AccumEXP
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    BYTE                        gender_;                    //�ش� ĳ������ ����
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_DATA'
};
