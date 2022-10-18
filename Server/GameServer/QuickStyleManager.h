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
        - 2006/2/14(�߷�Ÿ�ε���^^/) : SerializeStyleTotalInfo()�Լ� �߰�, stream���� �Լ� templateȭ
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

    void UpdateStyleSlot(); // ��Ÿ�� ��������� ���ϸ� ��Ÿ�� ������ ������ �ڵ����� ���Ž����ش�.

private:

    Player* m_pPlayer;
    SlotManager* m_pSlotMGR;
    SCSlotContainer** m_ppSlotContainer;
};

//==================================================================================================
// stream���� template ������
#include "QuickStyleManager_Stream.hpp"
//==================================================================================================
*/
#endif //GAMESERVER_QUICK_STYLE_MANAGER_H