#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "uiSUNRankingStatueInfo.h"
#include "uiSUNRankingMan.h"
#include "GlobalFunc.h"
#include "StatuePlayer.h"
#include "ObjectManager.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildMarkInfoParser.h"
#include "uiInventoryMan/uiInventoryMan.h"

WzID uiSUNRankingStatueInfo::m_wzId[DIALOG_MAX] = 
{
	StrToWzID("PI00"),
	StrToWzID("PI02"),
	StrToWzID("BT00"),
	StrToWzID("P007"),
	StrToWzID("ST01"),
	StrToWzID("B999"),
};

uiSUNRankingStatueInfo::uiSUNRankingStatueInfo(InterfaceManager* ui_manager) : 
uiBase(ui_manager),
guild_mark_control_(NULL),
guild_mark_background_control_(NULL),
statue_name_picture_control_(NULL),
equip_show_button_control_(NULL),
statue_description_control_(NULL),
statue_player_key_(0)
{
}

uiSUNRankingStatueInfo::~uiSUNRankingStatueInfo(void)
{
	guild_mark_control_ = NULL;
	guild_mark_background_control_ = NULL;
	statue_name_picture_control_ = NULL;
	equip_show_button_control_ = NULL;
	statue_description_control_ = NULL;
	statue_player_key_ = 0;
}

void uiSUNRankingStatueInfo::Init( CDrawBase* draw_base )
{
	uiBase::Init(draw_base);

	guild_mark_control_ = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_PICTURE_PI00]));
	guild_mark_background_control_ = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_PICTURE_PI02]));
	statue_name_picture_control_ = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_PICTURE_P007]));
	RECT rc = {0, };
	statue_name_ftext_render_.Init(draw_base, rc);
	if ( statue_name_picture_control_ )
	{
		rc = statue_name_picture_control_->GetSizeRect();
		statue_name_ftext_render_.SetRect(rc);
		statue_name_ftext_render_.SetVCenterAlign(false);
		statue_name_ftext_render_.SetHCenterAlign(true);
	}

	statue_description_ftext_render_.Init(draw_base, rc);
	statue_description_control_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_TEXT_ST01]));
	if ( statue_description_control_ )
	{
		statue_description_control_->SetTextWZ(_T(""));
		rc = statue_description_control_->GetSizeRect();
		statue_description_ftext_render_.SetRect(rc);
		statue_description_ftext_render_.SetVCenterAlign(false);
		statue_description_ftext_render_.SetHCenterAlign(true);
	}

	TCHAR statue_description_string[INTERFACE_STRING_LENGTH] = {0, };
	g_InterfaceManager.GetInterfaceString(3622, statue_description_string);
	if ( statue_name_ftext_parser_.Parser(statue_description_string, statue_name_ftext_sentence_))
	{
		statue_description_ftext_render_.ClearSentence();
		statue_description_ftext_render_.Append(statue_name_ftext_sentence_);
		statue_name_ftext_sentence_.Release();
	}
}

void uiSUNRankingStatueInfo::OnShowWindow( BOOL is_show )
{
	if ( is_show )
	{
		MoveDefaultWindowPos();

		keyMsg msg;
		ZeroMemory(&msg, sizeof(msg));
		msg.dwType = eDoType_SUNRanking_Statue_Info;
		msg.wParam = IM_SUNRANKING_MANAGER::SUNRANKING_STATUE_INFO;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		statue_player_key_ = 0;
		g_KeyQueueManager.DeleteMsg(eDoType_SUNRanking_Statue_Info);
	}
}

void uiSUNRankingStatueInfo::OnUpdateSolarDialog()
{
	if ( statue_name_picture_control_ )
	{
		RECT rc = statue_name_picture_control_->GetSizeRect();
		statue_name_ftext_render_.SetRect(rc);
	}

	if ( statue_description_control_ )
	{
		RECT rc = statue_description_control_->GetSizeRect();
		statue_description_ftext_render_.SetRect(rc);
	}
}

void uiSUNRankingStatueInfo::OnRenderSolarDialog()
{
	HANDLE hGuildMarkTexture = GuildSystem::GetGuildSystemData().GetGuildMarkTexture();

	//점령 길드 마크 출력
	if (hGuildMarkTexture != INVALID_HANDLE_VALUE)
	{
		ENUM_ALPHABLEND eOldAlphaMode =	g_pSunRenderer->GetAlphaBlendMode();
		BASE_GuildMarkInfo* pMarkInfo = NULL;

		RECT rtSize = guild_mark_control_->GetSizeRect();

		float fWidth = (float) rtSize.right - rtSize.left;
		float fHeight = (float) rtSize.bottom - rtSize.top;
		g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);

		WzColor Color[4] = {WzColor_RGBA(255,255,255,255)
			, WzColor_RGBA(255,255,255,255)
			, WzColor_RGBA(255,255,255,255)
			, WzColor_RGBA(255,255,255,255)};

		for(int i =0; i < 2; ++i)
		{
			StatuePlayer* target_object = static_cast<StatuePlayer*>(g_ObjectManager.GetObject(statue_player_key_));
			if (target_object && (target_object->IsKindOfObject(STATUE_PLAYER_OBJECT)))
			{
				StatueGuildInfo statue_guild_info = target_object->get_statue_guild_info();
				if( i == 0)
					pMarkInfo =  GuildMarkInfoParser::Instance()->GetGuildMarkInfo(statue_guild_info.guild_mark_background_idx);
				else
					pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(statue_guild_info.guild_mark_pattern_idx);
			}

			if( pMarkInfo )
			{
				g_pSunRenderer->RenderTexture(hGuildMarkTexture, (float)rtSize.left , (float)rtSize.top, fWidth , fHeight,
					(float)pMarkInfo->iRow * 32.0f , (float) pMarkInfo->iColumn * 32.0f, 32.0f, 32.0f, Color);
			}
		}
		g_pSunRenderer->SetAlphaBlendMode(eOldAlphaMode);
	}

	statue_name_ftext_render_.Render();

	statue_description_ftext_render_.Render();
}

void uiSUNRankingStatueInfo::MessageProc( SI_MESSAGE* message )
{
	if ( IsLock() )
		return;

	if ( GetMan() == NULL )
		return;

	switch ( message->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			OnLButtonClick( message );
		}
		break;
	default:
		SolarDialog::MessageProc( message );
		break;
	}
}

void uiSUNRankingStatueInfo::UpdateStatuePlayerInfo( const DWORD& statue_player_key )
{
	if ( statue_player_key == 0 )
	{
		return;
	}

	statue_player_key_ = statue_player_key;
	StatuePlayer* statue_player = static_cast<StatuePlayer*>(g_ObjectManager.GetObject(statue_player_key_));
	if ( statue_player )
	{
		TCHAR guild_result_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR* guild_name;
		TCHAR guild_format_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR ftext_format_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
		const TCHAR* statue_name = statue_player->GetName();

		StatueGuildInfo statue_guild_info = statue_player->get_statue_guild_info();
		guild_name = statue_guild_info.guild_name;
		bool is_guild = true;
		if ( strcmp(guild_name, "0") == 0 )
		{
			is_guild = false;
		}

		if ( is_guild == true )
		{
			// 356 : 길드
			g_InterfaceManager.GetInterfaceString(356, guild_format_string);			
			Snprintf(guild_result_string, INTERFACE_STRING_LENGTH, _T("%s %s"), guild_name, guild_format_string);

			Snprintf(result_string, 
				INTERFACE_STRING_LENGTH-1, 
				_T("<font=tt12><color=ff00ff00>%s</font></color><return=1/><font=tt12><color=fff0c979>%s</font></color>"), guild_result_string, statue_name);
		}
		else
		{
			Snprintf(result_string, 
				INTERFACE_STRING_LENGTH-1, 
				_T("<return=1/><font=tt12><color=fff0c979>%s</font></color>"), statue_name);
		}
		
		 if ( statue_name_ftext_parser_.Parser(result_string, statue_name_ftext_sentence_))
		 {
			 statue_name_ftext_render_.ClearSentence();
			 statue_name_ftext_render_.Append(statue_name_ftext_sentence_);
			 statue_name_ftext_sentence_.Release();
			 if ( statue_name_picture_control_ )
			 {
				 RECT rc = statue_name_picture_control_->GetSizeRect();
				 statue_name_ftext_render_.SetRect(rc);
			 }
		 }
	}
}

bool uiSUNRankingStatueInfo::OnLButtonClick( SI_MESSAGE* message )
{
	DWORD control_position = getCtrlIDToPos( message->dwCtrlID );
	switch ( control_position )
	{
	case DLGPOS_BUTTON_BT00:
		{
			// 장비 보기 버튼
			//해당 플레이어의 장비 창을 보여주자.
			uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
			if (statue_player_key_ && inventory_manager)
			{
				Object* target_object = g_ObjectManager.GetObject(statue_player_key_);
				if (target_object && (target_object->IsKindOfObject(STATUE_PLAYER_OBJECT)))
				{
					Player* player = static_cast<Player *>(target_object);
					StatuePlayer* statue_player = static_cast<StatuePlayer*>(target_object);
					if (player)
					{
						if ( inventory_manager->GetOtherPlayerEquipmentContainerVisible() == FALSE )
						{
							statue_player->SetObjectType(PLAYER_OBJECT);
							statue_player->set_equip_container_(statue_player->get_proto_equip_container_());
							statue_player->SetHideWing(FALSE);

							inventory_manager->SetOtherPlayerEquipInventoryDialog(player);

							statue_player->SetObjectType(STATUE_PLAYER_OBJECT);
							statue_player->set_equip_container_(statue_player->get_statue_equip_container_());
							statue_player->SetHideWing(TRUE);
						}
					}
				}
			}
		}
		break;
	case DLGPOS_BUTTON_B999:
		{
			ShowInterface(FALSE);
		}
		break;
	}

	return true;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
