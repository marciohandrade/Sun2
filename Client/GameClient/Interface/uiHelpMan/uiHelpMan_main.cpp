#include "SunClientPrecompiledHeader.h"
#include "uiHelpMan.h"
#include "uiHelpMenu/uiHelpMenu.h"
#include "uiHelpWindow/uiHelpWindow.h"
#include "uiHelpAnnounceWindow/uiHelpAnnounceWindow.h"
#include "Hero.h"
#include "ItemInfoParser.h"
#include "HelpInfoParser.h"
#include "uiLoginMan/uiLoginMan.h"
#include "SoundEffect.h"
#include "interfacemanager.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "uiHelpMission/uiHelpMission.h"
#include "GlobalFunc.h"
#include "uiHelpMonsterInfo/uiHelpMonsterInfo.h"

#include "uiHelpMissionTimer/uiHelpMissionTimer.h"

int __stdcall SimpleXorCryption( PBYTE in_data, DWORD in_size, PBYTE *out_data,	DWORD *out_size )
{
	PBYTE tmp = (PBYTE) calloc(1, in_size);

	if ( NULL == tmp )
	{
		_ASSERTE( !"calloc fail" );
		return -1;
	}

	for (DWORD i = 0; i < in_size; ++i)
	{
		tmp[i] = in_data[i] ^ 0x33;
	}

	*out_data = tmp;
	*out_size = in_size;

	return 0;
}

//------------------------------------------------------------------------------
/**
*/
BOOL IsVillageMap()
{
	if(GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_VILLAGE)
		return TRUE;

	if(GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_FIELD)
		return TRUE;

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
uiHelpMan::uiHelpMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_pUIHelpMenu = NULL;
	m_pUIHelpWindow = NULL;

	TCHAR szCurDirectory[512+1] = {0,};
	
	ZeroMemory(szCurDirectory,sizeof(szCurDirectory));
	ZeroMemory(m_szFileName,sizeof(m_szFileName));

	// 2008.03.10 : lammy : GetCurrentDirectory�� ���װ� �ִ�. directory�� ã���� �� �Լ��� ����.
	GlobalFunc::GetModuleFileNameDirectory( szCurDirectory, 512 );

	Snprintf( m_szFileName, 512,  _T("%s%s"), szCurDirectory, _T("HelpReadInfo10.dat") );
	SetFileAttributes( m_szFileName, FILE_ATTRIBUTE_NORMAL );
	TCHAR szTemp[512 + 1];
	Snprintf( szTemp, 512,  _T("%s%s"), szCurDirectory, _T("HelpReadInfo.dat") );
	remove( szTemp );
	m_MemoryINI.LoadFromFile( m_szFileName, SimpleXorCryption );

	m_bLoadInfo = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
uiHelpMan::~uiHelpMan()
{
    //Release();
}


//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::OnInitialize()
{
    _LoadUI();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::OnRelease()
{
    m_MemoryINI.SaveToFile( m_szFileName, SimpleXorCryption );
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::InitHelpMenuList()
{
	m_bLoadInfo = FALSE;
	m_mapHelpInfo.clear();
	m_vecNewHelp.clear();
	
	ClearHelpMission();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::CheckHelpMessage(int iCharStatus, int iParam/* = 0*/, BOOL bIsLimited/* = 0*/)
{
	if(!g_pHero) return;

	if(!m_bLoadInfo)
	{
		LoadReadInfo();
	}

	// ��Ʋ �κ񿡼��� üũ ���Ѵ�
	if (( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_LOBBY )
		|| ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHARSELECT )
		|| ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_LOGIN )
		|| ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_NONE ) )
		return;

	switch(iCharStatus)
	{
	case eHK_CREATE_CHAR:	//1. ĳ���� ����
		{
			if(!IsVillageMap())	
				return;		
		}
		break;

	
	case eHK_JOIN_BATTLE_ZONE:		//3. ��Ʋ�� ����
		{
			if(IsVillageMap())	
				return;		
		}
		break;

	case eHK_ITEM:					//5. ������ ����
		{
			BASE_ITEMINFO *pInfo = ItemInfoParser::Instance()->GetItemInfo((CODETYPE)iParam);

            if (!pInfo)
            {
                return;
            }


			if(bIsLimited)
			{
				_addHelp(iCharStatus, eHP_LIMITED_ITEM);
			}
			else if(pInfo->IsArmor() || pInfo->IsWeapon())
			{
				_addHelp(iCharStatus, eHP_ARMOR_OR_WEAPON);
			}
			else if(pInfo->m_wType == eITEMTYPE_ENCHANT)
			{
				_addHelp(iCharStatus, eHP_ENCHANT_MATERIAL);
			}
			else if(pInfo->m_wType == eITEMTYPE_AC)
			{
				_addHelp(iCharStatus, eHP_AC_ITEM);
			}
			else if(pInfo->m_wType == eITEMTYPE_EVENT)
			{
				_addHelp(iCharStatus, eHP_EVENT_ITEM);
			}
			else if(pInfo->m_wType == eITEMTYPE_JUNK)
			{
				_addHelp(iCharStatus, eHP_MATERIAL_ITEM);
			}
			else if(pInfo->m_wType == eITEMTYPE_QUEST)
			{
				_addHelp(iCharStatus, eHP_QUEST_ITEM);
			}
			else if(pInfo->m_wType == eITEMTYPE_CRYSTAL)
			{
				_addHelp(iCharStatus, eHP_CRYSTAL_ITEM);
			}

			return;
		}
		break;

	case eHK_CLICK_NPC:				//2. NPC Ŭ��
	case eHK_EXP:					//4. ����ġ ����
	case eHK_LEVEL_UP:				//6. ������
	case eHK_ETHERAIN:				//8. ���ڷ��� ���ԺҰ���
	case eHK_AC:					//9. AC 5% ���޽� 
	case eHK_GUILD:					//10. ���â�� �� 
	case eHK_GM_MSG:				//11. GM �޽���
	case eHK_AC_REWARD:				//14. AC ���� (������)
		break;


	default:
		return;
		
	}

	_addHelp(iCharStatus);
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::_addHelp(int iCharStatus, int iParam /*= 0*/)
{
	HELPINDEX_ITER it;
	std::pair<HELPINDEX_ITER, HELPINDEX_ITER> range;

	range = HelpInfoParser::Instance()->m_HelpIndex.equal_range(iCharStatus);

	if(!g_pHero)
		return;

	for(it=range.first; it!=range.second; ++it)
	{
		BASE_HelpIndex index = (*it).second;

		if((index.m_dw_Level == 0) || (g_pHero->GetLevel() == index.m_dw_Level))
		{
			if((index.m_dw_Param == iParam))
			{
				AddNewHelpIndex(index.m_dw_HelpIndex);
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::_addNewHelpVec(DWORD dwHelpIndex)
{
	for(size_t i=0; i<m_vecNewHelp.size(); ++i)
	{
		if(m_vecNewHelp[i] == dwHelpIndex)
			return;
	}

	m_vecNewHelp.push_back(dwHelpIndex);
	
	BASE_HelpInfo*	pInfo = HelpInfoParser::Instance()->GetHelpInfo(dwHelpIndex);
	if(pInfo)
	{
		if(pInfo->m_bSoundControl && pInfo->m_dwSoundValue)
			g_SoundEffect.PlayUI( pInfo->m_dwSoundValue );
	}

	ShowHelpMenu(TRUE);
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::AddNewHelpIndex(DWORD dwHelpIndex)
{
	int iCount = GetReadHelpCount(dwHelpIndex);
	BASE_HelpInfo*	pInfo = HelpInfoParser::Instance()->GetHelpInfo(dwHelpIndex);
	if(pInfo)
	{
		if((pInfo->m_dw_Repeat == 0) || (pInfo->m_dw_Repeat > (DWORD)iCount))		// ��� �ݺ� or �ִ�ݺ�Ƚ���� ���� �ʾ�����..
		{
			if(pInfo->m_bHelpMethod == eHM_ANNOUNCE)
			{
#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
				ShowHelpAnnounce(dwHelpIndex);
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW
			}
			else
			{
				if(pInfo->m_bWindow_States)	// �����ܿ� �����ʰ� �ٷ� Show
				{
					ShowHelpWindowForIndex(dwHelpIndex);
				}
				else
				{
					_addNewHelpVec(dwHelpIndex);
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::SetHelpIndex(DWORD dwHelpIndex, int iCount)
{
	//TODO: ����� ������ �о�ͼ� m_mapHelpInfo�� �ִ´�.
	std::map<DWORD, stHelpReadInfo>::iterator it;
	it = m_mapHelpInfo.find(dwHelpIndex);
	
	if(it != m_mapHelpInfo.end())
	{
		// �̹� �ִ� �ε�����
		(*it).second.m_dw_Repeat = iCount;
		return;
	}

	stHelpReadInfo info;
	info.m_dw_HelpIndex = dwHelpIndex;
	info.m_dw_Repeat = iCount;

	
	m_mapHelpInfo.insert(std::pair<DWORD, stHelpReadInfo>(dwHelpIndex,info));
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::AddReadHelpIndex(DWORD dwHelpIndex,  BOOL bSave)
{
	int iCount = GetReadHelpCount(dwHelpIndex);
	++iCount;

	if(iCount >= 100)
		iCount = 100;	// 100 ������ �׳� 100����.. (�̷��� �Ǹ� ��ǻ� ���ѹݺ��̹Ƿ� ��� ���ڸ� �ø��ʿ�� ����)

	SetHelpIndex(dwHelpIndex, iCount);

	std::vector<DWORD>::iterator it;
	it = std::find(m_vecNewHelp.begin(), m_vecNewHelp.end(), dwHelpIndex);
	if(it != m_vecNewHelp.end())
	{
		m_vecNewHelp.erase(it);
	}

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	if(m_vecNewHelp.size() == 0)
		m_pUIHelpMenu->ShowInterface(FALSE);
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

	if(bSave)
	{
		SaveReadInfo(dwHelpIndex, iCount);
	}

}

//------------------------------------------------------------------------------
/**
*/
int uiHelpMan::GetReadHelpCount(DWORD dwHelpIndex)
{
	int iCount = 0;

	std::map<DWORD, stHelpReadInfo>::iterator it;
	it = m_mapHelpInfo.find(dwHelpIndex);

	if(it != m_mapHelpInfo.end())
	{
		// �̹� �ִ� �ε�����
		iCount = (*it).second.m_dw_Repeat;
		return iCount;
	}

	return iCount;
}


//------------------------------------------------------------------------------
/**
*/
int uiHelpMan::GetNewHelpIndex(DWORD dwHelpIndex)
{
	for(size_t i=0; i<m_vecNewHelp.size(); ++i)
	{
		if(m_vecNewHelp[i] == dwHelpIndex)
			return i;
	}

	return -1;
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::ShowHelpMenu(BOOL bShow, BOOL bForce/* = FALSE*/)
{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    if(bForce)
    {
        m_pUIHelpMenu->ShowInterface(bShow);
    }
    else
    {
        if(bShow)
        {
            if(!GAMEOPTION->GetShowHelp()) return;

            m_pUIHelpMenu->ShowInterface(TRUE);
        }
        else
        {
            m_pUIHelpMenu->ShowInterface(FALSE);
        }
    }
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
}


#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
//------------------------------------------------------------------------------
void uiHelpMan::RestoreHelpInfo()
{
    if (m_pUIHelpAnnounceWindow)
    {
        m_pUIHelpAnnounceWindow->RestoreHelpInfo();
    }
}

#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW
//------------------------------------------------------------------------------
/**
*/

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
void uiHelpMan::ShowHelpAnnounce(DWORD dwHelpIndex,BOOL bIgnoreOption)
{
	// bIgnoreOption : �ɼǿ� ���� �Ⱥ��ٴ� ������ �Ǿ��־ ������ ����Ϸ��� bIgnoreOption�� true�� ����
	if(!bIgnoreOption && !GAMEOPTION->GetShowHelp()) return;

	BASE_HelpInfo* pInfo = HelpInfoParser::Instance()->GetHelpInfo(dwHelpIndex);
	
	if(pInfo)
	{
		if(pInfo->m_bSoundControl && pInfo->m_dwSoundValue)
			g_SoundEffect.PlayUI( pInfo->m_dwSoundValue );

		if(pInfo->m_dw_Repeat > 0)	// m_dw_Repeat�� 0�̸� ���� �ݺ��̹Ƿ� Ƚ�� ������ ���Ѵ�
		{
			AddReadHelpIndex(dwHelpIndex, TRUE);
		}

		m_pUIHelpAnnounceWindow->ShowInterface(TRUE);

		BASE_HelpInfo helpInfo;
		memcpy(&helpInfo, pInfo, sizeof(helpInfo));
		m_pUIHelpAnnounceWindow->SetHelpInfo(helpInfo);
	}
	
}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::ShowGMMsg(DWORD dwHelpIndex, int iParam/*=0*/, bool bShutDown/* = false*/)	// GM �޽��� ��¿� �Լ�
{
	if (( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_LOGIN )
				|| ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHARSELECT )
				|| ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_NONE ))
		return;

	BASE_HelpInfo* pInfo = HelpInfoParser::Instance()->GetHelpInfo(dwHelpIndex);
	
	if(pInfo)
	{
		if(pInfo->m_dw_CharStatus != eHK_GM_MSG)
			return;

		if(pInfo->m_bSoundControl && pInfo->m_dwSoundValue)
			g_SoundEffect.PlayUI( pInfo->m_dwSoundValue );

		m_pUIHelpAnnounceWindow->ShowInterface(TRUE);

		BASE_HelpInfo helpInfo;
		memcpy(&helpInfo, pInfo, sizeof(helpInfo));
		m_pUIHelpAnnounceWindow->SetGMHelpInfo(helpInfo, iParam);
	}
	else
	{
		if(bShutDown)	// ���� ���� �޽����� ���, �߸��� �ε����� ���� �⺻ ���� �޽����� ���
		{
			ShowGMMsg(60001, iParam);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::ClearHelpMission()
{
	m_vecHelpMissionInfo.clear();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::HideHelpMission()
{
	if(m_pUIHelpMission)
	{
		m_pUIHelpMission->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::AddHelpMission(int iTextID, DWORD dwNCode, DWORD dwDCode, DWORD dwImgCode)
{
	stHelpMissionInfo info;
	info.iTextID = iTextID;
	info.dwNCode = dwNCode;
	info.dwDCode = dwDCode;
	info.dwImgCode = dwImgCode;

	for(unsigned int i=0; i<m_vecHelpMissionInfo.size(); ++i)
	{
		if(m_vecHelpMissionInfo[i].iTextID == iTextID)
			return;		// �ߺ��� �ε����� ������ �߰����� ����
	}

	m_vecHelpMissionInfo.push_back(info);
	
	UpdateHelpMission(iTextID);
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::DelHelpMission(int iTextID)
{
	HELP_MISSION_VEC::iterator it;
	for(it = m_vecHelpMissionInfo.begin(); it != m_vecHelpMissionInfo.end(); ++it)
	{
		if((*it).iTextID == iTextID)
		{
			m_vecHelpMissionInfo.erase(it);
			break;
		}
	}

	UpdateHelpMission();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::UpdateHelpMission(int iNewItemTextID/* = -1*/)	// ������ �߰��� ��� iNewItemTextID �� TextID �ѱ�
{
	// �ɼǿ��� ���� �Ⱥ��ٴ� �������� ������� ������ �����ֹǷ� ���� üũ ����
	if(m_pUIHelpMission)
	{
		if(m_vecHelpMissionInfo.size() <= 0)
		{
			m_pUIHelpMission->ShowInterface(FALSE);
		}
		else
		{
			if(!m_pUIHelpMission->IsVisibleWindow())
			{
				m_pUIHelpMission->ShowInterface(TRUE);
			}

			m_pUIHelpMission->UpdateDisplay(iNewItemTextID);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::ShowHelpMonsterInfo(BOOL bVal)
{
    // �̱۸��� ���� �̵� �� ��ų ���� ���
	m_pUIHelpMonsterInfo->ShowInterface(bVal);
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::ShowHelpWindow(int iBtnIndex)
{
	if(iBtnIndex >= c_iMaxHelpIcon) return;

	if((size_t)iBtnIndex >= m_vecNewHelp.size()) return;

	g_SoundEffect.PlayUI( UI_SOUND_HELP_CLICK );

	DWORD dwHelpIndex;
	if(m_vecNewHelp.size() > c_iMaxHelpIcon)
	{
		int iGap = c_iMaxHelpIcon - iBtnIndex;
		int iIndex = m_vecNewHelp.size() - iGap;
		dwHelpIndex = m_vecNewHelp[iIndex];
	}
	else
	{
		dwHelpIndex = m_vecNewHelp[iBtnIndex];
	}

	BASE_HelpInfo* pInfo = HelpInfoParser::Instance()->GetHelpInfo(dwHelpIndex);
	
	AddReadHelpIndex(dwHelpIndex, TRUE);

	m_pUIHelpWindow->ShowInterface(TRUE);

	BASE_HelpInfo helpInfo;
	memcpy(&helpInfo, pInfo, sizeof(helpInfo));
	m_pUIHelpWindow->SetHelpInfo(helpInfo);
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::ShowHelpWindowForIndex(DWORD dwHelpIndex)
{
	BASE_HelpInfo* pInfo = HelpInfoParser::Instance()->GetHelpInfo(dwHelpIndex);
	
	AddReadHelpIndex(dwHelpIndex, TRUE);

	m_pUIHelpWindow->ShowInterface(TRUE);

	BASE_HelpInfo helpInfo;
	memcpy(&helpInfo, pInfo, sizeof(helpInfo));
	m_pUIHelpWindow->SetHelpInfo(helpInfo);
}

//------------------------------------------------------------------------------
/**
*/
#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
void uiHelpMan::ShowHelpTest(DWORD dwHelpIndex)
{
	BASE_HelpInfo* pInfo = HelpInfoParser::Instance()->GetHelpInfo(dwHelpIndex);

	if(pInfo->m_bHelpMethod == eHM_ANNOUNCE)
	{
		m_pUIHelpAnnounceWindow->ShowInterface(TRUE);

		BASE_HelpInfo helpInfo;
		memcpy(&helpInfo, pInfo, sizeof(helpInfo));
		m_pUIHelpAnnounceWindow->SetHelpInfo(helpInfo);
	}
	else
	{
		m_pUIHelpWindow->ShowInterface(TRUE);

		BASE_HelpInfo helpInfo;
		memcpy(&helpInfo, pInfo, sizeof(helpInfo));
		m_pUIHelpWindow->SetHelpInfo(helpInfo);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::ShowHelpTestStringCode(DWORD dwStringCode)
{
	m_pUIHelpWindow->ShowInterface(TRUE);

	m_pUIHelpWindow->SetHelpStringCode(dwStringCode);
}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::LoadReadInfo()
{
	m_bLoadInfo = TRUE;

	TCHAR lpszTemp[1024];
	TCHAR szCharInfo[256] = {0,};
	TCHAR szIndex[64] = {0,};

	uiLoginMan* loginMan =
        static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
	if(loginMan && g_pHero)
	{
		Snprintf(szCharInfo, 255, _T("%s-%s"), loginMan->GetID(), g_pHero->GetName());
		
		for(int i = 0; i !=  HelpInfoParser::Instance()->m_vecHelpIndex.size(); ++i)
		{
			int iIndex = HelpInfoParser::Instance()->m_vecHelpIndex[i];
			Sprintf(szIndex, _T("%d"), iIndex);


		if( m_MemoryINI.getString( szCharInfo, szIndex, lpszTemp, 1023 ) == -1 )
			_tcsncpy( lpszTemp, _T("0"), 1023 );
			int iRead = ParamStr2Int( lpszTemp);
			
			if(iRead)
				SetHelpIndex(iIndex, iRead);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::SaveReadInfo(DWORD dwIndex, int iCount)
{
	TCHAR szCharInfo[256] = {0,};
	TCHAR szIndex[64] = {0,};

	uiLoginMan* loginMan =
        static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
	if(loginMan && g_pHero)
	{

		Snprintf(szCharInfo, 255, _T("%s-%s"), loginMan->GetID(), g_pHero->GetName());

		char lpszParamBuffer[1024] = {0,};	// �Ķ���� Save/Load �� ���� ����

		Sprintf(szIndex, _T("%d"), dwIndex);

		TCHAR szResult[INTERFACE_STRING_LENGTH] ={0,};

		StrnCopy( szResult, ParamInt2Str( iCount, lpszParamBuffer ), INTERFACE_STRING_LENGTH - 1 );

		m_MemoryINI.putString( szCharInfo, szIndex, szResult );
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::ResetReadInfo(TCHAR* pCharName)
{
	TCHAR szCharInfo[256] = {0,};

	uiLoginMan* loginMan =
        static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
	if(loginMan)
	{
		Snprintf(szCharInfo, 255, _T("%s-%s"), loginMan->GetID(), pCharName);

		m_MemoryINI.putString( szCharInfo, "", "" );
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMan::SetPosition()
{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	if(m_pUIHelpMenu && m_pUIHelpMenu->IsVisible())
		m_pUIHelpMenu->UpdatePosition();
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

	if(m_pUIHelpWindow && m_pUIHelpWindow->IsVisible())
		m_pUIHelpWindow->UpdatePosition();
}


//------------------------------------------------------------------------------
/**
*/
#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
void uiHelpMan::ShowQuestHelp(int iParam,eHELP_KIND eKind)
{
	if(!g_pHero)
		return;

	HELPINDEX_ITER it;
	std::pair<HELPINDEX_ITER, HELPINDEX_ITER> range;

	range = HelpInfoParser::Instance()->m_HelpIndex.equal_range(eKind);

	for(it=range.first; it!=range.second; ++it)
	{
		BASE_HelpIndex index = (*it).second;

		if((index.m_dw_Param == iParam))	
		{
			// ������ ����Ʈ�� ���� ���� ������ �ִٸ� �̱״Ͻ�â ���
			BASE_HelpInfo* pInfo = HelpInfoParser::Instance()->GetHelpInfo(index.m_dw_HelpIndex);
			if(pInfo)
			{
				if(pInfo->m_bSoundControl && pInfo->m_dwSoundValue)
					g_SoundEffect.PlayUI( pInfo->m_dwSoundValue );

				m_pUIHelpAnnounceWindow->ShowInterface(TRUE);

				BASE_HelpInfo helpInfo;
				memcpy(&helpInfo, pInfo, sizeof(helpInfo));
				m_pUIHelpAnnounceWindow->SetHelpInfo(helpInfo);
			}
		}
	}
}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW

void uiHelpMan::SetMissionTimer(DWORD require_tIme, BYTE progress_type)
{
    if (help_mission_timer_)
    {
        help_mission_timer_->ShowInterface(TRUE);
        help_mission_timer_->SetMissionTimer(require_tIme, progress_type);

    }
}
