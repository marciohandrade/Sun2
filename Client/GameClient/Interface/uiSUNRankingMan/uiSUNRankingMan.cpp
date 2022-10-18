#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "uiSUNRankingMan.h"
#include "uiSUNRankingDialog.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "uiSUNRankingStatueInfo.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "InterfaceManager.h"
#include "GlobalFunc.h"

const DWORD IM_SUNRANKING_MANAGER::SUNRANKING_DIALOG = StrToWzID("0811");
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
const DWORD IM_SUNRANKING_MANAGER::SUNRANKING_STATUE_INFO = StrToWzID("9735");
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

uiSUNRankingMan::uiSUNRankingMan(InterfaceManager* ui_manager) : 
uiBaseMan(ui_manager),
sun_ranking_dialog_(NULL)
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
,sun_ranking_statue_info_(NULL)
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
{
}

uiSUNRankingMan::~uiSUNRankingMan(void)
{
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	statue_info_map_.clear();
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
}

void uiSUNRankingMan::OnInitialize()
{
	sun_ranking_dialog_ = CreateUIDialog<uiSUNRankingDialog>(IM_SUNRANKING_MANAGER::SUNRANKING_DIALOG, "Data\\Interface\\81_1_Ranking.iwz", this, TRUE);
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	sun_ranking_statue_info_ = CreateUIDialog<uiSUNRankingStatueInfo>(IM_SUNRANKING_MANAGER::SUNRANKING_STATUE_INFO, "Data\\Interface\\97_35_statue.iwz", this, TRUE);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

	Is_wait_receive_packet_ = false;
}

void uiSUNRankingMan::ToggleSUNRankingDialog()
{
	if ( sun_ranking_dialog_ == NULL )
	{
		return;
	}

	if (sun_ranking_dialog_->IsVisibleWindow() == TRUE)
	{
		sun_ranking_dialog_->ShowInterface(FALSE);
	}
	else
	{
		sun_ranking_dialog_->ShowInterface(TRUE);
		sun_ranking_dialog_->set_is_init_(true);
	}
}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
void uiSUNRankingMan::ShowSUNRankingStatueInfo( const DWORD& statue_player_key )
{
	if ( sun_ranking_statue_info_ )
	{
		sun_ranking_statue_info_->UpdateStatuePlayerInfo(statue_player_key);
		sun_ranking_statue_info_->ShowInterface(TRUE);
	}
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

void uiSUNRankingMan::InitPKBooleans()
{
	this->m_vPKBool.clear();

	for (int index = PKBOOL_RANKING_STATUS_BASE; index < PKBOOL_RANKING_MAX; ++index)
	{
		uicPacketControl packet_control;
		this->m_vPKBool.push_back(packet_control);
	}
}

void uiSUNRankingMan::Send_CG_ETC_SUN_RANKING_SYN( const int& mode )
{
	MSG_CG_ETC_SUN_RANKING_SYN send_packet;
	send_packet.ranking_type = eSUN_RANKING_TYPE_START;
	send_packet.is_guild_ranking = false;
	switch ( mode )
	{
	case SUN_RANKING_BEST_OF_BEST + PRIVATE_MODE:
	case SUN_RANKING_BEST_OF_BEST + GUILD_MODE:
		{
			send_packet.ranking_type = eSUN_RANKING_TYPE_BEST_OF_BEST;
		}
		break;
	case SUN_RANKING_ELETE_WARRIOR + PRIVATE_MODE:
	case SUN_RANKING_ELETE_WARRIOR + GUILD_MODE:
		{
			send_packet.ranking_type = eSUN_RANKING_TYPE_ELETE_WARRIOR;
		}
		break;
	case SUN_RANKING_LEVEL_KING + PRIVATE_MODE:
	case SUN_RANKING_LEVEL_KING + GUILD_MODE:
		{
			send_packet.ranking_type = eSUN_RANKING_TYPE_LEVEL_KING;
		}
		break;
	case SUN_RANKING_LEGEND_EQUIP + PRIVATE_MODE:
	case SUN_RANKING_LEGEND_EQUIP + GUILD_MODE:
		{
			send_packet.ranking_type = eSUN_RANKING_TYPE_LEGEND_EQUIP;
		}
		break;
	case SUN_RANKING_STRONG_ONE_SHOT + PRIVATE_MODE:
	case SUN_RANKING_STRONG_ONE_SHOT + GUILD_MODE:
		{
			send_packet.ranking_type = eSUN_RANKING_TYPE_STRONG_ONE_SHOT;
		}
		break;
	case SUN_RANKING_BEST_ATTACKKER + PRIVATE_MODE:
	case SUN_RANKING_BEST_ATTACKKER + GUILD_MODE:
		{
			send_packet.ranking_type = eSUN_RANKING_TYPE_BEST_ATTACKER;
		}
		break;
	case SUN_RANKING_MASTER_OF_HEAL + PRIVATE_MODE:
	case SUN_RANKING_MASTER_OF_HEAL + GUILD_MODE:
		{
			send_packet.ranking_type = eSUN_RANKING_TYPE_MASTER_OF_HEAL;
		}
		break;
	default:
		{
			send_packet.ranking_type = eSUN_RANKING_TYPE_END;
		}
	}
	if ( mode % 2 == 1)
	{
		send_packet.is_guild_ranking = true;
	}

	if ( send_packet.ranking_type < eSUN_RANKING_TYPE_END )
	{
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
		Is_wait_receive_packet_ = true;
	}
}

void uiSUNRankingMan::Send_CG_ETC_SUN_RANKING_SETTLE_INFO_SYN()
{
	MSG_CG_ETC_SUN_RANKING_SETTLE_INFO_SYN send_packet;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
}

void uiSUNRankingMan::NetworkProc( MSG_BASE* message )
{
	switch ( message->m_byCategory )
	{
	case CG_ETC:
		{
			switch ( message->m_byProtocol )
			{
			case CG_ETC_SUN_RANKING_ACK:
				{
					Recv_CG_ETC_SUN_RANKING_ACK(message);
				}
				break;
			case CG_ETC_SUN_RANKING_NAK:
				{
					Recv_CG_ETC_SUN_RANKING_NAK(message);
				}
				break;
			case CG_ETC_SUN_RANKING_SETTLE_INFO_ACK:
				{
					Recv_CG_ETC_SUN_RANKING_SETTLE_INFO_ACK(message);
				}
				break;
			}
		}
		break;
	}
}

void uiSUNRankingMan::Recv_CG_ETC_SUN_RANKING_ACK( MSG_BASE* message )
{
	MSG_CG_ETC_SUN_RANKING_ACK* recv_packet = static_cast<MSG_CG_ETC_SUN_RANKING_ACK*>(message);

	sun_ranking_dialog_->UpdateSlotList(recv_packet->infos, recv_packet->my_ranking, recv_packet->my_score);
	Is_wait_receive_packet_ = false;
}

void uiSUNRankingMan::Recv_CG_ETC_SUN_RANKING_NAK( MSG_BASE* message )
{
	MSG_CG_ETC_SUN_RANKING_NAK* recv_packet = static_cast<MSG_CG_ETC_SUN_RANKING_NAK*>(message);

	sun_ranking_dialog_->ResultMessageProc((RC::eSUN_RANKING_RESULT)recv_packet->error_code);
	Is_wait_receive_packet_ = false;
}

void uiSUNRankingMan::Recv_CG_ETC_SUN_RANKING_SETTLE_INFO_ACK( MSG_BASE* message )
{
	MSG_CG_ETC_SUN_RANKING_SETTLE_INFO_ACK* recv_packet = static_cast<MSG_CG_ETC_SUN_RANKING_SETTLE_INFO_ACK*>(message);
	SETTLE_DATE settle_date;
	settle_date.type = recv_packet->type;
	settle_date.day  = recv_packet->day;
	settle_date.time = recv_packet->time;
	settle_date.is_initialize = true;
	if ( sun_ranking_dialog_ )
	{
		sun_ranking_dialog_->set_settle_date_(settle_date);
	}
}

void uiSUNRankingMan::HotKeyToggleSUNRankingDialog()
{
	uiSUNRankingMan* ui_sun_ranking_manager = GET_DIALOG_MANAGER(uiSUNRankingMan, UIMAN_SUNRANKING);
	if ( ui_sun_ranking_manager )
	{
		ui_sun_ranking_manager->ToggleSUNRankingDialog();
	}
}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
void uiSUNRankingMan::AddStatueInfo( const eSTATUE_INDEX& statue_index, const StatueInfo& statue_info )
{
	// 나중에 여러개가 올 수 있다. 하지만 지금은 일단 하나만 담아두자.
	statue_info_map_.clear();
	statue_info_map_.insert(make_pair(statue_index, statue_info));
}

bool uiSUNRankingMan::IsStatueInfo( const eSTATUE_INDEX& statue_index )
{
	std::map<eSTATUE_INDEX, StatueInfo>::iterator iter;
	iter = statue_info_map_.find(statue_index);
	if ( iter == statue_info_map_.end() )
	{
		return false;
	}

	return true;
}

void uiSUNRankingMan::GetStatueInfo( const eSTATUE_INDEX& statue_index, OUT StatueInfo& statue_info )
{
	std::map<eSTATUE_INDEX, StatueInfo>::iterator iter;
	iter = statue_info_map_.find(statue_index);
	if ( iter == statue_info_map_.end() )
	{
		// no operation
	}
	else
	{
		statue_info = iter->second;
	}
}

#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
