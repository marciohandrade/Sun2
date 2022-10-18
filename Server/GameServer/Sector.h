#pragma once
#ifndef GAMESERVER_SECTOR_H
#define GAMESERVER_SECTOR_H
//==================================================================================================
/// Sector class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2004. 12. 14
    @remark
        - �ʵ带 �����ϴ� �⺻ ���� ����
    @note
        - 
    @history 
        - 2006/01/11 : �÷��̾ ���Ϳ� ��� ���� �� ��� ���� �����ֵ��� �߰�, 
                       ������ m_ResultMsg�� ����ϴ� �κ��� ����, ������� �ʾƵ� ��.
                       SetFirst(), GetNext() -> iterator�� �ٲ�
        - 2007/10/21 : (Waverix) �ڷᱸ�� ���� (���� 172 --> ���� 44)
                       �޸� ��뷮 �� ó���ӵ� ����
        - 2011/02/14 : waverix, fit in coding style
        - 2011/02/21 : waverix, CHANGES: f110221.4L, upgrade to using search node

    @old-history
      <Paradigm><Waverix><0710xx>
          - Memory Optimization
      <Notes>
          ������ ������ ���� ��Ĵ�� ���� ���� ����. ������ �޸� ��뷮�� �ʹ� ���ٴ� ������ �־�����,
          �ε��� ������ ���� ó���� �����Ѵ�. �޸� ����ȭ (���� ��� 1/4, ����... ���� ���̴�.)
      <Description>
          - ��� Object (Player, Character, Item, ...)�� �ϳ��� List�� �Էµȴ�.
          - Object Ÿ�Ժ� Loop������ ���� ��end������ �ϴ� delimeter�� ���Եȴ�.
          �Ѹ���� end()������ �ϴ� iterator�� 2�� �߰��ȴ�.
    
          (Sample - _OBJECT_LIST �ʱ� ����)
          begin() <-> end(for Player) | begin(for Character) <-> end(for Character) | begin(for Item) <-> end()
              (��) init list size == 2
          (Sample Insert Player)      list.insert(end(for Player), Player)
          (Sample Insert Character)   list.insert(end(for Character), Character)
          (Sample Insert Item)        list.insert(list.end(), Item)
          - STLX_MAP�� �˻��� ���� �ڷᱸ���̸�, Value�� STLX_LIST�� iterator�̴�.
          �� ���� ��ǻ� ���� �������� pointer�̴�, (��) �߰��� ����/������ �Ͼ���� �����Ǵ� ���� �ȴ�.
*/
//==================================================================================================

#include "SkillSystem/SearchLinkNode.h"

//==================================================================================================
//
class Sector
{
public:
    Sector();
    virtual ~Sector();

    void Init(DWORD dwSectorIndex);
    void Release();
    void Update(DWORD dwDeltaTick);
    //
    RC::eSECTOR_REULT EnterObject(Object* object);
    RC::eSECTOR_REULT AddObject(Object* object);
    RC::eSECTOR_REULT LeaveObject(Object* object);
    RC::eSECTOR_REULT RemoveObject(Object* object);

    void OnEnterThisToNeighbor(Object* object);   // �ڽ������� �ֺ�����
    void OnEnterNeighborToThis(Object* object);   // �ֺ������� �ڽſ���
    void OnLeaveThisToNeighbor(Object* object);   // �ڽ������� �ֺ�����
    void OnLeaveNeighborToThis(Object* object);   // �ֺ������� �ڽſ���
    //
    DWORD GetSectorIndex() const;
    bool IsExistInSector(Object* object) const;
    DWORD GetPlayerNum() const;
    int GetReferenceCount() const;
    void UpdateReferenceCount(Object* object, BOOL bEnter);
    // interfaces for iterator control
    ns_object::SearchNodeConstIterator GetPlayerRoot() const;
    ns_object::SearchNodeConstIterator GetPlayerEnd() const;
    ns_object::SearchNodeConstIterator GetNpcRoot() const;
    ns_object::SearchNodeConstIterator GetNpcEnd() const;
    ns_object::SearchNodeConstIterator GetCharacterRoot() const;
    ns_object::SearchNodeConstIterator GetCharacterEnd() const;
    ns_object::SearchNodeConstIterator GetItemRoot() const;
    ns_object::SearchNodeConstIterator GetItemEnd() const;
    ns_object::SearchNodeConstIterator GetEtcRoot() const;
    ns_object::SearchNodeConstIterator GetEtcEnd() const;
    // inter control interfaces, handle carefully.
    bool Add(Object* object);
    bool Remove(Object* object);
private:
    struct Delimiter : public util::__LList<Delimiter>
    {   // NOTE: f110607.5L, make memory optimization cheat
        eOBJECT_TYPE object_type; // equals to SearchNode::kNullObject
    };
    // fields...
    DWORD sector_index_; // Secter Index
    SHORT number_of_references_; // �� ������ ������ �����ϴ� �÷��̾��� ��
    WORD  number_of_players_; // �� ���Ϳ� �����ϴ� �÷��̾� ��
    union {
        util::__LList<ns_object::SearchNode::LinkNode> etc_end_;
        util::__LList<ns_object::SearchNode::LinkNode> player_root_;
        Delimiter delimiter0_;
    };
    union {
        util::__LList<ns_object::SearchNode::LinkNode> player_end_;
        util::__LList<ns_object::SearchNode::LinkNode> npc_root_;
        Delimiter delimiter1_;
    };
    union {
        util::__LList<ns_object::SearchNode::LinkNode> npc_end_;
        util::__LList<ns_object::SearchNode::LinkNode> item_root_;
        Delimiter delimiter2_;
    };
    union {
        util::__LList<ns_object::SearchNode::LinkNode> item_end_;
        util::__LList<ns_object::SearchNode::LinkNode> etc_root_;
        Delimiter delimiter3_;
    };
    //
    friend class Player;
    friend class GameField;
    friend class Skill;
    friend class HighLoadBalancer;
    //
    __DISABLE_COPY(Sector);
};

//==================================================================================================

inline DWORD Sector::GetSectorIndex() const {
    return sector_index_;
}

inline DWORD Sector::GetPlayerNum() const {
    return number_of_players_;
}

inline int Sector::GetReferenceCount() const {
    return number_of_references_;
}

//==================================================================================================

inline void Sector::OnEnterThisToNeighbor(Object* object)
{
    object->OnEnterObject(*this);
}

inline void Sector::OnEnterNeighborToThis(Object* object)
{
    // �ֺ� ����(sector) ����
    object->OnEnterNeighbor(*this);
}

inline void Sector::OnLeaveThisToNeighbor(Object* object)
{
    // �ڽ��� �ֺ����� �˸�
    object->OnLeaveObject(*this);
}

inline void Sector::OnLeaveNeighborToThis(Object* object)
{
    // �ֺ� ����(sector) ����
    object->OnLeaveNeighbor(*this);
}

//==================================================================================================
// interfaces for iterator control
inline ns_object::SearchNodeConstIterator Sector::GetPlayerRoot() const
{
    ns_object::SearchNodeConstIterator it = { player_root_.next };
    return it;
}

inline ns_object::SearchNodeConstIterator Sector::GetPlayerEnd() const
{
    ns_object::SearchNodeConstIterator it = {
        static_cast<const ns_object::SearchNode::LinkNode*>(&player_end_)
    };
    return it;
}

inline ns_object::SearchNodeConstIterator Sector::GetNpcRoot() const
{
    ns_object::SearchNodeConstIterator it = { npc_root_.next };
    return it;
}

inline ns_object::SearchNodeConstIterator Sector::GetNpcEnd() const
{
    ns_object::SearchNodeConstIterator it = {
        static_cast<const ns_object::SearchNode::LinkNode*>(&npc_end_)
    };
    return it;
}

inline ns_object::SearchNodeConstIterator Sector::GetCharacterRoot() const
{
    ns_object::SearchNodeConstIterator it = { player_root_.next };
    return it;
}

inline ns_object::SearchNodeConstIterator Sector::GetCharacterEnd() const
{
    ns_object::SearchNodeConstIterator it = {
        static_cast<const ns_object::SearchNode::LinkNode*>(&npc_end_)
    };
    return it;
}

inline ns_object::SearchNodeConstIterator Sector::GetItemRoot() const
{
    ns_object::SearchNodeConstIterator it = { item_root_.next };
    return it;
}

inline ns_object::SearchNodeConstIterator Sector::GetItemEnd() const
{
    ns_object::SearchNodeConstIterator it = {
        static_cast<const ns_object::SearchNode::LinkNode*>(&item_end_)
    };
    return it;
}

inline ns_object::SearchNodeConstIterator Sector::GetEtcRoot() const
{
    ns_object::SearchNodeConstIterator it = { etc_root_.next };
    return it;
}

inline ns_object::SearchNodeConstIterator Sector::GetEtcEnd() const
{
    ns_object::SearchNodeConstIterator it = {
        static_cast<const ns_object::SearchNode::LinkNode*>(&etc_end_)
    };
    return it;
}


#endif //GAMESERVER_SECTOR_H