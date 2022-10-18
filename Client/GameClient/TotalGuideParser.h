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
// ���� ���̵� ���� ������
struct TotalGuidePage
{
    enum { kMaxSize = 50 }; // ���� ������ �ִ� ����
    
    BYTE number; // ������ ��ȣ
    CODETYPE head_str_code; // ���� ��Ʈ�� �ڵ�
    CODETYPE content_str_code; // ���� ��Ʈ�� �ڵ�
    CODETYPE image_code; // �̹��� �ڵ�
    CODETYPE hold_window_id; // Ȧ��â ���̵�
    CODETYPE hold_str_code; // Ȧ��� ����� ��Ʈ�� �ڵ�
    CODETYPE highlight_target_id; // ������ ��� ���̵�
    CODETYPE highlight_child_target_id; // ������ ��� ���� �ڽ� ��ü ���̵�

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

// ���� ���̵�
struct TotalGuide
{
    typedef WORD Index;
    enum { kMaxSize = 200 }; // ���� ������ �ִ� ����
    enum { kInvalid = 0 };
    enum ConfirmType
    {
        kConfirmPerCharacter = 0, // ĳ���� ��
        kConfirmPerUser = 1 // ����� ���� ��
    };
    
    Index code; // ���̵� �ڵ�
    CODETYPE name_str_code; // ���̵� �̸� ��Ʈ�� �ڵ�
    LEVELTYPE level; // ���̵� ��� ����
    BYTE page_size; // ������ ��
    ConfirmType confirm_type; // Ȯ�� ����
    Index category_code; // ī�װ� �ڵ�
    
    explicit TotalGuide(
        const Index code, 
        const CODETYPE name_str_code, 
        const LEVELTYPE level, 
        const BYTE page_size, 
        const ConfirmType confirm_type, 
        const Index category_code);
    ~TotalGuide();

    // �ε����� �ƴ϶� ������ ��ȣ�� Ű�� �������� ���´�. (������ ��ȣ�� 1���� ����)
    const TotalGuidePage* GetPage(const BYTE page_number) const;
    // �������� �߰��Ѵ�. �߰� �����ϸ� �������� �����ȴ�.
    bool AddPage(const TotalGuidePage*& page);
private:
    const TotalGuidePage* page_list_[TotalGuidePage::kMaxSize];
    __DISABLE_COPY(TotalGuide);
}; // TotalGuide

// ���� ���̵� ī�װ�
struct TotalGuideCategory
{
    enum { kMaxSize = 100 }; // ���� ������ �ִ� ����
    typedef STLX_MAP<TotalGuide::Index, TotalGuide*> GuideMap;

    TotalGuide::Index code; // ī�װ� �ڵ�
    CODETYPE name_str_code; // ī�װ� �̸� ��Ʈ�� �ڵ�
    GuideMap guide_map; // ���̵� �� 

    explicit TotalGuideCategory(const TotalGuide::Index code, const CODETYPE name_str_code);
    ~TotalGuideCategory();

    // ���̵带 ��´�.
    TotalGuide* GetGuide(const TotalGuide::Index guide_code) const;
    // ���̵带 �߰��Ѵ�. �߰� �����ϸ� ���̵�� �����ȴ�.
    bool AddGuide(TotalGuide*& guide);
private:
    __DISABLE_COPY(TotalGuideCategory);
};

// ���� ���̵� ī�װ� ������
struct TotalGuideCategoryManager
{
    typedef STLX_MAP<TotalGuide::Index, TotalGuideCategory*> CategoryMap;
    CategoryMap category_map; // ī�װ� ��

    TotalGuideCategoryManager();
    ~TotalGuideCategoryManager();

    // ī�װ��� ��´�.
    TotalGuideCategory* GetCategory(const TotalGuide::Index category_code) const;
    // ī�װ��� �߰��Ѵ�. �߰� �����ϸ� ī�װ��� �����ȴ�.
    bool AddCategory(TotalGuideCategory*& category);
    // �ʱ�ȭ �Ѵ�.
    void Clear();
private:
    __DISABLE_COPY(TotalGuideCategoryManager);
};

class TotalGuideParser;

// ���� ���̵� �˸�â
struct TotalGuideNotice
{
    friend class TotalGuideParser;
    enum { kMaxSize = 200 }; // ���� ������ �ִ� ����
    enum ButtonStyle // ��ư ��� ��Ÿ��
    {
        kViewAndNextView = 1, // ����, ������ ���� �Ѵ� ���
        kOK = 2, // Ȯ�θ� ���
        kView = 3 // ���⸸ ���
    };
    enum CharStatus // ĳ���� ����
    {
        kBoundaryMin = 0, 
        kCreate = 1, // ĳ���� ����
        kNpcClick = 2, // NCP Ŭ��
        kEnterToMap = 3, // Ư�� �� ����
        kGetExp = 4, // ����ġ ����
        kGetItem = 5, // ������ ����
        kLevelUp = 6, // ������
        kFreeChaos = 7, // ����ī��
        kChaos = 8, // ī��
        kDead = 9, // ĳ���� �����
        kCreateGuild = 10, // ��� â��
        kUnderHP = 11, // HP Ư�� �ۼ�Ʈ ����
        kQuest = 12, // ����Ʈ ����/�Ϸ�        
        kAccumulatePoint = 14, // ������ ����Ʈ ȹ���
        kRemainGuide = 99, // ���� ���� ���̵� ������ �ִ� ���
        kBoundaryMax
    };
    enum RepeatType // �ݺ� ����
    {
        kOnce = 0, // �ѹ���
        kContinue = 1, // �ݺ���
        kOnceForRun = 2, // 1ȸ ��� �� �����ӽñ��� �ݺ� ����
    };
    enum OpenType // â���� ����
    {
        kDoNotOpen = 0, // â�� ���� ����
        kMiniWindow = 1, // �̴� ����â 
        kGuideWindow = 2, // ���̵�â
        kNoticeWindow = 3, // �˸�â
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
        kItemCode = 1,      //�ڵ�� ����
        kItemType = 2,      //���� �� �迭�� ����
        kItemStatus = 3,        //�����(1)/���׸���(2)/��Ų(3)
        kItemRareAndElite = 4,  //����(1)/����Ʈ(2)

    };
    enum ItemStatusType
    {
        kDivine = 1,        //�����
        kEtheriaItem = 2,   //���׸���
        kShellItem = 3,   //��Ų
    };
    enum ItemRareAndEliteType
    {
        kRare = 1,      //���� ������
        kElite = 2,     //����Ʈ ������
    };
    typedef WORD Index;
    Index code; // �˸�â �ڵ�
    ButtonStyle button_style; // ��ư ��� ����
    CharStatus char_status; // ĳ���� ����
    LEVELTYPE min_level; // �ּ� ����
    LEVELTYPE max_level; // �ִ� ����
    RepeatType repeat_type; // �ݺ� ����
    OpenType open_type; // â ���� ����
    TotalGuide::Index guide_code; // ����� ���̵� �ڵ�
    CODETYPE title_str_code; // ���� ��Ʈ�� �ڵ�
    const TotalGuide* guide; // ����� ���̵�
    CODETYPE desc_str_code; // ���� ��Ʈ�� �ڵ�

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

    // �߰� �Ķ���͸� ��´�.
    DWORD GetParam(const int index) const;

private:
    DWORD param_list_[kParamMaxSize]; // �߰� �Ķ���� ���
    __DISABLE_COPY(TotalGuideNotice);
};

// ���� ���̵� �˸�â ������
struct TotalGuideNoticeManager
{
    typedef STLX_MAP<TotalGuideNotice::Index, const TotalGuideNotice*> NoticeMap;
    typedef STLX_MULTIMAP<TotalGuideNotice::CharStatus, const TotalGuideNotice*> CharStatusMap;
    
    NoticeMap notice_map; // �˸�â ��
    CharStatusMap char_status_map; // ĳ���� ���� ������ ���ĵ� �˸�â ��

    TotalGuideNoticeManager();
    ~TotalGuideNoticeManager();

    // �˸��� ��´�.
    const TotalGuideNotice* GetNotice(const TotalGuideNotice::Index notice_code) const;
    // �˸��� �߰��Ѵ�. �߰� �����ϸ� �˸��� �����ȴ�.
    bool AddNotice(const TotalGuideNotice*& notice);
    // �ʱ�ȭ �Ѵ�.
    void Clear();

private:
    __DISABLE_COPY(TotalGuideNoticeManager);
};

// ���� ���̵� �̹���
struct TotalGuideImage
{
    CODETYPE image_code; // �̹��� �ڵ�
    TCHAR name_str[MAX_GUIDE_IMAGE_TEXT_SIZE]; // �̸�

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

// ���� ���̵� �̹��� ������
struct TotalGuideImageManager
{
    typedef STLX_MAP<CODETYPE, const TotalGuideImage*> GuideImageMap;

    TotalGuideImageManager();
    ~TotalGuideImageManager();

    // ���̵� �̹����� ��´�.
    const TotalGuideImage* GetGuideImage(const CODETYPE image_code) const;
    // �̹����� �߰��Ѵ�. �߰� �����ϸ� �̹����� �����ȴ�.
    bool AddGuideImage(TotalGuideImage*& guide_image);
    // �ʱ�ȭ �Ѵ�.
    void Clear();
private:
    GuideImageMap guide_image_map; // �̹��� ��
    __DISABLE_COPY(TotalGuideImageManager);
};

// ���� ������ ���� ���̵� �ý��� �ļ��� ������ Ŭ����
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
    // ī�װ� �����ڸ� ��´�.
    const TotalGuideCategoryManager& GetCategoryManager() const;
    // ���̵带 ��´�.
    TotalGuide* GetGuide(const TotalGuide::Index guide_code) const;
    // ī�װ��� ��´�.
    const TotalGuideCategory* GetCategory(const TotalGuide::Index category_code) const;
    // �˸�â �����ڸ� ��´�.
    const TotalGuideNoticeManager& GetNoticeManager() const;
    // �˸��� ��´�.
    const TotalGuideNotice* GetNotice(const TotalGuideNotice::Index notice_code) const;
    // �̹����� ��´�.
    const TotalGuideImageManager* GetImageManager() const { return &image_manager_; }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    // ���� ���̵� ��ũ��Ʈ�� �о�´�.
    bool LoadGuide(const bool is_reload);
    // ���� ���̵� ���� ������ ��ũ��Ʈ�� �о�´�.
    bool LoadGuidePage(const bool is_reload);
    // ���� ���̵� �˸�â ��ũ��Ʈ�� �о�´�.
    bool LoadNotice(const bool is_reload);
    // ���� ���̵� �̹��� ��ũ��Ʈ�� �о�´�.
    bool LoadGuideImage(const bool is_reload);

private:
    TotalGuideCategoryManager category_manager_;
    TotalGuideNoticeManager notice_manager_;
    TotalGuideImageManager image_manager_;
    __DISABLE_COPY(TotalGuideParser);

}; // TotalGuideParser

#endif // TOTALGUIDEPARSER_H_