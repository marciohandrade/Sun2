#include "StdAfx.h"
#include ".\varmsg.h"

#if SUN_CODE_BACKUP
VarMsg::VarMsg(void)
{
	for( int i = 0 ; i < _MAX_MSG_NUM ; ++i )
	{
		m_ppMsgOut[i] = m_ppMsg[i];
	}
}

VarMsg::~VarMsg(void)
{
}
#endif //SUN_CODE_BACKUP