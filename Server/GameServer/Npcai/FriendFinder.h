#pragma once

#include "Character.h"

// ȭ���̸� �ٲ�� �Ѵ�. -_-;

//--------------------------------------------------------------------------------------------------------------
//
//										CharacterFinder
//
//--------------------------------------------------------------------------------------------------------------
// Ư�� ������ ĳ���͸� ã�´�.
class CharacterFinder
{
public :
	CharacterFinder() 
	{
		Init();
	}

	VOID			Init()
	{
		m_pOwner			= NULL;
		m_SkillCode			= SKILLCODE_INVALID;
		m_pTargetCharacter	= NULL;
		m_Value				= 0;
	}

	virtual VOID	SetOwner( Character* pOwner )		{ m_pOwner = pOwner; }
	VOID			SetSkillCode(CODETYPE skillCode)	{ m_SkillCode = skillCode; }
	VOID			SetValue( DWORD value )				{ m_Value = value; }

	// ���
	Character*		GetTargetCharacter()				{ return m_pTargetCharacter;	}

	virtual bool	operator () (Object* pObject) = 0;

protected :
	Character*		m_pOwner;
	CODETYPE		m_SkillCode;
	Character*		m_pTargetCharacter;
	DWORD			m_Value;			// TODO: �̰Ŵ� SkillCode���� �о�;� �ҵ�.
};

//--------------------------------------------------------------------------------------------------------------
//
//										WeakestCharacterFinder
//
//--------------------------------------------------------------------------------------------------------------
// ���� ü���� ���� ĳ���� ã��
class WeakestCharacterFinder : public CharacterFinder
{
public :
	virtual bool operator () (Object* pObject)
	{
		ASSERT( pObject != NULL );
		ASSERT( pObject->GetObjectType() & CHARACTER_OBJECT );

		Character* pCharacter = dynamic_cast<Character*>(pObject);
		ASSERT( pCharacter != NULL );

		DWORD dwMaxHP = max( 1, pCharacter->GetMaxHP() );
		DWORD RemainPercent = pCharacter->GetHP() * 100 / dwMaxHP;
		DWORD TargetRemainPercent = 100;
		if( m_pTargetCharacter )	
		{
			DWORD dwMaxHP = max( 1, m_pTargetCharacter->GetMaxHP() );
			TargetRemainPercent = m_pTargetCharacter->GetHP() * 100 / dwMaxHP;
		}

		if ( pCharacter->IsAlive()
			&& ( m_pTargetCharacter==NULL || RemainPercent < TargetRemainPercent ) )
		{
			m_pTargetCharacter = pCharacter;
		}		

		// not found --> continue
		return false;
	}
};


//--------------------------------------------------------------------------------------------------------------
//
//										DoBuffCharacterFinder
//
//--------------------------------------------------------------------------------------------------------------
// ������ �ɾ��ٸ��� ĳ���� ã��
class DoBuffCharacterFinder : public CharacterFinder
{
public :
	virtual bool			operator () (Object* pObject)
	{
		ASSERT( pObject != NULL );
		ASSERT( pObject->GetObjectType() & CHARACTER_OBJECT );

		Character* pCharacter = dynamic_cast<Character*>(pObject);
		ASSERT( pCharacter != NULL );

		if (m_pTargetCharacter==NULL
			&& pCharacter->IsAlive())// && !m_pTargetCharacter->HasBuff(m_SkillCode) )
		{
			m_pTargetCharacter = pCharacter;

			return true;
		}		

		// not found --> continue
		return false;
	}
};



//--------------------------------------------------------------------------------------------------------------
//
//										DeadCharacterFinder
//
//--------------------------------------------------------------------------------------------------------------
// ���� ĳ���� ã��
class DeadCharacterFinder : public CharacterFinder
{
public :	
	bool			operator () (Object* pObject)
	{
		ASSERT( pObject != NULL );
		ASSERT( pObject->GetObjectType() & CHARACTER_OBJECT );

		Character* pCharacter = dynamic_cast<Character*>(pObject);
		ASSERT( pCharacter != NULL );

		if (m_pOwner != pCharacter
			&& !pCharacter->IsAlive())
		{
			m_pTargetCharacter = pCharacter;

			// found! --> break
			return true;
		}		

		// not found --> continue
		return false;
	}
};

//--------------------------------------------------------------------------------------------------------------
//
//										NearestCharacterFinder
//
//--------------------------------------------------------------------------------------------------------------
class NearestCharacterFinder : public CharacterFinder
{
public :	
	NearestCharacterFinder() : m_eTargetType(SKILL_TARGET_NONE)
	{	
	}

	bool			operator () (Object* pObject)
	{
		Character* pTarget = dynamic_cast<Character*>(pObject);
		if( !pTarget )	return false;

		float fDist = pTarget->GetDist( m_AttackerPos );
		
		if( m_pOwner == pTarget )		return false;
		if( pTarget->IsDead() )			return false;
		if( fDist >= m_AttackerSight )	return false;
		if( m_pTargetCharacter!=NULL && fDist >= m_TargetDist )					return false;
		if( !m_pOwner->IsCoincidentWithTargetType( pTarget, m_eTargetType ) )	return false;

		m_pTargetCharacter = pTarget;
		m_TargetDist = fDist;
		return false;
	}

	virtual VOID SetOwner( Character* pAttacker )
	{
		CharacterFinder::SetOwner(pAttacker);

		ASSERT( pAttacker != NULL );
		pAttacker->GetPos( &m_AttackerPos );
		m_AttackerSight = pAttacker->GetSightRange();
	}

	virtual VOID SetTargetType( eSKILL_TARGET_TYPE eType )
	{
		m_eTargetType = eType;
	}

private :
	float				m_AttackerSight;
	WzVector			m_AttackerPos;
	float				m_TargetDist;

	eSKILL_TARGET_TYPE	m_eTargetType;	// ã�ƾ��� Ÿ�� Ÿ��
};

//--------------------------------------------------------------------------------------------------------------
//
//										NearestCharacterFinder
//
//--------------------------------------------------------------------------------------------------------------
class SameObjectKeyFinder : public CharacterFinder
{
public :	
	SameObjectKeyFinder()
	{		
	}

	bool			operator () (Object* pObject)
	{
		Character* pCharacter = dynamic_cast<Character*>(pObject);
		ASSERT( pCharacter != NULL );

		if (pObject->GetObjectKey() == m_Value)
		{
			m_pTargetCharacter	= pCharacter;
			return true;
		}		

		// not found --> continue
		return false;
	}
};

//--------------------------------------------------------------------------------------------------------------
//
//										SelfCharacterFinder
//
//--------------------------------------------------------------------------------------------------------------
// ���� ã�´�.
class SelfCharacterFinder : public CharacterFinder
{
public :
	SelfCharacterFinder() 
	{
	}

	bool			operator () (Object* pObject)
	{
		ASSERT(m_pOwner != NULL);
		m_pTargetCharacter	= m_pOwner;
		return true;
	}
};