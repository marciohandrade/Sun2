#pragma once

class Character;
class SkillManager;
class StatusManager;

namespace nsAI
{

//────────────────────────────────────────────────────────────────
class PlanEventObject;
class MovingPlanner;

//────────────────────────────────────────────────────────────────
//
struct TileEvent
{
	enum eTileCtrl {
		eTile_NotUpdated			= 0,		//	초기화된 상태
		eTile_Updated				= 1<< 0,	//	속성 업데이트된 상태
		eTile_RuleAbsolute			= 1<< 1,	//	절대 명령 상태
		eTile_HighDanger			= 1<< 2,	//	'위험'판단 여부 (damage...)
	};

	WORD			Ctrl;			// eTileCtrl
	WORD			Index;			// Tile ID
	WORD			Attr;			// Tile Attribute
	uint8_t			Cost;			// Tile Cost - nsAI::eWeight (AI 평가 수치)
    uint8_t         Event;          // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
	WORD			NearNodes[3];	// 주변 타일 설정
    // (CHANGES) (091213) (WAVERIX) add initialize value
    // (NOTE) 엔진에서 제공하는 값은 -1 : int이지만, 
    static const WORD InvalidTileIndex = USHRT_MAX;
};

//───────────────────────────────────────────────────────────
//
class IPlanEventObject
{
	__DISABLE_COPY(IPlanEventObject);
	friend class MovingPlanner;
	friend class PlanEventObject;

public:
	enum ePolicy {
		DISABLED_ALL				= 0,
		AITYPE_PC					= 1<< 0,
		AITYPE_NPC					= 1<< 1,
		ACTIVATED_OBJECT			= (AITYPE_NPC|AITYPE_PC),
		MOVE_HOLDING				= 1<< 5,	//	이동 처리들을 무시하게 된다.
		AIACT_ESCAPE				= 1<<10,	//	<TEMPORARY>
	};
    // (CHANGES) typedef
    typedef DWORD EventField_t;

public:		IPlanEventObject(
				CPathExplorer*&		pPathExplorer
				, SkillManager*&	pSkillManager
				, StatusManager*&	pStatusManager
				);
			~IPlanEventObject();
public:
	inline	BOOLEAN				IsActivated() const { return !!(m_EventField&ACTIVATED_OBJECT); }
	inline	VOID				AddField( const ePolicy flag )		{ m_EventField |= flag; }
	inline	VOID				RemoveField( const ePolicy flag )	{ m_EventField &= ~flag; }

	inline	PlanEventObject*	operator->() const { return m_pHandler; }
			VOID				Init( Character* const pCharacter );
			VOID				Release();

public:		//////////////////////////////////////////////////////////////////////////
			//	CPathExplorer Interfaces
	const EventField_t& EventField;
private:
    EventField_t        m_EventField;
    PlanEventObject*	m_pHandler;
};


//───────────────────────────────────────────────────────────




//───────────────────────────────────────────────────────────


}; // end namespace nsAI


