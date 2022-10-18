#pragma once

#ifndef PROGRAMCOMMON_SOCKETSYSTEM_ZARDCOMPOSEINFOPARSER_H_
#define PROGRAMCOMMON_SOCKETSYSTEM_ZARDCOMPOSEINFOPARSER_H_

#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include <ItemOptionHeader.h>

class SCItemSlotContainer;
struct MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN;

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
struct ZardComposeInfo
{
    enum { kSuccessRatioCount = 4 };
    enum { kMaxRatio = 1000 };
    typedef int ZardGrade;
    typedef WORD ZardRatio;

    ZardGrade zard_grade_; // 쟈드 등급
    ZardRatio success_ratio_[kSuccessRatioCount]; // 성공 확률
};

class ZardComposeInfoParser : public util::Singleton<ZardComposeInfoParser>, public IParser
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Define
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    enum { kMaterialSize = 3 }; // 합성에 사용할 재료 개수
    

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    ZardComposeInfoParser();
    ~ZardComposeInfoParser();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Public
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    virtual void Release() {}
    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);
    
public:
    const ZardComposeInfo* FindZardCompose(const ZardComposeInfo::ZardGrade zard_grade) const;

public:
    // 쟈드 합성 성공 확률을 얻는다.
    // 리턴값 : 0이면 성공 확률을 얻는데 실패
    ZardComposeInfo::ZardRatio GetZardCompositionSuccessRatio(
        SCItemSlotContainer& inventory, 
        const POSTYPE zard_position_list[kMaterialSize], 
        const POSTYPE additive_position, 
        int fail_number) const;

    // 쟈드 합성 요청을 얻는다.
    bool GetZardCompositionRequest(
        SCItemSlotContainer& inventory, 
        const POSTYPE zard_position_list[kMaterialSize], 
        const POSTYPE additive_position, 
        MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& OUT request) const;

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    bool IsValid(const ZardComposeInfo& zard_compose) const;
    bool LoadZardCompose(const bool is_reload);
    
private:
    ZardComposeInfo zard_compose_list_[kZardGradeMax-1];
    __DISABLE_COPY(ZardComposeInfoParser);
};
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#endif // PROGRAMCOMMON_SOCKETSYSTEM_ZARDCOMPOSEINFOPARSER_H_