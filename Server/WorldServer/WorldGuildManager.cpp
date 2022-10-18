#include "StdAfx.h"
#include "WorldGuildmanager.h"
#include "WorldGuild.h"
#include "WorldGuildMember.h"
#include "WorldGuild.h"
#include "User.h"
#include <PacketStruct_WZ.h>
#include <PacketSTruct_CW.h>
#include <PacketStruct_DW.h>

#include "ServerSession.h"
#include "ServerSessionManager.h"

//WorldGuildManager g_WorldGuildManager;
WorldGuildManager::WorldGuildManager(void)
{
	m_GuildHash.Initialize(MAX_GUILD_POOL_NUM);
}

WorldGuildManager::~WorldGuildManager(void)
{
//	Release();
}

VOID WorldGuildManager::Release()
{
	for( GUILD_HASH::iterator it = m_GuildHash.begin() ; it != m_GuildHash.end() ; ++it )
	{
		(*it)->Destroy();
		WorldGuild::FREE( (*it) );
	}
	m_GuildHash.RemoveAll();
}


// 1. ����ػ��� ��û�� �̰� ��帶���;� �Ѵ� 
// 2. ���� ��峻�� ������ ��帶���� 1����̾�� �Ѵ� 
// 3. ���������� �¸��Ͽ� �������ϰ� ���� ���� ��忩�� �Ѵ�. 
RC::eGUILD_RESULT WorldGuildManager::DestroyGuild( User * pUser )
{
	GUILDGUID GuildGuid = pUser->GetGuildGuid();
	if( 0 == GuildGuid )
		return RC::RC_GUILD_CANNOT_DESTROY_STATE;

	WorldGuild * pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_INVALID_STATE;

	if( 1 != pGuild->GetMemberNum() )
		return RC::RC_GUILD_CANNOT_DESTROY_EXIST_MEMBER_STATE;
/*
	MSG_GZ_GUILD_DESTROY_GUILD_SYN msg;
	msg.m_GuildGuid			= GuildGuid;
	msg.m_MasterCharGuid	= pUser->GetCharGuid();
	pUser->SendToGuildServer( &msg, sizeof(msg) );
*/
	return RC::RC_GUILD_SUCCESS;
}



VOID WorldGuildManager::DestroyGuild( GUILDGUID GuildGuid )
{
	WorldGuild * pGuild = FindGuild( GuildGuid );
	if( !pGuild )
	{
		SUNLOG( eCRITICAL_LOG,"[WorldGuildManager::DestroyGuild] Fail, GuildGuid = %u", GuildGuid);
		return;
	}

	//1. Manager���� ����
	RemoveGuild( pGuild->GetGuildGuid() );
	//2. Member�� Pool���� ����.
	pGuild->Destroy();
	//3. Manager Pool���� ����.
	WorldGuild::FREE( pGuild );
}

VOID WorldGuildManager::WithdrawGuild( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	WorldGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )
	{
		SUNLOG( eCRITICAL_LOG,"[WorldGuildManager::WithdrawGuild] Fail, GuildGuid = %u, CharGuid = %u", GuildGuid, CharGuid );
		return;
	}

	pGuild->Withdraw( CharGuid );
}


VOID WorldGuildManager::DisplayerGuildInfo()
{
	DWORD dwNum = 0;
	for( GUILD_HASH::iterator it = m_GuildHash.begin() ; it != m_GuildHash.end() ; ++it )
	{
		dwNum += (*it)->GetMemberNum();
	}
	DISPMSG( "Guild : %u, GuildMember : %u\n", m_GuildHash.GetDataNum(), dwNum );
}

VOID WorldGuildManager::SendGuildMessage( BYTE* pMsg, WORD wSize )
{
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
VOID WorldGuildManager::SetChuntingPVPInfo( GUILDGUID GuildGuid, CHUNTING_PVP_INFO & IN PVPInfo )
{
	WorldGuild *pGuild = FindGuild( GuildGuid );
	if( !pGuild )	return;

	pGuild->SetChuntingPVPInfo( PVPInfo );
}
#endif

//��� ��å ����
BOOL WorldGuildManager::ChangeMemberDuty(GUILDGUID GuildGuid, CHARGUID TargetCharGuid, eGUILD_DUTY eGuildDuty)
{
	WorldGuild *pGuild = FindGuild(GuildGuid);
	if(!pGuild)
	{
		SUNLOG( eCRITICAL_LOG, "[WorldGuildManager::ChangeMemberDuty] Guild Not Exist, GuildGuid = %d", GuildGuid );
		return FALSE;
	}

	if( !pGuild->ChangeMemberDuty( TargetCharGuid, eGuildDuty ) )
	{
		SUNLOG( eCRITICAL_LOG, "[WorldGuildManager::ChangeMemberDuty] ChangeMemberDuty Fail, CharGuid = %u, Duty = %d", TargetCharGuid, eGuildDuty );
		return FALSE;
	}

	return TRUE;
}

BOOL	WorldGuildManager::SendGuildPacket( GUILDGUID GuildGuid, BYTE* pMsg, WORD wSize )
{
	WorldGuild *pGuild = FindGuild(GuildGuid);
	if(!pGuild)
	{
		SUNLOG( eCRITICAL_LOG, "[WorldGuildManager::SendGuildPacket] Guild Not Exist, GuildGuid = %d", GuildGuid );
		return FALSE;
	}

	pGuild->SendAllMember( pMsg, wSize );
	return TRUE;
}

BOOL	WorldGuildManager::CreateRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum )
{
	WorldGuild* pGuild = FindGuild(GuildGuid);
	if(!pGuild)		return FALSE;

	pGuild->CreateRearCamp( byCorpsNum ); 

	return TRUE;
}

BOOL	WorldGuildManager::DestroyRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum )
{
	WorldGuild* pGuild = FindGuild(GuildGuid);
	if(!pGuild)		return FALSE;

	pGuild->DestroyRearCamp( byCorpsNum ); 

	return TRUE;
}

//�����庯��
VOID		WorldGuildManager::ChangeCommander( GUILDGUID GuildGuid, CHARGUID TargetCharGuid,  BYTE byCorpsNum )
{
	//���Ӽ��������� ������ ���� ���� �ʴ´�. 
	//���� ��� ����� ���ܹ�ȣ�� �ٲ��, ��å�� �ٲ�� �ȴ�.
	WorldGuild* pGuild = FindGuild(GuildGuid);
	if( !pGuild )	return;

	WorldGuildMember* pMember = pGuild->FindMember( TargetCharGuid );
	if( !pMember )	return;

	pMember->SetDuty( eGUILD_DUTY_COMMANDER );
	pMember->SetCorpsNum( byCorpsNum );
}

//�����̵�.
VOID		WorldGuildManager::ChangeCorps( GUILDGUID GuildGuid,  CHARGUID TargetCharGuid, BYTE byCorpsNum )
{
	WorldGuild* pGuild = FindGuild(GuildGuid);
	if( !pGuild )	return;

	WorldGuildMember* pMember = pGuild->FindMember( TargetCharGuid );
	if( !pMember )	return;

	pMember->SetCorpsNum( byCorpsNum );
}

VOID	WorldGuildManager::CreateGuild( const WORLD_GUILD_INFO& rGuildInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo )
{
	if( FindGuild( rGuildInfo.m_GuildGuid ) )
	{
		//���� ��尡 �����ϸ�, ��带 ���� �Ѵ�.
		TCHAR ptszGuildName[MAX_GUILDNAME_LENGTH+1] = {0,};
		_tcsncpy( ptszGuildName, rGuildInfo.m_tszGuildName, MAX_GUILDNAME_LENGTH );
		RemoveGuild( rGuildInfo.m_GuildGuid );
	}

	WorldGuild* pGuild = WorldGuild::ALLOC();
	pGuild->Create( rGuildInfo, rMemberInfo );
	AddGuild( pGuild, pGuild->GetGuildGuid() );
}

VOID WorldGuildManager::LoginGuild( const WORLD_GUILD_INFO& GuildInfo, const BASE_GUILD_MEMBER_INFO& memberInfo )
{
	WorldGuild* pGuild = FindGuild( GuildInfo.m_GuildGuid );

	// ��尡 �����ϸ� ����� �����ϰ�, ��尡 �������� ������ ���� �����Ѵ�.
	if( pGuild )
	{
		pGuild->CreateMember( memberInfo );
	}
	else
	{
		CreateGuild( GuildInfo, memberInfo );
	}
}

RC::eGUILD_RESULT WorldGuildManager::LogoutGuild( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	WorldGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	// ��� ����� �ϳ�?
	WorldGuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// �α� �ƿ� �����Ƿ� ���弭�� ��ü���� �����Ѵ�.
	pGuild->DestroyMember( CharGuid );

	// ������� 0�̸� ��带 �ı��Ѵ�.
	if( pGuild->GetMemberNum() == 0 )
	{
		pGuild->Destroy();
		RemoveGuild( GuildGuid );
		WorldGuild::FREE( pGuild );
	}

	return RC::RC_GUILD_SUCCESS;
}

VOID	WorldGuildManager::GetCharName( GUILDGUID GuildGuid, CHARGUID CharGuid, OUT char* pszCharName, int nSize )
{
	pszCharName[0] = '\0';

	WorldGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )	return;

	WorldGuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )			return;

	strncpy( pszCharName, pMember->GetCharName(), nSize );
}

// ��� ������ �����Ѵ�.
// �� �Լ��� ���� ������ ���弭���� �����ϱ� �� ���弭���� ���� ��� �ܰ迡�� �̷�� ����.
// ���� ���� ���弭���� ������ ������ ���� ���� �����̴�.
VOID	WorldGuildManager::RequestGuildRestore( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	ServerSession* pGuildServer = ServerSessionManager::Instance()->GetGuildServer();
	if( !pGuildServer )
	{
		SUNLOG( eCRITICAL_LOG, "[WorldGuildManager::RequestGuildRestore] GuildServer Not Connect, GuildGuid = %u, CharGuid = %u", GuildGuid, CharGuid );
		return;
	}

	//1. ��尡 ������ ��� ���� ��û�Ѵ�. ��� ���� ��û�ÿ� ĳ���� �������� ���� ��û�Ѵ�.
	WorldGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )
	{
		MSG_WZ_GUILD_RESTORE_SYN	synMsg;
		synMsg.m_GuildGuid = GuildGuid;
		synMsg.m_CharGuid = CharGuid;
		pGuildServer->Send( (BYTE*)&synMsg, sizeof(synMsg) );
		return;
	}

	//3. ��尡 �����ϴµ� ����� ������ ��� ������ ��û�Ѵ�.
	WorldGuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )
	{
		//����� ���� ��û.
		MSG_WZ_GUILD_MEMBER_INFO_REQ_SYN	msg;
		msg.m_GuildGuid = GuildGuid;
		msg.m_CharGuid = CharGuid;
		pGuildServer->Send( (BYTE*)&msg, sizeof(msg) );
		return;
	}
	
}

VOID	WorldGuildManager::GuildRestore( const WORLD_GUILD_INFO& GuildInfo, const BASE_GUILD_MEMBER_INFO& memberInfo )
{
	WorldGuild* pGuild = FindGuild( GuildInfo.m_GuildGuid );

	//1. ������尡 ������ ��� ����� �����Ѵ�.
	if( pGuild )
	{
		pGuild->CreateMember( memberInfo );
	}
	else
	{
		CreateGuild( GuildInfo, memberInfo );
	}
}

VOID	WorldGuildManager::SerializeWorldGuildInfo( GUILDGUID GuildGuid, INOUT WORLD_GUILD_INFO& Info, eSERIALIZE se )
{
	WorldGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )		return;
	
	pGuild->SerializeWorldGuildInfo( Info, se );
}

VOID	WorldGuildManager::SerializeBaseMemberInfo( GUILDGUID GuildGuid, INOUT BASE_GUILD_MEMBER_INFO& Info, eSERIALIZE se )
{
	WorldGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )		return;
	
	pGuild->SerializeBaseMemberInfo( Info, se );
}


//VOID	WorldGuildManager::SerializeBaseMemberTotalInfo( GUILDGUID GuildGuid, INOUT BASE_GUILD_MEMBER_TOTAL_INFO& Info, eSERIALIZE se )
//{
//	WorldGuild* pGuild = FindGuild( GuildGuid );
//	if( !pGuild )		return;

//	pGuild->SerializeBaseMemberTotalInfo( Info, se );
//}

VOID	WorldGuildManager::KickMember(  GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	WorldGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )		return;

	pGuild->DestroyMember( CharGuid );
}

VOID	WorldGuildManager::JoinGuild( GUILDGUID GuildGuid, const BASE_GUILD_MEMBER_INFO& rMemberInfo )
{
	WorldGuild * pGuild = FindGuild( GuildGuid );
	if( !pGuild )		return;

	pGuild->Join(rMemberInfo);
}


RC::eGUILD_RESULT	WorldGuildManager::CanChangeNotice( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* pszNotice, OUT int& nNoticeLen )
{
	nNoticeLen = 0;

	WorldGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	WorldGuildMember* pMember = pGuild->FindMember( OperCharGuid );
	if( !pMember )
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	const SIZE_T noticeLen = strlen( pszNotice );
	if( noticeLen > MAX_GUILDNOTICE_LENGTH )
		return RC::RC_GUILD_STRING_OVERFLOW;

	nNoticeLen = (INT)noticeLen;

	return RC::RC_GUILD_SUCCESS;
}

bool WorldGuildManager::ChangeNotice(
    GUILDGUID guild_guid, const util::StrArrayForGuildNotice& notice)
{
    WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL) {
        return false;
    };
    guild->SetGuildNotice(notice);
    return true;
}


VOID WorldGuildManager::SendDominationHeimReturnMemo(const MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD* msg)
{    
    BYTE data_count = msg->m_DataCount;

    if (data_count > _countof(msg->m_ReturnInfo)) //MAX_DOMINATION_AUCTION
    {
        assert(!"Memo count is over");
        SUNLOG(eCRITICAL_LOG,  __FUNCTION__"Memo count is over : %d", msg->m_MapCode);
        return;
    }

    for (int index = 0; index < data_count; ++index)
    {
        MSG_DW_MEMO_SEND_SYN send_msg;
        send_msg.m_dwKey = 0;    // �ý������κ����� ����
        send_msg.m_SendGuid = 0; // �ý������κ����� ����
        //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
        send_msg.m_typeSystemMemo = NORMAL_MEMO;
        send_msg.m_recvItemCode = 0;
        send_msg.m_amountCost = 0;
        send_msg.m_recvGuid = 0;
        //}
        _tcsncpy(send_msg.m_ptszRecvName, msg->m_ReturnInfo[index].m_tszMaster,
                 _countof(send_msg.m_ptszRecvName));
        send_msg.m_ptszRecvName[_countof(send_msg.m_ptszRecvName) - 1]='\0';

        _sntprintf(send_msg.m_ptszMemo, _countof(send_msg.m_ptszMemo), "%d|%d|%I64d|%s|", 
                   msg->m_ReturnInfo[index].m_CostType, 
                   msg->m_MapCode,
                   msg->m_ReturnInfo[index].m_TenderCost,
                   msg->m_tszGuild );
        send_msg.m_ptszMemo[_countof(send_msg.m_ptszMemo) - 1] = '\0';

        ServerSession* game_DB_proxy = ServerSessionManager::Instance()->GetGameDBProxy();
        if (game_DB_proxy == NULL)
        {
            SUNLOG(eCRITICAL_LOG,  "["__FUNCTION__"]No GameDBProxy");
            return ;
        }
        game_DB_proxy->Send((BYTE*)&send_msg, sizeof(send_msg));
    }
}

BOOL WorldGuildManager::GetValueInMemo(const TCHAR* pszMemo, eSYSTEM_MEMO_TYPE& OUT type,
                                       MAPCODE& OUT mapcode)
{ 
    std::string strMemo;
    strMemo.assign(pszMemo);
    std::string::size_type firstPos;
    std::string::size_type secondPos;

    //ù��° : Ÿ���� ��´�.
    firstPos = 0;
    secondPos = strMemo.find('|');
    if (firstPos != std::string::npos) 
    {
        std::string tempString = strMemo.substr(firstPos, secondPos);
        type =(eSYSTEM_MEMO_TYPE) atoi( tempString.c_str() );
    }
    else
    {
        MessageOut( eCRITICAL_LOG, "["__FUNCTION__"]Not Fount Memo Type" );
        return FALSE;
    }

    //�ι�° : MapCode�� ��´�.
    firstPos = strMemo.find('|');
    secondPos = strMemo.find('|', firstPos+1);
    if( secondPos != std::string::npos )
    {
        std::string tempString = strMemo.substr(firstPos+1, secondPos-1);
        mapcode = atoi( tempString.c_str() );
    }
    else
    {
        MessageOut( eCRITICAL_LOG, "["__FUNCTION__"]Not Fount mapcode" );
        return FALSE;
    }

    return TRUE;
}

