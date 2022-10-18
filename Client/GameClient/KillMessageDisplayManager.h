#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"

class KillMessage
{
public:
    KillMessage();
    ~KillMessage();

    BOOL Process(DWORD tick);

    void set_first_user_id(TCHAR* string) { StrnCopy(first_user_id, string, MAX_CHARNAME_LENGTH); }
    void set_first_user_color(DWORD color) { first_user_color = color;}
    void set_second_user_id(TCHAR* string) { StrnCopy(second_user_id, string, MAX_CHARNAME_LENGTH); }
    void set_second_user_color(DWORD color) { second_user_color = color;}
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    void set_monster_id(TCHAR* string) { StrnCopy(monster_id, string, MAX_MONSTERNAME_LENGTH); }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

    void set_format_string(TCHAR* string) { StrnCopy(format_string, string, INTERFACE_STRING_LENGTH); }

    void GetOutPutString(TCHAR* message);
    void GetStringSize(SIZE& size_rect);
    BYTE GetAlpha() const { return alpha; }

private:
    TCHAR first_user_id[MAX_CHARNAME_LENGTH+1];  // 1번째 아이디
    DWORD first_user_color;
    TCHAR second_user_id[MAX_CHARNAME_LENGTH+1]; // 2번째 아이디
    DWORD second_user_color;
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    TCHAR monster_id[MAX_MONSTERNAME_LENGTH+1]; // 2번째 아이디가 없을 경우 monster_user_id 사용
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

    TCHAR format_string[INTERFACE_STRING_LENGTH]; // 스트링 포맷

    DWORD display_time;
    BYTE alpha;
};

class KillMessageDisplayManager
{
public:
    typedef STLX_LIST<KillMessage> KillMessageContainer;
    
    KillMessageDisplayManager();
    ~KillMessageDisplayManager();

    void Init();
    void Render();
    void Process(DWORD tick);

    void Insert(KillMessage message_data);

private:
    KillMessageContainer kill_message_container_;

    FTextRender	ftext_render_;

};






#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND