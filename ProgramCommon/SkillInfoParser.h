#pragma once

#ifndef GAMESERVER_SKILL_INFO_PARSER_H_
#define GAMESERVER_SKILL_INFO_PARSER_H_

#include <Struct.h>
#include <ScriptCode.IParser.h>


struct SKILL_COOLTIME_INFO
{
    SKILL_COOLTIME_INFO() : 
        m_SkillCode(0), m_dwCoolTime(0)
    {
    }
    SLOTCODE m_SkillCode;
    DWORD    m_dwCoolTime;
};

class SkillScriptInfo : public BASE_SKILLINFO
{
public:
    SkillScriptInfo() : 
        iterator_index_for_old_interface_(0)
    {
    }
    ~SkillScriptInfo()
    {
    }

public:
    const BASE_ABILITYINFO* GetAbilityInfo(const int ability_id) const
    {
        for (AbilityInfos::const_iterator it = ability_infos_.begin(); 
            it != ability_infos_.end(); ++it)
        {
            const BASE_ABILITYINFO& ability_info = *it;
            if (ability_info.m_wAbilityID == ability_id)
            {
                return &ability_info;
            }
        }
        return NULL;
    }
    BASE_ABILITYINFO* GetAbilityInfo(const int ability_id)
    {
        for (AbilityInfos::iterator it = ability_infos_.begin(); 
            it != ability_infos_.end(); ++it)
        {
            BASE_ABILITYINFO& ability_info = *it;
            if (ability_info.m_wAbilityID == ability_id)
            {
                return &ability_info;
            }
        }
        return NULL;
    }
    const BASE_ABILITYINFO* GetAbilityInfoByIndex(const BYTE ability_index) const
    {
        if (ability_index < 0 || ability_index >= ability_infos_.size())
        {
            return NULL;
        }
        return &(ability_infos_[ability_index]);
    }
    BASE_ABILITYINFO* GetAbilityInfoByIndex(const BYTE ability_index)
    {
        if (ability_index < 0 || ability_index >= ability_infos_.size())
        {
            return NULL;
        }
        return &(ability_infos_[ability_index]);
    }

    void SetFirst() const 
    { 
        iterator_index_for_old_interface_ = 0; 
    }
    const BASE_ABILITYINFO* GetNext() const
    {
        if (iterator_index_for_old_interface_ >= ability_infos_.size())
        {
            return NULL;
        }
        return &(ability_infos_[iterator_index_for_old_interface_++]);
    }
    BASE_ABILITYINFO* GetNext()
    {
        if (iterator_index_for_old_interface_ >= ability_infos_.size())
        {
            return NULL;
        }
        return &(ability_infos_[iterator_index_for_old_interface_++]);
    }

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    bool IsPassiveSkill() const // 토글 중의 패시브 스킬(SkillInfo.txt 스킬타입과 무관하게 작동)
    {
        return m_SkillClassCode == eSKILL_RISINGFORCE;
    }
    bool IsToggleSkill() const
    {
        return             
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
			m_SkillClassCode == eSKILL_SPIRIT_RECHARGE || // 스피릿리차지
#endif // _NA_008472_20150903_IMPROVED_BALANCE
            m_SkillClassCode == eSKILL_WITCHBLADE_1111 || // 바이스소드
            m_SkillClassCode == eSKILL_WITCHBLADE_1112 || // 이베이드            
            m_SkillClassCode == eSKILL_WITCHBLADE_1102 || // 피드온레이지
            m_SkillClassCode == eSKILL_RISINGFORCE;       // 라이징포스
    }

    bool IsAuraSkill() const
    {
        return 
            m_SkillClassCode == eSKILL_HELLROID_1004 || // 슬로우오라
            m_SkillClassCode == eSKILL_HELLROID_1006 || // 위크너스오라
            m_SkillClassCode == eSKILL_HELLROID_1011 || // 미스챈스오라
            m_SkillClassCode == eSKILL_HELLROID_1013 || // 디클라인오라
            m_SkillClassCode == eSKILL_HELLROID_1020 || // 리커버리오라
            m_SkillClassCode == eSKILL_HELLROID_1022 || // 부스트오라
            m_SkillClassCode == eSKILL_HELLROID_1027 || // 이그노어오라
            m_SkillClassCode == eSKILL_HELLROID_1029;   // 컨센트레이션오라
    }

    bool IsToggleEffectSkill() const
    {
        return 
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
			m_SkillClassCode == eSKILL_SPIRIT_RECHARGE    || // 스피릿리차지
#endif // _NA_008472_20150903_IMPROVED_BALANCE
            m_SkillClassCode == eSKILL_HELLROID_AURA_1032 || // 슬로우오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1033 || // 위크너스오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1034 || // 미스챈스오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1035 || // 디클라인오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1036 || // 리커버리오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1037 || // 부스트오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1038 || // 이그노어오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1039 || // 컨센트레이션오라
            m_SkillClassCode == eSKILL_WITCHBLADE_1112    || // 이베이드
            m_SkillClassCode == eSKILL_WITCHBLADE_1102    || // 피드온레이지
            m_SkillClassCode == eSKILL_RISINGFORCE;          // 라이징포스
    }
#else
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
public:
    bool IsAuraSkill() const
    {
        return 
            m_SkillClassCode == eSKILL_HELLROID_1004 || // 슬로우오라
            m_SkillClassCode == eSKILL_HELLROID_1006 || // 위크너스오라
            m_SkillClassCode == eSKILL_HELLROID_1011 || // 미스챈스오라
            m_SkillClassCode == eSKILL_HELLROID_1013 || // 디클라인오라
            m_SkillClassCode == eSKILL_HELLROID_1020 || // 리커버리오라
            m_SkillClassCode == eSKILL_HELLROID_1022 || // 부스트오라
            m_SkillClassCode == eSKILL_HELLROID_1027 || // 이그노어오라
            m_SkillClassCode == eSKILL_HELLROID_1029;   // 컨센트레이션오라
    }

    bool IsAuraEffectSkill() const
    {
        return 
            m_SkillClassCode == eSKILL_HELLROID_AURA_1032 || // 슬로우오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1033 || // 위크너스오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1034 || // 미스챈스오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1035 || // 디클라인오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1036 || // 리커버리오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1037 || // 부스트오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1038 || // 이그노어오라
            m_SkillClassCode == eSKILL_HELLROID_AURA_1039;   // 컨센트레이션오라

    }

#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

public:
    void AddAbility(const BASE_ABILITYINFO& ability_info)
    {
        if (ability_info.m_wAbilityID == 0)
        {
            return;
        }
        if (ability_infos_.size() >= MAX_ABILITY_NUM)
        {
            return;
        }
        ability_infos_.push_back(ability_info);
    }
	bool IsCastingSkill() const
    { 
        return m_wzidCastAniCode.id != StrToWzID("NULL") && m_wSkillCasting > 0; 
    }
	WzID GetCastingAnimationID() const
    { 
        return m_wzidCastAniCode.id; 
    }
	WORD GetCastingTime() const
    { 
        return m_wSkillCasting; 
    }

private:
    typedef STLX_VECTOR<BASE_ABILITYINFO> AbilityInfos;
    AbilityInfos ability_infos_;
    mutable BYTE iterator_index_for_old_interface_;

}; // SkillScriptInfo


// 클래스ID 순으로 정렬
typedef std::map<SLOTCODE, SKILL_COOLTIME_INFO>  COOLTIME_MAP;
typedef std::pair<SLOTCODE, SKILL_COOLTIME_INFO> COOLTIME_PAIR;
typedef COOLTIME_MAP::iterator                   COOLTIME_ITER;

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM

class SkillTree
{
public:
    struct SkillDescript
    {        
        uint8_t character_class_id;
        uint8_t skill_line;
        SLOTCODE skill_class;
        SLOTCODE upper_skill_class;
        SLOTCODE first_skill_code;

        SkillDescript()
        {
            ::ZeroMemory(this, sizeof(*this));
        }
    };

    SkillTree() 
    {
        // CHANGES: f110216.1L
        ZeroMemory(skill_descript, sizeof(skill_descript));
    }

    ~SkillTree() 
    {
    }

    uint get_line_count() const
    {
        return MAX_SKILLTREE_LINE;
    }

    uint get_column_count() const
    {
        return MAX_SKILLTREE_COLUMN;
    }

    const SkillDescript* get_skill_descript(uint line) const
    {
        if (line < MAX_SKILLTREE_LINE)
            return skill_descript[line];
        return NULL;
    }

    const SkillDescript* find_skill_descript(SLOTCODE skill_class) const
    {
        SkillHashMap::const_iterator iter = skill_hashmap.find(skill_class);
        if (iter != skill_hashmap.end())
            return iter->second;
        return NULL;
    }

private:
    enum 
    {
        MAX_SKILLTREE_LINE = 7,
        MAX_SKILLTREE_COLUMN = 10, // MAX_SKILL_INVENTORY_STORE_NUM 동일한 사이즈여야함.
    };

    friend class SkillInfoParser;
    typedef STLX_HASH_MAP<SLOTCODE, SkillDescript*> SkillHashMap;
    SkillHashMap  skill_hashmap;
    SkillDescript skill_descript[MAX_SKILLTREE_LINE][MAX_SKILLTREE_COLUMN];
};

#endif //_NA_0_20100901_SKILLTREE_SYSTEM

// 스킬 직업 트리
class SkillJobTree
{
public:
    // 스킬 설명
    struct Desc
    {
        int job_type; // 스킬 직업 종류
        SLOTCODE skill_class_code; // 스킬 종류 코드
        SLOTCODE upper_skill_class_code; // 상위 스킬 종류 코드
        SLOTCODE first_skill_code; // 같은 종류에 속한 첫번째 스킬 코드
        int line_index; // 줄번호 인덱스

        Desc()
        {
            ::ZeroMemory(this, sizeof(*this));
        }
    };

public:
    SkillJobTree()
    {
    }
    ~SkillJobTree()
    {
    }

public:
    // 초기화 한다.
    void Clear()
    {
        skill_class_map_.clear();
        ::ZeroMemory(desc_list_, sizeof(desc_list_));
    }

    // 줄 개수를 얻는다.
    int GetLineSize() const
    {
        return kMaxLineSize;
    }

    // 칼럼 개수를 얻는다.
    int GetColumnSize() const
    {
        return kMaxColumnSize;
    }

    // 스킬 설명을 얻는다.
    const Desc* GetDesc(const int line_index, const int column_index) const
    {
        if (line_index < 0 || 
            line_index >= kMaxLineSize || 
            column_index < 0 || 
            column_index >= kMaxColumnSize)
        {
            return NULL;
        }
        return &(desc_list_[line_index][column_index]);
    }

    // 스킬 설명을 얻는다.
    Desc* GetDescByMutable(const int line_index, const int column_index)
    {
        if (line_index < 0 || 
            line_index >= kMaxLineSize || 
            column_index < 0 ||
            column_index >= kMaxColumnSize)
        {
            return NULL;
        }
        return &(desc_list_[line_index][column_index]);
    }

    // 스킬 종류 코드로 스킬 설명을 삽입한다.
    bool InsertDesc(const Desc* desc)
    {
        if (desc == NULL || desc->skill_class_code == 0)
        {
            return false;
        }
        skill_class_map_[desc->skill_class_code] = desc;
        return true;
    }

    // 스킬 종류 코드로 스킬 설명을 찾는다.
    const Desc* FindDesc(const SLOTCODE skill_class_code) const
    {
        SkillClassMap::const_iterator it = skill_class_map_.find(skill_class_code);
        if (it == skill_class_map_.end())
        {
            return NULL;
        }
        return it->second;
    }


private:
    enum { kMaxLineSize = 7 };
    enum { kMaxColumnSize = 3 };
    typedef STLX_HASH_MAP<SLOTCODE, const Desc*> SkillClassMap;
    Desc desc_list_[kMaxLineSize][kMaxColumnSize];
    SkillClassMap skill_class_map_;

}; // SkillJobTree

// 스킬 정보 파서
class SkillInfoParser : public IParser
{
public:
    static SkillInfoParser* Instance()
    {
        static SkillInfoParser instance;
        return &instance;
    }
    static void DestroyInstance()
    {
    }

    virtual void Release()
    {
    }
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

public:
    void SetFirst() const
    {
        script_infos_const_iterator = script_infos_.begin();
        style_infos_const_iterator = style_infos_.begin();
    }
    void SetFirst()
    {
        script_infos_iterator = script_infos_.begin();
        style_infos_iterator = style_infos_.begin();
    }
    const SkillScriptInfo* GetNextSkill() const
    {
        if (script_infos_const_iterator == script_infos_.end())
        {
            return NULL;
        }
        const SkillScriptInfo* const script_info = &(script_infos_const_iterator->second);
        ++script_infos_const_iterator;
        return script_info;
    }
    SkillScriptInfo* GetNextSkill()
    {
        if (script_infos_iterator == script_infos_.end())
        {
            return NULL;
        }
        SkillScriptInfo* const script_info = &(script_infos_iterator->second);
        ++script_infos_iterator;
        return script_info;
    }
    const BASE_STYLEINFO* GetNextStyle() const
    {
        if (style_infos_const_iterator == style_infos_.end())
        {
            return NULL;
        }
        const BASE_STYLEINFO* const style_info = &(style_infos_const_iterator->second);
        ++style_infos_const_iterator;
        return style_info;
    }
    BASE_STYLEINFO* GetNextStyle()
    {
        if (style_infos_iterator == style_infos_.end())
        {
            return NULL;
        }
        BASE_STYLEINFO* const style_info = &(style_infos_iterator->second);
        ++style_infos_iterator;
        return style_info;
    }    
    const SkillScriptInfo* GetSkillInfo(const int skill_code) const
    {
        ScriptInfos::const_iterator it = script_infos_.find(skill_code);
        if (it == script_infos_.end())
        {
            return NULL;
        }
        return &(it->second);
    }
    SkillScriptInfo* GetSkillInfo(const int skill_code)
    {
        ScriptInfos::iterator it = script_infos_.find(skill_code);
        if (it == script_infos_.end())
        {
            return NULL;
        }
        return &(it->second);
    }
    const BASE_STYLEINFO* GetStyleInfo(const int style_code) const
    {
        StyleInfos::const_iterator it = style_infos_.find(style_code);
        if (it == style_infos_.end())
        {
            return NULL;
        }
        return &(it->second);
    }    
    BASE_STYLEINFO* GetStyleInfo(const int style_code)
    {
        StyleInfos::iterator it = style_infos_.find(style_code);
        if (it == style_infos_.end())
        {
            return NULL;
        }
        return &(it->second);
    }
    const ROOT_SKILLINFO* GetInfo(const int root_skill_code) const
    {
        const ROOT_SKILLINFO* const skill_info = GetSkillInfo(root_skill_code);
        if (skill_info == NULL)
        {
            return GetStyleInfo(root_skill_code);
        }
        return skill_info;
    }
    ROOT_SKILLINFO* GetInfo(const int root_skill_code)
    {
        ROOT_SKILLINFO* const skill_info = GetSkillInfo(root_skill_code);
        if (skill_info == NULL)
        {
            return GetStyleInfo(root_skill_code);
        }
        return skill_info;
    }

    COOLTIME_MAP* GetSkillCoolTimeMap()
    {
        return &skill_cooltime_table_;
    }

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    void CalculateSkillLineFromSkillInventoryStoreParser();
    const SkillTree* get_skill_tree(eCHAR_TYPE character_type) const
    {
        if (character_type < eCHAR_TYPE_MAX)
            return &(skill_tree_[character_type]);
        return NULL;
    }
#endif
    // 스킬 직업 트리를 얻는다.
    const SkillJobTree* GetJobTree(const int char_type, const int job_type) const
    {
        return job_tree_manager_.GetTree(char_type, job_type);
    }

    // 스킬 직업 개수를 얻는다.
    int GetJobSize() const
    {
        return job_tree_manager_.GetJobSize();
    }

    // 스킬 직업 설명을 찾는다.
    const SkillJobTree::Desc* FindJobDesc(
        const int char_type, const int job_type, const SLOTCODE skill_class_code) const
    {
        const SkillJobTree* const job_tree = job_tree_manager_.GetTree(char_type, job_type);
        if (job_tree == NULL)
        {
            return NULL;
        }
        return job_tree->FindDesc(skill_class_code);
    }

    // 모든 스킬 직업 대상으로 스킬 설명을 찾는다.
    const SkillJobTree::Desc* FindAllJobDesc(
        const int char_type, const SLOTCODE skill_class_code) const
    {
        const int job_size = job_tree_manager_.GetJobSize();
        for (int job_type = 0; job_type < job_size; ++job_type)
        {
            const SkillJobTree::Desc* const job_desc = 
                FindJobDesc(char_type, job_type, skill_class_code);
            if (job_desc != NULL)
            {
                return job_desc;
            }
        }

        return NULL;
    }

private:
    typedef STLX_MAP<int, SkillScriptInfo> ScriptInfos;
    typedef STLX_MAP<int, BASE_STYLEINFO> StyleInfos;

private:
    // 스킬 직업 트리 관리자
    class SkillJobTreeManager
    {
    public:
        SkillJobTreeManager()
        {
        }
        ~SkillJobTreeManager() 
        {
        }

    public:
        // 초기화 한다.
        void Clear()
        {
            for (int char_type = 0; char_type < kMaxCharTypeSize; ++char_type)
            {
                for (int job_type = 0; job_type < kMaxJobSize; ++job_type)
                {
                    SkillJobTree& job_tree = trees_[char_type][job_type];
                    job_tree.Clear();
                }
            }
        }

        // 트리를 읽는다.
        void LoadTree(const SkillInfoParser* skill_info_parser);

        // 스킬 직업 개수를 얻는다.
        int GetJobSize() const
        {
            return kMaxJobSize;
        }
        
        // 트리를 얻는다.
        const SkillJobTree* GetTree(const int char_type, const int job_type) const
        {
            if (char_type < 0 || 
                char_type >= kMaxCharTypeSize || 
                job_type < 0 || 
                job_type >= kMaxJobSize)
            {
                return NULL;
            }
            return &(trees_[char_type][job_type]);
        }

        // 트리를 얻는다.
        SkillJobTree* GetTreeByMutable(const int char_type, const int job_type)
        {
            if (char_type < 0 || 
                char_type >= kMaxCharTypeSize || 
                job_type < 0 || 
                job_type >= kMaxJobSize)
            {
                return NULL;
            }
            return &(trees_[char_type][job_type]);
        }

    private:
        enum { kMaxCharTypeSize = eCHAR_TYPE_MAX };
        enum { kMaxJobSize = 2 };
        SkillJobTree trees_[kMaxCharTypeSize][kMaxJobSize];

    }; // SkillJobTreeManager

private:
    SkillInfoParser();
    ~SkillInfoParser();

private:
    bool _Load(const bool is_reload);
    bool _LoadStyle(const bool is_reload = false);
    bool CheckValidity(SkillScriptInfo* const skill_info, const int row) const;   // 스크립트의 유효성 체크!
    
private:
    COOLTIME_MAP skill_cooltime_table_;
    
    ScriptInfos script_infos_;
    ScriptInfos::iterator script_infos_iterator;
    mutable ScriptInfos::const_iterator script_infos_const_iterator;

    StyleInfos style_infos_;
    StyleInfos::iterator style_infos_iterator;
    mutable StyleInfos::const_iterator style_infos_const_iterator;

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    SkillTree skill_tree_[eCHAR_TYPE_MAX];
    SkillJobTreeManager job_tree_manager_;
#endif
};

#endif // GAMESERVER_SKILL_INFO_PARSER_H_