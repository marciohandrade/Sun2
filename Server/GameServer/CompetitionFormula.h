#pragma once

// 몬스터 등급에 따른 킬 포인트
BYTE CalcNPCKillPoint( eNPC_GRADE eGrade );

// 플레이어 레벨차에 따른 킬 포인트
BYTE CalcPlayerKillPoint( int iLevelDiff );

// 경쟁헌팅 승리시 획득 경험치
int CalcCompetitionExp( eCOMPETITION_MODE_TYPE eMode, WORD wProcessTime, BYTE byPartyMemberNum, LEVELTYPE MyLV, LEVELTYPE MyPartyTotLV, LEVELTYPE MyPartyAvgLV, LEVELTYPE EnemyPartyAvgLV, WORD wWinPoint );

// 경쟁헌팅 승리시 획득 하임
int CalcCompetitionMoney( eCOMPETITION_MODE_TYPE eMode, WORD wProcessTime, BYTE byPartyMemberNum, LEVELTYPE MyLV, LEVELTYPE MyPartyAvgLV, LEVELTYPE EnemyPartyAvgLV, WORD wWinPoint );


















