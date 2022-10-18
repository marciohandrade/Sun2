#pragma once
#include "scenebase.h"
#include "Narray.h"
#include "bink.h"

//------------------------------------------------------------------------------
/**
    ������ ó�� �������� ��, "MSG_CG_CONNECTION_ENTERSERVER_SYN" ��Ŷ�� ������ ������ ���۵ȴ�.
    "MSG_CG_CONNECTION_ENTERSERVER_SYN" �̰� ������ ĳ���� ����Ʈ�� ������鼭 ���۵Ǵµ�
    �ѹ� ���� �ٽ� ĳ���� ���� ȭ������ ���� ���� MSG_CG_CONNECTION_REENTERSERVER_SYN �̰� 
	������ ����Ʈ�� ��û�ؾ� �Ѵ�. �׷� ������ ������ ����.

    @author
    - ���翵,������ 

    @since

    @remarks
    - 2005.06.23 �ҽ� ����-����
    - 2005.06.23 ĳ���� ���� �۾� �߰� -���� 
    - 2005.06.27 ĳ���� ����,����,���� �۾� ���� �� ���� ����-���� 
    - 2005.07.05 ĳ���� ���� ���� �۾� -���� 


*/


namespace CHARACTERSCENE
{

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    const int MAX_CINEMATIC_CHARACTER = 12;
#else
    const int MAX_CINEMATIC_CHARACTER = 10;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    const DWORD GENERATE_KEY	= ONLYCLIENT_BATTLEZONE_OBJECT_KEY - 100;
#else
    const DWORD GENERATE_KEY	= MONSTER_OBJECT_KEY - 100;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    const DWORD BERSERKER		= GENERATE_KEY;
    const DWORD DRAGON  		= GENERATE_KEY + 2;
    const DWORD SHADOW			= GENERATE_KEY + 4;
    const DWORD VALKYRIE        = GENERATE_KEY + 6;
    const DWORD ELEMENTALIST    = GENERATE_KEY + 8;
    const DWORD BERSERKER_W		= GENERATE_KEY + 1;
    const DWORD DRAGON_W		= GENERATE_KEY + 3;
    const DWORD SHADOW_W		= GENERATE_KEY + 5;
    const DWORD VALKYRIE_W	    = GENERATE_KEY + 7;
    const DWORD ELEMENTALIST_W	= GENERATE_KEY + 9;
    const DWORD MYSTIC	        = GENERATE_KEY + 10; //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    const DWORD MYSTIC_W        = GENERATE_KEY + 11;
    const DWORD HELLROID        = GENERATE_KEY + 12; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    const DWORD HELLROID_W      = GENERATE_KEY + 13;
    const DWORD WITCHBLADE      = GENERATE_KEY + 14; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    const DWORD WITCHBLADE_W    = GENERATE_KEY + 15;
    const DWORD PLAYER_LIST		= GENERATE_KEY + 16;

    const DWORD THEOTHERS		= NPC_OBJECT_KEY - 100;

    const DWORD kBaseCinematicCharacter[(kTempClassTypeMax - 1)*2] = {
        // ��ǥ Ŭ����
        CHARACTERSCENE::BERSERKER, 
            CHARACTERSCENE::DRAGON,
            CHARACTERSCENE::SHADOW, 
            CHARACTERSCENE::VALKYRIE_W, 
            CHARACTERSCENE::ELEMENTALIST_W,
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
            CHARACTERSCENE::MYSTIC,
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            CHARACTERSCENE::HELLROID,
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            CHARACTERSCENE::WITCHBLADE_W,
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

            // �߰��� Ŭ����
            CHARACTERSCENE::BERSERKER_W, 
            CHARACTERSCENE::DRAGON_W,
            CHARACTERSCENE::SHADOW_W, 
            CHARACTERSCENE::VALKYRIE, 
            CHARACTERSCENE::ELEMENTALIST,
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
            CHARACTERSCENE::MYSTIC_W,
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            CHARACTERSCENE::HELLROID_W,
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            CHARACTERSCENE::WITCHBLADE,
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };

};

///	Ŀ���� �� �̵��� �⺻ī�޶���ġ��
struct CUSTOM_CHAR_CAMERA_VALUE
{
	float fYaw;
	float fPitch;
	float fDis;
	float fMinDis;
	float fMaxDis;
};

class Object;
class Character;
class SCSlotContainer;

struct CharListInfo
{
    DWORD					dwObjectKey;
    CLIENT_CHARACTER_PART	CharacterInfo;
};

struct posInfo
{
    DWORD       dwObjectKey;
    WzVector    wzVecPosition;
};

// ����ȭ�鿡�� ĳ���� ������ ���� ����Ŭ����
class ObjectStatus
{
    enum CHARACTER_STATUS
    { 
        CHARACTER_STATUS_NONE		    = 0, 
        CHARACTER_STATUS_MOVE_FRONT	    = 1,        //������ �ȱ�(����)
        CHARACTER_STATUS_MOVE_BACK	    = (2 << 1), //�ڷ� �ȱ�(���)
        CHARACTER_STATUS_SELECT_IDLE    = (2 << 2), //���� ����
        CHARACTER_STATUS_IDLE           = (2 << 3), //����
        CHARACTER_STATUS_SELECT         = (2 << 4), //���� 
        CHARACTER_STATUS_SELECT_OTHER   = (2 << 5), //�ٸ� ĳ�� ����
    };

    CHARACTER_STATUS m_CurrentStatus;
    BOOL m_Selected;

public:
    DWORD       object_key;             // ������Ʈ Ű
    float       animation_rate;         // �ִϸ��̼� ����� (0.0 ~ 1.0)
    int		    status_time;            // ����ó���� ����� �ð�
    int         status_time_backup;     // ����ó���� ����� �ð�2

public:
    ObjectStatus( DWORD ObjectKey, const WzVector& StandPosition )
    {
        m_Selected = FALSE;
        m_CurrentStatus = CHARACTER_STATUS_NONE;

        object_key = ObjectKey;                         //ĳ���� ���� �����  

        status_time = 0;
    }

    void SetSelect( BOOL bSelect ) { m_Selected = bSelect; }
    BOOL IsSelected() { return m_Selected; }

    void SetDefaultStatus() { m_CurrentStatus = CHARACTER_STATUS_NONE; }

    void SetMovingBackStatus();
    void SetIdleStatus();
    void SetSelectIdleStatus();
    void SetMovingFrontStatus();

    void SetStartMotion();

    BOOL IsMovingBackStatus() { return m_CurrentStatus == CHARACTER_STATUS_MOVE_BACK; }
    BOOL IsMovingFrontStatus() { return m_CurrentStatus == CHARACTER_STATUS_MOVE_FRONT; }

    BOOL IsSelectIdleStatus() { return m_CurrentStatus == CHARACTER_STATUS_SELECT_IDLE; }
    BOOL IsIdleStatus() { return m_CurrentStatus = CHARACTER_STATUS_IDLE; }
};

class CharacterScene : public SceneBase
{
	typedef	std::pair< DWORD, DWORD >	PARTS_NODE;
	typedef	std::list< PARTS_NODE >		PARTS_LIST_TYPE;
	typedef	PARTS_LIST_TYPE::iterator	PART_LIST_ITR_TYPE;

public:
    enum PACKET_STATUS_CHARACTERSCENE                         
    { 
        PACKET_STATUS_CHARACTERSCENE_NONE		= 0, 
        PACKET_STATUS_CHARACTERSCENE_CREATECHAR	= 1,        //ĳ���� ���� ��Ŷ
        PACKET_STATUS_CHARACTERSCENE_DELCHAR    = (2 << 1), //ĳ���� ���� ��Ŷ 
        PACKET_STATUS_CHARACTERSCENE_SELCHAR    = (2 << 2), //ĳ���� ���� ��Ŷ
        PACKET_STATUS_CHARACTERSCENE_CONECT     = (2 << 3), //���� ��û 
		PACKET_STATUS_CHARACTERSCENE_RECOVERY	= (2 << 4),
    };

    CharacterScene( void );
    virtual ~CharacterScene( void );

private:

public: 
	
    void            Activate( void );					    //	Ȱ��ȭ �Ǿ��� ��
    BOOL            Init( void );						    //	�ʱ�ȭ, �ε�
    BOOL            Process( DWORD dwTick );			    //	���� ���
	void            SetViewport();
    BOOL            Render( DWORD dwTick );			        //	���� ������
	BOOL            PostInterfaceRender( DWORD dwTick );			        //	���� ������
    void            Release( void );					    //	����
    void            ParsePacket( MSG_BASE * pMsg );	        //  ��Ʈ��ũ ��Ŷ ó��
    void            ProcessKeyInput( DWORD dwTick );
    void            ProcessMouse( DWORD dwTick );
    void            ProcessMouseCreateCharacterMode( DWORD dwTick );
    void            ProcessUnits( DWORD dwTick );
    void            ProcessState( Player * pPlayer,DWORD dwTick );

    void            ProcessCamera( LPDIMOUSESTATE2 pMouseState ,DWORD dwTick);
	void            CreatePlayerList( void );
    void            OnRecvCA_CHARINFO( MSG_BASE * pMsg );
    void            OnRecvCG_CHARINFO( MSG_BASE * pMsg );
    void            OnRecvCG_SYNC( MSG_BASE * pMsg );
    void            OnRecvCG_CONNECTION(MSG_BASE * pMsg);
	void            OnRecvCG_AREACONQUEST(MSG_BASE * pMsg);
	void            ShowErrorCode(DWORD dwErrorCode,BOOL bCreate=FALSE);

	void			OnRecvCG_EVENT( MSG_BASE * pMsg);
    void            OnRecvCharacterIDCheck(MSG_BASE* pMsg);

    BOOL            SendMakeCharPacket(DWORD classCode,const char *pName);	
    BOOL            SendDeleteCharPacket(const char *pString);	
    BOOL            SendCharSelectPacket();                 // ĳ���� ���� ��Ŷ�� ������.
    CharListInfo *  GetCurSelectCharacterInfo();
    CharListInfo *  GetCharacterInfoBySlotIndex(int slot_index);
    Character*      GetPickedChar(WzRay *pRay);
    
    void            MakeMyChar( DWORD ObjectKey, const CLIENT_CHARACTER_PART& CharacterInfo );
    VOID            DeleteMyChar(DWORD ObjKey);
  
    BOOL            IsSendCreateCharPacket();
	BOOL			IsSendSelectCharPacket();				// ������ ���� ��Ŷ�� ���ȳ�?

    //BOOL            CheckStatus(DWORD objKey,CHARACTER_STATUS status);
    ObjectStatus *  GetStatus(DWORD objKey);

	void            SetCreateCharacterMode(BOOL bMode);
//#ifdef _YJ_RENEWAL_CHARACTER_CREATE_SCENE
//#else //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
	void            ReleaseCreateCharacterResource();
    void            PlayCameraAnimation(WzID anim,BOOL bLoop);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
//#endif //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
	WzMatrix        GetCinematicBoneMatrix(int index);

public:
    void            RenderTexts( void );                    // �ؽ�Ʈ ���
    void            InitSlotForPositionAndAni();                  // ĳ���� ��� ��ġ ����
    void            SelectCharecter(Character * pChar);      // ĳ���� ���� 
//#ifdef _YJ_RENEWAL_CHARACTER_CREATE_SCENE
//#else //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
	void            RenderCharacterCreate();
//#endif //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
	void            MakeCinematicCharacter();
	void            ReleaseCinematicCharacter();
	void            RenderCinematicCharacter();
	void            SetObjectPosToCinematicMatrix(Object *pObject,int iboneindex);
	void            ProcessCreateCharacterMouse();

    void OnClickedChar(BYTE byClassCode, BYTE byModelCode);
    void OnSelectedCharacter(eCHAR_TYPE class_code);
    BYTE GetCharacterSlotNumber(){ return m_byCharacterSlot; }
    eCHAR_TYPE GetRace(DWORD class_code);
	void            OnCancelCreateCharacter();
	void            OnBackCreateCharacter();
	void            OnClickChangeHeight(BOOL bFoward);
	void            OnClickChangeHair(BOOL bFoward);
	void            OnClickChangeFace(BOOL bFoward);
	BOOL            IsCameraMoving();
	void            ProcessScrollInterface(DWORD dwTick);
	DWORD           GetSelectedClass();
	int             GetEmptySlot();//�̱ۿ� 
	DWORD           GetSelectCharOId();
	DWORD           GetSelectCharLevel();
	void            SetChangeSelectedCharacter(BOOL bChange);

	DWORD			GetSelectedObjectKey( void ) { return m_dwSelectedObjectKey; }


	void			SetCharOldAngle( void );
    void			SetCustomViewCamera();
	BOOL			SendCharacterIdCheckSyn( LPCTSTR szName );
	void			LoadCreateCharacterRoom();
    void			LoadCinematicCamera();

public:
	void			ForceDelChar();
	void			SEND_CHARINFO_RECOVER_SYN();
	BOOL			m_bVerifyLock;

	int				GetNumCharacters( void );

	void			SetCinematicCharShow( BOOL b );

#if defined(_DEBUG) || defined(_GAMECLIENT_VER)
    BOOL			SetSelectCharacterAutoTestByName(LPCTSTR CharacterName);
	void			SetSelectCharacterAutoTest(DWORD dwSelectCharacter);
#endif

//#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
//    WzVector m_camera;
//    WzVector m_character;
//    WzVector m_target;
//	float m_zoom;
//#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

private:
	void			    AddZeroWhenUnder10(TCHAR * pszNumber , int number);
	void			    ShowRecoveryErrorCode(DWORD errorCode);	
	void			    SelectedCharacterRotate( void );

	SCSlotContainer*	GetEquipContainer( DWORD objectkey );
	void			    Init_UIState();

private:
    typedef list<CharListInfo *>::iterator CHARLIST_ITR;
    list<CharListInfo *>	m_CharList;                     // ĳ���� ���
    DWORD					m_dwSelectCharacter;            // ���� ���õ� ĳ������ ���� �ε���
    DWORD					m_dwSelectOid;                  // ���� ���õ� ĳ������ ���� �ε���
	BOOL					m_bChangeSelectedCharacter;     //
    nArray<DWORD>           m_SelectionArray;
    std::vector<ObjectStatus*> m_StatusArray;

 
    DWORD                   m_curDelOid;
    DWORD                   m_curClassType;
    DWORD                   m_lastSelectionId;
	posInfo					m_PosInfoArray[MAX_CHARACTER_LIST_NUM];

    MSG_CG_CHARINFO_CREATE_SYN  m_SendCharCreatePacket;
    DWORD                   m_preTime;
	float                   m_fOriginalFarClip;

	// ĳ���� ���� ���� �׽�Ʈ 
	BOOL                    m_bCreateCharacter;
//#ifdef _YJ_RENEWAL_CHARACTER_CREATE_SCENE
//#else //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
	CWzUnitDraw             *m_pCreateMapUnitDraw;
    CWzUnit                 *m_pCreateMapUnit;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
//#endif //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
	DWORD                   m_dwCameraObjectKey;
	float                   m_fCameraRate;
	DWORD                   m_dwPickedObjectKey;
    BYTE                    m_bySelectedModel;
    BYTE                    m_byCharacterSlot;
	BYTE                    m_bySelectedClass;
	DWORD					m_dwSelectedObjectKey;

	PARTS_LIST_TYPE			m_PartsList;

	int                     m_iHeightVariation;
	int                     m_iHairVariation;
	int                     m_iFaceVariation;
	float					m_fRt;

	float					m_fOldAngle;

    BOOL                    m_bWorldServerConnected;
	BOOL					m_bMouseChange;

	BOOL					m_bLoadCreateCharacterRoom;

public:
    HANDLE	                m_hTexture;
    HANDLE                  m_hCharacterSceneTexture;
    BOOL			        m_bGameStart;
    BOOL                    m_bNextStage;
    BOOL                    m_bInitCam;

	BOOL					m_bConnected;
	DWORD                   m_dwInterfaceScroll;

	BOOL                    m_bPickCharLock;

    MSG_CG_CONNECTION_ENTERVILLAGEREQ_SYN	m_MsgCharSelectSync;

	void                    InitVariation();
    Player                  *GetCinematicCharacter(BYTE byClass);

	int                     GetCurrentHeightVariation();
	int                     GetCurrentHairVariation();
	int                     GetCurrentFaceVariation();

	DWORD					GetCurrentSelectedPlayerKey();

	St_FogParameter			m_stFogCharCreate;
	St_PE_BLUR				m_stBlurCharCreate;

	///	�⺻ ���� �ڵ�
	DWORD					m_BasicWeaponCode[MAX_CHARACTER_LIST_NUM];

	///	Ŀ���� ���� ī�޶� �⺻ ��ġ��
	CUSTOM_CHAR_CAMERA_VALUE	m_CustomCharValue[MAX_CHARACTER_LIST_NUM];

    void ShowCharacterSelectSceneUI();
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
public:
    void InitRenameProcess();
    bool NeedRenameProcess(); // �̸������� �ʿ�����.
    int GetNumOfNeedRename();
    bool NeedRenameByIndex(BYTE index);
    void UnsetRenameStatusByIndex(BYTE index);

private:
    void RecvRenameStatus(BYTE* rename_status);
    bool rename_status_[MAX_CHARACTER_LIST_NUM]; // �̸����� ���� �÷���
#endif //_JP_20110222_INGAME_CHARNAME_CHANGE

public:
	void ResetCameraPosition(eCHAR_TYPE class_type );
    void ShowOnlyCinematicCharacter(DWORD show_object_key);
    void EquipPartItem();
    void ReleaseAllParts(DWORD object_key);
    bool CheckEnableSelectClass(eCHAR_TYPE check_class_type);
    DWORD ChangeCreateClass(eCHAR_TYPE change_class_type);
    DWORD GetCreateClassObjectKey(eCHAR_TYPE change_class_type);
    void RotateCinematicCharacter(float angle);
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	void ZoomCamera( float rate );
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    void ShowSelectCharacter(int slot_index);
    void HideAllCharacter();
    void SetBackCinematicCharacter();
    void ChangeSelectCharacter(DWORD show_object_key);
    void ShowBaseCinematicCharacter();    

    void OnStartGame( int MapCode );

    DWORD create_selected_character_object_key(){ return create_selected_character_object_key_; }
private:
    void SelectCharecter(); // list���� ���õǾ� �ִ� ĳ���͸� ���� ���� �õ�    
//#ifdef _NA_20100307_BETAKEY_SYSTEM - ��ŸŰ ������� ������ ���������� ��
    bool IsMakeCharacterBetakey(BYTE class_code, eCHAR_TYPE base_gender_of_class);
//#endif//_NA_20100307_BETAKEY_SYSTEM

    void AddCinematicCharacter(DWORD class_code, DWORD object_key, int bone_index_for_position);
    void ProcessLoadCharacter(); 
    bool IsLoadedCreateRoom();

private:
    DWORD create_selected_character_object_key_;
    std::vector<DWORD> image_class_object_key_list_;

    bool is_loaded_my_character_;

    //------------------------------------------------------------------------------
    // �߱� ĳ���� ������ ������ ����
    struct
    {
        bool    is_show_movie;
        HBINK	bink;
        HANDLE	bink_texture;
        WzColor backup_clear_color;
    }Movie;
        
    void set_show_movie(bool val);
    bool NextFrame(HBINK bink);
};

inline BOOL CharacterScene::IsSendCreateCharPacket()
{
    return BIT_CHECK(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_CREATECHAR);
}

inline BOOL CharacterScene::IsSendSelectCharPacket()
{
    return BIT_CHECK(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_SELCHAR);
}

inline DWORD CharacterScene::GetSelectCharOId()
{
	return m_dwSelectCharacter;
}


inline void CharacterScene::SetChangeSelectedCharacter(BOOL bChange)
{
	m_bChangeSelectedCharacter=bChange;
}

inline int CharacterScene::GetNumCharacters( void )
{
	return m_CharList.size();
}

inline DWORD CharacterScene::GetCurrentSelectedPlayerKey()
{
    return m_dwSelectOid;
}

extern CharacterScene	g_CharSelectScene;
