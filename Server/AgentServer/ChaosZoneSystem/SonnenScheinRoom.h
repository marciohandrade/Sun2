#pragma once
#ifdef _NA_008334_20150608_SONNENSCHEIN

class InstanceDungeon;
class SonnenScheinRoom
{
public:
    SonnenScheinRoom(KEYTYPE roomkey, GUILDGUID guildguid): 
      room_key_(roomkey)
      ,guild_guid_(guildguid)
      ,is_stamp_complete_(FALSE)
      {}
    ~SonnenScheinRoom();

    const GUILDGUID GetGuildGuid() { return guild_guid_;}
    const KEYTYPE GetZoneKey() { return room_key_; }

    VOID Release();

    VOID KickAllUsers();

    VOID StampComplete() { is_stamp_complete_ = TRUE;}
    BOOL IsStampComplete() {return is_stamp_complete_;}

    INT GetPlayerList_CharGuid(CHARGUID* list OUT); // return enter_player_count
    User* FindUser(DWORD player_key);
private:
    //Room���� ���� �ִ� Field�� ��������
    VOID _KickUser(User* user);

    KEYTYPE room_key_;
    //�ش� ���� �Ҽӵ� ���
    GUILDGUID guild_guid_;

    //���� �Ϸ� ����
    BOOL is_stamp_complete_;
};



#endif //_NA_008334_20150608_SONNENSCHEIN