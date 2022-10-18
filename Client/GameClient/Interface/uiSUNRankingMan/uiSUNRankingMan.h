#pragma once

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM

#include "uiBaseMan.h"

enum SUN_RANKING_MODE
{
	SUN_RANKING_BEST_OF_BEST = 0,
	//SUN_RANKING_BEST_OF_BEST_GUILD,
	SUN_RANKING_ELETE_WARRIOR = SUN_RANKING_BEST_OF_BEST + 2,
	//SUN_RANKING_ELETE_WARRIOR_GUILD,
	SUN_RANKING_LEVEL_KING = SUN_RANKING_ELETE_WARRIOR + 2,
	//SUN_RANKING_LEVEL_KING_GUILD,
	SUN_RANKING_LEGEND_EQUIP = SUN_RANKING_LEVEL_KING + 2,
	//SUN_RANKING_LEGEND_EQUIP_GUILD,
	SUN_RANKING_STRONG_ONE_SHOT = SUN_RANKING_LEGEND_EQUIP + 2,
	//SUN_RANKING_STRONG_ONE_SHOT_GUILD,
	SUN_RANKING_BEST_ATTACKKER = SUN_RANKING_STRONG_ONE_SHOT + 2,
	//SUN_RANKING_BEST_ATTACKKER_GUILD,
	SUN_RANKING_MASTER_OF_HEAL = SUN_RANKING_BEST_ATTACKKER + 2,
	//SUN_RANKING_MASTER_OF_HEAL_GUILD,
	SUN_RANKING_MAX = SUN_RANKING_MASTER_OF_HEAL + 2,
};

enum
{
	PRIVATE_MODE = 0,
	GUILD_MODE,
	MODE_MAX,
};

struct IM_SUNRANKING_MANAGER
{
	static const DWORD SUNRANKING_DIALOG;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	static const DWORD SUNRANKING_STATUE_INFO;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
};

class uiSUNRankingDialog;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
class uiSUNRankingStatueInfo;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
class uiSUNRankingMan : public uiBaseMan
{
	enum PacketPending
	{
		PKBOOL_RANKING_STATUS_BASE = 0,
		PKBOOL_RANKING_SENDING_SYN,
		PKBOOL_RANKING_MAX,
	};

public:
	uiSUNRankingMan(InterfaceManager* ui_manager);
	~uiSUNRankingMan(void);

public:
	virtual void OnInitialize();
	virtual void InitPKBooleans();

public:
	bool get_Is_wait_receive_packet_() const { return Is_wait_receive_packet_; }
	void set_Is_wait_receive_packet_(const bool& Is_wait_receive_packet) { Is_wait_receive_packet_ = Is_wait_receive_packet; }

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
public:
	uiSUNRankingStatueInfo* get_sun_ranking_statue_info_() { return sun_ranking_statue_info_; }
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

public:
	void Recv_CG_ETC_SUN_RANKING_ACK( MSG_BASE* message );
	void Recv_CG_ETC_SUN_RANKING_NAK( MSG_BASE* message );
	void Recv_CG_ETC_SUN_RANKING_SETTLE_INFO_ACK( MSG_BASE* message );

public:
	void Send_CG_ETC_SUN_RANKING_SYN(const int& mode);
	void Send_CG_ETC_SUN_RANKING_SETTLE_INFO_SYN();

public:
	virtual void          NetworkProc( MSG_BASE* message );

public:
	void ToggleSUNRankingDialog();
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	void ShowSUNRankingStatueInfo(const DWORD& statue_player_key);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	void AddStatueInfo(const eSTATUE_INDEX& statue_index, const StatueInfo& statue_info);
	bool IsStatueInfo(const eSTATUE_INDEX& statue_index); // index에 해당하는 동상 정보가 있는지 알 수 있도록 한다.
	void GetStatueInfo(const eSTATUE_INDEX& statue_index, OUT StatueInfo& statue_info);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

	static void HotKeyToggleSUNRankingDialog();

private:
	uiSUNRankingDialog* sun_ranking_dialog_;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	uiSUNRankingStatueInfo* sun_ranking_statue_info_;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	bool                Is_wait_receive_packet_;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	std::map<eSTATUE_INDEX, StatueInfo> statue_info_map_; 
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
};

#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
