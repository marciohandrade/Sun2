
#pragma once

#include <boost/shared_ptr.hpp>

class Player;

//==================================================================================================

class FieldPvP
{
public:
	FieldPvP( WORD PTATroubled ) : m_PTATroubled(PTATroubled) {}
	virtual ~FieldPvP() 
    {
    }

public:

	virtual BOOL CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eSKILL_RESULT*  pErrCode );
	virtual BOOL CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT* pErrCode );

    virtual BOOL IsPart( const WORD wTileAtt ) { return (wTileAtt & m_PTATroubled) ? TRUE : FALSE; }

	virtual eUSER_RELATION_TYPE IsFriendPVP( Player* pAttacker, Player *pTarget );
    //_NA_008706_20160229_TEXTLOG_PK
	virtual VOID KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0 ) 
    { __UNUSED((pAttacker, pDeadPlayer, bSummoned, damage_value)); }

	BOOL IsPvPAtt(WORD PTATroubled) const
    {
        return !!(m_PTATroubled & PTATroubled);
    }
protected:

	virtual eUSER_RELATION_TYPE PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode )  = 0;//{ return USER_RELATION_NEUTRALIST; }

	BOOL ChkServer( OUT RC::eBATTLE_RESULT* pErrCode );
	BOOL ChkTarget( Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode );

	BOOL ChkDropItem( Player* pAttacker, Player* pDeadPlayer );
	BYTE PKDropRatio( int iLevelDiff );
	BOOL SelectPKDropItem( SCSlotContainer* pContainer, POSTYPE & OUT atSlotPos );

	VOID DropItem( Player* pAttacker, Player* pDeadPlayer, BYTE DropRatio );

	VOID Resurrection( Player* pDeadPlayer );

	bool HostilityRelationCheck(Player *pAttacker, Player *pTarget);
	bool IsOneSideHostility(Player *pAttacker, Player *pTarget);

	RC::eSKILL_RESULT ConvErrCode( RC::eBATTLE_RESULT ErrCode );

protected:
	const WORD  m_PTATroubled;
    __DISABLE_COPY(FieldPvP);
};

//==================================================================================================

class BoundaryPart : public FieldPvP
{
public:
	BoundaryPart() : FieldPvP(PTA_PK_TILE) {}
	virtual ~BoundaryPart() {}

public:
    virtual BOOL IsPart( const WORD wTileAtt );
    //_NA_008706_20160229_TEXTLOG_PK
    VOID KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0 );

protected:
	BOOL RandomizePercent( Player* pAttacker, Player* pDeadPlayer );
	eUSER_RELATION_TYPE PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode );
    __DISABLE_COPY(BoundaryPart);
};

//==================================================================================================

class FreePart : public FieldPvP
{
public:
	FreePart() : FieldPvP(PTA_FREE_CONFLICT_TILE) {}
	virtual ~FreePart() {}

public:
    virtual BOOL IsPart( const WORD wTileAtt );
    //_NA_008706_20160229_TEXTLOG_PK
	VOID KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0 );

protected:
	BOOL RandomizePercent( Player* pAttacker, Player* pDeadPlayer );
	eUSER_RELATION_TYPE PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode );
    __DISABLE_COPY(FreePart);
};

//==================================================================================================

class NomalPart : public FieldPvP
{
public:
	NomalPart() : FieldPvP(PTA_CONFLICT_TILE) {}
	virtual ~NomalPart() {}

public:
    virtual BOOL IsPart( const WORD wTileAtt );
    //_NA_008706_20160229_TEXTLOG_PK
	VOID KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0 );

protected:
	eUSER_RELATION_TYPE PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode );
    __DISABLE_COPY(NomalPart); // misspell -.-;;
};

//==================================================================================================

class DominationPart : public FieldPvP
{
public:
    DominationPart() : FieldPvP(PTA_DOMINATION_WAR_TILE) {}
    virtual ~DominationPart() {}

public:
    virtual BOOL IsPart( const WORD wTileAtt );
    //_NA_008706_20160229_TEXTLOG_PK
    VOID KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0 );

protected:
    eUSER_RELATION_TYPE PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode );
    __DISABLE_COPY(DominationPart);
};

//==================================================================================================
class SafetyPart : public FieldPvP
{
public:
    SafetyPart() : FieldPvP(PTA_SAFETY_ZONE) {}
    virtual ~SafetyPart() {}

public:
    virtual BOOL IsPart(const WORD wTileAtt);
    //_NA_008706_20160229_TEXTLOG_PK
    VOID KillPlayer(Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0);

protected:
    eUSER_RELATION_TYPE PvPRule(Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT* pErrCode);
    __DISABLE_COPY(SafetyPart);
};

//==================================================================================================
// (CHANGES) separate declaration with implementation, moved to inline implementation or code-file

class FieldPvPRuleManager
{
public:
    FieldPvPRuleManager(WORD wConflictParts);
    ~FieldPvPRuleManager();

private:
    typedef boost::shared_ptr<FieldPvP> FieldPvpPtr;
    typedef STLX_VECTOR<FieldPvpPtr> FieldPvP_List;
    void CreateTroubledPart();
    FieldPvpPtr GetPart(Player* const player) const;

public:
    BOOL CanPVP(Player* pAttacker, Player* pTarget, OUT RC::eSKILL_RESULT* pErrCode) const
    {
        FieldPvpPtr part = GetPart(pAttacker);
        if (!part)
        {
            //마을일경우[마을은 타일속성이 없다]나 존이 처리 할수 없는 파트룰일경우
            // 마을이 예외인 이유? 기획이 속성을 일일이 찍기 싫어 해서.IGNORE_PKTILE_OPTION이 생긴 이유이기도함
            return USER_RELATION_FRIEND;
        }
        return part->CanPVP(pAttacker,pTarget,pErrCode);
    }
    BOOL CanPVP(Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT* pErrCode) const
    {
        FieldPvpPtr part = GetPart(pAttacker);
        if (!part)
        {
            //마을일경우[마을은 타일속성이 없다]나 존이 처리 할수 없는 파트룰일경우
            return USER_RELATION_FRIEND;
        }
        return part->CanPVP(pAttacker,pTarget,pErrCode);
    }
    eUSER_RELATION_TYPE IsFriendPVP(Player* pAttacker, Player* pTarget) const
    {
        FieldPvpPtr part = GetPart(pAttacker);
        if (!part)
        {
            //마을일경우[마을은 타일속성이 없다]나 존이 처리 할수 없는 파트룰일경우
            return USER_RELATION_FRIEND;
        }
        return part->IsFriendPVP(pAttacker,pTarget);
    }
    //_NA_008706_20160229_TEXTLOG_PK
    VOID KillPlayer(Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0) const
    {
        FieldPvpPtr part = GetPart(pAttacker);
        if (!part)
        {
            return;
        }
        part->KillPlayer(pAttacker,pDeadPlayer,bSummoned, damage_value);
    }
    BOOL IsPvPAtt(Player* pPlayer, WORD PTATroubled) const
    {
        FieldPvpPtr part = GetPart(pPlayer);
        if (!part)
        {
            return FALSE;
        }
        return part->IsPvPAtt(PTATroubled);
    }

private:
    WORD m_wConflictParts;
    FieldPvP_List m_vTroubledPart;
};