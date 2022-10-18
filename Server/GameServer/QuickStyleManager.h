#pragma once
#ifndef GAMESERVER_QUICK_STYLE_MANAGER_H
#define GAMESERVER_QUICK_STYLE_MANAGER_H

//==================================================================================================
#define SKILL_SYSTEM_VERSION_ROUTING_INL
    #include "./SkillSystem/SkillSlotManager.inl"
#undef SKILL_SYSTEM_VERSION_ROUTING_INL
//==================================================================================================

//==================================================================================================
/// QuickStyleManager class
/**
    @author
        Kim Min Wook < taiyo@webzen.com >   
    @since
        2004. 8. 11~
    @history
        - 2006/2/14(발렌타인데이^^/) : SerializeStyleTotalInfo()함수 추가, stream관련 함수 template화
        - 2010/8/26, waverx, f100826.2L,
            this main functions extract to an interface class 'IQuickStyleManager' and
            moved to a 'SkillSlotManager' to support The Episode II perk system.
*/
//==================================================================================================

class SCSlotContainer;
class SCQuickStyleSlotContainer;
class SlotManager;
class Player;

//==================================================================================================
/*
// CHANGES, f100826.2L, removed
class QuickStyleManager
{
public:
    QuickStyleManager(void);
    ~QuickStyleManager(void);

    void Init(SlotManager* pMGR);
    void Release();

    SCQuickStyleSlotContainer* GetQuickStyleSlotContainer();

    template< class STYLE_TOTAL_INFO_TYPE >
    void SerializeStyleTotalInfo(STYLE_TOTAL_INFO_TYPE& StyleInfo, eSERIALIZE eType);
    template< class STYLESTREAM_TYPE >
    void SerializeStyleStream(POSTYPE pos, STYLESTREAM_TYPE* IN pStream, eSERIALIZE eType = SERIALIZE_LOAD);

    RC::eSTYLE_RESULT Link(SLOTCODE code, POSTYPE toPos);
    RC::eSTYLE_RESULT Unlink(POSTYPE atPos);
    RC::eSTYLE_RESULT Move(POSTYPE fromPos, POSTYPE toPos);

    void UpdateStyleSlot(); // 스타일 사용조건이 변하면 스타일 퀵슬롯 정보도 자동으로 갱신시켜준다.

private:

    Player* m_pPlayer;
    SlotManager* m_pSlotMGR;
    SCSlotContainer** m_ppSlotContainer;
};

//==================================================================================================
// stream관련 template 구현부
#include "QuickStyleManager_Stream.hpp"
//==================================================================================================
*/
#endif //GAMESERVER_QUICK_STYLE_MANAGER_H