#pragma once

#if defined(_NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM) || defined(_NA_008124_20150313_AWAKENING_SYSTEM)

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>
#include <SCItemSlot.h>

#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
struct AllRoundMaterialInfo
{
	CODETYPE allround_material_code;
	BYTE     allround_material_count;
};
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD

struct AwakeningInfo
{
    enum
    {
        MAX_ALLROUND_MATERIAL = 5,
    };

	// ������ �ڵ�
	CODETYPE item_code;
	// ������ �̸�
	std::string item_name;
	// ���� ����
	DWORD awake_separation;
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	std::list<AllRoundMaterialInfo> allround_material_info_list;
#else
	// ��������ڵ�
    std::set<CODETYPE> allround_material_code_map;
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	// ������뷹��
	BYTE awake_permit_level;
    // ÷���� ������ �ڵ�
    CODETYPE additive_item_code;
    // ÷���� ����
    BYTE additive_count;
    // ���� Ȯ��
    BYTE probability_success;
	// ������ �ɼ�
	std::map<BYTE, DWORD> awakening_level_info_map;
	// ��ȭ ������ �ڵ�
	CODETYPE evolution_item_code;
	// ��ȭ ÷���� ������ �ڵ�
	CODETYPE evolution_additive_item_code;
	// ��ȭ ÷���� ����
	BYTE evolution_additive_count;
	// ��ȭ ���� Ȯ��
	BYTE evolution_probability_success;

	AwakeningInfo()
	{
		item_code = 0;
		awake_separation = 0;
		awake_permit_level = 0;
		evolution_item_code = 0;
		evolution_additive_item_code = 0;
		evolution_additive_count = 0;
		evolution_probability_success = 0;
        additive_item_code = 0;
        additive_count = 0;
        probability_success = 0;
	}
};

class AwakeningInfoParser : public util::Singleton<AwakeningInfoParser>, public IParser
{
public:
	typedef STLX_HASH_MAP<CODETYPE, AwakeningInfo*> AwakeningInfo_Hash;

public:
	AwakeningInfoParser(void);
	~AwakeningInfoParser(void);

public:
	void Init(DWORD pool_size);
	void Init(DWORD pool_size, char* pszPackFileName);

	virtual void Release();
	virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

	AwakeningInfo_Hash& awakeninginfo_hash() { return awakeninginfo_hash_; }
	const AwakeningInfo* FindData(const CODETYPE& item_code) const;

public:
	// ���� ����
	RC::eITEM_RESULT PermitEquipmentCheck(const SCItemSlot& equip_item);
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	RC::eITEM_RESULT PermitMaterialCheck(const CODETYPE equip_item_code, 
		const CODETYPE material_item_code, const BYTE material_item_num);
#else
	RC::eITEM_RESULT PermitMaterialCheck(const CODETYPE equip_item_code, 
                                         const CODETYPE material_item_code);
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	RC::eITEM_RESULT PermitAdditiveCheck(const SCItemSlot& equip_item, 
                                         const SCItemSlot& material_item, 
                                         const SCItemSlot& additive_item);
	
	RC::eITEM_RESULT ResultAwakeningCheck(const SCItemSlot& equip_item, 
                                          const SCItemSlot& material_item, 
                                          const SCItemSlot& additive_item);
	
    const DWORD GetOptionValue(const CODETYPE equip_item_code, 
                               const BYTE awakening_grade);
    const BYTE  GetResultAwakening(const SCItemSlot& equip_item, 
                                   const CODETYPE material_item_code, 
                                   const BYTE material_awakening);
    const int   GetSuccessProbability(const SCItemSlot& equip_item, 
                                      const CODETYPE material_item_code, 
                                      const BYTE material_awakening);

#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	RC::eITEM_RESULT IsAllroundMaterial(const SLOTCODE equip_item_code, 
		const CODETYPE material_item_code, const BYTE material_item_num);
#else
    const bool  IsAllroundMaterial(const SLOTCODE equip_item_code, 
                                   const CODETYPE material_item_code);
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	bool GetAllRoundMaterialInfo_First(const CODETYPE& equip_item_code, OUT AllRoundMaterialInfo& allround_material_info);
	int  GetAllRoundMaterialNum(const CODETYPE& equip_item_code, const CODETYPE& material_code);
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD

public:
	// ��ȭ ����
	RC::eITEM_RESULT PermitEvolutionEquipmentCheck(const SCItemSlot& equip_item);
	RC::eITEM_RESULT PermitEvolutionAdditiveCheck(const SCItemSlot& equip_item,
		                                          const SCItemSlot& additive_item);
	RC::eITEM_RESULT ResultEvolutionCheck(const SCItemSlot& equip_item,
		                                  const SCItemSlot& additive_item);
	const CODETYPE   GetResultEvolutionItemCode(const CODETYPE& equip_item_code);
	const CODETYPE   GetEvolutionAdditiveItemCode(const CODETYPE& equip_item_code);
    const BYTE       GetEvolutionAdditiveCount(const CODETYPE& equip_item_code);
    const int        GetEvolutionSuccessProbability(const CODETYPE& equip_item_code);

private:
	BOOL _Load(BOOL is_reload);
	void Unload();

	AwakeningInfo_Hash awakeninginfo_hash_;
};
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM // _NA_008124_20150313_AWAKENING_SYSTEM
