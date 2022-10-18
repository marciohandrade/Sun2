#pragma once

//=============================================================================================================================
/// GuildMember class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2005. 12. 26
	@remark
		- ����� �������� ��Ÿ���� Ŭ����
	@note
		- ���� ���������� �ǽð� ó���� �Ѵ�.
		- OnXXX �迭�� �Լ��� DBR�� ������� �� �� ȣ��Ǵ� �ݺ���Ÿ���� �Լ��̴�.
		- 
	@history 
		- 
*/
//=============================================================================================================================

#include "GuildCommon.h"

enum eGUILD_MEMBER_STATE
{
	eGUILD_MEMBER_STATE_READY,
	eGUILD_MEMBER_STATE_START,
	eGUILD_MEMBER_STATE_END,
};

struct BASE_GUILD_MEMBER_INFO;
struct GUILD_MEMBER_INFO;

class GuildMember
{
	__DECL_GUILDPOOL(GuildMember)
	friend class Guild;
	friend class GuildManager;
public:
	GuildMember(void);
	virtual ~GuildMember(void);

	// Ǯ���� �����ͼ� �ʱ�ȭ
	VOID InitializeMember();
	VOID ReleaseMember();
	VOID Join(CHARGUID guid, const TCHAR * tszCharName, BYTE byClass, LEVELTYPE CharLV);

public:

    const TCHAR* GetCharName() const { return m_MemberInfo.m_tszCharName; }
	BYTE GetCorpsNumber() const { return m_MemberInfo.m_byCorpsNum; }
	CHARGUID GetCharGuid() const { return m_MemberInfo.m_CharGuid; }
	eGUILD_DUTY GetDuty() const { return m_MemberInfo.m_eGuildDuty; }
	BYTE GetCharState() const { return m_MemberInfo.m_byChaoState; }
	BYTE GetTotalInfoReqCnt() const { return m_byTotalInfoReqCnt; }
	LEVELTYPE GetLevel() const { return m_MemberInfo.m_Level; }
	BYTE GetClass() const { return m_MemberInfo.m_byClass; }
	BOOL IsLogIn() const { return m_MemberInfo.m_bLogin; }
	
	VOID SetCorpsNumber(BYTE byCorpsNum) { m_MemberInfo.m_byCorpsNum = byCorpsNum; }
	VOID SetChaoState(BYTE byChaoState) { m_MemberInfo.m_byChaoState = byChaoState; }
	VOID SetDuty(eGUILD_DUTY eDuty) { m_MemberInfo.m_eGuildDuty = eDuty; }
	VOID SetCharGuid(CHARGUID guid) { m_MemberInfo.m_CharGuid = guid; }
	VOID SetTotalInfoReqCnt(BYTE byCnt) { m_byTotalInfoReqCnt = byCnt; }
	VOID GetDelegateRight(OUT GUILD_RIGHT_USE_INFO* pDelegateRight);
	VOID SetClass(BYTE byClass) { m_MemberInfo.m_byClass = byClass; }
	VOID SetLevel(LEVELTYPE level) { m_MemberInfo.m_Level = level; }
	
	BOOL CheckDelegateRight(eGUILD_RIGHTS Right);
	VOID SerializeServerMemberInfo(INOUT SERVER_GUILD_MEMBER_INFO& rInfo, eSERIALIZE se);
	VOID SerializeBaseMemberInfo(INOUT BASE_GUILD_MEMBER_INFO& rInfo, eSERIALIZE se);
	VOID SerializeMemberInfo(INOUT GUILD_MEMBER_INFO& rInfo, eSERIALIZE se);
	VOID SetDelegateRight(GUILD_RIGHT_USE_INFO* pDelegateRight);
	VOID SetCharName(const TCHAR* pszCharName);
	VOID SetLogin(BOOL bLogin);
	VOID InitDelegateRight();

private:
	SERVER_GUILD_MEMBER_INFO m_MemberInfo;
	BYTE m_byTotalInfoReqCnt;
};
