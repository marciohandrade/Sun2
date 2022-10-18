#pragma once

#include "PetInfoParser.h"

class CPetManager
{
public:
    CPetManager();
    ~CPetManager();

    void Init(Player* pPlayer);
    void Clear();
    RC::ePET_SUMMON_RESULT UsePetItem(SCItemSlot& rPetItemSlot);

    BOOLEAN AlreadySummonedPet() { return m_SummonedPet; }
    
public:
    RC::ePET_SUMMON_RESULT ReturnPet(BOOL bForce = FALSE);
    RC::ePET_SUMMON_RESULT ReturnPet(SCItemSlot* pPetItemSlot, BOOL bForce = FALSE);
    // �� �� �������θ� �����ؾ� �ϴ°�?... ¼��... ���߿� â�������� �����ؼ� �ٷ�ItemSlot��� �κп� ���ؼ� ��������.
    SCItemSlot* GetUsedPetSlot() { return m_SummonedPet ? m_pPetItemSlot : 0; }

    void Update();

    void IncreaseFullness(BYTE value);
    void DecreaseFullness(BYTE value);
    void IncreaseIntimacy(BYTE value);
    void DecreaseIntimacy(BYTE value);

    BYTE GetFullness();
    BYTE GetIntimacy();

    void OnDead();
    sPet_Script_Info* GetPetScriptInfo() { return m_pPetInfo; }
    // (CHANGES) (f100524.2L) added by _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    const sPet_Script_Info* GetImagePetScriptInfo() const { return m_pImagePetInfo; };
    void SetSummonInfo();
    void SetPetName(const char* pszPetName);
    const char* GetPetName();
    BOOL IsSetPetName();

private:
    RC::ePET_SUMMON_RESULT SummonPet(SCItemSlot* pPetItemSlot);
    void ApplyBasicOption();
    void CancelBasicOption();

    void StartAddOption();
    void EndAddOption();
    void ApplyAddOption(BYTE byptionLV);
    void CancelAddOption(); 

    BOOLEAN IsFullnessMin();
    BYTE GetOptionLVByIntimacy();

    BOOL IsValidPetItemSlot();

private:
    Player* m_pPlayer;

    BOOLEAN m_SummonedPet;
    POSTYPE  m_PetItemPos;
    DBSERIAL m_PetItemSerial;
    SCItemSlot* m_pPetItemSlot;
    sPet_Script_Info* m_pPetInfo;
    // (CHANGES) (f100524.2L) added by _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    const sPet_Script_Info* m_pImagePetInfo;

    BOOLEAN m_bAppliedBasicOption; //�⺻ �ɼ��� ���� �Ǿ��°�?
    BOOLEAN m_bAppliedAddOption; //�߰� �ɼ��� ���� �Ǿ��°�?
    BYTE    m_AppliedOptionLV;
    BOOLEAN m_bMAXIntimacy;

    util::Timer m_FiveMinTimer;
    util::Timer m_TenMinTimer;
    util::Timer m_MaxIntimacyTimer;
};

