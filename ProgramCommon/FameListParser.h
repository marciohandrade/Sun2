#pragma once

#ifndef PROGRAMCOMMON_HONORSYSTEM_FAMELISTPARSER_H_
#define PROGRAMCOMMON_HONORSYSTEM_FAMELISTPARSER_H_

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>


class FameListParser : public util::Singleton<FameListParser>, public IParser
{
public:
    enum
    {
        kMaxFameRating = 5,
    };

    struct FameInfo 
    {
        WORD fame_code; // 명성코드
        WORD fame_rating_point[kMaxFameRating]; // 명성 단계별 필요점수
        DWORD fame_rating_accumulate_point[kMaxFameRating]; // 명성 단계까지 필요 누적점수
#ifndef _SERVER //if'N'def
        DWORD fame_name_code; // 명성 제목
        DWORD fame_description_code; // 명성 설명
        DWORD fame_effectcode[kMaxFameRating]; // 단계별 달성 이펙트
#endif //_SERVER
    };

    typedef STLX_MAP<WORD, FameInfo*> FameInfoMap;

public:
    FameListParser(void);
	virtual	~FameListParser(void);

    void Init();
	void Init(char* pack_filename);
	void Release();

	virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    FameInfo* GetFameInfo(WORD fame_code);

    FameInfoMap& GetFameInfoContainer() { return fame_info_container_; }

    static int GetFameRating(FameInfo* fame_info, DWORD current_fame_point);

private:
    BOOL _Load(BOOL is_reload);

    FameInfoMap fame_info_container_;
};

#endif //_NA_003027_20111013_HONOR_SYSTEM

#endif //PROGRAMCOMMON_HONORSYSTEM_FAMELISTPARSER_H_