#include "SunClientPrecompiledHeader.h"
#include "uichaosystemman.h"
#include "interfacemanager.h"
#include "ChaoSystemDialog.h"
#include "Hero.h"
#include "Map.h"
#include "GameFramework.h"
#include "SceneBase.h"

uiChaoSystemMan::uiChaoSystemMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)

{
    m_pChaoSystemDialog = NULL;
}

DWORD uiChaoSystemMan::GetTime()
{
	return m_pChaoSystemDialog->GetTime();
}

void uiChaoSystemMan::SetTime(DWORD dwTime)
{
    // CG_CHAO_NOTIFIY_CHAO_TIME_CMD 패킷왔을경우 다이얼로그 오픈하는데 
    // 현재 맵그려질때만 그리도록한다(g_pMap->IsRender())
    if (g_pMap)
    {
       m_pChaoSystemDialog->ShowWindow(g_pMap->IsRender());
    } 
	m_pChaoSystemDialog->SetTime(dwTime);
}

void uiChaoSystemMan::Process(DWORD dwTick)
{
    //로딩할때는 리셋해준다
    if (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_NONE)
    {
        if (m_pChaoSystemDialog->IsVisibleWindow())
        {
            m_pChaoSystemDialog->ShowWindow(FALSE);
        }
        
        m_pChaoSystemDialog->SetTime(0);

        return;
    }

	if (m_pChaoSystemDialog->IsVisibleWindow())
	{
		if (m_pChaoSystemDialog->GetTime() == 0)
        {
			m_pChaoSystemDialog->ShowWindow(FALSE);
        }
	}
    else
    {
        //외부에서 다이얼로그를 강제로 닫았더라도 시간이 존재하면 오픈해야한다
        if (g_pMap && 
            g_pMap->IsRender())
        {
            if (m_pChaoSystemDialog->GetTime() > 0)
            {
                m_pChaoSystemDialog->ShowWindow(TRUE);
            }
        }
    }
}


void uiChaoSystemMan::OnInitialize()
{
    m_pChaoSystemDialog = new ChaoSystemDialog;
    if( !g_InterfaceManager.LoadInterface( 
        m_pChaoSystemDialog, 
        "Data\\Interface\\64_1_PK_Clock.iwz", 
        StrToWzID("0641"), 
        FALSE ) )
    {

    }

	m_pChaoSystemDialog->SetDialogKey(StrToWzID("0641"));

    m_pChaoSystemDialog->ShowWindow(FALSE);
}

