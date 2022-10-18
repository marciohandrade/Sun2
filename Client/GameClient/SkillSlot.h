#pragma once

#include <scskillslot.h>

class SkillSlot : public SCSkillSlot
{
public:
	SkillSlot(void);
	virtual ~SkillSlot(void);
	SkillSlot(const SKILLSTREAM & SkillStream);

	virtual DBSERIAL	GetSerial() const				{ return m_Serial; }
	virtual VOID		SetSerial( DBSERIAL serial )	{ m_Serial = serial; }

	virtual VOID		Copy( SCSlot & IN slot );
	virtual VOID		Clear();
private:
	DBSERIAL				m_Serial;//<юс╫ц
};
