#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiSystemMan/uiSystemMessage.h"

struct BASE_GuildMarkInfo;

namespace GuildSystem
{
    namespace Function
    {
        // common functions
        bool IsGuildMember(const TCHAR* guild_name);

        void ShowGuildMenu();
        void ShowGuildRanking();

        void ShowNoticeRegisterGuildMark();

        void ShowConfirmDeleteGuildMark();

        void ShowConfirmKickGuildMember();
        void ShowConfirmWithdrawGuild();
        void ShowConfirmDelegateMaster();

        void RenderGuildMark(DWORD mark_info_index, RECT& draw_rect);

        void RenderGuildMark(BASE_GuildMarkInfo* mark_info, RECT& draw_rect);

        void ShowNoticeUseGuildWareHouse();

        eGUILD_DELCHK GetGuildDestroyFlag();

        void ToggleGuildInfoDialog();

        void CloseGuildWareHouse();

        void CloseAllGuildDialog();

        void ClearPlayerRelation();

        void InvitePlayer(const TCHAR* player_name);

        void InviteTargetPlayer();

        void SetPlayerGuildRelation(GUILDGUID guild_guid, eGUILD_RELATION guild_relation);

        void ClearGuildRelation();

        void ProcessErrorCode(DWORD error_code, bool is_popup = true);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        void ProcessGuildFacilityResultCode(DWORD result_code, bool is_popup = true);
        void Process_Guild_Join_Request(const eGUILD_JOIN_REQUEST& request_type, const GUILD_JOIN_REQUEST_INFO& request_info);
        void Delete_Element_Guild_Join_Request_List(const CHARGUID& char_guid);
        void UpdateAllGuildInstitution(); // 길드 시설 업데이트
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        DWORD GetGuildDutyStringCode(eGUILD_DUTY guild_duty);
        DWORD GetInstitutionNameCode(int Institution_index);

        // callback functions
        // 길드 초대/탈퇴/제명
        void Callback_GuildInvite(uiSystemMessage::ButtonIndex button_index);
        void Callback_KickGuildMember(uiSystemMessage::ButtonIndex button_index);
        void Callback_WithdrawGuild(uiSystemMessage::ButtonIndex button_index);
        void Callback_DelegateMaster(uiSystemMessage::ButtonIndex button_index);

        // 길드관계
        void Callback_GuildRelationAnswerAllince(uiSystemMessage::ButtonIndex button_index);
        void Callback_GuildRelationAnswerHostility(uiSystemMessage::ButtonIndex button_index);

        // 길드 마크 등록
        void Callback_RegisterGuildMark(uiSystemMessage::ButtonIndex button_index);
        void Callback_DeleteGuildMark(uiSystemMessage::ButtonIndex button_index);

        // 창고이용
        void Callback_UseGuildWareHouse(uiSystemMessage::ButtonIndex button_index);
        
        // 길드 창설
        void Callback_GuildCreate(uiSystemMessage::ButtonIndex button_index);
        
        // 해산 신청
        void Callback_GuildDestroy(uiSystemMessage::ButtonIndex button_index);
        
        // 해산 대기 해제
        void Callback_GuildDestroyWait(uiSystemMessage::ButtonIndex button_index);

        // 길드명 변경
        void Callback_GuildRename(uiSystemMessage::ButtonIndex button_index);

        // 글쓰기 취소(공지,길드소개)
        void Callback_GuildNoticeCancle(uiSystemMessage::ButtonIndex button_index);

        // 글쓰기 삭제(공지,길드소개)
        void Callback_GuildNoticeDelete(uiSystemMessage::ButtonIndex button_index);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        // 길드가입 요청
        void Callback_GuildJoinRequestSend(uiSystemMessage::ButtonIndex button_index);
        // 길드시설 업그레이드 요청
        void Callback_GuildFacilityUpgrade(uiSystemMessage::ButtonIndex button_index);
        // 길드시설 활성화 요청
        void Callback_GuildFacilityActive(uiSystemMessage::ButtonIndex button_index);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        
    }; //namespace Function

}; //namespace GuildSystem

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL