#ifndef __PROGRAMCOMMON_SETITEMOPTIONINFOPARSER_H
#define __PROGRAMCOMMON_SETITEMOPTIONINFOPARSER_H
#pragma once

//==================================================================================================

#include <Singleton.h>
#include <ScriptCode.IParser.h>

//==================================================================================================

#define MAX_SETITEM_ACTIVE_EQUIP_OPTION 8
#ifdef _NA_0_SETITEM_FULL_OPTION_COUNT_CHANGE
#define MAX_SETITEM_FULL_OPTION         6
#else
#define MAX_SETITEM_FULL_OPTION         5
#endif
#define MAX_SETITEM_OPTION_LEVEL        3   
#define MAX_SETITEM_SPECIAL_OPTION      5 

#define MAX_SETITEM_EFFECT_NUM          4 //_YJ_ADD_SET_ITEM_EFFECT_001177

//--------------------------------------------------------------------------------------------------

enum eSETITEM_OPTION_LEVEL
{
    SETITEM_OPTION_LEVEL_NONE,
    SETITEM_OPTION_LEVEL_FIRST,
    SETITEM_OPTION_LEVEL_SECOND,
    SETITEM_OPTION_LEVEL_THIRD,
    SETITEM_OPTION_LEVEL_FULL,          
};

//��Ʈ ������ �⺻ �ɼ� ����
struct SET_ITEM_OPTION
{
    BYTE m_byOptionKind; //�ɼ�����
    BYTE m_byUseType;    //��ġŸ��
    int  m_nOption;
};

// ��� ��ġ�� ���� Ȱ��ȭ �ɼ�
struct EQUIP_ACTIVE_OPTION
{
    POSTYPE         m_EqPos; //�����ġ
    //Ȱ��ȭ �ɼ� (2, 4, 6�� �̷��� 3���� �ִ�.)
    SET_ITEM_OPTION m_ActiveOption[MAX_SETITEM_OPTION_LEVEL];
};

// Ư�� ���� ������ �ɼ� 
struct SPECIAL_EQUIP_ACTIVE_OPTION
{
    POSTYPE         m_EqPos;
    WORD            m_wItemCode; //������ Ư�� ������
    SET_ITEM_OPTION m_ActiveOption[MAX_SETITEM_SPECIAL_OPTION];
};

//--------------------------------------------------------------------------------------------------

struct SETITEM_OPTION_INFO
{
    WORD  m_wSetGroupCode;          //��Ʈ �׷� �ڵ�
    BYTE  m_byItemType;             //������Ÿ��
    BYTE  m_byActiveFullOptionNum;  //Ǯ�ɼ� Ȱ��ȭ ��

    //��� ��ġ�� ���� Ȱ��ȭ �ɼ�(��� ���� ��ġ�� 8������ �ִ�.)
    EQUIP_ACTIVE_OPTION m_EquipActiveOption[MAX_SETITEM_ACTIVE_EQUIP_OPTION];

    //Ǯ ��� ��������  �ɼ�
    SET_ITEM_OPTION     m_FullOption[MAX_SETITEM_FULL_OPTION]; 

#ifdef _YMS_FULLSETITEM_RESOURCE_DIVISION_
#else
    //! Ǯ���ϰ�� �������� �ٲٴ� ���Ŀ��� ��ī������ ���ҽ��� �ٴ� ���·� �ٸ鼭 ������
    DWORD m_FullSetChangeItem;
#endif // _YMS_FULLSETITEM_RESOURCE_DIVISION_

    SPECIAL_EQUIP_ACTIVE_OPTION m_SpecialEquipOptionItem;

#ifdef _YMS_GENDER_FULLSETITEM
    DWORD m_dwEffectCode[MAX_SETITEM_EFFECT_NUM][GenderType::kFemale + 1]; // Ǯ �ɼ� ȿ���ڵ�
    BYTE  m_btBoneIdx[MAX_SETITEM_EFFECT_NUM][GenderType::kFemale + 1];    // ����Ʈ ���� �� �ε���

    DWORD change_armor_resource[GenderType::kFemale + 1];
#else
    DWORD m_dwEffectCode[MAX_SETITEM_EFFECT_NUM]; // Ǯ �ɼ� ȿ���ڵ�
    BYTE  m_btBoneIdx[MAX_SETITEM_EFFECT_NUM];    // ����Ʈ ���� �� �ε���

    DWORD change_armor_resource;      //! Ǯ���ϰ�� �߰������� �ٿ��� ���ҽ�
#endif // _YMS_GENDER_FULLSETITEM
};

//==================================================================================================

class SetItemOptionInfoParser : public util::Singleton<SetItemOptionInfoParser>, public IParser
{
private:
    typedef STLX_HASH_MAP<DWORD, SETITEM_OPTION_INFO*> SETITEM_OPTION_INFO_MAP;
public:
    SetItemOptionInfoParser(void);
    ~SetItemOptionInfoParser(void);

    virtual void Release();
    BOOL LoadScript(eSCRIPT_CODE script_code, BOOL bReload);
    BOOL _Load(BOOL bReload);

    // CHANGES: f110325.3L, rearrangement interfaces
    // interfaces
    eSETITEM_OPTION_LEVEL GetSetItemOptionLevel(int number_of_same_set_items) const;
    // NOTE: f110325.3L, added interface to support fast calculation
    const SET_ITEM_OPTION* GetCurrentSetItemOption(const SETITEM_OPTION_INFO* set_item_option_info,
        POSTYPE pos, eSETITEM_OPTION_LEVEL level) const;
    bool IsFullOptionCount(const SETITEM_OPTION_INFO* set_item_option_info,
        int num_of_set_items) const;

    SETITEM_OPTION_INFO* Find(DWORD set_group_code);
    SET_ITEM_OPTION* GetSetItemOption(DWORD set_group_code, POSTYPE pos, eSETITEM_OPTION_LEVEL level);
    BYTE GetSetItemFullOption(DWORD set_group_code, SET_ITEM_OPTION** ppOption);
    BOOL IsFullOption(DWORD set_group_code, BYTE num_of_set_item);
    void ValidCheck(SETITEM_OPTION_INFO* pInfo);
    //
private:
    SETITEM_OPTION_INFO_MAP m_mapSetItemOptionInfo;
};

//==================================================================================================

inline bool SetItemOptionInfoParser::IsFullOptionCount(
    const SETITEM_OPTION_INFO* set_item_option_info, int num_of_set_items) const
{
    return (set_item_option_info->m_byActiveFullOptionNum == num_of_set_items);
}

#endif //__PROGRAMCOMMON_SETITEMOPTIONINFOPARSER_H
