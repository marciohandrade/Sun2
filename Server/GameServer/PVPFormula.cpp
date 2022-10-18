#include "stdafx.h"
#include "PVPFormula.h"




//---------------------------------------------------------------------------------
// PVP Score, Point ╟Ь╫д
//---------------------------------------------------------------------------------
DWORD CalcPVPPoint( DWORD MaxKillSeries, DWORD TotalKill, DWORD TotalDie )
{
	if( TotalKill+TotalDie == 0 ) return 0;

	return (DWORD)(MaxKillSeries*2+TotalKill*(1+
		TotalKill/(TotalKill+TotalDie)						//< ╫б╥Э
		));
}
DWORD CalcPVPScore( LEVELTYPE CharLV, INT LVInterval )
{
	INT score = 0;
	if( 10 <= CharLV && CharLV < 30 )
		score = (INT)(16+LVInterval*16/8);
	else if( 30 <= CharLV && CharLV < 70 )
		score = (INT)(16+LVInterval*21/11);
	else if( 70 <= CharLV && CharLV < 100 )
		score = (INT)(16+LVInterval*26/16);
	else // ╠в©э╢б юс╫ц
		score = (INT)(16+LVInterval*16/8);

	return (score<0?0:(DWORD)score);
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
