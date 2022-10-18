#pragma once
#ifndef PROGRAMCOMMON_STRENGTH_PENALTY_PARSER_H
#define PROGRAMCOMMON_STRENGTH_PENALTY_PARSER_H

//==================================================================================================
// @history
//  - 08.06.26, lst1024, created
//  - 11.05.06, waverix, refactoring. for further details show the below comment 'NOTE: f110505.3L'

#include <Define.h>
#include <ScriptCode.IParser.h>

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
//==================================================================================================
// NOTE: f110505.3L, this class is used all version except China version.
// '__NA_001062_20080623_STRENGTH_PENALTY' used for version control
// currently, the penalty index is one and is does not look alike to increase
// so that the writer that is 'waverix' change a root data structure.

struct sSTRENGTH_PENALTY
{
    sSTRENGTH_PENALTY(); // dummy
    const float    m_fStrengthPro;
    const int      m_nStrengthUIIndex;
    const COLORREF m_StrengthColorCode;
    //
    __DISABLE_COPY(sSTRENGTH_PENALTY);
};

//==================================================================================================

class StrengthPenaltyParser : public IParser
{
public:
    static const int kMaxNumberOfStrengthPenaltySections = 8;
    struct StrengthPenaltyRecord;
    //
    static StrengthPenaltyParser* Instance();
    static void DestroyInstance(); // old util::Singleton support interface

    //void Init(char * pszPackFileName);
    // IParser interfaces
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);

    sSTRENGTH_PENALTY* GetStrengthPenaltyInfo(int index,
        int max_durability, int cur_durability) const;
    //
private:
    enum { Percent_Min = 0, Percent_Max = 1,Err_Idx = -1 };
    //
    StrengthPenaltyParser();
    virtual ~StrengthPenaltyParser();
    static StrengthPenaltyParser* CreateInstance();
    //
    void Unload();
    bool _Load(BOOL reloaded);
    //
    //void AddStrengthPenalty(StrengthPenaltyRecord* record, int index);
    StrengthPenaltyRecord* FindStrengthPenalty(int index) const;
    //StrengthPenaltyRecord* _NewStrengthPenalty(int index);

    int CalcStrengthToIdx(int max_durability, int cur_durability) const;
    bool CheckValidated(StrengthPenaltyRecord* record);
    //----------------------------------------------------------------------------------------------
    // data fields...
    int number_of_records_;
    StrengthPenaltyRecord* table_; // size is 1

    static StrengthPenaltyParser* instance_static_;
    //
    __DISABLE_COPY(StrengthPenaltyParser);
};

//==================================================================================================
// inlining implements

inline StrengthPenaltyParser* StrengthPenaltyParser::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

#endif

#endif //__PROGRAMCOMMON_STRENGTH_PENALTY_PARSER_H