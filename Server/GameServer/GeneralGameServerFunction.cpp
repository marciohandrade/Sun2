#include "stdafx.h"
#include "GeneralGameServerFunction.h"

//����������������������������������������������������������������������������������������������
//
namespace Generic
{
	static INT		s_MaxLevel = 0;
	static INT		s_MaxLevelForChangeOfClass1st = 0;
	// ��ũ��Ʈ ��, �� ���� ���� ���� ���� �޶����� ��� <- ServerOption�� �޷� �ִ�.
    static INT		__GetDefaultMaxLevel()
	{
		s_MaxLevel = GameOptionParser::Instance()->GetMaxLevel();
		return s_MaxLevel;
	}

	// ��ũ��Ʈ ��, ���� ���� �ְ� ���� ���� <- ExpRatio & ServerOption�� �޷� �ִ�.
	static INT		__GetChangeOfClassMaxLevel()
	{
		s_MaxLevelForChangeOfClass1st = GameOptionParser::Instance()->GetMaxLevelChangeOfClass1st();
		return s_MaxLevelForChangeOfClass1st;
	}

	static BOOL			__LoadMaxLevelInfo()
	{
		return (!!__GetDefaultMaxLevel()) &&
			(!!__GetChangeOfClassMaxLevel())
			;
	}
};


//����������������������������������������������������������������������������������������������
//
LEVELTYPE		Generic::GetMaxLevel( Player* IN pPlayer )
{
	static BOOL bInit = __LoadMaxLevelInfo();

	if( !pPlayer )
		return s_MaxLevelForChangeOfClass1st;

	// ���ʿ��� ������ ����. Player�� �������� ������ �Է��� ��? ��...
	const BASE_PLAYERINFO* pCharInfo = pPlayer->GetCharInfo();
	switch( pCharInfo->m_ChangeOfClassStep )
	{
	case eCHAREX_BERSERKER_100LEVEL:
	case eCHAREX_DRAGON_KNIGHT_100LEVEL:
	case eCHAREX_SHADOW_100LEVEL:
	case eCHAREX_VALKYRIE_100LEVEL:
	case eCHAREX_ELEMENTALIST_100LEVEL:
    case eCHAREX_MYSTIC_100LEVEL:
    case eCHAREX_HELLROID_100LEVEL:
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eCHAREX_WITCHBLADE_100LEVEL:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		return (s_MaxLevelForChangeOfClass1st);
	}

	return (s_MaxLevel);
}

BOOL			Generic::CheckMaxLevel( Player* IN pPlayer, const INT level )
{
	const LEVELTYPE curMaxLevelPerPlayer = GetMaxLevel( pPlayer );
	return !(curMaxLevelPerPlayer < level);
}




