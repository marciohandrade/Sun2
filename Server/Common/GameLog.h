#pragma once

#include <Constant.h>
#include <WriteLogMgr.h>
#include <WZTime.h>
#include <vector>
#include <windows.h>
#include <process.h>
using namespace std;

//��þƮ�� ��� ��������
struct COMPOSITEMATERIAL
{
	int		nMaterialCode;
	WORD	wCntMaterial;
};

//�����۰ŷ��� ���Ǵ� ������ ����
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


//���ӷα� �������̽� Ŭ����
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
// ����ġ �α� 
	// 1. ������ �α�
	void WriteLevelUp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwOldExp, DWORD dwNewExp,
		WORD wOldLevel, WORD wNewLevel );
	// 2. ���� 50�̻�� ����ġ 10��� �α�
	void WriteDivideExp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwOldExp, DWORD dwNewExp,
		WORD wOldLevel, WORD wNewLevel, WORD wDivideCount );
	// 3. ����ġ �߰�/�ٿ�
	void WriteDownExp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwTotalExp, DWORD dwChangeExp,
		WORD wLevel );

//////////////////////////////////////////////////////////////////////////
// ������ �α�
	// 1. ������ ���(���� ����) �� �ݱ�
	void WriteItemOwner( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bOwner, int nItemCode, int nItemSerial, 
		int nItemCount, int nMapCode, int nPosX, int nPosY, int nPosZ);
	// 2. ��þƮ ����/����
	void WriteEnchantResult( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bSuccess, int nItemCode,
		int nItemSerial, vector<COMPOSITEMATERIAL> vecMatInfo );
	// 3. NPC���� �ŷ�
	void WriteItemTradeWithNPC( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bSell, int nItemCode, 
		int nItemSerial, int nItemCount, DWORD dwMoney );
	// 4. ������ ���ΰŷ�
	void WriteItemTradeWithPlayer( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, BOOL bGive, ITEMLOGINFO ItemInfo, 
		TCHAR* szPlayer2_ID, TCHAR* szPlayer2_CharName);
	// 5. �� ���ΰŷ�
	void WriteMoneyTradeWithPlayer( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, BOOL bGive, DWORD dwMoney, 
		TCHAR* szPlayer2_ID, TCHAR* szPlayer2_CharName);
	// 6. ���λ��� �ŷ�
	void WriteItemVendorWithPlayer( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, BOOL bSell, ITEMLOGINFO ItemInfo, 
		DWORD dwMoney, TCHAR* szPlayer2_ID, TCHAR* szPlayer2_CharName);
	// 7. ������ ��ũ��
	void WriteItemRankUpResult( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, ITEMLOGINFO ItemInfo, 
		DWORD dwMoney, BOOL bCreateSocket );
	// 8. â�� �±�� / ã��
	void WriteItemMoveInWareHouse( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bKeep, ITEMLOGINFO ItemInfo );
	// 9. ��þƮ �õ�
	void WriteEnchantStart( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, vector<COMPOSITEMATERIAL> vecMatInfo );
	// 10. ���� ������ ���
	void WriteMonsterItemDrop( int nServerCode, ITEMLOGINFO ItemInfo, DWORD dwMonsterCode, DWORD dwMonsterLevel, int nMapCode,
		int nPosX, int nPosY, int nPosZ );
	// 11. ������ ���ΰŷ� ��û / ���� / ��û�ź�
	void WriteItemTradeStatus( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, TCHAR* szPlayer2_ID,
		TCHAR* szPlayer2_CharName, int nType );
	// 12. ���λ��� ����/���
	void WriteItemVendorStatus( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bOpen, DWORD dwMoney, 
		int nMapCode, int nPosX, int nPosY, int nPosZ );
	// 13. ���λ��� ����
	void WriteViewItemVendor( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, TCHAR* szPlayer2_ID,
		TCHAR* szPlayer2_CharName );
	// 14. â�� ����
	void WriteWareHouseInfo( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, vector<ITEMLOGINFO> vecItem, DWORD dwMoney );
	// 15. ���λ��� ����
	void WriteVendorInfo( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, vector<ITEMLOGINFO> vecItem );


//////////////////////////////////////////////////////////////////////////
// �����׼� �α�
	// 1. ��Ȱ �α�
	void WritePlayerResurrection( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwExp, int nMapCode, int nPosX, 
		int nPosY, int nPosZ );
	// 2. ��� �α�
	void WritePlayerDead( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwExp, int nMapCode, int nPosX, 
		int nPosY, int nPosZ );
	// 3. �α��ν� �κ��丮 ���� �����
	void WriteInvenInfoOnLogin( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode );
	// 4. ���ݻ�� �α�
	void WriteUseStat( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, byte byStatType );
	// 5. ĳ���� ���� 
	void WriteCharacter( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bCreate );
	// 6. ������ �̵�
	void WriteMoveVillage( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, int nMapCode );
	// 7. ��ų������
	void WriteSkillLevelUp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, int nSkillCode, BOOL bLevelUp );


//////////////////////////////////////////////////////////////////////////
// ���� �α� 
	// 1. �÷��̾� �α��� 
	void WritePlayerLogin( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, TCHAR* szIP, WORD wClass, WORD wLevel );
	// 2. �÷��̾� �α��� ����
	void WritePlayerLoginFail( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, TCHAR* szIP );
	// 3. �α׾ƿ�
	void WritePlayerLogOut( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, TCHAR* szIP, TCHAR* szLoginTime,
		WORD wClass, WORD wLevel );
	// 4. ĳ���� ���� / ����
	void WriteCharConnect( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bConnect );

//////////////////////////////////////////////////////////////////////////
// ä�� �α�
	// 1. ���� ä��
	void WriteVillageChat( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, short sMapCode, TCHAR* szChatMsg );
	// 2. ��ġ��
	void WriteShoutChat( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, short sMapCode, TCHAR* szChatMsg );
	// 3. ��Ʋ�� ä��
	void WriteBattleZoneChat( TCHAR* szAccountID, TCHAR* szCharName, int nRoomNo, int nServerCode, short sMapCode, TCHAR* szChatMsg );
	// 4. ��� ä��
	void WriteGuildChat( TCHAR* szAccountID, TCHAR* szCharName, TCHAR* szGuildName, int nServerCode, short sMapCode, TCHAR* szChatMsg );
	// 5. �ӼӸ�
	void WriteWhisperChat( TCHAR* szAccountID, TCHAR* szCharName, TCHAR* szRecvCharName, int nServerCode, TCHAR* szChatMsg );
	// 6. ģ�� ä��(1:1 ä��)
	void WriteFriendChat( TCHAR* szAccountID, TCHAR* szCharName, TCHAR* szRecvCharName, int nServerCode, TCHAR* szChatMsg );

//////////////////////////////////////////////////////////////////////////
// �������� ���ӷα�
	void StartRegularLog( int nLogType );


//////////////////////////////////////////////////////////////////////////
// �������� ���ӷα� ������
	friend unsigned int __stdcall SkillWorkThread(void* pArg);
};
