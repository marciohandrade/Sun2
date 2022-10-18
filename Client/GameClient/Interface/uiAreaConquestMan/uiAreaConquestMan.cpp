#include "SunClientPrecompiledHeader.h"
#include "uiareaconquestman.h"
#include "interfacemanager.h"
#include "Achievement.h"
#include "AchievementSearch.h"
#include "GlobalFunc.h"
#include "uiAreaConquestDialog/uiAreaConquestDialog.h"
#include "uiAreaConquestCompliment/uiAreaConquestCompliment.h"
#include "ItemManager.h"
#include "Hero.h"
#include "Item.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "HonorSystem/HonorSystem.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM

#include "ChatMsg.h"


const DWORD IM_AREACONQUEST_MANAGER::AREACONQUEST_DIALOG = StrToWzID("0921");
const DWORD IM_AREACONQUEST_MANAGER::AREACONQUEST_COMPLIMENT = StrToWzID("0922");
const char  c_strDlgFname_AreaConquest[] = ("Data\\Interface\\92_1_AC.iwz");

uiAreaConquestMan::uiAreaConquestMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    ui_ac_dialog_ = NULL;
    ui_ac_notice_ = NULL;
    achievement_ = NULL;
}

//------------------------------------------------------------------------------
/**
*/


void uiAreaConquestMan::OnInitialize()
{
     ui_ac_dialog_ = CreateUIDialog<uiAreaConquestDialog>(
         IM_AREACONQUEST_MANAGER::AREACONQUEST_DIALOG,
         c_strDlgFname_AreaConquest,
         this, TRUE);

     ui_ac_notice_ = CreateUIDialog<uiAreaConquestCompliment>(
         IM_AREACONQUEST_MANAGER::AREACONQUEST_COMPLIMENT,
         "Data\\Interface\\92_2_result.iwz",
         this, TRUE);

    achievement_ = new Achievement;
#ifdef _NA_006541_RENEWER_ACDIALOG
    Load_Ac_Data();

    std::sort(ac_data_list_.begin(),ac_data_list_.end(),_stAcTreelist::list_compare_lower);

    //자식들에 대한 sort추가.
    for (int i = 0 ; i < (int)ac_data_list_.size() ; ++i)
    {
        ac_data_list_[i]->sort();
    }

#endif
}

void uiAreaConquestMan::InitPKBooleans()
{
    m_vPKBool.clear();
    for (int index = kPacketPending_Base; index < kPacketPending_Max; ++index)
    {
        uicPacketControl packet_control;
        m_vPKBool.push_back(packet_control);
    }
}

void uiAreaConquestMan::OnRelease()
{
    for( size_t i=0;i!=ac_data_list_.size();++i)
    {
        ac_data_list_[i]->Release();
        delete ac_data_list_[i];
    }
    ac_data_list_.clear();

    SAFE_DELETE(achievement_);
}

void uiAreaConquestMan::Update()
{
    if (ui_ac_notice_ != NULL)
    {
        if ((ui_ac_notice_->IsVisible() == FALSE) &&
            (ui_ac_notice_->IsHaveCompliment() == true))
        {
            ui_ac_notice_->ShowACCompliment(0);
        }
    }
}

bool uiAreaConquestMan::IsCompleteACItem(SLOTCODE item_code)
{
	return true;
}


void uiAreaConquestMan::DeleteComplimentList()
{
    if (ui_ac_notice_ != NULL)
    {
        ui_ac_notice_->DeleteList();
    }
}

bool uiAreaConquestMan::CanPickACItem(Item* item_object, bool is_show_notice)
{
    if (item_object == NULL)
    {
        return false;
    }

    const BASE_ITEMINFO* item_info = item_object->GetItemSlot().GetItemInfo();
    if (item_info != NULL &&
        item_info->m_wACCode != 0 &&
        item_info->m_wType == eITEMTYPE_AC &&
        g_pHero != NULL && 
        g_pHero->CanUseACItem(item_info) == false)
    {
        // 해당 아이템은 AC아이템이며, 수집이 완료되어 더이상 사용할 수 없는 아이템
        // 해당 아이템은 안내메시지를 출력하며 줍지 않도록 한다.
        
        if (is_show_notice == true)
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {0, };
            // 5664 해당 AC아이템은 모두 수집하여 더 이상 습득할 수 없습니다.
            g_InterfaceManager.GetInterfaceString(5664, message);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);

            // 드랍되는 애니메이션 출력
            item_object->InitState(STATE::ITEM_DROP_STATE, g_CurTime);
        }        

        return false;
    }

    //일반이이템 및 AC아이템수집중이면 주을 수 있다.
    return true;
}

void uiAreaConquestMan::NetworkProc(MSG_BASE* recv_msg)
{
    if (recv_msg->m_byCategory != CG_AREACONQUEST)
    {
        return;
    }

    switch (recv_msg->m_byProtocol)
    {
    case CG_AREACONQUEST_STREAM_CMD:
        {
            RECV_CG_AREACONQUEST_STREAM_CMD(recv_msg);
        }
        break;
    case CG_AREACONQUEST_VALUE_CHANGE_CMD:
        {
            RECV_CG_AREACONQUEST_VALUE_CHANGE_CMD(recv_msg);
        }
        break;
    case CG_AREACONQUEST_REWARD_ACK:
        {
            RECV_CG_AREACONQUEST_REWARD_ACK(recv_msg);
        }
        break;
    case CG_AREACONQUEST_REWARD_NAK:
        {
            RECV_CG_AREACONQUEST_REWARD_NAK(recv_msg);
        }
        break;
    }
}

void uiAreaConquestMan::RECV_CG_AREACONQUEST_STREAM_CMD(MSG_BASE* recv_msg)
{
    MSG_CG_AREACONQUEST_STREAM_CMD* recv_packet = 
        static_cast<MSG_CG_AREACONQUEST_STREAM_CMD*>(recv_msg);

    if (achievement_ != NULL)
    {
        achievement_->Init(NULL);
        achievement_->Serialize(recv_packet->achievement_info_, ACHIEVEMENT_SAVE);
    }    

    if (ui_ac_dialog_ != NULL)
    {
        ui_ac_dialog_->InitACDialog();
    }
}

void uiAreaConquestMan::RECV_CG_AREACONQUEST_VALUE_CHANGE_CMD(MSG_BASE* recv_msg)
{
    if (achievement_ != NULL)
    {
        MSG_CG_AREACONQUEST_VALUE_CHANGE_CMD* recv_packet = 
            static_cast<MSG_CG_AREACONQUEST_VALUE_CHANGE_CMD*>(recv_msg);

        achievement_->SetAchievementValue(recv_packet->location_code, recv_packet->index_number_, 
            recv_packet->object_index_, recv_packet->object_value_, 
            ACHIEVEMENT_VALUE_SET);

        if (achievement_->IsAccomplishment(recv_packet->location_code, recv_packet->index_number_))
        {
            ui_ac_notice_->ShowACCompliment(recv_packet->index_number_);
            UpdateCanGetACItem(recv_packet->index_number_);

            if (ui_ac_dialog_ != NULL)
            { 
                ui_ac_dialog_->UpdateValue();
            }
        }
		
		AchievementVaulePrint(recv_packet->index_number_,recv_packet->location_code,recv_packet->object_index_);

        if (ui_ac_dialog_ != NULL)
        {   
            ui_ac_dialog_->OnUpdateACInfo();
        }  
    }    
}

void uiAreaConquestMan::SEND_CG_AREACONQUEST_REWARD_SYN()
{
    if ((ui_ac_dialog_ == NULL) || 
        (CheckPacketPending_Reward() == false))
    {
        return;
    }

    MSG_CG_AREACONQUEST_REWARD_SYN send_packet;
    send_packet.location_code_ = ui_ac_dialog_->GetCurrentAreaCode();
    send_packet.index_number_ = ui_ac_dialog_->GetPendingRewardACIndex();
    
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    SetPKBool(kPacketPending_Reward, TIMEOUT_PACKET_PENDING);
}

void uiAreaConquestMan::RECV_CG_AREACONQUEST_REWARD_ACK(MSG_BASE* recv_msg)
{
    UnsetPKBool(kPacketPending_Reward);

    if (achievement_ == NULL)
    {
        return;
    }

    MSG_CG_AREACONQUEST_REWARD_ACK* recv_packet = 
        static_cast<MSG_CG_AREACONQUEST_REWARD_ACK*>(recv_msg);

    achievement_->RewardAchievement(recv_packet->index_, recv_packet->location_code_, true);
    if (recv_packet->m_accomplishment_ratio > 0)
    {
        achievement_->AddAccomplishmentRation(recv_packet->location_code_, 
            recv_packet->m_accomplishment_ratio);
    }
    else
    {
        if (g_pHero)
        {
            ItemManager::Instance()->InsertInventoryTotalInfo( REWARD, recv_packet->m_ItemInfo );
            g_InterfaceManager.ShowGetMoneyMessage(recv_packet->m_Money);

            g_pHero->SetMoney(recv_packet->m_Money);
            g_pHero->SetExp(recv_packet->m_i64Exp);
        }
    }

    if (ui_ac_dialog_ != NULL)
    {
        ui_ac_dialog_->OnUpdateACInfo();
    }

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    if (recv_packet->honor_title != 0)
    {
        HonorSystem::HonorSystemFunction::OnRewardTitle(recv_packet->honor_title);
    }
#endif //_NA_003027_20111013_HONOR_SYSTEM
}

void uiAreaConquestMan::RECV_CG_AREACONQUEST_REWARD_NAK(MSG_BASE* recv_msg)
{
    UnsetPKBool(kPacketPending_Reward);

    MSG_CG_AREACONQUEST_REWARD_NAK* recv_packet = 
        static_cast<MSG_CG_AREACONQUEST_REWARD_NAK*>(recv_msg);

    TCHAR error_message[INTERFACE_STRING_LENGTH] = {0, };
    g_InterfaceManager.GetStringACError(recv_packet->m_byErrorCode, 
                                        error_message, 
                                        INTERFACE_STRING_LENGTH);
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, error_message, INTERFACE_STRING_LENGTH);
}

void uiAreaConquestMan::UpdateCanGetACItem(WORD index_number)
{
    // 필드에 떨어진 AC아이템들의 *표시를 없앰
    Achievement_ParserInfo* ac_parser_info =
        AchievementParser::Instance()->GetAchievement_ParserInfo(index_number);
    for (int i=0; i<MAX_ACHIEVEMENT_OBJECT; ++i)
    {
        if (ac_parser_info->obejcts_[i].object_code_ == 0)
        {
            break;
        }
        else
        {
            ItemManager::Instance()->UpdateCanGetACItem(ac_parser_info->obejcts_[i].object_code_);
        }
    }
}

void uiAreaConquestMan::AchievementVaulePrint(WORD	index_number_,WORD	location_code,BYTE	object_index_)
{
	AchievementPart* achievement_part =  achievement_->GetAchievementPart(location_code);
	AchievementInfo* achievement_info = NULL;
	if (achievement_part)
	{
		achievement_info = achievement_part->GetAchievement(index_number_);
	}

	if(achievement_info)
	{
		TCHAR szText[INTERFACE_STRING_LENGTH];

		int current_value = 0;
		int achievement_value = 0;
		AchievementObjectInfo ac_object_info;

		WORD ObjType = achievement_info->GetObjectType();

		//ACHIEVEMENT_OBJECT_ITEM타입 의 경우에만 처리 하도록함
		if(ObjType != ACHIEVEMENT_OBJECT_ITEM)
			return;


		for (BYTE index = 0; index < MAX_ACHIEVEMENT_OBJECT; ++index)
		{
			ac_object_info = achievement_info->GetObjectInfo(index);
			current_value += ac_object_info.value_;
			achievement_value += ac_object_info.Achievement_;
		}

		TCHAR	isNameItem[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(achievement_info->GetStringCode(), isNameItem);

		g_InterfaceManager.GetInterfaceStringFormat(szText,INTERFACE_STRING_LENGTH, 5094, isNameItem);
		sprintf(szText,"%s - (%d/%d)",szText,current_value,achievement_value);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szText, CHAT_TYPE_SYSTEM);
	}

}
#ifdef _NA_006541_RENEWER_ACDIALOG
void uiAreaConquestMan::Load_Ac_Data()
{
    ACHIEVEMENTINFO_ITERATOR iter = AchievementParser::Instance()->Begin();
    ACHIEVEMENTINFO_ITERATOR iterEnd = AchievementParser::Instance()->End();

    for ( ; iter != iterEnd ; ++ iter)
    {
        make_ac_tree((*iter));
    }
}

void uiAreaConquestMan::make_ac_tree(Achievement_ParserInfo* info)
{
    // 이미 해당 카테고리가 만들어져 있는 경우 찾아서 추가
    for (int i = 0 ; i < (int)ac_data_list_.size() ; ++i)
    {
        if(info->upper_group_index_ == ac_data_list_[i]->index)
        {
            AddSubItem(info, ac_data_list_[i]);
            return;
        }
    }

    // 새로운 카테고리를 만듬
    _stAcTreelist* Category = new _stAcTreelist();
    Category->perentidx = -1;
    Category->iStringCode = info->upper_group_string_;
    Category->index = info->upper_group_index_;
    Category->type = _stAcTreelist::eType_Main;
    Category->nation_code_ = info->location_code_;
    Category->index_to_ac_ = info->index_;
    ac_data_list_.push_back(Category);

    // 카테고리에 추가
    AddSubItem(info,Category);
}

void uiAreaConquestMan::AddSubItem( Achievement_ParserInfo* info,_stAcTreelist* perent )
{
    _stAcTreelist* Category = NULL;
    bool isFine = false;
    //2단계 계산,
    //2단계만 있으면 되나 정말 레알 리얼?
    for (int i = 0 ; i < (int)perent->childTree.size() ; ++i)
    {
        //해당 아이템의 하위에 들어가야됨.
        if(info->lower_group_index_ == perent->childTree[i]->index)
        {
            Category = perent->childTree[i];
            isFine = true;
            break;
        }
    }

    if(isFine == false)
    {
        Category = new _stAcTreelist();
        Category->perentidx = -1;
        Category->iStringCode = info->lower_group_string_;
        Category->index = info->lower_group_index_;
        Category->type = _stAcTreelist::eType_Sun;
        Category->nation_code_ = info->location_code_;

        perent->AddChild(Category);
    }
}
#endif