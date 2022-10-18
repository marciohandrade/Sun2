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
    // CG_CHAO_NOTIFIY_CHAO_TIME_CMD ��Ŷ������� ���̾�α� �����ϴµ� 
    // ���� �ʱ׷������� �׸������Ѵ�(g_pMap->IsRender())
    if (g_pMap)
    {
       m_pChaoSystemDialog->ShowWindow(g_pMap->IsRender());
    } 
	m_pChaoSystemDialog->SetTime(dwTime);
}

void uiChaoSystemMan::Process(DWORD dwTick)
{
    //�ε��Ҷ��� �������ش�
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
        //�ܺο��� ���̾�α׸� ������ �ݾҴ��� �ð��� �����ϸ� �����ؾ��Ѵ�
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

