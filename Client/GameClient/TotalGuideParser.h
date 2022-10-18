////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   TotalGuideParser.h
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2011/06/15 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef TOTALGUIDEPARSER_H_
#define TOTALGUIDEPARSER_H_

#include "ScriptCode.IParser.h"
#include "Singleton.h"

#define MAX_GUIDE_IMAGE_TEXT_SIZE 128
// 통합 가이드 세부 페이지
struct TotalGuidePage
{
    enum { kMaxSize = 50 }; // 생성 가능한 최대 개수
    
    BYTE number; // 페이지 번호
    CODETYPE head_str_code; // 제목 스트링 코드
    CODETYPE content_str_code; // 내용 스트링 코드
    CODETYPE image_code; // 이미지 코드
    CODETYPE hold_window_id; // 홀드창 아이디
    CODETYPE hold_str_code; // 홀드시 출력할 스트링 코드
    CODETYPE highlight_target_id; // 강조할 대상 아이디
    CODETYPE highlight_child_target_id; // 강조할 대상에 속한 자식 객체 아이디

    explicit TotalGuidePage(
        const BYTE number, 
        const CODETYPE head_str_code, 
        const CODETYPE content_str_code, 
        const CODETYPE image_code, 
        const CODETYPE hold_window_id, 
        const CODETYPE hold_str_code, 
        const CODETYPE highlight_target_id, 
        const CODETYPE highlight_child_target_id);
private:
    __DISABLE_COPY(TotalGuidePage);
}; // TotalGuidePage

// 통합 가이드
struct TotalGuide
{
    typedef WORD Index;
    enum { kMaxSize = 200 }; // 생성 가능한 최대 개수
    enum { kInvalid = 0 };
    enum ConfirmType
    {
        kConfirmPerCharacter = 0, // 캐릭터 별
        kConfirmPerUser = 1 // 사용자 계정 별
    };
    
    Index code; // 가이드 코드
    CODETYPE name_str_code; // 가이드 이름 스트링 코드
    LEVELTYPE level; // 가이드 출력 레벨
    BYTE page_size; // 페이지 수
    ConfirmType confirm_type; // 확인 형식
    Index category_code; // 카테고리 코드
    
    explicit TotalGuide(
        const Index code, 
        const CODETYPE name_str_code, 
        const LEVELTYPE level, 
        const BYTE page_size, 
        const ConfirmType confirm_type, 
        const Index category_code);
    ~TotalGuide();

    // 인덱스가 아니라 페이지 번호를 키로 페이지를 얻어온다. (페이지 번호는 1부터 시작)
    const TotalGuidePage* GetPage(const BYTE page_number) const;
    // 페이지를 추가한다. 추가 실패하면 페이지는 삭제된다.
    bool AddPage(const TotalGuidePage*& page);
private:
    const TotalGuidePage* page_list_[TotalGuidePage::kMaxSize];
    __DISABLE_COPY(TotalGuide);
}; // TotalGuide

// 통합 가이드 카테고리
struct TotalGuideCategory
{
    enum { kMaxSize = 100 }; // 생성 가능한 최대 개수
    typedef STLX_MAP<TotalGuide::Index, TotalGuide*> GuideMap;

    TotalGuide::Index code; // 카테고리 코드
    CODETYPE name_str_code; // 카테고리 이름 스트링 코드
    GuideMap guide_map; // 가이드 맵 

    explicit TotalGuideCategory(const TotalGuide::Index code, const CODETYPE name_str_code);
    ~TotalGuideCategory();

    // 가이드를 얻는다.
    TotalGuide* GetGuide(const TotalGuide::Index guide_code) const;
    // 가이드를 추가한다. 추가 실패하면 가이드는 삭제된다.
    bool AddGuide(TotalGuide*& guide);
private:
    __DISABLE_COPY(TotalGuideCategory);
};

// 통합 가이드 카테고리 관리자
struct TotalGuideCategoryManager
{
    typedef STLX_MAP<TotalGuide::Index, TotalGuideCategory*> CategoryMap;
    CategoryMap category_map; // 카테고리 맵

    TotalGuideCategoryManager();
    ~TotalGuideCategoryManager();

    // 카테고리를 얻는다.
    TotalGuideCategory* GetCategory(const TotalGuide::Index category_code) const;
    // 카테고리를 추가한다. 추가 실패하면 카테고리는 삭제된다.
    bool AddCategory(TotalGuideCategory*& category);
    // 초기화 한다.
    void Clear();
private:
    __DISABLE_COPY(TotalGuideCategoryManager);
};

class TotalGuideParser;

// 통합 가이드 알림창
struct TotalGuideNotice
{
    friend class TotalGuideParser;
    enum { kMaxSize = 200 }; // 생성 가능한 최대 개수
    enum ButtonStyle // 버튼 출력 스타일
    {
        kViewAndNextView = 1, // 보기, 다음에 보기 둘다 출력
        kOK = 2, // 확인만 출력
        kView = 3 // 보기만 출력
    };
    enum CharStatus // 캐릭터 상태
    {
        kBoundaryMin = 0, 
        kCreate = 1, // 캐릭터 생성
        kNpcClick = 2, // NCP 클릭
        kEnterToMap = 3, // 특정 맵 진입
        kGetExp = 4, // 경험치 습득
        kGetItem = 5, // 아이템 습득
        kLevelUp = 6, // 레벨업
        kFreeChaos = 7, // 프리카오
        kChaos = 8, // 카오
        kDead = 9, // 캐릭터 사망시
        kCreateGuild = 10, // 길드 창설
        kUnderHP = 11, // HP 특정 퍼센트 이하
        kQuest = 12, // 퀘스트 수락/완료        
        kAccumulatePoint = 14, // 접속자 포인트 획득시
        kRemainGuide = 99, // 보지 않은 가이드 정보가 있는 경우
        kBoundaryMax
    };
    enum RepeatType // 반복 형식
    {
        kOnce = 0, // 한번만
        kContinue = 1, // 반복함
        kOnceForRun = 2, // 1회 출력 후 재접속시까지 반복 안함
    };
    enum OpenType // 창열기 형식
    {
        kDoNotOpen = 0, // 창을 열지 않음
        kMiniWindow = 1, // 미니 정보창 
        kGuideWindow = 2, // 가이드창
        kNoticeWindow = 3, // 알림창
    };
    enum { kParamMaxSize = 2 };

    enum UnderHPType
    {
        kNumbericType = 1,
        kPercentageType = 2,
    };
    enum QuestType
    {
        kAcceptQuest = 1,
        kCompleteQuest = 2,
    };
    enum GetItemType
    {
        kItemCode = 1,      //코드로 구분
        kItemType = 2,      //종류 및 계열로 구분
        kItemStatus = 3,        //디바인(1)/에테리아(2)/스킨(3)
        kItemRareAndElite = 4,  //레어(1)/엘리트(2)

    };
    enum ItemStatusType
    {
        kDivine = 1,        //디바인
        kEtheriaItem = 2,   //에테리아
        kShellItem = 3,   //스킨
    };
    enum ItemRareAndEliteType
    {
        kRare = 1,      //레어 아이템
        kElite = 2,     //엘리트 아이템
    };
    typedef WORD Index;
    Index code; // 알림창 코드
    ButtonStyle button_style; // 버튼 출력 형식
    CharStatus char_status; // 캐릭터 상태
    LEVELTYPE min_level; // 최소 레벨
    LEVELTYPE max_level; // 최대 레벨
    RepeatType repeat_type; // 반복 형식
    OpenType open_type; // 창 열기 형식
    TotalGuide::Index guide_code; // 연결된 가이드 코드
    CODETYPE title_str_code; // 제목 스트링 코드
    const TotalGuide* guide; // 연결된 가이드
    CODETYPE desc_str_code; // 설명 스트링 코드

    explicit TotalGuideNotice(
        const Index code, 
        const ButtonStyle button_style, 
        const CharStatus char_status, 
        const LEVELTYPE min_level, 
        const LEVELTYPE max_level, 
        const RepeatType repeat_type, 
        const OpenType open_type, 
        const TotalGuide::Index guide_code, 
        const CODETYPE title_str_code, 
        const TotalGuide* guide, 
        const DWORD param_list[kParamMaxSize], 
        const CODETYPE desc_str_code);

    // 추가 파라미터를 얻는다.
    DWORD GetParam(const int index) const;

private:
    DWORD param_list_[kParamMaxSize]; // 추가 파라미터 목록
    __DISABLE_COPY(TotalGuideNotice);
};

// 통합 가이드 알림창 관리자
struct TotalGuideNoticeManager
{
    typedef STLX_MAP<TotalGuideNotice::Index, const TotalGuideNotice*> NoticeMap;
    typedef STLX_MULTIMAP<TotalGuideNotice::CharStatus, const TotalGuideNotice*> CharStatusMap;
    
    NoticeMap notice_map; // 알림창 맵
    CharStatusMap char_status_map; // 캐릭터 상태 순으로 정렬된 알림창 맵

    TotalGuideNoticeManager();
    ~TotalGuideNoticeManager();

    // 알림을 얻는다.
    const TotalGuideNotice* GetNotice(const TotalGuideNotice::Index notice_code) const;
    // 알림을 추가한다. 추가 실패하면 알림은 삭제된다.
    bool AddNotice(const TotalGuideNotice*& notice);
    // 초기화 한다.
    void Clear();

private:
    __DISABLE_COPY(TotalGuideNoticeManager);
};

// 통합 가이드 이미지
struct TotalGuideImage
{
    CODETYPE image_code; // 이미지 코드
    TCHAR name_str[MAX_GUIDE_IMAGE_TEXT_SIZE]; // 이름

#ifdef _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
    RECT image_rect;
    explicit TotalGuideImage(const CODETYPE code, TCHAR* name_str, RECT image_rect);
#else
    explicit TotalGuideImage(const CODETYPE code, TCHAR* name_str);
#endif//_NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
    ~TotalGuideImage();

private:
    __DISABLE_COPY(TotalGuideImage);
};

// 통합 가이드 이미지 관리자
struct TotalGuideImageManager
{
    typedef STLX_MAP<CODETYPE, const TotalGuideImage*> GuideImageMap;

    TotalGuideImageManager();
    ~TotalGuideImageManager();

    // 가이드 이미지를 얻는다.
    const TotalGuideImage* GetGuideImage(const CODETYPE image_code) const;
    // 이미지를 추가한다. 추가 실패하면 이미지는 삭제된다.
    bool AddGuideImage(TotalGuideImage*& guide_image);
    // 초기화 한다.
    void Clear();
private:
    GuideImageMap guide_image_map; // 이미지 맵
    __DISABLE_COPY(TotalGuideImageManager);
};

// 정보 열람실 통합 가이드 시스템 파서를 구현한 클래스
class TotalGuideParser : public util::Singleton<TotalGuideParser>, public IParser
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    TotalGuideParser();
    ~TotalGuideParser();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Public
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    virtual void Release() {}
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

public:
    // 카테고리 관리자를 얻는다.
    const TotalGuideCategoryManager& GetCategoryManager() const;
    // 가이드를 얻는다.
    TotalGuide* GetGuide(const TotalGuide::Index guide_code) const;
    // 카테고리를 얻는다.
    const TotalGuideCategory* GetCategory(const TotalGuide::Index category_code) const;
    // 알림창 관리자를 얻는다.
    const TotalGuideNoticeManager& GetNoticeManager() const;
    // 알림을 얻는다.
    const TotalGuideNotice* GetNotice(const TotalGuideNotice::Index notice_code) const;
    // 이미지를 얻는다.
    const TotalGuideImageManager* GetImageManager() const { return &image_manager_; }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    // 통합 가이드 스크립트를 읽어온다.
    bool LoadGuide(const bool is_reload);
    // 통합 가이드 세부 페이지 스크립트를 읽어온다.
    bool LoadGuidePage(const bool is_reload);
    // 통합 가이드 알림창 스크립트를 읽어온다.
    bool LoadNotice(const bool is_reload);
    // 통합 가이드 이미지 스크립트를 읽어온다.
    bool LoadGuideImage(const bool is_reload);

private:
    TotalGuideCategoryManager category_manager_;
    TotalGuideNoticeManager notice_manager_;
    TotalGuideImageManager image_manager_;
    __DISABLE_COPY(TotalGuideParser);

}; // TotalGuideParser

#endif // TOTALGUIDEPARSER_H_