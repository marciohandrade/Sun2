#include "stdafx.h"
#include "SummonedNPCs.h"
#include "Summoned.h"
#include "ObjectFactory.h"

SummonedNPCs::SummonedNPCs() :
    m_dwSummonerKey(0)
{
#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
    m_pSummoner = NULL;
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
}

SummonedNPCs::~SummonedNPCs()
{
	Release();
}

// ó���� �ѹ��� �����Ѵ�.
VOID SummonedNPCs::Init( Character *pSummoner, eSUMMON_COMMAND eCommand )
{
#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
	m_pSummoner = pSummoner;
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
	m_dwSummonerKey = pSummoner->GetObjectKey();
	m_eCommandState = eCommand;
}

VOID SummonedNPCs::Release( BOOL bFollowerDestroy )
{
	// ��ȯü���� ���� �Ҹ��Ų��.
	if( bFollowerDestroy )
		Destroy( 0 );

#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
	// ��ȯ���� ��ȯŰ�� �ʱ�ȭ�Ѵ�.
	if( m_pSummoner )
		m_pSummoner->SetSummonerKey( 0 );

	m_pSummoner = NULL;
    m_mapNumberOfNPCs.clear();
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

	m_dwSummonerKey = 0;
	m_SummonedList.clear();
}


NPC*	SummonedNPCs::GetSummoned( DWORD dwObjKey )
{
	SUMMONED_MAP_IT itFIND = m_SummonedList.find( dwObjKey );
	NPC* pNPC = (itFIND != m_SummonedList.end()) ? itFIND->second : NULL;

	return pNPC;
}

#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
VOID	SummonedNPCs::IncreaseSummoned( DWORD dwNPCCode, NPC *pNPC )
{
    m_SummonedList.insert( std::make_pair( pNPC->GetObjectKey(), pNPC ) );
}

VOID SummonedNPCs::DecreaseSummoned( DWORD dwNPCCode, DWORD dwObjectKey )
{
    m_SummonedList.erase( dwObjectKey );
}
#else
DWORD	SummonedNPCs::GetNumberOfAliveNPCs( DWORD dwNPCCode )
{
	NUMBER_OF_NPCS_MAP_ITER it = m_mapNumberOfNPCs.find( dwNPCCode );
	SASSERT( it != m_mapNumberOfNPCs.end(), "[SummonedNPCs::GetNumberOfAliveNPCs] it == m_mapNumberOfNPCs.end()" );
	return it->second;
}

VOID	SummonedNPCs::IncreaseSummoned( DWORD dwNPCCode, NPC *pNPC )
{
	NUMBER_OF_NPCS_MAP_ITER mapit = m_mapNumberOfNPCs.find( dwNPCCode );
	if ( mapit != m_mapNumberOfNPCs.end() )
	{
		// ��ȯ�� NPC�� ���� ����
		++mapit->second;
	}
	else
	{
		// �ű� ������ ��� �߰�
		m_mapNumberOfNPCs.insert( NUMBER_OF_NPCS_MAP_PAIR(dwNPCCode, 1) ); 
	}

	m_SummonedList.insert( std::make_pair( pNPC->GetObjectKey(), pNPC ) );
}

VOID SummonedNPCs::DecreaseSummoned( DWORD dwNPCCode, DWORD dwObjectKey )
{
	NUMBER_OF_NPCS_MAP_ITER mapit = m_mapNumberOfNPCs.find( dwNPCCode );
	if ( mapit == m_mapNumberOfNPCs.end() )
		return;

	// ��ȯ�� NPC ���� ����
	--mapit->second;

	m_SummonedList.erase( dwObjectKey );
}
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

VOID	SummonedNPCs::Destroy( DWORD dwObjectKey )
{
	if( dwObjectKey )
	{
		Summoned* pSummoned = (Summoned*)GetSummoned( dwObjectKey );
		if( !pSummoned )
			return;

		GameField *pField = pSummoned->GetField();
		if( !pField )
			return;

		pField->DestroyObject( pSummoned );
	}
	else
	{
		// (WAVERIX)(REMARK) 0���� �Է����� ��, ��� ��ȯü�� �����ϵ��� �Ǿ� �ִ°�?
		SUMMONED_MAP_IT itend = m_SummonedList.end();
		for( SUMMONED_MAP_IT it = m_SummonedList.begin(); it != itend ; )
		{
			NPC* pNPC = it->second;
			//(lst1024)(1009) DestroyObject() �� ���� ������ it 
			++it;

			if(pNPC->IsEqualObjectKind(SUMMON_OBJECT) == 0)
				continue;

			Summoned* pSummoned = (Summoned*)pNPC;
			GameField *pField = pSummoned->GetField();
			if( !pField )
				continue;

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
            //[mks]
            // ���۾��� �� �ߴ��� ����� �ȳ�...
            // �ش� �б� ������ SummonedNPCs Release()�ÿ� �����Ƿ�, ���� �������ִ� ���� ����

            //��ġ���̵��� ��������ȯ���� ������������
            //if (pSummoned->GetSummonType() == eSUMMON_IMMOVABLE) {
                //continue;
            //}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
			pField->DestroyObject( pSummoned );
		}
	}
}

// __NA_S00015_20080828_NEW_HOPE_SHADOW
VOID			SummonedNPCs::LeaveField()
{
	ObjectFactory* const pObjectFactory = ObjectFactory::Instance();

	SUMMONED_MAP_IT itend = m_SummonedList.end();
	for( SUMMONED_MAP_IT it = m_SummonedList.begin(); it != itend ; )
	{
		NPC* pNPC = it->second;
		//(lst1024)(1009) DestroyObject() �� ���� ������ it 
		++it;

		if(pNPC->IsEqualObjectKind(SUMMON_OBJECT) == 0)
			continue;

		Summoned* pSummoned = (Summoned*)pNPC;
		GameField *pField = pSummoned->GetField();
		if( !pField )
			continue;

		if( !pField->LeaveField( pSummoned ) )
		{
			SUNLOG( eCRITICAL_LOG, "[SummonedNPCs::LeaveField()] ��ȯ�� �ʵ� Leave ����" );
		}

		pObjectFactory->FreeObject( pNPC );
	}
}

VOID			SummonedNPCs::EnterField( GameField* pField, WzVector* pStartPos )
{
	if( !pField  || !pStartPos )
		return;

	SUMMONED_MAP_IT itend = m_SummonedList.end();
	for( SUMMONED_MAP_IT it = m_SummonedList.begin(); it != itend ; )
	{
		NPC* pNPC = it->second;
		++it;

		if(pNPC->IsEqualObjectKind(SUMMON_OBJECT) == 0)
			continue;

		Summoned* pSummoned = (Summoned*)pNPC;
		pField->EnterField( pSummoned, pStartPos );
	}
}
// end of shadow part

RC::eSUMMON_RESULT SummonedNPCs::SetCommandState( eSUMMON_COMMAND eCommand, DWORD dwObjectKey, DWORD dwTargetKey )
{
	// ��Ŵ� ������ �ȵ����� ����ó���� ����!!
	if( eCommand == eSUMMON_COMMAND_DESTROY )
	{
        //!~ _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
        if (dwTargetKey)
            Destroy(dwTargetKey);
        else
		    Destroy( dwObjectKey );
        //~! _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL

		SUNLOG( eMIDDLE_LOG, "[SummonedNPCs::SetCommandState] eSUMMON_COMMAND_DESTROY" );
		return RC::RC_SUMMON_SUCCESS;
	}

	if( m_eCommandState == eCommand )
	{
#ifdef _NA_004791_20120430_1202_SUMMON_STATE
        Summoned* summoned = FindSummoned(dwObjectKey);
        if (summoned && summoned->GetCommandState() == eSUMMON_COMMAND_ATTACK)
        {
            summoned->SetCommandState( eCommand, dwTargetKey );
            return RC::RC_SUMMON_SUCCESS;
        }
#endif // _NA_004791_20120430_1202_SUMMON_STATE
        return RC::RC_SUMMON_SAME_COMMAND;
	}

	// ��� ����
	if( eCommand == eSUMMON_COMMAND_FOLLOW			||
		eCommand == eSUMMON_COMMAND_DELEGATE_ATTACK ||
		eCommand == eSUMMON_COMMAND_DELEGATE_ATTACK_IGNORE_PLAYER ||
		eCommand == eSUMMON_COMMAND_DELEGATE_DEFENSE )
	{
		m_eCommandState = eCommand;
	}

	// (WAVERIX)(REMARK) ������ ���ذ� �Ȱ���. SUMMON_KEY�� 0�� ��츦 ������ �� �˾ƺ��� �� ���̴�.
	SUMMONED_MAP_IT itend = m_SummonedList.end();
	for( SUMMONED_MAP_IT it = m_SummonedList.begin(); it != itend ; ++it )
	{
		NPC* pNPC = it->second;
		// dwObjectKey�� ���� SUMMON_OBJECT �� ��츸 �˻��ϰ� ����Ѵ�. ���� �̰�...
		if( dwObjectKey && dwObjectKey != pNPC->GetObjectKey() )
			continue;
		if(pNPC->IsEqualObjectKind(SUMMON_OBJECT) == 0)
			continue;

		Summoned* pSummoned = (Summoned*)pNPC;
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        if (pSummoned->GetSummonType() == eSUMMON_IMMOVABLE) {
            continue;
        }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
		pSummoned->SetCommandState( eCommand, dwTargetKey );
	}
	return RC::RC_SUMMON_SUCCESS;
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
Summoned* SummonedNPCs::FindSummonedUsingType( eSUMMON_TYPE type )
{
    for( SUMMONED_MAP_IT it = m_SummonedList.begin(); it != m_SummonedList.end() ; ++it )
    {
        NPC* npc = it->second;
        
        if(npc->IsEqualObjectKind(SUMMON_OBJECT) == false) {
            continue;
        }
        Summoned* summoned = reinterpret_cast<Summoned*>(npc);
        if (summoned->GetSummonType() == type) {
            return summoned;
        }
    }

    return NULL;
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

#ifdef _NA_004791_20120430_1202_SUMMON_STATE
Summoned* SummonedNPCs::FindSummoned(DWORD dwObjectKey)
{
    for( SUMMONED_MAP_IT it = m_SummonedList.begin(); it != m_SummonedList.end() ; ++it )
    {
        NPC* npc = it->second;
        // dwObjectKey�� ���� SUMMON_OBJECT �� ��츸 �˻��ϰ� ����Ѵ�. ���� �̰�...
        if( dwObjectKey && dwObjectKey != npc->GetObjectKey() )
            continue;
        if(npc->IsEqualObjectKind(SUMMON_OBJECT) == false)
            continue;

        return static_cast<Summoned*>(npc);
    }

    return NULL;
}

#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
Character* SummonedNPCs::GetSummoner()
{
    if (obj_util::IsValid(m_pSummoner, __FUNCTION__, __LINE__) == false) {
        return NULL;
    }
    return m_pSummoner;
}
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

#endif  // _NA_004791_20120430_1202_SUMMON_STATE

