#ifndef __ACHIEVEMENT_ACHIEVEMENTSEARCH_H
#define __ACHIEVEMENT_ACHIEVEMENTSEARCH_H
#pragma once

#include ".\achievementparser.h"

struct Achievement_SearchInfo
{
	BYTE index_;
	Achievement_ParserInfo* parser_info_;
};

class AchievementSearch : public util::Singleton<AchievementSearch>
{
	typedef STLX_HASH_MAP<WORD, Achievement_SearchInfo>	 ACHIEVEMENT_INFO_MAP;    
	typedef ACHIEVEMENT_INFO_MAP::iterator				 ACHIEVEMENT_INFO_ITER;
	typedef std::pair<ACHIEVEMENT_INFO_ITER, bool>		 ACHIEVEMENT_INFO_RESULT;

    typedef STLX_HASH_MAP<DWORD, Achievement_SearchInfo>        ACHIEVEMENT_ACCOMPLISHMENT_MAP;    
    typedef ACHIEVEMENT_ACCOMPLISHMENT_MAP::iterator			ACHIEVEMENT_ACCOMPLISHMENT_ITER;
    typedef std::pair<ACHIEVEMENT_ACCOMPLISHMENT_ITER, bool>	ACHIEVEMENT_ACCOMPLISHMENT_RESULT;

	typedef STLX_VECTOR<Achievement_SearchInfo>			 ACHIEVEMENT_INFO_VECTOR;    
	typedef ACHIEVEMENT_INFO_VECTOR::iterator			 ACHIEVEMENT_INFO_VECTOR_ITER;

public:
    AchievementSearch(void);
	~AchievementSearch(void);

    void AddAchievementParserInfo(Achievement_ParserInfo* parser_info);
	Achievement_SearchInfo* GetAchievement_SearchInfo(WORD object_key);
    Achievement_SearchInfo* GetAccomplishment_SearchInfo(DWORD object_key);

	void SetFirst();
	bool GetNext(Achievement_SearchInfo** search_info);

#ifdef _NA_001990_ACSYSTEM_ADD
    typedef STLX_HASH_MAP<WORD, ACHIEVEMENT_INFO_VECTOR>                            ACHIEVEMENT_INFO_MAP_OF_VECTOR;

    typedef ACHIEVEMENT_INFO_MAP_OF_VECTOR    ACMapElem;
    ACMapElem* GetSearchInfo_find(ACHIEVEMENT_OBJECT_TYPE object_type);
    
    typedef ACHIEVEMENT_INFO_VECTOR ACVectorElem;
    ACVectorElem* GetSearchInfoVector_find(ACHIEVEMENT_OBJECT_TYPE object_type, WORD object_key);
#endif //_NA_001990_ACSYSTEM_ADD

private:    
	ACHIEVEMENT_INFO_MAP achievement_infos_;
    ACHIEVEMENT_ACCOMPLISHMENT_MAP achievement_accomplishment_;       //보상을 위한 컨테이너 (디비 컨버팅 작업에 한시적으로 사용한다.)
	ACHIEVEMENT_INFO_VECTOR achievement_cursetower_;        //저탑 을 위한 컨테이너 (디비 컨버팅 작업에 한시적으로 사용한다.)

	ACHIEVEMENT_INFO_VECTOR_ITER achievement_cursetower_iter_;

#ifdef _NA_001990_ACSYSTEM_ADD
    typedef STLX_HASH_MAP<ACHIEVEMENT_OBJECT_TYPE, ACHIEVEMENT_INFO_MAP_OF_VECTOR>  ACHIEVEMENT_INFO_RACK_OF_VECTOR;
    ACHIEVEMENT_INFO_RACK_OF_VECTOR achievement_info_rack_of_vector_;
#endif //_NA_001990_ACSYSTEM_ADD
};

#endif //__ACHIEVEMENT_ACHIEVEMENTSEARCH_H
