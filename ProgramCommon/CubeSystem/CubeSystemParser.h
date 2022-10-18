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


// 큐브 시스템 스크립트 파서를 구현한 클래스
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
    // 합성 정보를 얻는다.
    CompositionInfo* FindComposition(const CompositionInfo::CompositionCode code) const;    
    
    // 분해할 재료 목록을 얻는다.
    bool GetDecompositionMaterialList(
        SCItemSlotContainer& inventory, 
        const MaterialInventoryPositionList& inventory_position_list, 
        DecompositionInfo::MaterialList& OUT material_list) const;

    // 분해 포인트를 얻는다.
    DecompositionInfo::DecompositionPoint GetDecompositionPoint(
        const DecompositionInfo::MaterialList& material_list) const;

    // 분해 포인트에 해당하는 보상 목록을 얻는다.
    bool GetDecompositionRewardList(
        const DecompositionInfo::DecompositionPoint decomposition_point, 
        DecompositionInfo::RewardList& OUT reward_list) const;
    
public:
    // 합성 또는 분해할 재료 목록으로 서버에 대한 요청을 얻는다.
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
    // 합성 정보 스크립트를 읽는다.
    bool LoadComposition(const bool is_reload);
    
    // 분해 정보 스크립트를 읽는다.
    bool LoadDecomposition(const bool is_reload);

private:
    // 합성 정보를 해제한다.
    void ReleaseComposition();
    
private:
    CompositionMap composition_map_;
    DecompositionInfo::RewardList decomposition_reward_list_;
    __DISABLE_COPY(CubeSystemParser);
}; //}} CubeSystemParser

#endif //}} PROGRAMCOMMON_CUBESYSTEM_CUBESYSTEMPARSER_H_