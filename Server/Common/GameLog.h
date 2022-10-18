#pragma once

#include <Constant.h>
#include <WriteLogMgr.h>
#include <WZTime.h>
#include <vector>
#include <windows.h>
#include <process.h>
using namespace std;

//인첸트시 사용 보석정보
struct COMPOSITEMATERIAL
{
	int		nMaterialCode;
	WORD	wCntMaterial;
};

//아이템거래시 사용되는 아이템 정보
struct ITEMLOGINFO
{
	int		nItemCode;
	int		nItemSerial;
	int		nItemCount;
	DWORD	dwMoney;
};

enum eREGULAR_LOGTYPE
{	
	REGULAR_SKILL = 0,
};

//Thread Argument
unsigned int __stdcall SkillWorkThread(void* pArg);


//게임로그 인터페이스 클래스
class CGameLog
{
public:
	CGameLog(void);
	~CGameLog(void);

private:
	CWriteLogMgr m_LogMng;
	WZTime		 m_Time;
	BOOL		 m_bWriteText;
	
public:
	BOOL Init( TCHAR* pszDirName = NULL, TCHAR* pszFilenamePrefix = NULL, TCHAR* pszTextDirName = NULL );
	void WriteBaseData( BaseData* pData, TCHAR* szAccountID, TCHAR* szCharName );

//////////////////////////////////////////////////////////////////////////
// 경험치 로그 
	// 1. 레벨업 로그
	void WriteLevelUp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwOldExp, DWORD dwNewExp,
		WORD wOldLevel, WORD wNewLevel );
	// 2. 레벨 50이상시 경험치 10등분 로그
	void WriteDivideExp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwOldExp, DWORD dwNewExp,
		WORD wOldLevel, WORD wNewLevel, WORD wDivideCount );
	// 3. 경험치 추가/다운
	void WriteDownExp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwTotalExp, DWORD dwChangeExp,
		WORD wLevel );

//////////////////////////////////////////////////////////////////////////
// 아이템 로그
	// 1. 아이템 드롭(땅에 떨굼) 과 줍기
	void WriteItemOwner( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bOwner, int nItemCode, int nItemSerial, 
		int nItemCount, int nMapCode, int nPosX, int nPosY, int nPosZ);
	// 2. 인첸트 성공/실패
	void WriteEnchantResult( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bSuccess, int nItemCode,
		int nItemSerial, vector<COMPOSITEMATERIAL> vecMatInfo );
	// 3. NPC와의 거래
	void WriteItemTradeWithNPC( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bSell, int nItemCode, 
		int nItemSerial, int nItemCount, DWORD dwMoney );
	// 4. 아이템 개인거래
	void WriteItemTradeWithPlayer( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, BOOL bGive, ITEMLOGINFO ItemInfo, 
		TCHAR* szPlayer2_ID, TCHAR* szPlayer2_CharName);
	// 5. 돈 개인거래
	void WriteMoneyTradeWithPlayer( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, BOOL bGive, DWORD dwMoney, 
		TCHAR* szPlayer2_ID, TCHAR* szPlayer2_CharName);
	// 6. 개인상점 거래
	void WriteItemVendorWithPlayer( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, BOOL bSell, ITEMLOGINFO ItemInfo, 
		DWORD dwMoney, TCHAR* szPlayer2_ID, TCHAR* szPlayer2_CharName);
	// 7. 아이템 랭크업
	void WriteItemRankUpResult( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, ITEMLOGINFO ItemInfo, 
		DWORD dwMoney, BOOL bCreateSocket );
	// 8. 창고에 맞기기 / 찾기
	void WriteItemMoveInWareHouse( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bKeep, ITEMLOGINFO ItemInfo );
	// 9. 인첸트 시도
	void WriteEnchantStart( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, vector<COMPOSITEMATERIAL> vecMatInfo );
	// 10. 몬스터 아이템 드랍
	void WriteMonsterItemDrop( int nServerCode, ITEMLOGINFO ItemInfo, DWORD dwMonsterCode, DWORD dwMonsterLevel, int nMapCode,
		int nPosX, int nPosY, int nPosZ );
	// 11. 아이템 개인거래 요청 / 승인 / 요청거부
	void WriteItemTradeStatus( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, TCHAR* szPlayer2_ID,
		TCHAR* szPlayer2_CharName, int nType );
	// 12. 개인상점 개설/폐쇄
	void WriteItemVendorStatus( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bOpen, DWORD dwMoney, 
		int nMapCode, int nPosX, int nPosY, int nPosZ );
	// 13. 개인상점 보기
	void WriteViewItemVendor( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, TCHAR* szPlayer2_ID,
		TCHAR* szPlayer2_CharName );
	// 14. 창고 정보
	void WriteWareHouseInfo( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, vector<ITEMLOGINFO> vecItem, DWORD dwMoney );
	// 15. 개인상점 정보
	void WriteVendorInfo( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, vector<ITEMLOGINFO> vecItem );


//////////////////////////////////////////////////////////////////////////
// 유저액션 로그
	// 1. 부활 로그
	void WritePlayerResurrection( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwExp, int nMapCode, int nPosX, 
		int nPosY, int nPosZ );
	// 2. 사망 로그
	void WritePlayerDead( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwExp, int nMapCode, int nPosX, 
		int nPosY, int nPosZ );
	// 3. 로그인시 인벤토리 정보 남기기
	void WriteInvenInfoOnLogin( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode );
	// 4. 스텟사용 로그
	void WriteUseStat( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, byte byStatType );
	// 5. 캐릭터 생성 
	void WriteCharacter( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bCreate );
	// 6. 마을로 이동
	void WriteMoveVillage( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, int nMapCode );
	// 7. 스킬레벨업
	void WriteSkillLevelUp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, int nSkillCode, BOOL bLevelUp );


//////////////////////////////////////////////////////////////////////////
// 세션 로그 
	// 1. 플레이어 로그인 
	void WritePlayerLogin( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, TCHAR* szIP, WORD wClass, WORD wLevel );
	// 2. 플레이어 로그인 실패
	void WritePlayerLoginFail( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, TCHAR* szIP );
	// 3. 로그아웃
	void WritePlayerLogOut( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, TCHAR* szIP, TCHAR* szLoginTime,
		WORD wClass, WORD wLevel );
	// 4. 캐릭터 접속 / 종료
	void WriteCharConnect( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bConnect );

//////////////////////////////////////////////////////////////////////////
// 채팅 로그
	// 1. 마을 채팅
	void WriteVillageChat( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, short sMapCode, TCHAR* szChatMsg );
	// 2. 외치기
	void WriteShoutChat( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, short sMapCode, TCHAR* szChatMsg );
	// 3. 배틀존 채팅
	void WriteBattleZoneChat( TCHAR* szAccountID, TCHAR* szCharName, int nRoomNo, int nServerCode, short sMapCode, TCHAR* szChatMsg );
	// 4. 길드 채팅
	void WriteGuildChat( TCHAR* szAccountID, TCHAR* szCharName, TCHAR* szGuildName, int nServerCode, short sMapCode, TCHAR* szChatMsg );
	// 5. 귓속말
	void WriteWhisperChat( TCHAR* szAccountID, TCHAR* szCharName, TCHAR* szRecvCharName, int nServerCode, TCHAR* szChatMsg );
	// 6. 친구 채팅(1:1 채팅)
	void WriteFriendChat( TCHAR* szAccountID, TCHAR* szCharName, TCHAR* szRecvCharName, int nServerCode, TCHAR* szChatMsg );

//////////////////////////////////////////////////////////////////////////
// 정기적인 게임로그
	void StartRegularLog( int nLogType );


//////////////////////////////////////////////////////////////////////////
// 정기적인 게임로그 쓰레드
	friend unsigned int __stdcall SkillWorkThread(void* pArg);
};
