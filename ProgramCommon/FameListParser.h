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
        WORD fame_code; // ���ڵ�
        WORD fame_rating_point[kMaxFameRating]; // �� �ܰ躰 �ʿ�����
        DWORD fame_rating_accumulate_point[kMaxFameRating]; // �� �ܰ���� �ʿ� ��������
#ifndef _SERVER //if'N'def
        DWORD fame_name_code; // �� ����
        DWORD fame_description_code; // �� ����
        DWORD fame_effectcode[kMaxFameRating]; // �ܰ躰 �޼� ����Ʈ
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