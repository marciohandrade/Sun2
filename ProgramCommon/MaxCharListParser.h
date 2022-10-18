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


// ĳ���� �ִ� ��� ���� �ļ��� ������ Ŭ����
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
    // �⺻ ĳ���� ���� ������ ��´�.
    CharSlotSize GetDefaultSlotSize() const
    {
        return default_slot_size_;
    }

    // �ִ� ĳ���� ���� ������ ��´�.
    CharSlotSize GetMaxSlotSize() const
    {
        return max_slot_size_;
    }

    // ĳ���� ���� Ȯ�� ��� ���θ� ��´�.
    bool UseExtension() const
    {
        return use_extension_;
    }

    // Ȯ�� �õ��� �þ�� ĳ���� ���� ������ ��´�.
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