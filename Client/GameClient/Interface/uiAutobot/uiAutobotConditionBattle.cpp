#include "SunClientPrecompiledHeader.h"

#ifdef _YMS_AUTOBOT_GAME

#include "uiAutobotConditionBattle.h"
#include "uiAutobotMan.h"
#include "uiAutobotConditionSupport.h"
#include "AutomaticInfoParser.h"
#include "AutomaticComboInfoParser.h"

#include "Autobot/Condition/AutobotConditionManager.h"

#include "GameFramework.h"
#include "SceneBase.h"
#include "Map.h"
#include "Hero.h"
#include "MouseHandler.h"
#include "Interface/uiPartyMan/uiPartyMan.h"
#include "GlobalFunc.h"
#include "QuickSlot.h"
#include "SCItemSlot.h"
#include "SCSkillSlot.h"
#include "StyleSlot.h"
#include "SlotKeyGenerator.h"
#include "ItemManager.h"
#include "ItemInfoParser.h"
#include "SkillInfoParser.h"
#include "GlobalData.h"
#include "HeroEquipmentContainer.h"
#include "uiToolTipMan/uiToolTipMan.h"

//------------------------------------------------------------------------------ 
const DWORD uiAutobotConditionBattle::kDialog_ID = StrToWzID("9510");
const int uiAutobotConditionBattle::kSlotSize = 24;
const int uiAutobotConditionBattle::kDefaultCooltimeSecond = 60;

//------------------------------------------------------------------------------ 
uiAutobotConditionBattle::uiAutobotConditionBattle(InterfaceManager* ui_man) : 
    uiAutobotDialog(ui_man),
    m_pItemUnitRenderer(NULL)
{
    QuickSlotContainer::Init(uiAutobotConditionBattle::kSlotSize, SI_AUTOMATIC_INFO );

    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
    RegisterControlPosition(StrToWzID("C007"), kControl_CheckBT_C007);
    RegisterControlPosition(StrToWzID("C009"), kControl_CheckBT_C009);
    RegisterControlPosition(StrToWzID("C010"), kControl_CheckBT_C010);
    RegisterControlPosition(StrToWzID("C011"), kControl_CheckBT_C011);
    RegisterControlPosition(StrToWzID("C012"), kControl_CheckBT_C012);
    RegisterControlPosition(StrToWzID("C013"), kControl_CheckBT_C013);
    RegisterControlPosition(StrToWzID("C014"), kControl_CheckBT_C014);
    RegisterControlPosition(StrToWzID("C015"), kControl_CheckBT_C015);
    RegisterControlPosition(StrToWzID("C016"), kControl_CheckBT_C016);
    RegisterControlPosition(StrToWzID("C017"), kControl_CheckBT_C017);
    RegisterControlPosition(StrToWzID("C018"), kControl_CheckBT_C018);
    RegisterControlPosition(StrToWzID("C024"), kControl_CheckBT_C024);
    RegisterControlPosition(StrToWzID("C025"), kControl_CheckBT_C025);
    RegisterControlPosition(StrToWzID("C026"), kControl_CheckBT_C026);
    RegisterControlPosition(StrToWzID("C027"), kControl_CheckBT_C027);
    RegisterControlPosition(StrToWzID("C030"), kControl_CheckBT_C030);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);

    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B013"), kControl_Button_B013);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("B015"), kControl_Button_B015);
    RegisterControlPosition(StrToWzID("B014"), kControl_Button_B014);
    RegisterControlPosition(StrToWzID("B016"), kControl_Button_B016);
    RegisterControlPosition(StrToWzID("B017"), kControl_Button_B017);
    RegisterControlPosition(StrToWzID("B018"), kControl_Button_B018);
    RegisterControlPosition(StrToWzID("B019"), kControl_Button_B019);
    RegisterControlPosition(StrToWzID("B020"), kControl_Button_B020);
    RegisterControlPosition(StrToWzID("B021"), kControl_Button_B021);
    RegisterControlPosition(StrToWzID("B022"), kControl_Button_B022);
    RegisterControlPosition(StrToWzID("B023"), kControl_Button_B023);
    RegisterControlPosition(StrToWzID("B024"), kControl_Button_B024);
    RegisterControlPosition(StrToWzID("B025"), kControl_Button_B025);
    RegisterControlPosition(StrToWzID("B026"), kControl_Button_B026);
    RegisterControlPosition(StrToWzID("B027"), kControl_Button_B027);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("B029"), kControl_Button_B029);
    RegisterControlPosition(StrToWzID("B030"), kControl_Button_B030);
    RegisterControlPosition(StrToWzID("B031"), kControl_Button_B031);
    RegisterControlPosition(StrToWzID("B032"), kControl_Button_B032);
    RegisterControlPosition(StrToWzID("B033"), kControl_Button_B033);

    RegisterControlPosition(StrToWzID("T101"), kControl_Text_T101);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("T006"), kControl_Text_T006);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S010"), kControl_Text_S010);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);
    RegisterControlPosition(StrToWzID("S008"), kControl_Text_S008);
    RegisterControlPosition(StrToWzID("S009"), kControl_Text_S009);
    RegisterControlPosition(StrToWzID("S012"), kControl_Text_S012);
    RegisterControlPosition(StrToWzID("S019"), kControl_Text_S019);
    RegisterControlPosition(StrToWzID("S020"), kControl_Text_S020);
    RegisterControlPosition(StrToWzID("S021"), kControl_Text_S021);
    RegisterControlPosition(StrToWzID("S022"), kControl_Text_S022);
    RegisterControlPosition(StrToWzID("S025"), kControl_Text_S025);
    RegisterControlPosition(StrToWzID("S026"), kControl_Text_S026);
    RegisterControlPosition(StrToWzID("S035"), kControl_Text_S035);
    RegisterControlPosition(StrToWzID("S047"), kControl_Text_S047);
    RegisterControlPosition(StrToWzID("S048"), kControl_Text_S048);
    RegisterControlPosition(StrToWzID("S049"), kControl_Text_S049);
    RegisterControlPosition(StrToWzID("S050"), kControl_Text_S050);
    RegisterControlPosition(StrToWzID("S051"), kControl_Text_S051);
    RegisterControlPosition(StrToWzID("S011"), kControl_Text_S011);
    RegisterControlPosition(StrToWzID("S013"), kControl_Text_S013);
    RegisterControlPosition(StrToWzID("S014"), kControl_Text_S014);
    RegisterControlPosition(StrToWzID("S015"), kControl_Text_S015);
    RegisterControlPosition(StrToWzID("S016"), kControl_Text_S016);
    RegisterControlPosition(StrToWzID("S017"), kControl_Text_S017);
    RegisterControlPosition(StrToWzID("S018"), kControl_Text_S018);
    RegisterControlPosition(StrToWzID("S023"), kControl_Text_S023);
    RegisterControlPosition(StrToWzID("S024"), kControl_Text_S024);
    RegisterControlPosition(StrToWzID("S027"), kControl_Text_S027);
    RegisterControlPosition(StrToWzID("S028"), kControl_Text_S028);
    RegisterControlPosition(StrToWzID("S029"), kControl_Text_S029);
    RegisterControlPosition(StrToWzID("S030"), kControl_Text_S030);
    RegisterControlPosition(StrToWzID("S031"), kControl_Text_S031);
    RegisterControlPosition(StrToWzID("S032"), kControl_Text_S032);
    RegisterControlPosition(StrToWzID("T021"), kControl_Text_T021);
    RegisterControlPosition(StrToWzID("S036"), kControl_Text_S036);
    RegisterControlPosition(StrToWzID("S037"), kControl_Text_S037);
    RegisterControlPosition(StrToWzID("S038"), kControl_Text_S038);
    RegisterControlPosition(StrToWzID("S039"), kControl_Text_S039);
    RegisterControlPosition(StrToWzID("S040"), kControl_Text_S040);
    RegisterControlPosition(StrToWzID("S041"), kControl_Text_S041);
    RegisterControlPosition(StrToWzID("S042"), kControl_Text_S042);
    RegisterControlPosition(StrToWzID("S043"), kControl_Text_S043);
    RegisterControlPosition(StrToWzID("S044"), kControl_Text_S044);
    RegisterControlPosition(StrToWzID("S045"), kControl_Text_S045);
    RegisterControlPosition(StrToWzID("S046"), kControl_Text_S046);
    RegisterControlPosition(StrToWzID("S034"), kControl_Text_S034);
    RegisterControlPosition(StrToWzID("S033"), kControl_Text_S033);

    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
    RegisterControlPosition(StrToWzID("E001"), kControl_Edit_E001);
    RegisterControlPosition(StrToWzID("E002"), kControl_Edit_E002);
    RegisterControlPosition(StrToWzID("E003"), kControl_Edit_E003);
    RegisterControlPosition(StrToWzID("E004"), kControl_Edit_E004);
    RegisterControlPosition(StrToWzID("E005"), kControl_Edit_E005);
    RegisterControlPosition(StrToWzID("E006"), kControl_Edit_E006);
    RegisterControlPosition(StrToWzID("E007"), kControl_Edit_E007);
    RegisterControlPosition(StrToWzID("E008"), kControl_Edit_E008);
    RegisterControlPosition(StrToWzID("E009"), kControl_Edit_E009);
    RegisterControlPosition(StrToWzID("E010"), kControl_Edit_E010);
    RegisterControlPosition(StrToWzID("E011"), kControl_Edit_E011);

    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("L002"), kControl_List_L002);
    RegisterControlPosition(StrToWzID("L003"), kControl_List_L003);
    RegisterControlPosition(StrToWzID("L004"), kControl_List_L004);
    RegisterControlPosition(StrToWzID("L005"), kControl_List_L005);
    RegisterControlPosition(StrToWzID("L006"), kControl_List_L006);
    RegisterControlPosition(StrToWzID("L007"), kControl_List_L007);

    RegisterControlPosition(StrToWzID("P013"), kControl_Picture_P013);
    RegisterControlPosition(StrToWzID("P015"), kControl_Picture_P015);
    RegisterControlPosition(StrToWzID("P017"), kControl_Picture_P017);
    RegisterControlPosition(StrToWzID("P018"), kControl_Picture_P018);
    RegisterControlPosition(StrToWzID("P019"), kControl_Picture_P019);
    RegisterControlPosition(StrToWzID("P020"), kControl_Picture_P020);
    RegisterControlPosition(StrToWzID("P021"), kControl_Picture_P021);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
    RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);
    RegisterControlPosition(StrToWzID("P011"), kControl_Picture_P011);
    RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);
    RegisterControlPosition(StrToWzID("P023"), kControl_Picture_P023);
    RegisterControlPosition(StrToWzID("P024"), kControl_Picture_P024);
    RegisterControlPosition(StrToWzID("P025"), kControl_Picture_P025);
    RegisterControlPosition(StrToWzID("P026"), kControl_Picture_P026);
    RegisterControlPosition(StrToWzID("P027"), kControl_Picture_P027);
    RegisterControlPosition(StrToWzID("P028"), kControl_Picture_P028);
    RegisterControlPosition(StrToWzID("P029"), kControl_Picture_P029);
    RegisterControlPosition(StrToWzID("P014"), kControl_Picture_P014);
    RegisterControlPosition(StrToWzID("P022"), kControl_Picture_P022);
    RegisterControlPosition(StrToWzID("P030"), kControl_Picture_P030);
    RegisterControlPosition(StrToWzID("P031"), kControl_Picture_P031);
    RegisterControlPosition(StrToWzID("P043"), kControl_Picture_P043);
    RegisterControlPosition(StrToWzID("P042"), kControl_Picture_P042);
    RegisterControlPosition(StrToWzID("P041"), kControl_Picture_P041);
    RegisterControlPosition(StrToWzID("P040"), kControl_Picture_P040);
    RegisterControlPosition(StrToWzID("P039"), kControl_Picture_P039);
    RegisterControlPosition(StrToWzID("P038"), kControl_Picture_P038);
    RegisterControlPosition(StrToWzID("P037"), kControl_Picture_P037);
    RegisterControlPosition(StrToWzID("P036"), kControl_Picture_P036);
    RegisterControlPosition(StrToWzID("P035"), kControl_Picture_P035);
    RegisterControlPosition(StrToWzID("P034"), kControl_Picture_P034);
    RegisterControlPosition(StrToWzID("P033"), kControl_Picture_P033);
    RegisterControlPosition(StrToWzID("P032"), kControl_Picture_P032);
    RegisterControlPosition(StrToWzID("PI04"), kControl_Picture_PI04);
    RegisterControlPosition(StrToWzID("P044"), kControl_Picture_P044);
    RegisterControlPosition(StrToWzID("P045"), kControl_Picture_P045);
    RegisterControlPosition(StrToWzID("P046"), kControl_Picture_P046);
    RegisterControlPosition(StrToWzID("P047"), kControl_Picture_P047);
    RegisterControlPosition(StrToWzID("P048"), kControl_Picture_P048);
    RegisterControlPosition(StrToWzID("P050"), kControl_Picture_P050);
    RegisterControlPosition(StrToWzID("P052"), kControl_Picture_P052);
    RegisterControlPosition(StrToWzID("P053"), kControl_Picture_P053);

    //! 그래이(disable) 판
    RegisterControlPosition(StrToWzID("P016"), kControl_Picture_P016);
    RegisterControlPosition(StrToWzID("P049"), kControl_Picture_P049);
    RegisterControlPosition(StrToWzID("P051"), kControl_Picture_P051);
    RegisterControlPosition(StrToWzID("P054"), kControl_Picture_P054);
    RegisterControlPosition(StrToWzID("P055"), kControl_Picture_P055);

    RegisterControlPosition(StrToWzID("TI07"), kControl_TextImage_TI07);
    RegisterControlPosition(StrToWzID("TI06"), kControl_TextImage_TI06);


}

//------------------------------------------------------------------------------ 
uiAutobotConditionBattle::~uiAutobotConditionBattle( void )
{
    QuickSlotContainer::Release();
}
//------------------------------------------------------------------------------ 
SCSlotContainer* uiAutobotConditionBattle::GetSlotContainer()
{
    return static_cast<SCSlotContainer *>(this);
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionBattle::Init(CDrawBase* drawbase)
{
    m_pItemUnitRenderer = new ItemUnitRenderer;
    if (m_pItemUnitRenderer)
    {
        m_pItemUnitRenderer->Init(g_pSunRenderer, GetMaxSlotNum());
        m_pItemUnitRenderer->ShowCooldown(TRUE);
    }

    //------------------------------------------------------------------------------ 
    GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C005))->SetCheckState(true);
    GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C005))->SetDiscardMessage(true);

    int combo_index = 0;
    std::vector<BASE_AutomaticComboInfo*> combo_vector;

    //! 범위 지정
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_RANGE_USER)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L006, combo_vector);

    //! 회복(hp 물약)
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_CURE_HP)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L002, combo_vector);

    //! 회복(mp 물약)
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_CURE_MPSP)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L005, combo_vector);

    //! 회복(즉시회복)
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_CURE_INSTANT)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L001, combo_vector);

    //! 회복(사료)
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_CURE_PET_FEED)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L004, combo_vector);

    //! 휴식 앉기(hp)
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_CURE_SITDOWN_HP)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L000, combo_vector);

    //! 휴식 앉기(mp)
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_CURE_SITDOWN_MP)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L003, combo_vector);



    // 아 진짜 컨트롤 엑세스하는게 뭐이리 드러워 ㅅㅂ
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E000)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S011)));
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E001)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S013)));
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E003)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S015)));
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E002)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S028)));
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E004)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S029)));
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E005)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S030)));
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E006)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S016)));
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E007)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S017)));
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E008)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S018)));
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E009)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S023)));
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E010)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S024)));
    RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(GetControlID_byPosition(kControl_Edit_E011)),(CCtrlStaticWZ*)GetControlWZ(GetControlID_byPosition(kControl_Text_S027)));

    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E000")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S011")));
    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E001")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S013")));
    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E003")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S015")));
    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E002")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S028")));
    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E004")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S029")));
    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E005")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S030")));
    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E006")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S016")));
    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E007")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S017")));
    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E008")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S018")));
    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E009")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S023")));
    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E010")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S024")));
    //RegisterEditControl( (CCtrlDxEditBox*)GetControlWZ(StrToWzID("E011")),(CCtrlStaticWZ*)GetControlWZ(StrToWzID("S027")));


    for( size_t i=0;i!=12;++i)
    {
        GetEditControlWZ(i)->SetNumberic(true);
        GetEditControlWZ(i)->SetStyle(CTRL_ES_LEFT);
        GetEditControlWZ(i)->SetColorCaret(0xFFFFFFFF);
    }

    InitEditStateViewState();

}
//------------------------------------------------------------------------------ 
void uiAutobotConditionBattle::Release(void)
{
    SAFE_RELEASENDELETE(m_pItemUnitRenderer);
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionBattle::OnUpdateSolarDialog()
{
    if (m_pItemUnitRenderer)
    {
        m_pItemUnitRenderer->Update();
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionBattle::OnRenderSolarDialog()
{
    if (m_pItemUnitRenderer)
    {
        m_pItemUnitRenderer->Render();
    }

    if( m_MouseOverObject.eMouseOverType == eMOUSE_OVER_SLOT )
    {
        
        if (uiToolTipMan* tooltip = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP) )
        {
            tooltip->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
            tooltip->SetTooltipDialogID(GetDialogKey());

        }
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionBattle::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        //! 파티원 리스트 넣기
        GetListControlWZ(GetControlID_byPosition(kControl_List_L007))->DeleteItemAll();
        
        uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
        if ((party_manager != NULL) && (party_manager->GetPartyNum() > 1))
        {
            for (int i = 1; i < party_manager->GetPartyNum(); ++i)
            {
                InsertItemToDropdown(kControl_List_L007, 
                                    (char*)(party_manager->GetParty(i).Get().m_szCharName), NULL);
            }
        }

        InitEditStateViewState();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_Dialog_Autobot;
        msg.wParam = uiAutobotConditionBattle::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_Dialog_Autobot);
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionBattle::NetworkProc(MSG_BASE* msgbase)
{

}
//------------------------------------------------------------------------------
void uiAutobotConditionBattle::MessageProc(SI_MESSAGE* message)
{
    if( ProcessEditControlMessage(message) == true )
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;

    case RT_MSG_MOUSEOVER:
        {
            m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;

            int mouse_x = Mouse::Instance()->GetMouseX();
            int mouse_y = Mouse::Instance()->GetMouseY();

            for( size_t i=kControl_Button_B010;i<=kControl_Button_B033;++i)
            {
                CControlWZ* pControl = GetControlWZ(GetControlID_byPosition(i));
                RECT rect = pControl->GetSizeRect();
                if (rect.left <= mouse_x && rect.right >= mouse_x)
                {
                    if (rect.top <= mouse_y && rect.bottom >= mouse_y)
                    {
                        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
                        m_MouseOverObject.pSlot = &GetSlot(i-kControl_Button_B010);
                        break;
                    }
                }
            }

        }
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}


//------------------------------------------------------------------------------
bool uiAutobotConditionBattle::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    UIControlPos control_pos = static_cast<UIControlPos>(GetControlPosition_byID(message->dwCtrlID));
    switch (control_pos)
    {
    case kControl_CheckBT_C005: 
        {
            //! 전투 설정
        }
        break;
    case kControl_CheckBT_C006:
        {
            //! 지원 설정
            static_cast<uiAutobotMan*>(GetMan())->ChangeTab(uiAutobotConditionSupport::kDialog_ID);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C006))->SetCheckState(false);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_CheckBT_C001: //!< 전체사냥
        {
            if (IsChecked(kControl_CheckBT_C001))
            {
                SetChecked(kControl_CheckBT_C007, false);
                SetChecked(kControl_CheckBT_C010, false);

                SetEnable(kControl_Button_B003, false);
                SetEnable(kControl_Button_B000, false);
            }
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_CheckBT_C007: //!< 파티원
        {
            if (IsChecked(kControl_CheckBT_C007))
            {
                SetChecked(kControl_CheckBT_C001, false);
                SetChecked(kControl_CheckBT_C010, false);

                SetEnable(kControl_Button_B003, true);
                SetEnable(kControl_Button_B000, false);
            }
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_CheckBT_C010: //!< 지정범위
        {
            if (IsChecked(kControl_CheckBT_C010))
            {
                SetChecked(kControl_CheckBT_C001, false);
                SetChecked(kControl_CheckBT_C007, false);

                SetEnable(kControl_Button_B003, false);
                SetEnable(kControl_Button_B000, true);
            }
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;

    case kControl_CheckBT_C009:
        {
            //! 보스발견시 도망
        }
        break;
    case kControl_CheckBT_C011:
        {
            //! 아이템 줍기
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C012))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C013))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C014))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C015))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C016))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C017))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C018))->SetCheckState(false);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_CheckBT_C012:
    case kControl_CheckBT_C013:
    case kControl_CheckBT_C014:
    case kControl_CheckBT_C015:
    case kControl_CheckBT_C016:
    case kControl_CheckBT_C017:
    case kControl_CheckBT_C018:
        {
            //! 아이템 줍기
            if (GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C012))->GetCheckState() || 
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C013))->GetCheckState() || 
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C014))->GetCheckState() || 
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C015))->GetCheckState() || 
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C016))->GetCheckState() || 
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C017))->GetCheckState() || 
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C018))->GetCheckState())
            {
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C011))->SetCheckState(false);
            }
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;

    case kControl_CheckBT_C024:
    case kControl_CheckBT_C025:
    case kControl_CheckBT_C026:
    case kControl_CheckBT_C027:
    case kControl_CheckBT_C030:
    case kControl_CheckBT_C000:
        {
            OnDropdownEnable();
        }
        break;

    case kControl_Button_B999:
        {
            //닫기버튼(x)
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B000:
        {
            //!< 범위
            OnDropdownShow(kControl_List_L006, kControl_Picture_P052);
        }
        break;
    case kControl_Button_B003:
        {
            //!< 파티원
            OnDropdownShow(kControl_List_L007, kControl_Picture_P053);
        }
        break;
    case kControl_Button_B004:
        {
            //!< hp 회복 물약 먹기
            OnDropdownShow(kControl_List_L002, kControl_Picture_P045);
        }
        break;
    case kControl_Button_B005:
        {
            //!< mp 회복 물약 먹기
            OnDropdownShow(kControl_List_L005, kControl_Picture_P048);
        }
        break;
    case kControl_Button_B006:
        {
            //!< 즉시회복
            OnDropdownShow(kControl_List_L001, kControl_Picture_P044);
        }
        break;
    case kControl_Button_B007:
        {
            //!< 사료
            OnDropdownShow(kControl_List_L004, kControl_Picture_P047);
        }
        break;
    case kControl_Button_B008:
        {
            //!< 앉기 휴식 hp
            OnDropdownShow(kControl_List_L000, kControl_Picture_PI04);
        }
        break;
    case kControl_Button_B009:
        {
            //! 앉기 휴식 mp
            OnDropdownShow(kControl_List_L003, kControl_Picture_P046);
        }
        break;
    case kControl_Button_B010:  //! 슬롯 아이템들
    case kControl_Button_B011:
    case kControl_Button_B013:
    case kControl_Button_B012:
    case kControl_Button_B015:
    case kControl_Button_B014:
    case kControl_Button_B016:
    case kControl_Button_B017:
    case kControl_Button_B018:
    case kControl_Button_B019:
    case kControl_Button_B020:
    case kControl_Button_B021:
    case kControl_Button_B022:
    case kControl_Button_B023:
    case kControl_Button_B024:
    case kControl_Button_B025:
    case kControl_Button_B026:
    case kControl_Button_B027:
    case kControl_Button_B028:
    case kControl_Button_B029:
    case kControl_Button_B030:
    case kControl_Button_B031:
    case kControl_Button_B032:
    case kControl_Button_B033:
        {
            MouseHandler::Instance()->ItemTransaction(SI_AUTOMATIC_INFO, 
                static_cast<POSTYPE>((GetControlPosition_byID(message->dwCtrlID) - kControl_Button_B010)), 
                &(GetControlWZ(message->dwCtrlID)->GetSizeRect()));
        }
        break;
    case kControl_List_L000:
        {
            ChangeDropdownItem(control_pos, kControl_Text_S050);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L001:
        {
            ChangeDropdownItem(control_pos, kControl_Text_S048);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L002:
        {
            ChangeDropdownItem(control_pos, kControl_Text_S035);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L003:
        {
            ChangeDropdownItem(control_pos, kControl_Text_S051);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L004:
        {
            ChangeDropdownItem(control_pos, kControl_Text_S049);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L005:
        {
            ChangeDropdownItem(control_pos, kControl_Text_S047);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L006:
        {
            ChangeDropdownItem(control_pos, kControl_Text_S034);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L007:
        {
            ChangeDropdownItem(control_pos, kControl_Text_S033);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_TextImage_TI07:
        {
            //! 확인
            static_cast<uiAutobotMan*>(GetMan())->UpdateCondition();
            ShowInterface(FALSE);
        }
        break;
    case kControl_TextImage_TI06:
        {
            //! 초기화
            DefaultUI();
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionBattle::OnDropdownShow(UIControlPos list_index, UIControlPos picture_index)
{
    //! 리스트 박스 배경은 다 히든 시킨다.
    GetControlWZ(GetControlID_byPosition(kControl_Picture_PI04))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P044))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P045))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P046))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P047))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P048))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P052))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P053))->ShowWindowWZ(WZ_HIDE); 

    for (int i = (int)kControl_List_L000; i <= (int)kControl_List_L007; ++i)
    {
        if ((static_cast<UIControlPos>(i) == list_index) && 
            (GetListControlWZ(GetControlID_byPosition(list_index))->GetState() == WZ_HIDE))
        {
            GetListControlWZ(GetControlID_byPosition(list_index))->ShowWindowWZ(WZ_SHOW);
            GetControlWZ(GetControlID_byPosition(picture_index))->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
            GetListControlWZ(GetControlID_byPosition(static_cast<UIControlPos>(i)))->ShowWindowWZ(WZ_HIDE);
        }
    }
}

//------------------------------------------------------------------------------ 
int uiAutobotConditionBattle::GetCoolTime(SCSlot* slot)
{
    if (slot != NULL)
    {
        if (slot->GetSlotType() == ST_ITEM)
        {
            SCItemSlot* item_slot = static_cast<SCItemSlot*>(slot);
            BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_slot->GetCode());
            if (item_info != NULL)
            {
                return item_info->m_dCoolTime / 1000;
            }
        }
        else if (slot->GetSlotType() == ST_SKILL)
        {
            SCSkillSlot* skill_slot = static_cast<SCSkillSlot*>(slot);
            SkillScriptInfo* skill_info = SkillInfoParser::Instance()->GetSkillInfo(skill_slot->GetCode());

            if (skill_info != NULL)
            {
                return skill_info->m_dwCoolTime / 1000;
            }
        }
    }
    return kDefaultCooltimeSecond;//!< 최소 10초
}
//------------------------------------------------------------------------------ 
RC::eSLOT_INSERT_RESULT uiAutobotConditionBattle::InsertSlot(POSTYPE at_pos, SCSlot& slot)
{
    //slot_safe_code:안전한 슬롯체크 구문(삽입)
    if (ValidPos(at_pos) == false)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

    if (m_pItemUnitRenderer == NULL)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

    CControlWZ* button_control = GetControlWZ(GetControlID_byPosition(kControl_Button_B010 + at_pos));
    if(!button_control)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

    DBSERIAL slot_serial = g_SlotKeyGenerator.GetKey();
    if (slot_serial <= 0)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

    RC::eSLOT_INSERT_RESULT slot_result = QuickSlotContainer::InsertSlot(at_pos, slot);

    QuickSlot& quick_slot = static_cast<QuickSlot&>(GetSlot(at_pos));

    quick_slot.SetOrgSerial(slot_serial);

    if(m_pItemUnitRenderer)
    {
        // 3D 모델링 Render
        m_pItemUnitRenderer->AddItemUnit(quick_slot.GetSerial(), 
                                    quick_slot.GetOrgCode(), 
                                    &(button_control->GetSizeRect()), 
                                    &quick_slot,
                                    button_control);
    }

    if (slot_result == RC::RC_INSERT_SLOT_SUCCESS)
    {
        //! 시간을 쿨타임보다 크게 한다.

        int slot_cool_time = GetCoolTime(quick_slot.GetOrgSlot());

        TCHAR str[INTERFACE_STRING_LENGTH] = {NULL, };
        GetEditText(at_pos, str, INTERFACE_STRING_LENGTH);

        int edit_cool_time = atoi(str);

        if (slot_cool_time > edit_cool_time)
        {
            SetEditText(at_pos, slot_cool_time);
        }
    }

    return slot_result;
}
//------------------------------------------------------------------------------ 
BOOL uiAutobotConditionBattle::DeleteSlot(POSTYPE at_pos, SCSlot* slotOut)
{
    //slot_safe_code:안전한 슬롯체크 구문(삭제)
    if ((ValidPos(at_pos) == false) || IsEmpty(at_pos))
    {
        return FALSE;
    }

    if (m_pItemUnitRenderer == NULL)
    {
        return FALSE;
    }

    QuickSlot& quick_slot = (QuickSlot&)GetSlot(at_pos);

    if (quick_slot.GetOrgSerial() > 0)
    {
        g_SlotKeyGenerator.Restore((DWORD)quick_slot.GetOrgSerial());

        m_pItemUnitRenderer->RemoveItemUnit(quick_slot.GetOrgSerial());
    }

    return QuickSlotContainer::DeleteSlot(at_pos, slotOut);
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionBattle::DeleteAllSlots(void)
{
    int numSlots = GetMaxSlotNum();

    for (int i = 0; i < numSlots; ++i)
    {
        if (IsEmpty(i) == FALSE)
        {
            DeleteSlot(i, NULL);
        }
    }
}
//------------------------------------------------------------------------------ 
bool uiAutobotConditionBattle::IsAttackSkillSlot(POSTYPE at_pos)
{
    return (at_pos >= (uiAutobotConditionBattle::kSlotSize / 2));
}
//------------------------------------------------------------------------------ 
bool uiAutobotConditionBattle::CanLinkSlot(const SCSlot* slot, POSTYPE to_pos)
{
    if (slot == NULL)
    {
        return false;
    }

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {NULL, };

    if (ST_QUICK == slot->GetSlotType())
    {
        eSlotType org_slot_type = ((QuickSlot*)(slot))->GetOrgSlotType();

        if (org_slot_type == ST_ITEM)
        {
            if (IsAttackSkillSlot(to_pos) == true)
            {
                return false;
            }
        }
        else if (org_slot_type == ST_SKILL)
        {
            SLOTCODE org_slot_code = ((QuickSlot*)(slot))->GetOrgCode();

            SkillType skill_type = GetSkilType(org_slot_code);
            if (IsAttackSkillSlot(to_pos) == true)
            {
                if (skill_type != kSkill_Attack)
                {
                    //! 공격 스킬만 올려 놓을수 있습니다.
                    return false;
                }
            }
            else
            {
                if (skill_type != kSkill_Defense)
                {
                    //! 공격 스킬만 올려 놓을수 있습니다.
                    return false;
                }
            }
        }

        return true;
    }
    else if (ST_ITEM == slot->GetSlotType())
    {
        if (IsAttackSkillSlot(to_pos) == true)
        {
            //! 공격 슬롯에는 아이템을 올려 놓을수 없습니다.
            return false;
        }

        bool is_possible = false;
        const BASE_ITEMINFO* item_info = ((SCItemSlot*)slot)->GetItemInfo();
        if (item_info != NULL)
        {
            if (item_info->m_wType == eITEMTYPE_WASTE)
            {
                if ((item_info->m_byWasteType == eITEMWASTE_HPPOTION) || 
                    (item_info->m_byWasteType == eITEMWASTE_MPPOTION) || 
                    (item_info->m_byWasteType == eITEMWASTE_FUNCTION) || 
                    (item_info->m_byWasteType == eITEMWASTE_FUNCTION_HOLDUP_BUFF_POST_DEAD))
                {
                    is_possible = true;
                }
            }
        }

        //if (item_info == NULL || item_info->m_byMaterialType != eITEM_MATERIAL_TYPE_CAN_USE)
        if (is_possible == false)
        {
            //// 장착 및 사용 가능한 아이템만 퀵 창에 올릴 수 있다.
            //g_InterfaceManager.GetInterfaceString( eST_ITEM_MATERIAL_TYPE_CAN_EQUIP, szMessage, INTERFACE_STRING_LENGTH );
            //GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
            return false;
        }

        return true;
    }
    else if (ST_SKILL == slot->GetSlotType())
    {
        SkillType skill_type = GetSkilType(slot->GetCode());
        if (IsAttackSkillSlot(to_pos) == true)
        {
            if (skill_type != kSkill_Attack)
            {
                //! 공격 스킬만 올려 놓을수 있습니다.
                return false;
            }
        }
        else
        {
            if (skill_type != kSkill_Defense)
            {
                //! 방어 스킬만 올려 놓을수 있습니다.
                return false;
            }
        }

        return true;
    }

    return false;
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionBattle::DefaultUI()
{
    //! 아이템 종류별로 옵션 활성 여부 판대기
    SetVisible(kControl_Picture_P016, WZ_HIDE); 
    SetVisible(kControl_Picture_P049, WZ_HIDE); 
    SetVisible(kControl_Picture_P051, WZ_HIDE); 
    SetVisible(kControl_Picture_P054, WZ_HIDE); 
    SetVisible(kControl_Picture_P055, WZ_HIDE); 

    //! 기능 별로 check button control 활성화 여부
    SLOTCODE item_code = 0;
    g_pHero->GetHeroEquipContainer()->IsEquipItem(eITEMTYPE_AUTO_HUNTING, &item_code);

    //------------------------------------------------------------------------------ 
    SetEnableByItem(kControl_CheckBT_C001, true, item_code, kControl_Text_T006);  // 사냥설정_맵전체
    SetEnableByItem(kControl_CheckBT_C009, true, item_code, kControl_Text_S000);  // 사냥설정_보스경고
    SetEnableByItem(kControl_CheckBT_C007, true, item_code, kControl_Text_S001);  // 사냥설정_파티원주변
    SetEnableByItem(kControl_CheckBT_C010, true, item_code, kControl_Text_S002);  // 사냥설정_특정범위(자신 주변)
    SetEnableByItem(kControl_CheckBT_C011, true, item_code, kControl_Text_S003);  // 줍기설정_모든아이템
    SetEnableByItem(kControl_CheckBT_C012, true, item_code, kControl_Text_S004);  // 줍기설정_일반아이템
    SetEnableByItem(kControl_CheckBT_C017, true, item_code, kControl_Text_S009);  // 줍기설정_퀘스트아이템
    SetEnableByItem(kControl_CheckBT_C013, true, item_code, kControl_Text_S005);  // 줍기설정_스톤아이템
    SetEnableByItem(kControl_CheckBT_C014, true, item_code, kControl_Text_S006);  // 줍기설정_무기아이템
    SetEnableByItem(kControl_CheckBT_C018, true, item_code, kControl_Text_S010);  // 줍기설정_방어구아이템
    SetEnableByItem(kControl_CheckBT_C015, true, item_code, kControl_Text_S007);  // 줍기설정_AC아이템
    SetEnableByItem(kControl_CheckBT_C016, true, item_code, kControl_Text_S008);  // 줍기설정_하임
    SetEnableByItem(kControl_CheckBT_C024, true, item_code, kControl_Text_S019);  // 회복설정_HP
    SetEnableByItem(kControl_CheckBT_C026, true, item_code, kControl_Text_S021);  // 회복설정_MPSP
    SetEnableByItem(kControl_CheckBT_C025, true, item_code, kControl_Text_S020);  // 회복설정_즉시회복
    SetEnableByItem(kControl_CheckBT_C027, true, item_code, kControl_Text_S022);  // 회복설정_사료(동물 먹이)
    SetEnableByItem(kControl_CheckBT_C030, true, item_code, kControl_Text_S025);  // 회복설정_휴식(앉기, 해당 HP 이하일경우)
    SetEnableByItem(kControl_CheckBT_C000, true, item_code, kControl_Text_S014);  // 회복설정_휴식(앉기, 해당 MP 이하일경우)

    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if ((party_manager != NULL) && (party_manager->IsParty()))
    {
        SetEnableByItem(kControl_CheckBT_C007, true, item_code, kControl_Text_S001);              // 사냥설정_파티원주변
    }
    else
    {
        SetEnableByItem(kControl_CheckBT_C007, false, item_code, kControl_Text_S001);              // 사냥설정_파티원주변
    }
    if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_FIELD) ||
        (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHUNTING) ||
        (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_MISSION))
    {
        SetEnableByItem(kControl_CheckBT_C010, true, item_code, kControl_Text_S002);
    }
    else
    {
        SetEnableByItem(kControl_CheckBT_C010, false, item_code, kControl_Text_S002);
    }
    //------------------------------------------------------------------------------ 

    //! 에디트박스에 디폴트값으로 설정
    for( size_t i=0;i!=GetEditCount();++i)
    {
        SetEditText(i, kDefaultCooltimeSecond);
    }


    //! 콤보박스 첫번째것으로 셋팅
    SelectDropdownItem(kControl_List_L000, kControl_Text_S050, 0);
    SelectDropdownItem(kControl_List_L001, kControl_Text_S048, 0);
    SelectDropdownItem(kControl_List_L002, kControl_Text_S035, 0);
    SelectDropdownItem(kControl_List_L003, kControl_Text_S051, 0);
    SelectDropdownItem(kControl_List_L004, kControl_Text_S049, 0);
    SelectDropdownItem(kControl_List_L005, kControl_Text_S047, 0);
    SelectDropdownItem(kControl_List_L006, kControl_Text_S034, 0);
    SelectDropdownItem(kControl_List_L007, kControl_Text_S033, 0);

    //사냥 설정
    SetCheckedByItem(kControl_CheckBT_C001, false, item_code);    // 사냥설정_맵전체
    SetCheckedByItem(kControl_CheckBT_C009, false, item_code);    // 사냥설정_보스경고
    SetCheckedByItem(kControl_CheckBT_C007, false, item_code);    // 사냥설정_파티원주변
    SetCheckedByItem(kControl_CheckBT_C010, false, item_code);    // 사냥설정_특정범위(자신 주변)

    //! 줍기 옵션
    SetCheckedByItem(kControl_CheckBT_C011, false, item_code); // 줍기설정_모든아이템
    SetCheckedByItem(kControl_CheckBT_C012, false, item_code); // 줍기설정_일반아이템
    SetCheckedByItem(kControl_CheckBT_C017, false, item_code); // 줍기설정_퀘스트아이템
    SetCheckedByItem(kControl_CheckBT_C013, false, item_code); // 줍기설정_스톤아이템
    SetCheckedByItem(kControl_CheckBT_C014, false, item_code); // 줍기설정_무기아이템
    SetCheckedByItem(kControl_CheckBT_C018, false, item_code); // 줍기설정_방어구아이템
    SetCheckedByItem(kControl_CheckBT_C015, false, item_code); // 줍기설정_AC아이템
    SetCheckedByItem(kControl_CheckBT_C016, false, item_code); // 줍기설정_하임

    //! 회복 설정
    SetCheckedByItem(kControl_CheckBT_C024, false, item_code);    // 회복설정_HP
    SetCheckedByItem(kControl_CheckBT_C026, false, item_code);    // 회복설정_MPSP
    SetCheckedByItem(kControl_CheckBT_C025, false, item_code);    // 회복설정_즉시회복
    SetCheckedByItem(kControl_CheckBT_C027, false, item_code);    // 회복설정_사료(동물 먹이)
    SetCheckedByItem(kControl_CheckBT_C030, false, item_code);    // 회복설정_휴식(앉기, 해당 HP 이하일경우)
    SetCheckedByItem(kControl_CheckBT_C000, false, item_code);    // 회복설정_휴식(앉기, 해당 MP 이하일경우)

    OnDropdownEnable();

    //! 슬롯컨테이너
    DeleteAllSlots();

    GetDialogWZ()->RequestFocus(NULL);

}
//------------------------------------------------------------------------------ 
bool uiAutobotConditionBattle::UpdateUI()
{
    if (AutobotConditionManager::Instance()->IsLoaded() == false)
    {
        return false;
    }
    
    int option_value = 0;

    //------------------------------------------------------------------------------ 
    //사냥 설정
    SetChecked(kControl_CheckBT_C009, AutobotConditionManager::Instance()->hunting().IsAvoidMonster(eNPC_BOSS));
    SetChecked(kControl_CheckBT_C001, AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Free);
    SetChecked(kControl_CheckBT_C007, AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Party);
    SetChecked(kControl_CheckBT_C010, AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Area);

    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if ((party_manager == NULL) || (party_manager->IsParty() == false))
    {
        SetEnable(kControl_CheckBT_C007, false);
    }
    if (IsChecked(kControl_CheckBT_C007))
    {
        option_value = AutobotConditionManager::Instance()->hunting().party_follow_no();
        SelectDropdownItem(kControl_List_L007, kControl_Text_S033, option_value);
    }
    if (IsChecked(kControl_CheckBT_C010))
    {
        option_value = static_cast<int>(AutobotConditionManager::Instance()->hunting().area_range());
        SelectDropdownItemByComboValue(kControl_List_L006, kControl_Text_S034, option_value);
    }

    //------------------------------------------------------------------------------ 
    //! 아이템 줍기
    SetChecked(kControl_CheckBT_C011, AutobotConditionManager::Instance()->pickup().HasItemType(AutobotCondition::kItem_All));
    SetChecked(kControl_CheckBT_C012, AutobotConditionManager::Instance()->pickup().HasItemType(AutobotCondition::kItem_Normal));
    SetChecked(kControl_CheckBT_C013, AutobotConditionManager::Instance()->pickup().HasItemType(AutobotCondition::kItem_Stone));
    SetChecked(kControl_CheckBT_C014, AutobotConditionManager::Instance()->pickup().HasItemType(AutobotCondition::kItem_Weapon));
    SetChecked(kControl_CheckBT_C015, AutobotConditionManager::Instance()->pickup().HasItemType(AutobotCondition::kItem_AC));
    SetChecked(kControl_CheckBT_C016, AutobotConditionManager::Instance()->pickup().HasItemType(AutobotCondition::kItem_Heim));
    SetChecked(kControl_CheckBT_C017, AutobotConditionManager::Instance()->pickup().HasItemType(AutobotCondition::kItem_Quest));
    SetChecked(kControl_CheckBT_C018, AutobotConditionManager::Instance()->pickup().HasItemType(AutobotCondition::kItem_Armor));

    //------------------------------------------------------------------------------ 
    //! 회복 설정
    SetChecked(kControl_CheckBT_C024, AutobotConditionManager::Instance()->recovery().hp_potion_percent() > 0.0f);
    SetChecked(kControl_CheckBT_C026, AutobotConditionManager::Instance()->recovery().mp_potion_percent() > 0.0f);
    SetChecked(kControl_CheckBT_C025, AutobotConditionManager::Instance()->recovery().immediate_potion_percent() > 0.0f);
    SetChecked(kControl_CheckBT_C027, AutobotConditionManager::Instance()->recovery().feed_potion_percent() > 0.0f);
    SetChecked(kControl_CheckBT_C030, AutobotConditionManager::Instance()->recovery().relax_hp_percent() > 0.0f);
    SetChecked(kControl_CheckBT_C000, AutobotConditionManager::Instance()->recovery().relax_mp_percent() > 0.0f);

    if (IsChecked(kControl_CheckBT_C024))
    {
        option_value = static_cast<int>(AutobotConditionManager::Instance()->recovery().hp_potion_percent());
        SelectDropdownItemByComboValue(kControl_List_L002, kControl_Text_S035, option_value);
    }
    if (IsChecked(kControl_CheckBT_C026))
    {
        option_value = static_cast<int>(AutobotConditionManager::Instance()->recovery().mp_potion_percent());
        SelectDropdownItemByComboValue(kControl_List_L005, kControl_Text_S047, option_value);
    }
    if (IsChecked(kControl_CheckBT_C025))
    {
        option_value = static_cast<int>(AutobotConditionManager::Instance()->recovery().immediate_potion_percent());
        SelectDropdownItemByComboValue(kControl_List_L001, kControl_Text_S048, option_value);
    }
    if (IsChecked(kControl_CheckBT_C027))
    {
        option_value = static_cast<int>(AutobotConditionManager::Instance()->recovery().feed_potion_percent());
        SelectDropdownItemByComboValue(kControl_List_L004, kControl_Text_S049, option_value);
    }
    if (IsChecked(kControl_CheckBT_C030))
    {
        option_value = static_cast<int>(AutobotConditionManager::Instance()->recovery().relax_hp_percent());
        SelectDropdownItemByComboValue(kControl_List_L000, kControl_Text_S050, option_value);
    }
    if (IsChecked(kControl_CheckBT_C000))
    {
        option_value = static_cast<int>(AutobotConditionManager::Instance()->recovery().relax_mp_percent());
        SelectDropdownItemByComboValue(kControl_List_L003, kControl_Text_S051, option_value);
    }

    //------------------------------------------------------------------------------ 
    //! 스킬 등록
    int to_pos = 0;
    {
        AutobotCondition_Skill::ConditionIntervalSkillVector::iterator begin = AutobotConditionManager::Instance()->skill().interval_skill_vector().begin();
        AutobotCondition_Skill::ConditionIntervalSkillVector::iterator end = AutobotConditionManager::Instance()->skill().interval_skill_vector().end();
        for (; begin != end && to_pos < 12; ++begin, ++to_pos)
        {
            AutobotCondition_Skill::Slot slot = *begin;
            
            if (slot.slot_type == ST_SKILL)
            {
                ItemManager::Instance()->AutobotLinkSkill(to_pos, slot.slot_code);
            }
            else if (slot.slot_type == ST_ITEM)
            {
                SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(slot.container_index, slot.slot_code);

                if (item_slot != NULL)
                {
                    ItemManager::Instance()->AutobotLinkItem(slot.container_index,
                                                            item_slot->GetPos(), 
                                                            to_pos, 
                                                            item_slot->GetCode());
                }
            }

            int second_time = slot.interval_millisecond / 1000;
            if (second_time < 1)
            {
                second_time = 1;
            }

            SetEditText(to_pos, second_time);
        }
    }
    {
        to_pos = 12; //!< 공격용 스킬을 아래부터
        AutobotCondition_Skill::ConditionAttackSkillVector::iterator begin = AutobotConditionManager::Instance()->skill().attack_skill_vector().begin();
        AutobotCondition_Skill::ConditionAttackSkillVector::iterator end = AutobotConditionManager::Instance()->skill().attack_skill_vector().end();
        for (; begin != end && to_pos < 24; ++begin, ++to_pos)
        {
            AutobotCondition_Skill::Slot slot = *begin;

            if (slot.slot_type == ST_SKILL)
            {
                ItemManager::Instance()->AutobotLinkSkill(to_pos, slot.slot_code);
            }
            else if (slot.slot_type == ST_ITEM)
            {
                SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(slot.container_index, slot.slot_code);

                if (item_slot != NULL)
                {
                    ItemManager::Instance()->AutobotLinkItem(slot.container_index,
                                                            item_slot->GetPos(), 
                                                            to_pos, 
                                                            item_slot->GetCode());
                }
            }
        }
    }

    OnDropdownEnable();

    return true;
}
//------------------------------------------------------------------------------ 
bool uiAutobotConditionBattle::UpdateCondition()
{
    //------------------------------------------------------------------------------ 
    //! 사냥 설정
    //AutobotConditionManager::Instance()->hunting().set_target_range(15.0f);

    if (IsChecked(kControl_CheckBT_C001) == true)
    {
        AutobotConditionManager::Instance()->hunting().set_hunting_type(AutobotCondition_Hunting::kHuntingType_Free);
    }
    else if (IsChecked(kControl_CheckBT_C007) == true)
    {
        int party_index = -1;
        uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
        if (party_manager != NULL)
        {
            const char* party_name = GetCurDropdownItemText(kControl_List_L007);
            if (party_name != NULL)
            {
                party_index = party_manager->GetPartyIndex((TCHAR*)party_name);
            }
        }

        AutobotConditionManager::Instance()->hunting().set_hunting_type(AutobotCondition_Hunting::kHuntingType_Party);
        AutobotConditionManager::Instance()->hunting().set_party_follow_no(party_index);
    }
    else if (IsChecked(kControl_CheckBT_C010) == true)
    {
        BASE_AutomaticComboInfo* info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L006));

        if (info && g_pHero)
        {
            AutobotConditionManager::Instance()->hunting().set_hunting_type(AutobotCondition_Hunting::kHuntingType_Area);
            AutobotConditionManager::Instance()->hunting().set_area_position(g_pHero->GetPosition());
            AutobotConditionManager::Instance()->hunting().set_area_range(static_cast<float>(info->option_value));
        }        
    }
    else
    {
        AutobotConditionManager::Instance()->hunting().set_hunting_type(AutobotCondition_Hunting::kHuntingType_Max);
    }

    AutobotConditionManager::Instance()->hunting().avoid_monster_set().insert(eNPC_LUCKY_MONSTER);
    if (IsChecked(kControl_CheckBT_C009) == true)
    {
        AutobotConditionManager::Instance()->hunting().avoid_monster_set().insert(eNPC_BOSS);
    }

    //------------------------------------------------------------------------------ 
    //! 아이템 줍기
    if (IsChecked(kControl_CheckBT_C011))
    {
        AutobotConditionManager::Instance()->pickup().Insert(AutobotCondition::kItem_All);
    }
    else
    {
        if (IsChecked(kControl_CheckBT_C012))
        {
            AutobotConditionManager::Instance()->pickup().Insert(AutobotCondition::kItem_Normal);
        }
        if (IsChecked(kControl_CheckBT_C013))
        {
            AutobotConditionManager::Instance()->pickup().Insert(AutobotCondition::kItem_Stone);
        }
        if (IsChecked(kControl_CheckBT_C014))
        {
            AutobotConditionManager::Instance()->pickup().Insert(AutobotCondition::kItem_Weapon);
        }
        if (IsChecked(kControl_CheckBT_C015))
        {
            AutobotConditionManager::Instance()->pickup().Insert(AutobotCondition::kItem_AC);
        }
        if (IsChecked(kControl_CheckBT_C016))
        {
            AutobotConditionManager::Instance()->pickup().Insert(AutobotCondition::kItem_Heim);
        }
        if (IsChecked(kControl_CheckBT_C017))
        {
            AutobotConditionManager::Instance()->pickup().Insert(AutobotCondition::kItem_Quest);
        }
        if (IsChecked(kControl_CheckBT_C018))
        {
            AutobotConditionManager::Instance()->pickup().Insert(AutobotCondition::kItem_Armor);
        }
    }

    //------------------------------------------------------------------------------ 
    //! 회복 설정
    BASE_AutomaticComboInfo* info = NULL;
    if (IsChecked(kControl_CheckBT_C024))
    {
        //! 회복 설정 : HP
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L002));
        AutobotConditionManager::Instance()->recovery().set_hp_potion_percent(static_cast<float>(info->option_value));
    }
    if (IsChecked(kControl_CheckBT_C026))
    {
        //! 회복 설정 : MP
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L005));
        AutobotConditionManager::Instance()->recovery().set_mp_potion_percent(static_cast<float>(info->option_value));
    }
    if (IsChecked(kControl_CheckBT_C025))
    {
        //! 회복 설정 : 즉시
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L001));
        AutobotConditionManager::Instance()->recovery().set_immediate_potion_percent(static_cast<float>(info->option_value));
    }        
    if (IsChecked(kControl_CheckBT_C027))
    {
        //! 회복 설정 : 사료
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L004));
        AutobotConditionManager::Instance()->recovery().set_feed_potion_percent(static_cast<float>(info->option_value));
    }        
    if (IsChecked(kControl_CheckBT_C030))
    {
        //! 회복 설정 : 앉기 mp
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L000));
        AutobotConditionManager::Instance()->recovery().set_relax_hp_percent(static_cast<float>(info->option_value));
    }  
    if (IsChecked(kControl_CheckBT_C000))
    {
        //! 회복 설정 : 앉기 mp
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L003));
        AutobotConditionManager::Instance()->recovery().set_relax_mp_percent(static_cast<float>(info->option_value));
    }  

    //! 스킬 등록
    int max_slot = GetMaxSlotNum();
    for (int i = 0; i < max_slot; ++i)
    {
        QuickSlot& quick_slot = static_cast<QuickSlot&>(GetSlot(i));
        if (quick_slot.IsEmptySlot() == false)
        {
            if (IsAttackSkillSlot(i) == true)
            {
                AutobotConditionManager::Instance()->skill().InsertAttackSkill(static_cast<eSlotContainerIndex>(quick_slot.GetOrgSlotIndex()),
                    quick_slot.GetOrgSlotType(),
                    quick_slot.GetOrgCode());
            }
            else
            {
                std::string text = GetEditText(i);
                int second = atoi(text.c_str());
                if( second < kDefaultCooltimeSecond )
                {
                    second = kDefaultCooltimeSecond;
                }
                
                AutobotConditionManager::Instance()->skill().InsertIntervalSkill(static_cast<eSlotContainerIndex>(quick_slot.GetOrgSlotIndex()),
                    quick_slot.GetOrgSlotType(),
                    quick_slot.GetOrgCode(),
                    second * 1000);
            }
        }
    }


    return true;
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionBattle::OnDropdownEnable()
{
    SetEnable(kControl_Button_B004, GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C024))->GetCheckState());
    SetEnable(kControl_Button_B006, GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C025))->GetCheckState());
    SetEnable(kControl_Button_B005, GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C026))->GetCheckState());
    SetEnable(kControl_Button_B007, GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C027))->GetCheckState());
    SetEnable(kControl_Button_B008, GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C030))->GetCheckState());
    SetEnable(kControl_Button_B009, GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C000))->GetCheckState());
    SetEnable(kControl_Button_B003, GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C007))->GetCheckState());
    SetEnable(kControl_Button_B000, GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C010))->GetCheckState());

    OnDropdownShow(kControl_Size, kControl_Size);
}
//------------------------------------------------------------------------------ 
eAUTO_HUNTING_TYPE uiAutobotConditionBattle::GetHuntingTypeFromControlIndex(DWORD control_index)
{
    switch (control_index)
    {
        case kControl_CheckBT_C001:     return eAUTO_RANGE_FIELD;      
        case kControl_CheckBT_C009:     return eAUTO_RANGE_BOSS;       
        case kControl_CheckBT_C007:     return eAUTO_RANGE_PARTY;      
        case kControl_CheckBT_C010:     return eAUTO_RANGE_USER;       
        case kControl_CheckBT_C011:     return eAUTO_PICKUP_ALL;       
        case kControl_CheckBT_C012:     return eAUTO_PICKUP_NORMAL;    
        case kControl_CheckBT_C017:     return eAUTO_PICKUP_QUEST;     
        case kControl_CheckBT_C013:     return eAUTO_PICKUP_STONE;     
        case kControl_CheckBT_C014:     return eAUTO_PICKUP_WEAPON;    
        case kControl_CheckBT_C018:     return eAUTO_PICKUP_ARMOR;     
        case kControl_CheckBT_C015:     return eAUTO_PICKUP_AC;        
        case kControl_CheckBT_C016:     return eAUTO_PICKUP_HEIM;      
        //case kControl_CheckBT_C001:     return eAUTO_PICKUP_ITEMTYPE;  
        case kControl_CheckBT_C024:     return eAUTO_CURE_HP;          
        case kControl_CheckBT_C026:     return eAUTO_CURE_MPSP;        
        case kControl_CheckBT_C025:     return eAUTO_CURE_INSTANT;     
        case kControl_CheckBT_C027:     return eAUTO_CURE_PET_FEED;    
        case kControl_CheckBT_C030:     return eAUTO_CURE_SITDOWN_HP;  
        case kControl_CheckBT_C000:     return eAUTO_CURE_SITDOWN_MP;  
    }

    return eAUTO_TYPE_INVALID;
}
//------------------------------------------------------------------------------ 
uiAutobotConditionBattle::SkillType uiAutobotConditionBattle::GetSkilType(SLOTCODE skill_code)
{
    if (skill_code == eACTION_START_SKILL_CODE)
    {
        //! 자동공격
        return uiAutobotConditionBattle::kSkill_Attack;
    }

    ROOT_SKILLINFO* root_info = SkillInfoParser::Instance()->GetInfo(skill_code);
    if (root_info == NULL || root_info->IsSkill() == false)
    {
        return uiAutobotConditionBattle::kSkill_Max;
   }

    SkillScriptInfo* skill_info = static_cast<SkillScriptInfo*>(root_info);
    if (skill_info == NULL)
    {
        return uiAutobotConditionBattle::kSkill_Max;
    }

    //409	스타일
    //11236	근거리물리
    //11237	원거리물리
    //11238	보조기능
    //11239	강화
    //11240	약화
    //11241	회복
    //11242	변신
    //11243	소환
    //11244	부활
    //11245	마법
    switch (skill_info->m_wSkillAttribute)
    {
    case 409:   //!<스타일
        break;
    case 11238: //!<보조기능
    case 11239: //!<강화
    case 11241: //!<회복
    case 11242: //!<변신
    case 11243: //!<소환
    case 11244: //!<부활
        {
            BASE_ABILITYINFO* ability_info = NULL;
            skill_info->SetFirst();
            while (ability_info = skill_info->GetNext())
            {
                if ((ability_info->m_byRangeType == SKILL_ABILITY_FRIEND) || 
                    (ability_info->m_byRangeType == SKILL_ABILITY_TARGETAREA_FRIEND) || 
                    (ability_info->m_byRangeType == SKILL_ABILITY_MYAREA_FRIEND) || 
                    (ability_info->m_byRangeType == SKILL_ABILITY_ME) || 
                    (ability_info->m_byRangeType == SKILL_ABILITY_SUMMON))

                {
                    return uiAutobotConditionBattle::kSkill_Defense;
                }
            }
            return uiAutobotConditionBattle::kSkill_Max;

            //if ((skill_info->m_byTarget == SKILL_TARGET_FRIEND) || 
            //    (skill_info->m_byTarget == SKILL_TARGET_ME) ||
            //    (skill_info->m_byTarget == SKILL_TARGET_REACHABLE_FRIEND) || 
            //    (skill_info->m_byTarget == SKILL_TARGET_REACHABLE_ME))
            //{
            //    return uiAutobotConditionBattle::kSkill_Defense;
            //}
        }
        break;
    case 11240: //!< 약화
    case 11236: //!< 근거리물리
    case 11237: //!< 원거리물리
    case 11245: //!< 마법
        {
            BASE_ABILITYINFO* ability_info = NULL;
            skill_info->SetFirst();
            while (ability_info = skill_info->GetNext())
            {
                if ((ability_info->m_byRangeType == SKILL_ABILITY_ENEMY) ||
                    (ability_info->m_byRangeType == SKILL_ABILITY_TARGETAREA_ENEMY) ||
                    (ability_info->m_byRangeType == SKILL_ABILITY_MYAREA_ENEMY) ||
                    (ability_info->m_byRangeType == SKILL_ABILITY_PLAYER_ENEMY) || 
                    (ability_info->m_byRangeType == SKILL_ABILITY_MYAREA_CORPSE_ENEMY) ||
                    (ability_info->m_byRangeType == SKILL_ABILITY_CORPSE_ENEMY))
                {
                    return uiAutobotConditionBattle::kSkill_Attack;
                }
            }
            return uiAutobotConditionBattle::kSkill_Max;

            //if ((skill_info->m_byTarget == SKILL_TARGET_ENEMY) || 
            //    (skill_info->m_byTarget == SKILL_TARGET_AREA) ||
            //    (skill_info->m_byTarget == SKILL_TARGET_REACHABLE_ENEMY) || 
            //    (skill_info->m_byTarget == SKILL_TARGET_ENEMY_PLAYER) ||
            //    (skill_info->m_byTarget == SKILL_TARGET_ENEMY_AND_ME))
            //{
            //    // 어빌리티 체크
            //    BASE_ABILITYINFO* ability_info = NULL;
            //    skill_info->SetFirst();
            //    while (ability_info = skill_info->GetNext())
            //    {
            //        if ((ability_info->m_wAbilityID == 0) || 
            //            (ability_info->m_wAbilityID == eABILITY_TELEPORT) || 
            //            (ability_info->m_wAbilityID >= eABILITY_MAX))
            //        {
            //            return uiAutobotConditionBattle::kSkill_Max;
            //        }
            //    }
            //    return uiAutobotConditionBattle::kSkill_Attack;
            //}
        }
        break;
    default:
        break;
    }

    return uiAutobotConditionBattle::kSkill_Max;
}
#endif //_YMS_AUTOBOT_GAME