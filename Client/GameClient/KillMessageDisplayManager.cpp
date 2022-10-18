#include "SunClientPrecompiledHeader.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "KillMessageDisplayManager.h"
#include "Application.h"
#include "InterfaceManager.h"

const DWORD kShowTime = 3000;
const DWORD kFadeOutTime = 500;

KillMessage::KillMessage() :
display_time(0),
alpha(255)
{
    ZeroMemory(first_user_id, MAX_CHARNAME_LENGTH+1);
    first_user_color = WzColor_RGBA(255,255,255,255);
    ZeroMemory(second_user_id, MAX_CHARNAME_LENGTH+1);
    second_user_color = WzColor_RGBA(255,255,255,255);

    ZeroMemory(format_string, INTERFACE_STRING_LENGTH);

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    ZeroMemory(monster_id, MAX_MONSTERNAME_LENGTH+1);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
}

KillMessage::~KillMessage()
{

}

BOOL KillMessage::Process(DWORD tick)
{
    display_time += tick;

    if (display_time >= kShowTime + kFadeOutTime)
    {
        return FALSE;
    }
    else if (display_time >= kShowTime)
    {
        float rate = 1.0f - (display_time - kShowTime) / static_cast<float>(kFadeOutTime);
        rate = min(max(rate, 0.0f), 1.0f);

        alpha = static_cast<BYTE>(255 * rate);
    }
    else
    {
        alpha = 255;
    }

    return TRUE;
}
void KillMessage::GetOutPutString(TCHAR* message)
{
    // 태그 붙이기
    TCHAR first_format_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR second_format_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR result_format_string[INTERFACE_STRING_LENGTH] = {0,};

    // 아이디에 색상
    Snprintf(first_format_string, INTERFACE_STRING_LENGTH-1, "<COLOR=%x>%s</COLOR>", WzColor_ChangeAlpha(first_user_color, alpha), first_user_id);
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    // second_id가 없을 경우 monster_id를 복사해서 넣는다.
    if (_tcscmp(second_user_id, _T("")) == 0)
    {
        Snprintf(second_format_string, INTERFACE_STRING_LENGTH-1, "<COLOR=%x>%s</COLOR>",WzColor_ChangeAlpha(second_user_color, alpha), monster_id);        
    }
    else
    {
        Snprintf(second_format_string, INTERFACE_STRING_LENGTH-1, "<COLOR=%x>%s</COLOR>",WzColor_ChangeAlpha(second_user_color, alpha), second_user_id);
    }    
#else
    Snprintf(second_format_string, INTERFACE_STRING_LENGTH-1, "<COLOR=%x>%s</COLOR>",WzColor_ChangeAlpha(second_user_color, alpha), second_user_id);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    // 아이디를 포맷 문자열과 연결
    Snprintf(result_format_string, INTERFACE_STRING_LENGTH-1, format_string, first_format_string, second_format_string);

    // 전체 문자열에 색상
    Snprintf(message, INTERFACE_STRING_LENGTH-1, "<SHADOW=2><SDWCOLOR=%x><font=tt13><COLOR=%x>%s</COLOR></font></SDWCOLOR></SHADOW>",
        WzColor_RGBA(0,0,0,alpha), WzColor_RGBA(255,255,255,alpha) , result_format_string);
}

void KillMessage::GetStringSize(SIZE& size_rect)
{
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    // second_id가 없을 경우 monster_id를 복사해서 넣는다.
    if (_tcscmp(second_user_id, _T("")) == 0)
    {
        Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, format_string, first_user_id, monster_id);
    }
    else
    {
        Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, format_string, first_user_id, second_user_id);
    }
#else
    Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, format_string, first_user_id, second_user_id);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

    g_pSunRenderer->x_pManagerTextOut->StartToUseFont(StrToWzID("tt13"));
    g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(temp_string, strlen(temp_string), &size_rect);
    g_pSunRenderer->x_pManagerTextOut->EndUseFont();

    size_rect.cx += 6; // 글 안짤리게 여유분
}

KillMessageDisplayManager::KillMessageDisplayManager()
{

}

KillMessageDisplayManager::~KillMessageDisplayManager()
{

}

void KillMessageDisplayManager::Init()
{
    POINT pt = g_pApplication->Get2DModeMapping();

    RECT rect = {0, 0, pt.x, pt.y};
    ftext_render_.Init(g_pSunRenderer, rect);
    ftext_render_.SetMargin(0,0,0,0);

    kill_message_container_.clear();
}
void KillMessageDisplayManager::Render()
{
    if (kill_message_container_.empty())
    {
        return;
    }

    ftext_render_.Render();
}

void KillMessageDisplayManager::Process(DWORD tick)
{
    if (kill_message_container_.empty())
    {
        return;
    }

    KillMessageContainer::iterator itr = kill_message_container_.begin();
    KillMessageContainer::iterator itr_end = kill_message_container_.end();
    if (itr == itr_end)
    {
        return;
    }

    KillMessage* message = &(*itr);
    if (message->Process(tick) == FALSE)
    {
        kill_message_container_.pop_front();
    }
    else
    {
        FTextParser parser;
        FTextSentence sentence;

        ftext_render_.ClearSentence();

        TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0,};
        message->GetOutPutString(temp_message);

        if (parser.Parser(temp_message, sentence))
        {
            ftext_render_.Append(sentence);
        }
        ftext_render_.convertLineInfo();


        POINT pt = g_pApplication->Get2DModeMapping();
        int pos_x = pt.x / 2;
        int pos_y = 175;


        SIZE size_rect;
        message->GetStringSize(size_rect);

        RECT rect;
        rect.left = pos_x - size_rect.cx / 2;
        rect.top = pos_y - size_rect.cy / 2;
        rect.right = rect.left + size_rect.cx;
        rect.bottom = rect.top + size_rect.cy;
        
        ftext_render_.SetRect(rect);
    }

    
}

void KillMessageDisplayManager::Insert(KillMessage message_data)
{
    kill_message_container_.push_back(message_data);
}

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND