#pragma once

#include <RankingManager.h>
#include <PacketStruct_GP.h>

class PortalRankingManager :
	public RankingManager
{
public:
	PortalRankingManager(void);
	virtual ~PortalRankingManager(void);

	virtual void Initialize();

protected:
	// ��� ���� Ŭ�������� �����ؾ� �ϴ°�
	virtual void OnChangedStatus();
	virtual void OnSettled();

private:
	BOOL m_bInitialized;
	MSG_GP_RANKING_SETTLED_INFO_CMD m_msg;
};

extern PortalRankingManager g_RankingMgr;