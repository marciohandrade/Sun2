#include "StdAfx.h"
#include ".\functionalrestrictmanager.h"
#include <Const.h>

FunctionalRestrictManager g_FunctionalRestrictMgr;

FunctionalRestrictManager::FunctionalRestrictManager(void)
{
	m_FunctionalType = eFUNCTIONALRESTRICT_ALLOW;
}

FunctionalRestrictManager::~FunctionalRestrictManager(void)
{

}
