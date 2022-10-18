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
	// 아래 필드들은 내부적으로 사용되는 참조정보들
	__PROPERTY( Player*, Player );
	__PROPERTY( MissionManager*, MissionManager );
	// 다음은 GetMissionRewardInfoNClassCode() 이후 내부적으로 설정된다.
	// 이 값을 사용해 다른 RewardXXX를 빠르게 사용할 수 있다.
	__PROPERTY( MissionRewardInfo::sCLASS*, MissionRewardInfoClassCode );
	__PROPERTY( WORD, MissionPoint );
	// 아래 필드들은 클라로부터 입력된 정보들
	__PROPERTY( BYTE, Num );								//< 보상스크립트의 번호
	__PROPERTY( BYTE, Type );								//< 0 or 1
	__PROPERTY( BYTE, SelectedItem );						//< 선택한 아이템들
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
	//   m_MissionRewardInfo 정보에 의거 계산된다.
	// <Description>
	//   MissionManager::MissionPoint 값을 입력했을 때,
	//   스크립트에 저장된 n 개의 정보 중 어떤 것을 제공해 줄지 그 인덱스를 얻을 수 있다.
	//
	// <범위> 060414 현재
	//   max(n)=3, n={0,1,2}
	// <반환값>
	//   TRUE:  스크립트상 인덱스
	//   FALSE: MissionRewardInfo::MISSION_REWARD_MAX_SIZE
	BYTE				GetTypeOfMissionPoint( );

	// <REMARK>
	//   m_MissionRewardInfo 정보에 의거 계산된다.
	// <Arguments>
	//   bPreUpdate : BOOL (미리 증가치를 감안한 계산여부)
	//   완료되는 시점의 처리상 MissionPoint를 업데이트 먼저 했는지 안했는지의 차이에 따라 처리가 달라질 수 있다.
	//   예, 처음 미션을 완료했을 경우, 업데이트 이전에 완료 회수(MissionPoint)는 0이며, 1을 감안한 보상 코드를 찾아야 할 것이다.
	// <반환값>
	//   TRUE:  스크립트상 보상코드
	//   FALSE: RC::RC_REWARD_FAILED;
	WORD				GetRewardCode( BOOL bPreUpdate );

	// <REMARK>
	//   m_MissionRewardInfo 정보에 의거 계산된다.
	// <반환값>
	//   내부적으로 MissionRewardArgs의 필드값인
	//     MissionRewardInfo::sCLASS* Get(Set)MissionRewardInfoClassCode()
	//   에 값을 할당해준다.
	MissionRewardInfo*	GetMissionRewardInfoNClassCode();

	// <REMARK>
	//   m_MissionRewardInfo 정보에 의거 계산된다.
	RC::eREWARD_RESULT	RewardExp( EXPTYPE* OUT pExp );
	// <REMARK>
	//   m_MissionRewardInfo 정보에 의거 계산된다.
	RC::eREWARD_RESULT	RewardExp( const MissionRewardInfo::sCLASS* IN pMRIclass, EXPTYPE* OUT pExp );

	// <REMARK>
	//   m_MissionRewardInfo 정보에 의거 계산된다.
	RC::eREWARD_RESULT	RewardHime( MONEY* OUT pMoney );
	// <REMARK>
	//   m_MissionRewardInfo 정보에 의거 계산된다.
	RC::eREWARD_RESULT	RewardHime( const MissionRewardInfo::sCLASS* IN pMRIclass, MONEY* OUT pMoney );

	// <REMARK>
	//   m_MissionRewardInfo 정보에 의거 계산된다.
	RC::eREWARD_RESULT	RewardItem( INVENTORY_TOTAL_INFO& OUT rInvenInfo );
	// <REMARK>
	//   m_MissionRewardInfo 정보에 의거 계산된다.
	RC::eREWARD_RESULT	RewardItem( const MissionRewardInfo::sCLASS* IN pMRIclass, INVENTORY_TOTAL_INFO& OUT rInvenInfo );

	//// m_MissionRewardInfo 정보에 의거 계산된다. 1차 보상인 경우
	//RC::eREWARD_RESULT	RewardDivineItem( INVENTORY_TOTAL_INFO& OUT rInvenInfo );
	//// m_MissionRewardInfo 정보에 의거 계산된다. 2차 보상인 경우
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

