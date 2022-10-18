#include "StdAfx.h"
#if defined(_NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM) || defined(_NA_008124_20150313_AWAKENING_SYSTEM)
#include "AwakeningInfoParser.h"
#include "ItemInfoParser.h"

AwakeningInfoParser::AwakeningInfoParser(void)
{
}

AwakeningInfoParser::~AwakeningInfoParser(void)
{
}

void AwakeningInfoParser::Init( DWORD pool_size )
{

}

void AwakeningInfoParser::Init( DWORD pool_size, char* pszPackFileName )
{
	SetPackFileName(pszPackFileName);
}

void AwakeningInfoParser::Release()
{
	Unload();
}

void AwakeningInfoParser::Unload()
{
	AwakeningInfo_Hash::const_iterator it, end( awakeninginfo_hash_.end() );
	for(it = awakeninginfo_hash_.begin() ; it != end ; ++it)
	{
		AwakeningInfo* info = (it->second);
		info->awakening_level_info_map.clear();
		SAFE_DELETE(info);
	}

	awakeninginfo_hash_.clear();	
}

BOOL AwakeningInfoParser::_Load( BOOL is_reload )
{
	if ( is_reload == TRUE )
	{
		awakeninginfo_hash().clear();
	}

	int nRowSize = GetRowSize();
	for ( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		AwakeningInfo* info = new AwakeningInfo();

		info->item_code = GetDataDWORD("ItemCode", nRow);
		info->item_name = GetDataString("ItemName", nRow);
		info->awake_separation = GetDataDWORD("AwakeSeparation", nRow);

        TCHAR attribute_string[2048] = {0, };

#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
		for ( int i = 0; i < AwakeningInfo::MAX_ALLROUND_MATERIAL; ++i )
		{
			AllRoundMaterialInfo allround_material_info;
			sprintf(attribute_string, _T("AllroundMaterialCode%d"), i + 1);
			allround_material_info.allround_material_code = GetDataDWORD(attribute_string, nRow);
			if (allround_material_info.allround_material_code == 0)
			{
				continue;
			}
			sprintf(attribute_string, _T("AllroundMaterialCount%d"), i + 1);
			allround_material_info.allround_material_count = GetDataBYTE(attribute_string, nRow);
			info->allround_material_info_list.push_back(allround_material_info);
		}
#else
        for ( int i = 0; i < AwakeningInfo::MAX_ALLROUND_MATERIAL; ++i )
        {
            CODETYPE allround_material_code = 0;
            sprintf(attribute_string, _T("AllroundMaterialCode%d"), i + 1);
            allround_material_code = GetDataDWORD(attribute_string, nRow);
            info->allround_material_code_map.insert(allround_material_code);
        }
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD

		info->awake_permit_level = GetDataBYTE("AwakeLimitLevel", nRow);

        info->additive_item_code = GetDataDWORD("AdditiveItemCode", nRow);
        info->additive_count = GetDataBYTE("AdditiveCount", nRow);
        info->probability_success = GetDataBYTE("ProbabilitySuccess", nRow);

		for ( int i = 1; i <= MAX_AWAKENING_GRADE; ++i )
		{
			DWORD awakening_level_info = 0;
			sprintf(attribute_string, _T("OptionParam%d"), i);
			awakening_level_info = GetDataDWORD(attribute_string, nRow);
			info->awakening_level_info_map.insert(std::make_pair(i, awakening_level_info));
		}

		info->evolution_item_code = GetDataDWORD("EvolutionItemCode", nRow);
		info->evolution_additive_item_code = GetDataDWORD("EvolutionAdditiveItemCode", nRow);
		info->evolution_additive_count = GetDataBYTE("EvolutionAdditiveItemCount", nRow);
		info->evolution_probability_success = GetDataBYTE("EvolutionProbabilitySuccess", nRow);

		AwakeningInfo_Hash::_Pairib result = awakeninginfo_hash().insert(
			AwakeningInfo_Hash::value_type(static_cast<DWORD>(info->item_code), info));
		ASSERT(result.second == true);
	}

	return TRUE;
}

BOOL AwakeningInfoParser::LoadScript( eSCRIPT_CODE script_code, BOOL is_reload )
{
	switch ( script_code )
	{
	case SCRIPT_AWAKENING_INFO:
		return _Load(is_reload);
	}

	return FALSE;
}

const AwakeningInfo* AwakeningInfoParser::FindData( const CODETYPE& item_code ) const
{
	AwakeningInfo_Hash::const_iterator begin_iterator = awakeninginfo_hash_.begin(),
		end_iterator = awakeninginfo_hash_.end();

	for ( ; begin_iterator != end_iterator; ++begin_iterator )
	{
		const AwakeningInfo* find_info = begin_iterator->second;
		if ( find_info->item_code == item_code )
		{
			return find_info;
		}
	}

	return NULL;
}

RC::eITEM_RESULT 
AwakeningInfoParser::ResultAwakeningCheck( const SCItemSlot& equip_item, 
										   const SCItemSlot& material_item, 
										   const SCItemSlot& additive_item )
{
	RC::eITEM_RESULT result_code = PermitEquipmentCheck(equip_item);
	if ( result_code != RC::RC_ITEM_SUCCESS )
	{
		return result_code;
	}

#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	result_code = PermitMaterialCheck(equip_item.GetCode(), material_item.GetCode(), material_item.GetNum());
#else
	result_code = PermitMaterialCheck(equip_item.GetCode(), material_item.GetCode());
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	if ( result_code != RC::RC_ITEM_SUCCESS )
	{
		return result_code;
	}

	result_code = PermitAdditiveCheck(equip_item, material_item, additive_item);
	if ( result_code != RC::RC_ITEM_SUCCESS )
	{
		return result_code;
	}

	return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT 
AwakeningInfoParser::PermitEquipmentCheck( const SCItemSlot& equip_item )
{
	if (equip_item.IsShellItem() == true || equip_item.IsEtheriaItem() == true)
	{
		return RC::RC_EQUIPMENT_AWAKENING_INVALID_EQUIP_INFO;
	}

	const AwakeningInfo* awakening_info = FindData(equip_item.GetCode());
	if ( awakening_info == NULL )
	{
		return RC::RC_EQUIPMENT_AWAKENING_INVALID_EQUIP_INFO;
	}

	if ( equip_item.GetAwakening() >= MAX_AWAKENING_GRADE )
	{
		return RC::RC_EQUIPMENT_AWAKENING_EQUIP_MAX_LEVEL;
	}

	if ( equip_item.GetAwakening() >= awakening_info->awake_permit_level )
	{
		return RC::RC_EQUIPMENT_AWAKENING_EQUIP_EXCEEDED_PERMIT_LEVEL;
	}

	return RC::RC_ITEM_SUCCESS;
}

#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
RC::eITEM_RESULT 
AwakeningInfoParser::PermitMaterialCheck( const CODETYPE equip_item_code, 
										 const CODETYPE material_item_code, 
										 const BYTE material_item_num )
{
	const AwakeningInfo* awakening_info = FindData(equip_item_code);
	if ( awakening_info == NULL )
	{
		return RC::RC_EQUIPMENT_AWAKENING_INVALID_EQUIP_INFO;
	}
	
	// 재료 정보가 없을 때 만능재료 검사.
	return IsAllroundMaterial(equip_item_code, material_item_code, material_item_num);
	
}
#else

RC::eITEM_RESULT 
AwakeningInfoParser::PermitMaterialCheck( const CODETYPE equip_item_code, 
										  const CODETYPE material_item_code )
{
	const AwakeningInfo* awakening_info = FindData(equip_item_code);
#ifdef _NA_000000_20150424_AWAKENING_FROM_SEPARATION
    const AwakeningInfo* awakening_material_info = FindData(material_item_code);
#endif // _NA_000000_20150424_AWAKENING_FROM_SEPARATION
	if ( awakening_info == NULL )
	{
		return RC::RC_EQUIPMENT_AWAKENING_INVALID_EQUIP_INFO;
	}

#ifdef _NA_000000_20150424_AWAKENING_FROM_SEPARATION
    if ( awakening_material_info == NULL )
#endif // _NA_000000_20150424_AWAKENING_FROM_SEPARATION
	{
		// 재료 정보가 없을 때 만능재료 검사.
		if ( IsAllroundMaterial(equip_item_code, material_item_code) == true )
		{
			return RC::RC_ITEM_SUCCESS;
		}

		return RC::RC_EQUIPMENT_AWAKENING_INVALID_MATERIAL_INFO;
	}
#ifdef _NA_000000_20150424_AWAKENING_FROM_SEPARATION
	else
	{
		
		if ( awakening_info->awake_separation == awakening_material_info->awake_separation )
		{
			return RC::RC_ITEM_SUCCESS;
		}
	}

	return RC::RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_MATERIAL;
#endif // _NA_000000_20150424_AWAKENING_FROM_SEPARATION
}
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD

RC::eITEM_RESULT 
AwakeningInfoParser::PermitAdditiveCheck( const SCItemSlot& equip_item, 
										  const SCItemSlot& material_item, 
								    	  const SCItemSlot& additive_item )
{
	if (additive_item.IsShellItem() == true)
	{
		return RC::RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_ADDITIVE;
	}

	const AwakeningInfo* awakening_info = FindData(equip_item.GetCode());
	if ( awakening_info == NULL )
	{
		return RC::RC_EQUIPMENT_AWAKENING_INVALID_EQUIP_INFO;
	}

	int result_awakening_level = GetResultAwakening(equip_item, material_item.GetCode(), material_item.GetAwakening());
    
	int need_additive_number = awakening_info->additive_count;

	std::map<BYTE, DWORD>::const_iterator iter;
	iter = awakening_info->awakening_level_info_map.find(result_awakening_level);
	if ( iter == awakening_info->awakening_level_info_map.end() )
	{
		return RC::RC_EQUIPMENT_AWAKENING_INVALID_AWAKENING_INFO;
	}

	if ( additive_item.GetCode() != awakening_info->additive_item_code )
	{
		return RC::RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_ADDITIVE;
	}

	if ( additive_item.GetNum() < need_additive_number )
	{
		return RC::RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_ADDITIVE_AMOUNT;
	}

	return RC::RC_ITEM_SUCCESS;
}

const DWORD 
AwakeningInfoParser::GetOptionValue( const CODETYPE equip_item_code, 
                                     const BYTE awakening_grade )
{
    const AwakeningInfo* awakening_info = FindData(equip_item_code);
    if ( awakening_info == NULL )
    {
        return 0;
    }

    if (awakening_grade <= 0 || awakening_grade > MAX_AWAKENING_GRADE)
    {
        return 0;
    }

    std::map<BYTE, DWORD>::const_iterator iter;
    iter = awakening_info->awakening_level_info_map.find(awakening_grade);
    if ( iter == awakening_info->awakening_level_info_map.end() )
    {
        return 0;
    }

    return  (iter->second);
}

const BYTE 
AwakeningInfoParser::GetResultAwakening( const SCItemSlot& equip_item, 
                                         const CODETYPE material_item_code, 
                                         const BYTE material_awakening)
{
    BYTE material_awakening_grade = 0;
    const BASE_ITEMINFO* material_info = ItemInfoParser::Instance()->GetItemInfo(material_item_code);
    if (material_info != NULL)
    {
        if (material_info->m_wType == eITEMTYPE_AWAKENING_ALLROUND_MATERIAL)
        {
            material_awakening_grade = material_info->m_iOptionValue[0];
        }
        else
        {
            material_awakening_grade = material_awakening + 1;
        }
    }

    BYTE result_awakening_grade = equip_item.GetAwakening() + material_awakening_grade;
    if ( result_awakening_grade >= MAX_AWAKENING_GRADE )
    {
        result_awakening_grade = MAX_AWAKENING_GRADE;
    }

    const AwakeningInfo* awakening_info = FindData(equip_item.GetCode());
    if ( awakening_info != NULL && result_awakening_grade >= (awakening_info->awake_permit_level) )
    {
        result_awakening_grade = awakening_info->awake_permit_level;
    }

    return result_awakening_grade;
}

const int 
AwakeningInfoParser::GetSuccessProbability( const SCItemSlot& equip_item, 
                                            const CODETYPE material_item_code, 
                                            const BYTE material_awakening )
{
    BYTE result_awakening = AwakeningInfoParser::Instance()->GetResultAwakening(equip_item, material_item_code, material_awakening);

    const AwakeningInfo* awakening_info = FindData(equip_item.GetCode());
    if (awakening_info == NULL)
    {
        return 0;
    }

    return awakening_info->probability_success;
}

#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
RC::eITEM_RESULT 
AwakeningInfoParser::IsAllroundMaterial( const SLOTCODE equip_item_code, 
										 const CODETYPE material_item_code, 
										 const BYTE material_item_num )
{
	const AwakeningInfo* awakening_info = FindData(equip_item_code);
	if ( awakening_info == NULL )
	{
		return RC::RC_EQUIPMENT_AWAKENING_INVALID_EQUIP_INFO;
	}

	RC::eITEM_RESULT result_code = RC::RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_MATERIAL;

	std::list<AllRoundMaterialInfo>::const_iterator cur_iter = 
		awakening_info->allround_material_info_list.begin();
	std::list<AllRoundMaterialInfo>::const_iterator end_iter = 
		awakening_info->allround_material_info_list.end();
	for ( ; cur_iter != end_iter; ++ cur_iter )
	{
		if ( (*cur_iter).allround_material_code == material_item_code )
		{
			if ((*cur_iter).allround_material_count > material_item_num)
			{
				result_code = RC::RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_MATERIAL_AMOUNT;
				break;
			}

			const BASE_ITEMINFO* material_info = 
				ItemInfoParser::Instance()->GetItemInfo(material_item_code);
			if (material_info == NULL)
			{
				continue;
			}

			if (material_info->m_wType == eITEMTYPE_AWAKENING_ALLROUND_MATERIAL)
			{
				return RC::RC_ITEM_SUCCESS;
			}
		}
	}

	return result_code;
}
#else

const bool 
AwakeningInfoParser::IsAllroundMaterial( const SLOTCODE equip_item_code, 
                                         const CODETYPE material_item_code )
{
    const AwakeningInfo* awakening_info = FindData(equip_item_code);
    if ( awakening_info == NULL )
    {
        return false;
    }

    std::set<CODETYPE>::const_iterator cur_iter = 
        awakening_info->allround_material_code_map.begin();
    std::set<CODETYPE>::const_iterator end_iter = 
        awakening_info->allround_material_code_map.end();
    for ( ; cur_iter != end_iter; ++ cur_iter )
    {
        if ( *cur_iter == material_item_code)
        {
            const BASE_ITEMINFO* material_info = 
                ItemInfoParser::Instance()->GetItemInfo(material_item_code);
            if (material_info == NULL)
            {
                continue;
            }

            if (material_info->m_wType == eITEMTYPE_AWAKENING_ALLROUND_MATERIAL)
            {
                return true;
            }
        }
    }

    return false;
}
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD

RC::eITEM_RESULT AwakeningInfoParser::PermitEvolutionEquipmentCheck( const SCItemSlot& equip_item )
{
	if (equip_item.IsShellItem() == true)
	{
		return RC::RC_EQUIPMENT_EVOLUTION_ITEM_CAN_NOT_EVOLUTION;
	}

	const AwakeningInfo* awakening_info = FindData(equip_item.GetCode());
	if ( awakening_info == NULL )
	{
		return RC::RC_EQUIPMENT_EVOLUTION_ITEM_CAN_NOT_EVOLUTION;
	}

	if ( awakening_info->evolution_item_code == 0 )
	{
		return RC::RC_EQUIPMENT_EVOLUTION_ITEM_CAN_NOT_EVOLUTION;
	}

	if ( equip_item.GetAwakening() < MAX_AWAKENING_GRADE )
	{
		return RC::RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_AWAKENING_GRADE;
	}

	return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT AwakeningInfoParser::PermitEvolutionAdditiveCheck( const SCItemSlot& equip_item, const SCItemSlot& additive_item )
{
	if (additive_item.IsShellItem() == true)
	{
		return RC::RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_ADDITIVE;
	}

	const AwakeningInfo* awakening_info = FindData(equip_item.GetCode());
	if ( awakening_info == NULL )
	{
		return RC::RC_EQUIPMENT_EVOLUTION_ITEM_CAN_NOT_EVOLUTION;
	}

	if ( awakening_info->evolution_additive_item_code != additive_item.GetCode() )
	{
		return RC::RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_ADDITIVE;
	}

	if ( additive_item.GetNum() < awakening_info->evolution_additive_count )
	{
		return RC::RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_ADDITIVE_AMOUT;
	}

	return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT AwakeningInfoParser::ResultEvolutionCheck( const SCItemSlot& equip_item, const SCItemSlot& additive_item )
{
	RC::eITEM_RESULT result_code = PermitEvolutionEquipmentCheck(equip_item);
	if ( result_code != RC::RC_ITEM_SUCCESS )
	{
		return result_code;
	}

	result_code = PermitEvolutionAdditiveCheck(equip_item, additive_item);
	if ( result_code != RC::RC_ITEM_SUCCESS )
	{
		return result_code;
	}

	return RC::RC_ITEM_SUCCESS;
}

const CODETYPE AwakeningInfoParser::GetResultEvolutionItemCode( const CODETYPE& equip_item_code )
{
	const AwakeningInfo* awakening_info = FindData(equip_item_code);
	if ( awakening_info == NULL )
	{
		return 0;
	}

	return awakening_info->evolution_item_code;
}

const CODETYPE AwakeningInfoParser::GetEvolutionAdditiveItemCode( const CODETYPE& equip_item_code )
{
	const AwakeningInfo* awakening_info = FindData(equip_item_code);
	if ( awakening_info == NULL )
	{
		return 0;
	}

	return awakening_info->evolution_additive_item_code;
}

const BYTE AwakeningInfoParser::GetEvolutionAdditiveCount( const CODETYPE& equip_item_code )
{
	const AwakeningInfo* awakening_info = FindData(equip_item_code);
	if ( awakening_info == NULL )
	{
		return 0;
	}

	return awakening_info->evolution_additive_count;
}

const int AwakeningInfoParser::GetEvolutionSuccessProbability( const CODETYPE& equip_item_code )
{
    const AwakeningInfo* awakening_info = FindData(equip_item_code);
    if ( awakening_info == NULL )
    {
        return 0;
    }

    return awakening_info->evolution_probability_success;
}

#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
bool AwakeningInfoParser::GetAllRoundMaterialInfo_First( const CODETYPE& equip_item_code, OUT AllRoundMaterialInfo& allround_material_info )
{
	const AwakeningInfo* awakening_info = FindData(equip_item_code);

	if (awakening_info == NULL)
	{
		return false;
	}

	std::list<AllRoundMaterialInfo>::const_iterator iter = awakening_info->allround_material_info_list.begin();
	if (iter == awakening_info->allround_material_info_list.end())
	{
		return false;
	}

	allround_material_info = (*iter);

	return true;
}

int AwakeningInfoParser::GetAllRoundMaterialNum( const CODETYPE& equip_item_code, const CODETYPE& material_code )
{
	const AwakeningInfo* awakening_info = FindData(equip_item_code);

	if (awakening_info == NULL)
	{
		return false;
	}

	std::list<AllRoundMaterialInfo>::const_iterator iter = awakening_info->allround_material_info_list.begin();
	for ( ; iter != awakening_info->allround_material_info_list.end(); ++iter )
	{
		if ((*iter).allround_material_code == material_code)
		{
			return (*iter).allround_material_count;
		}
	}

	return 0;
}

#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
