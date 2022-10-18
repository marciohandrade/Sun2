#include "stdafx.h"
#include "./ServerStruct.h"

//==================================================================================================
// @history
//  11.04.28, waverix, this file added to split implements from declaration sector

void SERVER_CHARACTER_PART::Output(ArchiveOfStream& archive)
{
    //write
    archive << m_CharGuid;
    archive << m_bySlot;

    m_tszCharName[_countof(m_tszCharName) - 1] = '\0';
    BYTE len = static_cast<BYTE>(strlen(m_tszCharName));
    archive << len;
    archive.Write(m_tszCharName, len);

    archive << m_byHeight;
    archive << m_byFace;
    archive << m_byHair;
    archive << m_byClass;
    archive << m_LV;
    archive << m_CharState;
    archive << m_iRegion;
    archive << m_sX;
    archive << m_sY;
    archive << m_sZ;
    archive << m_ChangeOfClassStep;
    archive << m_GuildGuid;
    if (m_GuildGuid != 0)
    {
        m_tszGuildNicName[_countof(m_tszGuildNicName) - 1] = '\0';
        len = static_cast<BYTE>(strlen(m_tszGuildNicName));
        archive << m_eGuildDuty;
        archive << len;
        archive.Write(m_tszGuildNicName, len);
    }

    archive << m_PetSlotStream;
    archive << m_bSummoned;
    archive << m_EquipItemInfo.m_Count;

    for (BYTE i = 0; i < m_EquipItemInfo.m_Count; ++i) {
        archive << m_EquipItemInfo.m_Slot[i];
    }

    archive << m_DelChk;
    if (m_DelChk != 0)
    {
        m_tszDelDate[_countof(m_tszDelDate) - 1] = '\0';
        len = static_cast<BYTE>(strlen(m_tszDelDate));
        archive << len;
        archive.Write(m_tszDelDate, len);
    }

    // f100929.4L, removed an unused section about total_state_info
    //
#ifdef _NA_002050_20110216_ADD_GENDER
    archive << gender;
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
	archive << honor_title;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_004035_20120227_COSTUME_ITEM
    archive << is_equipped_costume;
    archive << costume_item_pos;
    archive << costume_item_code;
#endif  // _NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    archive << is_equipped_deco_costume;
    archive << deco_costume_item_pos;
    archive << deco_costume_item_code;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
}

void SERVER_CHARACTER_PART::Input(ArchiveOfStream& archive)
{
    //read
    archive >> m_CharGuid;
    archive >> m_bySlot;
    BYTE len = 0;
    archive >> len;
    archive.Read(m_tszCharName, len);
    m_tszCharName[len] = '\0';

    archive >> m_byHeight;
    archive >> m_byFace;
    archive >> m_byHair;
    archive >> m_byClass;
    archive >> m_LV;
    archive >> m_CharState;
    archive >> m_iRegion;
    archive >> m_sX;
    archive >> m_sY;
    archive >> m_sZ;
    archive >> m_ChangeOfClassStep;

    archive >> m_GuildGuid;
    if (m_GuildGuid != 0)
    {
        archive >> m_eGuildDuty;
        archive >> len;
        archive.Read(m_tszGuildNicName, len);
        m_tszGuildNicName[len] = '\0';
    }

    archive >> m_PetSlotStream;
    archive >> m_bSummoned;
    archive >> m_EquipItemInfo.m_Count;
    for (BYTE i = 0; i < m_EquipItemInfo.m_Count; ++i) {
        archive >> m_EquipItemInfo.m_Slot[i];
    }
    archive >> m_DelChk;
    if (m_DelChk != 0)
    {
        archive >> len;
        archive.Read(m_tszDelDate, len);
        m_tszDelDate[len] = '\0';
    }

    // f100929.4L, removed an unused section about total_state_info
    //
#ifdef _NA_002050_20110216_ADD_GENDER
    archive >> gender;
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
	archive >> honor_title;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_004035_20120227_COSTUME_ITEM
    archive >> is_equipped_costume;
    archive >> costume_item_pos;
    archive >> costume_item_code;
#endif  // _NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    archive >> is_equipped_deco_costume;
    archive >> deco_costume_item_pos;
    archive >> deco_costume_item_code;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
}

//==================================================================================================

tag_MMEMO_INFOS& tag_MMEMO_INFOS::operator=(const tag_MMEMO_INFOS& rh)
{
    CopyMemory(this, &rh, sizeof(*this));
    m_OperCharName[_countof(m_OperCharName) - 1] = '\0';
    m_szMessage[_countof(m_szMessage) - 1] = '\0';

    return *this;
}

void tag_MMEMO_ERR_RESULT::SetResult(DWORD result)
{
    m_dwResult = !!result;

    const char* kDescription
        = (result == RC::RC_MEMO_NOSERVER) ? "SERVER SEND PACKET ERROR"
        : (result == RC::RC_MEMO_WRONG_RECVNAMES) ? "WRONG RECVNAMES"
        : (result == RC::RC_MEMO_DBFAILED) ? "DB FAILED"
        : (result == RC::RC_MEMO_NOUSER) ? "WRONG CHARNAME"
        : NULL;

    if (kDescription == NULL) {
        m_szResultDescription[0] = '\0';
    }
    else {
        size_t len = strlen(kDescription);
        ;      len = min(_countof(m_szResultDescription) - 1, len);
        strncpy(m_szResultDescription, kDescription, len);
        m_szResultDescription[len] = '\0';
    }
}

tag_MMEMO_ERR_RESULT& tag_MMEMO_ERR_RESULT::operator=(const tag_MMEMO_ERR_RESULT& rh)
{
    CopyMemory(this, &rh, sizeof(*this));
    m_szRecvCharName[_countof(m_szRecvCharName) - 1] = '\0';
    m_szResultDescription[_countof(m_szResultDescription) - 1] = '\0';

    return *this;
}

bool tag_MMEMO_RESULT::Set(
    const TCHAR* receiver_charname, DWORD db_result_error, CHARGUID receiver_charguid,
    MEMOGUID memo_guid, const TCHAR* created_date, const TCHAR* subject)
{
    m_ErrResult.SetCharName(receiver_charname);

    // 성공(0) 값은 결과를 셋팅하지 않는다.
    if (db_result_error != 0)
    {
        RC::eMEMO_RESULT memo_result = db_result_error == 1 ? RC::RC_MEMO_DBFAILED
                                     : db_result_error == 2 ? RC::RC_MEMO_NOUSER
                                     :                        RC::RC_MEMO_DBFAILED;
        m_ErrResult.SetResult(memo_result);
        return false;
    }

    m_RecvGuid = receiver_charguid;
    m_MemoGuid = memo_guid;

    _tcsncpy(m_tszCreateDate, created_date, _countof(m_tszCreateDate));
    m_tszCreateDate[_countof(m_tszCreateDate) - 1] = '\0';

    _tcsncpy(m_ptszSubject, subject, _countof(m_ptszSubject));
    m_ptszSubject[_countof(m_ptszSubject) - 1] = '\0';

    return true;
}
