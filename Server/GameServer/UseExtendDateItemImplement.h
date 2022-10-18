#ifndef SUN_GAMESERVER_USEEXTENDDATEITEMIMPLEMENT_H_
#define SUN_GAMESERVER_USEEXTENDDATEITEMIMPLEMENT_H_


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


class UseExtendDateItemImplement
{
public:
    UseExtendDateItemImplement();
	~UseExtendDateItemImplement();
    void Implement(MSG_CG_ITEM_USE_EXTEND_DATE_SYN* recv_message);

protected:
    RC::eITEM_RESULT Use_Extend_Date_Applier(Player* player, 
                                             POSTYPE applier_position,
                                             POSTYPE target_position);

};

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL

#endif //SUN_GAMESERVER_USEEXTENDDATEITEMIMPLEMENT_H_