#pragma once
#include <scslot.h>

class EventItemSlot :
	public SCSlot
{
public:
	EventItemSlot(void);
	virtual ~EventItemSlot(void);

	virtual VOID		Clear(){ memset(&m_EventSlot, 0, sizeof(m_EventSlot)); }
	virtual VOID		Copy( SCSlot & IN slot );

	virtual eSlotType	GetSlotType() const	{ return ST_EVENT; }
	virtual DBSERIAL	GetSerial() const	{ return m_EventSlot.m_ItemSeq; }
	virtual SLOTCODE	GetCode() const		{ return m_EventSlot.m_ItemCode;}
	virtual DURATYPE	GetNum() const		{ return m_EventSlot.m_ItemCount; }
	DWORD	GetDateTime() const					{ return m_EventSlot.m_DateTime; }
public:
	VOID	CopyOut( EVENTSLOT & OUT rInfo ) { memcpy(&rInfo, &m_EventSlot, sizeof(rInfo)); }
	VOID	Serialize( EVENTSLOTEX & rInfo, eSERIALIZE se ) { if(se == SERIALIZE_STORE ) memcpy( &m_EventSlot, &rInfo, sizeof(m_EventSlot) ); else memcpy(&rInfo, &m_EventSlot, sizeof(rInfo)); }

private:
	EVENTSLOTEX m_EventSlot;
};
