#include "SunClientPrecompiledHeader.h"

#include "AutobotConditionManager.h"
#include "ChatMsg.h"
#include "InterfaceManager.h"

//------------------------------------------------------------------------------
AutobotConditionManager::AutobotConditionManager()
{
    condition_filename_.clear();
    chat_log_filename_.clear();
}
//------------------------------------------------------------------------------
AutobotConditionManager::~AutobotConditionManager()
{
}
//------------------------------------------------------------------------------ 
bool AutobotConditionManager::Init(char* character_name)
{
    condition_filename_.clear();
    chat_log_filename_.clear();

    condition_filename_ += character_name;
    condition_filename_ += ".atb";

    SYSTEMTIME utc_time;
    util::TimeSync::GetSystemTime(&utc_time);
    char str[256] = {NULL, };
    sprintf(str, "_%d%d%d", utc_time.wYear, utc_time.wMonth, utc_time.wDay);

    //70238	ScreenShot
    TCHAR dir[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(70238, dir, INTERFACE_STRING_LENGTH);
    CreateDirectory(dir, NULL);

    chat_log_filename_ += dir;
    chat_log_filename_ += character_name;
    chat_log_filename_ += str;
    chat_log_filename_ += ".txt";

    Clear();

    return true;
}
//------------------------------------------------------------------------------ 
void AutobotConditionManager::Clear()
{
    loaded_ = false;

    recovery().Clear();
    skill().Clear();
    revival().Clear();
    shop().Clear();
    hunting().Clear();
    pickup().Clear();
    recall().Clear();
    community().Clear();
}
//------------------------------------------------------------------------------ 
bool AutobotConditionManager::Save()
{
    //HERO_INFO
    FILE* fp = fopen(condition_filename_.c_str(), "wb");
    if (fp != NULL)
    {
        fwrite(&AutobotCondition::kAutobotFileVersion, sizeof(int), 1, fp);

        recovery().Save(fp);
        skill().Save(fp);
        revival().Save(fp);
        shop().Save(fp);
        hunting().Save(fp);
        pickup().Save(fp);
        recall().Save(fp);
        community().Save(fp);

        fclose(fp);

        loaded_ = true;
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool AutobotConditionManager::Load()
{
    FILE* fp = fopen(condition_filename_.c_str(), "rb");
    if (fp != NULL)
    {
        int file_version = 0;
        fread(&file_version, sizeof(int), 1, fp);

        recovery().Load(fp, file_version);
        skill().Load(fp, file_version);
        revival().Load(fp, file_version);
        shop().Load(fp, file_version);
        hunting().Load(fp, file_version);
        pickup().Load(fp, file_version);
        recall().Load(fp, file_version);
        community().Load(fp, file_version);

        fclose(fp);

        loaded_ = true;
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool AutobotConditionManager::SaveToBuffer(ArchiveOfStream& ar)
{
    ar << AutobotCondition::kAutobotFileVersion;

    recovery().SaveToBuffer(ar);
    skill().SaveToBuffer(ar);
    revival().SaveToBuffer(ar);
    shop().SaveToBuffer(ar);
    hunting().SaveToBuffer(ar);
    pickup().SaveToBuffer(ar);
    recall().SaveToBuffer(ar);
    community().SaveToBuffer(ar);

    dummy_recovery().CopyData(recovery());
    dummy_skill().CopyData(skill());
    dummy_revival().CopyData(revival());
    dummy_shop().CopyData(shop());
    dummy_hunting().CopyData(hunting());
    dummy_pickup().CopyData(pickup());
    dummy_recall().CopyData(recall());
    dummy_community().CopyData(community());

    loaded_ = true;

    return true;
}
//------------------------------------------------------------------------------ 
bool AutobotConditionManager::LoadFromBuffer(ArchiveOfStream& ar)
{
    if (ar.GetBufferSize() > 0)
    {
        int file_version = 0;
        ar >> file_version;

        recovery().LoadFromBuffer(ar, file_version);
        skill().LoadFromBuffer(ar, file_version);
        revival().LoadFromBuffer(ar, file_version);
        shop().LoadFromBuffer(ar, file_version);
        hunting().LoadFromBuffer(ar, file_version);
        pickup().LoadFromBuffer(ar, file_version);
        recall().LoadFromBuffer(ar, file_version);
        community().LoadFromBuffer(ar, file_version);

        dummy_recovery().CopyData(recovery());
        dummy_skill().CopyData(skill());
        dummy_revival().CopyData(revival());
        dummy_shop().CopyData(shop());
        dummy_hunting().CopyData(hunting());
        dummy_pickup().CopyData(pickup());
        dummy_recall().CopyData(recall());
        dummy_community().CopyData(community());
    }

    loaded_ = true;

    return true;
}
//------------------------------------------------------------------------------ 
void AutobotConditionManager::CheckValid(SLOTCODE item_code)
{
    recovery().CheckValid(item_code);
    skill().CheckValid(item_code);
    revival().CheckValid(item_code);
    shop().CheckValid(item_code);
    hunting().CheckValid(item_code);
    pickup().CheckValid(item_code);
    recall().CheckValid(item_code);
    community().CheckValid(item_code);
}
//------------------------------------------------------------------------------ 
void AutobotConditionManager::WriteChat(ChatMsg* chat)
{
    if (community().is_save_chat_message() == false)
    {
        return;
    }
    if (chat_log_filename_.length() <= 0)
    {
        return;
    }
    if ((chat == NULL) || (chat->m_eType == CHAT_TYPE_SYSTEM))
    {
        //! 저장하지 않음
        return;
    }

    static char chat_message[INTERFACE_STRING_LENGTH + 1] = {0,};
    ZeroMemory(chat_message, sizeof(chat_message));

    if (chat->m_pszCharName)
    {
        strncpy(chat_message, chat->m_pszCharName, MAX_CLIENT_CHARNAME_LENGTH);
    }
    if (strlen(chat->m_pszRecvName) > 0)
    {
        strcat(chat_message, "->");
        strcat(chat_message, chat->m_pszRecvName);
    }
    if (strlen(chat->m_pszMessage) > 0)
    {
        strcat(chat_message, " : ");
        strcat(chat_message, chat->m_pszMessage);
        strcat(chat_message, "\n");
    }

    FILE* fp = _tfopen(chat_log_filename_.c_str(), _T("a+t"));
    if (fp)
    {
        fwrite(chat_message, strlen(chat_message), 1, fp);
        fclose(fp);
    }
}
//------------------------------------------------------------------------------ 
bool AutobotConditionManager::SendCheck()
{
    bool recovery_check  = recovery().SendCheck(dummy_recovery());
    bool skill_check     = skill().SendCheck(dummy_skill());
    bool revival_check   = revival().SendCheck(dummy_revival());
    bool shop_check      = shop().SendCheck(dummy_shop());
    bool hunting_check   = hunting().SendCheck(dummy_hunting());
    bool pickup_check    = pickup().SendCheck(dummy_pickup());
    bool recall_check    = recall().SendCheck(dummy_recall());
    bool community_check = community().SendCheck(dummy_community());

    // 하나라도 변경 되었으면 send true
    bool is_send = (recovery_check || skill_check || revival_check || shop_check ||
        hunting_check || pickup_check || recall_check || community_check);

    return is_send;
}