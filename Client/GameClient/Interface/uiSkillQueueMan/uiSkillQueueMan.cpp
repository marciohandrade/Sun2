#include "SunClientPrecompiledHeader.h"
#include "uiskillqueueman.h"
#include "interfaceManager.h"
#include "SkillQueueDialog.h"
#include "Application.h"

const DWORD IM_SKILL_QUEUE_MANAGER::SKILL_QUEUE = StrToWzID("0425");

uiSkillQueueMan::uiSkillQueueMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pSkillQueueDialog = NULL;
}


//------------------------------------------------------------------------------
/**
*/
void	uiSkillQueueMan::OnInitialize()
{
	m_pSkillQueueDialog = CreateUIDialog<SkillQueueDialog>(
							IM_SKILL_QUEUE_MANAGER::SKILL_QUEUE, 
							"Data\\Interface\\42_5_Skill_Queing.iwz", 
							this, TRUE);
	assert(m_pSkillQueueDialog);
}


//------------------------------------------------------------------------------
/**
*/
void uiSkillQueueMan::MoveSkillQueueDialog(int value)
{
    int gap_value = 0;
    if (value == 1)
    {
        gap_value = c_iOneLineYPostionToQuickSlot;
    }
    else// value = 2
    {
        gap_value = c_iTwoLineYPostionToQuickSlot;
    }

    if (m_pSkillQueueDialog != NULL)
    {
        POINT pt = g_pApplication->Get2DModeMapping();
        RECT rcClient;
        POINT my_point;
        m_pSkillQueueDialog->GetDialogWZ()->GetClientRect(&rcClient);
        int left_point = (pt.x / 2) - ((rcClient.right - rcClient.left) / 2);
        my_point.x = left_point;
        my_point.y = pt.y - (rcClient.bottom - rcClient.top + gap_value) - 90;
        // + m_iGapOfDialogYPosition);
        m_pSkillQueueDialog->MoveWindowWZ((float)my_point.x, (float)my_point.y);
    }
}
