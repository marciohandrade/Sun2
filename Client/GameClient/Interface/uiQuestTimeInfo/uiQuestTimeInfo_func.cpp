//------------------------------------------------------------------------------
//  uiQuestTimeInfo_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan/uiQuestMan.h"
#include "uiQuestTimeInfo.h"
#include "ItemUnitRenderer.h"
#include "SlotKeyGenerator.h"
#include "SCItemSlot.h"
#include "ItemManager.h"
#include "QuestManager_Concrete.h"
#include "ItemInfoParser.h"
#include "Quest.h"
#include <SolarHashTable.h>
#include "NPCInfoParser.h"
#include "RewardInfoList.h"
#include "ItemTypeList.h"
#include "ItemInfoParser.h"
#include "QuestTextInfoParser.h"


//------------------------------------------------------------------------------
/**
*/
uiQuestMan*
uiQuestTimeInfo::GetManager()
{
    uiQuestMan* puiQuestMan  =
        static_cast<uiQuestMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_QUEST));
    assert (puiQuestMan);
    return puiQuestMan;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestTimeInfo::ClearInfo()
{
	if ( !this->CheckControls() )
		return;

	m_pQuest = NULL;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestTimeInfo::TimeLimitQuest(QCODE QuestCode)
{
	m_pQuest = g_pQMGR_Real->FindQuest(QuestCode); 	
	if ( !m_pQuest )
	{
		ShowInterface(FALSE);
		return;
	}


	DWORD dwRemainTime = m_pQuest->GetRemainedTime();
	if (dwRemainTime) 
	{
		if ( !IsVisible() )
		{
			ShowInterface(TRUE);
		}
	}
	else
	{
		if (IsVisible())
		{
			ShowInterface(FALSE);
		}
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestTimeInfo::initInfo()
{
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestTimeInfo::updateInfo()
{
	if (!m_pQuest)
	{
		return;
	}

	DWORD dwRemainTime = m_pQuest->GetRemainedTime();
	int iRemainTimeSec = 0;
	int iRemainTimeMin = 0;
	int iRemainTimeHour = 0;
	int iRemainTimeDay = 0;
    TCHAR szMessage[INTERFACE_STRING_LENGTH];

	SetCaptionQuestName( m_pQuest->GetQuestInfo()->GetQNCode() );

	if ( dwRemainTime )
	{
		dwRemainTime = dwRemainTime / SEC_PER_TICK;
		iRemainTimeSec = dwRemainTime % 60;
		iRemainTimeMin = dwRemainTime / 60;

		m_szResult[0] = 0;

		iRemainTimeHour = iRemainTimeMin / 60;
		if ( iRemainTimeHour )
		{
			iRemainTimeMin -= iRemainTimeHour*60;

            // (9067) %3d½Ã
            GetUIMan()->GetInterfaceString( 9067, szMessage, INTERFACE_STRING_LENGTH);
            Sprintf( m_szText, szMessage, iRemainTimeHour );
            _tcscat( m_szResult, m_szText );
            _tcscat( m_szResult, _T(" "));  // ÇÑ Ä­ ¶ç¿öÁØ´Ù.		

            // (9068) %2dºÐ
            GetUIMan()->GetInterfaceString( 9068, szMessage, INTERFACE_STRING_LENGTH);
			Sprintf( m_szText, szMessage, iRemainTimeMin );
			_tcscat( m_szResult, m_szText );
            _tcscat( m_szResult, _T(" "));  // ÇÑ Ä­ ¶ç¿öÁØ´Ù.		
		}
		else
		{
			if (iRemainTimeMin > 0)
			{
                // (9068) %2dºÐ
                GetUIMan()->GetInterfaceString( 9068, szMessage, INTERFACE_STRING_LENGTH);
                Sprintf( m_szText, szMessage, iRemainTimeMin );
                _tcscat( m_szResult, m_szText );
                _tcscat( m_szResult, _T(" "));  // ÇÑ Ä­ ¶ç¿öÁØ´Ù.		
			}	
		}

        // (9069) %2dÃÊ
        GetUIMan()->GetInterfaceString( 9069, szMessage, INTERFACE_STRING_LENGTH);
		Sprintf( m_szText, szMessage, iRemainTimeSec );
		_tcscat( m_szResult, m_szText );		

		SetCaptionQuestTimeInfo( m_szResult );
	}
	else
	{
		ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
