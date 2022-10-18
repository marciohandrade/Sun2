#include "StdAfx.h"
#include ".\questconditioninfo_concrete.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
RC::eQUEST_RESULT QUEST_CONDITION_REPEAT_INFO::IsSatisfy( QUESTCOUNT CurRepeatNum )
{
	RC::eQUEST_RESULT rt;

	if( 0 == m_MaxRepeatNum || CurRepeatNum < m_MaxRepeatNum )
		rt = RC::RC_QUEST_SUCCESS;
	else
		rt = RC::RC_QUEST_OVER_REPEAT;

	return rt;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
RC::eQUEST_RESULT QUEST_CONDITION_CHARLV_INFO::IsSatisfy( WORD CurCharLV )
{
	RC::eQUEST_RESULT rt;

	if( m_MinCharLV <= CurCharLV && CurCharLV <= m_MaxCharLV )
		rt = RC::RC_QUEST_SUCCESS;
	else
		rt = RC::RC_QUEST_MISMATCH_CHARACTER_LV;

	return rt;
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
RC::eQUEST_RESULT QUEST_CONDITION_CHAO_INFO::IsSatisfy( BYTE byChaoState )
{	
	if( m_ChaoState >= byChaoState )
		return RC::RC_QUEST_SUCCESS;

    return RC::RC_QUEST_NOT_CHAO;
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
RC::eQUEST_RESULT QUEST_CONDITION_CHARCLASS_INFO::IsSatisfy( BYTE byCharClass )
{
	RC::eQUEST_RESULT rt;

	if( m_CharClassBit & byCharClass )
		rt = RC::RC_QUEST_SUCCESS;
	else
		rt = RC::RC_QUEST_MISMATCH_CHARACTER_CLASS;
	
	return rt;
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
RC::eQUEST_RESULT QUEST_CONDITION_MONEY_INFO::IsSatisfy( DWORD64 CurMoney )
{
	RC::eQUEST_RESULT rt;	

	if( CurMoney < m_NeedMoney )
		rt = RC::RC_QUEST_LESS_MONEY;
	else
		rt = RC::RC_QUEST_SUCCESS;
	
	return rt;
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收