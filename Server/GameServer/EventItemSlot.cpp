#include "StdAfx.h"
#include ".\eventitemslot.h"

EventItemSlot::EventItemSlot(void)
{
}

EventItemSlot::~EventItemSlot(void)
{
}

VOID EventItemSlot::Copy( SCSlot & IN slot )
{
	SCSlot::Copy( slot );

	EventItemSlot & rEventSlot = (EventItemSlot &)slot;
	m_EventSlot = rEventSlot.m_EventSlot;
}