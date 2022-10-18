#include "SunClientPrecompiledHeader.h"
#include "uitargetman.h"
#include "interfacemanager.h"
#include "TargetDialog.h"
#include "uiTargetZoneMarkDialog/uiTargetZoneMarkDialog.h"
#include "uiTargetAggro/uiTargetAggro.h"

const DWORD IM_TARGET_MANAGER::TARGET_DIALOG = StrToWzID("0270");
// 97_29_zone_mark.iwz UI�� Ŭ���Ͽ� ��������� ID�� Ŭ�󿡼� "0272"�ο��Ͽ� �����
// ���� Ÿ�ٰ��� UI������ �߰��� �� ID "0272"�� ����ϸ� �ȵ�
const DWORD IM_TARGET_MANAGER::TARGET_ZONEMARK_DIALOG = StrToWzID("0272");

const DWORD IM_TARGET_MANAGER::TARGET_AGGRO = StrToWzID("0273");

uiTargetMan::uiTargetMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    target_dialog_	= NULL;
    target_zonemark_dialog_ = NULL;
    target_aggro_dialog_ = NULL;

}

//------------------------------------------------------------------------------
/**
*/
void	uiTargetMan::OnInitialize()
{
	target_dialog_ = CreateUIDialog<TargetDialog>(IM_TARGET_MANAGER::TARGET_DIALOG, 
					                             "Data\\Interface\\27_target_window.iwz", this, TRUE);
    target_zonemark_dialog_ = CreateUIDialog<uiTargetZoneMarkDialog>(IM_TARGET_MANAGER::TARGET_ZONEMARK_DIALOG, 
                                                          "Data\\Interface\\97_29_zone_mark.iwz", this, TRUE, TRUE);

    target_aggro_dialog_ = CreateUIDialog<uiTargetAggro>(IM_TARGET_MANAGER::TARGET_AGGRO,
        "Data\\Interface\\27_3_aggressive_list.iwz", this, TRUE);

    assert(target_dialog_);
    assert(target_zonemark_dialog_);
    assert(target_aggro_dialog_);
}
