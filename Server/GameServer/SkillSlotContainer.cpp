// SkillSlotContainer.cpp: implementation of the CSkillSlotContainer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SkillSlotContainer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillSlotContainer::CSkillSlotContainer()
{
	Init( MAX_SKILL_SLOT_NUM, SI_SKILL );
}

CSkillSlotContainer::~CSkillSlotContainer()
{

}
