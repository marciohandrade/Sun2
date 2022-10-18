#include "stdafx.h"
#include "CompetitionFormula.h"
#include "FormulaRatioParser.h"
#include "ServerOptionParser.h"
#include <ServerOptionParserEx.h>

BYTE CalcNPCKillPoint( eNPC_GRADE eGrade )
{
	switch( eGrade )
	{
	case eNPC_GENERAL:		return 1;
	case eNPC_LEADER:		return 2;
	case eNPC_ELITE:		return 4;
	case eNPC_MIDDLEBOSS:	return 7;
	case eNPC_BOSS:			return 15;
	default:				return 1;
	}
}

BYTE CalcPlayerKillPoint( int iLevelDiff )
{
	if( iLevelDiff < 3 )		return 1;
	else if( iLevelDiff < 7 )	return 2;
	else if( iLevelDiff < 11 )	return 3;
	else if( iLevelDiff < 16 )	return 5;
	else if( iLevelDiff < 21 )	return 6;
	else						return 10;
}

float CalcCompetitionExpLevelRatio( int iPartyLevelDiff )
{
	switch( iPartyLevelDiff )
	{
	case 6:				return 0.9f;
	case 7:				return 0.85f;
	case 8:				return 0.8f;
	case 9:				return 0.75f;
	case 10:			return 0.7f;
	case 11:			return 0.65f;
	case 12:			return 0.6f;
	case 13:			return 0.55f;
	case 14:			return 0.5;
	case 15:			return 0.45f;
	case 16:			return 0.4f;
	case 17:			return 0.35f;
	case 18:			return 0.3f;
	case 19:			return 0.2f;
	case 20:			return 0.1f;
	default:
		if( iPartyLevelDiff <= 5 )	return 1.0f;
		else						return 0;
	}
}

VOID GetModeValue( eCOMPETITION_MODE_TYPE eMode, float & OUT fModeRatio, WORD & OUT wProcessTime )
{
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	WORD wMaxTimeAttackTime = (WORD)(CHUNTING_TIMEATTACK_EXPIRE_TIME/1000);
	WORD wMaxSurvivalTime = (WORD)(CHUNTING_SURVIVAL_EXPIRE_TIME/1000);
#else
	SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

	WORD wMaxTimeAttackTime = (WORD)(stServerOptionInfo.m_dwTIMEATTACK_PLAY_TIME/1000);
	WORD wMaxSurvivalTime = (WORD)(stServerOptionInfo.m_dwSURVIVAL_PLAY_TIME/1000);
#endif

	switch( eMode )
	{
	case eCOMPETITION_MODE_TIME_ATTACK:
		{
			BASE_FORMULA_RATIOINFO *pFormulaRatio = FormulaRatioParser::Instance()->GetFormulaRatio( RATIO_TYPE_TIMEATTACK, 0 );
			if( pFormulaRatio )	fModeRatio = pFormulaRatio->m_fRatio;

			wProcessTime = min( wProcessTime, wMaxTimeAttackTime );
		}
		break;
	case eCOMPETITION_MODE_DEATH_MATCH:
		{
			BASE_FORMULA_RATIOINFO *pFormulaRatio = FormulaRatioParser::Instance()->GetFormulaRatio( RATIO_TYPE_DEATHMATCH, 0 );
			if( pFormulaRatio )	fModeRatio = pFormulaRatio->m_fRatio;

			wProcessTime = min( wProcessTime, wMaxSurvivalTime );
		}
		break;
	case eCOMPETITION_MODE_SURVIVAL:
		{
			BASE_FORMULA_RATIOINFO *pFormulaRatio = FormulaRatioParser::Instance()->GetFormulaRatio( RATIO_TYPE_SURVIVAL, 0 );
			if( pFormulaRatio )	fModeRatio = pFormulaRatio->m_fRatio;

			wProcessTime = min( wProcessTime, wMaxSurvivalTime );
		}
		break;
	default:
		{
			wProcessTime = min( wProcessTime, wMaxSurvivalTime );
			SUNLOG( eCRITICAL_LOG, "[CalcCompetitionExp] Invalid eMode[%d]", eMode );
		}
	}
}

int CalcCompetitionExp( eCOMPETITION_MODE_TYPE eMode, WORD wProcessTime, BYTE byPartyMemberNum, LEVELTYPE MyLV, LEVELTYPE MyPartyTotLV, LEVELTYPE MyPartyAvgLV, LEVELTYPE EnemyPartyAvgLV, WORD wWinPoint )
{
	// 연승값
	BYTE byVictoryNum = (BYTE)(wWinPoint / 100);
	if( byVictoryNum > 40 )		return 0;

	MyPartyTotLV = max( 1, MyPartyTotLV );
	byPartyMemberNum = min( MAX_PARTYMEMBER_NUM, byPartyMemberNum );

	// 씨드값
	double dSeed = 0.03 * ( EnemyPartyAvgLV + 45 );

	// 파티레벨차에 대한 조정%값
	double LevelDiffRatio = CalcCompetitionExpLevelRatio( MyPartyAvgLV - EnemyPartyAvgLV );

	// 중간 획득경험치
	int iDecisionExp = (int)( ( pow(dSeed, 4) + 3 * pow(dSeed, 3) - 5 ) * ( 30.f / ( 41 - byVictoryNum ) + 0.25f ) * LevelDiffRatio );

	// 스크립트 배율%값
	BASE_FORMULA_RATIOINFO *pFormulaRatio = FormulaRatioParser::Instance()->GetFormulaRatio( RATIO_TYPE_CHUNTING_EXP, MyLV );
	float fScriptRatio = 1.0f;
	if( pFormulaRatio )		fScriptRatio = pFormulaRatio->m_fRatio;

	// 파티원증가에따른 %값
	pFormulaRatio = FormulaRatioParser::Instance()->GetFormulaRatio( RATIO_TYPE_CHUNTING_PARTYNUM );
	float fPartyRatio = 0;
	if( pFormulaRatio )		fPartyRatio = pFormulaRatio->m_fRatio;

	// Mode 배율%값, Time 값 보정
	float fModeRatio = 1.0f;
	GetModeValue( eMode, fModeRatio, wProcessTime );

	// 최종 획득경험치
	float fResultExp = (float)iDecisionExp * wProcessTime/10 * fScriptRatio * fModeRatio;
	byPartyMemberNum = max( 1, byPartyMemberNum );
	fResultExp *= ( 1 + fPartyRatio * ( byPartyMemberNum - 1 ) );

	if( fResultExp < 1 )	return 1;

	return (int)fResultExp;
}

int CalcCompetitionMoney( eCOMPETITION_MODE_TYPE eMode, WORD wProcessTime, BYTE byPartyMemberNum, LEVELTYPE MyLV, LEVELTYPE MyPartyAvgLV, LEVELTYPE EnemyPartyAvgLV, WORD wWinPoint )
{
	// 연승값
	BYTE byVictoryNum = (BYTE)(wWinPoint / 100);
	if( byVictoryNum > 40 )		return 0;

	// 씨드값
	WORD wSeed = EnemyPartyAvgLV;

	// 파티레벨차에 대한 조정%값
	float fLevelDiffRatio = CalcCompetitionExpLevelRatio( MyPartyAvgLV - EnemyPartyAvgLV );

	// 스크립트 배율%값
	BASE_FORMULA_RATIOINFO *pFormulaRatio = FormulaRatioParser::Instance()->GetFormulaRatio( RATIO_TYPE_CHUNTING_MONEY, MyLV );
	float fScriptRatio = 1.0f;
	if( pFormulaRatio )	fScriptRatio = pFormulaRatio->m_fRatio;

	// 파티원증가에따른 %값
	pFormulaRatio = FormulaRatioParser::Instance()->GetFormulaRatio( RATIO_TYPE_CHUNTING_PARTYNUM );
	float fPartyRatio = 0;
	if( pFormulaRatio )		fPartyRatio = pFormulaRatio->m_fRatio;

	// Mode 배율%값, Time 값 보정
	float fModeRatio = 1.0f;
	GetModeValue( eMode, fModeRatio, wProcessTime );

	// 최종 획득하임
	float fTempValue = ( (float)sqrt( wSeed * 100.f + 2000 ) - 43.6f ) * 4;
	float fResultMoney = fTempValue * ( 30.f / ( 41 - byVictoryNum ) + 0.25f ) * fLevelDiffRatio * fScriptRatio * fModeRatio * wProcessTime/10;
	byPartyMemberNum = max( 1, byPartyMemberNum );
	fResultMoney *= ( 1 + fPartyRatio * ( byPartyMemberNum - 1 ) );

	if( fResultMoney < 1 )		return 1;

	return (int)fResultMoney;
}




















