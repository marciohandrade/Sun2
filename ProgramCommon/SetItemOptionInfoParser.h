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

//세트 아이템 기본 옵션 정보
struct SET_ITEM_OPTION
{
    BYTE m_byOptionKind; //옵션종류
    BYTE m_byUseType;    //수치타입
    int  m_nOption;
};

// 장비 위치에 따른 활성화 옵션
struct EQUIP_ACTIVE_OPTION
{
    POSTYPE         m_EqPos; //장비위치
    //활성화 옵션 (2, 4, 6개 이렇게 3개가 있다.)
    SET_ITEM_OPTION m_ActiveOption[MAX_SETITEM_OPTION_LEVEL];
};

// 특수 장착 아이템 옵션 
struct SPECIAL_EQUIP_ACTIVE_OPTION
{
    POSTYPE         m_EqPos;
    WORD            m_wItemCode; //장착된 특수 아이템
    SET_ITEM_OPTION m_ActiveOption[MAX_SETITEM_SPECIAL_OPTION];
};

//--------------------------------------------------------------------------------------------------

struct SETITEM_OPTION_INFO
{
    WORD  m_wSetGroupCode;          //세트 그룹 코드
    BYTE  m_byItemType;             //아이템타입
    BYTE  m_byActiveFullOptionNum;  //풀옵션 활성화 수

    //장비 위치에 따른 활성화 옵션(장비 장착 위치는 8군데가 있다.)
    EQUIP_ACTIVE_OPTION m_EquipActiveOption[MAX_SETITEM_ACTIVE_EQUIP_OPTION];

    //풀 장비 했을시의  옵션
    SET_ITEM_OPTION     m_FullOption[MAX_SETITEM_FULL_OPTION]; 

#ifdef _YMS_FULLSETITEM_RESOURCE_DIVISION_
#else
    //! 풀셋일경우 아이템을 바꾸는 형식에서 추카적으로 리소스가 붙는 형태로 바면서 없어짐
    DWORD m_FullSetChangeItem;
#endif // _YMS_FULLSETITEM_RESOURCE_DIVISION_

    SPECIAL_EQUIP_ACTIVE_OPTION m_SpecialEquipOptionItem;

#ifdef _YMS_GENDER_FULLSETITEM
    DWORD m_dwEffectCode[MAX_SETITEM_EFFECT_NUM][GenderType::kFemale + 1]; // 풀 옵션 효과코드
    BYTE  m_btBoneIdx[MAX_SETITEM_EFFECT_NUM][GenderType::kFemale + 1];    // 이펙트 붙일 본 인덱스

    DWORD change_armor_resource[GenderType::kFemale + 1];
#else
    DWORD m_dwEffectCode[MAX_SETITEM_EFFECT_NUM]; // 풀 옵션 효과코드
    BYTE  m_btBoneIdx[MAX_SETITEM_EFFECT_NUM];    // 이펙트 붙일 본 인덱스

    DWORD change_armor_resource;      //! 풀셋일경우 추가적으로 붙여줄 리소스
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
