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

    ZardGrade zard_grade_; // ��� ���
    ZardRatio success_ratio_[kSuccessRatioCount]; // ���� Ȯ��
};

class ZardComposeInfoParser : public util::Singleton<ZardComposeInfoParser>, public IParser
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Define
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    enum { kMaterialSize = 3 }; // �ռ��� ����� ��� ����
    

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
    // ��� �ռ� ���� Ȯ���� ��´�.
    // ���ϰ� : 0�̸� ���� Ȯ���� ��µ� ����
    ZardComposeInfo::ZardRatio GetZardCompositionSuccessRatio(
        SCItemSlotContainer& inventory, 
        const POSTYPE zard_position_list[kMaterialSize], 
        const POSTYPE additive_position, 
        int fail_number) const;

    // ��� �ռ� ��û�� ��´�.
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