#pragma once

// ���� ��޿� ���� ų ����Ʈ
BYTE CalcNPCKillPoint( eNPC_GRADE eGrade );

// �÷��̾� �������� ���� ų ����Ʈ
BYTE CalcPlayerKillPoint( int iLevelDiff );

// �������� �¸��� ȹ�� ����ġ
int CalcCompetitionExp( eCOMPETITION_MODE_TYPE eMode, WORD wProcessTime, BYTE byPartyMemberNum, LEVELTYPE MyLV, LEVELTYPE MyPartyTotLV, LEVELTYPE MyPartyAvgLV, LEVELTYPE EnemyPartyAvgLV, WORD wWinPoint );

// �������� �¸��� ȹ�� ����
int CalcCompetitionMoney( eCOMPETITION_MODE_TYPE eMode, WORD wProcessTime, BYTE byPartyMemberNum, LEVELTYPE MyLV, LEVELTYPE MyPartyAvgLV, LEVELTYPE EnemyPartyAvgLV, WORD wWinPoint );


















