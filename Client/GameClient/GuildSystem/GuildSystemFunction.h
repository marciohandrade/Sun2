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
        void UpdateAllGuildInstitution(); // ��� �ü� ������Ʈ
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        DWORD GetGuildDutyStringCode(eGUILD_DUTY guild_duty);
        DWORD GetInstitutionNameCode(int Institution_index);

        // callback functions
        // ��� �ʴ�/Ż��/����
        void Callback_GuildInvite(uiSystemMessage::ButtonIndex button_index);
        void Callback_KickGuildMember(uiSystemMessage::ButtonIndex button_index);
        void Callback_WithdrawGuild(uiSystemMessage::ButtonIndex button_index);
        void Callback_DelegateMaster(uiSystemMessage::ButtonIndex button_index);

        // ������
        void Callback_GuildRelationAnswerAllince(uiSystemMessage::ButtonIndex button_index);
        void Callback_GuildRelationAnswerHostility(uiSystemMessage::ButtonIndex button_index);

        // ��� ��ũ ���
        void Callback_RegisterGuildMark(uiSystemMessage::ButtonIndex button_index);
        void Callback_DeleteGuildMark(uiSystemMessage::ButtonIndex button_index);

        // â���̿�
        void Callback_UseGuildWareHouse(uiSystemMessage::ButtonIndex button_index);
        
        // ��� â��
        void Callback_GuildCreate(uiSystemMessage::ButtonIndex button_index);
        
        // �ػ� ��û
        void Callback_GuildDestroy(uiSystemMessage::ButtonIndex button_index);
        
        // �ػ� ��� ����
        void Callback_GuildDestroyWait(uiSystemMessage::ButtonIndex button_index);

        // ���� ����
        void Callback_GuildRename(uiSystemMessage::ButtonIndex button_index);

        // �۾��� ���(����,���Ұ�)
        void Callback_GuildNoticeCancle(uiSystemMessage::ButtonIndex button_index);

        // �۾��� ����(����,���Ұ�)
        void Callback_GuildNoticeDelete(uiSystemMessage::ButtonIndex button_index);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        // ��尡�� ��û
        void Callback_GuildJoinRequestSend(uiSystemMessage::ButtonIndex button_index);
        // ���ü� ���׷��̵� ��û
        void Callback_GuildFacilityUpgrade(uiSystemMessage::ButtonIndex button_index);
        // ���ü� Ȱ��ȭ ��û
        void Callback_GuildFacilityActive(uiSystemMessage::ButtonIndex button_index);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        
    }; //namespace Function

}; //namespace GuildSystem

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL