#pragma once
//=======================================================================================================================
/// 
/**
	@history
			- 2006.5.17 : [taiyo] 
				1. ServerCode ����
*/
//=======================================================================================================================

#include "LogCommonDefine.h"

/*
struct _BaseData
{
	eLOG_CATEGORY		m_eCategory;
	int					m_nLogType;
	TCHAR				m_szWriteTime[MAX_STRING_TIME_SIZE+1];
	TCHAR				m_szAccoutID[MAX_ACCOUT_ID_SIZE+1];
	TCHAR				m_szCharName[MAX_CHAR_NAME_SIZE+1];
	TCHAR 				m_szUserIP[MAX_IP_ADDR_SIZE+1];					//< Ŭ���̾�ƮIP
	WORD				m_wLogSize;
	BYTE				m_byAddInfoCount;
	TCHAR				m_szAddInfo[MAX_ADDINFO_SIZE+1];
	int					GetSize()
	{
						// NULL���ڿ������ϱ� ���� : MAX_ADDINFO_SIZE+1�� ��
						return (int)(sizeof(_BaseData) - sizeof(TCHAR)*(MAX_ADDINFO_SIZE-m_byAddInfoCount));
	}
};

struct _SessionData
{
	BYTE				m_byLogoutType;									//< eLOG_SESSION_LOGOUT_TYPE ����
	TCHAR				m_tszLoginTime[MAX_STRING_TIME_SIZE+1];
	DWORD				m_dwUsedTime;									//< ���ð� (�д���)
};

struct _ItemData
{
	WORD 				m_wMapCode;
	SHORT				m_sMapX;
	SHORT 				m_sMapY;
	SHORT 				m_sMapZ;
	TCHAR 				m_szToAccountID[MAX_ACCOUT_ID_SIZE+1];			//< ��Ʈ��id
	TCHAR 				m_szToCharName[MAX_CHAR_NAME_SIZE+1];			//< ��Ʈ�ʸ�
	// ������ ����
	DWORD				m_dwItemSerial;
	WORD				m_wItemCode;
	BYTE				m_byItemNum;
	DWORD				m_dwMoney;
	BYTE				m_pItemStreamEX1[MAX_ITEMSTREAMEX_SIZE];
	BYTE				m_pItemStreamEX2[MAX_ITEMSTREAMEX_SIZE];
};

struct _MoneyData
{
	WORD 				m_wMapCode;
	SHORT				m_sMapX;
	SHORT 				m_sMapY;
	SHORT 				m_sMapZ;
	TCHAR 				m_szToAccountID[MAX_ACCOUT_ID_SIZE+1];			//< ��Ʈ��id
	TCHAR 				m_szToCharName[MAX_CHAR_NAME_SIZE+1];			//< ��Ʈ�ʸ�

	DWORD64				m_dw64Money;
	DWORD64				m_dw64ToMoney;
	WORD				m_wRoomKey;
};

struct _ExpData
{
	int   m_nNewLevel;
	int   m_nOldLevel;
	int   m_nNewExp;
	int   m_nOldExp;
	int   m_nDivideCount;
};

struct _ActionData
{
	WORD 				m_wMapCode;
	SHORT 				m_sMapX;
	SHORT 				m_sMapY;
	SHORT 				m_sMapZ;
};


struct _ChatData
{
	int		m_nChannel;
	int		m_nUserGuid;
	int		m_nListenUserGuid;
	TCHAR	m_szListenCharName[MAX_CHAR_NAME_SIZE+1];
	TCHAR	m_szContext[MAX_CHAT_CONTEXT_SIZE+1];
	WORD	m_wMapCode;
	int		m_nRoomNo;	
};



struct _SkillData
{
	WORD	m_wSkillLevel;						//��ų����
	int		m_nSkillUseCnt;						//��ų ���ȸ��
};


struct _SnapShotData
{
	BYTE 	m_byCharStat[MAX_CHAR_STAT_SIZE];
	BYTE 	m_byInventory[MAX_INVENTORYITEM_SIZE_NOW];
	BYTE 	m_byTempInventory[MAX_TEMPINVENTORYITEM_SIZE];
	BYTE 	m_byEquipItem[MAX_EQUIPITEM_SIZE];
	BYTE 	m_bySkill[MAX_SKILL_SIZE];
//	BYTE 	m_byQuest[MAX_QUEST_SIZE];
	BYTE 	m_byMission[MAX_MISSION_SIZE];
//#ifdef _DEV_TAIYO_ADD_SNAPSHOT_AC
	BYTE	m_byProgrQuest[MAX_PROGR_QUEST_SIZE];
	BYTE	m_byComplQuest[MAX_COMPL_QUEST_SIZE];
	//20060717 ���� 11:50:02
	BYTE	m_byAC[MAX_AC_SIZE];
//#endif
};

struct _WareHouseData
{
	BYTE 		m_byWareHouse[MAX_WAREHOUSE_SIZE];
	//2006�� 6�� 12�� ������ ���� 3:38:29 : DWORD->ULONGLONG���� Ÿ�Ժ���
	ULONGLONG	m_Money;
};

struct _InventoryData
{
	BYTE 		m_byInventory[MAX_INVENTORYITEM_SIZE];
	BYTE 		m_byTempInventory[MAX_TEMPINVENTORYITEM_SIZE];
	//2006�� 6�� 12�� ������ ���� 3:38:49 : �� Ÿ�� �߰�
	ULONGLONG	m_InventoryMoney;
};

struct _AdminData
{
	WORD 	m_wMapCode;
	int 	m_nMapX;
	int 	m_nMapY;
	int 	m_nMapZ;
	WORD	m_wRoomNo;												//���� �� �ѹ�.
	TCHAR 	m_szToCharName[MAX_CHAR_NAME_SIZE+1];					//���� ĳ���� ��.
};
*/