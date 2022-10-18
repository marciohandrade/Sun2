#include "SunClientPrecompiledHeader.h"
#include "skillslot.h"

SkillSlot::SkillSlot(void)
{
}

SkillSlot::~SkillSlot(void)
{
}

SkillSlot::SkillSlot(const SKILLSTREAM & SkillStream):
SCSkillSlot(SkillStream)
{
}
VOID SkillSlot::Copy( SCSlot & IN slot )
{
	SCSkillSlot::Copy( slot );
	m_Serial = slot.GetSerial();

	SkillSlot & rSkillSlot = (SkillSlot &)slot;
}
VOID SkillSlot::Clear()
{
	SCSkillSlot::Clear();
}