#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "uiCharacterStatusSockecInfo.h"
#include "uiCharacterStatus/uiCharacterStatusMain.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Hero.h"
#include "SCItemSlot.h"
#include "OptionListParser.h"
#include "HeroEquipmentContainer.h"
#include "ItemUnitRenderer.h"
#include "GlobalFunc.h"

const DWORD uiCharacterStatusSockecInfo::kDialog_ID = StrToWzID("0403");

namespace 
{
    bool SortSocketOptionByOptionPoint(const ItemAttrCalculator::SocketOptionCalculator::CalculationResult& lhs, 
        const ItemAttrCalculator::SocketOptionCalculator::CalculationResult& lhr)
    {
        return (lhs.option_point > lhr.option_point);
    }
    //------------------------------------------------------------------------------ 
    int GetRowSocketOptionByOptionPoint(const eATTR_TYPE attr_type, 
        uiCharacterStatusSockecInfo::SocketOptionVector& socketcount_vector)
    {
        uiCharacterStatusSockecInfo::SocketOptionVector::iterator begin = socketcount_vector.begin();
        uiCharacterStatusSockecInfo::SocketOptionVector::iterator end = socketcount_vector.end();

        for (int i = 0; begin != end; ++begin, ++i)
        {
            if (begin->attr_option_code == attr_type)
            {
                return i;
            }
        }
        return -1;
    }
    //------------------------------------------------------------------------------ 
    int GetColSocketOptionByAttribite(eEQUIPCONTAINER_POS equip_position)
    {
        switch (equip_position)
        {
        case eEQUIPCONTAINER_WEAPON:    return 0; 
        case eEQUIPCONTAINER_ARMOR:     return 1; 
        case eEQUIPCONTAINER_PROTECTOR: return 2; 
        case eEQUIPCONTAINER_HELMET:    return 3; 
        case eEQUIPCONTAINER_PANTS:     return 4; 
        case eEQUIPCONTAINER_BOOTS:     return 5; 
        case eEQUIPCONTAINER_GLOVE:     return 6; 
        case eEQUIPCONTAINER_BELT:      return 7; 
        case eEQUIPCONTAINER_SHIRTS:    return 8; 
        }

        return -1;
    }
    //------------------------------------------------------------------------------ 
    WzColor GetSocketOptionTextColor(int val)
    {
        WzColor text_color = WzColor_RGBA(255,255,255,255);

        if (val > 0)
        {
            text_color = WzColor_RGBA(0,255,0,255);
        }
        else if (val < 0)
        {
            text_color = WzColor_RGBA(0,0,255,255);
        }

        return text_color;
    }
};

uiCharacterStatusSockecInfo::uiCharacterStatusSockecInfo(InterfaceManager* interface_manager):
uiBase(interface_manager)
{
}

uiCharacterStatusSockecInfo::~uiCharacterStatusSockecInfo()
{
}

void uiCharacterStatusSockecInfo::Init( CDrawBase* draw_base )
{
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);
    RegisterControlPosition(StrToWzID("S008"), kControl_Text_S008);
    RegisterControlPosition(StrToWzID("S009"), kControl_Text_S009);
    RegisterControlPosition(StrToWzID("S010"), kControl_Text_S010);
    RegisterControlPosition(StrToWzID("S011"), kControl_Text_S011);
    RegisterControlPosition(StrToWzID("S012"), kControl_Text_S012);
    RegisterControlPosition(StrToWzID("S013"), kControl_Text_S013);
    RegisterControlPosition(StrToWzID("S014"), kControl_Text_S014);
    RegisterControlPosition(StrToWzID("S015"), kControl_Text_S015);
    RegisterControlPosition(StrToWzID("S016"), kControl_Text_S016);
    RegisterControlPosition(StrToWzID("S017"), kControl_Text_S017);
    RegisterControlPosition(StrToWzID("S018"), kControl_Text_S018);
    RegisterControlPosition(StrToWzID("S019"), kControl_Text_S019);
    RegisterControlPosition(StrToWzID("S020"), kControl_Text_S020);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S021"), kControl_Text_S021);
    RegisterControlPosition(StrToWzID("S022"), kControl_Text_S022);
    RegisterControlPosition(StrToWzID("S023"), kControl_Text_S023);
    RegisterControlPosition(StrToWzID("S024"), kControl_Text_S024);
    RegisterControlPosition(StrToWzID("S025"), kControl_Text_S025);
    RegisterControlPosition(StrToWzID("S026"), kControl_Text_S026);
    RegisterControlPosition(StrToWzID("S027"), kControl_Text_S027);
    RegisterControlPosition(StrToWzID("S028"), kControl_Text_S028);
    RegisterControlPosition(StrToWzID("S029"), kControl_Text_S029);
    RegisterControlPosition(StrToWzID("S031"), kControl_Text_S031);
    RegisterControlPosition(StrToWzID("S032"), kControl_Text_S032);
    RegisterControlPosition(StrToWzID("S033"), kControl_Text_S033);
    RegisterControlPosition(StrToWzID("S034"), kControl_Text_S034);
    RegisterControlPosition(StrToWzID("S035"), kControl_Text_S035);
    RegisterControlPosition(StrToWzID("S036"), kControl_Text_S036);
    RegisterControlPosition(StrToWzID("S037"), kControl_Text_S037);
    RegisterControlPosition(StrToWzID("S038"), kControl_Text_S038);
    RegisterControlPosition(StrToWzID("S039"), kControl_Text_S039);
    RegisterControlPosition(StrToWzID("S041"), kControl_Text_S041);
    RegisterControlPosition(StrToWzID("S042"), kControl_Text_S042);
    RegisterControlPosition(StrToWzID("S043"), kControl_Text_S043);
    RegisterControlPosition(StrToWzID("S044"), kControl_Text_S044);
    RegisterControlPosition(StrToWzID("S045"), kControl_Text_S045);
    RegisterControlPosition(StrToWzID("S046"), kControl_Text_S046);
    RegisterControlPosition(StrToWzID("S047"), kControl_Text_S047);
    RegisterControlPosition(StrToWzID("S048"), kControl_Text_S048);
    RegisterControlPosition(StrToWzID("S049"), kControl_Text_S049);
    RegisterControlPosition(StrToWzID("S051"), kControl_Text_S051);
    RegisterControlPosition(StrToWzID("S052"), kControl_Text_S052);
    RegisterControlPosition(StrToWzID("S053"), kControl_Text_S053);
    RegisterControlPosition(StrToWzID("S054"), kControl_Text_S054);
    RegisterControlPosition(StrToWzID("S055"), kControl_Text_S055);
    RegisterControlPosition(StrToWzID("S056"), kControl_Text_S056);
    RegisterControlPosition(StrToWzID("S057"), kControl_Text_S057);
    RegisterControlPosition(StrToWzID("S058"), kControl_Text_S058);
    RegisterControlPosition(StrToWzID("S059"), kControl_Text_S059);
    RegisterControlPosition(StrToWzID("S061"), kControl_Text_S061);
    RegisterControlPosition(StrToWzID("S062"), kControl_Text_S062);
    RegisterControlPosition(StrToWzID("S063"), kControl_Text_S063);
    RegisterControlPosition(StrToWzID("S064"), kControl_Text_S064);
    RegisterControlPosition(StrToWzID("S065"), kControl_Text_S065);
    RegisterControlPosition(StrToWzID("S066"), kControl_Text_S066);
    RegisterControlPosition(StrToWzID("S067"), kControl_Text_S067);
    RegisterControlPosition(StrToWzID("S068"), kControl_Text_S068);
    RegisterControlPosition(StrToWzID("S069"), kControl_Text_S069);
    RegisterControlPosition(StrToWzID("S071"), kControl_Text_S071);
    RegisterControlPosition(StrToWzID("S072"), kControl_Text_S072);
    RegisterControlPosition(StrToWzID("S073"), kControl_Text_S073);
    RegisterControlPosition(StrToWzID("S074"), kControl_Text_S074);
    RegisterControlPosition(StrToWzID("S075"), kControl_Text_S075);
    RegisterControlPosition(StrToWzID("S076"), kControl_Text_S076);
    RegisterControlPosition(StrToWzID("S077"), kControl_Text_S077);
    RegisterControlPosition(StrToWzID("S078"), kControl_Text_S078);
    RegisterControlPosition(StrToWzID("S079"), kControl_Text_S079);
    RegisterControlPosition(StrToWzID("S081"), kControl_Text_S081);
    RegisterControlPosition(StrToWzID("S082"), kControl_Text_S082);
    RegisterControlPosition(StrToWzID("S083"), kControl_Text_S083);
    RegisterControlPosition(StrToWzID("S084"), kControl_Text_S084);
    RegisterControlPosition(StrToWzID("S085"), kControl_Text_S085);
    RegisterControlPosition(StrToWzID("S086"), kControl_Text_S086);
    RegisterControlPosition(StrToWzID("S087"), kControl_Text_S087);
    RegisterControlPosition(StrToWzID("S088"), kControl_Text_S088);
    RegisterControlPosition(StrToWzID("S089"), kControl_Text_S089);
    RegisterControlPosition(StrToWzID("S091"), kControl_Text_S091);
    RegisterControlPosition(StrToWzID("S092"), kControl_Text_S092);
    RegisterControlPosition(StrToWzID("S093"), kControl_Text_S093);
    RegisterControlPosition(StrToWzID("S094"), kControl_Text_S094);
    RegisterControlPosition(StrToWzID("S095"), kControl_Text_S095);
    RegisterControlPosition(StrToWzID("S096"), kControl_Text_S096);
    RegisterControlPosition(StrToWzID("S097"), kControl_Text_S097);
    RegisterControlPosition(StrToWzID("S098"), kControl_Text_S098);
    RegisterControlPosition(StrToWzID("S099"), kControl_Text_S099);
    RegisterControlPosition(StrToWzID("S101"), kControl_Text_S101);
    RegisterControlPosition(StrToWzID("S102"), kControl_Text_S102);
    RegisterControlPosition(StrToWzID("S103"), kControl_Text_S103);
    RegisterControlPosition(StrToWzID("S104"), kControl_Text_S104);
    RegisterControlPosition(StrToWzID("S105"), kControl_Text_S105);
    RegisterControlPosition(StrToWzID("S106"), kControl_Text_S106);
    RegisterControlPosition(StrToWzID("S107"), kControl_Text_S107);
    RegisterControlPosition(StrToWzID("S108"), kControl_Text_S108);
    RegisterControlPosition(StrToWzID("S109"), kControl_Text_S109);
    RegisterControlPosition(StrToWzID("S111"), kControl_Text_S111);
    RegisterControlPosition(StrToWzID("S112"), kControl_Text_S112);
    RegisterControlPosition(StrToWzID("S113"), kControl_Text_S113);
    RegisterControlPosition(StrToWzID("S114"), kControl_Text_S114);
    RegisterControlPosition(StrToWzID("S115"), kControl_Text_S115);
    RegisterControlPosition(StrToWzID("S116"), kControl_Text_S116);
    RegisterControlPosition(StrToWzID("S117"), kControl_Text_S117);
    RegisterControlPosition(StrToWzID("S118"), kControl_Text_S118);
    RegisterControlPosition(StrToWzID("S119"), kControl_Text_S119);
    RegisterControlPosition(StrToWzID("S121"), kControl_Text_S121);
    RegisterControlPosition(StrToWzID("S122"), kControl_Text_S122);
    RegisterControlPosition(StrToWzID("S123"), kControl_Text_S123);
    RegisterControlPosition(StrToWzID("S124"), kControl_Text_S124);
    RegisterControlPosition(StrToWzID("S125"), kControl_Text_S125);
    RegisterControlPosition(StrToWzID("S126"), kControl_Text_S126);
    RegisterControlPosition(StrToWzID("S127"), kControl_Text_S127);
    RegisterControlPosition(StrToWzID("S128"), kControl_Text_S128);
    RegisterControlPosition(StrToWzID("S129"), kControl_Text_S129);
    RegisterControlPosition(StrToWzID("S131"), kControl_Text_S131);
    RegisterControlPosition(StrToWzID("S132"), kControl_Text_S132);
    RegisterControlPosition(StrToWzID("S133"), kControl_Text_S133);
    RegisterControlPosition(StrToWzID("S134"), kControl_Text_S134);
    RegisterControlPosition(StrToWzID("S135"), kControl_Text_S135);
    RegisterControlPosition(StrToWzID("S136"), kControl_Text_S136);
    RegisterControlPosition(StrToWzID("S137"), kControl_Text_S137);
    RegisterControlPosition(StrToWzID("S138"), kControl_Text_S138);
    RegisterControlPosition(StrToWzID("S139"), kControl_Text_S139);
    RegisterControlPosition(StrToWzID("S141"), kControl_Text_S141);
    RegisterControlPosition(StrToWzID("S142"), kControl_Text_S142);
    RegisterControlPosition(StrToWzID("S143"), kControl_Text_S143);
    RegisterControlPosition(StrToWzID("S144"), kControl_Text_S144);
    RegisterControlPosition(StrToWzID("S145"), kControl_Text_S145);
    RegisterControlPosition(StrToWzID("S146"), kControl_Text_S146);
    RegisterControlPosition(StrToWzID("S147"), kControl_Text_S147);
    RegisterControlPosition(StrToWzID("S148"), kControl_Text_S148);
    RegisterControlPosition(StrToWzID("S149"), kControl_Text_S149);
    RegisterControlPosition(StrToWzID("S151"), kControl_Text_S151);
    RegisterControlPosition(StrToWzID("S152"), kControl_Text_S152);
    RegisterControlPosition(StrToWzID("S153"), kControl_Text_S153);
    RegisterControlPosition(StrToWzID("S154"), kControl_Text_S154);
    RegisterControlPosition(StrToWzID("S155"), kControl_Text_S155);
    RegisterControlPosition(StrToWzID("S156"), kControl_Text_S156);
    RegisterControlPosition(StrToWzID("S157"), kControl_Text_S157);
    RegisterControlPosition(StrToWzID("S158"), kControl_Text_S158);
    RegisterControlPosition(StrToWzID("S159"), kControl_Text_S159);
    RegisterControlPosition(StrToWzID("S161"), kControl_Text_S161);
    RegisterControlPosition(StrToWzID("S162"), kControl_Text_S162);
    RegisterControlPosition(StrToWzID("S163"), kControl_Text_S163);
    RegisterControlPosition(StrToWzID("S164"), kControl_Text_S164);
    RegisterControlPosition(StrToWzID("S165"), kControl_Text_S165);
    RegisterControlPosition(StrToWzID("S166"), kControl_Text_S166);
    RegisterControlPosition(StrToWzID("S167"), kControl_Text_S167);
    RegisterControlPosition(StrToWzID("S168"), kControl_Text_S168);
    RegisterControlPosition(StrToWzID("S169"), kControl_Text_S169);
    RegisterControlPosition(StrToWzID("S171"), kControl_Text_S171);
    RegisterControlPosition(StrToWzID("S172"), kControl_Text_S172);
    RegisterControlPosition(StrToWzID("S173"), kControl_Text_S173);
    RegisterControlPosition(StrToWzID("S174"), kControl_Text_S174);
    RegisterControlPosition(StrToWzID("S175"), kControl_Text_S175);
    RegisterControlPosition(StrToWzID("S176"), kControl_Text_S176);
    RegisterControlPosition(StrToWzID("S177"), kControl_Text_S177);
    RegisterControlPosition(StrToWzID("S178"), kControl_Text_S178);
    RegisterControlPosition(StrToWzID("S179"), kControl_Text_S179);
    RegisterControlPosition(StrToWzID("S030"), kControl_Text_S030);
    RegisterControlPosition(StrToWzID("S040"), kControl_Text_S040);
    RegisterControlPosition(StrToWzID("S050"), kControl_Text_S050);
    RegisterControlPosition(StrToWzID("S060"), kControl_Text_S060);
    RegisterControlPosition(StrToWzID("S070"), kControl_Text_S070);
    RegisterControlPosition(StrToWzID("S080"), kControl_Text_S080);
    RegisterControlPosition(StrToWzID("S090"), kControl_Text_S090);
    RegisterControlPosition(StrToWzID("S100"), kControl_Text_S100);
    RegisterControlPosition(StrToWzID("S110"), kControl_Text_S110);
    RegisterControlPosition(StrToWzID("S120"), kControl_Text_S120);
    RegisterControlPosition(StrToWzID("S130"), kControl_Text_S130);
    RegisterControlPosition(StrToWzID("S140"), kControl_Text_S140);
    RegisterControlPosition(StrToWzID("S150"), kControl_Text_S150);
    RegisterControlPosition(StrToWzID("S160"), kControl_Text_S160);
    RegisterControlPosition(StrToWzID("S170"), kControl_Text_S170);
    RegisterControlPosition(StrToWzID("S180"), kControl_Text_S180);
    RegisterControlPosition(StrToWzID("S201"), kControl_Text_S201);
    RegisterControlPosition(StrToWzID("S202"), kControl_Text_S202);
    RegisterControlPosition(StrToWzID("S203"), kControl_Text_S203);
    RegisterControlPosition(StrToWzID("S204"), kControl_Text_S204);
    RegisterControlPosition(StrToWzID("S205"), kControl_Text_S205);
    RegisterControlPosition(StrToWzID("S206"), kControl_Text_S206);
    RegisterControlPosition(StrToWzID("S207"), kControl_Text_S207);
    RegisterControlPosition(StrToWzID("S208"), kControl_Text_S208);
    RegisterControlPosition(StrToWzID("S209"), kControl_Text_S209);
    RegisterControlPosition(StrToWzID("S210"), kControl_Text_S210);
    RegisterControlPosition(StrToWzID("S211"), kControl_Text_S211);
    RegisterControlPosition(StrToWzID("S212"), kControl_Text_S212);
    RegisterControlPosition(StrToWzID("S213"), kControl_Text_S213);
    RegisterControlPosition(StrToWzID("S214"), kControl_Text_S214);
    RegisterControlPosition(StrToWzID("S215"), kControl_Text_S215);
    RegisterControlPosition(StrToWzID("S216"), kControl_Text_S216);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("P035"), kControl_Picture_P035);
    RegisterControlPosition(StrToWzID("P043"), kControl_Picture_P043);
    RegisterControlPosition(StrToWzID("P044"), kControl_Picture_P044);
    RegisterControlPosition(StrToWzID("P045"), kControl_Picture_P045);
    RegisterControlPosition(StrToWzID("P046"), kControl_Picture_P046);
    RegisterControlPosition(StrToWzID("P047"), kControl_Picture_P047);
    RegisterControlPosition(StrToWzID("P048"), kControl_Picture_P048);
    RegisterControlPosition(StrToWzID("P049"), kControl_Picture_P049);
    RegisterControlPosition(StrToWzID("P050"), kControl_Picture_P050);

    for (DWORD control_position = kControl_Text_S006; control_position <= kControl_Text_S216; ++control_position)
    {
        GetControlWZ_byPosition(control_position)->SetTextWZ("");
    }

    socket_option_show_begin_index_ = 0;
    socketoption_vector().clear();

    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
        scroll_control->SetScrollRangeWZ(0, 0);
        scroll_control->SetScrollPosWZ(0);
    }
}

void uiCharacterStatusSockecInfo::Release()
{

}

void uiCharacterStatusSockecInfo::OnShowWindow(BOOL is_show)
{
    if (is_show == TRUE)
    {
        CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
        if (scroll_control != NULL)
        {
            socket_option_show_begin_index_ = 0;
            scroll_control->SetScrollRangeWZ(0, 0);
            scroll_control->SetScrollPosWZ(0);
            scroll_control->SetChangeSizeThumb(100);
            socketoption_vector().clear();
        }
        RefreshTab();
    }
    else
    {

    }    
}

void uiCharacterStatusSockecInfo::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }

    if (GetMan() == NULL)
    {
        return;
    }

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_LINETOP:
        OnVScrollUp(static_cast<CCtrlVScrollWZ*>(GetControlWZ(message->dwCtrlID)));
        break;
    case RT_MSG_SB_LINEBOTTOM:
        OnVScrollDown(static_cast<CCtrlVScrollWZ*>(GetControlWZ(message->dwCtrlID)));
        break;
    case RT_MSG_SB_THUMBTRACK:
    case RT_MSG_SB_THUMBPOSITION:
    case RT_MSG_SB_ENDSCROLL:
        OnVScrollThumb(static_cast<CCtrlVScrollWZ*>(GetControlWZ(message->dwCtrlID)));
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiCharacterStatusSockecInfo::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C000:
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	case kControl_CheckBT_C004:
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C002:
    case kControl_CheckBT_C003:
        {
            uiCharacterStatusMan* ui_status_manager = GET_DIALOG_MANAGER(uiCharacterStatusMan, UIMAN_CHARACTERSTATUS);
            if (ui_status_manager != NULL)
            {
                TabCheckButtonControllManager& tab_manager = ui_status_manager->status_dialog_tab_manager();
                tab_manager.OnClickTab(control_position - kControl_CheckBT_C000);
            }
            RefreshTab();
        }
        break;
    default:
        {
        }
    }

    return result;
}

void uiCharacterStatusSockecInfo::OnUpdateSolarDialog()
{
    UpdateMouseWheel();

    if (GetDialogWZ()->HaveFocus() == true)
    {
        GlobalFunc::MoveToTopDialog(uiCharacterStatusMain::kDialog_ID);
    }
}

void uiCharacterStatusSockecInfo::OnRenderSolarDialog()
{
    for (int control_position = kControl_Text_S006; control_position <= kControl_Text_S216; ++control_position)
    {
        GetControlWZ_byPosition(control_position)->SetTextWZ("");
    }

    int backup_cur_apply_socket_count = socketoption_vector().size();
    socketoption_vector().clear();
    CalcTotalSocketOption(socketoption_vector());

    //------------------------------------------------------------------------------ 
    //! 소켓 옵션이 바뀌었으면 스크롤바 수정.
    if (backup_cur_apply_socket_count != socketoption_vector().size())
    {
        set_socket_option_show_begin_index(0);

        backup_cur_apply_socket_count = socketoption_vector().size();

        CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
        if (scroll_control != NULL)
        {
            // 스크롤 바 업데이트
            int max_num = max(0, backup_cur_apply_socket_count - kSocketOptionShowRowCount);

            scroll_control->SetScrollRangeWZ(0, max_num);
            scroll_control->SetScrollPosWZ(socket_option_show_begin_index());

            int thumb_size = 100 / (max_num + 1);
            scroll_control->SetChangeSizeThumb(thumb_size);
        }
    }
    //------------------------------------------------------------------------------ 

    //! 내림차순으로 정렬
    std::sort(socketoption_vector().begin(), socketoption_vector().end(), SortSocketOptionByOptionPoint);

    //! 소캣 기능 출력
    UpdateSocketOptionName(socketoption_vector());

    //------------------------------------------------------------------------------ 
    HeroEquipmentContainer* equip_container = static_cast<HeroEquipmentContainer*>(g_pHero->GetHeroEquipContainer());
    ItemUnitRenderer::RenderUnit * render_unit = NULL;
    ItemUnitRenderer* item_renderer = equip_container->GetItemUnitRender();
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    SCItemSlot* item_slot_back_up = NULL;

    item_renderer->GetItemUnitHashTable()->SetFirst();
    while (render_unit = item_renderer->GetItemUnitHashTable()->GetNext())
    {		
        if ((render_unit->m_pIconImage == NULL) || 
            (render_unit->m_pRefSlot == NULL) || 
            (render_unit->m_pRefSlot->GetSlotType() != ST_ITEM))
        {
            continue;
        }

        SCItemSlot* item_slot = (SCItemSlot*)(render_unit->m_pRefSlot);

        int col_index = GetColSocketOptionByAttribite(static_cast<eEQUIPCONTAINER_POS>(item_slot->GetItemInfo()->m_wEqPos));

        if ((col_index >= 0) && (col_index < kSocketOptionShowColCount))
        {
            UpdateSocketOptionValue(item_slot, col_index, socketoption_vector());

            RECT area_rect = GetControlWZ_byPosition(kControl_Picture_P035 + col_index)->GetSizeRect();
            render_unit->m_pIconImage->Render(&area_rect);

            //소켓 툴팁
            if (IsOverControl(GetControlWZ_byPosition(kControl_Picture_P035 + col_index)))
            {
                item_slot_back_up = item_slot;
            }
        }

    }

    // 툴팁 출력
    if (this->IsLocked() == false)
    {
        RenderTooltip_SocketOption(socketoption_vector());

        if (item_slot_back_up != NULL)
        {
            tooltip_manager->RegisterSlotInfoTooltip(item_slot_back_up);
			tooltip_manager->SetTooltipDialogID(GetDialogKey());
        }
    }
}

bool uiCharacterStatusSockecInfo::OnVScrollUp(CCtrlVScrollWZ* scroll_control)
{
    bool result = true;
    if (scroll_control == NULL)
    {
        result = false;
        return result;
    }
    
    int pos = scroll_control->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    scroll_control->GetScrollRangeWZ(&mmin, &mmax);
    if(pos > mmin)
    {
        --pos;
        scroll_control->SetScrollPosWZ(pos);
    }

    DWORD control_index = GetControlPosition_byID(scroll_control->GetCtrlID());
    if (control_index == kControl_VScroll_V000)
    {
        set_socket_option_show_begin_index(pos);
    }

    return result;
}

bool uiCharacterStatusSockecInfo::OnVScrollDown(CCtrlVScrollWZ* scroll_control)
{
    bool result = true;
    if (scroll_control == NULL)
    {
        result = false;
        return result;
    }

    int pos = scroll_control->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    scroll_control->GetScrollRangeWZ(&mmin, &mmax); 

    if (pos < mmax)
    {
        ++pos;
        scroll_control->SetScrollPosWZ(pos);
    }

    DWORD control_index = GetControlPosition_byID(scroll_control->GetCtrlID());
    if (control_index == kControl_VScroll_V000)
    {
        set_socket_option_show_begin_index(pos);
    }

    return result;
}

bool uiCharacterStatusSockecInfo::OnVScrollThumb(CCtrlVScrollWZ* scroll_control)
{
    bool result = true;
    if (scroll_control == NULL)
    {
        result = false;
        return result;
    }

    int pos2 = scroll_control->GetScrollPosWZ();
    int pos = pos2;
    int mmin = 0, mmax = 0;
    scroll_control->GetScrollRangeWZ(&mmin, &mmax);

    pos = max(mmin, pos);
    pos = min(mmax, pos);

    if(pos != pos2)
    {
        scroll_control->SetScrollPosWZ(pos);
    }

    DWORD control_index = GetControlPosition_byID(scroll_control->GetCtrlID());
    if (control_index == kControl_VScroll_V000)
    {
        set_socket_option_show_begin_index(pos);
    }

    return result;
}

void uiCharacterStatusSockecInfo::RefreshTab()
{
    CCtrlButtonCheckWZ* check_button = NULL;
    for (int position = kControl_CheckBT_C000; position < (kControl_CheckBT_C003 + 1); ++position)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(position)));
        if (check_button == NULL)
        {
            continue;
        }
        check_button->SetCheckState((position == kControl_CheckBT_C001) ? (true) : (false));
    }
}

void uiCharacterStatusSockecInfo::UpdateSocketOptionValue(SCItemSlot* item_slot, 
                                                          int col, 
                                                          IN uiCharacterStatusSockecInfo::SocketOptionVector& socketcount_vector)
{
    ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap socket_option_calc_result; 
    if (ItemAttrCalculator::SocketOptionCalculator::CalculateItemOptionPoint(*item_slot, socket_option_calc_result) == true)
    {
        FOREACH_CONTAINER(ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap::value_type& socket_option_calc_pair, 
            socket_option_calc_result, 
            ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap)
        {
            ItemAttrCalculator::SocketOptionCalculator::CalculationResult& socket_option_calc = socket_option_calc_pair.second;

            int row = GetRowSocketOptionByOptionPoint(socket_option_calc.attr_option_code, 
                socketcount_vector);

            if ((row < 0) ||
                (row < socket_option_show_begin_index()) ||
                (row - socket_option_show_begin_index() >= kSocketOptionShowRowCount) ||
                (row >= (int)socketcount_vector.size()))
            {
                continue;
            }

            ZardOptionInfo* zard_option_info = SocketOptionParser::Instance()->FindZardOption(socket_option_calc.item_option_code);
            if (zard_option_info == NULL)
            {
                continue;
            }

            WzColor text_color = WzColor_RGBA(255,255,255,255);

            const ZardOptionInfo::OptionValueInfo* option_value_info = zard_option_info->Find(socketcount_vector[row].option_point);
            if (item_slot->IsExpiredTime() || item_slot->IsExpiredWaitTime())
            {
                text_color = WzColor_RGBA(110,110,110,255);
            }
            else
            {
                if (option_value_info != NULL)
                {
                    if (option_value_info->point > 0)
                    {
                        text_color = WzColor_RGBA(0,255,0,255);
                    }
                    else if (option_value_info->point < 0)
                    {
                        text_color = WzColor_RGBA(0,0,255,255);
                    }
                }
            }

            SetSocketOptionValue(col, 
                row - socket_option_show_begin_index(), 
                socket_option_calc.option_point, 
                text_color);
        }
    }
}

void uiCharacterStatusSockecInfo::UpdateSocketOptionName(IN uiCharacterStatusSockecInfo::SocketOptionVector& socketcount_vector)
{
    if (socket_option_show_begin_index() >= (int)socketcount_vector.size())
    {
        return;
    }

    //! 스킬명
    uiCharacterStatusSockecInfo::SocketOptionVector::iterator begin = socketcount_vector.begin() + socket_option_show_begin_index();
    uiCharacterStatusSockecInfo::SocketOptionVector::iterator end = socketcount_vector.end();
    int index = 0;
    for ( ; begin != end; ++begin)
    {
        BASE_OptionList* socket_option_desc = OptionListParser::Instance()->GetOptionList(begin->item_option_code);
        if (socket_option_desc != NULL)
        {
            WzColor text_color = WzColor_RGBA(255,255,255,255);

            TCHAR str_name[INTERFACE_STRING_LENGTH] = {NULL, };
            TCHAR str_msg[INTERFACE_STRING_LENGTH] = {NULL, };
            g_InterfaceManager.GetInterfaceString(socket_option_desc->mdwNameCode, str_name);
            GetControlWZ_byPosition(kControl_Text_S006 + index)->SetTextWZ(str_name);

            ZardOptionInfo* zard_option_info = SocketOptionParser::Instance()->FindZardOption(begin->item_option_code);
            if (zard_option_info != NULL)
            {
                const ZardOptionInfo::OptionValueInfo* option_value_info = zard_option_info->Find(begin->option_point);
                if (option_value_info != NULL)
                {
                    text_color = GetSocketOptionTextColor(option_value_info->value);
                
                    Sprintf(str_msg, _T("%+d"), option_value_info->value);

                    //옵션명 접미사
                    GlobalFunc::AddOptionSuffix(option_value_info->type, socket_option_desc, str_msg);
                }
                else
                {
                    //! 미발동
                    g_InterfaceManager.GetInterfaceString(3444, str_msg);
                }

                SetSocketOptionValue(static_cast<UIControlPos>(kControl_Text_S201 + index), str_msg, text_color);
            }

            SetSocketOptionValue(static_cast<UIControlPos>(kControl_Text_S030 + index), begin->option_point, text_color);
        }

        ++index;

        if (kControl_Text_S006 + index > kControl_Text_S000)
        {
            break;
        }
    }
}

void uiCharacterStatusSockecInfo::SetSocketOptionValue(int col, int row, int val, WzColor text_color)
{
    int control_posistion = kControl_Text_S021 + (col * kSocketOptionShowRowCount) + row;
    if (col < 0 || col >= kSocketOptionShowColCount || 
        row < 0 || row >= kSocketOptionShowRowCount || 
        control_posistion < kControl_Text_S021 || control_posistion > kControl_Text_S180)
    {
        return;
    }

    SetSocketOptionValue(static_cast<UIControlPos>(control_posistion), val, text_color);
}

void uiCharacterStatusSockecInfo::SetSocketOptionValue(UIControlPos control_position, TCHAR* text_string, WzColor text_color)
{
    CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);

    if (control_ptr != NULL)
    {
        if (control_ptr->IsKind(CTRL_KIND_TEXT) == true)
        {
            (static_cast<CCtrlStaticWZ*>(control_ptr))->SetTextColor(text_color);
        }

        control_ptr->SetTextWZA(text_string);
    }
}

void uiCharacterStatusSockecInfo::SetSocketOptionValue(UIControlPos control_position, int val, WzColor text_color)
{
    CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);

    if (control_ptr != NULL)
    {
        static TCHAR str[MAX_PATH] = {NULL, };
        Sprintf(str, _T("%d"), val);

        if (control_ptr->IsKind(CTRL_KIND_TEXT) == true)
        {
            (static_cast<CCtrlStaticWZ*>(control_ptr))->SetTextColor(text_color);
        }

        control_ptr->SetTextWZA(str);
    }
}

void uiCharacterStatusSockecInfo::CalcTotalSocketOption(OUT uiCharacterStatusSockecInfo::SocketOptionVector& socketcount_vector)
{
    HeroEquipmentContainer* equip_container = static_cast<HeroEquipmentContainer*>(g_pHero->GetHeroEquipContainer());

    ItemUnitRenderer::RenderUnit * render_unit = NULL;
    ItemUnitRenderer* item_renderer = equip_container->GetItemUnitRender();

    item_renderer->GetItemUnitHashTable()->SetFirst();
    while (render_unit = item_renderer->GetItemUnitHashTable()->GetNext())
    {		
        if ((render_unit->m_pIconImage == NULL) || 
            (render_unit->m_pRefSlot == NULL) || 
            (render_unit->m_pRefSlot->GetSlotType() != ST_ITEM))
        {
            continue;
        }

        SCItemSlot* item_slot = (SCItemSlot*)(render_unit->m_pRefSlot);
        if (item_slot->IsExpiredTime() || item_slot->IsExpiredWaitTime())
        {
            continue;
        }

        ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap socket_option_calc_result; 
        if (ItemAttrCalculator::SocketOptionCalculator::CalculateItemOptionPoint(*item_slot, socket_option_calc_result) == false)
        {
            continue;
        }

        ns_formula::EquipSlotsManageInfo* equip_slot_manage_info = g_pHero->GetEquipmentContainer()->GetEquipmentManageInfo();
        ns_formula::EquipSlotsManageInfo::Node* it = equip_slot_manage_info->slots;
        if (!equip_slot_manage_info->slots[item_slot->GetPos()].activated) // 아이템이 활성화인 경우만 적용
        {
            continue;
        }

        FOREACH_CONTAINER(ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap::value_type& socket_option_calc_pair, 
            socket_option_calc_result, 
            ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap)
        {
            ItemAttrCalculator::SocketOptionCalculator::CalculationResult& socket_option_calc = socket_option_calc_pair.second;

            //------------------------------------------------------------------------------ 
            uiCharacterStatusSockecInfo::SocketOptionVector::iterator begin = socketcount_vector.begin();
            uiCharacterStatusSockecInfo::SocketOptionVector::iterator end = socketcount_vector.end();
            bool is_find = false;
            for (; begin != end; ++begin)
            {
                if (begin->attr_option_code == socket_option_calc.attr_option_code)
                {
                    begin->option_point += socket_option_calc.option_point;
                    is_find = true;
                    break;
                }
            }
            if (is_find == false)
            {
                socketcount_vector.push_back(socket_option_calc);
            }
        }
    }
}

void uiCharacterStatusSockecInfo::RenderTooltip_SocketOption(IN uiCharacterStatusSockecInfo::SocketOptionVector& socketcount_vector)
{
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip_manager == NULL)
    {
        return;
    }

    for (int position_index = 0; position_index < kSocketOptionShowRowCount; ++position_index)
    {
        if (position_index + socket_option_show_begin_index() >= (int)socketcount_vector.size())
        {
            return;
        }

        if (IsOverControl(GetControlWZ_byPosition(kControl_Text_S006 + position_index)))
        {
            ItemAttrCalculator::SocketOptionCalculator::CalculationResult* result = &(socketcount_vector[position_index + socket_option_show_begin_index()]);

            tooltip_manager->RegisterTooltipSocketOption(result->item_option_code);
			tooltip_manager->SetTooltipDialogID(GetDialogKey());
        }
    }
}

void uiCharacterStatusSockecInfo::UpdateMouseWheel()
{
    //! 소켓 옵션 리스트들 스크롤 체크
    CControlWZ* control_lt = GetControlWZ_byPosition(kControl_Text_S006);
    CControlWZ* control_rb = GetControlWZ_byPosition(kControl_Text_S216);
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if ((control_lt != NULL) && (control_rb != NULL) && (scroll_control != NULL))
    {
        RECT area;
        area.left = control_lt->GetSizeRect().left;
        area.top = control_lt->GetSizeRect().top;
        area.right = control_rb->GetSizeRect().right;
        area.bottom = control_rb->GetSizeRect().bottom;

        switch (GetMouseWheelStatus(&area))
        {
        case MOUSE_WHEEL_UP:
            {
                OnVScrollUp(scroll_control);
            }
            break;

        case MOUSE_WHEEL_DOWN:
            {
                OnVScrollDown(scroll_control);
            }
            break;
        }
    }
}

#endif //_NA_003027_20111013_HONOR_SYSTEM