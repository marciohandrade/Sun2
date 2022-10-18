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
    //  -   Owner가 사망했을 시 즉시 Release() 시켜버리면 Update() 중이던 스킬에 문제가 발생할 수 있다.
    //  따라서 STL을 사용하는 모든 자료형은 즉시 삭제를 금한다.
    //
    //  (WAVERIX) (080603) (NOTE) - 방안 중에 한 가지일 뿐이다.
    //  - 안될 것은 없고, 발명하는 것도 아니다. 발견하는 것 뿐이다.
    //  - 초기 설계시 모듈화 및 객체라는 것에 좀더 고민하고 설계 및 프로토타입 모델을 작성하는 것이 첫째...
    //  - 이미 작성된 것에 대한 보완책은 현 시점과 같은 방안으로 우회하는 것 역시 방안 중 하나...
    //  - 상태 변경 방법은 전통적인 알고리즘에서 언급하는 Coloring 방법 중에 하나이다. 단 아래와 같이
    //  특정 상태에 대한 마킹 방법이 외부의 지원을 받아야 한다는 점은 그다지 권장되는 형태라고 보기는 힘들다.
    //  유사 로직이 생겨나거나 기타 등등에서 주기적으로 미스 발생 확률이 생기기 때문이다.
    //  - 하지만 나도 이제 이런 방식으로 전환한 관계로 투덜거릴 여지 없다. 발생하면 수정하면 되지 뭐...
    //
    //  (WAVERIX) (080603) (THINKING)
    //  - 확실히 구현하는 방법적인 면에서 봤을 때, 기본 마인드의 차이점을 느낀다.
    //  나의 경우 기존 인터페이스의 변화는 최소화 하고 모듈 내부에서 해결할 수 있는 쪽,
    //  또는 전/후 단계에서의 Wrapper Layer를 두는 쪽(이 경우는 수정 불가 라이브러리에 대한 추상화층이 대표적)을 우선시하지만,
    //  지금까지 봐온 대부분의 경우 전방위 수정을 가하는 형태를 취한다. 아마도 대부분의 라이브러리에 접근이
    //  가능하다는 점에서 모듈화가 많이 무너지는 방식으로 작업되지 않나 추측된다.
    //  -   Example) SyncIterator <-> SetSkillRemove
    //
    //  - 나름대로 장단점이야 있겠지만, 우리 쪽은 후자쪽이 대세이므로 여기에 따르기로 하자.
    //  Code Complete에서도 언급할 정도로 일반화된 접근 방법론이더라도 우리팀에는 맞지 않는 방식일 수 있다.
    //  확실히 유지보수 측면은 넘어가더라도 후자가 더 개발하기 편하고 더 보기 편할 가능성도 높다.
    //  또한 자잘한 증분 개발 방식보다 화끈한 리팩토링이 더 나을 수도 있다.
    //  (예외: 나중에 규모가 커지면 골치... 스파게티면 골치...)
    //  무엇보다 개발자도 몇명 되지 않고 프로그램 규모도 중간 정도이므로 앞선 방식이 훨씬 나을 거라 보인다.

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
    // resolved, (WAVERIX) (090709) (THINKING NOTE) 중간에 스킬을 바꾸지는 않는다. 아님 바뀌길 바라는 건가?
    // 상수화 되는 계열에 대해 앞으로 좀더 확정 로직으로 전환하면서 대체하도록 하자.
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
        // 오토캐스트 스킬은 스킬당 1개씩만 예약하기로
        // 라이징포스 같은 경우에 중첩으로 많이걸리는 경우가 있다
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

    // 타겟 검사
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

    //연결스킬과 그렇지 않은 스킬이 사용하는 함수를 구분한다
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



