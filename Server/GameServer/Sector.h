#pragma once
#ifndef GAMESERVER_SECTOR_H
#define GAMESERVER_SECTOR_H
//==================================================================================================
/// Sector class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2004. 12. 14
    @remark
        - 필드를 구성하는 기본 단위 섹터
    @note
        - 
    @history 
        - 2006/01/11 : 플레이어가 섹터에 들어 왔을 때 길드 정보 보내주도록 추가, 
                       기존의 m_ResultMsg를 사용하던 부분을 삭제, 사용하지 않아도 됨.
                       SetFirst(), GetNext() -> iterator로 바꿈
        - 2007/10/21 : (Waverix) 자료구조 변경 (현재 172 --> 변경 44)
                       메모리 사용량 및 처리속도 증진
        - 2011/02/14 : waverix, fit in coding style
        - 2011/02/21 : waverix, CHANGES: f110221.4L, upgrade to using search node

    @old-history
      <Paradigm><Waverix><0710xx>
          - Memory Optimization
      <Notes>
          엄격히 따지면 기존 방식대로 가는 것이 낮다. 하지만 메모리 사용량이 너무 많다는 문제가 있었으며,
          부득이 다음과 같은 처리로 변경한다. 메모리 최적화 (종전 대비 1/4, 성능... 향상될 것이다.)
      <Description>
          - 모든 Object (Player, Character, Item, ...)는 하나의 List에 입력된다.
          - Object 타입별 Loop지원을 위해 각end역할을 하는 delimeter가 삽입된다.
          한마디로 end()역할을 하는 iterator가 2개 추가된다.
    
          (Sample - _OBJECT_LIST 초기 구조)
          begin() <-> end(for Player) | begin(for Character) <-> end(for Character) | begin(for Item) <-> end()
              (∴) init list size == 2
          (Sample Insert Player)      list.insert(end(for Player), Player)
          (Sample Insert Character)   list.insert(end(for Character), Character)
          (Sample Insert Item)        list.insert(list.end(), Item)
          - STLX_MAP은 검색을 위한 자료구조이며, Value는 STLX_LIST의 iterator이다.
          이 값은 사실상 실제 데이터의 pointer이다, (∴) 중간에 삽입/삭제가 일어나더라도 유지되는 값이 된다.
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

    void OnEnterThisToNeighbor(Object* object);   // 자신정보를 주변에게
    void OnEnterNeighborToThis(Object* object);   // 주변정보를 자신에게
    void OnLeaveThisToNeighbor(Object* object);   // 자신정보를 주변에게
    void OnLeaveNeighborToThis(Object* object);   // 주변정보를 자신에게
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
    SHORT number_of_references_; // 이 섹터의 정보를 참조하는 플레이어의 수
    WORD  number_of_players_; // 이 섹터에 존재하는 플레이어 수
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
    // 주변 정보(sector) 받음
    object->OnEnterNeighbor(*this);
}

inline void Sector::OnLeaveThisToNeighbor(Object* object)
{
    // 자신을 주변에게 알림
    object->OnLeaveObject(*this);
}

inline void Sector::OnLeaveNeighborToThis(Object* object)
{
    // 주변 정보(sector) 받음
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