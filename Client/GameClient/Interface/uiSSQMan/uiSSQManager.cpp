#include "SunClientPrecompiledHeader.h"
#include "uissqmanager.h"

const DWORD IM_SSQ_MANAGER::SSQ_SYSTEMTIME		=	StrToWzID("0763");			
const DWORD IM_SSQ_MANAGER::SSQ_ENTER_WINDOW	=	StrToWzID("0764");		
const DWORD IM_SSQ_MANAGER::SSQ_REWARD_WINDOW	=	StrToWzID("0765");
const DWORD IM_SSQ_MANAGER::SSQ_DICE_WINDOW		=	StrToWzID("0768");


uiSSQManager::uiSSQManager( InterfaceManager * pUIMAN )
:uiBaseMan( pUIMAN )
{

}

void uiSSQManager::OnInitialize()
{
	CreateUIDialog<uiSSQSystemWindow>(IM_SSQ_MANAGER::SSQ_SYSTEMTIME, "Data\\Interface\\76_3_SSQSystem.iwz", this);
	CreateUIDialog<uiSSQEnterWindow>(IM_SSQ_MANAGER::SSQ_ENTER_WINDOW, "Data\\Interface\\76_4_SSQSystem.iwz", this);
	CreateUIDialog<uiSSQRewardWindow>(IM_SSQ_MANAGER::SSQ_REWARD_WINDOW, "Data\\Interface\\76_5_SSQSystem.iwz", this);
	CreateUIDialog<uiSSQDiceWindow>(IM_SSQ_MANAGER::SSQ_DICE_WINDOW, "Data\\Interface\\76_8_SSQSystem.iwz", this);
}
