#pragma once

#include "NPCAttributes.h"

class NPCAttrForUpdate : public NPCAttributes
{
public :
	NPCAttrForUpdate() {}
	virtual ~NPCAttrForUpdate() {}

	virtual VOID		SetOwner( SCNPC* pNPC );
	virtual VOID		UpdateEx();

private:
	NPC*		m_pNPC;
};

