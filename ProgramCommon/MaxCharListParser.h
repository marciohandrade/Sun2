////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   MaxCharListParser.h
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2011/05/04 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef PROGRAMCOMMON_MAXCHARLISTPARSER_H_
#define PROGRAMCOMMON_MAXCHARLISTPARSER_H_

#ifdef _USE_MAX_CHAR_LIST_PARSER

#include "ScriptCode.IParser.h"
#include "Singleton.h"


// 캐릭터 최대 목록 설정 파서를 구현한 클래스
class MaxCharListParser : public util::Singleton<MaxCharListParser>, public IParser
{   
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Define
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    typedef unsigned char CharSlotSize;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    MaxCharListParser();
    ~MaxCharListParser();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Public
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    virtual void Release() {}
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

public:
    // 기본 캐릭터 슬롯 개수를 얻는다.
    CharSlotSize GetDefaultSlotSize() const
    {
        return default_slot_size_;
    }

    // 최대 캐릭터 슬롯 개수를 얻는다.
    CharSlotSize GetMaxSlotSize() const
    {
        return max_slot_size_;
    }

    // 캐릭터 슬롯 확장 사용 여부를 얻는다.
    bool UseExtension() const
    {
        return use_extension_;
    }

    // 확장 시도시 늘어나는 캐릭터 슬롯 개수를 얻는다.
    CharSlotSize GetSlotSizePerExtension() const
    {
        return slot_size_per_extension_;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    bool Load(const bool is_reload);

private:
    CharSlotSize default_slot_size_;
    CharSlotSize max_slot_size_;
    bool use_extension_;
    CharSlotSize slot_size_per_extension_;

private:
    __DISABLE_COPY(MaxCharListParser);

}; //}} MaxCharListParser

#endif // _USE_MAX_CHAR_LIST_PARSER

#endif //}} PROGRAMCOMMON_MAXCHARLISTPARSER_H_