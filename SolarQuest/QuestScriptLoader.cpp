#include "StdAfx.h"
#include ".\questscriptloader.h"
#include <SolarFileReader.h>
#include "QuestInfoManager.h"
#include "QuestInfo.h"
#include "QuestConditionInfo_Concrete.h"
#include "QuestActionInfo_Concrete.h"
#include <iostream>


#define ADD_ACCEPTCONDITION( CONDITION_CLASS )							\
	CONDITION_CLASS##_INFO* pCondition = new CONDITION_CLASS##_INFO;	\
	pInfo->AddAcceptCondition( pCondition )								
#define ADD_COMPLETECONDITION( CONDITION_CLASS )						\
	CONDITION_CLASS##_INFO* pCondition = new CONDITION_CLASS##_INFO;	\
	pInfo->AddCompleteCondition( pCondition )
#define ADD_ACCEPTACTION( ACTION_CLASS )								\
	ACTION_CLASS##_INFO* pAction = new ACTION_CLASS##_INFO;				\
	pInfo->AddAcceptAction( pAction )
#define ADD_COMPLETEACTION( ACTION_CLASS )								\
	ACTION_CLASS##_INFO* pAction = new ACTION_CLASS##_INFO;				\
	pInfo->AddCompleteAction( pAction )
#define ADD_ABANDONACTION( ACTION_CLASS )								\
	ACTION_CLASS##_INFO* pAction = new ACTION_CLASS##_INFO;				\
	pInfo->AddAbandonAction( pAction )


#define MAX_AREACODE (5)
#define ConvertID(str)		((DWORD)((*(str))|(*((str)+1)<<8)|(*((str)+2)<<16)|(*((str)+3)<<24)))

template<typename T>
int GetValueCount( T& t, int MaxSize )
{
    int i = 0;
	for(; i < MaxSize; ++i )
		if( 0 == t[i] )
			break;
	
	return i;
}

template<typename T1, typename T2>
int GetValueCount( T1& t1, T2& t2, int MaxSize )
{
    int i = 0;
    for(; i < MaxSize; ++i )
        if( 0 == t1[i] && 0 == t2[i] )
            break;

    return i;
}


struct QuestScriptRecode
{
	WORD  QuestCode;
	char  QuestName[MAX_QUEST_NAME_LENGTH];

#ifdef _YJ_RENEWAL_QUEST_UI_001265
	BYTE  Qtype;
#endif

	DWORD NPCCodeRewardLocate;
	WORD  CCKillMonsterLocate[MAX_QUEST_KILL_MONSTER_MAP_CODE];

	DWORD QNCode;
	WORD  QLevel;
	BYTE  QDisplay;
	WORD  ParentQuestCode;
	int   QuestAcceptType;					//< ����Ʈ �׼� Ÿ��
	DWORD ACCode;
	int   ACType;
	DWORD QImgCode;
	DWORD QGroupCode;
	WORD  ContinueQuestCode;
	DWORD NPCCodeStart;
	DWORD NPCCodeReward;

	//.// ����Ʈ �׼� Ÿ�� ó��
	BYTE  ACMaxRepeatNum;				//�����ִ�ݺ�Ƚ��
	WORD  ACReqMinCharLV;				//�����ʿ��ּҷ���
	WORD  ACReqMaxCharLV;				//�����ʿ��ִ뷹��
	BYTE  ACReqCharClassBit;			//�����ʿ�ĳ��������

	BYTE  ACReqChaoState;				//ī�� ����Ʈ�� ���� �߰� �ʵ�
	BYTE  ACReqNpcDialog;
	//.// �ʿ��� �̼� ����
	WORD  ACReqMissionCode[QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM];
	BYTE  ACReqMissionNum[QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM];

	//.// �������� ����Ʈ ����
	WORD  ACReqProgQuestCode[QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM];		// �����ʿ����� ����Ʈ�ڵ�
	BYTE  ACReqProgOperator;

	//.// �Ϸ�� ����Ʈ ����
	WORD  ACReqEndQuestCode[QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM];			
	BYTE  ACReqEndQuestNum[QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM];			
	BYTE  ACReqEndQuestOperator;

	//.// �ʿ� ������
	WORD  ACReqItemCode[QUEST_CONDITION_ITEM_INFO::_DEF_CHECK_NUM];
	BYTE  ACReqItemNum[QUEST_CONDITION_ITEM_INFO::_DEF_CHECK_NUM];

	//.// ��������
	WORD  ACResEndQuestCode[QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM];			// �������� ����Ʈ �ڵ�
	BYTE  ACResEndQuestNum[QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM];			// �������� ����Ʈ �Ϸ� Ƚ�� 
	BYTE  ACResEndQuestOperator;

	WORD  ACResItemCode;				// ����� �ϴ� ������ : ���� ������
	BYTE  ACResItemClass;				// ���� Ŭ���� ������


	BYTE  AAReqItemDelete;				// �����ʿ� ������ ��������
	WORD  AAGiveItemCode[QUEST_ACTION_ITEM_INFO::_DEF_CHECK_NUM];			// �ο� ������
	BYTE  AAGiveItemNum[QUEST_ACTION_ITEM_INFO::_DEF_CHECK_NUM];

	DWORD SummonMonsterCode;
	DWORD MapPortalCode;
	DWORD AAGiveExpiredTime;			// Ÿ�̸�

	//.// CompletionCondition( CC )
	WORD  CCReqCharLV;
	DWORD64 CCReqHiem;
	WORD  CCReqMissionCode[QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM];	// �ʿ��� �̼� ����
	BYTE  CCReqMissionNum[QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM];

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    WORD CCReqEndMissionCode[QUEST_CONDITION_MISSION_CLEAR_INFO::_MAX_CHECK_NUM];
    BYTE CCReqEndMissionNum[QUEST_CONDITION_MISSION_CLEAR_INFO::_MAX_CHECK_NUM];
#endif

#ifdef _DEVTAIYO_QUEST_TRIGGER_WORK
	DWORD CCReqTrigger[QUEST_CONDITION_TRIGGER_INFO::_MAX_CHECK_NUM];		// Ʈ���� ����
#endif

	// �Ϸ�� ����Ʈ ����
	WORD  CCReqEndQuestCode[QUEST_CONDITION_QUEST_INFO::_MAX_CHECK_NUM];	// �䱸�Ϸ�����Ʈ
	BYTE  CCReqEndQuestNum[QUEST_CONDITION_QUEST_INFO::_MAX_CHECK_NUM];		// �䱸�Ϸ�����Ʈ Ƚ��
	WORD  CCReqItemCode[QUEST_CONDITION_ITEM_INFO::_MAX_CHECK_NUM];			// �ʿ� ������
	BYTE  CCReqItemNum[QUEST_CONDITION_ITEM_INFO::_MAX_CHECK_NUM];
	BYTE  CCReqItemDelete;													// �ʿ� ������ ���� ����
	DWORD CCKillMonster[QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM];	//�䱸����Kill
    BYTE  CCKillSetMonsterGrade[QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM];	// �䱸 ���� ���
    WORD  CCKillSetMonsterLevel[QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM];	// �䱸 ���� ����
	BYTE  CCKillMonsterNum[QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM];//�䱸����Kill��
    DWORD CCAreaSearchID[QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM];        // Ž�� ���� ID
    DWORD CAShowTextMsgCode;			// �Ϸ�� �������� �ؽ�Ʈ �ڵ�

#ifdef _DH_QUEST_ACCEPT_MESSAGE
    DWORD AAShowTextMsgCode;			// ������ �������� �ؽ�Ʈ �ڵ�
#endif//_DH_QUEST_ACCEPT_MESSAGE

	//.// ���� �ڵ�
	DWORD RIRewardCode[eCHAR_TYPE_MAX];

	//.// TextIfo
	DWORD TISCodeStart;					
	DWORD TISCodeProgr1;
	DWORD TISCodeProgr2;
	DWORD TISCodeCompl;
	DWORD TISCodeBrief;

    DWORD TISMissionText[QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM];

#ifdef _NA_20100307_BETAKEY_SYSTEM
    INT32 BetaKey;
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    BYTE DailyQuest;
    WORD SetBackTime;
    BYTE SetBackDay;
    WORD SquadCode;

    WORD ACFameCode;        // �������� �� ���
    BYTE ACFameRating;
    WORD ACReputeCode;      // �������� ���� ���
    BYTE ACReputeRating;
#endif //_NA_003027_20111013_HONOR_SYSTEM
};

//==================================================================================================
// f100220, #9499, waverix, this bound checker test a validation at compile time
// NOTE: QuestScriptRecode�� Ÿ���� QuestInfo�� ����� �� �ְų� �ؼ� Ÿ�� ����ġ�� �߻����� �ʵ���
// ������ �ʿ� �ִ�.
class QuestInfoTest : public QuestInfo
{
    void BoundChecker()
    {
        const QuestScriptRecode record = { 0, };
        __TOUCHED(record);
        BOOST_STATIC_ASSERT(sizeof(m_pKillMonsterMapCode) == sizeof(record.CCKillMonsterLocate) &&
                            _countof(m_pKillMonsterMapCode) == _countof(record.CCKillMonsterLocate));
    };
};
//==================================================================================================


QuestScriptLoader::QuestScriptLoader(VOID)
{
}

QuestScriptLoader::~QuestScriptLoader(VOID)
{
}

VOID	QuestScriptLoader::Release()
{
	m_pQuestInfoManager = NULL;
	m_pEventRegister = NULL;
}

BOOL	QuestScriptLoader::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_QUESTINFO:
	case SCRIPT_QUESTINFO_MANAGER:
		return LoadQuestInfo( bReload );
	}

	return FALSE; 
}

BOOL	QuestScriptLoader::LoadQuestInfo( BOOL bReload )
{
    __UNUSED(bReload);
	if( !m_pQuestInfoManager )
		return FALSE;

#if defined(_SERVER)
	if( !m_pEventRegister )
		return FALSE;
#endif

    QuestScriptRecode Recode = {0,};

	int i = 0;
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		GetData(nRow, Recode.QuestCode, "QuestCode");
		GetData(nRow, MAX_QUEST_NAME_LENGTH, Recode.QuestName, "QuestName");

#ifdef _YJ_RENEWAL_QUEST_UI_001265
		GetData(nRow,  Recode.Qtype, "Qtype");
#endif

		GetData(nRow, Recode.NPCCodeRewardLocate, "NPCCodeRewardLocate");

 		for( i = 0 ; i < MAX_QUEST_KILL_MONSTER_MAP_CODE ; ++i )
			GetData(nRow, Recode.CCKillMonsterLocate[i], "CCKillMonsterLocate%d", i+1 );

		GetData(nRow, Recode.QNCode, "QNCode");
		GetData(nRow, Recode.QLevel, "QLevel");
		GetData(nRow, Recode.QDisplay, "QDisplay" );
		GetData(nRow, Recode.ParentQuestCode, "ParentQuestCode");
		GetData(nRow, Recode.QuestAcceptType, "QuestAcceptType");					//< ����Ʈ �׼� Ÿ��
		GetData(nRow, Recode.ACCode			, "ACCode");
		GetData(nRow, Recode.ACType			, "ACType");
		GetData(nRow, Recode.QImgCode		, "QImgCode");
		GetData(nRow, Recode.QGroupCode		, "QGroupCode");
		GetData(nRow, Recode.ContinueQuestCode, "ContinueQuestCode");
		GetData(nRow, Recode.NPCCodeStart	, "NPCCodeStart");
		GetData(nRow, Recode.NPCCodeReward	, "NPCCodeReward");


		//.// ����Ʈ �׼� Ÿ�� ó��
		GetData(nRow, Recode.ACMaxRepeatNum, "ACMaxRepeatNum");				//�����ִ�ݺ�Ƚ��
		GetData(nRow, Recode.ACReqMinCharLV, "ACReqMinCharLV");				//�����ʿ��ּҷ���
		GetData(nRow, Recode.ACReqMaxCharLV, "ACReqMaxCharLV");				//�����ʿ��ִ뷹��
		GetData(nRow, Recode.ACReqCharClassBit, "ACReqCharClassBit");		//�����ʿ�ĳ��������
		GetData(nRow, Recode.ACReqChaoState, "ACReqChaoState");				//ī�� ����Ʈ�� ���� �߰� �ʵ�
		GetData(nRow, Recode.ACReqNpcDialog, "ACReqNpcDialog");

		//.// �ʿ��� �̼� ����

		for( i = 0 ; i < QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM; ++i )
		{
			GetData(nRow, Recode.ACReqMissionCode[i], "ACReqMissionCode%d", i+1 );
			GetData(nRow, Recode.ACReqMissionNum[i] , "ACReqMissionNum%d" , i+1 );
		}


		//.// �������� ����Ʈ ����

		for( i = 0 ; i < QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM; ++i )
			GetData(nRow, Recode.ACReqProgQuestCode[i], "ACReqProgQuestCode%d" , i+1 );

		GetData(nRow, Recode.ACReqProgOperator, "ACReqProgOperator" );


		//.// �Ϸ�� ����Ʈ ����

		for( i = 0 ; i < QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM; ++i )	
			GetData(nRow, Recode.ACReqEndQuestCode[i], "ACReqEndQuestCode%d", i+1 );

		for( i = 0 ; i < QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM; ++i )	
			GetData(nRow, Recode.ACReqEndQuestNum[i], "ACReqEndQuestNum%d" , i+1 );

		GetData(nRow, Recode.ACReqEndQuestOperator, "ACReqEndQuestOperator" );


		//.// �ʿ� ������

		for( i = 0 ; i < QUEST_CONDITION_ITEM_INFO::_DEF_CHECK_NUM; ++i )	
			GetData(nRow, Recode.ACReqItemCode[i], "ACReqItemCode%d", i+1 );

		for( i = 0 ; i < QUEST_CONDITION_ITEM_INFO::_DEF_CHECK_NUM; ++i )	
			GetData(nRow, Recode.ACReqItemNum[i], "ACReqItemNum%d" , i+1 );


		//.// ��������

		for( i = 0 ; i < QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM; ++i )	
			GetData(nRow, Recode.ACResEndQuestCode[i], "ACResEndQuestCode%d", i+1 );

		for( i = 0 ; i < QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM; ++i )	
			GetData(nRow, Recode.ACResEndQuestNum[i], "ACResEndQuestNum%d" , i+1 );
		
		GetData(nRow, Recode.ACResEndQuestOperator, "ACResEndQuestOperator" );

		GetData(nRow, Recode.ACResItemCode, "ACResItemCode" );				// ����� �ϴ� ������ : ���� ������
		GetData(nRow, Recode.ACResItemClass, "ACResItemClass");				// ���� Ŭ���� ������


		//.// �����׼�
		GetData(nRow, Recode.AAReqItemDelete, "AAReqItemDelete" );				// �����ʿ� ������ ��������

		// �ο� ������
		for( i = 0 ; i < QUEST_ACTION_ITEM_INFO::_DEF_CHECK_NUM; ++i )	
			GetData(nRow, Recode.AAGiveItemCode[i], "AAGiveItemCode%d", i+1 );

		for( i = 0 ; i < QUEST_ACTION_ITEM_INFO::_DEF_CHECK_NUM; ++i )			//3
			GetData(nRow, Recode.AAGiveItemNum[i], "AAGiveItemNum%d" , i+1 );
		
		GetData(nRow, Recode.SummonMonsterCode, "SummonMonsterCode" );
        //__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
		GetData(nRow, Recode.MapPortalCode, "MapPortalCode" );
        //
		GetData(nRow, Recode.AAGiveExpiredTime, "AAGiveExpiredTime" );			// Ÿ�̸�


		//.// CompletionCondition( CC )

		GetData(nRow, Recode.CCReqCharLV, "CCReqCharLV" );
		GetData64(nRow, Recode.CCReqHiem, "CCReqHiem" );

		for( i = 0 ; i < QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM; ++i )	//2
		{
			GetData(nRow, Recode.CCReqMissionCode[i], "CCReqMissionCode%d", i+1 );
			GetData(nRow, Recode.CCReqMissionNum[i], "CCReqMissionNum%d" , i+1 );
		}	

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        for( i = 0; i < QUEST_CONDITION_MISSION_CLEAR_INFO::_MAX_CHECK_NUM; ++i)
        {
            GetData(nRow, Recode.CCReqEndMissionCode[i], "cc_ReqEnd_MissionCode%d", i+1);
            GetData(nRow, Recode.CCReqEndMissionNum[i], "cc_ReqEnd_MissionNum%d", i+1);
        }
#endif // _NA_006607_20130402_ADD_QUESTTYPE

#ifdef _DEVTAIYO_QUEST_TRIGGER_WORK
		// Ʈ���� ����
		for( i = 0 ; i < QUEST_CONDITION_TRIGGER_INFO::_MAX_CHECK_NUM; ++i )
			GetData(nRow, Recode.CCReqTrigger[i], "CCReqTrigger%d", i+1 );
#endif


		//.// �Ϸ�� ����Ʈ ����
		
		for( i = 0 ; i < QUEST_CONDITION_QUEST_INFO::_MAX_CHECK_NUM; ++i )
			GetData(nRow, Recode.CCReqEndQuestCode[i], "CCReqEndQuestCode%d", i+1 );

		for( i = 0 ; i < QUEST_CONDITION_QUEST_INFO::_MAX_CHECK_NUM; ++i )
			GetData(nRow, Recode.CCReqEndQuestNum[i], "CCReqEndQuestNum%d" , i+1 );

		
		for( i = 0 ; i < QUEST_CONDITION_ITEM_INFO::_MAX_CHECK_NUM; ++i )
			GetData(nRow, Recode.CCReqItemCode[i], "CCReqItemCode%d" , i+1 );

		for( i = 0 ; i < QUEST_CONDITION_ITEM_INFO::_MAX_CHECK_NUM; ++i )
			GetData(nRow, Recode.CCReqItemNum[i], "CCReqItemNum%d" , i+1 );

		// �ʿ� ������ ���� ����
		GetData(nRow, Recode.CCReqItemDelete, "CCReqItemDelete" );					

		//�䱸����Kill
		for( i = 0 ; i < QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM; ++i )
        {
            GetData(nRow, Recode.CCKillMonster[i], "CCKillMonster%d" , i+1 );
            GetData(nRow, Recode.CCKillSetMonsterGrade[i], "CCKillSetMonsterGrade%d" , i+1 );
            GetData(nRow, Recode.CCKillSetMonsterLevel[i], "CCKillSetMonsterLevel%d" , i+1 );
            GetData(nRow, Recode.CCKillMonsterNum[i], "CCKillMonsterNum%d" , i+1 );
        }

        for( i = 0 ; i < QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM; ++i )
            GetDataWZID(nRow, Recode.CCAreaSearchID[i], "CCAreaSearchID%d", i+1 );

#ifdef _DH_QUEST_ACCEPT_MESSAGE
        GetData(nRow, Recode.AAShowTextMsgCode, "AAShowTextMsgCode" );
#endif//_DH_QUEST_ACCEPT_MESSAGE
		// �Ϸ�� �������� �ؽ�Ʈ �ڵ�
		GetData(nRow, Recode.CAShowTextMsgCode, "CAShowTextMsgCode" );

		//.// ���� �ڵ�
		for (i = eCHAR_NONE+1; i < eCHAR_TYPE_MAX; ++i)
        {
            DWORD reward_code = 0;
            if (GetSafeNumericData(
                __FUNCTION__, __LINE__, nRow, reward_code, "RIRewardCode%d", i) == false)
            {
                continue;
            }
            Recode.RIRewardCode[i] = reward_code;
        }			

		//.// TextIfo
		GetData(nRow, Recode.TISCodeStart , "TISCodeStart"  );					
		GetData(nRow, Recode.TISCodeProgr1, "TISCodeProgr1" );
		GetData(nRow, Recode.TISCodeProgr2, "TISCodeProgr2" );
		GetData(nRow, Recode.TISCodeCompl , "TISCodeCompl"  );
		GetData(nRow, Recode.TISCodeBrief , "TISCodeBrief"  );

        //.// Ž�� ����Ʈ �ӹ� �ڵ�
        for( i = 0 ; i < QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM; ++i )
            GetData(nRow, Recode.TISMissionText[i], "TISMissionText%d", i+1 );

		if( FALSE == _Validating(Recode) )
		{
			SUNLOG( eCRITICAL_LOG, "Script File Data Error, File[%s], ����Ʈ[%s] �ش� ����Ʈ�� ���� ���� �ʽ��ϴ�.",
				GetCurFileName(), Recode.QuestName);			
			continue;
		}

#ifdef _NA_20100307_BETAKEY_SYSTEM
        GetData(nRow, Recode.BetaKey , "BetaKeyCode"  );        
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        GetData(nRow, Recode.DailyQuest,  "DailyQuest");
        GetData(nRow, Recode.SetBackTime, "SetBackTime");
        GetData(nRow, Recode.SetBackDay,  "SetBackDay");
        GetData(nRow, Recode.SquadCode,   "SquadCode");

        GetData(nRow, Recode.ACFameCode,     "ACReqFameCode");
        GetData(nRow, Recode.ACFameRating,   "ACReqFameRating");
        GetData(nRow, Recode.ACReputeCode,   "ACReqReputeCode");
        GetData(nRow, Recode.ACReputeRating, "ACReqReputeRating");
#endif //_NA_003027_20111013_HONOR_SYSTEM
		
		_newQuestInfo(Recode);
	}

	// QuestInfo �籸�� : ���� ����Ʈ ó���� ���� ������
	// (lst1024)(090408) ��ȹ�� ���ǰ� ���� ���� ���.
#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
	Rebuilding( *m_pQuestInfoManager );
#endif

	return TRUE;

}

QuestInfo* QuestScriptLoader::_newQuestInfo( const QuestScriptRecode& Recode )
{
	QuestInfo* pInfo = new QuestInfo;
	m_pQuestInfoManager->AddQuestInfo(pInfo, Recode.QuestCode);

	pInfo->SetQuestCode(Recode.QuestCode);
	strncpy( pInfo->m_pszQuestName, Recode.QuestName, MAX_QUEST_NAME_LENGTH );

#ifdef _YJ_RENEWAL_QUEST_UI_001265
	pInfo->SetQtype( Recode.Qtype );
#endif //_YJ_RENEWAL_QUEST_UI_001265

	pInfo->SetRewardMapCode( Recode.NPCCodeRewardLocate );
    // (CHANGES) (100220) (WAVERIX) reference #9499 - problem point
	memcpy( pInfo->GetKillMonsterMapCodePtr(), Recode.CCKillMonsterLocate, sizeof(Recode.CCKillMonsterLocate));

	pInfo->SetQNCode( Recode.QNCode );
	pInfo->SetQLevel( Recode.QLevel );
	pInfo->SetQDisplay( Recode.QDisplay );
	pInfo->SetParentQuestCode( Recode.ParentQuestCode );
	pInfo->SetQuestAcceptType( (eQUEST_ACCEPT_TYPE)Recode.QuestAcceptType );

	if( eQUEST_ACCEPT_AUTO == pInfo->GetQuestAcceptType() )
		m_pQuestInfoManager->AddAutoQuestInfo( pInfo, Recode.QuestCode);

#ifdef _NA_20100307_BETAKEY_SYSTEM
    pInfo->SetBetaKey(Recode.BetaKey);
#endif
	pInfo->SetACCode( Recode.ACCode );
	pInfo->SetACType( Recode.ACType );
	pInfo->SetQImgCode( Recode.QImgCode );
	pInfo->SetGroupCode( Recode.QGroupCode );
	pInfo->SetContinueQuestCode( Recode.ContinueQuestCode );
	pInfo->SetNPCCode_Start( Recode.NPCCodeStart );
	pInfo->SetNPCCode_Reward( Recode.NPCCodeReward );

	// Ÿ�̸�
	pInfo->SetExpiredTime( Recode.AAGiveExpiredTime );

#ifdef _DH_QUEST_ACCEPT_MESSAGE
    pInfo->SetAccept_Text( Recode.AAShowTextMsgCode );
#endif//_DH_QUEST_ACCEPT_MESSAGE
	// �Ϸ�� �������� �ؽ�Ʈ �ڵ�
	pInfo->SetCompl_Text( Recode.CAShowTextMsgCode );

	// ���� �ڵ�
	for( DWORD idx=eCHAR_BERSERKER ; idx<eCHAR_TYPE_MAX ; ++idx )
		pInfo->SetRewardCode( (eCHAR_TYPE)idx, Recode.RIRewardCode[idx] );

	// TextIfo 
	// text
	pInfo->SetSCode_Start( Recode.TISCodeStart );
	pInfo->SetSCode_Progr1( Recode.TISCodeProgr1 );
	pInfo->SetSCode_Progr2( Recode.TISCodeProgr2 );
	pInfo->SetSCode_Compl( Recode.TISCodeCompl );
	pInfo->SetSCode_Brief( Recode.TISCodeBrief );

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    pInfo->SetACMaxRepeatNum( Recode.ACMaxRepeatNum );
    pInfo->SetDailyQuest( Recode.DailyQuest );
    pInfo->SetSetBackTime( Recode.SetBackTime );
    pInfo->SetSetBackDay( Recode.SetBackDay );
    pInfo->SetSquadCode( Recode.SquadCode );
#endif //_NA_003027_20111013_HONOR_SYSTEM

    //.// Ž�� ����Ʈ �ӹ� �ڵ�
    for( int i = 0 ; i < QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM; ++i )
        pInfo->SetTISMissionText( i, Recode.TISMissionText[i] );

	// 
	AddAcceptCondition(Recode, pInfo);
	AddAcceptAction(Recode, pInfo);

	AddAbandonAction(Recode, pInfo);

	AddCompleteCondition(Recode, pInfo);
	AddCompleteAction(Recode, pInfo);

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    if( pInfo->GetSquadCode() > 0 )
        m_pQuestInfoManager->AddGroupQuestCode(pInfo->GetSquadCode(), pInfo->GetQuestCode());
#endif //_NA_003027_20111013_HONOR_SYSTEM

	return pInfo;
}

BOOL QuestScriptLoader::AddAcceptCondition( const QuestScriptRecode& Recode, QuestInfo* pInfo )
{
	int nValueCount = 0;

	if( 0 != Recode.ACMaxRepeatNum )
	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_REPEAT );				// 0 : ���� �ݺ�
		pCondition->SetMaxRepeatNum( Recode.ACMaxRepeatNum );		// �����ִ�ݺ�Ƚ��
	}

	if( 0 != Recode.ACReqCharClassBit )
	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_CHARCLASS );			// 0 : Ŭ���� äũ ����
		pCondition->SetCharClassBit( Recode.ACReqCharClassBit );	//�����ʿ�ĳ��������
	}


	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_CHARLV );
		pCondition->SetMinCharLV( Recode.ACReqMinCharLV );			//�����ʿ��ּҷ���
		pCondition->SetMaxCharLV( Recode.ACReqMaxCharLV );			//�����ʿ��ִ뷹��
	}
	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_CHAO );
		pCondition->SetChaoState( Recode.ACReqChaoState );
	}


	// �ʿ��� �̼� ����
	nValueCount = GetValueCount(Recode.ACReqMissionCode,QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM);
	if( nValueCount )
	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_MISSION );
		for( int i = 0; i < QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM; ++i )
		{
			pCondition->SetMissionCode(i,Recode.ACReqMissionCode[i]);
			pCondition->SetMissionNum(i,Recode.ACReqMissionNum[i]);
		}
	}

	// �������� ����Ʈ ����
	nValueCount = GetValueCount(Recode.ACReqProgQuestCode,QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM);
	if( nValueCount )
	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_QUEST );
		pCondition->SetProgrQuest( TRUE );				//< �������� ����Ʈ
		pCondition->SetNecessaryQuest( TRUE );			//< �ʿ�����
		pCondition->SetQuestArrayCount( (BYTE)nValueCount );
		pCondition->SetOperator( Recode.ACReqProgOperator );
		for( int i = 0; i < QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM; ++i )
			pCondition->SetQuestCode(i,Recode.ACReqProgQuestCode[i]);
	}

	// �Ϸ�� ����Ʈ ����
	nValueCount = GetValueCount(Recode.ACReqEndQuestCode,QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM);
	if( nValueCount )
	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_QUEST );
		pCondition->SetProgrQuest( FALSE );				//< �Ϸ�� ����Ʈ
		pCondition->SetNecessaryQuest( TRUE );			//< �ʿ�����
		pCondition->SetQuestArrayCount( (BYTE)nValueCount );
		pCondition->SetOperator( Recode.ACReqEndQuestOperator );
		
		for( int i = 0; i < QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM; ++i )
		{
			pCondition->SetQuestCode(i,Recode.ACReqEndQuestCode[i]);
			pCondition->SetQuestNum(i,Recode.ACReqEndQuestNum[i]);
		}
	}

	// �ʿ� ������
	nValueCount = GetValueCount(Recode.ACReqItemCode,QUEST_CONDITION_ITEM_INFO::_DEF_CHECK_NUM);
	if( nValueCount )
	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_ITEM );
		pCondition->SetNecessaryItem( TRUE );			//< �ʿ�����
		pCondition->SetItemArrayCount( (BYTE)nValueCount );

		for( int i = 0; i < QUEST_CONDITION_ITEM_INFO::_DEF_CHECK_NUM; ++i )
		{
			pCondition->SetItemCode(i,Recode.ACReqItemCode[i]);
			pCondition->SetItemNum(i,Recode.ACReqItemNum[i]);
		}
	}

	// �Ϸ�� ����Ʈ ����
	nValueCount = GetValueCount(Recode.ACResEndQuestCode,QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM);
	if( nValueCount )
	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_QUEST );
		pCondition->SetProgrQuest( FALSE );				//< �Ϸ�� ����Ʈ
		pCondition->SetNecessaryQuest( FALSE );			//< ������ϴ�����
		pCondition->SetQuestArrayCount( (BYTE)nValueCount );
		pCondition->SetOperator( Recode.ACResEndQuestOperator );

		for( int i = 0; i < QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM; ++i )
		{
			pCondition->SetQuestCode( i, Recode.ACResEndQuestCode[i] );
			pCondition->SetQuestNum( i, Recode.ACResEndQuestNum[i] );
		}
	}

	// ����� �ϴ� ������ : ���� ������
	if( 0 != Recode.ACResItemCode )
	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_ITEM );
		pCondition->SetNecessaryItem( FALSE );			//< ����� �ϴ� ������
		pCondition->SetItemCode( 0, Recode.ACResItemCode );
		pCondition->SetItemArrayCount(1);
		pCondition->SetItemNum(0, 1);
	}

	// ���� Ŭ���� ������
	if( 0 != Recode.ACResItemClass )
	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_CLASSITEM );
		pCondition->SetCharClassLimit( Recode.ACResItemClass );
	}


	//<���� �ൿ�� ������ �ο��� ������ ���� ���ǿ��� ������ ���� üũ		
	nValueCount = GetValueCount(Recode.AAGiveItemCode,QUEST_ACTION_ITEM_INFO::_DEF_CHECK_NUM);
	if( nValueCount )
	{
		ADD_ACCEPTCONDITION( QUEST_CONDITION_ITEM_SPACE );
		pCondition->SetItemArrayCount( (BYTE)nValueCount );

		for( int i = 0; i < QUEST_ACTION_ITEM_INFO::_DEF_CHECK_NUM; ++i )
		{
			pCondition->SetItemCode(i, Recode.AAGiveItemCode[i] );
			pCondition->SetItemNum( i, Recode.AAGiveItemNum[i] );
		}
	}

	if( QUEST_NPC_DIALOG_CREATE_PORTAL == Recode.ACReqNpcDialog &&
		0 != Recode.MapPortalCode )
	{
		if(SAFE_NUMERIC_TYPECAST(DWORD, Recode.MapPortalCode, MAPCODE))
		{
			ADD_ACCEPTCONDITION(QUEST_CONDITION_CREATE_PORTAL);
			pCondition->SetMapPortalCode((MAPCODE)Recode.MapPortalCode);
		}
	}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    if (Recode.ACFameCode != 0 && Recode.ACFameRating != 0)
    {
        ADD_ACCEPTCONDITION(QUEST_CONDITION_HONOR_RATING);
        pCondition->SetHonorType(1);
        pCondition->SetHonorCode(Recode.ACFameCode);
        pCondition->SetHonorRating(Recode.ACFameRating);
    }

    if (Recode.ACReputeCode != 0 && Recode.ACReputeRating != 0)
    {
        ADD_ACCEPTCONDITION(QUEST_CONDITION_HONOR_RATING);
        pCondition->SetHonorType(2);
        pCondition->SetHonorCode(Recode.ACReputeCode);
        pCondition->SetHonorRating(Recode.ACReputeRating);
    }

    if( Recode.DailyQuest != 0 )
    {
        ADD_ACCEPTCONDITION(QUEST_CONDITION_DAILY_QUEST);
    }
#endif //_NA_003027_20111013_HONOR_SYSTEM

	return TRUE;
}

BOOL QuestScriptLoader::AddAcceptAction( const QuestScriptRecode& Recode, QuestInfo* pInfo )
{
	int nValueCount = 0;

	// �ʿ� ������ ���� ���� ó��
	if( Recode.AAReqItemDelete )
	{	
		nValueCount = GetValueCount(Recode.ACReqItemCode,QUEST_CONDITION_ITEM_INFO::_DEF_CHECK_NUM);
		if( nValueCount )
		{
			ADD_ACCEPTACTION( QUEST_ACTION_ITEM );
			pAction->SetDelete(TRUE);
			pAction->SetItemArrayCount((BYTE)nValueCount);

			for( int i = 0; i < QUEST_CONDITION_ITEM_INFO::_DEF_CHECK_NUM; ++i )
			{
				pAction->SetItemCode(i,Recode.ACReqItemCode[i]);
				pAction->SetItemNum(i,Recode.ACReqItemNum[i]);
			}
		}
	}

	// �ο� ������
	nValueCount = GetValueCount(Recode.AAGiveItemCode,QUEST_ACTION_ITEM_INFO::_DEF_CHECK_NUM);
	if( nValueCount )
	{
		ADD_ACCEPTACTION( QUEST_ACTION_ITEM );
		pAction->SetDelete(FALSE);
		pAction->SetItemArrayCount( (BYTE)nValueCount );

		for( int i = 0; i < QUEST_ACTION_ITEM_INFO::_DEF_CHECK_NUM; ++i )
		{
			pAction->SetItemCode( i, Recode.AAGiveItemCode[i] );
			pAction->SetItemNum( i, Recode.AAGiveItemNum[i] );
		}
	}

	if( QUEST_NPC_DIALOG_SPAWN_MOB == Recode.ACReqNpcDialog)
	{
		if( 0 != Recode.SummonMonsterCode)
		{
			ADD_ACCEPTACTION( QUEST_ACTION_SPAWN_MONSTER );
			pAction->SetSummonMonsterCode( Recode.SummonMonsterCode );
		}
	}
	else if( QUEST_NPC_DIALOG_CREATE_PORTAL == Recode.ACReqNpcDialog )
	{
		if( 0 != Recode.MapPortalCode)
		{
			if(SAFE_NUMERIC_TYPECAST(DWORD, Recode.MapPortalCode, MAPCODE))
			{
				ADD_ACCEPTACTION(QUEST_ACTION_CREATE_PORTAL);
				pAction->SetMapPortalCode((MAPCODE)Recode.MapPortalCode);
			}
		}
	}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    if( Recode.DailyQuest > 0 )
    {
        ADD_ACCEPTACTION( QUEST_ACTION_DAILY_QUEST );
    }
#endif //_NA_003027_20111013_HONOR_SYSTEM

	return TRUE;
}

BOOL QuestScriptLoader::AddAbandonAction( const QuestScriptRecode& Recode, QuestInfo* pInfo )
{
	
	if( QUEST_NPC_DIALOG_SPAWN_MOB == Recode.ACReqNpcDialog )
	{
		if( 0 != Recode.SummonMonsterCode )
		{
			ADD_ABANDONACTION(QUEST_ACTION_SPAWN_MONSTER_ABANDON);
		}
	}
	else if( QUEST_NPC_DIALOG_CREATE_PORTAL == Recode.ACReqNpcDialog )
	{
		if( 0 != Recode.MapPortalCode )
		{
			if(SAFE_NUMERIC_TYPECAST(DWORD, Recode.MapPortalCode, MAPCODE))
			{
				ADD_ABANDONACTION(QUEST_ACTION_CLOSE_PORTAL);
			}
		}
	}

	// ����� �ο��� ������ ����
	int nValueCount = GetValueCount(Recode.AAGiveItemCode,QUEST_ACTION_ITEM_INFO::_DEF_CHECK_NUM);
	if( nValueCount )
	{
		ADD_ABANDONACTION( QUEST_ACTION_ITEM );
		pAction->SetDelete(TRUE);
		pAction->SetItemArrayCount( (BYTE)nValueCount );

		for( int i = 0; i < QUEST_ACTION_ITEM_INFO::_DEF_CHECK_NUM; ++i )
		{
			pAction->SetItemCode( i, Recode.AAGiveItemCode[i] );
			pAction->SetItemNum( i, Recode.AAGiveItemNum[i] );
		}
	}

	// ä���� �ʱ�ȭ
	{ ADD_ABANDONACTION( QUEST_ACTION_DELETE_COLLECTIONID ); }

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    if( Recode.DailyQuest > 0 )
    {
        ADD_ABANDONACTION( QUEST_ACTION_DAILY_QUEST_DELETE );
    }
#endif //_NA_003027_20111013_HONOR_SYSTEM

	return TRUE;
}

BOOL QuestScriptLoader::AddCompleteCondition( const QuestScriptRecode& Recode, QuestInfo* pInfo )
{
	{
		ADD_COMPLETECONDITION( QUEST_CONDITION_CHARLV );
		pCondition->SetMinCharLV( Recode.CCReqCharLV );
		pCondition->SetMaxCharLV( 0xFFFF );
	}

	if( 0 != Recode.CCReqHiem )
	{
		ADD_COMPLETECONDITION( QUEST_CONDITION_MONEY );
		pCondition->SetNeedMoney( Recode.CCReqHiem );
	}

	// �ʿ��� �̼� ����

	int nValueCount = GetValueCount(Recode.CCReqMissionCode,QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM);	//2
	if( nValueCount )
	{
		ADD_COMPLETECONDITION( QUEST_CONDITION_MISSION );

        // nValueCount ���� Ƚ���� ���� �ʴ� ����: ��ũ�η� ������ �ɹ��� ���� �ʱ�ȭ ���� �ʴ´�.
		for( int i = 0; i < QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM; ++i )
		{
			pCondition->SetMissionCode( i, Recode.CCReqMissionCode[i] );
			pCondition->SetMissionNum( i, Recode.CCReqMissionNum[i] );
		}
	}

#ifdef _DEVTAIYO_QUEST_TRIGGER_WORK
	// Ʈ���� ����	//4
	nValueCount = GetValueCount(Recode.CCReqTrigger,QUEST_CONDITION_TRIGGER_INFO::_MAX_CHECK_NUM);
	if( nValueCount )
	{
		ADD_COMPLETECONDITION( QUEST_CONDITION_TRIGGER );
		for( int i = 0; i < QUEST_CONDITION_TRIGGER_INFO::_MAX_CHECK_NUM; ++i )
			pCondition->SetTriggerNCode( i, Recode.CCReqTrigger[i] );
	}
#endif

	// �Ϸ�� ����Ʈ ����
	nValueCount = GetValueCount(Recode.CCReqEndQuestCode,QUEST_CONDITION_QUEST_INFO::_MAX_CHECK_NUM);
	if( nValueCount )
	{
		ADD_COMPLETECONDITION( QUEST_CONDITION_QUEST );
		pCondition->SetProgrQuest( FALSE );				//< �Ϸ�� ����Ʈ
		pCondition->SetNecessaryQuest( TRUE );			//< �ʿ�����
		pCondition->SetQuestArrayCount( (BYTE)nValueCount );
		pCondition->SetOperator( 0 );		//< AND ����
		for( int i = 0; i < QUEST_CONDITION_QUEST_INFO::_MAX_CHECK_NUM; ++i )
		{
			pCondition->SetQuestCode( i, Recode.CCReqEndQuestCode[i] );
			pCondition->SetQuestNum( i, Recode.CCReqEndQuestNum[i] );
		}
	}


	// �ʿ� ������
	nValueCount = GetValueCount(Recode.CCReqItemCode,QUEST_CONDITION_ITEM_INFO::_MAX_CHECK_NUM);
	if( nValueCount )
	{
		ADD_COMPLETECONDITION( QUEST_CONDITION_ITEM );
		pCondition->SetNecessaryItem( TRUE );			//< �ʿ�����
		pCondition->SetItemArrayCount( (BYTE)nValueCount );
		for( int i = 0; i < QUEST_CONDITION_ITEM_INFO::_MAX_CHECK_NUM; ++i )
		{
			pCondition->SetItemCode( i, Recode.CCReqItemCode[i] );
			pCondition->SetItemNum( i, Recode.CCReqItemNum[i] );
		}
	}

	// ���� ų ��

    nValueCount = GetValueCount(Recode.CCKillMonster,Recode.CCKillSetMonsterGrade,QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM);
    if( nValueCount )
    {
        ADD_COMPLETECONDITION( QUEST_CONDITION_KILLMONSTER );

        for( int i = 0; i < QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM; ++i )
        {
            const DWORD CCKillMonster = Recode.CCKillMonster[i];

        #if defined(_SERVER)
            if( m_pEventRegister && 0 != CCKillMonster )
                m_pEventRegister->RegisterNpcKillInfo(Recode.QuestCode, (MONSTERCODE)CCKillMonster);
        #endif

            pCondition->SetMonsterCode( i, CCKillMonster );
            pCondition->SetMonsterGrade( i, Recode.CCKillSetMonsterGrade[i] );
            pCondition->SetMonsterLevel( i, Recode.CCKillSetMonsterLevel[i] );
            pCondition->SetMonsterNum( i, Recode.CCKillMonsterNum[i] );
        }
    }

	// ���� �Ϸ� ����
    nValueCount = GetValueCount(Recode.CCAreaSearchID,QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM);
	if( nValueCount )
	{
		ADD_COMPLETECONDITION( QUEST_CONDITION_AREA );

        for( int i = 0; i < QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM; ++i )
		    pCondition->SetAreaID( i, Recode.CCAreaSearchID[i] );
	}

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    // �̼� �Ϸ�
    nValueCount = GetValueCount(Recode.CCReqEndMissionCode, QUEST_CONDITION_MISSION_CLEAR_INFO::_MAX_CHECK_NUM);
    if(nValueCount)
    {
        ADD_COMPLETECONDITION(QUEST_CONDITION_MISSION_CLEAR);
        for(int i=0; i < QUEST_CONDITION_MISSION_CLEAR_INFO::_MAX_CHECK_NUM; ++i)
        {
            pCondition->SetMissionCode(i, Recode.CCReqEndMissionCode[i]);
            pCondition->SetMissionNum(i, Recode.CCReqEndMissionNum[i]);
        }
            
    }
#endif

	return TRUE;
}

BOOL QuestScriptLoader::AddCompleteAction( const QuestScriptRecode& Recode, QuestInfo* pInfo )
{
	// �Ϸ� ���� �ʿ� ������ ���� ���� ó��
	if( !Recode.CCReqItemDelete )
	{
		int nValueCount = GetValueCount(Recode.CCReqItemCode,QUEST_CONDITION_ITEM_INFO::_MAX_CHECK_NUM);
		if( nValueCount )
		{
			ADD_COMPLETEACTION( QUEST_ACTION_ITEM );
			pAction->SetDelete(TRUE);
			pAction->SetItemArrayCount((BYTE)nValueCount);
			for( int i = 0; i < QUEST_CONDITION_ITEM_INFO::_MAX_CHECK_NUM; ++i )
			{
				pAction->SetItemCode(i,Recode.CCReqItemCode[i]);
				pAction->SetItemNum(i,Recode.CCReqItemNum[i]);
			}
		}
	}

	//	(WAVERIX) (081125) (BUG-FIX) ü�������� �Ϸ� �޽��� 5�� ���ư��� ���� ����
	//	- â������ �� ���� Ȯ���� ����������, �������� ������� �ʾҴ� ���̹Ƿ�,
	//	�ش� �۾� ��ũ�� �����ϴ� �������� ����
	//	- ���� �ٸ� changeup, �Ǵ� ���� �迭 Ȯ���� �����ؼ� BOOL bAdd~ �迭, changeUpCode�� ����
	eSPECIAL_QUEST_REWARD changeUpCode = (eSPECIAL_QUEST_REWARD)0;
	for( DWORD idx=eCHAR_BERSERKER ; idx<eCHAR_TYPE_MAX ; ++idx )
	{
		switch( (eSPECIAL_QUEST_REWARD)Recode.RIRewardCode[idx] )
		{
		case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_BERSERKER:
		case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_DRAGON_KNIGHT:
		case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_SHADOW:
		case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_VALKYRIE:
		case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_ELEMENTALIST:
			{
				changeUpCode = SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL;
			}
			break;
			// ��Ÿ ChangeUp, ChangeOfClass �߰��� ��� ���� ó���� ������ ��.
			//case SPECIAL_REWARD_TYPE_CHANGEUP_200LEVEL_ELEMENTALIST:
			//	blah~
		};
	}

	if( changeUpCode )
	{
		ADD_COMPLETEACTION( QUEST_ACTION_CHANGEofCLASS );
		pAction->ChangeOfClassStep.Set( changeUpCode );
	}


	// ä���� �ʱ�ȭ
	{ ADD_COMPLETEACTION( QUEST_ACTION_DELETE_COLLECTIONID ); }

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    if( Recode.DailyQuest > 0 )
    {
        ADD_COMPLETEACTION( QUEST_ACTION_DAILY_QUEST );
    }
#endif //_NA_003027_20111013_HONOR_SYSTEM

	return TRUE;
}


BOOL QuestScriptLoader::_Validating( const QuestScriptRecode& Recode )
{
	BOOL RetValue = TRUE;

	if( NULL != m_pQuestInfoManager->FindQuestInfo(Recode.QuestCode) )
	{
		RetValue = FALSE;
		SUNLOG( eCRITICAL_LOG, "�̹� �����ϴ� ����Ʈ �ڵ�[%d] [%s]", Recode.QuestCode, Recode.QuestName);
	}

	for( int i = 0 ; i < QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM; ++i )
	{
		BOOL bDiscontinuous = FALSE;
		if( 0 != Recode.ACReqEndQuestCode[i] )
		{
			// �ڵ�� �ִµ� ������ ����.
			if( 0 == Recode.ACReqEndQuestNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] ACReqEndQuestCode%d[%d] ACReqEndQuestNum%d[%d] ",
					Recode.QuestName, i, Recode.ACReqEndQuestCode[i], i, Recode.ACReqEndQuestNum[i]);
			}

			// �߰��� 0�� ���� ����.
			if( FALSE != bDiscontinuous )
			{
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] ACReqEndQuestCode%d[%d] ACReqEndQuestCode%d[%d] ",
					Recode.QuestName, i-1, Recode.ACReqEndQuestCode[i-1], i, Recode.ACReqEndQuestCode[i]);
			}
		}
		else
		{
			bDiscontinuous = TRUE;

			// �ڵ尡 ���µ� ������ ������.
			if( 0 != Recode.ACReqEndQuestNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] ACReqEndQuestCode%d[%d] ACReqEndQuestNum%d[%d] ",
					Recode.QuestName, i, Recode.ACReqEndQuestCode[i], i, Recode.ACReqEndQuestNum[i]);
			}
		}
	}

	for( int i = 0 ; i <  QUEST_CONDITION_ITEM_INFO::_DEF_CHECK_NUM; ++i )
	{
		BOOL bDiscontinuous = FALSE;
		if( 0 != Recode.ACReqItemCode[i] )
		{
			if( 0 == Recode.ACReqItemNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] ACReqItemCode%d[%d] ACReqItemNum%d[%d] ",
					Recode.QuestName, i, Recode.ACReqItemCode[i], i, Recode.ACReqItemNum[i]);
			}

			if( FALSE != bDiscontinuous )
			{
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] ACReqItemCode%d[%d] ACReqItemCode%d[%d] ",
					Recode.QuestName, i-1, Recode.ACReqItemCode[i-1], i, Recode.ACReqItemCode[i]);
			}
		}
		else
		{
			bDiscontinuous = TRUE;
			if( 0 != Recode.ACReqItemNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] ACReqItemCode%d[%d] ACReqItemNum%d[%d] ",
					Recode.QuestName, i, Recode.ACReqItemCode[i], i, Recode.ACReqItemNum[i]);
			}
		}
	}

	for( int i = 0 ; i < QUEST_CONDITION_QUEST_INFO::_DEF_CHECK_NUM; ++i )
	{
		BOOL bDiscontinuous = FALSE;
		if( 0 != Recode.ACResEndQuestCode[i] )
		{
			if( 0 == Recode.ACResEndQuestNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] ACResEndQuestCode%d[%d] ACResEndQuestNum%d[%d] ",
					Recode.QuestName, i, Recode.ACResEndQuestCode[i], i, Recode.ACResEndQuestNum[i]);
			}

			if( FALSE != bDiscontinuous )
			{
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] ACResEndQuestCode%d[%d] ACResEndQuestCode%d[%d] ",
					Recode.QuestName, i-1, Recode.ACResEndQuestCode[i-1], i, Recode.ACResEndQuestCode[i]);
			}
		}
		else
		{
			bDiscontinuous = TRUE;
			if( 0 != Recode.ACResEndQuestNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] ACResEndQuestCode%d[%d] ACResEndQuestNum%d[%d] ",
					Recode.QuestName, i, Recode.ACResEndQuestCode[i], i, Recode.ACResEndQuestNum[i]);
			}
		}
	}

	for( int i = 0 ; i < QUEST_ACTION_ITEM_INFO::_DEF_CHECK_NUM; ++i )
	{
		BOOL bDiscontinuous = FALSE;
		if( 0 != Recode.AAGiveItemCode[i] )
		{
			if( 0 == Recode.AAGiveItemNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] AAGiveItemCode%d[%d] AAGiveItemNum%d[%d] ",
					Recode.QuestName, i, Recode.AAGiveItemCode[i], i, Recode.AAGiveItemNum[i]);
			}

			if( 0 != Recode.ParentQuestCode )
			{
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] �ڽ�����Ʈ�� �ο��������� �����ϸ� �ȵȴ�.", Recode.QuestName);
			}

			if( FALSE != bDiscontinuous )
			{
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] AAGiveItemCode%d[%d] AAGiveItemCode%d[%d] ",
					Recode.QuestName, i-1, Recode.AAGiveItemCode[i-1], i, Recode.AAGiveItemCode[i]);
			}
		}
		else
		{
			bDiscontinuous = TRUE;
			if( 0 != Recode.AAGiveItemNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] AAGiveItemCode%d[%d] AAGiveItemNum%d[%d] ",
					Recode.QuestName, i, Recode.AAGiveItemCode[i], i, Recode.AAGiveItemNum[i]);
			}
		}
	}

	for( int i = 0 ; i < QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM ; ++i )
	{
		BOOL bDiscontinuous = FALSE;
		if( 0 != Recode.CCReqMissionCode[i] )
		{
			if( 0 == Recode.CCReqMissionNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqMissionCode%d[%d] CCReqMissionNum%d[%d] ",
					Recode.QuestName, i, Recode.CCReqMissionCode[i], i, Recode.CCReqMissionNum[i]);
			}
			if( FALSE != bDiscontinuous )
			{
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqMissionCode%d[%d] CCReqMissionCode%d[%d] ",
					Recode.QuestName, i-1, Recode.CCReqMissionCode[i-1], i, Recode.CCReqMissionCode[i]);
			}
		}
		else
		{
			bDiscontinuous = TRUE;
			if( 0 != Recode.CCReqMissionNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqMissionCode%d[%d] CCReqMissionNum%d[%d] ",
					Recode.QuestName, i, Recode.CCReqMissionCode[i], i, Recode.CCReqMissionNum[i]);
			}
		}
	}

	for( int i = 0 ; i < QUEST_CONDITION_QUEST_INFO::_MAX_CHECK_NUM; ++i )
	{
		BOOL bDiscontinuous = FALSE;
		if( 0 != Recode.CCReqEndQuestCode[i] )
		{
			if( 0 == Recode.CCReqEndQuestNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqEndQuestCode%d[%d] CCReqEndQuestNum%d[%d] ",
					Recode.QuestName, i, Recode.CCReqEndQuestCode[i], i, Recode.CCReqEndQuestNum[i]);
			}
			if( FALSE != bDiscontinuous )
			{
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqEndQuestCode%d[%d] CCReqEndQuestCode%d[%d] ",
					Recode.QuestName, i-1, Recode.CCReqEndQuestCode[i-1], i, Recode.CCReqEndQuestCode[i]);
			}
		}
		else
		{
			bDiscontinuous = TRUE;
			if( 0 != Recode.CCReqEndQuestNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqEndQuestCode%d[%d] CCReqEndQuestNum%d[%d] ",
					Recode.QuestName, i, Recode.CCReqEndQuestCode[i], i, Recode.CCReqEndQuestNum[i]);
			}
		}
	}

	for( int i = 0 ; i < QUEST_CONDITION_ITEM_INFO::_MAX_CHECK_NUM ; ++i )
	{
		BOOL bDiscontinuous = FALSE;
		if( 0 != Recode.CCReqItemCode[i] )
		{
			if( 0 == Recode.CCReqItemNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqItemCode%d[%d] CCReqItemNum%d[%d] ",
					Recode.QuestName, i, Recode.CCReqItemCode[i], i, Recode.CCReqItemNum[i]);
			}
			if( FALSE != bDiscontinuous )
			{
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqItemCode%d[%d] CCReqItemCode%d[%d] ",
					Recode.QuestName, i-1, Recode.CCReqItemCode[i-1], i, Recode.CCReqItemCode[i]);
			}
		}
		else
		{
			bDiscontinuous = TRUE;
			if( 0 != Recode.CCReqItemNum[i] )
			{	
				RetValue = FALSE;
				SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqItemCode%d[%d] CCReqItemNum%d[%d] ",
					Recode.QuestName, i, Recode.CCReqItemCode[i], i, Recode.CCReqItemNum[i]);
			}
		}
	}

	for( int i = 0 ; i < QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM ; ++i )
	{
		BOOL bDiscontinuous = FALSE;

        if( 0 != Recode.CCKillMonster[i] || 
            0 != Recode.CCKillSetMonsterGrade[i] )
        {
            if( 0 == Recode.CCKillMonsterNum[i] )
            {	
                RetValue = FALSE;
                SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCKillMonsterNum%d[%d] " 
                                       "CCKillMonster%d[%d] CCKillSetMonsterGrade%d[%d]",
                                        Recode.QuestName, i, Recode.CCKillMonsterNum[i], 
                                        i, Recode.CCKillMonster[i], i, Recode.CCKillSetMonsterGrade[i]);
            }
            if( FALSE != bDiscontinuous )
            {
                RetValue = FALSE;
                SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCKillMonster%d[%d] CCKillMonster%d[%d] ",
                    Recode.QuestName, i-1, Recode.CCKillMonster[i-1], i, Recode.CCKillMonster[i]);
                SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCKillSetMonsterGrade%d[%d] CCKillSetMonsterGrade%d[%d] ",
                    Recode.QuestName, i-1, Recode.CCKillSetMonsterGrade[i-1], i, Recode.CCKillSetMonsterGrade[i]);
            }
        }
        else
        {
            bDiscontinuous = TRUE;
            if( 0 != Recode.CCKillMonsterNum[i] )
            {	
                RetValue = FALSE;
                SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCKillMonsterNum%d[%d] "
                                       "CCKillMonster%d[%d] CCKillSetMonsterGrade%d[%d]",
                                        Recode.QuestName, i, Recode.CCKillMonsterNum[i], 
                                        i, Recode.CCKillMonster[i], i, Recode.CCKillSetMonsterGrade[i]);
            }
        }
	}

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    for( int i = 0 ; i < QUEST_CONDITION_MISSION_CLEAR_INFO::_MAX_CHECK_NUM ; ++i )
    {
        BOOL bDiscontinuous = FALSE;
        if( 0 != Recode.CCReqEndMissionCode[i] )
        {
            if( 0 == Recode.CCReqEndMissionNum[i] )
            {	
                RetValue = FALSE;
                SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqEndMissionCode%d[%d] CCReqEndMissionNum%d[%d] ",
                    Recode.QuestName, i, Recode.CCReqEndMissionCode[i], i, Recode.CCReqEndMissionNum[i]);
            }
            if( FALSE != bDiscontinuous )
            {
                RetValue = FALSE;
                SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqEndMissionCode%d[%d] CCReqEndMissionCode%d[%d] ",
                    Recode.QuestName, i-1, Recode.CCReqEndMissionCode[i-1], i, Recode.CCReqEndMissionCode[i]);
            }
        }
        else
        {
            bDiscontinuous = TRUE;
            if( 0 != Recode.CCReqEndMissionNum[i] )
            {	
                RetValue = FALSE;
                SUNLOG( eCRITICAL_LOG, "����Ʈ ����[%s] CCReqEndMissionCode%d[%d] CCReqEndMissionNum%d[%d] ",
                    Recode.QuestName, i, Recode.CCReqEndMissionCode[i], i, Recode.CCReqEndMissionNum[i]);
            }
        }
    }
#endif //_NA_006607_20130402_ADD_QUESTTYPE

	return RetValue;
}

#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
VOID QuestScriptLoader::Rebuilding( QuestInfoManager& rMNGR )
{
	QUEST_INFO_HASH_ITR it, end(rMNGR.m_QuestInfoHash.end());
	for( it = rMNGR.m_QuestInfoHash.begin() ; it != end ; ++it )
	{
		QuestInfo* pInfo = (*it);
		if( 0 != pInfo->GetParentQuestCode() )
		{
			QuestInfo* pParent = rMNGR.FindQuestInfo( pInfo->GetParentQuestCode() );
			pParent->AddChildQuestInfo( pInfo );

			if(pParent->GetParentQuestCode() != 0 )
				SUNLOG( eCRITICAL_LOG, "Script File Data Error[QuestScript] ����Ʈ[%d]�� �θ�[%d]�� ParentQuestCode�� ����.", pInfo->GetParentQuestCode(), pParent->GetParentQuestCode() );
		}
	}
}
#endif

