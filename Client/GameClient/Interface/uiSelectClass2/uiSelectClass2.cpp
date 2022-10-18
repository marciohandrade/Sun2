#include "SunClientPrecompiledHeader.h"
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#include "uiSelectClass2.h"
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#include "CharacterScene.h"
#include "GlobalFunc.h"
#include "uiLoginBack/uiLoginBackTop.h"
#include "CreateCharCustomDialog.h"
#include "uiCreateCharacterMan/uiCreateCharacterMan.h"

SelectClass2::SelectClass2( void )
{
    current_class_type_ = eCHAR_BERSERKER;
}
SelectClass2::~SelectClass2()
{
    Release();
}

void SelectClass2::Init( void )
{   
    current_class_type_ = eCHAR_BERSERKER;    
}

void SelectClass2::Release( void )
{    
}

// CustomDialog에서 불러서 사용
void SelectClass2::OnExcute( void )
{    
    eCHAR_TYPE select_class_type = static_cast<eCHAR_TYPE>(current_class_type_);
    
    if (select_class_type == eCHAR_NONE)
    {
        return;
    }
    else
    {    
        select_create_class(select_class_type, get_selected_class_code());
    }
}

// 제일 중요한 함수 여기서 불러서 셋팅
void SelectClass2::select_create_class( eCHAR_TYPE select_class_type, BYTE select_class_code )
{
    if (g_CharSelectScene.CheckEnableSelectClass(select_class_type) == true)
    {
        // Top UI에 클래스명 및 마크 세팅
        uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
        if (login_back_top != NULL)
        {
            DWORD class_name_code = GlobalFunc::GetClassNameCode(select_class_type);
            login_back_top->SetSceneTitleClassName(select_class_type, class_name_code);
        }
    }

    // UI 상태 초기화 - 커스터마이즈/장비 관련
    CreateCharCustomDialog* create_character_custom_dialog_ptr = GET_CAST_DIALOG(CreateCharCustomDialog, IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG);
    if (create_character_custom_dialog_ptr != NULL)
    {
        create_character_custom_dialog_ptr->first_setting_reset();
        g_CharSelectScene.InitVariation();
        create_character_custom_dialog_ptr->UpdateVariationText();

        // 선택된 클래스 캐릭터로 교체
        DWORD change_create_class = g_CharSelectScene.ChangeCreateClass(select_class_type);
        create_character_custom_dialog_ptr->set_selected_character_object_key(change_create_class);
        create_character_custom_dialog_ptr->InsertWeaponParts(select_class_type);
    }
    g_CharSelectScene.OnClickedChar(select_class_type, select_class_code);
}
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES