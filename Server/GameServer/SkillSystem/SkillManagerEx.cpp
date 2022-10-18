#include "stdafx.h"
#include "SkillManager.h"
#include <SkillInfoParser.h>
//
#include "SkillFactory.h"
#include "Skill.h"

//==================================================================================================

SkillManager::SkillManager(Character* owner)
{
    owner_ = owner;
}

SkillManager::~SkillManager()
{
    Release();
}

void SkillManager::Init()
{
    active_skills_.resize(0);
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    autocast_skills_.clear();
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
}

void SkillManager::Release()
{
    // (WAVERIX) (080603) (CHANGES) - decrease instance count
    SkillFactory* const factory = SkillFactory::Instance();
    FOREACH_CONTAINER(const SKILL_LIST::value_type& node, active_skills_, SKILL_LIST)
    {
        Skill* skill = node.skill_;
        skill->Release();
        factory->FreeSkill(skill);
    }
    active_skills_.resize(0);
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    autocast_skills_.clear();
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
}

void SkillManager::ReleaseDead()
{
    //  (SOURCE)(NOTE)
    //  -   Owner�� ������� �� ��� Release() ���ѹ����� Update() ���̴� ��ų�� ������ �߻��� �� �ִ�.
    //  ���� STL�� ����ϴ� ��� �ڷ����� ��� ������ ���Ѵ�.
    //
    //  (WAVERIX) (080603) (NOTE) - ��� �߿� �� ������ ���̴�.
    //  - �ȵ� ���� ����, �߸��ϴ� �͵� �ƴϴ�. �߰��ϴ� �� ���̴�.
    //  - �ʱ� ����� ���ȭ �� ��ü��� �Ϳ� ���� ����ϰ� ���� �� ������Ÿ�� ���� �ۼ��ϴ� ���� ù°...
    //  - �̹� �ۼ��� �Ϳ� ���� ����å�� �� ������ ���� ������� ��ȸ�ϴ� �� ���� ��� �� �ϳ�...
    //  - ���� ���� ����� �������� �˰��򿡼� ����ϴ� Coloring ��� �߿� �ϳ��̴�. �� �Ʒ��� ����
    //  Ư�� ���¿� ���� ��ŷ ����� �ܺ��� ������ �޾ƾ� �Ѵٴ� ���� �״��� ����Ǵ� ���¶�� ����� �����.
    //  ���� ������ ���ܳ��ų� ��Ÿ ���� �ֱ������� �̽� �߻� Ȯ���� ����� �����̴�.
    //  - ������ ���� ���� �̷� ������� ��ȯ�� ����� �����Ÿ� ���� ����. �߻��ϸ� �����ϸ� ���� ��...
    //
    //  (WAVERIX) (080603) (THINKING)
    //  - Ȯ���� �����ϴ� ������� �鿡�� ���� ��, �⺻ ���ε��� �������� ������.
    //  ���� ��� ���� �������̽��� ��ȭ�� �ּ�ȭ �ϰ� ��� ���ο��� �ذ��� �� �ִ� ��,
    //  �Ǵ� ��/�� �ܰ迡���� Wrapper Layer�� �δ� ��(�� ���� ���� �Ұ� ���̺귯���� ���� �߻�ȭ���� ��ǥ��)�� �켱��������,
    //  ���ݱ��� ���� ��κ��� ��� ������ ������ ���ϴ� ���¸� ���Ѵ�. �Ƹ��� ��κ��� ���̺귯���� ������
    //  �����ϴٴ� ������ ���ȭ�� ���� �������� ������� �۾����� �ʳ� �����ȴ�.
    //  -   Example) SyncIterator <-> SetSkillRemove
    //
    //  - ������� ������̾� �ְ�����, �츮 ���� �������� �뼼�̹Ƿ� ���⿡ ������� ����.
    //  Code Complete������ ����� ������ �Ϲ�ȭ�� ���� ������̴��� �츮������ ���� �ʴ� ����� �� �ִ�.
    //  Ȯ���� �������� ������ �Ѿ���� ���ڰ� �� �����ϱ� ���ϰ� �� ���� ���� ���ɼ��� ����.
    //  ���� ������ ���� ���� ��ĺ��� ȭ���� �����丵�� �� ���� ���� �ִ�.
    //  (����: ���߿� �Ը� Ŀ���� ��ġ... ���İ�Ƽ�� ��ġ...)
    //  �������� �����ڵ� ��� ���� �ʰ� ���α׷� �Ը� �߰� �����̹Ƿ� �ռ� ����� �ξ� ���� �Ŷ� ���δ�.

    FOREACH_CONTAINER(const SKILL_LIST::value_type& node, active_skills_, SKILL_LIST)
    {
        Skill* skill = node.skill_;
        skill->SetSkillRemove(true);
    }
}

void SkillManager::Update()
{
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    _ExecuteAutoCastSkill();
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    if (active_skills_.empty()) {
        return;
    }
    //
    DWORD cur_tick = GetTickCount();
    SkillFactory* const factory = SkillFactory::Instance();
    //
    SKILL_LIST::iterator it = active_skills_.begin();
    const SKILL_LIST::const_iterator end = active_skills_.end();
    while (it != end)
    {
        Skill* skill = it->skill_;
        bool need_delete = false;
        if (skill->IsSkillCancel())
        {
            skill->CancelExcute();
            need_delete = true;
        }
        else if (cur_tick >= skill->GetExecuteTick() ||
                 skill->IsSkillRemove())
        {
            skill->EndExecute();
            
            if (owner_->IsEqualObjectKind(PLAYER_OBJECT) && 
                skill->GetSkillClassCode() != eSKILL_HIDE)
            {
                Player* owner_player = static_cast<Player*>(owner_) ;
                owner_player->Player_TransOff();
            }

            need_delete = true;
        }
        //
        if (need_delete)
        {
            //skill->Release();
            factory->FreeSkill(skill);
            it = active_skills_.erase(it);
            continue;
        };
        // last point
        ++it;
    }
}

void SkillManager::AddActiveSkill(Skill* skill)
{
    // depth 1 interface, you should be call on front level logic
    if (skill == NULL) {
        return;
    }
    //
    const SLOTCODE skill_code = skill->GetSkillCode();
    ;{
        SKILL_LIST::iterator found = FindActiveSkill(skill_code);
        if (found != active_skills_.end()) {
            RemoveActiveSkill(found); // need reconsideration
        }
    };
    skill->StartExecute();

    SkillNode node = { 0, };
    node.code_ = skill_code;
    node.skill_ = skill;
    active_skills_.push_back(node);
}

SkillManager::SKILL_LIST::iterator
SkillManager::RemoveActiveSkill(SKILL_LIST::iterator del_iterator)
{
    assert(del_iterator != active_skills_.end());
    Skill* skill = del_iterator->skill_;
    skill->Release();
    SkillFactory::Instance()->FreeSkill(skill);    
    return active_skills_.erase(del_iterator);
}

void SkillManager::CancelActiveSkill(SLOTCODE skill_code) const
{
    SKILL_LIST::const_iterator found = FindActiveSkill(skill_code);
    if (found == active_skills_.end()) {
        return;
    }
    Skill* skill = found->skill_;
    skill->SetSkillCancel(true);
}

void SkillManager::CancelAllSkill() const
{
    FOREACH_CONTAINER(const SKILL_LIST::value_type& node, active_skills_, SKILL_LIST)
    {
        Skill* skill = node.skill_;
        skill->SetSkillCancel(true);
    }
}

bool SkillManager::RegisterSkill(eSKILL_TYPE skill_type, const SkillInfo* skill_info)
{
    if (skill_type == SKILL_TYPE_PASSIVE) {
        return false;
    }
    // resolved, (WAVERIX) (090709) (THINKING NOTE) �߰��� ��ų�� �ٲ����� �ʴ´�. �ƴ� �ٲ�� �ٶ�� �ǰ�?
    // ���ȭ �Ǵ� �迭�� ���� ������ ���� Ȯ�� �������� ��ȯ�ϸ鼭 ��ü�ϵ��� ����.
    if (skill_info->root_skill_info_ == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|[%s]|Msg=null reference about a root_skill_info|SkillType=%u, SkillCode=%u|"),
               __FUNCTION__, skill_type, skill_info->skill_code_);
        return false;
    }

    SkillFactory* factory = SkillFactory::Instance();
    Skill* new_skill = factory->AllocSkill(skill_type, skill_info->root_skill_info_,
                                           skill_info->skill_delay_);

    new_skill->Init(owner_, skill_info, skill_info->root_skill_info_);
    // register a new skill in an owner skill container.
    AddActiveSkill(new_skill);

    return true;
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
bool SkillManager::SetAutoCastSkill( SLOTCODE skill_code, bool is_connected, Character* target /*= NULL*/ )
{
    if (autocast_skills_.find(skill_code) != autocast_skills_.end() ) {
        // ����ĳ��Ʈ ��ų�� ��ų�� 1������ �����ϱ��
        // ����¡���� ���� ��쿡 ��ø���� ���̰ɸ��� ��찡 �ִ�
        return false;
    }
    AutoSkillNode node;

    node.skill_code = skill_code;
    node.is_connected_skill = is_connected;
    if (target == NULL) {
        node.target = owner_;
    }
    else {
        node.target = target;
    }

    autocast_skills_[skill_code] = node;
    return true;
}

void SkillManager::_ExecuteAutoCastSkill()
{
    if (autocast_skills_.empty()) {
        return;
    }

    if(owner_ == NULL) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] SkillManager's Owner is Empty!!");
        return;
    }

    if (owner_->IsDead() == TRUE || owner_->IsDeleted() == true) {
        _ClearAutoCastSkills();
        return;
    }

    STLX_MAP<SLOTCODE, AutoSkillNode>::iterator node = autocast_skills_.begin();
    SkillScriptInfo* skill_info = SkillInfoParser::Instance()->GetSkillInfo(node->first);

    // Ÿ�� �˻�
    Character* skill_target = node->second.target;
    if (obj_util::IsValid(skill_target) == false) {
        autocast_skills_.erase(node);
        return;
    }
    if (skill_target->IsDead() == TRUE) 
    {
        autocast_skills_.erase(node);
        return;
    }

    SkillOption skill_option;
    skill_option.Clear();
    skill_option.AddOption(skill_option.eOption_HoldupBuffPostDead);

    const WzVector* cur_pos = owner_->GetPosPtr();
    SkillInfo skill_info_data(owner_, node->first, 0);
    skill_info_data.SetArgsObjectInfo(node->second.target->GetObjectKey(), cur_pos, cur_pos, skill_target->GetPosPtr());
    skill_info_data.SetArgsSkillExt(skill_info_data.kDefaultSequence,
        skill_info_data.kDefaultSkillEffect,
        skill_info_data.kDefaultSkillDelay,
        SKILL_FACTOR_NOMAL,
        static_cast<eATTACK_PROPENSITY>(ATTACK_PROPENSITY_NORMAL),
        0);
    skill_info_data.skill_option_.ApplyOption(skill_option);

    //���ὺų�� �׷��� ���� ��ų�� ����ϴ� �Լ��� �����Ѵ�
    if (node->second.is_connected_skill == false && owner_->IsEqualObjectKind(PLAYER_OBJECT) == true) 
    {
        Player* player = reinterpret_cast<Player*>(owner_);
        player->UseSkill(&skill_info_data,ATTACK_PROPENSITY_NORMAL);
    }
    else {
        RegisterSkill(SKILL_TYPE_ACTIVE, &skill_info_data);
    }

    autocast_skills_.erase(node);

    return;
}

void SkillManager::_ClearAutoCastSkills()
{
    autocast_skills_.clear();
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL



