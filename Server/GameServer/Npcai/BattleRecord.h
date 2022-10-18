#ifndef __GAMESERVER_BATTLE_RECORD_H__
#define __GAMESERVER_BATTLE_RECORD_H__
#pragma once

struct AI_TYPE_INFO;
class NPC;
class AIParamParser;
class BattleRecord;

// (CHANGES) (091216) code arrangement
// (CHANGES) (091216) move 'eBATTLE_POINT_TYPE' to 'AITypes.h'

typedef BATTLEPOINT BattlePoints[BATTLE_POINT_MAX];

//==================================================================================================
//
//                                      BattleRecord
//
//==================================================================================================

/// 한 NPC 혹은 NPCGroup이 적으로 인식하고 있는 Character들에 대한 전투기록
class BattleRecord
{
    friend class TargetManager;
public:
    //----------------------------------------------------------------------------------------------
    /// 개인별 전투 기록
    class Node
    {
    public:
        Node();
        //~Node();

        void    Init();
        void    Update();

        void    SetObjectKey(DWORD objectKey);
        DWORD   GetObjectKey() const;

        void    SetBattlePoint(eBATTLE_POINT_TYPE ePointType, int iPoint);
        BATTLEPOINT GetBattlePoint(eBATTLE_POINT_TYPE ePointType) const;
#ifdef _NA_0_20100819_AGGRO_RENEWAL
		void    SetAIPaserInfo(AI_TYPE_INFO* pAITypeInfo);
		BATTLEPOINT GetCalcTotalPoint() const;

		void    SetTargetDistance(float TargetDistance);
		float   GetTargetDistance() const;
#endif 
        // (CHANGES) (091223) (WAVERIX) add interface
        //void    GetBattlePoints(BattlePoints* OUT points) const;

        //DWORD   GetCreateTick() const;

        void    PlusDamage(int iDamage);
        DWORD   GetTotDamage() const;

        void    SetPointDamage(int iDamage);
        DWORD   GetPointDamage() const;		

        BOOLEAN IsFirstAttacker() const;    
    private:
        DWORD   m_ObjectKey;
        //DWORD   m_CreateTick;
        BOOLEAN m_bFirstAttack;

        DWORD   m_dwPointDamage;    // 데미지 포인트 계산을 위한 데미지(포인트 계산후 초기화)
        DWORD   m_dwTotDamage;      // 전체 데미지(소유권 계산을 위함)

        // 타입별 포인트
        BattlePoints m_wBattlePoint;

#ifdef _NA_0_20100819_AGGRO_RENEWAL		
		float m_TargetDistance;		//대상과의 거리 값
		AI_TYPE_INFO* m_pAITypeInfo;
#endif
    };

    //----------------------------------------------------------------------------------------------
public:
    typedef STLX_MAP<DWORD, Node>  Records;
#ifdef _NA_20100322_AGGRO_DISPLAY
    typedef STLX_VECTOR<Records::iterator> AggroDisplayList;
    struct compare_node_value
    {
        bool operator() (const Records::const_iterator& it1,
                         const Records::const_iterator& it2) const
        {
            return it1->second.GetCalcTotalPoint() > it2->second.GetCalcTotalPoint();
        }
    };
#endif

    BattleRecord(NPC* pNPC);
    ~BattleRecord();

    void Init();
    void Update();

    int  GetSize() const;

    BOOL Remove(DWORD object_key);
    void Clear(DWORD dwExceptKey = 0);

    Character* SelectBestTarget();

    BOOLEAN IsFirstAttacked();
    void SetFirstAttacker(BattleRecord::Node* pRecord);
#ifdef _NA_20100322_AGGRO_DISPLAY
    void SetFirstVictim(BattleRecord::Node* pRecord);
#endif

    template<class RecordOperator>
    void ForEachRecord(RecordOperator& Opr);
    template<class RecordOperator>
    void ForEachRecord(const RecordOperator& Opr) const;
    const Records& GetRecords() const;
    const BattleRecord::Node* Get(DWORD object_key) const;
#ifdef _NA_20100322_AGGRO_DISPLAY
    void SendAggroDisPlayInfo(Player* const player);    
#endif

private:
    Records& GetRecords();
    BattleRecord::Node* Get(DWORD object_key);
    BattleRecord::Node* Add(Character* pAttacker);
    void UpdateTotalPoint(const BOOLEAN need_battlerecord_update = false);
    void SetBattlePoint(DWORD object_key, eBATTLE_POINT_TYPE ePointType, int iPoint);
    BATTLEPOINT GetBattlePoint(DWORD object_key, eBATTLE_POINT_TYPE ePointType) const;
#ifdef _NA_0_20100819_AGGRO_RENEWAL
	BATTLEPOINT GetCalcTotalPoint(DWORD object_key) const;
#endif 

private:
    NPC* const m_pNPC;
    const AI_TYPE_INFO* m_pAITypeInfo; // for caching
    Records m_Records;
    DWORD m_LastUpdateTick;
    BOOLEAN m_bFirstAttacked;   // 처음으로 공격받은 상태인가?
    //
    __DISABLE_COPY(BattleRecord);
};


//==================================================================================================
//  Implementations

inline
BattleRecord::Node::Node() {
    ZeroMemory(this, sizeof(*this));
}

//inline
//BattleRecord::Node::~Node() {
//}

inline void
BattleRecord::Node::Init() {
    new (this) Node();
}

#ifdef _NA_0_20100819_AGGRO_RENEWAL
inline void
BattleRecord::Node::SetAIPaserInfo(AI_TYPE_INFO* pAITypeInfo)
{	
	m_pAITypeInfo = pAITypeInfo;
}
#endif

inline void
BattleRecord::Node::SetObjectKey(DWORD objectKey) {
    m_ObjectKey = objectKey;
}

inline DWORD
BattleRecord::Node::GetObjectKey() const {
    return m_ObjectKey;
}

inline BATTLEPOINT
BattleRecord::Node::GetBattlePoint(eBATTLE_POINT_TYPE ePointType) const {
    return m_wBattlePoint[ePointType];
}


// (CHANGES) (091223) (WAVERIX) add interface
// youngmoon _NA_0_20100819_AGGRO_RENEWAL 추가된 이후 BATTLE_POINT_TOTAL 는 캐릭터별 비율 계산이 들어가므로 사용하지 않기로 한다.
//inline void
//BattleRecord::Node::GetBattlePoints(BattlePoints* OUT points) const {
//    CopyMemory(points, m_wBattlePoint, sizeof(*points));
//}

//inline DWORD
//BattleRecord::Node::GetCreateTick() const {
//    return m_CreateTick;
//}

inline void
BattleRecord::Node::PlusDamage(int iDamage) {
    m_dwTotDamage += iDamage;
    m_dwPointDamage += iDamage;
}

inline DWORD
BattleRecord::Node::GetTotDamage() const {
    return m_dwTotDamage;
}

inline void
BattleRecord::Node::SetPointDamage(int iDamage) {
    m_dwPointDamage = iDamage;
}

inline DWORD
BattleRecord::Node::GetPointDamage() const {
    return m_dwPointDamage;
}

inline BOOLEAN
BattleRecord::Node::IsFirstAttacker() const {
    return m_bFirstAttack;
}

#ifdef _NA_0_20100819_AGGRO_RENEWAL
inline void
BattleRecord::Node::SetTargetDistance(float TargetDistance){
	m_TargetDistance = TargetDistance;
}

inline float
BattleRecord::Node::GetTargetDistance() const{
	return m_TargetDistance;
}
#endif 

//==================================================================================================


inline int
BattleRecord::GetSize() const {
    return (int)m_Records.size();
}

inline BOOLEAN
BattleRecord::IsFirstAttacked() {
    return m_bFirstAttacked;
}

inline BattleRecord::Records&
BattleRecord::GetRecords() {
    return m_Records;
}

inline const BattleRecord::Records&
BattleRecord::GetRecords() const {
    return m_Records;
}

// (CHANGES) add const function
inline const BattleRecord::Node*
BattleRecord::Get(DWORD object_key) const {
    Records::const_iterator it = m_Records.find(object_key);
    return it != m_Records.end() ? &it->second
                                 : NULL;
}

// (CHANGES) add const function
inline BattleRecord::Node*
BattleRecord::Get(DWORD object_key) {
    Records::iterator it = m_Records.find(object_key);
    return it != m_Records.end() ? &it->second
                                 : NULL;
}

template<class RecordOperator>
void BattleRecord::ForEachRecord(RecordOperator& Opr)
{
    Records::iterator it = m_Records.begin(),
                      itend = m_Records.end();
    for( ; it != itend; ++it)
    {
        BattleRecord::Node* pRecord = &it->second;
        Opr(pRecord);
    }
}

template<class RecordOperator>
void BattleRecord::ForEachRecord(const RecordOperator& Opr) const
{
    Records::const_iterator it = m_Records.begin(),
                            itend = m_Records.end();
    for( ; it != itend; ++it)
    {
        const BattleRecord::Node* pRecord = &it->second;
        Opr(pRecord);
    }
}

#endif //__GAMESERVER_BATTLE_RECORD_H__
