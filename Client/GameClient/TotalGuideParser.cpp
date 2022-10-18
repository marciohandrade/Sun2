////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   TotalGuideParser.cpp
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2011/06/15 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SunClientPrecompiledHeader.h"
#include "TotalGuideParser.h" // header
#include "GlobalFunc.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : TotalGuidePage
////////////////////////////////////////////////////////////////////////////////////////////////////

TotalGuidePage::TotalGuidePage(
    const BYTE _number, 
    const CODETYPE _head_str_code, 
    const CODETYPE _content_str_code, 
    const CODETYPE _image_code, 
    const CODETYPE _hold_window_id, 
    const CODETYPE _hold_str_code, 
    const CODETYPE _highlight_target_id, 
    const CODETYPE _highlight_child_target_id) : 
    number(_number), 
    head_str_code(_head_str_code), 
    content_str_code(_content_str_code), 
    image_code(_image_code), 
    hold_window_id(_hold_window_id), 
    hold_str_code(_hold_str_code), 
    highlight_target_id(_highlight_target_id), 
    highlight_child_target_id(_highlight_child_target_id)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : TotalGuide
////////////////////////////////////////////////////////////////////////////////////////////////////

TotalGuide::TotalGuide(
    const Index _code, 
    const CODETYPE _name_str_code, 
    const LEVELTYPE _level, 
    const BYTE _page_size, 
    const ConfirmType _confirm_type, 
    const Index _category_code) : 
    code(_code), 
    name_str_code(_name_str_code), 
    level(_level), 
    page_size(_page_size), 
    confirm_type(_confirm_type), 
    category_code(_category_code)
{
    ::ZeroMemory(page_list_, sizeof(page_list_));
}

TotalGuide::~TotalGuide()
{
    for (BYTE i = 0; i < page_size; ++i)
    {
        const TotalGuidePage* page = page_list_[i];
        delete page;
    }
}

const TotalGuidePage* TotalGuide::GetPage(const BYTE page_number) const
{
    if (page_number > page_size || page_number > _countof(page_list_))
    {
        return NULL;
    }
    return page_list_[page_number-1];
}

bool TotalGuide::AddPage(const TotalGuidePage*& page)
{
    if (page == NULL)
    {
        return false;
    }
    if (page->number > page_size || 
        page->number > _countof(page_list_) || 
        page_list_[page->number - 1] != NULL)
    {
        delete page;
        page = NULL;
        return false;
    }
    page_list_[page->number - 1] = page;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : TotalGuideCategory
////////////////////////////////////////////////////////////////////////////////////////////////////

TotalGuideCategory::TotalGuideCategory(const TotalGuide::Index _code, const CODETYPE _name_str_code) : 
    code(_code), name_str_code(_name_str_code)
{
}

TotalGuideCategory::~TotalGuideCategory()
{
    for (GuideMap::iterator it = guide_map.begin(); it != guide_map.end(); ++it)
    {
        TotalGuide* guide = it->second;
        delete guide;
    }
}

TotalGuide* TotalGuideCategory::GetGuide(const TotalGuide::Index guide_code) const
{
    GuideMap::const_iterator it = guide_map.find(guide_code);
    if (it != guide_map.end())
    {
        return it->second;
    }
    return NULL;
}

bool TotalGuideCategory::AddGuide(TotalGuide*& guide)
{
    if (guide == NULL)
    {
        return false;
    }
    if (guide_map.size() >= TotalGuide::kMaxSize || GetGuide(guide->code) != NULL)
    {
        delete guide;
        return false;
    }
    guide_map[guide->code] = guide;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : TotalGuideCategoryManager
////////////////////////////////////////////////////////////////////////////////////////////////////

TotalGuideCategoryManager::TotalGuideCategoryManager()
{
}

TotalGuideCategoryManager::~TotalGuideCategoryManager()
{
    Clear();
}

TotalGuideCategory* TotalGuideCategoryManager::GetCategory(const TotalGuide::Index category_code) const
{
    CategoryMap::const_iterator it = category_map.find(category_code);
    if (it != category_map.end())
    {
        return it->second;
    }
    return NULL;
}

bool TotalGuideCategoryManager::AddCategory(TotalGuideCategory*& category)
{
    if (category == NULL)
    {
        return false;
    }
    if (category_map.size() >= TotalGuideCategory::kMaxSize || GetCategory(category->code) != NULL)
    {
        delete category;
        return false;
    }
    category_map[category->code] = category;
    return true;
}

void TotalGuideCategoryManager::Clear()
{
    for (CategoryMap::iterator it = category_map.begin(); it != category_map.end(); ++it)
    {
        TotalGuideCategory* category = it->second;
        delete category;
    }
    category_map.clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : TotalGuideNotice
////////////////////////////////////////////////////////////////////////////////////////////////////

TotalGuideNotice::TotalGuideNotice( 
    const Index _code, 
    const ButtonStyle _button_style, 
    const CharStatus _char_status, 
    const LEVELTYPE _min_level, 
    const LEVELTYPE _max_level, 
    const RepeatType _repeat_type, 
    const OpenType _open_type, 
    const TotalGuide::Index _guide_code, 
    const CODETYPE _title_str_code, 
    const TotalGuide* _guide, 
    const DWORD _param_list[kParamMaxSize], 
    const CODETYPE _desc_str_code_) : 

    code(_code), 
    button_style(_button_style), 
    char_status(_char_status), 
    min_level(_min_level), 
    max_level(_max_level), 
    repeat_type(_repeat_type), 
    open_type(_open_type), 
    guide_code(_guide_code), 
    title_str_code(_title_str_code), 
    guide(_guide),
    desc_str_code(_desc_str_code_)
{
    BOOST_STATIC_ASSERT(sizeof(param_list_[0]) == sizeof(_param_list[0]));

    // ������ �ܺο��� �̸� �����ȴ�.
    ::CopyMemory(param_list_, _param_list, sizeof(param_list_));
}

DWORD TotalGuideNotice::GetParam(const int index) const
{
    if (index < 0 || index >= _countof(param_list_))
    {
        return 0;
    }
    return param_list_[index];
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : TotalGuideNoticeManager
////////////////////////////////////////////////////////////////////////////////////////////////////

TotalGuideNoticeManager::TotalGuideNoticeManager()
{
}

TotalGuideNoticeManager::~TotalGuideNoticeManager()
{
    Clear();
}

const TotalGuideNotice* TotalGuideNoticeManager::GetNotice(const TotalGuideNotice::Index notice_code) const
{
    NoticeMap::const_iterator it = notice_map.find(notice_code);
    if (it != notice_map.end())
    {
        return it->second;
    }
    return NULL;
}

bool TotalGuideNoticeManager::AddNotice(const TotalGuideNotice*& notice)
{
    if (notice == NULL)
    {
        return false;
    }
    if (notice_map.size() >= TotalGuideNotice::kMaxSize || GetNotice(notice->code) != NULL)
    {
        delete notice;
        return false;
    }
    notice_map[notice->code] = notice;
    char_status_map.insert(CharStatusMap::value_type(notice->char_status, notice));
    return true;
}

void TotalGuideNoticeManager::Clear()
{
    char_status_map.clear();
    for (NoticeMap::iterator it = notice_map.begin(); it != notice_map.end(); ++it)
    {
        const TotalGuideNotice* notice = it->second;
        delete notice;
    }
    notice_map.clear();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : TotalGuideImage
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
TotalGuideImage::TotalGuideImage(const CODETYPE _code, TCHAR* _name_str, RECT _rect) : 
#else
TotalGuideImage::TotalGuideImage(const CODETYPE _code, TCHAR* _name_str) : 
#endif//_NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
image_code(_code)
{
    StrnCopy(name_str,_name_str,MAX_GUIDE_IMAGE_TEXT_SIZE);
#ifdef _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
    image_rect = _rect;
#endif//_NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
}

TotalGuideImage::~TotalGuideImage()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : TotalGuideImageManager
////////////////////////////////////////////////////////////////////////////////////////////////////

TotalGuideImageManager::TotalGuideImageManager()
{

}

TotalGuideImageManager::~TotalGuideImageManager()
{
    Clear();
}

const TotalGuideImage* TotalGuideImageManager::GetGuideImage(const CODETYPE image_code) const
{
    GuideImageMap::const_iterator cosnt_itr = guide_image_map.find(image_code);
    if (cosnt_itr != guide_image_map.end())
    {
        return cosnt_itr->second;
    }
    return NULL;
}

bool TotalGuideImageManager::AddGuideImage(TotalGuideImage*& guide_image)
{
    if (guide_image == NULL)
    {
        return false;
    }
    if (guide_image->name_str == NULL)   
    {
        return false;
    }

    guide_image_map.insert(GuideImageMap::value_type(guide_image->image_code, guide_image));
    return true;
}

void TotalGuideImageManager::Clear()
{
    for (GuideImageMap::iterator it = guide_image_map.begin(); it != guide_image_map.end(); ++it)
    {
        const TotalGuideImage* guide_image = it->second;
        delete guide_image;
    }
    guide_image_map.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

TotalGuideParser::TotalGuideParser()
{
}

TotalGuideParser::~TotalGuideParser()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL TotalGuideParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    switch (script_code)
    {
    case SCRIPT_TOTAL_GUIDE:
        if (LoadGuide(is_reload == TRUE))
        {
            return TRUE;
        }
        break;
    case SCRIPT_TOTAL_GUIDE_PAGE:
        if (LoadGuidePage(is_reload == TRUE))
        {
            return TRUE;
        }
        break;
    case SCRIPT_TOTAL_GUIDE_NOTICE:
        if (LoadNotice(is_reload == TRUE))
        {
            return TRUE;
        }
        break;
    case SCRIPT_TOTAL_GUIDE_IMAGE:
        if (LoadGuideImage(is_reload == TRUE))
        {
            return TRUE;
        }
        break;
    default:
        FASSERT(false && _T("��ũ��Ʈ �����Ϳ� ������ �ֽ��ϴ�."));
    }
    return FALSE;
}

const TotalGuideCategoryManager& TotalGuideParser::GetCategoryManager() const
{
    return category_manager_;
}

TotalGuide* TotalGuideParser::GetGuide(const TotalGuide::Index guide_code) const
{
    const TotalGuideCategoryManager::CategoryMap& category_map = category_manager_.category_map;
    TotalGuideCategoryManager::CategoryMap::const_iterator it = category_map.begin();
    for (; it != category_map.end(); ++it)
    {
        const TotalGuideCategory* const category = it->second;
        if (category != NULL)
        {
            TotalGuide* guide = category->GetGuide(guide_code);
            if (guide != NULL)
            {
                return guide;
            }
        }
    }
    return NULL;
}

const TotalGuideCategory* TotalGuideParser::GetCategory(const TotalGuide::Index category_code) const
{
    return category_manager_.GetCategory(category_code);
}

const TotalGuideNoticeManager& TotalGuideParser::GetNoticeManager() const
{
    return notice_manager_;
}

const TotalGuideNotice* TotalGuideParser::GetNotice(const TotalGuideNotice::Index notice_code) const
{
    return notice_manager_.GetNotice(notice_code);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Private
////////////////////////////////////////////////////////////////////////////////////////////////////

bool TotalGuideParser::LoadGuide(const bool is_reload)
{
    if (is_reload)
    {
        category_manager_.Clear();
    }
    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        const TotalGuide::Index guide_code = GetDataWORD(_T("Code"), row);
        if (guide_code == TotalGuide::kInvalid)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|���̵� �ڵ尡 �ǹٸ��� �ʽ��ϴ�.|������ = %u, ���̵� = %u|"), 
                   __FUNCTION__, row, guide_code);
            continue;
        }
        const CODETYPE guide_name_str_code = GetDataDWORD(_T("Ncode"), row);
        const LEVELTYPE guide_level = GetDataWORD(_T("Level"), row);
        const TotalGuide::Index category_code = GetDataWORD(_T("Category"), row);
        const CODETYPE category_name_str_code = GetDataDWORD(_T("CategoryText"), row);
        const BYTE guide_page_size = GetDataBYTE(_T("EndPage"), row);
        if (guide_page_size > TotalGuidePage::kMaxSize)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�� �������� �ִ� ������ �ʰ� �߽��ϴ�.|������ = %u, �������� = %u, �ִ������� = %u|"), 
                   __FUNCTION__, row, guide_page_size, TotalGuidePage::kMaxSize);
            continue;
        }
        const int temp_confirm_type = GetDataInt(_T("RCCondition"), row);
        if (temp_confirm_type != TotalGuide::kConfirmPerCharacter && 
            temp_confirm_type != TotalGuide::kConfirmPerUser)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�� �� ���� ���� Ȯ�� �����Դϴ�.|������ = %u, Ȯ������ = %d|"), 
                   __FUNCTION__, row, temp_confirm_type);
            continue;
        }
        const TotalGuide::ConfirmType confirm_type = 
            static_cast<const TotalGuide::ConfirmType>(temp_confirm_type);
        TotalGuideCategory* category = category_manager_.GetCategory(category_code);
        if (category == NULL)
        {
            category = new TotalGuideCategory(category_code, category_name_str_code);
            if (category_manager_.AddCategory(category) == false)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|��з��� �߰��ϴµ� ���� �߽��ϴ�.|������ = %u, ��з� = %u|"), 
                       __FUNCTION__, row, category_code);
                continue;
            }
        }
        TotalGuide* guide = category->GetGuide(guide_code);
        if (guide != NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��з��ȿ� ���� ���̵尡 �̹� �����մϴ�.|������ = %u, ��з� = %u, ���̵� = %u|"), 
                   __FUNCTION__, row, category_code, guide_code);
            continue;
        }
        guide = new TotalGuide(
            guide_code, guide_name_str_code, guide_level, guide_page_size, confirm_type, category_code);
        if (category->AddGuide(guide) == false)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��з��� ���̵带 �߰��ϴµ� ���� �߽��ϴ�.|������ = %u, ��з� = %u, ���̵� = %u|"), 
                   __FUNCTION__, row, category_code, guide_code);
            continue;
        }
    }
    return true;
}

bool TotalGuideParser::LoadGuidePage(const bool is_reload)
{
    __UNUSED(is_reload);
    TCHAR temp_string[sizeof(DWORD)+1] = {0,};

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        const TotalGuide::Index guide_code = GetDataWORD(_T("Code"), row);
        TotalGuide* guide = GetGuide(guide_code);
        if (guide == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�������� �߰��� ���̵尡 �������� �ʽ��ϴ�.|������ = %u, ���̵� = %u|"), 
                   __FUNCTION__, row, guide_code);
            continue;
        }
        const BYTE number = GetDataBYTE(_T("Pages"), row);
        if (number > guide->page_size)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�ִ� �������� �ʰ� �߽��ϴ�.|������ = %u, ������ = %u, �ִ� ������ = %u|"), 
                   __FUNCTION__, row, number, guide->page_size);
            continue;
        }
        const TotalGuidePage* page = guide->GetPage(number);
        if (page != NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�̹� �߰� �Ǿ� �ִ� ������ �Դϴ�.|������ = %u, ���̵� = %u, ������ = %u|"), 
                   __FUNCTION__, row, guide_code, number);
            continue;
        }
        const CODETYPE head_str_code = GetDataDWORD(_T("HeadText"), row);
        const CODETYPE content_str_code = GetDataDWORD(_T("Text"), row);
        const CODETYPE image_code = GetDataDWORD(_T("Image"), row);
        
        Sprintf(temp_string, GetDataString(_T("HoldWindowID"), row));
        if (strlen(temp_string) != 4 && atoi(temp_string) != 0)
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|���̵忡 HoldWindowID �߰��ϴµ� ���� �߽��ϴ�.(%s) |������ = %u, ���̵� = %u, ������ = %u|"), 
                __FUNCTION__, temp_string, row, guide_code, number);
            continue;
        }
        const CODETYPE hold_window_id = (atoi(temp_string))?StrToWzID(temp_string):0;
        const CODETYPE hold_str_code = GetDataDWORD(_T("HoldText"), row);

        Sprintf(temp_string, GetDataString(_T("TLineID"), row));

        if (strlen(temp_string) != 4 && atoi(temp_string) != 0)
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|���̵忡 TLineID �߰��ϴµ� ���� �߽��ϴ�.(%s) |������ = %u, ���̵� = %u, ������ = %u|"), 
                __FUNCTION__, temp_string, row, guide_code, number);
            continue;
        }
        const CODETYPE highlight_target_id = (atoi(temp_string))?StrToWzID(temp_string):0;

        Sprintf(temp_string, GetDataString(_T("TLineControl"), row));
        if (strlen(temp_string) != 4 && atoi(temp_string) != 0)
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|���̵忡 TLineControl �߰��ϴµ� ���� �߽��ϴ�.(%s) |������ = %u, ���̵� = %u, ������ = %u|"), 
                __FUNCTION__, temp_string, row, guide_code, number);
            continue;
        }
        const CODETYPE highlight_child_target_id = (strcmp(temp_string, "0"))?StrToWzID(temp_string):0;

        page = new TotalGuidePage(
            number, 
            head_str_code, 
            content_str_code, 
            image_code, hold_window_id, 
            hold_str_code, 
            highlight_target_id, 
            highlight_child_target_id);
        if (guide->AddPage(page) == false)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|���̵忡 �������� �߰��ϴµ� ���� �߽��ϴ�.|������ = %u, ���̵� = %u, ������ = %u|"), 
                   __FUNCTION__, row, guide_code, number);
            continue;
        }
    }
    return true;
}

bool TotalGuideParser::LoadNotice(const bool is_reload)
{
    if (is_reload)
    {
        notice_manager_.Clear();
    }
    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        const TotalGuide::Index notice_code = GetDataWORD(_T("HelpIndex"), row);
        const TotalGuideNotice* notice = notice_manager_.GetNotice(notice_code);
        if (notice != NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�̹� �߰��Ǿ� �ִ� �˸� �Դϴ�.|������ = %u, �˸� = %u|"), 
                   __FUNCTION__, row, notice_code);
            continue;
        }
        const BYTE temp_button_style = GetDataBYTE(_T("HelpMethod"), row);
        if (temp_button_style != TotalGuideNotice::kViewAndNextView && 
            temp_button_style != TotalGuideNotice::kOK &&
            temp_button_style != TotalGuideNotice::kView)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�˼� ���� ��ư���� �Դϴ�.|������ = %u, �˸� = %u, ��ư���� = %u|"), 
                   __FUNCTION__, row, notice_code, temp_button_style);
            continue;
        }
        const TotalGuideNotice::ButtonStyle button_style = 
            static_cast<const TotalGuideNotice::ButtonStyle>(temp_button_style);
        const BYTE temp_char_status = GetDataBYTE(_T("CharStatus"), row);
        if (temp_char_status <= TotalGuideNotice::kBoundaryMin ||
            temp_char_status >= TotalGuideNotice::kBoundaryMax)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�˼� ���� ĳ���ͻ��� �Դϴ�.|������ = %u, �˸� = %u, ĳ���ͻ��� = %u|"), 
                   __FUNCTION__, row, notice_code, temp_char_status);
            continue;
        }
        const TotalGuideNotice::CharStatus char_status = 
            static_cast<const TotalGuideNotice::CharStatus>(temp_char_status);
        const LEVELTYPE min_level = GetDataWORD(_T("MinLevel"), row);
        const LEVELTYPE max_level = GetDataWORD(_T("MaxLevel"), row);
        const BYTE temp_repeat_type = GetDataBYTE(_T("Repeat"), row);
        if (temp_repeat_type != TotalGuideNotice::kOnce &&
            temp_repeat_type != TotalGuideNotice::kContinue &&
            temp_repeat_type != TotalGuideNotice::kOnceForRun)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�˼� ���� �ݺ����� �Դϴ�.|������ = %u, �˸� = %u, �ݺ����� = %u|"), 
                   __FUNCTION__, row, notice_code, temp_repeat_type);
            continue;
        }
        const TotalGuideNotice::RepeatType repeat_type = 
            static_cast<const TotalGuideNotice::RepeatType>(temp_repeat_type);
        const BYTE temp_open_type = GetDataBYTE(_T("WindowStates"), row);
        if (temp_open_type != TotalGuideNotice::kDoNotOpen &&
            temp_open_type != TotalGuideNotice::kMiniWindow && 
            temp_open_type != TotalGuideNotice::kGuideWindow &&
            temp_open_type != TotalGuideNotice::kNoticeWindow)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�˼� ���� â�������� �Դϴ�.|������ = %u, �˸� = %u, â�������� = %u|"), 
                   __FUNCTION__, row, notice_code, temp_open_type);
            continue;
        }
        const TotalGuideNotice::OpenType open_type = 
            static_cast<const TotalGuideNotice::OpenType>(temp_open_type);
        const TotalGuide* guide = NULL;
        const TotalGuide::Index guide_code = GetDataWORD(_T("LinkHelpListIndex"), row);
        if (guide_code != TotalGuide::kInvalid)
        {
            guide = GetGuide(guide_code);
            if (guide == NULL)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|���̵尡 �������� �ʽ��ϴ�.|������ = %u, �˸� = %u, ���̵� = %u|"), 
                       __FUNCTION__, row, notice_code, guide_code);
                continue;
            }
        }
        const CODETYPE title_str_code = GetDataDWORD(_T("TitleStringCode"), row);        
        DWORD param_list[TotalGuideNotice::kParamMaxSize];
        static const TCHAR* param_column_name[_countof(param_list)] = {
            _T("Param1"), _T("Param2")
        };
        for (int i = 0; i < _countof(param_list); ++i)
        {
            param_list[i] = GetDataDWORD(param_column_name[i], row);
        }

        CODETYPE desc_str_code;
        desc_str_code = GetDataDWORD(_T("Desc1"), row);

        notice = new TotalGuideNotice(
            notice_code, 
            button_style, 
            char_status, 
            min_level, 
            max_level, 
            repeat_type, 
            open_type, 
            guide_code, 
            title_str_code, 
            guide, 
            param_list, 
            desc_str_code);
        if (notice_manager_.AddNotice(notice) == false)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�˸��� �߰��ϴµ� ���� �߽��ϴ�.|������ = %u, �˸� = %u|"), 
                   __FUNCTION__, row, notice_code);
            continue;
        }
    }
    return true;
}

bool TotalGuideParser::LoadGuideImage(const bool is_reload)
{
    if (is_reload)
    {
        image_manager_.Clear();
    }

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        const CODETYPE image_code = GetDataDWORD(_T("ImgCode"), row);
        const TotalGuideImage* guide_image = image_manager_.GetGuideImage(image_code);
        if (guide_image != NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|�̹� �߰��Ǿ� �ִ� �̹��� ��ȣ�Դϴ�.|������ = %u, �̹��� ��ȣ = %u|"), 
                __FUNCTION__, row, image_code);
            continue;
        }

        TCHAR guide_image_str[MAX_GUIDE_IMAGE_TEXT_SIZE];
        StrnCopy(guide_image_str, GetDataString("Name", row), MAX_GUIDE_IMAGE_TEXT_SIZE);
        if (guide_image_str == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|�߰��� ���ڿ��� �������� �ʽ��ϴ�.|������ = %u, �̹��� ��ȣ = %u|"), 
                __FUNCTION__, row, image_code);
            continue;
        }

#ifdef _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
        RECT guide_rect = {0,};
        guide_rect.left = GetDataWORD(_T("Left"), row);
        guide_rect.top = GetDataWORD(_T("Top"), row);
        guide_rect.right = GetDataWORD(_T("Right"), row);
        guide_rect.bottom = GetDataWORD(_T("Bottom"), row);

        TotalGuideImage* total_guide_image = new TotalGuideImage(image_code, guide_image_str, guide_rect);
#else
        TotalGuideImage* total_guide_image = new TotalGuideImage(image_code, guide_image_str);
#endif//_NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
        if (image_manager_.AddGuideImage(total_guide_image) == false)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�̹����� �߰��ϴµ� ���� �߽��ϴ�.|������ = %u, �˸� = %u|"), 
                   __FUNCTION__, row, image_code);
            continue;
        }
    }
    return true;
}