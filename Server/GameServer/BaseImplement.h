#pragma once


class Player;
class ItemManager;
class SCItemSlot;
class SCSlotContainer;


class BaseImplement
{
public:
    BaseImplement( void );
	virtual ~BaseImplement( void );

    VOID DoMessageImplement( MSG_BASE* pRecvMessage );

protected:
    BOOLEAN IsException( MSG_BASE* pRecvMessage );

protected:
    virtual BOOL OnCheck( MSG_BASE* pRecvMessage, DWORD& OUT dwResult ) = 0;
    virtual VOID OnImplement( MSG_BASE* pRecvMessage ) = 0;
    virtual VOID OnFailed( DWORD dwResult ) = 0;

protected:
    uchar   receive_protocol_;
    Player* m_pPlayer;
    Player* player_;
};
