#include "stdafx.h"
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

#include "PlayerAssistMeter.h"

#include "Player.h"
#include "Summoned.h"
#include "GameField.h"

//==================================================================================================
// Node
//==================================================================================================

//==================================================================================================
// PlayerAssistMeter
//==================================================================================================
void PlayerAssistMeter::Init()
{
    records_.clear();
}

PlayerAssistMeter::Node* PlayerAssistMeter::Add(const Player* _attacker)
{
    const CHARGUID guid = _attacker->GetCharGuid();
    PlayerAssistMeter::Node* node = GetNode(guid);

    if(node == NULL)
    {
        node = &records_[guid];
        node->SetCharGuid(guid);
    }

    return node;
}

Player* PlayerAssistMeter::GetBestAssistPlayer(const CHARGUID _killer_guid)
{
    GameField* current_field = player_->GetField();
    DWORD assister_guid=0;

    if(records_.empty() || current_field == NULL)
        return NULL;

    //레코드 정렬, map은 sort를 사용할 수 없으니, vector에 복사 후 수행
    VectorRecords temp_vector(records_.begin(), records_.end());

    std::sort(temp_vector.begin(), temp_vector.end(), GreaterSortRecords());

    //첫 번째 플레이어를 리턴, 단, killer는 제외한다
    VectorRecords::const_iterator itr = temp_vector.begin();
    if(itr->first == _killer_guid)
    {
        ++itr;
        if (itr == temp_vector.end())
            return NULL;
        assister_guid = itr->first;
    }
    else
        assister_guid =  itr->first;

    
    

    return PlayerManager::Instance()->FindPlayerByCharKey(assister_guid);
}

void PlayerAssistMeter::AddAssistPoint(Character* _attacker, const DWORD _damage)
{
    if(_attacker == NULL)
        return;
    
    CHARGUID char_guid = 0;
    if (_attacker->IsEqualObjectKind(PLAYER_OBJECT))
    {
        //자신을 공격한 오브젝트가 플레이어일 경우
        //guid를 가져오기 위해선 Player객체일 필요가 있음.
        //attacker 형변환 하기보단 PlayerOwner로 Player객체인 자기자신을 불러오자
        char_guid = _attacker->GetPlayerOwner()->GetCharGuid();
    }
    else if (_attacker->IsEqualObjectKind(SUMMON_OBJECT))
    {
        //자신을 공격한 오브젝트가 소환수일 경우
        Player* owner_player = _attacker->GetPlayerOwner();
        if(owner_player)
        {
            char_guid = owner_player->GetCharGuid();
            //공격자를 소환수의 주인으로 처리한다
            _attacker = owner_player;
        }
        else
            return ;
    }
    else //어태커가 플레이어도, 소환수도 아닐경우 그냥 리턴
        return;

    PlayerAssistMeter::Node* node = GetNode(char_guid);

    // 어시스트미터기에 등록되어있지않으면 등록시킨다.
    if(node == NULL)
    {
        node = Add(static_cast<Player*>(_attacker));
    }

    node->PlusAssistDamagePoint(_damage);
}

bool PlayerAssistMeter::Remove( const CHARGUID _char_guid )
{
    if(_char_guid)
    {
        Records::iterator itr = records_.find(_char_guid);
        if(itr != records_.end())
        {
            records_.erase(itr);
            return true;
        }
    }
    return false;
}

void PlayerAssistMeter::Clear()
{
    records_.clear();
}

#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST