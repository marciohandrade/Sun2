#include "stdafx.h"
#include "GMCmdManager.h"
//
//
//
#include <FatigueOptionParser.h>
//
#include "GameCHuntingRoom.h"

#include "CHuntingManager.h"
#include "CompetitionInfo.h"
#include "ChaoState.h"

#include "AchievementManager.h" //C_NA_0_20100520_ACSYSTEM_CHANGED
#include "MissionManager.h"
#include "TargetManager.h"
#include "TriggerManagerEx.h"
#include "MissionRewardManager.h"
#include "CollectionManager.h"
#include "QuestManager_Concrete.h"
#include "FatigueManager.h"

#include "AccumulatePoint.h"

// Internal utilities
#include "./GMHandler_InterUtils.hxx"
#include <MapInfoParser.h>

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
#include <PacketStruct_GZ.h>
#endif //_NA_004034_20120102_GUILD_INTRODUCTION
#include "VendorManager.h"
#include "ExtraNPCManager.h"

namespace nsGM {
;

static bool ProcessChuntingClear(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 1)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // 경쟁헌팅 전적 초기화
    pPlayer->GetCompetitionInfo().ClearScore();

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    //GAMELOG->LogGM(GM_CHUNTING_CLEAR, pPlayer);

    return true;
}


static bool ProcessChuntingIncWin(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    MSG_CG_GM_STRING_CMD_NAK NakMsg;

    if (vecCmd.size() != 2)      //인자 조건이 맞지 않습니다.
    {
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    int iIncVCount = atoi(vecCmd[1].c_str());

    // 경쟁헌팅 연승 추가
    if (ePRS_AT_CHUNTING == pPlayer->GetGameZoneState())
    {
        GameCHuntingRoom *pCHuntingRoom = (GameCHuntingRoom *)pPlayer->GetGameZonePtr();
        if (pCHuntingRoom)
        {
            pCHuntingRoom->GetCHuntingManager()->IncWinCount(pPlayer, iIncVCount);

            SendGMCmdAck(pPlayer, vecCmd[0].c_str());

            //GAMELOG->LogGM(GM_CHUNTING_INCWIN, pPlayer);
        }
    }
    else
    {
        NakMsg.m_byErrorCode = RC::RC_GM_CURROOM_ISNOT_CHUNTING;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    return true;
}


static bool ProcessClearChaoState(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 1)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // 카오상태 초기화
    pPlayer->GetChaoState().ClearChaoState();

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}


// <Waverix, 060608>
// <Desc>
//   <FORMAT: 1~4args>
//     클리어미션 [미션번호] [완료회수(0~511)] [보상여부]
//     clearmission [미션번호] [완료회수(0~511)] [보상여부]
//   <ARG0> CMD (필수겠죠?)
//     이 인자만 넣는다면, 플레이어의 위치가 미션맵일 경우, 해당 미션 클리어, 그렇지 않는다면, 무시
//   <ARG1> 미션번호 (선택)
//     MissionRewardInfo.txt에 기재된 미션 번호
//     이 인자만 넣고 수행한다면, 기본 액션은 <보상을 받는다>,<완료 카운트 1증가>
////   <ARG2> 보상여부 (선택)
////     보상을 받고자 한다면, 1
////     보상을 받지 않고자 한다면, 0
////     이후 [완료회수]를 넣는 경우,
////       [보상여부]의 1일 경우, [완료회수] 값에 해당하는 보상을 준다. (0 제외)
////       [보상여부]의 0일 경우, [완료회수]값은 무시된다.
////     이후 [완료회수]를 넣지 않는 경우,
////       [보상여부]의 1일 경우, 현재 완료회수에 따른 보상창 띄운다.
////       [보상여부]의 0일 경우 무시된다.
//   <ARG2> 완료회수 (선택)
//     0 : 미션 완료 초기화
//     1~511 : 미션 완료 회수 설정
static bool ProcessClearMission(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    // 기본 옵션
    BYTE byMissionNum = 0;
    BOOL bGetReward = true;
    WORD wMissionPoint = (WORD)-1;
    BOOL bPreUpdate = false; //이전 보상을 적용할 것인가?

    MissionManager* pMissionMng = pPlayer->GetMissionManager();
    pMissionMng->CheckEnableWindowOpen(); // 그럴리 없다고 단정지어진 것이지만, 중복된 보상이 존재할 경우를 대비
    if (pMissionMng->EnableWindowOpen())
    {
        TriggerManagerEx::Check_RewardWindow(pPlayer, true);
        return true;
    }

    DWORD dwCount = (DWORD)vecCmd.size();
    if (dwCount < 5)
    {
        switch(dwCount)
        {
        case 1: // 클리어미션
            {
                // 필드에서 했는지, 미션방에서 했는지 구분할 필요가 있다.
                GameZone * pEnterZone = NULL;
                MAPCODE FieldCode = 0;
                WzID AreaID = 0;
                eZONE_STATE eRoomState = pPlayer->GetGameZone(pEnterZone, FieldCode, &AreaID);
                CODETYPE ctMapCode = pEnterZone->GetMapCode();

                if (ePRS_AT_MISSION == eRoomState)
                {
                    if (!(MAX_MISSION_NUM > ctMapCode))
                        return false;
                    //미션넘버와 맵코드는 동일하다.
                    byMissionNum = (BYTE)ctMapCode;
                }
                else
                {
                    MSG_CG_GM_STRING_CMD_NAK NakMsg;
                    NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
                    pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
                    return false;
                }

                bGetReward = true;
                wMissionPoint = pMissionMng->GetMissionPoint(byMissionNum);
                bPreUpdate = true;
            }
            break;
        default:    // 클리어미션 [미션번호]
            //미션번호
            if (dwCount > 1)
            {
                int tmpNum = atoi(vecCmd[1].c_str());
                if (!tmpNum || !((DWORD)tmpNum < (DWORD)MAX_MISSION_NUM))
                {
                    MSG_CG_GM_STRING_CMD_NAK NakMsg;
                    NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
                    pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
                    return false;
                }

                byMissionNum = (BYTE)tmpNum;
                wMissionPoint = pMissionMng->GetMissionPoint(byMissionNum);

                bGetReward = true;
            }

            //[완료회수]
            if (dwCount > 2) //== 3
            {   // 클리어미션 [미션번호] [완료회수(0~511)]
                int iCnt = atoi(vecCmd[2].c_str());
                if (!((DWORD)iCnt < (DWORD)512))
                {
                    MSG_CG_GM_STRING_CMD_NAK NakMsg;
                    NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
                    pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
                    return false;
                }

                if (!iCnt)
                {
                    bGetReward = false;
                    wMissionPoint = 0;
                }
                else
                {
                    wMissionPoint = (WORD)iCnt;
                }
                bPreUpdate = false;
            }

            //보상여부
            if (dwCount > 3)
            {   // 클리어미션 [미션번호] [완료회수(0~511)] [보상여부(1/0)]
                int tmpFlag = atoi(vecCmd[3].c_str());
                if (!SAFE_NUMERIC_TYPECAST(int, tmpFlag, BYTE))
                {
                    MSG_CG_GM_STRING_CMD_NAK NakMsg;
                    NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
                    pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
                    return false;
                }

                bGetReward = (!tmpFlag) ? false : true;
            }

            break;
        } // end switch

        // 결과 취합
        printf("GMCMD_ClearMission:User [%u] MissonNumber[%u], MissionPoint[%u], RewardState [%d]\n",
            pPlayer->GetUserKey(), byMissionNum, wMissionPoint, bGetReward);

        if (bGetReward)
        {
            MissionRewardManager* pRewardMng = MissionRewardManager::Instance();
            pRewardMng->Init();

            MissionRewardArgs& rMRA = pRewardMng->GetMissionRewardArgs();
            rMRA.SetPlayer(pPlayer);
            rMRA.SetMissionManager(pMissionMng);
            rMRA.SetNum(byMissionNum);
            pMissionMng->SetMissionPoint(byMissionNum, wMissionPoint, false);
            ;{
                MSG_CG_MISSION_SET_CLEAR_COUNT_CMD cmd;
                cmd.m_MapCode    = byMissionNum;
                cmd.m_ClearCount = wMissionPoint;
                pPlayer->SendPacket(&cmd, sizeof(cmd));
            };
            // 필드의 모든 사람에게 미션 클리어 정보 저장
            // 중복 보상은 없다!
            WORD wRewardCode = pRewardMng->GetRewardCode(bPreUpdate);
            ASSERT(0xFFFF != wRewardCode);

            if (0xFFFF != wRewardCode)
            {
                MSG_CG_TRIGGER_REWARD_WINDOW_CMD cmsg;
                cmsg.m_RewardCode = wRewardCode;
                pPlayer->SendPacket(&cmsg, sizeof(cmsg));

                ASSERT(pMissionMng->GetMissionState(byMissionNum) != MISSION_REWARD);
                pMissionMng->SetMissionState(byMissionNum, MISSION_REWARD);
                //              pMissionMng->SetMissionPoint(byMissionNum, wMissionPoint, false);
            }

            return true;
        }
        else
        {
            if (wMissionPoint == 0)
            {
                pMissionMng->SetMissionPoint(byMissionNum, 0);

                MSG_CG_MISSION_SET_CLEAR_COUNT_CMD cmd;
                cmd.m_MapCode       = byMissionNum;
                cmd.m_ClearCount    = 0;
                pPlayer->SendPacket(&cmd, sizeof(cmd));
            }
        }
    }

    MSG_CG_GM_STRING_CMD_NAK NakMsg;
    NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
    pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
    return false;
}



static bool ProcessChangeACCountCapMonKill(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
    {   
        return false;
    }

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    MSG_CG_GM_STRING_CMD_NAK NakMsg;
    NakMsg.m_byErrorCode = RC::RC_GM_CMD_FAILED; 
    pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
    return false; 
#else

    int nArgSize = (int)vecCmd.size();
    if (nArgSize != 3)
    {   
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT; 
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }   

    GameField* pField = pPlayer->GetField();
    if (!pField)
    {   
        return false;
    }
    AchievementManager* pACManager
        = pPlayer->GetACManager();
    if (NULL == pACManager)
    {
        return false;
    }

    int nMapCode    = atoi(vecCmd[1].c_str()); 
    int nnChangeVal = atoi(vecCmd[2].c_str());

    return pACManager->AddConditionValue(nMapCode, eACC_HUNTING_KILL_CAPTAIN, nnChangeVal);
#endif C_NA_0_20100520_ACSYSTEM_CHANGED
}

#ifdef __NA_0_ADD_GM_CMD_ABOUT_AC
static bool ProcessACInit(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }
    AchievementManager* pACManager
        = pPlayer->GetACManager();
    if (!pACManager)
        return false;

    int iAreaIndex  = max(atoi(vecCmd[1].c_str()), 1); // AreaIndex

    if (!pACManager->ClearAC(iAreaIndex))
        return false;
    
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    //pPlayer->GetACManager()->Serialize(ac_msg.achievement_info_, ACSERIALIZE_CLIENT_LOAD);
    //pPlayer->SendPacket(&ac_msg, ac_msg.GetSize());
#else
    MSG_CG_AREACONQUEST_STREAM_CMD ac_msg;
    pPlayer->GetACManager()->Serialize(ac_msg.m_ACStream, MAX_ACSTREAM_SIZE, ACSERIALIZE_LOAD);
    pPlayer->SendPacket(&ac_msg, sizeof(ac_msg));
#endif

    return true;
}

static bool ProcessACValueChange(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false; 

    if (vecCmd.size() != 5)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    AchievementManager* pACManager = pPlayer->GetACManager();
    if (!pACManager)
        return false;

    WORD location_code      = atoi(vecCmd[1].c_str());
    WORD achievement_indwx  = atoi(vecCmd[2].c_str());
    BYTE object_indwx       = atoi(vecCmd[3].c_str());
    WORD value              = atoi(vecCmd[4].c_str());

    if (pACManager->SetConditionValue(location_code, achievement_indwx, object_indwx, value)) {
        return true;
    };
    return false;
}

#endif

//{__NA001283_20090303_COLLECTION_GM
static bool ProcessCollectable(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    char ch[5] = {0,};
    _tcsncpy(ch, vecCmd[1].c_str(), 4);

    WzID CollectionID = StrToWzID(ch);

    GameField* pField = pPlayer->GetField();
    CHKSendErr(!pField, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    CollectionManager* pMng = pField->GetCollectManeger();
    RC::eCOLLECT_RESULT rc = pMng->GmCancel(CollectionID);
    CHKSendErr(RC::RC_COLLECT_SUCCESS != rc , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_CMD_FAILED, pPlayer);

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}
//}__NA001283_20090303_COLLECTION_GM

//{__NA_0_20090407_QUESTACCEPT_GM_CMD

static bool ProcessAcceptQuest(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    int QCode = atoi(vecCmd[1].c_str());

    //__NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
    if (pPlayer->GetQuestManager()->IsQuestMax())
    {
        MSG_CG_QUEST_ACCEPT_NAK msg;
        msg.m_byErrorCode = RC::RC_QUEST_CANNOTQUEST_MAXCOUNT;
        pPlayer->SendPacket(&msg, sizeof(msg));
        return false;
    }

    Quest* pQuest = pPlayer->GetQuestManager()->CreateQuest(QCode);
    if (!pQuest)
    {   
        MSG_CG_QUEST_ACCEPT_NAK msg;
        msg.m_byErrorCode = RC::RC_QUEST_EMPTY;
        pPlayer->SendPacket(&msg, sizeof(msg));
        return false;
    }

    if( pQuest->GetQuestInfo()->IsChildQuest()  )
    {
        pPlayer->GetQuestManager()->DestroyQuest(QCode);

        MSG_CG_QUEST_ACCEPT_NAK msg;
        msg.m_byErrorCode = RC::RC_QUEST_CHILD;
        pPlayer->SendPacket(&msg, sizeof(msg));
        return false;
    }

    if (false != pPlayer->GetQuestManager()->GmAccept(QCode,pPlayer))
    {
        MSG_CG_QUEST_ACCEPT_CMD msg;
        msg.m_QuestCode = (WORD)QCode;
        pPlayer->SendPacket(&msg, sizeof(msg));
        pPlayer->SendAllFieldCollectionInfo();

        MSG_CG_QUEST_OBJECT_NAVI_POS_CMD msgNavi;
        if (pPlayer->GetQuestManager()->MakeInfoQuestNpcNaviStream(&msgNavi, QCode))
            pPlayer->SendPacket(&msgNavi, msgNavi.GetSize());

        GAMELOG->LogAction(ACT_QUEST_ACCEPT, pPlayer, NULL, QCode);
    }
    else
    {
        pPlayer->GetQuestManager()->DestroyQuest(QCode);

        MSG_CG_QUEST_ACCEPT_NAK msg;
        msg.m_byErrorCode = RC::RC_QUEST_EMPTY;
        pPlayer->SendPacket(&msg, sizeof(msg));
    }

    return true;
}
//}__NA_0_20090407_QUESTACCEPT_GM_CMD


//{__NA_001340_20090519_QUEST_GM_CMD
// 퀘스트 강제 종료
// 아이템 보상을 뺀 나머지 처리
static bool ProcessCompleteQuest(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;


    //.//

    int QCode = 0;
    DWORD rewardCode = 0;
    Quest* pQuest = NULL;

    S_TRY
    {
        INT nArgSize = (INT)vecCmd.size();
        if (2 != nArgSize)
            S_THROW0();

        QCode = atoi(vecCmd[1].c_str());
        if (false == pPlayer->GetQuestManager()->GmComplete(QCode,pPlayer))
            S_THROW0();

        pQuest = pPlayer->GetQuestManager()->FindQuest(QCode);
        if (!pQuest || 
            QS_COMPLETED != pQuest->GetQState())
            S_THROW0();

        //
        BOOL bChangUp = false;
        const eCHAR_TYPE charType = pPlayer->GetCharType();

        rewardCode = pQuest->GetRewardCode(charType);
        switch((eSPECIAL_QUEST_REWARD)rewardCode)
        {
        case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_BERSERKER:
        case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_DRAGON_KNIGHT:
        case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_SHADOW:
        case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_VALKYRIE:
        case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_ELEMENTALIST:
            bChangUp = true;
            break;
        };

        if (bChangUp)
            S_THROW0();
    }
    S_CATCH0()
    {
        _SendErr(MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);
        return false;
    }


    //.//

    const MONEY PreMoney = pPlayer->GetMoney();
    const EXPTYPE OldExp = pPlayer->GetExp();

    MSG_CG_QUEST_REWARD_CMD msg;

    S_TRY_
    {
        RC::eREWARD_RESULT rt = RC::RC_REWARD_FAILED;

        BYTE byExpRatio = pPlayer->GetFatigueManager()->GetExpRatio();
        rt = g_RewardManager.RewardExp(rewardCode, pPlayer, msg.m_i64Exp, byExpRatio);
        if (RC::RC_REWARD_FAILED == rt)
            S_THROW0();

        BYTE byHeimRatio = pPlayer->GetFatigueManager()->GetHeimRewardRatio();
        rt = g_RewardManager.RewardHime(rewardCode, pPlayer, msg.m_Money, byHeimRatio);
        if (RC::RC_REWARD_FAILED == rt)
            S_THROW0();

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
        rt = g_RewardManager.RewardAccumulatePoint(rewardCode, pPlayer, msg.m_wAccumulatePoint);
        if (RC::RC_REWARD_FAILED == rt)
            S_THROW0();
        GAMELOG->LogAccumulatePoint_GM(GM_ACQUISITION_ACCMULATE_POINT_BY_QUEST, pPlayer, msg.m_wAccumulatePoint, 0);
#endif

        g_RewardManager.RewardSkill(rewardCode, pPlayer, msg.m_SkillCode);
        g_RewardManager.RewardChaoTime(rewardCode, pPlayer);
    }
    S_CATCH0()
    {
        _SendErr(MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_CMD_FAILED, pPlayer);
        return false;
    }

    //.//
    _FATIGUE_TYPE* pFatigue = pPlayer->GetFatigueManager()->GetCurFatigueType();
    if (pFatigue)
        msg.m_byFatigueType = pFatigue->m_byType;
    else
        msg.m_byFatigueType = 0;

    msg.m_wQuestCode = pQuest->GetQuestCode();
    pPlayer->SendPacket(&msg, msg.GetSize());
    pPlayer->GetQuestManager()->DoReward(QCode, pPlayer);
    pPlayer->GetQuestManager()->SendQuestStream();

    MSG_CG_QUEST_OBJECT_NAVI_POS_CMD msgNavi;
    if (pPlayer->GetQuestManager()->MakeInfoQuestNpcNaviStream(&msgNavi))
        pPlayer->SendPacket(&msgNavi, msgNavi.GetSize());


    //.//

    GAMELOG->LogAction(ACT_QUEST_REWARD, pPlayer, NULL, QCode);

    if ((msg.m_Money-PreMoney) != 0)
        GAMELOG->LogMoney(MONEY_REWARD_QUEST, pPlayer, (msg.m_Money-PreMoney));

    if ((msg.m_i64Exp-OldExp) != 0)
        GAMELOG->LogExp(EXP_QUEST, pPlayer, msg.m_i64Exp, OldExp);

    return true;
}


static bool ProcessResetQuest(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    int QCode = atoi(vecCmd[1].c_str());

    CHKSendErr(false == pPlayer->GetQuestManager()->GmReset(QCode,pPlayer),
        MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    pPlayer->GetQuestManager()->SendQuestStream();

    return true;
}

//}__NA_001340_20090519_QUEST_GM_CMD

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
static bool ProcessAccumulatePoint(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (pPlayer == 0)
        return false;

    AccumulatePoint* pAccum = pPlayer->GetAccumulatePoint();
    ASSERT(pAccum);
    if (pAccum == NULL)
        return false;

    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    int Point = atoi(vecCmd[1].c_str());
    if (Point >= 0)
    {
        pAccum->PlusAccumulatePoint((ushort)Point);
        GAMELOG->LogAccumulatePoint_GM(GM_ACQUISITION_ACCMULATE_POINT, pPlayer, Point, 0);
    }
    else
    {
        ushort AbsPoint = (ushort)abs(Point);
        pAccum->MinusAccumulatePoint(AbsPoint);
        GAMELOG->LogAccumulatePoint_GM(GM_CONSUMPTION_ACCMULATE_POINT, pPlayer, AbsPoint, 0);
    }

    MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD send_message;
    send_message.m_dwReason = RC::RC_ACCUMULATE_POINT_SUCCESS;
    send_message.m_dwAccumulateTime = pAccum->get_accumulate_time();
    send_message.m_wAccumulatePoint = pAccum->get_accumulate_point();
    send_message.m_byRequirePoint = pAccum->get_require_point();
    pPlayer->SendPacket(&send_message, sizeof(send_message));
    return true; // (CHANGES) (f100503.2L) (WAVERIX) add return value to success result
}
#endif


#ifdef _NA_0_20100528_ACCUMULATE_POINT_EVENT
static bool ProcessAccumulateEventTime(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (pPlayer == 0)
        return false;

    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(4 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    ushort Dayflag = (ushort)atoi(vecCmd[1].c_str());
    uint StartSecond = (uint)atoi(vecCmd[2].c_str());
    uint EndSecond = (uint)atoi(vecCmd[3].c_str());

    if ((Dayflag < AccumulatePointParser::Min_Day) || (AccumulatePointParser::Max_Day <= Dayflag))
        return false;

    AccumulatePointParser::RewardGroup& reward_group = (AccumulatePointParser::RewardGroup&)
        AccumulatePointParser::Instance()->get_reward_group(Dayflag);

    AccumulatePointParser::RewardGroup::RewardEvent& reward_event = reward_group.reward_event;
    reward_event.start_time = StartSecond;
    reward_event.end_time = EndSecond;

    return true;
}
#endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
//static bool ProcessHonorTitle(DWORD player_key, const GmCmdTokens& vecCmd)
//{
//    GMCmdManager* const gm_manager = GMCmdManager::Instance();
//    Player* player = gm_manager->GetGMPlayer(player_key);
//    if (player == 0)
//        return false;
//
//    INT nArgSize = (INT)vecCmd.size();
//    CHKSendErr(2 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);
//
//    GameField* pField = player->GetField();
//    if (!pField)
//        return false;
//
//    WORD honor_title = (WORD)atoi(vecCmd[1].c_str());
//
//    player->GetHonorManager()->SetHonorTitle(honor_title);
//
//    return true;
//}

static bool ProcessHonorTitleAdd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

    WORD honor_title = (WORD)atoi(vecCmd[1].c_str());

    player->GetHonorManager()->RequestAddHonorTitle(honor_title);

    return true;
}

static bool ProcessHonorTitleRemove(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

    WORD honor_title = (WORD)atoi(vecCmd[1].c_str());

    player->GetHonorManager()->RequestRemoveHonorTitle(honor_title);

    return true;
}

static bool ProcessHonorPoints(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(4 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

    WORD type_ = (WORD)atoi(vecCmd[1].c_str());
    WORD code_ = (WORD)atoi(vecCmd[2].c_str());
    DWORD point_ = (DWORD)atoi(vecCmd[3].c_str());

    player->GetHonorManager()->SetFameRepute(type_, code_, point_);

    return true;
}

static bool ProcessReputeUp(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(3 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

    WORD code_ = (WORD)atoi(vecCmd[1].c_str());
    DWORD point_ = (DWORD)atoi(vecCmd[2].c_str());

    player->GetHonorManager()->AddReputePoint(code_, point_);

    return true;
}

static bool ProocessDailyQuest(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    //INT nArgSize = (INT)vecCmd.size();
    //CHKSendErr(3 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

    const char* command = vecCmd[1].c_str();
    /*if(!strcmp(command, "add"))
    {
        WORD code = (WORD)atoi(vecCmd[2].c_str());
        BYTE status = (BYTE)atoi(vecCmd[3].c_str());
        CTime time_now = CTime::GetCurrentTime();
        
        MSG_DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN msg;
        msg.quest_data.code_            = code;
        msg.quest_data.status_          = status;
        msg.quest_data.start_datetime_  = time_now;
        msg.quest_data.end_datetime_    = 0;
        player->SendToGameDBPServer(&msg, sizeof(msg));
    }*/

    if(!strcmp(command, "clear"))
    {
        player->GetHonorManager()->GMDailyQuestClear();
    }

    return true;
}

static bool ProcessTodayQuest(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    //INT nArgSize = (INT)vecCmd.size();
    //CHKSendErr(3 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

    const char* command = vecCmd[1].c_str();
    if(!strcmp(command, "reset"))
    {
        player->GetHonorManager()->GMTodayQuestReset();
    }

    return true;
}
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
static bool ProcessPointWallet(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    DWORD code  = static_cast<DWORD>(atoi(vecCmd[1].c_str()));
    DWORD point = static_cast<DWORD>(atoi(vecCmd[2].c_str()));

    PointWalletManager* point_wallet_manager = player->GetPointWalletManager();
    point_wallet_manager->ChangeWalletPoint(code, point);

    return true;
}
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
static bool ProcessGuildCoin(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    const char* command = vecCmd[1].c_str();
    if(!strcmp(command, "give")) {
        int point = 1;
        const char* point_str = vecCmd[2].c_str();
        if (point_str != "")
            point = static_cast<int>(atoi(point_str));
        player->GetPointWalletManager()->GuildPointDonation(point);
    }
    else if(!strcmp(command, "up")) {
        const char* point_str = vecCmd[2].c_str();
        if (point_str != "") 
        {
            int point = static_cast<DWORD>(atoi(point_str));
            
            PointWalletManager* point_wallet_manager = player->GetPointWalletManager();
            point_wallet_manager->ProcessGuildCoinDonation(0, point, point);
        }
    }

    return true;
}

static bool ProcessGuildRanking(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    const char* command = vecCmd[1].c_str();
    if(!strcmp(command, "refresh")) 
    {
        MSG_AG_GM_GUILD_RANKING_REFRESH msg;
        player->SendPacket(&msg, sizeof(msg));
    }

    return true;
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
static bool ProcessGuildIntroduction(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    const char* introduction = vecCmd[1].c_str();
    if(introduction[0] != '\0') 
    {
#ifndef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        MSG_GZ_GUILD_INTRODUCTION_SAVE_SYN msg;
        msg.init();
        msg.char_guid = player->GetCharGuid();
        msg.guild_guid = player->GetGuildGuid();
        strcpy_s(msg.guild_introductions_.introduction_strings_, MAX_GUILD_INTRODUCTION_LENGTH, introduction);
        msg.guild_introductions_.introduction_length_ = strlen(msg.guild_introductions_.introduction_strings_);
        player->SendToGuildServer(&msg, sizeof(msg));
#endif  //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    }

    return true;
}
#endif //_NA_004034_20120102_GUILD_INTRODUCTION

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
static bool ProcessVendorSearch(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(3 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

    const char* search_name = vecCmd[1].c_str();
    int page = atoi(vecCmd[2].c_str());
    
    g_VendorManager.vendor_search_find(player, search_name, page);

    return true;
}
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

static bool ProcessReload(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == 0)
        return false;

    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

    const char* command = vecCmd[1].c_str();

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
    if (strcmp(command, "npclua") == 0)
    {
        g_ExtraNPCManager.release();
        g_ExtraNPCManager.initialize();
    }
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

    return true;
}

}; //end of namespace 'nsGM'


bool GMCmdManager::RegisterRecord(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "헌팅클리어", "chuntingclear",  &ProcessChuntingClear),
        GMHANDLER_MAPPING(true, "헌팅연승", "chuntingincwin",  &ProcessChuntingIncWin),
        GMHANDLER_MAPPING(true, "클리어카오", "clearchao",  &ProcessClearChaoState),
        GMHANDLER_MAPPING(true, "클리어미션", "clearmission",  &ProcessClearMission),
        GMHANDLER_MAPPING(true, "캡틴베기", "killcaptain",  &ProcessChangeACCountCapMonKill), // 캡틴베기
        GMHANDLER_MAPPING(true, "채집초기화", "collectable",  &ProcessCollectable),
        GMHANDLER_MAPPING(true, "ac초기화", "acreset",  &ProcessACInit),
        GMHANDLER_MAPPING(true, NULL, "ac",  &ProcessACValueChange),
        GMHANDLER_MAPPING(true, "퀘스트수락", "acceptq",  &ProcessAcceptQuest),
        GMHANDLER_MAPPING(true, "퀘스트완료", "finishq",  &ProcessCompleteQuest),
        GMHANDLER_MAPPING(true, "퀘스트초기화", "resetq",  &ProcessResetQuest),
    #ifdef _NA_1419_20100121_ACCUMULATE_POINT
        GMHANDLER_MAPPING(true, "적립포인트", "accumulatepoint",  &ProcessAccumulatePoint),
    #endif
    #ifdef _NA_0_20100528_ACCUMULATE_POINT_EVENT
        // unused
        GMHANDLER_MAPPING(false, NULL, NULL,  &ProcessAccumulateEventTime),
    #endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        //GMHANDLER_MAPPING(true, NULL, "honortitle",  &ProcessHonorTitle),
        GMHANDLER_MAPPING(true, NULL, "honortitle_add",  &ProcessHonorTitleAdd),
        GMHANDLER_MAPPING(true, NULL, "honortitle_remove",  &ProcessHonorTitleRemove),
        GMHANDLER_MAPPING(true, NULL, "honorpoint", &ProcessHonorPoints),
        GMHANDLER_MAPPING(true, NULL, "reputeup", &ProcessReputeUp),
        GMHANDLER_MAPPING(true, NULL, "dailyquest", &ProocessDailyQuest),
        GMHANDLER_MAPPING(true, NULL, "todayquest", &ProcessTodayQuest),
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
        GMHANDLER_MAPPING(true, NULL, "pointwallet", &ProcessPointWallet),
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        GMHANDLER_MAPPING(true, NULL, "guildcoin", &ProcessGuildCoin),
        GMHANDLER_MAPPING(true, NULL, "guildranking", &ProcessGuildRanking),
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
        GMHANDLER_MAPPING(true, NULL, "guildintroduction", &ProcessGuildIntroduction),
#endif //_NA_004034_20120102_GUILD_INTRODUCTION
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
        GMHANDLER_MAPPING(true, NULL, "vendorsearch", &ProcessVendorSearch),
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
        GMHANDLER_MAPPING(true, NULL, "reload", &ProcessReload),
        //
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}
