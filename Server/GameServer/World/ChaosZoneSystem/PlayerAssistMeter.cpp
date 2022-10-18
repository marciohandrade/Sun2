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

    //���ڵ� ����, map�� sort�� ����� �� ������, vector�� ���� �� ����
    VectorRecords temp_vector(records_.begin(), records_.end());

    std::sort(temp_vector.begin(), temp_vector.end(), GreaterSortRecords());

    //ù ��° �÷��̾ ����, ��, killer�� �����Ѵ�
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
        //�ڽ��� ������ ������Ʈ�� �÷��̾��� ���
        //guid�� �������� ���ؼ� Player��ü�� �ʿ䰡 ����.
        //attacker ����ȯ �ϱ⺸�� PlayerOwner�� Player��ü�� �ڱ��ڽ��� �ҷ�����
        char_guid = _attacker->GetPlayerOwner()->GetCharGuid();
    }
    else if (_attacker->IsEqualObjectKind(SUMMON_OBJECT))
    {
        //�ڽ��� ������ ������Ʈ�� ��ȯ���� ���
        Player* owner_player = _attacker->GetPlayerOwner();
        if(owner_player)
        {
            char_guid = owner_player->GetCharGuid();
            //�����ڸ� ��ȯ���� �������� ó���Ѵ�
            _attacker = owner_player;
        }
        else
            return ;
    }
    else //����Ŀ�� �÷��̾, ��ȯ���� �ƴҰ�� �׳� ����
        return;

    PlayerAssistMeter::Node* node = GetNode(char_guid);

    // ��ý�Ʈ���ͱ⿡ ��ϵǾ����������� ��Ͻ�Ų��.
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