
#include "stdafx.h"
#include "Global.h"
#include <ProgramCommon/WzDataType.h>

eCLASS_TYPE CharTypeToClassType(const eCHAR_TYPE char_type)
{
	switch(char_type)
	{
	case eCHAR_BERSERKER: 
        return eCLASS_BERSERKER;
	case eCHAR_DRAGON: 
        return eCLASS_DRAGON;
	case eCHAR_SHADOW: 
        return eCLASS_SHADOW;
	case eCHAR_VALKYRIE: 
        return eCLASS_VALKYRIE;
	case eCHAR_ELEMENTALIST: 
        return eCLASS_ELEMENTALIST;
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return eCLASS_MYSTIC;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        return eCLASS_HELLROID;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return eCLASS_WITCHBLADE;    
	}
	return eCLASS_NONE;
}

std::string ToString(const WzVector& vec)
{
	char buf[128];
	sprintf(buf, "(%5.2f, %5.2f, %5.2f)", vec.x, vec.y, vec.z);
	return buf;
}

