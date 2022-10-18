#pragma once
#ifndef DBPROXY_GUILD_MANAGER_H
#define DBPROXY_GUILD_MANAGER_H

/*=============================================================================================================================
GuildManager class

	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2005. 12. 26
	@remark
		- 길드,길드원을 관리하는 클래스
	@note
		- 길드는 실시간 처리를 한다.
		- Agent와 Game서버와의 처리를 하는 인터페이스 창구이다.
		- GuildManager는 Guild를 Guild는 GuildMember를 관리한다.
		- 먼저 생성후 DBProxy로 부터 결과가 오면 길드 정보를 처리한다. ( 선생성 후처리 )
		[길드원정보의 처리 방법]
		- 길드원이 로그오프시 길드에서 Remove하지 않는다!
		- DB로 부터 SelectMember해서 받은 길드원 정보를 다 가지고 있는다.
	@history 
		- 
=============================================================================================================================*/

#include "GuildCommon.h"

class Guild;
class GuildMember;

struct BASE_GUILD_INFO;
struct BASE_GUILD_MEMBER_INFO;

#include <GuildRankInfoParser.h>
#include "GuildSmallContentEntry.h"

class GuildManager
{
public:
	GuildManager(void);
	~GuildManager(void);

#ifdef _NA_008334_20150608_SONNENSCHEIN
    VOID InitGuildManager();
#endif //_NA_008334_20150608_SONNENSCHEIN
    void Release();
    bool Update();  
    void StartGuildDestroyTimer();
    void StopGuildDestroyTimer();
    void DisplayGuildInfo();
    void UpdateForGuildDestroy();
    GuildSmallContentEntry::GuildPenalty::GuildPenaltyMgr* GetGuildPenaltyMgr();

	void RankUpGuild( 
        GUILDGUID guild_guid 
        );

    RC::eGUILD_RESULT CanRankUp( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        GuildRankInfo& guild_rank_info 
        );

    void SetGuildTransaction( 
        GUILDGUID guild_guid, 
        eGUILD_TRANSACTION tran 
        );

	bool IsGuildTransaction( 
        GUILDGUID guild_guid, 
        eGUILD_TRANSACTION trans = GUILD_TRANS_ING  
        );

	void OnChangeNoticeGuild( 
        bool is_success, 
        GUILDGUID guild_guid, 
        TCHAR * OUT changed_notice 
        );

	RC::eGUILD_RESULT ChangeChaoState( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid, 
        BYTE chao_state 
        );

	Guild* FindGuild(
        GUILDGUID guild_guid 
        );
	
	Guild* FindGuildName(
        std::string
        );

	void RemoveGuildName(
        std::string guild_name
        );

	void AddGuildName(
        std::string guild_name, 
        GUILDGUID guild_guid
        );

	GUILDGUID GetGuildGuidByGuildName(
        std::string guild_name
        );

	CHARGUID GetGuildMasterByGuildName(
        std::string guild_name
        );

	RC::eGUILD_RESULT CanChangeDuty( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        CHARGUID target_char_guid, 
        eGUILD_DUTY change_duty 
        );

	bool ChangeMemberDuty( 
        GUILDGUID guild_guid, 
        CHARGUID target_char_guid, 
        eGUILD_DUTY change_duty
        );

	GuildMember* FindGuildMemberByName( 
        GUILDGUID guild_guid, 
        TCHAR* member_name 
        );

	GuildMember* FindGuildMember( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid 
        );

	bool CheckRight( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        eGUILD_RIGHTS right 
        );

	void GetCharName( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid, 
        OUT TCHAR* char_name, 
        IN BYTE buf_size 
        );

	RC::eGUILD_RESULT IsDelegateChar( 
        GUILDGUID guild_guid, 
        CHARGUID target_char_guid 
        );

	RC::eGUILD_RESULT IsDelegateChar( 
        GUILDGUID guild_guid, 
        TCHAR* target_char_name 
        );

	void GetDelegateRight( 
        GUILDGUID guild_guid, 
        TCHAR* target_char_name, 
        OUT GUILD_RIGHT_USE_INFO* delegate_right
        );

	bool IsDelegateRight( 
        const GUILD_RIGHT_USE_INFO* right_use_info 
        );

	void ChangeDelegateRight( 
        GUILDGUID guild_guid, 
        CHARGUID target_char_guid, 
        GUILD_RIGHT_USE_INFO* delegate_right 
        );

	bool ChangeMaster( 
        GUILDGUID guild_guid, 
        CHARGUID old_master_char_guid, 
        CHARGUID new_master_char_guid, 
        INT64 master_change_time 
        );

	RC::eGUILD_RESULT CanChangeMaster( 
        GUILDGUID guild_guid, 
        CHARGUID old_master_guid , 
        TCHAR* new_master_name 
        );

	BYTE GetMaxMemberByDuty( 
        BYTE corps_num, 
        eGUILD_DUTY duty 
        );

	RC::eGUILD_RESULT CanRightDelegate( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        TCHAR* target_char_name, 
        const GUILD_RIGHT_USE_INFO* pRightInfo 
        );

	CHARGUID FindRightMember(
        GUILDGUID guild_guid, 
        eGUILD_DUTY eDuty
        );

	RC::eGUILD_RESULT CanCreateGuild(
        LEVELTYPE master_level
        );

	RC::eGUILD_RESULT CreateGuild( 
        GUILDGUID guild_guid, 
        const TCHAR* guid_name, 
        BYTE corps_num, 
        CHARGUID master_char_guid, 
        const TCHAR* master_char_name, 
        eGUILD_DUTY master_duty, 
		BYTE master_class, 
        LEVELTYPE master_level, 
        BYTE master_chao_state, 
        INT64 guild_panalty_time, 
        INT64 destroy_ruquest_time, 
        INT64 master_change_time
        );

	void DestroyGuild( 
        GUILDGUID guild_guid 
        );

	RC::eGUILD_RESULT CanDestroyGuild( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid 
        );

	GuildMember* FindMaster( 
        GUILDGUID guild_guid 
        );
    
	RC::eGUILD_RESULT CreateCorps( 
        GUILDGUID guild_guid, 
        BYTE corps_num, 
        CHARGUID target_char_guid 
        );

	BYTE GetEmptyCommanderInfo( 
        GUILDGUID guild_guid, 
        BYTE* empty_commander_info
        );

	RC::eGUILD_RESULT CanDestoryCorps( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        BYTE corps_num 
        );

	RC::eGUILD_RESULT DestoryCorps( 
        GUILDGUID guild_guid, 
        BYTE corps_num, 
        CHARGUID ruquest_char_guid 
        );

	RC::eGUILD_RESULT CanChangeCommander( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        const TCHAR* master_name, 
        BYTE corps_num 
        );

	RC::eGUILD_RESULT ChangeCommander( 
        GUILDGUID guild_guid, 
        BYTE corps_num, 
        CHARGUID commander_guid
        );

	RC::eGUILD_RESULT CanChangeCorps( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        const TCHAR* master_name, 
        BYTE corps_num 
        );

	RC::eGUILD_RESULT ChangeCorps( 
        GUILDGUID guild_guid, 
        CHARGUID target_char_guid, 
        BYTE corps_num 
        );

	RC::eGUILD_RESULT CanCreateRearCamp( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        const TCHAR* target_char_name, 
        OUT BYTE& corps_num 
        );

	RC::eGUILD_RESULT CreateRearCamp( 
        GUILDGUID guild_guid, 
        BYTE corps_num, 
        CHARGUID target_char_guid 
        );

	RC::eGUILD_RESULT CanDestroyRearCamp( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        BYTE corps_num 
        );

	RC::eGUILD_RESULT DestroyRearCamp( 
        GUILDGUID guild_guid, 
        BYTE corps_num, 
        CHARGUID request_char_guid 
        );

	RC::eGUILD_RESULT CanInviteGuild( 
        const GUILDGUID guild_guid, 
        const CHARGUID request_char_guid, 
        const CHARGUID target_char_guid, 
        const LEVELTYPE target_level 
        );

	RC::eGUILD_RESULT CanJoinGuild( 
        const GUILDGUID& guild_guid, 
        const CHARGUID& request_char_guid, 
        const CHARGUID& target_char_guid, 
        const LEVELTYPE& target_level, 
        OUT eGUILD_DUTY& duty, 
        OUT BYTE& corps_num 
        );

	RC::eGUILD_RESULT JoinGuild( 
        GUILDGUID guild_guid, 
        BYTE corps_num, 
        CHARGUID char_guid, 
        const TCHAR* char_name, 
        eGUILD_DUTY duty, 
        BYTE join_char_class, 
        LEVELTYPE level, 
        BYTE chao_state 
        );

	RC::eGUILD_RESULT CanLeaveGuild( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid 
        );

	RC::eGUILD_RESULT LeaveGuild( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid 
        );
	
	RC::eGUILD_RESULT CanKickMember(
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        TCHAR* char_name 
        );

	RC::eGUILD_RESULT KickMember( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        CHARGUID target_char_guid 
        );

	bool SerializeBaseGuildInfo( 
        GUILDGUID guild_guid, 
        INOUT BASE_GUILD_INFO& info, 
        eSERIALIZE se 
        );

	bool SerializeGameGuildInfo( 
        GUILDGUID guild_guid, 
        INOUT GAME_GUILD_INFO& info, 
        eSERIALIZE se 
        );

	bool SerializeGuildInfo( 
        GUILDGUID guild_guid, 
        INOUT GUILD_INFO& info, 
        eSERIALIZE se 
        );

	bool SerializeCorpsInfo( 
        GUILDGUID guild_guid, 
        INOUT GUILD_CORPS_TOTAL_INFO& info, 
        eSERIALIZE se 
        );

	bool SerializeBaseMemberTotalInfo(
        GUILDGUID guild_guid,
        INOUT BASE_GUILD_MEMBER_TOTAL_INFO& info, 
        eSERIALIZE se 
        );

	bool SerializeMemberTotalInfo( 
        GUILDGUID guild_guid, 
        INOUT GUILD_MEMBER_TOTAL_INFO& info, 
        eSERIALIZE se 
        );

	bool SerializeServerMemberTotalInfo( 
        GUILDGUID guild_guid, 
        INOUT SERVER_GUILD_MEMBER_TOTAL_INFO& info, 
        eSERIALIZE se 
        );

	bool GetGuildBaseMemberInfo( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid, 
        OUT BASE_GUILD_MEMBER_INFO& info 
        );

	bool GetGuildMemberInfo( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid,
        OUT GUILD_MEMBER_INFO& info 
        );

	bool GetWorldGuildInfo( 
        GUILDGUID guild_guid,
        OUT WORLD_GUILD_INFO& info
        );

	RC::eGUILD_RESULT CanLogin( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid
        );

	RC::eGUILD_RESULT Login( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid 
        );

	RC::eGUILD_RESULT Logout( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid 
        );

	RC::eGUILD_RESULT CanMoveServer( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid 
        );

	RC::eGUILD_RESULT CanConvertUP2GP( 
        GUILDGUID guild_guid,
        CHARGUID request_char_guid, 
        GPTYPE ConverGP 
        );

	bool SetPoint( 
        GUILDGUID guild_guid, 
        GPTYPE gp, 
        UPTYPE up 
        );

	RC::eGUILD_RESULT CanDonateUP( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid 
        );

	RC::eGUILD_RESULT DonateUP( 
        GUILDGUID guild_guid, 
        UPTYPE total_up, 
        CHARGUID char_guid, 
        UPTYPE donate_up 
        );

	RC::eGUILD_RESULT LevelUpMember( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid, 
        LEVELTYPE Level 
        );

	RC::eGUILD_RESULT CanRequestTotalInfo( 
        GUILDGUID guild_guid, 
        CHARGUID char_guid 
        );

	void SetGuildNotice( 
        GUILDGUID guild_guid, 
        const util::StrArrayForGuildNotice& guild_notice 
        );

	void ChangeGuildName( 
        GUILDGUID guid, 
        const TCHAR* guild_name
        );

	RC::eGUILD_RESULT CanChangeGuildName( 
        GUILDGUID guid, 
        CHARGUID request_char_guid = 0 
        );

    bool CheckDelegatedRight( 
        const eGUILD_DUTY& duty 
        );

    RC::eGUILD_RESULT CanCreateCorps( 
        GUILDGUID guild_guid, 
        CHARGUID request_char_guid, 
        const TCHAR* target_char_name, 
        OUT BYTE& corps_num 
        );

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    RC::eGUILD_RESULT	ChuntingPVPResult(
        GUILDGUID winguild_guid, 
        GUILDGUID loseguild_guid, 
        GUILDGUID disguild_guid
        );
#endif

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    void UpdateGuildRelation();
#endif
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    void ProcessGuildCoinChange(GUILDGUID guid, CHARGUID uuid, DWORD code, INT point);
    void ProcessGuildRankingUpdate(GUILD_RANKING_INFO* guild_ranking_info_);
    WORD GetGuildRanking(GUILDGUID guid) const;
    bool GetGuildInfoExtra(GUILDGUID guid, GUILD_INFO_EXTRA& guild_info_extra);
#endif 
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    // 길드 소개를 쓴다
    void SetGuildIntroduction(GUILDGUID guid, CHAR* introduction);
#endif //_NA_004034_20120102_GUILD_INTRODUCTION
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    // 길드아이디로 캐릭터를 찾는다
    GUILDGUID FindCharGuildGuid(CHARGUID char_guid);
    // 길드 가입 요청을 삽입한다
    const RC::eGUILD_RESULT InsertGuildJoinRequest(GUILD_JOIN_REQUEST_INFO* guild_join_request_info);
    // 길드 가입 요청을 삭제한다
    const RC::eGUILD_RESULT DeleteGuildJoinRequest(const CHARGUID char_guid, const GUILDGUID guild_guid);
    // 캐릭터의 길드 가입 요청 상태를 얻는다
    const RC::eGUILD_RESULT CanGuildJoinRequest(const CHARGUID char_guid, const GUILDGUID guild_guid);
    // 길드 가입 요청 목록을 업데이트 한다
    const void UpdateGuildJoinRequest();
    // 길드의 전체 가입 요청 목록을 얻는다
    const void GetGuildJoinRequestTotalInfo(const GUILDGUID guild_guid, OUT GUILD_JOIN_REQUEST_TOTAL_INFO& total);
    // 캐릭터의 길드 가입 요청 목록을 모두 초기화한다
    const void InitailizeGuildJoinRequest(const CHARGUID char_guid);
    // 캐릭터의 길드 가입 요청 목록 정보를 갱신한다
    const void UpdateGuildJoinRequestCharacter(const CHARGUID char_guid, LEVELTYPE char_level, const TCHAR* char_name = NULL);

    // 길드의 전체 길드 시설 목록을 얻는다
    const void GetGuildFacilityTotalInfo(const GUILDGUID guild_guid, OUT GUILD_FACILITY_TOTAL_INFO& total);
    // 길드 시설이 업데이트 가능한 상태인지 얻는다
    const RC::eGUILD_RESULT CanUpdateGuildFacility(FCODE code, FLEVEL add_level, GUILDGUID guild_guid);
    // 길드 시설을 업데이트 한다
    const RC::eGUILD_RESULT UpdateGuildFacility(GUILD_FACILITY_INFO info, FLEVEL add_level, GUILDGUID guild_guid, CHARGUID char_guid);
    // 임의의 길드 시설을 길드 포인트 만큼 다운 그레이드 한다
    const bool RandomDowngradeGuildFacility(GUILDGUID guild_guid,  IN OUT INT& guild_point);
    // 길드 시설 정보를 얻는다
    const GUILD_FACILITY_INFO* GetGuildFacility(FCODE code, GUILDGUID guild_guid);
    // 길드 시설 정보를 쓴다
    const void SetGuildFacility(GUILD_FACILITY_INFO info, GUILDGUID guild_guid);
    // 길드 시설 레벨을 얻는다
    const FLEVEL GetGuildFacilityLevel(FCODE code, GUILDGUID guild_guid);
    // 길드포인트를 소모시킨다
    const bool ProcessGuildPointConsumption(INT consumption_point, GUILDGUID guild_guid);
    // 길드 시설 및 포인트를 초기화시킨다
    const bool InitializeGuildPointAndFacility(GUILDGUID guild_guid);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
    VOID InitializeAllSonnenScheinPhase();
    BOOL SetGuildSonnenScheinPhase(GUILDGUID guid, BYTE phase);

    VOID OnScriptLoaded();
#endif //_NA_008334_20150608_SONNENSCHEIN
protected:
	void AddGuild(
        Guild * guild, 
        GUILDGUID guild_guid
        );

	void RemoveGuild( 
        GUILDGUID guild_guid 
        );

private:
    enum { _MAX_GUILD_POOL_NUM = 1000, _MAX_GUILD_MEMBER_POOL_NUM = 20000, };

#ifdef _DEBUG
    enum { MAX_GUILDDESTROY_TIME = 1 * 60 * 1000 }; /* 1분 */
#else
    enum { MAX_GUILDDESTROY_TIME = 5 * 60 * 1000 }; /* 5분 */
#endif

    ITimerBase guild_destroy_timer_;
	GUILD_HASH guild_hash_;
	GuildSmallContentEntry::GuildPenalty::GuildPenaltyMgr guild_penalty_mgr_;
	GUILD_NAME_HASH guild_name_hash_;

#ifdef _NA_003923_20120130_GUILD_RENEWAL
#ifdef _DEBUG
    enum { MAX_GUILDRELATION_TIME = 1 * 60 * 1000 }; /* 1분 */
#else
    enum { MAX_GUILDRELATION_TIME = 5 * 60 * 1000 }; /* 5분 */
#endif
    ITimerBase guild_relation_timer_;
#endif

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    typedef STLX_MAP<GUILDGUID, WORD> GUILD_RANKING_MAP;
    GUILD_RANKING_MAP guild_ranking_data_;
#endif

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //특정 캐릭터의 가입요청 목록을 찾기위한 컨테이너
    typedef STLX_MULTIMAP<CHARGUID, GUILDGUID> GUILD_JOIN_REQUEST_CHAR_MAP;
    GUILD_JOIN_REQUEST_CHAR_MAP  guild_join_request_char_list_;
#ifdef _DEBUG
    enum { MAX_GUILD_JOIN_REQUEST_TIME = 30 * 1000 }; /* 30초 */
#else
    enum { MAX_GUILD_JOIN_REQUEST_TIME = 1 * 60 * 1000 }; /* 2분 */
#endif
    ITimerBase guild_join_request_timer_;   //길드 가입 요청 목록을 업데이트 하기위한 타이머
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
    VOID _InitSonnenScheinInfo();
    VOID _UpdateSonnenSchein();
    VOID _CheckLastSonnenScheinDate();

    enum { UPDATE_TIME = 500 };
    util::Timer	sschein_update_timer_;      // 업데이트 주기 조절
    CTime next_sonnenschein_date_;     // 다음 소넨샤인이 시작되는 날짜

    sDOMINATION_EVENT_INFO sschein_event_info_;
    BOOL event_loaded_;
#endif //_NA_008334_20150608_SONNENSCHEIN
};
//===========================================================================================================
inline Guild*		
GuildManager::FindGuild(GUILDGUID guild_guid)		
{ 
    return guild_hash_.GetData(guild_guid); 
}

inline void 
GuildManager::UpdateForGuildDestroy()
{ 
    guild_penalty_mgr_.update(); 
}

inline GuildSmallContentEntry::GuildPenalty::GuildPenaltyMgr* 
GuildManager::GetGuildPenaltyMgr()	
{ 
    return &guild_penalty_mgr_; 
}

inline void 
GuildManager::AddGuild(Guild * pGuild, GUILDGUID guild_guid) 
{ 
    guild_hash_.Add(pGuild, guild_guid); 
}

inline void 
GuildManager::RemoveGuild(GUILDGUID guild_guid)		
{ 
    guild_hash_.Remove(guild_guid); 
}

extern GuildManager g_GuildManager;

#endif //DBPROXY_GUILD_MANAGER_H