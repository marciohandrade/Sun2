#include "SunClientPrecompiledHeader.h"
#include "EnchantDialog3.h"

#ifdef _NA_003966_20111227_ADD_ENCHANT

#include "ItemUnitRenderer.h"
#include "Mouse.h"
#include "MouseHandler.h"
#include "slotkeygenerator.h"
#include "SCItemSlot.h"
#include "ItemManager.h"
#include "SoundEffect.h"
#include "CursorChangeTransact.h"
#include "hero.h"
#include "Application.h"
#include "globalfunc.h"
#include "interfacemanager.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "HeroActionInput.h"
#ifdef _NA_20100307_BETAKEY_SYSTEM
#include "uiLoginMan/uiLoginMan.h"
#include "BetaKeyImpl.h"
#endif//_NA_20100307_BETAKEY_SYSTEM

//----------------------------------------------------------------------------
/**
*/
WzID EnchantDialog::m_wzId[] = 
{
    StrToWzID("B000"),//ENCHANT_BTN_SLOT_TARGET = 0,
    StrToWzID("B001"),//ENCHANT_BTN_SLOT_2,
    StrToWzID("B002"),//ENCHANT_BTN_SLOT_3, 
    StrToWzID("B003"),//ENCHANT_BTN_SLOT_4, 
    StrToWzID("B004"),//ENCHANT_BTN_SLOT_5, 



    StrToWzID("B999"),//ENCHANT_BTN_EXIT
    StrToWzID("B998"),//ENCHANT_BTN_ENCHANT,
    StrToWzID("B005"),//ENCHANT_BTN_RETRY,
    StrToWzID("B006"),//ENCHANT_BTN_CANCEL,

    StrToWzID("C001"),//ENCHANT_CHK_25_PER,
    StrToWzID("C002"),//ENCHANT_CHK_50_PER,
    StrToWzID("C003"),//ENCHANT_CHK_75_PER,
    StrToWzID("C004"),//ENCHANT_CHK_100_PER,

    StrToWzID("T999"),//ENCHANT_TXT_TITLE,
    StrToWzID("T004"),//ENCHANT_TXT_KIND,
    StrToWzID("T011"),//ENCHANT_TXT_ITEM_NAME,

    StrToWzID("T005"),//ENCHANT_TXT_MAIN_SUB_MAT1,
    StrToWzID("T006"),//ENCHANT_TXT_MAIN_SUB_MAT2,
    StrToWzID("T007"),//ENCHANT_TXT_MAIN_SUB_MAT3,

    StrToWzID("T008"),//ENCHANT_TXT_MAIN_SUB_OPTION,

    StrToWzID("T012"),//ENCHANT_TXT_MAIN_SUB_MAT1_NUM,
    StrToWzID("T013"),//ENCHANT_TXT_MAIN_SUB_MAT2_NUM,
    StrToWzID("T014"),//ENCHANT_TXT_MAIN_SUB_MAT3_NUM,

    StrToWzID("T009"),//ENCHANT_TXT_REQUIRE_MONEY,
    StrToWzID("VS02"),//ENCHANT_VSCR_SUB,

    StrToWzID("T002"),//ENCHANT_TXT_TITLE2,
    StrToWzID("S000"),//ENCHANT_TXT_EXTRA_TITLE,
};

//----------------------------------------------------------------------------
/**
*/
EnchantDialog::EnchantDialog( void )
: m_itemRenderer( NULL )
, m_targetPos(INVALID_SLOT_POS)
, m_extraItemPos( INVALID_SLOT_POS )
, m_pendingTargetPos( -1 )
, m_matsForLv( NULL )
, m_enchantLevel( ENCHANT_LOW )
, m_enchantCost( 0 )
, m_enchantOpt( eENCHANT_NOT_OPT )
, m_probabilityIdx( 4 )
, m_DialogKey(0)
{
    //// ���� 5��(������1 + ���3)
    //m_EnchantSlotContainer.Init( 4, SI_ENCHANT );
    //m_EnchantOptionItemContainer.Init( 1, SI_ENCHANT_OPTION );

    m_lockInfoMap.clear();
    m_matList.clear();

    m_packetStatus = PACKET_STATUS_NONE;
}

//----------------------------------------------------------------------------
/**
*/
EnchantDialog::~EnchantDialog( void )
{
    //m_EnchantSlotContainer.Release();
    //m_EnchantOptionItemContainer.Release();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::Init( CDrawBase* /*pDrawBase*/ )
{
    InitUiControls();

    m_SlotContainer[0].Initialize( 1, SI_ENCHANT );
    m_SlotContainer[1].Initialize( 3, SI_ENCHANT_MATERIAL );
    m_SlotContainer[2].Initialize( 1, SI_ENCHANT_OPTION );

    enchant_popup_dialog_key_ = 0;
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::Release( void )
{
    m_SlotContainer[0].Release();
    m_SlotContainer[1].Release();
    m_SlotContainer[2].Release();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnUpdateSolarDialog()
{
    m_SlotContainer[0].Update();
    m_SlotContainer[1].Update();
    m_SlotContainer[2].Update();

    UpdateMouseOverObj();

    UpdateTooltip();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnRenderSolarDialog()
{
    m_SlotContainer[0].Render();
    m_SlotContainer[1].Render();
    m_SlotContainer[2].Render();
}

SCItemSlot* EnchantDialog::GetTargetItem()
{
    return m_SlotContainer[0].GetSlotItem(0);
}

SCItemSlot* EnchantDialog::GetExteraItem()
{
    return m_SlotContainer[2].GetSlotItem(0);
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::NetworkProc( MSG_BASE* pMsg )
{
    if( pMsg->m_byCategory == CG_ITEM )
    {
        switch( pMsg->m_byProtocol )
        {
        case CG_ITEM_ENCHANT_SUCCESS_ACK:   // ��þƮ ����
            OnEnchantSuccessAck( pMsg );
            break;

        case CG_ITEM_ENCHANT_FAILED_ACK:    // ��þƮ ����
            OnEnchantFailedAck( pMsg );
            break;

        case CG_ITEM_ENCHANT_NAK:           // ��þƮ �õ� ����
            OnItemEnchantNak( pMsg );
            break;
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::MessageProc( SI_MESSAGE* pMsg )
{
    switch( pMsg->eResultMsg )
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick( pMsg );
        break;
    case RT_MSG_RBUTTONCLICK:
        OnRButtonClick( pMsg );
        break;

    case RT_MSG_SB_PAGETOP:     // ���� ������ ������.
    case RT_MSG_SB_LINETOP:     // ���� ȭ��ǥ�� ������.
        {
            if( pMsg->dwCtrlID == m_wzId[ENCHANT_VSCR_SUB] )
            {
                assert( m_ctlMatListScroll );

                int pos = m_ctlMatListScroll->GetScrollPosWZ();

                int mmin = 0, mmax = 0;
                m_ctlMatListScroll->GetScrollRangeWZ( &mmin, &mmax );

                if( pos > mmin )
                {
                    m_ctlMatListScroll->SetScrollPosWZ( --pos );

                    RefreshEnchantMatList();
                }
            }
        }
        break;

    case RT_MSG_SB_PAGEBOTTOM:  // ���� ������ ������.
    case RT_MSG_SB_LINEBOTTOM:  // �Ʒ��� ȭ��ǥ�� ������
        {
            if( pMsg->dwCtrlID == m_wzId[ENCHANT_VSCR_SUB] )
            {
                assert( m_ctlMatListScroll );

                int pos = m_ctlMatListScroll->GetScrollPosWZ();

                int mmin = 0, mmax = 0;
                m_ctlMatListScroll->GetScrollRangeWZ( &mmin, &mmax );

                if( pos < mmax )
                {
                    m_ctlMatListScroll->SetScrollPosWZ( ++pos );

                    RefreshEnchantMatList();
                }
            }
        }
        break;

    case RT_MSG_SB_THUMBTRACK:      // ���� �巡�� �ϰ� �ִ�.
    case RT_MSG_SB_THUMBPOSITION:   // ���� �巡������ ���콺�� ���Ҵ�.
    case RT_MSG_SB_ENDSCROLL:       // ��ũ���� ���� �Ǿ���.
        {
            if( pMsg->dwCtrlID == m_wzId[ENCHANT_VSCR_SUB] )
            {
                assert( m_ctlMatListScroll );

                RefreshEnchantMatList();
            }
        }
        break;

    default:
        SolarDialog::MessageProc( pMsg );
        break;
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::Enchant( void )
{
    if (enchant_popup_dialog_key_ != 0)
    {
        // �˾�â�� ���������� ����
        return;
    }

    // ��ȿ�� ��æƮ�ΰ�?
    if( !IsValidEnchant() )
    {
        return;
    }

    // �ʿ��� ���� �ֳ�?
    if( !HaveEnchantMaterials() )
    {
        TCHAR buf[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString( eST_ITEM_NOT_EXIST_COMPOSITE_MATERIALS, buf, sizeof( buf ) );

        GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, buf );
        return;
    }

    // ���� ����� �ִ°�?
    if( !HaveEnchantCost() )
    {
        TCHAR buf[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString( eST_NOT_ENOUGH_MONEY, buf, sizeof( buf ) );

        GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, buf );
        return;         
    }

    // �����þ� ����̰� �����þ��� ������ �ȳ� �˾�â�� ���
    const MATERIALS_FOR_LEVEL* const enchant_material_info = GetCurrentEnchantInfo();
    if (enchant_material_info == NULL)
    {
        return;
    }
    if ((enchant_material_info->GetDownGradeType() == 2) &&
        (m_extraItemPos == INVALID_SLOT_POS))
    {
        // 2193 �����þ��� ÷������ �ʾҽ��ϴ�. ��æƮ ���� �� �������� �Ҹ�� �� �ֽ��ϴ�. ��� �Ͻðڽ��ϱ�?
        enchant_popup_dialog_key_ = GlobalFunc::ConfirmMessage(2193, 0, Callback_SendPacketItemEnchantSyn);
        return;
    }

    // ���� ��Ŷ�� ������ �ʾ�����
    if (!BIT_CHECK(m_packetStatus, PACKET_STATUS_ENCHANT))
    {
        BYTE rateIdx = (BYTE)( m_enchantOpt | ProbabilityIdxToOpt( m_probabilityIdx ) );

        MSG_CG_ITEM_ENCHANT_SYN packet;
        packet.Init();
        packet.m_SuccessRateIndex = rateIdx;
        packet.m_TargetPos = m_targetPos;
        packet.m_NPCCode = g_HeroInput.GetClickedNpcCode();
        packet.down_guard_item_pos = m_extraItemPos;

        BOOL ret = GlobalFunc::SendPacket( CI_GAMESERVERIDX, &packet, sizeof( packet ) );

        if( ret )
        {
            BIT_ADD( m_packetStatus, PACKET_STATUS_ENCHANT );

            m_pendingTargetPos = m_targetPos;

            EnableDialog( FALSE );
        }
    }
}
//----------------------------------------------------------------------------
/** ��þƮ��Ŷ�� ������ ��ٸ������������� ����
*/
BOOL EnchantDialog::IsPendingEnchant(POSTYPE pSlotPos)
{
	if(BIT_CHECK( m_packetStatus, PACKET_STATUS_ENCHANT ) && pSlotPos==m_pendingTargetPos)
	{
		return TRUE;
	}
	return FALSE;
}
//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::ReTry( void )
{
    if( BIT_CHECK( m_packetStatus, PACKET_STATUS_ENCHANT ) )
    {
        return;
    }

    InitDialog();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::Exit( void )
{
    if( BIT_CHECK( m_packetStatus, PACKET_STATUS_ENCHANT ) )
    {
        return;
    }

    if (enchant_popup_dialog_key_ != 0)
    {
        // �˾�â�� ������ �˾�â ��
        uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (system_manager != NULL)
        {
            system_manager->Destroy_Sys_Confirm(enchant_popup_dialog_key_);
        }
        enchant_popup_dialog_key_ = 0;
    }

    ShowDialog( FALSE );
}

//----------------------------------------------------------------------------
/**
*/
CControlWZ* EnchantDialog::getControl( POSTYPE atPos )
{
    assert( atPos >= 0 && atPos < Dialog_Max );

    return GetControlWZ( m_wzId[atPos] );
}

//----------------------------------------------------------------------------
/**
*/
POSTYPE EnchantDialog::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0; i < Dialog_Max; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }

    return Dialog_Max;
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::InitUiControls( void )
{
    // ��æƮ ����(��/��/��)
    m_ctlEnchantLvl  = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_KIND );   
    assert( m_ctlEnchantLvl );

    // ������ �̸�
    m_ctlItemName = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_ITEM_NAME ); 
    assert( m_ctlItemName );

    // �ʿ� ��(����)
    m_ctlEnchantCost = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_REQUIRE_MONEY ); 
    assert( m_ctlEnchantCost );

    // ��� ��� ��ũ�� ��
    m_ctlMatListScroll = (CCtrlVScrollWZ*)getControl( ENCHANT_VSCR_SUB );
    assert( m_ctlMatListScroll );

    // ��� �̸�/��
    for( int i = 0; i < MAX_VISIBLE_MATERIAL_COUNT; ++i )
    {
        m_ctlMatName[i] = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_MAIN_SUB_MAT1 + i ); 
        m_ctlMatNum[i] = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_MAIN_SUB_MAT1_NUM + i ); 

        // mjk_080903 - ��ȹ���� ��� ���� ���ܴ޶�� ��û
        m_ctlMatNum[i]->ShowWindowWZ( WZ_HIDE );
    }


    m_ctlTargetTitle = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_TITLE2 );
    m_ctlExtraMatTitle = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_EXTRA_TITLE );
    m_ctlExtraMatName = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_MAIN_SUB_OPTION );

    m_ctlOptionMatName = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_MAIN_SUB_OPTION); 


    // Ȯ��(25%, 50%, 70%, 100%)
    for( int i = 0; i < ENCHANT_TAB_MAX; ++i )
    {
        m_ctlProbability[i] = (CCtrlButtonCheckWZ*)getControl( ENCHANT_CHK_25_PER + i ); 
    }

    m_DefaultTextColor = m_ctlExtraMatName->GetTextColor();
    m_InvalidTextColor = RGBA(141,141,141,255);

    m_TitleDefaultColor = m_ctlTargetTitle->GetTextColor();
    m_TitleTextColor1 = RGBA(0,156,255,255);
    m_TitleTextColor2 = RGBA(237,28,36,255);
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnShowWindow( BOOL show )
{
    if( show )
    {
        InitDialog();

        g_InterfaceManager.ShowWindow( InterfaceManager::DIALOG_EQUIP, TRUE );

        keyMsg msg;
        ZeroMemory( &msg, sizeof( msg ) );
        msg.dwType = eDoType_dialog_new_enchant;
        msg.wParam = InterfaceManager::DIALOG_ENCHANT;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack( msg );
    }
    else
    {
        g_InterfaceManager.ShowWindow( InterfaceManager::DIALOG_EQUIP, FALSE );

        g_KeyQueueManager.DeleteMsg( eDoType_dialog_new_enchant );

        m_SlotContainer[0].Clear();
        m_SlotContainer[1].Clear();
        m_SlotContainer[2].Clear();

        TargetItemRock(false);
        ExtraItemRock(false);
        m_targetPos = INVALID_SLOT_POS;
        m_extraItemPos = INVALID_SLOT_POS;
        
        if (enchant_popup_dialog_key_ != 0)
        {
            // �˾�â�� ������ �˾�â ��
            uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
            if (system_manager != NULL)
            {
                system_manager->Destroy_Sys_Confirm(enchant_popup_dialog_key_);
            }
            enchant_popup_dialog_key_ = 0;
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::InitAllInfos( void )
{
    m_SlotContainer[0].Clear();
    m_SlotContainer[1].Clear();
    m_SlotContainer[2].Clear();


    m_packetStatus = PACKET_STATUS_NONE;

    m_matsForLv = NULL;

    m_enchantLevel = ENCHANT_LOW;
    m_enchantCost = 0;

    // �κ��丮 �� ����
    TargetItemRock(false);
    ExtraItemRock(false);
    m_targetPos = INVALID_SLOT_POS;
    m_extraItemPos = INVALID_SLOT_POS;
    m_pendingTargetPos = -1;
    
    m_lockInfoMap.clear();
    m_matList.clear();

    if (enchant_popup_dialog_key_ != 0)
    {
        // �˾�â�� ������ �˾�â ��
        uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (system_manager != NULL)
        {
            system_manager->Destroy_Sys_Confirm(enchant_popup_dialog_key_);
        }
        enchant_popup_dialog_key_ = 0;
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::InitDialog( void )
{
    // ��� ���� �ʱ�ȭ
    InitAllInfos();

    TCHAR buf[INTERFACE_STRING_LENGTH];

    // Ÿ��Ʋ1
    if( CCtrlStaticWZ* text = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_TITLE ) )
    {
        g_InterfaceManager.GetInterfaceString( eST_ENCHANT_TEXT, buf, sizeof( buf ) );
        GlobalFunc::SetCaption( text, buf );
    }

    // Ÿ��Ʋ2
    if( CCtrlStaticWZ* text = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_TITLE2 ) )
    {
        // 721 : ��æƮ�� �������� �����ϼ���
        g_InterfaceManager.GetInterfaceString( 2194, buf, sizeof( buf ) );
        GlobalFunc::SetCaption( text, buf );
    }

    // Ÿ��Ʋ3
    if( CCtrlStaticWZ* text = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_EXTRA_TITLE ) )
    {
        // 721 : ��æƮ�� �������� �����ϼ���
        g_InterfaceManager.GetInterfaceString( 2197, buf, sizeof( buf ) );
        GlobalFunc::SetCaption( text, buf );
    }

    // ��æƮ ����(��/��/��)
    GlobalFunc::SetCaption( m_ctlEnchantLvl, _T( "" ) );

    // ������ �̸�
    GlobalFunc::SetCaption( m_ctlItemName, _T( "" ) );

    // �ʿ� ��(����)
    GlobalFunc::SetCaption( m_ctlEnchantCost, _T( "0" ) );
    m_ctlEnchantCost->SetTextColor( m_DefaultTextColor );

    // ��� ��� ��ũ�� ����/��ġ
    m_ctlMatListScroll->SetScrollRangeWZ( 0, 0 );
    m_ctlMatListScroll->SetScrollPosWZ( 0 );

    // ��� �̸�/��
    for( int i = 0; i < MAX_VISIBLE_MATERIAL_COUNT; ++i )
    {
        GlobalFunc::SetCaption( m_ctlMatName[i], _T( "" ) );
        GlobalFunc::SetCaption( m_ctlMatNum[i], _T( "" ) );
    }

    GlobalFunc::SetCaption( m_ctlOptionMatName, _T( "" ) );


    // Ȯ��(25%, 50%, 75%, 100%)
    for( int i = 0; i < ENCHANT_TAB_MAX; ++i )
    {
        m_ctlProbability[i]->SetCheckState( ( i == m_probabilityIdx ) );
    }

    m_ctlTargetTitle->SetTextColor( m_TitleDefaultColor );
    m_ctlExtraMatTitle->SetTextColor( m_TitleDefaultColor );
    m_ctlExtraMatName->SetTextColor( m_DefaultTextColor );
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::EnableDialog( BOOL enable )
{
    for( POSTYPE pos = ENCHANT_BTN_SLOT_TARGET; pos <= ENCHANT_CHK_100_PER; ++pos )
    {
        if( CControlWZ* ctl = getControl( pos ) )
        {
            ctl->EnableWindowWZ( enable == TRUE );
        }
    }
}

void EnchantDialog::SetTargetItem( int inventory_slot_index )
{
    if( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( inventory_slot_index ) )
    {
        TargetItemRock(false);
        m_targetPos = inventory_slot_index;
        TargetItemRock(true);

        m_SlotContainer[0].SetSlotItem(0, *item, getControl(ENCHANT_BTN_SLOT_TARGET));
        m_SlotContainer[0].GetSlotItem(0)->SetLock(false);
    }
    else
    {
        m_targetPos = INVALID_SLOT_POS;
        m_SlotContainer[0].RemoveSlot(0);
    }

    OnChangedEnchantItem();
}

void EnchantDialog::SetExtraItem( int inventory_slot_index )
{
    if( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( inventory_slot_index ) )
    {
        ExtraItemRock(false);
        m_extraItemPos = inventory_slot_index;
        ExtraItemRock(true);

        SCItemSlot copy_item = *item;
        copy_item.SetNum(1);
        copy_item.SetLock(false);

        m_SlotContainer[2].SetSlotItem( 0, copy_item, getControl( ENCHANT_BTN_SLOT_5 ) );
    }
    else
    {
        ExtraItemRock(false);
        m_extraItemPos = INVALID_SLOT_POS;
        m_SlotContainer[2].RemoveSlot(0);
    }

    OnChangedExtraItem();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::UpdateMouseOverObj( void )
{
    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;

    int mx = Mouse::Instance()->GetMouseX();
    int my = Mouse::Instance()->GetMouseY();

    for( POSTYPE pos = ENCHANT_BTN_SLOT_TARGET; pos <= ENCHANT_BTN_SLOT_5; ++pos )
    {
        if( CControlWZ * ctl = getControl( pos ) )
        {

            RECT rc = ctl->GetSizeRect();

            if( rc.left <= mx && rc.right >= mx && rc.top <= my && rc.bottom >= my)
            {
                int slotPos = ( pos - ENCHANT_BTN_SLOT_TARGET );

                if( pos == ENCHANT_BTN_SLOT_TARGET )
                {
                    if( SCItemSlot* item = m_SlotContainer[0].GetSlotItem(0) )
                    {
                        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
                        m_MouseOverObject.pSlot = item;
                    }
                }
                else if( pos == ENCHANT_BTN_SLOT_5 )
                {
                    if( SCItemSlot* item = m_SlotContainer[2].GetSlotItem(0) )
                    {
                        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
                        m_MouseOverObject.pSlot = item;
                    }
                }
                else if( pos < ENCHANT_BTN_SLOT_5 && pos >= ENCHANT_BTN_SLOT_2 )
                {
                    if( SCItemSlot* item = m_SlotContainer[1].GetSlotItem(pos - ENCHANT_BTN_SLOT_2 ) )
                    {
                        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
                        m_MouseOverObject.pSlot = item;
                    }
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::UpdateTooltip( void )
{
    uiToolTipMan* uiMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager( UIMAN_TOOLTIP );
    if (m_MouseOverObject.eMouseOverType == eMOUSE_OVER_SLOT)
    {
        if (uiMan)
        {
            assert( m_MouseOverObject.pSlot );
            uiMan->RegisterTooltipItem( m_MouseOverObject.pSlot, FALSE, kLimitStatCompositeEnchant );
			uiMan->SetTooltipDialogID(GetDialogKey());
        }

        CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
    }
#if defined(_GLOBAL) || defined(_RUSSIA)
    else if (m_ctlExtraMatTitle && uiMan)
    {
        RECT static_text_rect = m_ctlExtraMatTitle->GetSizeRect();
        int mouse_x = Mouse::Instance()->GetMouseX();
        int mouse_y = Mouse::Instance()->GetMouseY();
        if (mouse_x > static_text_rect.left && mouse_x < static_text_rect.right
            && mouse_y > static_text_rect.top && mouse_y < static_text_rect.bottom)
        {
            TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
            m_ctlExtraMatTitle->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH);
            uiMan->RegisterTooltip(string_buffer);
			uiMan->SetTooltipDialogID( GetDialogKey() );
        }
    }
#endif//_GLOBAL
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::RefreshDefEnchantInfo( void )
{
    assert( IsValidEnchant() );
    assert( m_enchantLevel >= ENCHANT_LOW && m_enchantLevel <= ENCHANT_HIGHEST );

    TCHAR buf[256];

    if( SCItemSlot* item = m_SlotContainer[0].GetSlotItem(0) )
    {
        // ������ �̸�
        g_InterfaceManager.GetItemName( item->GetCode(), buf, sizeof( buf ) );
        GlobalFunc::SetCaption( m_ctlItemName, buf );

        // ��æƮ ����(��/��/��)
        const int strId[4] = { eST_LOW_ENCHANT_TEXT, eST_MID_ENCHANT_TEXT, eST_HIGH_ENCHANT_TEXT, eST_HIGHEST_ENCHANT_TEXT };
        int idx = m_enchantLevel - ENCHANT_LOW;

        g_InterfaceManager.GetInterfaceString( strId[idx], buf, sizeof( buf ) );
        GlobalFunc::SetCaption( m_ctlEnchantLvl, buf );
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::RefreshEnchantMatInfo( void )
{
    assert( IsValidEnchant() );

    // ��ũ�� ����/��ġ (3�� ���Լ�)
    int size = m_matList.size() - MAX_VISIBLE_MATERIAL_COUNT;
    int rangeMax = max( 0, size );

    m_ctlMatListScroll->SetScrollRangeWZ( 0, rangeMax );
    m_ctlMatListScroll->SetScrollPosWZ( 0 );

    // ��� ��� ����
    RefreshEnchantMatList();

    // ��æƮ ��� ǥ��
    TCHAR buf[256], buf2[256];
    Sprintf( buf, _T("%I64u"), m_enchantCost );

    NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
    ::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, buf, &nFmt, buf2, sizeof( buf2 ) );

    GlobalFunc::SetCaption( m_ctlEnchantCost, buf2 );

    if( HaveEnchantCost() )
    {
        m_ctlEnchantCost->SetTextColor( m_DefaultTextColor );
    }
    else
    {
        m_ctlEnchantCost->SetTextColor( m_InvalidTextColor );
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::RefreshEnchantMatList( void )
{
    // ��� ��� ����
    m_SlotContainer[1].RemoveSlot(0);
    m_SlotContainer[1].RemoveSlot(1);
    m_SlotContainer[1].RemoveSlot(2);

    // ��� �̸�/�� ����
    for( int i = 0; i < MAX_VISIBLE_MATERIAL_COUNT; ++i )
    {
        GlobalFunc::SetCaption( m_ctlMatName[i], _T( "" ) );
        GlobalFunc::SetCaption( m_ctlMatNum[i], _T( "" ) );
    }



    // ��ũ�� ��ġ
    assert( m_ctlMatListScroll );
    int scrollPos = m_ctlMatListScroll->GetScrollPosWZ();

    // ��� ��� ��
    int listSize = m_matList.size();

    // ��� ���� ����ŭ �ݺ�
    for( int i = 0; i < MAX_VISIBLE_MATERIAL_COUNT; ++i )
    {
        int idx = scrollPos + i;
        if( idx >= listSize )
        {
            break;
        }

        // ���
        const sMATERIALS& mats = m_matList[idx];

        // ������ �ڵ�/��
        SLOTCODE itemCode = mats.m_MaterialItemCode;
        int itemNum = mats.m_MaterialItemNum;

        assert( itemCode != 0 );


        // �ӽ� ������ �����
        SCItemSlot tempSlot;
        tempSlot.SetCode( (SLOTCODE)itemCode );
        tempSlot.SetNum( itemNum );

        // ��� ���Կ� �߰�(����)
        m_SlotContainer[1].SetSlotItem( i, tempSlot, getControl(ENCHANT_BTN_SLOT_2+i) );

        // ��� �̸�
        TCHAR buf[256];
        g_InterfaceManager.GetItemName( itemCode, buf, sizeof( buf ) );
        GlobalFunc::SetCaption( m_ctlMatName[i], buf );

        // ��� ��
        Sprintf( buf, _T( "%d" ), itemNum );
        GlobalFunc::SetCaption( m_ctlMatNum[i], buf );


        // �ʿ� �������� �κ��� �ִ��� �˻�
        if( ItemManager::Instance()->IsExistItem( SI_INVENTORY, itemCode, itemNum ) )
        {
            m_SlotContainer[1].SetSlotRenderState_Active( i );
            m_ctlMatName[i]->SetTextColor( m_DefaultTextColor );
            m_ctlMatNum[i]->SetTextColor( m_DefaultTextColor );
        }
        else
        {
            m_SlotContainer[1].SetSlotRenderState_DeActive( i );
            m_ctlMatName[i]->SetTextColor( m_InvalidTextColor );
            m_ctlMatNum[i]->SetTextColor( m_InvalidTextColor );
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
BOOL EnchantDialog::HaveEnchantMaterials( void )
{
    MaterialList::iterator it = m_matList.begin();
    for( ; it != m_matList.end(); ++it )
    {
        if( !ItemManager::Instance()->IsExistItem( SI_INVENTORY, it->m_MaterialItemCode, it->m_MaterialItemNum ) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL EnchantDialog::HaveEnchantCost( void )
{
    return ( g_pHero->GetMoney() >= m_enchantCost );
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::RemoveEnchantMatsFromInven( void )
{
    // ���� �Һ�
    MONEY money = g_pHero->GetMoney() - m_enchantCost;
    assert( money >= 0 );

    g_pHero->SetMoney( money );

    // ��� �Һ�
    MaterialList::iterator it = m_matList.begin();
    for( ; it != m_matList.end(); ++it )
    {
        BOOL ret = ItemManager::Instance()->SpendItem( 
            SI_INVENTORY, it->m_MaterialItemCode, it->m_MaterialItemNum );

        assert( ret );
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::ActivateProbabilityTab( int index )
{
    assert( index >= 0 && index < ENCHANT_TAB_MAX );

    for( int i = 0; i < ENCHANT_TAB_MAX; ++i )
    {
        m_ctlProbability[i]->SetCheckState( ( i == index ) );
    }

    m_probabilityIdx = index;

    if( IsValidEnchant() )
    {
        // ��æƮ ��� �缳��
        ResetEnchantMatList();

        // ��æƮ ��� ����
        RecalcEnchantCost();

        // ��æƮ ��� ���� ����
        RefreshEnchantMatInfo();
    }
}

//----------------------------------------------------------------------------
/**
*/
int EnchantDialog::GetValidProbabilityIdx( int defIdx )
{
    assert( defIdx >= 0 && defIdx < 4 );

    if( SCItemSlot* item = GetTargetItem() )
    {
        if( const BASE_ITEMINFO* itemInfo = item->GetItemInfo() )
        {

            // ������ Ÿ�Ժ� ó��
            switch( (eITEMSUBTYPE)itemInfo->m_wSubType )
            {
            case eITEMSUBTYPE_ELITE_SSQ:
            case eITEMSUBTYPE_ELITE:    // ����Ʈ �������̸� ������ 50%
                return 1;

            case eITEMSUBTYPE_UNIQUE:   // ����ũ �������̸� ������ 25%
                return 0;
            }

            // ��æƮ ������ ó��
            int newEnchant = item->GetEnchant() +1;
            assert( newEnchant <= MAX_ENCHANT_GRADE );

            // 6��æ������ ������ 100%
            if( newEnchant <= 6 )
            {
                return 3;
            }

            // ĳ�� ��æ�� �ƴ� ���
            if( !IsCashEnchant() )
            {
                int maxIdx = 3;

                if( newEnchant >= 7 && newEnchant < 10 )
                {
                    // 7 ~ 9�� �ִ� 75%
                    maxIdx = 2;
                }
                else if( newEnchant >= 10 )
                {
                    // 10��æ �̻��� �ִ� 50%
                    maxIdx = 1;
                }

                // ����Ʈ ���� �ִ밪���� ū ���
                if( defIdx > maxIdx )
                {
                    // ���� ���� �ִ밪 �� ���� ������ ����
                    return max( 0, min( maxIdx, m_probabilityIdx ) );
                }
            }
        }
    }

    return defIdx;
}

//----------------------------------------------------------------------------
/**
*/
eENCHANT_OPTION EnchantDialog::ProbabilityIdxToOpt( int idx )
{
    assert( idx >= 0 && idx < 4 );

    switch( idx )
    {
    case 0:  return eENCHANT_25PER;
    case 1:  return eENCHANT_50PER;
    case 2:  return eENCHANT_75PER;
    case 3:  return eENCHANT_100PER;
    default: return eENCHANT_NOT_OPT;
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::ResetEnchantLevel( void )
{
    if( SCItemSlot* item = GetTargetItem() )
    {
        int newEnchant = item->GetEnchant() + 1;
        assert( newEnchant <= MAX_ENCHANT_GRADE );

        switch( newEnchant )
        {
        case 7: case 8: case 9: 
            m_enchantLevel = ENCHANT_MIDDLE;
            break;

        case 10: case 11: case 12:
            m_enchantLevel = ENCHANT_HIGH;
            break;

        case 13: case 14: case 15:
            m_enchantLevel = ENCHANT_HIGHEST;
            break;
            
        default:
            m_enchantLevel = ENCHANT_LOW;
            break;
        }
    }
}

//----------------------------------------------------------------------------
void EnchantDialog::ResetEnchantMatsForLv( void )
{
    if( SCItemSlot* item = GetTargetItem() )
    {
        // ������ ������ ���
        if( const BASE_ITEMINFO* itemInfo = item->GetItemInfo() )
        {
            // ��æƮ ��� ������ ���
            m_matsForLv = EnchantParser::Instance()->GetEnchantMatInfo(
                itemInfo->m_LV, item->GetEnchant(), itemInfo->IsWeapon(), item->IsLimited(), m_enchantOpt);

            WzAssert( m_matsForLv );
        }
    }
}

//----------------------------------------------------------------------------
void EnchantDialog::ResetEnchantMatList( void )
{
    assert( IsValidEnchant() );
    assert( m_matsForLv );

    // ��� ����Ʈ �ʱ�ȭ
    m_matList.clear();

    // ��� ���� ����
    eENCHANT_OPTION opt = ProbabilityIdxToOpt( m_probabilityIdx );

	//safecode
	if(!m_matsForLv) return;

    const sFARE_RATIO* ratio = m_matsForLv->GetFareRatio( opt );
    assert( ratio );

    // �ִ� ��� ����ŭ �ݺ�
    for( int i = 0; i < MATERIALS_FOR_LEVEL::kMaxMaterialNumber; ++i )
    {
        // ���
        const sMATERIALS* mats = m_matsForLv->GetMaterials( i );
        if( mats )
        {
            WORD matNum = (WORD)( mats->m_MaterialItemNum * ratio->m_MaterialFare );

            // ��� ����Ʈ�� �߰�
            sMATERIALS mat;
            mat.m_MaterialItemCode = mats->m_MaterialItemCode;
            mat.m_MaterialItemNum = max( 1, matNum );

            m_matList.push_back( mat );
        }
    }
}

//----------------------------------------------------------------------------
// 
void EnchantDialog::ResetEnchantExtraMaterialSlot()
{
    if( SCItemSlot* item = GetTargetItem() )
    {
        if( const MATERIALS_FOR_LEVEL* const enchant_material_info = GetCurrentEnchantInfo() )
        {
            if( enchant_material_info->GetDownGradeType() == 0 )
            {
                m_SlotContainer[2].RemoveSlot(0);
                GlobalFunc::SetCaption( m_ctlExtraMatTitle, g_InterfaceManager.GetInterfaceString(2197));
                GlobalFunc::SetCaption( m_ctlTargetTitle, g_InterfaceManager.GetInterfaceString(2194));

                m_ctlTargetTitle->SetTextColor( m_TitleDefaultColor );
                m_ctlExtraMatTitle->SetTextColor( m_TitleDefaultColor );
                m_ctlExtraMatName->SetTextColor( m_DefaultTextColor );
            }
            else if( enchant_material_info->GetDownGradeType() == 1 )
            {
                GlobalFunc::SetCaption( m_ctlExtraMatTitle, g_InterfaceManager.GetInterfaceString(2198));

                SCItemSlot tempSlot;
                tempSlot.SetCode( (SLOTCODE)63191 );
                tempSlot.SetNum( 1 );
                m_SlotContainer[2].SetSlotItem(0, tempSlot, getControl(ENCHANT_BTN_SLOT_5) );
                m_SlotContainer[2].SetSlotRenderState_DeActive(0);

                GlobalFunc::SetCaption( m_ctlExtraMatName, g_InterfaceManager.GetItemName(63191) );
                GlobalFunc::SetCaption( m_ctlTargetTitle, g_InterfaceManager.GetInterfaceString(2127));

                m_ctlTargetTitle->SetTextColor( m_TitleTextColor1 );
                m_ctlExtraMatTitle->SetTextColor( m_TitleTextColor1 );
                m_ctlExtraMatName->SetTextColor( m_InvalidTextColor );
            }
            else if( enchant_material_info->GetDownGradeType() == 2 )
            {
                GlobalFunc::SetCaption( m_ctlExtraMatTitle, g_InterfaceManager.GetInterfaceString(2199));

                SCItemSlot tempSlot;
                tempSlot.SetCode( (SLOTCODE)63192 );
                tempSlot.SetNum( 1 );
                m_SlotContainer[2].SetSlotItem(0, tempSlot, getControl(ENCHANT_BTN_SLOT_5) );
                m_SlotContainer[2].SetSlotRenderState_DeActive(0);

                GlobalFunc::SetCaption( m_ctlExtraMatName, g_InterfaceManager.GetItemName(63192) );
                GlobalFunc::SetCaption( m_ctlTargetTitle, g_InterfaceManager.GetInterfaceString(2128));

                m_ctlTargetTitle->SetTextColor( m_TitleTextColor2 );
                m_ctlExtraMatTitle->SetTextColor( m_TitleTextColor2 );
                m_ctlExtraMatName->SetTextColor( m_InvalidTextColor );
            }
        }
    }
    else
    {
        m_SlotContainer[2].RemoveSlot(0);
        GlobalFunc::SetCaption( m_ctlExtraMatTitle, g_InterfaceManager.GetInterfaceString(2197));
        GlobalFunc::SetCaption( m_ctlExtraMatName, "" );

        m_ctlTargetTitle->SetTextColor( m_TitleDefaultColor );
        m_ctlExtraMatTitle->SetTextColor( m_TitleDefaultColor );
        m_ctlExtraMatName->SetTextColor( m_DefaultTextColor );
    }
}

//----------------------------------------------------------------------------
void EnchantDialog::RecalcEnchantCost( void )
{
    if( m_matsForLv == NULL ) 
    {
        return;
    }

    if( SCItemSlot* item = GetTargetItem() )
    {
        if( const BASE_ITEMINFO* item_info = item->GetItemInfo() )
        {
            eENCHANT_OPTION opt = ProbabilityIdxToOpt( m_probabilityIdx );
            if( const sFARE_RATIO* ratio = m_matsForLv->GetFareRatio( opt ) )
            {
                // ���� ���
                {
                    // *���� ��ȯ ����� �Ͻ������� ��������� ����
                    const double temp_haim_fare = ratio->m_HaimFare * 100.f;

                    const int new_haim_fare = util::LL_Round(temp_haim_fare);

#ifdef _NA_006589_20120415_ENCHANT_PRICE_MODIFY
                    // ���� ���
                    LEVELTYPE touch_item_level = item_info->m_LV;
                    if (touch_item_level > 22) {
                        touch_item_level = 23;
                    }
                    else if (touch_item_level > 12) {
                        touch_item_level = 13;
                    }
                    else {
                        touch_item_level = 3;
                    }
                    int64_t calc_fare = item->GetPriceForUpGradeFare() * touch_item_level * new_haim_fare;
#else
                    int64_t calc_fare = item->GetPriceForUpGradeFare() * item_info->m_LV * new_haim_fare;
#endif //_NA_006589_20120415_ENCHANT_PRICE_MODIFY
                    m_enchantCost = calc_fare / 100;

                    // �ݿø�
                    calc_fare = calc_fare % 100;
                    if( calc_fare >= 50 )
                        m_enchantCost = m_enchantCost + 1;

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                    if (g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_ENCHANT_COST_RATIO) != 0)
                    {
                        int ratio = g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_ENCHANT_COST_RATIO);
                        MONEY ratio_result_value = m_enchantCost * abs(ratio) / 100;
                        ratio > 0 ? m_enchantCost += ratio_result_value : m_enchantCost -= ratio_result_value;
                    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                }

#ifdef _NA_20100307_BETAKEY_SYSTEM
                if( uiLoginMan* login_manager = static_cast<uiLoginMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN)) )
                {
                    if (m_matsForLv->GetBetaKeyInfo())
                    {
                        m_enchantCost = BetaKeyImpl::GetEnchantMoney(login_manager->GetBetaKeyInfo(), m_matsForLv->GetBetaKeyInfo(), m_enchantCost);
                    }
                }
#endif//_NA_20100307_BETAKEY_SYSTEM
            }
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnLButtonClick( SI_MESSAGE* pMsg )
{
    POSTYPE pos = getCtrlIDToPos( pMsg->dwCtrlID );

    if( pos == ENCHANT_BTN_SLOT_TARGET || pos == ENCHANT_BTN_SLOT_5 )
    {
        OnEnchantItemSlotClick();
    }
    else if( pos >= ENCHANT_BTN_EXIT && pos <= ENCHANT_BTN_CANCEL ) 
    {
        OnBtnClick( pos );
    }
    else if( pos >= ENCHANT_CHK_25_PER && pos <= ENCHANT_CHK_100_PER )
    {
        OnProbabilityTabClick( pos );
    }    
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnRButtonClick( SI_MESSAGE* pMsg )
{
    if (enchant_popup_dialog_key_ != 0)
    {
        // �˾�â�� ���������� �������� ������ ����
        return;
    }

    POSTYPE pos = getCtrlIDToPos( pMsg->dwCtrlID );

    if( pos == ENCHANT_BTN_SLOT_TARGET )
    {
        ReTry();
    }
    else if( pos == ENCHANT_BTN_SLOT_5 )
    {
        SetExtraItem( INVALID_SLOT_POS );
    }
}
//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnEnchantItemSlotClick( void )
{
    if( MouseHandler::Instance()->IsExistItemAtHand() )
    {
        MouseHandler::MouseItem* mouse_item = MouseHandler::Instance()->GetMouseItem();

        OnEnchantItem_InventoryRightClick( mouse_item->m_fromSlotIdx, mouse_item->m_fromPos );

        MouseHandler::Instance()->RollbackItemAtHand();
    }
}

//------------------------------------------------------------------------------
void EnchantDialog::OnChangedEnchantItem(void)
{
    if (IsValidEnchant() == FALSE)
    {
        return;
    }


    // ��æƮ ���� �缳��
    ResetEnchantLevel();

    // ��æƮ ��� �缳��
    ResetEnchantMatsForLv();

    // �⺻ ���� ����
    RefreshDefEnchantInfo();

    // Ȯ���� Ȱ��
#ifdef RESERVE_PREV_PROBABILITY_IDX
    // nothing to do
#else
    m_probabilityIdx = 3;
#endif

    ActivateProbabilityTab( GetValidProbabilityIdx(m_probabilityIdx) );

    ResetEnchantExtraMaterialSlot();
}

void EnchantDialog::OnChangedExtraItem(void)
{
    if( SCItemSlot* item = GetExteraItem() )
    {
        // ������ �̸�
        TCHAR buf[256];
        g_InterfaceManager.GetItemName( item->GetCode(), buf, sizeof( buf ) );
        GlobalFunc::SetCaption( m_ctlExtraMatName, buf );

        // ������ �̸��� ������ �⺻ ��������
        m_ctlExtraMatName->SetTextColor( m_DefaultTextColor );
    }
    else
    {
        // ������ �̸��� ������ �⺻ ��������
        m_ctlExtraMatName->SetTextColor( m_InvalidTextColor );
    }
}

//----------------------------------------------------------------------------
void EnchantDialog::OnEnchantItem_InventoryRightClick(const SLOTIDX from_container_index, 
                                                      const POSTYPE from_container_position)
{
    if (enchant_popup_dialog_key_ != 0)
    {
        // �˾�â�� ���������� �������� �ø��� ����
        return;
    }

    if( MouseHandler::Instance()->ItemCompositeNewEnchant_InventoryRightClick(
        from_container_index, SI_ENCHANT, from_container_position, 0) )
    {
        SetTargetItem( from_container_position );
    }
    else if( MouseHandler::Instance()->ItemCompositeNewEnchant_InventoryRightClick(from_container_index,
        SI_ENCHANT_OPTION, from_container_position, 0 ) )
    {
        SetExtraItem( from_container_position );
    }
}
//------------------------------------------------------------------------------


//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnProbabilityTabClick( POSTYPE pos )
{
    assert( pos >= ENCHANT_CHK_25_PER && pos <= ENCHANT_CHK_100_PER );

    int idx = ( pos - ENCHANT_CHK_25_PER );

    if( IsValidEnchant() )
    {
        idx = GetValidProbabilityIdx( idx );
    }

    ActivateProbabilityTab( idx );
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnBtnClick( POSTYPE pos )
{
    switch( pos )
    {
    case ENCHANT_BTN_ENCHANT:
        Enchant();
        break;

    case ENCHANT_BTN_RETRY:
        ReTry();
        break;

    case ENCHANT_BTN_EXIT:
    case ENCHANT_BTN_CANCEL:
        Exit();
        break;
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnEnchantSuccessAck( MSG_BASE* pMsg )
{
    assert( pMsg );

    MSG_CG_ITEM_ENCHANT_SUCCESS_ACK* pRecvMsg = (MSG_CG_ITEM_ENCHANT_SUCCESS_ACK*)pMsg;

    BIT_REMOVE( m_packetStatus, PACKET_STATUS_ENCHANT );

    // �κ����� ��æƮ ��� ����
    RemoveEnchantMatsFromInven();

    // ��æƮ â ������
    if( SCItemSlot* target_item = GetTargetItem() )
    {
        target_item->SetEnchant( target_item->GetEnchant() + 1 );
    }

    // ���� �� ������
    if( SCSlotContainer* oriContainer = ItemManager::Instance()->GetContainer( SI_INVENTORY ) )
    {
        SCItemSlot& oriSlot = (SCItemSlot&)oriContainer->GetSlot( m_pendingTargetPos );

        if( const BASE_ITEMINFO* itemInfo = oriSlot.GetItemInfo() )
        {
            // �޽��� ���
            TCHAR itemName[128];
            TCHAR szMessage[MAX_MESSAGE_LENGTH];

            GameFunc::GetItemName(&oriSlot, itemInfo, itemName, 128);

            // [%s] �������� ��þƮ�� ���� �Ͽ����ϴ�.
            g_InterfaceManager.GetInterfaceString( eST_SUCCESS_ITEM_ENCHANT, szMessage, sizeof( szMessage ) );

            GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage, itemName );

            // ȭ�� ��/�߾� ǥ��
            GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage, itemName );
        }
    }

    // ���� ���
    g_SoundEffect.PlayUI( UI_SOUND_COMPOSE_ENCHANT_SUCCESS );

    // ��ȭ���� Ȱ��/�ʱ�ȭ
    EnableDialog( TRUE );

    InitDialog();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnEnchantFailedAck( MSG_BASE* pMsg )
{
    assert( pMsg );

    MSG_CG_ITEM_ENCHANT_FAILED_ACK* pRecvMsg = (MSG_CG_ITEM_ENCHANT_FAILED_ACK*)pMsg;

    BIT_REMOVE( m_packetStatus, PACKET_STATUS_ENCHANT );

    // �κ����� ��æƮ ��� ����
    RemoveEnchantMatsFromInven();

    TCHAR szMessage[MAX_MESSAGE_LENGTH];

    // ��� �ڵ庰 ó��
    if( pRecvMsg->m_byErrorCode == RC_ITEM_ENCHANT_FAILED_AND_ENCHNAT_DOWN )
    {
        // ��æƮ ���� �ٿ� ����
        int enchant_fail_down_level = 1;
        if (m_matsForLv)
        {
            enchant_fail_down_level = m_matsForLv->GetDownLV();
        }

        // ��æƮ â ������
        if( SCItemSlot* target_item = GetTargetItem() )
        {
            target_item->SetEnchant( target_item->GetEnchant() - enchant_fail_down_level );
        }

        // ���� �� ������
        if( SCSlotContainer* oriContainer = ItemManager::Instance()->GetContainer( SI_INVENTORY ) )
        {
            SCItemSlot& oriSlot = (SCItemSlot&)oriContainer->GetSlot( m_pendingTargetPos );
            //oriSlot.SetEnchant( oriSlot.GetEnchant() - enchant_fail_down_level );

            if( const BASE_ITEMINFO* itemInfo = oriSlot.GetItemInfo() )
            {
                // �޽��� ���
                TCHAR szItemName[128], szItemName2[128], buf1[64], buf2[64];
                TCHAR szTempMessage[MAX_MESSAGE_LENGTH] = {0,};

                g_InterfaceManager.GetInterfaceString( itemInfo->m_NCode, buf1, sizeof( buf1 ) );
                g_InterfaceManager.GetInterfaceString( itemInfo->m_SCNCode, buf2, sizeof( buf2 ) );

                Sprintf( szItemName, "%s %s %+d", buf1, buf2, oriSlot.GetEnchant() + enchant_fail_down_level );
                Sprintf( szItemName2, "%s %s %+d", buf1, buf2, oriSlot.GetEnchant());

                // 5318 ��æƮ�� �����Ͽ�, %s�� %s�� �ٿ�Ǿ����ϴ�.
                g_InterfaceManager.GetInterfaceString( 5318, szTempMessage );
                Sprintf( szMessage, szTempMessage, szItemName, szItemName2 );
            }
        }
    }
	else if (pRecvMsg->m_byErrorCode == RC_ITEM_ENCHANT_FAILED_AND_DESTROY)
	{
		if( SCSlotContainer* oriContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY) )
        {
		    SCItemSlot& oriSlot = (SCItemSlot&)oriContainer->GetSlot(m_pendingTargetPos);

		    TCHAR szTemp1[64]	  = {0,};
		    TCHAR szTemp2[64]	  = {0,};
		    TCHAR szItemName[256] = {0,};
		    g_InterfaceManager.GetInterfaceString(oriSlot.GetItemInfo()->m_NCode, szTemp1, 63);
		    g_InterfaceManager.GetInterfaceString(oriSlot.GetItemInfo()->m_SCNCode, szTemp2, 63);
		    Snprintf(szItemName, 255, "%s %s +%d", szTemp1, szTemp2, oriSlot.GetEnchant());

		    oriContainer->DeleteSlot(m_pendingTargetPos, NULL);

		    // 5319 ��æƮ�� ���� �Ͽ�, %s�� �Ҹ� �Ǿ����ϴ�.
		    TCHAR szTempMessage[MAX_MESSAGE_LENGTH] = { '\0', };
		    g_InterfaceManager.GetInterfaceString(5319, szTempMessage);
		    Snprintf(szMessage, INTERFACE_STRING_LENGTH - 1, szTempMessage, szItemName);	
        }
	}
    else
    {
        // ��þƮ�� ���� �Ͽ����ϴ�.
        g_InterfaceManager.GetInterfaceString(eST_FAILED_ITEM_ENCHANT, szMessage);
    }

    // �޽��� ���
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    if (pRecvMsg->m_byErrorCode == RC_ITEM_ENCHANT_FAILED_AND_PREVENT)
    {
        TCHAR guild_facility_message[INTERFACE_STRING_LENGTH] = {0, };
        // 7100 : ���ü� ȿ���� �ߵ��Ǿ� ��æƮ �ٿ�(�Ҹ�)�� �߻����� �ʾҽ��ϴ�.
        g_InterfaceManager.GetInterfaceString(7100, guild_facility_message, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, guild_facility_message);
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    // ȭ�� ��/�߾� ǥ��
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);

    // ���� ���
    g_SoundEffect.PlayUI(UI_SOUND_COMPOSE_ENCHANT_FAIL);

    // ��ȭ���� Ȱ��/�ʱ�ȭ
    EnableDialog(TRUE);

    InitDialog();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnItemEnchantNak( MSG_BASE* pMsg )
{
    assert( pMsg );

    MSG_CG_ITEM_ENCHANT_NAK* pRecvMsg = (MSG_CG_ITEM_ENCHANT_NAK*)pMsg;

    BIT_REMOVE( m_packetStatus, PACKET_STATUS_ENCHANT );

    ItemManager::Instance()->NoticeReasonItemErrorCode( pRecvMsg->m_byErrorCode );

    EnableDialog( TRUE );

    InitDialog();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::TargetItemRock(bool is_rock)
{
    if (m_targetPos == INVALID_SLOT_POS)
    {
        return;
    }

    SCItemSlot* item_slot = ItemManager::Instance()->GetInventoryItem(m_targetPos);
    if (item_slot != NULL)
    {
        item_slot->SetLock(is_rock);
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::ExtraItemRock(bool is_rock)
{
    if (m_extraItemPos == INVALID_SLOT_POS)
    {
        return;
    }

    SCItemSlot* item_slot = ItemManager::Instance()->GetInventoryItem(m_extraItemPos);
    if (item_slot != NULL)
    {
        item_slot->SetLock(is_rock);
        m_SlotContainer[2].SetSlotRenderState_Active(0);
    }
}

void EnchantDialog::Callback_SendPacketItemEnchantSyn(uiSystemMessage::ButtonIndex button_index)
{
    EnchantDialog* enchant_dialog =
        static_cast<EnchantDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENCHANT));

    if (enchant_dialog)
    {
        enchant_dialog->SendPacketItemEnchantSyn(button_index == uiSystemMessage::kConfirm_Button);
    }
}

void EnchantDialog::SendPacketItemEnchantSyn(bool is_send_packet)
{
    // �˾� â�� ���� ����
    enchant_popup_dialog_key_ = 0;

    if (is_send_packet == false)
    {
        return;
    }

    // �Ʒ��� ��æƮ üũ ���� ��Ȯ�� �� ��Ŷ ����

    // ��ȿ�� ��æƮ�ΰ�?
    if( !IsValidEnchant() )
    {
        return;
    }

    // �ʿ��� ���� �ֳ�?
    if( !HaveEnchantMaterials() )
    {
        TCHAR buf[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString( eST_ITEM_NOT_EXIST_COMPOSITE_MATERIALS, buf, sizeof( buf ) );

        GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, buf );
        return;
    }

    // ���� ����� �ִ°�?
    if( !HaveEnchantCost() )
    {
        TCHAR buf[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString( eST_NOT_ENOUGH_MONEY, buf, sizeof( buf ) );

        GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, buf );
        return;
    }

    // ���� ��Ŷ�� ������ �ʾ�����
    if (!BIT_CHECK(m_packetStatus, PACKET_STATUS_ENCHANT))
    {
        BYTE rateIdx = (BYTE)( m_enchantOpt | ProbabilityIdxToOpt( m_probabilityIdx ) );

        MSG_CG_ITEM_ENCHANT_SYN packet;
        packet.Init();
        packet.m_SuccessRateIndex = rateIdx;
        packet.m_TargetPos = m_targetPos;
        packet.m_NPCCode = g_HeroInput.GetClickedNpcCode();
        packet.down_guard_item_pos = m_extraItemPos;

        BOOL ret = GlobalFunc::SendPacket( CI_GAMESERVERIDX, &packet, sizeof( packet ) );

        if( ret )
        {
            BIT_ADD( m_packetStatus, PACKET_STATUS_ENCHANT );

            m_pendingTargetPos = m_targetPos;

            EnableDialog( FALSE );
        }
    }
}
#endif // _NA_003966_20111227_ADD_ENCHANT