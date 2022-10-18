#include "stdafx.h"
#include "CCUControl.h"
#include <SunAuth/AuthDefine.h>

//////////////////////////////////////////////////////////////////////////
// 

CCUControl::CCUControl()
{
	m_CCU_INFO.dwVillageCount = 0;
	m_CCU_INFO.dwVillageCountPC = 0;
	m_CCU_INFO.dwBattleZoneCount = 0;
	m_CCU_INFO.dwBattleZoneCountPC = 0;
#ifdef _CHANNELING_AUTH_FLOW_HANGAME_USER_
    m_CCU_INFO.numberOfInVillageHangame = 0;
    m_CCU_INFO.numberOfInVillageHangamePC = 0;
    m_CCU_INFO.numberOfInBattleZoneHangame = 0;
    m_CCU_INFO.numberOfInBattleZoneHangamePC = 0;
#endif
#ifdef __NA000000_090911_NEOWIZ_USER_CCU_REPORT__
    m_CCU_INFO.numberOfInVillageNeowiz = 0;
    m_CCU_INFO.numberOfInVillageNeowizPC = 0;
    m_CCU_INFO.numberOfInBattleZoneNeowiz = 0;
    m_CCU_INFO.numberOfInBattleZoneNeowizPC = 0;
#endif
}

VOID	CCUControl::Calculation(User* const user)
{
#ifdef _CHANNELING_AUTH_FLOW_HANGAME_USER_
    const eZONETYPE zoneType = user->GetZoneType();
    BOOLEAN isInVillage = !!(MASK_VILLAGE_GROUP & (1 << zoneType));
    BOOLEAN isPcRoom = !!user->GetReservedValue();
    const BYTE user_type = user->GetAuthUserType();
    //BOOLEAN isHangame = auth_user_type == AUTH_USER_HANGAME;

    const DWORD select = MAKEDWORD(MAKEWORD(isInVillage, isPcRoom), MAKEWORD(FALSE, user_type));

    switch(select)
    {                   // Village, PC              Res., eAUTH_USER_TYPE (0:Webzen,1:Hangame,2:Neowiz)
    case MAKEDWORD(MAKEWORD(FALSE, FALSE), MAKEWORD(FALSE, AUTH_USER_DEFAULT)):
        // 배틀존, 일반 유저, 웹젠 유저
        ++m_CCU_INFO.dwBattleZoneCount;
        break;
    case MAKEDWORD(MAKEWORD(FALSE, TRUE ), MAKEWORD(FALSE, AUTH_USER_DEFAULT)):
        // 배틀존, PC방 유저, 웹젠 유저
        ++m_CCU_INFO.dwBattleZoneCountPC;
        break;
    case MAKEDWORD(MAKEWORD(TRUE,  FALSE), MAKEWORD(FALSE, AUTH_USER_DEFAULT)):
        // 마을, 일반 유저, 웹젠 유저
        ++m_CCU_INFO.dwVillageCount;
        break;
    case MAKEDWORD(MAKEWORD(TRUE,  TRUE ), MAKEWORD(FALSE, AUTH_USER_DEFAULT)):
        // 마을, PC방 유저, 웹젠 유저
        ++m_CCU_INFO.dwVillageCountPC;
        break;
    case MAKEDWORD(MAKEWORD(FALSE, FALSE), MAKEWORD(FALSE,  AUTH_USER_HANGAME)):
        // 배틀존, 일반 유저, 한게임 유저
        ++m_CCU_INFO.numberOfInBattleZoneHangame;
        break;
    case MAKEDWORD(MAKEWORD(FALSE, TRUE ), MAKEWORD(FALSE,  AUTH_USER_HANGAME)):
        // 배틀존, PC방 유저, 한게임 유저
        ++m_CCU_INFO.numberOfInBattleZoneHangamePC;
        break;
    case MAKEDWORD(MAKEWORD(TRUE,  FALSE), MAKEWORD(FALSE,  AUTH_USER_HANGAME)):
        // 마을, 일반 유저, 한게임 유저
        ++m_CCU_INFO.numberOfInVillageHangame;
        break;
    case MAKEDWORD(MAKEWORD(TRUE,  TRUE ), MAKEWORD(FALSE,  AUTH_USER_HANGAME)):
        // 마을, PC방 유저, 한게임 유저
        ++m_CCU_INFO.numberOfInVillageHangamePC;
        break;
  #ifdef __NA000000_090911_NEOWIZ_USER_CCU_REPORT__ //슬슬 정리할 때가 되었다.
    case MAKEDWORD(MAKEWORD(FALSE, FALSE), MAKEWORD(FALSE,  AUTH_USER_NEOWIZ)):
        // 배틀존, 일반 유저, 피망 유저
        ++m_CCU_INFO.numberOfInBattleZoneNeowiz;
        break;
    case MAKEDWORD(MAKEWORD(FALSE, TRUE ), MAKEWORD(FALSE,  AUTH_USER_NEOWIZ)):
        // 배틀존, PC방 유저, 피망 유저
        ++m_CCU_INFO.numberOfInBattleZoneNeowizPC;
        break;
    case MAKEDWORD(MAKEWORD(TRUE,  FALSE), MAKEWORD(FALSE,  AUTH_USER_NEOWIZ)):
        // 마을, 일반 유저, 피망 유저
        ++m_CCU_INFO.numberOfInVillageNeowiz;
        break;
    case MAKEDWORD(MAKEWORD(TRUE,  TRUE ), MAKEWORD(FALSE,  AUTH_USER_NEOWIZ)):
        // 마을, PC방 유저, 피망 유저
        ++m_CCU_INFO.numberOfInVillageNeowizPC;
        break;
  #endif
    };

#else //!_CHANNELING_AUTH_FLOW_HANGAME_USER_
        eZONETYPE ztZONE = user->GetZoneType();
        BOOLEAN bGRP_VILLAGE = (MASK_VILLAGE_GROUP & (1 << ztZONE)) ? TRUE : FALSE;
        BOOLEAN bPCR_USER = user->GetReservedValue() ? TRUE : FALSE;

        DWORD dwSELECT = MAKEDWORD(bGRP_VILLAGE, bPCR_USER);

        switch( dwSELECT )
        {
        case MAKEDWORD(FALSE, FALSE):	// 배틀존, 일반 유저
            ++m_CCU_INFO.dwBattleZoneCount;
            break;
        case MAKEDWORD(FALSE, TRUE):	// 배틀존, PC방 유저
            ++m_CCU_INFO.dwBattleZoneCountPC;
            break;
        case MAKEDWORD(TRUE, FALSE):	// 마을, 일반 유저
            ++m_CCU_INFO.dwVillageCount;
            break;
        case MAKEDWORD(TRUE, TRUE):		// 마을, PC방 유저
                ++m_CCU_INFO.dwVillageCountPC;
            break;
        }
#endif //!_CHANNELING_AUTH_FLOW_HANGAME_USER_
}


VOID	CCUControl::GetCurrentCCU(MSG_AM_CONNECTION_CCU_REPORT_NTF* OUT pInfo)
{
	pInfo->dwVillageCount = m_CCU_INFO.dwVillageCount;
	pInfo->dwVillageCountPC = m_CCU_INFO.dwVillageCountPC;
	pInfo->dwBattleZoneCount = m_CCU_INFO.dwBattleZoneCount;
	pInfo->dwBattleZoneCountPC = m_CCU_INFO.dwBattleZoneCountPC;
#ifdef _CHANNELING_AUTH_FLOW_HANGAME_USER_
    pInfo->numberOfInVillageHangame = m_CCU_INFO.numberOfInVillageHangame;
    pInfo->numberOfInVillageHangamePC = m_CCU_INFO.numberOfInVillageHangamePC;
    pInfo->numberOfInBattleZoneHangame = m_CCU_INFO.numberOfInBattleZoneHangame;
    pInfo->numberOfInBattleZoneHangamePC = m_CCU_INFO.numberOfInBattleZoneHangamePC;
#endif
#ifdef __NA000000_090911_NEOWIZ_USER_CCU_REPORT__
    pInfo->numberOfInVillageNeowiz = m_CCU_INFO.numberOfInVillageNeowiz;
    pInfo->numberOfInVillageNeowizPC = m_CCU_INFO.numberOfInVillageNeowizPC;
    pInfo->numberOfInBattleZoneNeowiz = m_CCU_INFO.numberOfInBattleZoneNeowiz;
    pInfo->numberOfInBattleZoneNeowizPC = m_CCU_INFO.numberOfInBattleZoneNeowizPC;
#endif
}


