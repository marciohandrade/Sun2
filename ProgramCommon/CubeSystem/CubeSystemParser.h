////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   CubeSystemParser.h
/// @author hukim
/// @brief  [2010/08/24 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef PROGRAMCOMMON_CUBESYSTEM_CUBESYSTEMPARSER_H_
#define PROGRAMCOMMON_CUBESYSTEM_CUBESYSTEMPARSER_H_

#include "ScriptCode.IParser.h"
#include "Singleton.h"
#include "CubeStruct.h"

class SCItemSlot;
class SCItemSlotContainer;
struct MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN;


// ť�� �ý��� ��ũ��Ʈ �ļ��� ������ Ŭ����
class CubeSystemParser : public util::Singleton<CubeSystemParser>, public IParser
{           
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    explicit CubeSystemParser();
    ~CubeSystemParser();

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Public
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

public:    
    // �ռ� ������ ��´�.
    CompositionInfo* FindComposition(const CompositionInfo::CompositionCode code) const;    
    
    // ������ ��� ����� ��´�.
    bool GetDecompositionMaterialList(
        SCItemSlotContainer& inventory, 
        const MaterialInventoryPositionList& inventory_position_list, 
        DecompositionInfo::MaterialList& OUT material_list) const;

    // ���� ����Ʈ�� ��´�.
    DecompositionInfo::DecompositionPoint GetDecompositionPoint(
        const DecompositionInfo::MaterialList& material_list) const;

    // ���� ����Ʈ�� �ش��ϴ� ���� ����� ��´�.
    bool GetDecompositionRewardList(
        const DecompositionInfo::DecompositionPoint decomposition_point, 
        DecompositionInfo::RewardList& OUT reward_list) const;
    
public:
    // �ռ� �Ǵ� ������ ��� ������� ������ ���� ��û�� ��´�.
    bool GetRequest(
        SCItemSlotContainer& inventory, 
        const bool is_composition, 
        const MaterialInventoryPositionList& inventory_position_list, 
        const POSTYPE using_item_position, 
        MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& OUT request) const;

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    // �ռ� ���� ��ũ��Ʈ�� �д´�.
    bool LoadComposition(const bool is_reload);
    
    // ���� ���� ��ũ��Ʈ�� �д´�.
    bool LoadDecomposition(const bool is_reload);

private:
    // �ռ� ������ �����Ѵ�.
    void ReleaseComposition();
    
private:
    CompositionMap composition_map_;
    DecompositionInfo::RewardList decomposition_reward_list_;
    __DISABLE_COPY(CubeSystemParser);
}; //}} CubeSystemParser

#endif //}} PROGRAMCOMMON_CUBESYSTEM_CUBESYSTEMPARSER_H_