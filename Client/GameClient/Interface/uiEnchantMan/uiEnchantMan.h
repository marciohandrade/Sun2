#pragma once
#include "uiBaseMan.h"

class EnchantDialog;
class uiEnchantMan :
	public uiBaseMan
{
public:
	uiEnchantMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnInitialize();

#ifdef __NA_001358_ENCHANT_UNIFICATION
	// 확인 팝업 및 패킷 관련 함수
public:	
	void SetInstantEnchantPos(POSTYPE pos)		{	m_InstantEnchantPos = pos; }
	POSTYPE GetInstantEnchantPos()				{	return m_InstantEnchantPos; }
	void SetTargetItemPos(POSTYPE pos)			{	m_TargetItemPos = pos; }
	POSTYPE GetTargetItemPos()					{	return m_TargetItemPos; }
	void UnsetPendingAsk()						{	m_bPendingAsk = false; }

	bool AskInstantEnchant();
	bool CheckEnableEnchant();
	void SendPacketInstantEnchant();
	
	void OnEnchantPacketAck(MSG_BASE * pMsg);
	void OnEnchantPacketNak(MSG_BASE * pMsg);

private:
	void SetPendingPacket(bool bPending)	{	m_bPendingPacket = bPending; }
	bool GetPendingPacket()					{	return m_bPendingPacket; }		

#endif //__NA_001358_ENCHANT_UNIFICATION

private:

#ifdef __NA_001358_ENCHANT_UNIFICATION
	POSTYPE m_InstantEnchantPos;
	POSTYPE m_TargetItemPos;
	bool m_bPendingPacket;
	bool m_bPendingAsk;
	// 확인 팝업 및 패킷 관련 변수
#else //__NA_001358_ENCHANT_UNIFICATION
	EnchantDialog *			m_pEnchantDialog;
#endif //__NA_001358_ENCHANT_UNIFICATION

};
