#pragma once

#include <Singleton.h>
#include "Protocol_CG.h"
#include "PacketStruct_CG.h"
#include <MissionRewardParser.h>
#include "RewardManager.h"

//

#define REWARD_MSG_CREATOR_SPECIFIC_TYPE_ALLOCATOR(name)	\
	name*	GetHeaderPointer_##name() {						\
		DWORD idx = HEADINFO_LASTINDEX-(sizeof(name)-sizeof(INVENTORY_TOTAL_INFO));	\
		return new (&m_HeadInfo[idx]) name;												\
	}

struct REWARD_MSG_CREATOR
{
	enum { MAX_HEADINFO_SIZE=64, HEADINFO_LASTINDEX=MAX_HEADINFO_SIZE-1 };
	BYTE					m_HeadInfo[MAX_HEADINFO_SIZE];
	INVENTORY_TOTAL_INFO	m_Inventory;

	REWARD_MSG_CREATOR_SPECIFIC_TYPE_ALLOCATOR(MSG_CG_TRIGGER_REWARD_ACK);

#define GetRewardMSG_HeaderPtr(name)				GetHeaderPointer_##name
};


struct MissionRewardArgs
{
	// �Ʒ� �ʵ���� ���������� ���Ǵ� ����������
	__PROPERTY( Player*, Player );
	__PROPERTY( MissionManager*, MissionManager );
	// ������ GetMissionRewardInfoNClassCode() ���� ���������� �����ȴ�.
	// �� ���� ����� �ٸ� RewardXXX�� ������ ����� �� �ִ�.
	__PROPERTY( MissionRewardInfo::sCLASS*, MissionRewardInfoClassCode );
	__PROPERTY( WORD, MissionPoint );
	// �Ʒ� �ʵ���� Ŭ��κ��� �Էµ� ������
	__PROPERTY( BYTE, Num );								//< ����ũ��Ʈ�� ��ȣ
	__PROPERTY( BYTE, Type );								//< 0 or 1
	__PROPERTY( BYTE, SelectedItem );						//< ������ �����۵�
};


class MissionRewardManager : public util::Singleton<MissionRewardManager>
{
public:
	MissionRewardManager() {}
	~MissionRewardManager() {}

	VOID		Init()
	{
		memset(&m_MissionRewardArgs, 0, sizeof(MissionRewardArgs));
		//memset(&m_sExtraInfo, 0, sizeof(m_sExtraInfo));
		//memset(rewardItemCode, 0, sizeof(rewardItemCode) );
		//memset(rewardItemNum, 0, sizeof(rewardItemNum) );
		//rewardItemDivine.ClearAll();
	}

	inline MissionRewardArgs&	GetMissionRewardArgs() { return m_MissionRewardArgs; }
	inline VOID					SetMissionRewardArgs( MissionRewardArgs& IN rMRI ) { m_MissionRewardArgs = rMRI; }

	// <REMARK>
	//   m_MissionRewardInfo ������ �ǰ� ���ȴ�.
	// <Description>
	//   MissionManager::MissionPoint ���� �Է����� ��,
	//   ��ũ��Ʈ�� ����� n ���� ���� �� � ���� ������ ���� �� �ε����� ���� �� �ִ�.
	//
	// <����> 060414 ����
	//   max(n)=3, n={0,1,2}
	// <��ȯ��>
	//   TRUE:  ��ũ��Ʈ�� �ε���
	//   FALSE: MissionRewardInfo::MISSION_REWARD_MAX_SIZE
	BYTE				GetTypeOfMissionPoint( );

	// <REMARK>
	//   m_MissionRewardInfo ������ �ǰ� ���ȴ�.
	// <Arguments>
	//   bPreUpdate : BOOL (�̸� ����ġ�� ������ ��꿩��)
	//   �Ϸ�Ǵ� ������ ó���� MissionPoint�� ������Ʈ ���� �ߴ��� ���ߴ����� ���̿� ���� ó���� �޶��� �� �ִ�.
	//   ��, ó�� �̼��� �Ϸ����� ���, ������Ʈ ������ �Ϸ� ȸ��(MissionPoint)�� 0�̸�, 1�� ������ ���� �ڵ带 ã�ƾ� �� ���̴�.
	// <��ȯ��>
	//   TRUE:  ��ũ��Ʈ�� �����ڵ�
	//   FALSE: RC::RC_REWARD_FAILED;
	WORD				GetRewardCode( BOOL bPreUpdate );

	// <REMARK>
	//   m_MissionRewardInfo ������ �ǰ� ���ȴ�.
	// <��ȯ��>
	//   ���������� MissionRewardArgs�� �ʵ尪��
	//     MissionRewardInfo::sCLASS* Get(Set)MissionRewardInfoClassCode()
	//   �� ���� �Ҵ����ش�.
	MissionRewardInfo*	GetMissionRewardInfoNClassCode();

	// <REMARK>
	//   m_MissionRewardInfo ������ �ǰ� ���ȴ�.
	RC::eREWARD_RESULT	RewardExp( EXPTYPE* OUT pExp );
	// <REMARK>
	//   m_MissionRewardInfo ������ �ǰ� ���ȴ�.
	RC::eREWARD_RESULT	RewardExp( const MissionRewardInfo::sCLASS* IN pMRIclass, EXPTYPE* OUT pExp );

	// <REMARK>
	//   m_MissionRewardInfo ������ �ǰ� ���ȴ�.
	RC::eREWARD_RESULT	RewardHime( MONEY* OUT pMoney );
	// <REMARK>
	//   m_MissionRewardInfo ������ �ǰ� ���ȴ�.
	RC::eREWARD_RESULT	RewardHime( const MissionRewardInfo::sCLASS* IN pMRIclass, MONEY* OUT pMoney );

	// <REMARK>
	//   m_MissionRewardInfo ������ �ǰ� ���ȴ�.
	RC::eREWARD_RESULT	RewardItem( INVENTORY_TOTAL_INFO& OUT rInvenInfo );
	// <REMARK>
	//   m_MissionRewardInfo ������ �ǰ� ���ȴ�.
	RC::eREWARD_RESULT	RewardItem( const MissionRewardInfo::sCLASS* IN pMRIclass, INVENTORY_TOTAL_INFO& OUT rInvenInfo );

	//// m_MissionRewardInfo ������ �ǰ� ���ȴ�. 1�� ������ ���
	//RC::eREWARD_RESULT	RewardDivineItem( INVENTORY_TOTAL_INFO& OUT rInvenInfo );
	//// m_MissionRewardInfo ������ �ǰ� ���ȴ�. 2�� ������ ���
	//RC::eREWARD_RESULT	RewardRandomItem( INVENTORY_TOTAL_INFO& OUT rInvenInfo );

private:
	//RC::eREWARD_RESULT		Reward1stItem( INVENTORY_TOTAL_INFO& OUT rInvenInfo );
	//RC::eREWARD_RESULT		Reward2ndItem( INVENTORY_TOTAL_INFO& OUT rInvenInfo );

	//RC::eREWARD_RESULT		Make1stRewardInfo( INVENTORY_TOTAL_INFO& OUT rInvenInfo );
	//RC::eREWARD_RESULT		Make2ndRewardInfo( INVENTORY_TOTAL_INFO& OUT rInvenInfo );

private:
	MissionRewardArgs	m_MissionRewardArgs;
	struct MRI_ExtraInfo
	{
		POSTYPE totalCount;
		POSTYPE divineCount;
	}					m_sExtraInfo;

	//
	static SLOTCODE		rewardItemCode[MAX_INVENTORY_SLOT_NUM];
	static POSTYPE		rewardItemNum[MAX_INVENTORY_SLOT_NUM];
};

