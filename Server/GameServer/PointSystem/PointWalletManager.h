#ifndef _POINT_WALLET_MANAGER_H_
#define _POINT_WALLET_MANAGER_H_

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include <PointSystem/PointSystem.h>

class Player;

class PointWalletManager
{
public:
    PointWalletManager();
    ~PointWalletManager();

    bool init(Player* player);

    void Initialize();
    void SetPointWallet(DWORD _code, INT _point);
    INT GetPointWallet(DWORD _code) const;
    INT ChangeWalletPoint(DWORD _code, INT _point);

    void ParseCGPacket(PVOID packet);
    void ParseDGPacket(PVOID packet);

    void ProcessPacketUseItem(PVOID packet);
    RC::eITEM_RESULT UseItem(SLOTIDX _slot_index, POSTYPE _position, INT _count, \
                            SCItemSlot& OUT rAtSlotOut);

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    void GuildPointDonation(INT donation_item_count);
    void ProcessGuildCoinDonation( \
        INT donation_count_of_day, INT donation_item_count, INT max_donation_count);
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

private:
    Player*             player_;
    BASE_PLAYERINFO*    playerinfo_;

    PointSystem::UserNode point_wallet_;
};

#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#endif //_POINT_WALLET_MANAGER_H_