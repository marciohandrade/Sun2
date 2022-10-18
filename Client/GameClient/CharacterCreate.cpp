#include "SunClientPrecompiledHeader.h"
#include "CharacterCreate.h"

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

#include "ObjectManager.h"
#include "Player.h"
#include "CharacterScene.h"
#include "AppearanceResourceCode.h"
#include "Camera.h"

#include "InterfaceManager.h"
#include "CastDialog.h"

cCharacterCreate* cCharacterCreate::ms_instance = NULL;
//
//namespace CHARACTERSCENE
//{
//	const DWORD GENERATE_KEY	= MONSTER_OBJECT_KEY - 100;
//	const DWORD BERSERKER		= GENERATE_KEY;
//	const DWORD DRAGON  		= GENERATE_KEY + 2;
//	const DWORD SHADOW			= GENERATE_KEY + 4;
//	const DWORD VALKYRIE        = GENERATE_KEY + 6;
//	const DWORD ELEMENTALIST    = GENERATE_KEY + 8;
//	const DWORD MYSTIC			= GENERATE_KEY + 10;
//
//	const DWORD BERSERKER_W		= GENERATE_KEY + 1;
//	const DWORD DRAGON_W		= GENERATE_KEY + 3;
//	const DWORD SHADOW_W		= GENERATE_KEY + 5;
//	const DWORD VALKYRIE_W	    = GENERATE_KEY + 7;
//	const DWORD ELEMENTALIST_W	= GENERATE_KEY + 9;
//
//	const DWORD MYSTIC_W		= GENERATE_KEY + 11;
//    const DWORD HELLROID        = GENERATE_KEY + 12;
//#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
//	const DWORD PLAYER_LIST		= GENERATE_KEY + 13;
//#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
//	const DWORD THEOTHERS		= NPC_OBJECT_KEY - 100;
//
//	const DWORD kBaseCinematicCharacter[(kTempClassTypeMax - 1)*2] = 
//	{
//		// 대표 클래스
//		CHARACTERSCENE::BERSERKER, 
//		CHARACTERSCENE::DRAGON,
//		CHARACTERSCENE::SHADOW, 
//		CHARACTERSCENE::VALKYRIE_W, 
//		CHARACTERSCENE::ELEMENTALIST_W,
//		CHARACTERSCENE::MYSTIC_W,
//		// 추가된 클래스
//		CHARACTERSCENE::BERSERKER_W, 
//		CHARACTERSCENE::DRAGON_W,
//		CHARACTERSCENE::SHADOW_W, 
//		CHARACTERSCENE::VALKYRIE, 
//		CHARACTERSCENE::ELEMENTALIST,
//		CHARACTERSCENE::MYSTIC,
//	};
//};


cCharacterCreate::cCharacterCreate()
{
	ms_instance = this;
}


void cCharacterCreate::Initialize()
{
	m_ZomeRate = 0.0f;
	SetVector(&m_CameraPosition, -47.31f, -4.914f, 1.2f);
	SetVector(&m_CharacterPosition, -47.286f, 0.f, 0.42f);
	SetVector(&m_TargetPosition, -47.31f, 0.f, 1.2f);
}

WzVector cCharacterCreate::GetStandPosition( eCHAR_TYPE CharacterType )
{
	WzVector Position;
	Position.x = GetCharacterX(CharacterType);
	Position.y = GetCharacterY(CharacterType);
    
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    Position.z = GetCharacterZ(CharacterType);
#else    
	Position.z = m_CharacterPosition.z;
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
	return Position;
}

void cCharacterCreate::GetCameraPosition( eCHAR_TYPE CharacterType, OUT WzVector& CameraPosition, OUT WzVector& CameraTargetPosition )
{
	// 각 캐릭터별 offset

	static float ho1 = 0.4f;
	static float ho2 = 0.4f;
	static float ho3 = 0.2f;
	static float ho4 = 0.2f;
	static float ho5 = 0.2f;
	static float ho6 = 0.0f;
    static float ho7 = 0.4f; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    static float ho8 = 0.2f; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	WzVector target_position = m_TargetPosition;
	switch (CharacterType)
	{
	case eCHAR_BERSERKER: target_position.z += ho1 * m_ZomeRate; break;
	case eCHAR_DRAGON: target_position.z += ho2 * m_ZomeRate; break;
	case eCHAR_SHADOW: target_position.z += ho3 * m_ZomeRate; break;
	case eCHAR_VALKYRIE: target_position.z += ho4 * m_ZomeRate; break;
	case eCHAR_ELEMENTALIST: target_position.z += ho5 * m_ZomeRate; break;
	case eCHAR_MYSTIC: target_position.z += ho6 * m_ZomeRate; break;
    case eCHAR_HELLROID: target_position.z += ho7 * m_ZomeRate; break; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eCHAR_WITCHBLADE: target_position.z += ho8 * m_ZomeRate; break; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	}

	CameraPosition = m_CameraPosition;
	CameraTargetPosition = target_position;

	WzVector Length = target_position - m_CameraPosition;
	Length *= 0.5f;
	Length *= m_ZomeRate;
	CameraPosition += Length;

}

void cCharacterCreate::ResetCameraPosition( eCHAR_TYPE CharacterType )
{
	m_CameraPosition.x = GetCameraX(CharacterType);
	m_TargetPosition.x = m_CameraPosition.x;
	m_ZomeRate = 0.0f;
}

void cCharacterCreate::AddCinematicCharacter(DWORD class_code, DWORD object_key, int bone_index_for_position)
{
	Player* cinematic_character = static_cast<Player*>(g_ObjectManager.Add(object_key, 
		PLAYER_OBJECT, 
		class_code, 
		FALSE));

	cinematic_character->RefreshAppearRender();

	SetObjectPosToCinematicMatrix(cinematic_character, bone_index_for_position);
}

void cCharacterCreate::ZoomCamera( float rate)
{
	m_ZomeRate += rate;

	if( m_ZomeRate > 1.0f )
		m_ZomeRate = 1.0f;

	if( m_ZomeRate < 0.0f )
		m_ZomeRate = 0.0f;
}

void cCharacterCreate::SetObjectPosToCinematicMatrix(Object *pObject,int iboneindex)
{
	eCHAR_TYPE class_type = eCHAR_NONE;
	Character* character = static_cast<Character*>(pObject);
	if (character)
	{
		class_type = character->GetClass();//character->GetClass();

		DWORD class_code = eCHAR_NONE;

		ClassCodeEx class_code_ex;
		class_code_ex.class_type = class_type;
		class_code_ex.gender = character->GetGender();
		class_code = class_code_ex.class_code;

		m_CharacterPosition.x = GetCharacterX(GetRace(class_code));
		m_CharacterPosition.y = GetCharacterY(GetRace(class_code));
		m_CameraPosition.x = GetCameraX(GetRace(class_code));
		m_TargetPosition.x = m_CameraPosition.x;
		m_ZomeRate = 0.0f;
	}

	WzVector vPos = m_CharacterPosition;

	if( pObject )
	{
		pObject->SetPosition( vPos );
		pObject->SetDirection(m_CameraPosition, vPos);
	}
}

void cCharacterCreate::MakeCinematicCharacter()
{
	const int kBoneIndexForCinematic[kTempClassTypeMax - 1] = {2, 3, 38, 33, 4, 4};

	DWORD object_key = CHARACTERSCENE::BERSERKER;
	eCHAR_TYPE class_Image_code = eCHAR_NONE;
	Player* cinematic_character = NULL;
	ClassCodeEx class_code_ex;
	for ( BYTE i = eCHAR_BERSERKER; i < kTempClassTypeMax; ++i)
	{
		eCHAR_TYPE CharacterType = (eCHAR_TYPE)i;

		// 남성
		class_code_ex.class_type = CharacterType;
		class_code_ex.gender = GenderType::kMale;
		class_Image_code = ResourceCode::GetImageClassType( static_cast<eCHAR_TYPE>(class_code_ex.class_type), static_cast<GenderType::Value>(class_code_ex.gender));
		if( Player* cinematic_character = static_cast<Player*>(g_ObjectManager.Add(object_key++, PLAYER_OBJECT, class_code_ex.class_code, FALSE)) )
		{
			cinematic_character->RefreshAppearRender();

			WzVector CameraPosition = m_CameraPosition;
			WzVector CharacterPosition = m_CharacterPosition;
			CameraPosition.x = GetCharacterX( GetRace(class_code_ex.class_code) );
			CharacterPosition.x = GetCharacterX( GetRace(class_code_ex.class_code) );
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            if ( i == eCHAR_HELLROID )
            {
                CharacterPosition.z = GetCharacterZ(GetRace(class_code_ex.class_code) );
            }
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID

			cinematic_character->SetPosition( CharacterPosition );
			cinematic_character->SetDirection( CameraPosition, CharacterPosition );
			cinematic_character->SetAnimation( StrToWzID("N001"), TRUE );
		}

		//여성
		class_code_ex.class_type = CharacterType;
		class_code_ex.gender = GenderType::kFemale;
		class_Image_code = ResourceCode::GetImageClassType( static_cast<eCHAR_TYPE>(class_code_ex.class_type), static_cast<GenderType::Value>(class_code_ex.gender));
		if( Player* cinematic_character = static_cast<Player*>(g_ObjectManager.Add(object_key++, PLAYER_OBJECT, class_code_ex.class_code, FALSE)) )
		{
			cinematic_character->RefreshAppearRender();

			WzVector CameraPosition = m_CameraPosition;
			WzVector CharacterPosition = m_CharacterPosition;
			CameraPosition.x = GetCharacterX( GetRace(class_code_ex.class_code) );
			CharacterPosition.x = GetCharacterX( GetRace(class_code_ex.class_code) );
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            if ( i == eCHAR_WITCHBLADE )
            {
                CharacterPosition.z = GetCharacterZ(GetRace(class_code_ex.class_code) );
            }
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
			cinematic_character->SetPosition( CharacterPosition );
			cinematic_character->SetDirection( CameraPosition, CharacterPosition );
			cinematic_character->SetAnimation( StrToWzID("N001"), TRUE );
		}

	}
}

void cCharacterCreate::ProcessCamera(eCHAR_TYPE CharacterType)
{
	// 카메라 초기화 
	WzVector wzCamera, wzTarget;

	static float ho1 = 0.4f;
	static float ho2 = 0.4f;
	static float ho3 = 0.2f;
	static float ho4 = 0.2f;
	static float ho5 = 0.2f;
	static float ho6 = 0.0f;
    static float ho7 = 0.4f; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    static float ho8 = 0.2f; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE    

	WzVector target_position = m_TargetPosition;
	switch (CharacterType)
	{
	case eCHAR_BERSERKER: target_position.z += ho1 * m_ZomeRate; break;
	case eCHAR_DRAGON: target_position.z += ho2 * m_ZomeRate; break;
	case eCHAR_SHADOW: target_position.z += ho3 * m_ZomeRate; break;
	case eCHAR_VALKYRIE: target_position.z += ho4 * m_ZomeRate; break;
	case eCHAR_ELEMENTALIST: target_position.z += ho5 * m_ZomeRate; break;
	case eCHAR_MYSTIC: target_position.z += ho6 * m_ZomeRate; break;
    case eCHAR_HELLROID: target_position.z += ho7 * m_ZomeRate; break; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eCHAR_WITCHBLADE: target_position.z += ho8 * m_ZomeRate; break; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	}

	wzCamera = m_CameraPosition;
	wzTarget = target_position;

	WzVector Length = target_position - m_CameraPosition;
	Length *= 0.5f;
	Length *= m_ZomeRate;
	wzCamera += Length;
	//SetVector(&wzCamera, m_camera.x, m_camera.y, m_camera.z);// -47.31f, -3.784f, 1.2f);
	//SetVector(&wzTarget, m_target.x, m_target.y, m_target.z);//-47.286f, 0.f, 0.42f);

	g_Camera.SetCameraFromTo(wzCamera,wzTarget);

	g_Camera.SetForceSpringCamera( FALSE );
	g_Camera.UpdateCamera();
}

eCHAR_TYPE cCharacterCreate::GetRace(DWORD code)
{
	eCHAR_TYPE class_type = eCHAR_NONE;

	ClassCodeEx class_code;
    class_code.class_code = code;
    switch (class_code.class_type)
    {
    case eCHAR_BERSERKER:
        if (class_code.gender== GenderType::kMale)
        {
            class_type = eCHAR_BERSERKER;
        }
        else
        {
            class_type = eCHAR_VALKYRIE;
        }
        break;
    case eCHAR_DRAGON:
        if (class_code.gender == GenderType::kMale)
        {
            class_type = eCHAR_DRAGON;
        }
        else
        {
            class_type = eCHAR_VALKYRIE;
        }
        break;
    case eCHAR_SHADOW:
        if (class_code.gender == GenderType::kMale)
        {
            class_type = eCHAR_SHADOW;
        }
        else
        {
            class_type = eCHAR_ELEMENTALIST;
        }
        break;
    case eCHAR_VALKYRIE:
        if (class_code.gender == GenderType::kMale)
        {
            class_type = eCHAR_DRAGON;
        }
        else
        {
            class_type = eCHAR_VALKYRIE;
        }
        break;
    case eCHAR_ELEMENTALIST:   
        if (class_code.gender == GenderType::kMale)
        {
            class_type = eCHAR_SHADOW;
        }
        else
        {
            class_type = eCHAR_ELEMENTALIST;
        }
        break;
    case eCHAR_MYSTIC:   
        /*if (class_code.gender == GenderType::kMale)
        {
            class_type = eCHAR_MYSTIC;
        }
        else*/
        {
            class_type = eCHAR_MYSTIC;
        }
        break;
    case eCHAR_HELLROID:
        {
            //_NA_000000_20130812_NEW_CHARACTER_HELLROID
            class_type = eCHAR_HELLROID;
        }
        break;
    case eCHAR_WITCHBLADE:
        {
            //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            class_type = eCHAR_WITCHBLADE;
        }
        break;
    }
    return class_type;
}

float cCharacterCreate::GetCameraX(eCHAR_TYPE CharacterType)
{
	float x_position = 0.0f;
	if (CharacterType == eCHAR_BERSERKER)
	{
		x_position =  -47.375f;
	}
	else
	{
		x_position = GetCharacterX(CharacterType);
	}
	return x_position;
}

float cCharacterCreate::GetCharacterX(eCHAR_TYPE CharacterType)
{
	float position = 0.0f;
	switch (CharacterType)
	{
	case eCHAR_BERSERKER:
		position =  -47.325f;
		break;
	case eCHAR_DRAGON:
		position =  -28.373f;
		break;
	case eCHAR_SHADOW:
		position =  -9.457f;
		break;
	case eCHAR_MYSTIC:
		position =  9.476f;
		break;
	case eCHAR_VALKYRIE:
		position =  28.418f;
		break;
	case eCHAR_ELEMENTALIST:
		position =  47.311f;
		break;;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        position = 66.206f;
        break;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE        
        position = 85.13f;
        break;
	}
	return position;
}

float cCharacterCreate::GetCharacterY(eCHAR_TYPE CharacterType)
{
	return 0.0f;
	//float position = 0.0f;
	//switch (CharacterType)
	//{
	//case eCHAR_BERSERKER:
	//	position = 0.f;
	//	break;
	//case eCHAR_DRAGON:
	//	position = 0.f;
	//	break;
	//case eCHAR_SHADOW:
	//	position = 0.f;
	//	break;
	//case eCHAR_MYSTIC:
	//	position = 0.0f;//-1.255f;
	//	break;
	//case eCHAR_VALKYRIE:
	//	position = 0.f;
	//	break;
	//case eCHAR_ELEMENTALIST:
	//	position = 0.f;
	//	break;;
	//}
	//return position;
}

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
float cCharacterCreate::GetCharacterZ( eCHAR_TYPE class_type )
{
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if ( class_type == eCHAR_HELLROID || class_type == eCHAR_WITCHBLADE )
#else
    if ( class_type == eCHAR_HELLROID )
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    {
        return (m_CharacterPosition.z - 0.2f);
    }
    else
    {
        return m_CharacterPosition.z;
    }
}
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT