#pragma once

#include <hash_map>
#include <SolarHashTable.h>

struct BASE_SUMMONINFO;
class Summoned;
class NPC;

class SummonedNPCs
{
public:
	SummonedNPCs();
	~SummonedNPCs();

	VOID						Init( Character* pSummoner, eSUMMON_COMMAND eCommand );
	VOID						Release( BOOL bFollowerDestroy=TRUE );

	inline BOOL					IsSummoner( DWORD dwObjectKey )		{ return ( m_dwSummonerKey == dwObjectKey );}
	inline DWORD				GetSummonerKey()	{ return m_dwSummonerKey; }

#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
	Character*			        GetSummoner();
	inline VOID					SetSummoner( Character*pSummoner )	{ m_pSummoner = pSummoner; }
    DWORD						GetNumberOfAliveNPCs( DWORD dwNPCCode );
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

	VOID						IncreaseSummoned( DWORD dwNPCCode, NPC *pNPC );
	VOID						DecreaseSummoned( DWORD dwNPCCode, DWORD dwObjectKey );

	eSUMMON_COMMAND				GetCommandState()	{ return m_eCommandState; }
	RC::eSUMMON_RESULT			SetCommandState( eSUMMON_COMMAND eCommand, DWORD dwObjectKey=0, DWORD dwTargetKey=0 );

	VOID						Destroy( DWORD dwObjectKey );

    DWORD GetNumberOfNPCs() const
    { 
        return m_SummonedList.size(); 
    }

	NPC*						GetSummoned( DWORD dwObjKey );

	template<typename _FN, typename _OBJ>
	VOID						ForeachSummonedList( _FN _func, const _OBJ& _left )
	{
		for_each( m_SummonedList.begin(), m_SummonedList.end(), bind1st(mem_fun(_func), _left) );
	}
	template<typename _Oper>
	VOID						ForeachSummonedList( const _Oper& _func )
	{
		std::for_each( m_SummonedList.begin(), m_SummonedList.end(), _func );
	}

	VOID						LeaveField();
	VOID						EnterField( GameField* pField, WzVector* pStartPos );
#ifdef _NA_004791_20120430_1202_SUMMON_STATE
    Summoned* FindSummoned(DWORD dwObjectKey);
#endif  // _NA_004791_20120430_1202_SUMMON_STATE
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    Summoned* FindSummonedUsingType(eSUMMON_TYPE type);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

private:
	struct LoopNode
	{
		LoopNode( DWORD dwObjectKey ) { OBJECT_KEY.Set(dwObjectKey); }
		PROPERTY<DWORD>				OBJECT_KEY;

		VOID						_Destroy( SUMMONED_MAP_PAIR rPair );
		VOID						_SetCommandState( SUMMONED_MAP_PAIR rPair );
	};

private:
#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
	Character*							m_pSummoner;			// ��ȯ��
    NUMBER_OF_NPCS_MAP					m_mapNumberOfNPCs;		// ��ȯü �ڵ庰 ���ڰ���
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

    DWORD								m_dwSummonerKey;		// ��ȯ�� Ű
	eSUMMON_COMMAND						m_eCommandState;		// ��ȯü�� ��ɻ���(���, ��������, ������� 3������ ����Ѵ�.)
	SUMMONED_MAP						m_SummonedList;
};



