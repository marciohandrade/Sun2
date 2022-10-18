#include "StdAfx.h"
#include "SummonManager.h"
#include "SummonedNPCs.h"
#include "SkillInfoParser.h"
#include "Summoned.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������
SummonManager::SummonManager()
{
	m_pSummonNPCsPool = new util::CMemoryPoolFactory<SummonedNPCs>;
}

SummonManager::~SummonManager()
{
	SAFE_DELETE( m_pSummonNPCsPool );
}

VOID SummonManager::Release()
{
	m_SummonNPCsList.clear();
	if( m_pSummonNPCsPool )
	{
		m_pSummonNPCsPool->Release();
	}
}

VOID SummonManager::Init( DWORD dwMaxPoolSize )
{
	//	(WAVERIX)(REMARK)(��ġ ����)
	//		- SummonedNPCs�� �׸� �δ㰡�� �ڷ� ������ �ƴϴ� <-( ���� �ٸ� �Ϳ� ���ؼ�... )
	//		- SUMMON_NPC_LIST�� ������ Ǯ�κ��� ���� �ʿ䰡 ���� �� �ѵ�, �ܺ� ��⿡�� �����͸�
	//		����ϰ� �ִ� ����� ��Ȯ�� Ȯ���� ��������.
	m_pSummonNPCsPool->Initialize( 100, 100, "SummonedNPCs", AssertionLogger4Pool_Server );
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
SummonedNPCs* SummonManager::AllocSummonNPCs( Character* pSummoner, BOOL bPlayerSkill, WORD ability_id /*= 0*/ )
#else
SummonedNPCs* SummonManager::AllocSummonNPCs( Character* pSummoner, BOOL bPlayerSkill )
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
{
	SummonedNPCs* pSummonedNPCs = FindSummonNPCs( pSummoner->GetObjectKey() );

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    if( pSummonedNPCs )
    {
        // �÷��̾� ��ȯ�� ���
        if( bPlayerSkill )
        {
            if (ability_id == eABILITY_SUMMON) {
                // �Ϲݼ�ȯ�� ��� �Ϲݼ�ȯ�� ��ȯ���� �����
                DeleteSummonedUsingType(pSummoner, eSUMMON_NORMAL);
            }
            else if (ability_id == eABILITY_SUMMON_IMMOVABLE) {
                // ��������ȯ���ϰ�� ������ ��ȯ���� �����
                DeleteSummonedUsingType(pSummoner, eSUMMON_IMMOVABLE);
            }
            else 
            {
                FreeSummonNPCs(pSummonedNPCs);
                // �ش��ϴ� �׷��� ������ ���λ���
                pSummonedNPCs = (SummonedNPCs*)m_pSummonNPCsPool->Alloc();
            }
        }
        else
        {
            // NPC�� ��� ���� ��ȯ������ �״�� �̿��Ѵ�.
            return pSummonedNPCs;
        }
    }
    else {
        pSummonedNPCs = (SummonedNPCs*)m_pSummonNPCsPool->Alloc();
    }
#else
    if( pSummonedNPCs )
    {
        // �÷��̾� ��ȯ�� ���
        if( bPlayerSkill )
        {
            // �̹� ��ȯ�� ������ ���� ��ȯü�� �����Ѵ�.
            FreeSummonNPCs( pSummonedNPCs );
        }
        else
        {
            // NPC�� ��� ���� ��ȯ������ �״�� �̿��Ѵ�.
            return pSummonedNPCs;
        }
    }
    // �ش��ϴ� �׷��� ������ ���λ���
    pSummonedNPCs = (SummonedNPCs*)m_pSummonNPCsPool->Alloc();
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

	if( !pSummonedNPCs )
	{
		SUNLOG( eCRITICAL_LOG,  "[SummonManager::AllocSummonNPCs] Can't Allocate SummonedNPCs " );
		return NULL;
	}

	// SummonedNPCs �ʱ�ȭ
	if(pSummoner->IsEqualObjectKind(PLAYER_OBJECT))
	{
		pSummonedNPCs->Init( pSummoner, eSUMMON_COMMAND_DELEGATE_ATTACK_IGNORE_PLAYER );
	}
	else
	{
		pSummonedNPCs->Init( pSummoner, eSUMMON_COMMAND_DELEGATE_ATTACK );
	}

	// SummonManager�� ���
	m_SummonNPCsList.insert( std::make_pair( pSummoner->GetObjectKey(), pSummonedNPCs ) );

#ifdef WAVERIX_CODE_DELETE_RESERVED
	m_SummonHash.insert( pSummonedNPCs );
#endif

	return pSummonedNPCs;
}

BOOL SummonManager::FreeSummonNPCs( SummonedNPCs *pSummonNPCs, BOOL bFollowerDestroy )
{
	if( NULL == FindSummonNPCs( pSummonNPCs->GetSummonerKey() ) )
	{
		SUNLOG( eCRITICAL_LOG, "[SummonManager::FreeSummonNPCs] Can't Remove SummonNPCs  " );
		return FALSE;
	}

	m_SummonNPCsList.erase( pSummonNPCs->GetSummonerKey() );

#ifdef WAVERIX_CODE_DELETE_RESERVED
	SUMMON_HASH::iterator it = m_SummonHash.find(pSummonNPCs);
	if( it == m_SummonHash.end() )
	{
		SUNLOG( eCRITICAL_LOG, "[SusunLove] FreeSummonNPCs Free Same Memory Pool! SummonerKey[%d]", pSummonNPCs->GetSummonerKey() );
		return FALSE;
	}
	else
	{
		m_SummonHash.erase(it);
	}
#endif

	pSummonNPCs->Release( bFollowerDestroy );

	m_pSummonNPCsPool->Free( pSummonNPCs );

	return TRUE;
}


VOID	SummonManager::LoopNode::_AddSummonedSkill( SUMMONED_MAP_PAIR rPair )
{
	NPC* pNPC = rPair.second;
	if(pNPC->IsEqualObjectKind(SUMMON_OBJECT) == 0)
		return;

	Summoned* pSummoned = (Summoned*)pNPC;
	pSummoned->AddSkill( SKILL_CODE.Get() );
}



VOID	SummonManager::AddSummonedSkill( DWORD dwSummonKey, SLOTCODE scSkillCode )
{
	SummonedNPCs* pSummonedNPCs = FindSummonNPCs( dwSummonKey );
	if( !pSummonedNPCs )
		return;

	SkillScriptInfo* pBaseCurSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( scSkillCode );
	if( pBaseCurSkillInfo->m_bySkillUserType != eSKILL_USER_SUMMONED )
		return;

	LoopNode loopNode;
	loopNode.SKILL_CODE.Set( scSkillCode );
	pSummonedNPCs->ForeachSummonedList( &SummonManager::LoopNode::_AddSummonedSkill, &loopNode );
}


VOID	SummonManager::LoopNode::_DelSummonedSkill( SUMMONED_MAP_PAIR rPair )
{
	NPC* pNPC = rPair.second;
	if(pNPC->IsEqualObjectKind(SUMMON_OBJECT) == 0)
		return;

	Summoned *pSummoned = (Summoned*)pNPC;
	pSummoned->DelSkill( SKILL_CODE.Get() );
}



VOID	SummonManager::DelSummonedSkill( DWORD dwSummonKey, SLOTCODE scSkillCode )
{
	SummonedNPCs *pSummonedNPCs = FindSummonNPCs( dwSummonKey );
	if( !pSummonedNPCs )
		return;

	SkillScriptInfo *pBaseCurSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( scSkillCode );
	if( pBaseCurSkillInfo->m_bySkillUserType != eSKILL_USER_SUMMONED )
		return;

	LoopNode loopNode;
	loopNode.SKILL_CODE.Set( scSkillCode );
	pSummonedNPCs->ForeachSummonedList( &SummonManager::LoopNode::_DelSummonedSkill, &loopNode );
}


// ����� ��ȯü�� 1������ ������ƮŰ�� ������ �ʿ������
// ������ ���� Ŭ��κ��� ����� ���� ��ȯü ������ƮŰ�� �޾ƾ� �Ѵ�!

RC::eSUMMON_RESULT SummonManager::SetCommandState( DWORD dwSummonKey, eSUMMON_COMMAND eCommand, DWORD dwObjectKey, DWORD dwTargetKey )
{
	SummonedNPCs *pSummonedNPCs = FindSummonNPCs( dwSummonKey );
	if( !pSummonedNPCs )
		return RC::RC_SUMMON_SUMMONED_NPC_NOTEXIST;

   switch(eCommand)
   {
   case eSUMMON_COMMAND_DESTROY:
       {
           //!~ _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
           if (dwTargetKey)
               pSummonedNPCs->Destroy( dwTargetKey );
           else
               pSummonedNPCs->Destroy( dwObjectKey );
           //~! _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
       } break;
   default:     
       return pSummonedNPCs->SetCommandState( eCommand, dwObjectKey, dwTargetKey ); // ���¸� �ٲپ��ش�.
   }

	return RC::RC_SUMMON_SUCCESS;
}


VOID	SummonManager::LoopNode::_LeaveSummonNPCs( SUMMONED_MAP_PAIR rPair )
{
	NPC* pNPC = rPair.second;
	pNPC->SetSummonerKey( 0 );
}


VOID	SummonManager::LeaveSummonNPCs( Character *pLeaveChar )
{
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    SummonedNPCs *pSummonedNPCs = FindSummonNPCs( pLeaveChar->GetObjectKey() );
    if (pSummonedNPCs)
    {
	    if(pLeaveChar->IsEqualObjectKind(PLAYER_OBJECT))
	    {
            // �÷��̾ ������� ��ȯ������ ������ �����Ѵ�.
		    FreeSummonNPCs( pSummonedNPCs );
	    }
        else
	    {
            // ��ȯ�� ��Ų ��ü�� ��� ��ȯNPC���� ������ȯŰ(m_dwSummonerKey)�� NULL�� �ʱ�ȭ�Ѵ�.
            // NPC�� ��ȯ�� ����(��ȯü)���� �������� �ʾƾ��Ѵ�.
		    LoopNode loopNode;
		    pSummonedNPCs->ForeachSummonedList( &SummonManager::LoopNode::_LeaveSummonNPCs, &loopNode );
		    FreeSummonNPCs( pSummonedNPCs, FALSE );
	    }
    }
    else
    {
        // ��ȯ�� ���� NPC�� ��� ��ȯ�������� �����Ѵ�.
        pSummonedNPCs = FindSummonNPCs( pLeaveChar->GetSummonerKey() );
        if (pSummonedNPCs)
        {
            DWORD dwObjectKey = pLeaveChar->GetObjectKey();
            NPC* pLeaveNPC = static_cast<NPC*>(pLeaveChar);

            pSummonedNPCs->DecreaseSummoned( pLeaveNPC->GetBaseInfo()->m_MonsterCode, dwObjectKey );

            // ��ȯ����Ʈ�� ������� 0�̸� Ǯ�� ��ȯ�Ѵ�.
            if (pSummonedNPCs->GetNumberOfNPCs() == 0) {
                FreeSummonNPCs( pSummonedNPCs );
            }
        }

        pLeaveChar->SetSummonerKey(0);
    }
#else
    SummonedNPCs *pSummonedNPCs = FindSummonNPCs( pLeaveChar->GetObjectKey() );
    if( !pSummonedNPCs )
        return;

    // �÷��̾ ������� ��ȯ������ ������ �����Ѵ�.
    if(pLeaveChar->IsEqualObjectKind(PLAYER_OBJECT))
    {
        FreeSummonNPCs( pSummonedNPCs );
    }
    else
    {
        DWORD dwObjectKey = pLeaveChar->GetObjectKey();
        NPC* pLeaveNPC = (NPC*)pLeaveChar;

        // ��ȯ�� ��Ų ��ü�� ��� ��ȯNPC���� ������ȯŰ(m_dwSummonerKey)�� NULL�� �ʱ�ȭ�Ѵ�.
        // NPC�� ��ȯ�� ����(��ȯü)���� �������� �ʾƾ��Ѵ�.
        if( pSummonedNPCs->IsSummoner( dwObjectKey ) )
        {
            LoopNode loopNode;
            pSummonedNPCs->ForeachSummonedList( &SummonManager::LoopNode::_LeaveSummonNPCs, &loopNode );
            FreeSummonNPCs( pSummonedNPCs, FALSE );
        }
        // ��ȯ�� ���� NPC�� ��� ��ȯ�������� �����Ѵ�.
        else
        {
            pSummonedNPCs->DecreaseSummoned( pLeaveNPC->GetBaseInfo()->m_MonsterCode, dwObjectKey );

            // ��ȯ����Ʈ�� ������� 0�̸� Ǯ�� ��ȯ�Ѵ�.
            if( !pSummonedNPCs->GetNumberOfNPCs() )
            {
                FreeSummonNPCs( pSummonedNPCs );
            }
        }
    }

    pLeaveChar->SetSummonerKey(0);
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
bool SummonManager::DeleteSummonedUsingType( Character* pSummoner, eSUMMON_TYPE type )
{
    SummonedNPCs* pSummonedNPCs = FindSummonNPCs( pSummoner->GetObjectKey() );

    if (pSummonedNPCs == NULL) {
        return false;
    }

    Summoned* summoned = pSummonedNPCs->FindSummonedUsingType(type);
    if (summoned == NULL) {
        //����Ǿ��� ���»��·� ������ �ϴ�..
        return true;
    }

    pSummonedNPCs->Destroy(summoned->GetObjectKey());
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL


