#pragma once

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

class Object;

class cCharacterCreate
{
private:
	static cCharacterCreate* ms_instance;

public:
	cCharacterCreate();

public:
	static cCharacterCreate* Instance()
	{
		return ms_instance;
	}

private:
	WzVector m_CameraPosition;		// �ʱ� ī�޶� ��ġ
	WzVector m_CharacterPosition;	// �ʱ� ĳ���� ��ġ
	WzVector m_TargetPosition;		// �ʱ� ��� ��ġ
	float m_ZomeRate;				// ���� ����

public:

	void Initialize();
	void GetCameraPosition( eCHAR_TYPE CharacterType, OUT WzVector& CameraPosition, OUT WzVector& CameraTargetPosition );
	void ResetCameraPosition( eCHAR_TYPE CharacterType );

	void ZoomCamera( float rate );

	WzVector GetStandPosition( eCHAR_TYPE CharacterType );

	const WzVector& GetCharacterPosition() const { return m_CharacterPosition; }
	const WzVector& GetTargetPosition() const { return m_TargetPosition; }
	const WzVector& GetCameraPosition() const { return m_CameraPosition; }


	void AddCinematicCharacter(DWORD class_code, DWORD object_key, int bone_index_for_position);
	void MakeCinematicCharacter();

	void SetObjectPosToCinematicMatrix(Object *pObject,int iboneindex);

	void ProcessCamera(eCHAR_TYPE CharacterType);

	bool IsLoadedCreateRoom();

	float GetCameraX(eCHAR_TYPE class_type);
	float GetCharacterX(eCHAR_TYPE class_type);
	float GetCharacterY(eCHAR_TYPE class_type);
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    float GetCharacterZ(eCHAR_TYPE class_type);
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID

	eCHAR_TYPE GetRace(DWORD code);

};

#define xGetCharacterCreate() cCharacterCreate::Instance()

#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
