#pragma once

#include "uiBase.h"

#include "HeroCustomInfoParser.h"

#define MAX_KIND_CHARACTER	5

class InterfaceManager;

struct WEAPON_DISPLAY_TYPE
{
	std::string	name;
	DWORD		itemcode;
};

struct ARMOR_DISPLAY_TYPE
{
	std::string	name;
	DWORD		itemcode[MAX_ARMORCODE];
};

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#define CREATE_CLASS_COUNT 8
#else
  #ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
      #define CREATE_CLASS_COUNT 7
  #else //_NA_000000_20130812_NEW_CHARACTER_HELLROID
      #define CREATE_CLASS_COUNT 6
  #endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE


class CreateCharCustomDialog : public uiBase
{
	typedef	std::vector<WEAPON_DISPLAY_TYPE>	WEAPON_DISPLAY_VECTOR_TYPE;
	typedef	std::vector<ARMOR_DISPLAY_TYPE>		ARMOR_DISPLAY_VECTOR_TYPE;

public:

	enum CONTROL_ID
	{
		NAME_EDIT,
		NAMECONFIRM_BTN,
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        CHECK_RACE1,
        CHECK_RACE2,
        CHECK_RACE3,
        CHECK_RACE4,
        CHECK_RACE5,
        CHECK_RACE6,
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        CHECK_RACE7,
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        CHECK_RACE8,
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE


        CHECK_CLASS_BERSERKER,
        CHECK_CLASS_DRAGON,
        CHECK_CLASS_SHADOW,
        CHECK_CLASS_VALKYRIE,
        CHECK_CLASS_ELEMENTALIST,
        CHECK_CLASS_MYSTIC,
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        CHECK_CLASS_HELLROID,
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        CHECK_CLASS_WITCHBLADE,
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

#else
        BERSERKER_CLASS_CHECK,
        DRAGON_CLASS_CHECK,
        SHADOW_CLASS_CHECK,
        VALKYRIE_CLASS_CHECK,
        ELEMENTALIST_CLASS_CHECK,
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

		HAIR_FORWARD_BTN,
		HAIR_BACKWARD_BTN,
		FACE_FORWARD_BTN,
		FACE_BACKWARD_BTN,
		HEIGHT_FORWARD_BTN,
		HEIGHT_BACKWARD_BTN,

		HAIR_TEXT,
		FACE_TEXT,
		HEIGHT_TEXT,

		WEAPON_FORWARD_BTN,
		WEAPON_BACKWARD_BTN,

		ARMOR_FORWARD_BTN,
		ARMOR_BACKWARD_BTN,

		WEAPON_TEXT,
		ARMOR_TEXT,

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
		WEAR_BTN,
		INIT_BTN,
        COSTUME_TEXT, // 현재까지 기능은 없음
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        NAME_TEXT,  // 이름을 입력해 주세요.

		MAX_CONTROL
	};

	CreateCharCustomDialog( InterfaceManager * pUIman);
	virtual ~CreateCharCustomDialog( void );

	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	OnUpdateSolarDialog();

	void    InitText();

	virtual VOID	OnShowWindow( BOOL val );

	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	void    SetClass( BYTE byClass );
	void    GetCharName( TCHAR *szName, BOOL bClear = FALSE );
	void	SetCharName( TCHAR * szName );

	void	UpdateVariationText();


	BOOL	LoadParts(DWORD object_key);

	BOOL	InsertParts( void );
	BOOL	DeleteParts( void );

    void first_setting_reset();
    void first_setting_name_off()      { is_first_setting_name_ = false; }
    void first_setting_hair_off()      { is_first_setting_hair_ = false; }
    void first_setting_face_off()      { is_first_setting_face_ = false; }
    void first_setting_height_off()    { is_first_setting_height_ = false; }
    void first_setting_costume_off()   { is_first_setting_costume_ = false; }
    void first_setting_weapon_off()    { is_first_setting_weapon_ = false; }
    void first_setting_armor_off()	   { is_first_setting_armor_ = false; }

    bool is_first_setting_name()       { return is_first_setting_name_; }
    bool is_first_setting_hair()       { return is_first_setting_hair_; }
    bool is_first_setting_face()       { return is_first_setting_face_; }
    bool is_first_setting_height()     { return is_first_setting_height_; }
    bool is_first_setting_costume()    { return is_first_setting_costume_; }
    bool is_first_setting_weapon()     { return is_first_setting_weapon_; }
    bool is_first_setting_armor()	   { return is_first_setting_armor_; }

    BOOL InsertWeaponParts(eCHAR_TYPE select_class_type);
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    void OnClickClass(eCHAR_TYPE select_class);
    void OnClickRace(eCHAR_TYPE select_race);
#else
    void RefreshClassTabState();
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    eCHAR_TYPE GetSelectedCharacterClass();
    GenderType::Value GetSelectedCharacterGender();
    void set_selected_character_object_key(DWORD val){ selected_character_object_key_ = val; }
    DWORD selected_character_object_key(){ return selected_character_object_key_; }

#ifdef _NA_000000_20130114_RENEWER_UI
	struct stTARGETOVER_
	{
		RECT image_source_;
		POSTYPE screen_position;
		bool is_over_mouse;
	};

	void RenderMouseOver();
	virtual VOID	OnRenderSolarDialog();
#endif

private:
	static	WzID				m_wzId[MAX_CONTROL];

	CCtrlButtonCheckWZ *		m_pExpainBtn;
	CCtrlButtonCheckWZ *		m_pCustomBtn;

	CCtrlStaticWZ	*			m_pWeaponName;
	CCtrlStaticWZ	*			m_pArmorName;

	WEAPON_DISPLAY_VECTOR_TYPE	m_WeaponItemVec;
	ARMOR_DISPLAY_VECTOR_TYPE	m_ArmorItemVec;

	std::string					m_ArmorName;

	int							m_WeaponPos;
	int							m_ArmorPos;

	DWORD						m_BasicWeaponCode[MAX_KIND_CHARACTER];

    //! 처음 보일때는 설정 txt 에 설정대신 이름을 보여주기 위함
    bool is_first_setting_name_;

    bool is_first_setting_hair_;
    bool is_first_setting_face_;
    bool is_first_setting_height_;
    bool is_first_setting_costume_;

    bool is_first_setting_weapon_;
    bool is_first_setting_armor_;

    DWORD selected_character_object_key_;
    POSTYPE mouse_over_pos_;

    CDrawBase* draw_base_;
    HANDLE background_texture_; // 배경 텍스처 (init()에서 로드해준다.)

	//내가 선택한 클래스
	eCHAR_TYPE is_now_char_Type_;
#ifdef _NA_000000_20130114_RENEWER_UI
	stTARGETOVER_ render_over_iamge_[CREATE_CLASS_COUNT*2];
	RECT		  isFillRectPos;
	HANDLE	      render_over_texture_;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    HANDLE        render_over_texture2_; // Common_bg02.tga 위치블레이드 texture가 여기에 있습니다.
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#endif
};
