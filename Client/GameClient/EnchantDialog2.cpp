#include "SunClientPrecompiledHeader.h"

#ifdef _NA_003966_20111227_ADD_ENCHANT
#else

#ifdef __NA_001358_ENCHANT_UNIFICATION
// no operation
#else //__NA_001358_ENCHANT_UNIFICATION

#include "enchantdialog2.h"
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
    StrToWzID("B000"),//ENCHANT_BTN_SLOT_1 = 0,
    StrToWzID("B001"),//ENCHANT_BTN_SLOT_2,
    StrToWzID("B002"),//ENCHANT_BTN_SLOT_3, 
    StrToWzID("B003"),//ENCHANT_BTN_SLOT_4, 
    StrToWzID("B004"),//ENCHANT_BTN_SLOT_5, 
    StrToWzID("B999"),//ENCHANT_BTN_EXIT
    StrToWzID("TI01"),//ENCHANT_BTN_ENCHANT,
    StrToWzID("TI02"),//ENCHANT_BTN_RETRY,
    StrToWzID("TI03"),//ENCHANT_BTN_CANCEL,
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
    StrToWzID("T008"),//ENCHANT_TXT_MAIN_SUB_MAT4,
    StrToWzID("T012"),//ENCHANT_TXT_MAIN_SUB_MAT1_NUM,
    StrToWzID("T013"),//ENCHANT_TXT_MAIN_SUB_MAT2_NUM,
    StrToWzID("T014"),//ENCHANT_TXT_MAIN_SUB_MAT3_NUM,
    StrToWzID("T015"),//ENCHANT_TXT_MAIN_SUB_MAT4_NUM,
    StrToWzID("T009"),//ENCHANT_TXT_REQUIRE_MONEY,
    StrToWzID("VS00"),//ENCHANT_VSCR_SUB,
    StrToWzID("T002"),//ENCHANT_TXT_TITLE2,
};

//----------------------------------------------------------------------------
/**
*/
EnchantDialog::EnchantDialog( void )
: m_itemRenderer( NULL )
, m_targetPos( -1 )
, m_pendingTargetPos( -1 )
, m_matsForLv( NULL )
, m_enchantLevel( ENCHANT_LOW )
, m_enchantCost( 0 )
, m_enchantOpt( eENCHANT_NOT_OPT )
, m_probabilityIdx( 3 )
{
    // 슬롯 5개(아이템1 + 재료4)
    SCItemSlotContainer::Init( 5, SI_ENCHANT );

    m_lockInfoMap.clear();
    m_matList.clear();

    m_packetStatus = PACKET_STATUS_NONE;
}

//----------------------------------------------------------------------------
/**
*/
EnchantDialog::~EnchantDialog( void )
{
    SCItemSlotContainer::Release();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::Init( CDrawBase* /*pDrawBase*/ )
{
    CreateItemRenderer();

    InitUiControls();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::Release( void )
{
    DeleteAllSlots();

    ReleaseItemRenderer();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::update( void )
{
    SUN_ASSERT(m_itemRenderer);

    if (m_itemRenderer)
    {
        m_itemRenderer->Update();
    }

    UpdateMouseOverObj();

    UpdateTooltip();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::render( void )
{
    SUN_ASSERT(m_itemRenderer);

    if (m_itemRenderer)
    {
        m_itemRenderer->Render();
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::NetworkProc( MSG_BASE* pMsg )
{
    assert( pMsg );

    if( pMsg->m_byCategory == CG_ITEM )
    {
        switch( pMsg->m_byProtocol )
        {
        case CG_ITEM_ENCHANT_SUCCESS_ACK:   // 인첸트 성공
            OnEnchantSuccessAck( pMsg );
            break;

        case CG_ITEM_ENCHANT_FAILED_ACK:    // 인첸트 실패
            OnEnchantFailedAck( pMsg );
            break;

        case CG_ITEM_ENCHANT_NAK:           // 인첸트 시도 실패
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
    assert( pMsg );

    switch( pMsg->eResultMsg )
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick( pMsg );
        break;

    case RT_MSG_SB_PAGETOP:     // 윗쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINETOP:     // 윗쪽 화살표를 눌렀다.
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

    case RT_MSG_SB_PAGEBOTTOM:  // 윗쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINEBOTTOM:  // 아랫쪽 화살표를 눌렀다
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

    case RT_MSG_SB_THUMBTRACK:      // 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   // 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       // 스크롤이 종료 되었다.
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
    // 유효한 인챈트인가?
    if( !IsValidEnchant() )
    {
        return;
    }

    // 필요한 재료는 있나?
    if( !HaveEnchantMaterials() )
    {
        TCHAR buf[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString( eST_ITEM_NOT_EXIST_COMPOSITE_MATERIALS, buf, sizeof( buf ) );

        GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, buf );
        return;
    }

    // 돈은 충분히 있는가?
    if( !HaveEnchantCost() )
    {
        TCHAR buf[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString( eST_NOT_ENOUGH_MONEY, buf, sizeof( buf ) );

        GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, buf );
        return;         
    }

    // 아직 패킷을 날리지 않았으면
    if( !BIT_CHECK( m_packetStatus, PACKET_STATUS_ENCHANT ) )
    {
        BYTE rateIdx = (BYTE)( m_enchantOpt | ProbabilityIdxToOpt( m_probabilityIdx ) );

        MSG_CG_ITEM_ENCHANT_SYN packet;
        packet.m_SuccessRateIndex = rateIdx;
        packet.m_TargetPos = m_targetPos;
        packet.m_NPCCode = g_HeroInput.GetClickedNpcCode();

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
/** 인첸트패킷을 보내서 기다리는중인지여부 묻기
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
void EnchantDialog::CreateItemRenderer( void )
{
    SUN_ASSERT(!m_itemRenderer);

    m_itemRenderer = new ItemUnitRenderer;
    SUN_ASSERT(m_itemRenderer);

    SUN_ASSERT(g_pSunRenderer);
    if (m_itemRenderer)
    {
        m_itemRenderer->Init(g_pSunRenderer, GetMaxSlotNum());
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::ReleaseItemRenderer( void )
{
    SAFE_RELEASENDELETE( m_itemRenderer );
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::InitUiControls( void )
{
    // 인챈트 레벨(상/중/하)
    m_ctlEnchantLvl  = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_KIND );   
    assert( m_ctlEnchantLvl );

    // 아이템 이름
    m_ctlItemName = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_ITEM_NAME ); 
    assert( m_ctlItemName );

    // 필요 돈(하임)
    m_ctlEnchantCost = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_REQUIRE_MONEY ); 
    assert( m_ctlEnchantCost );

    // 재료 목록 스크롤 바
    m_ctlMatListScroll = (CCtrlVScrollWZ*)getControl( ENCHANT_VSCR_SUB );
    assert( m_ctlMatListScroll );

    // 재료 이름/수
    for( int i = 0; i < 4; ++i )
    {
        m_ctlMatName[i] = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_MAIN_SUB_MAT1 + i ); 
        assert( m_ctlMatName[i] );

        m_ctlMatNum[i] = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_MAIN_SUB_MAT1_NUM + i ); 
        assert( m_ctlMatNum[i] );

        // mjk_080903 - 기획에서 재료 수는 숨겨달라고 요청
        m_ctlMatNum[i]->ShowWindowWZ( WZ_HIDE );
    }

    // 확률(25%, 50%, 70%, 100%)
    for( i = 0; i < 4; ++i )
    {
        m_ctlProbability[i] = (CCtrlButtonCheckWZ*)getControl( ENCHANT_CHK_25_PER + i ); 
        assert( m_ctlProbability[i] );
    }
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

        DeleteAllSlots();
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::InitAllInfos( void )
{
    DeleteAllSlots();

    m_packetStatus = PACKET_STATUS_NONE;

    m_matsForLv = NULL;

    m_enchantLevel = ENCHANT_LOW;
    m_enchantCost = 0;

    m_targetPos = -1;
    m_pendingTargetPos = -1;
    
    m_lockInfoMap.clear();
    m_matList.clear();
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::InitDialog( void )
{
    // 모든 정보 초기화
    InitAllInfos();

    TCHAR buf[INTERFACE_STRING_LENGTH];

    // 타이틀
    CCtrlStaticWZ* ctlTitle = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_TITLE );  
    assert( ctlTitle );

    g_InterfaceManager.GetInterfaceString( eST_ENCHANT_TEXT, buf, sizeof( buf ) );
    GlobalFunc::SetCaption( ctlTitle, buf );

    // 타이틀2
    CCtrlStaticWZ* ctlTitle2 = (CCtrlStaticWZ*)getControl( ENCHANT_TXT_TITLE2 );  
    assert( ctlTitle2 );

    // 721 : 인챈트할 아이템을 선택하세요
    g_InterfaceManager.GetInterfaceString( 721, buf, sizeof( buf ) );
    GlobalFunc::SetCaption( ctlTitle2, buf );

    // 인챈트 레벨(상/중/하)
    GlobalFunc::SetCaption( m_ctlEnchantLvl, _T( "" ) );

    // 아이템 이름
    GlobalFunc::SetCaption( m_ctlItemName, _T( "" ) );

    // 필요 돈(하임)
    GlobalFunc::SetCaption( m_ctlEnchantCost, _T( "0" ) );
    m_ctlEnchantCost->SetTextColor( RGBA( 255, 255, 255, 255 ) );

    // 재료 목록 스크롤 범위/위치
    m_ctlMatListScroll->SetScrollRangeWZ( 0, 0 );
    m_ctlMatListScroll->SetScrollPosWZ( 0 );

    // 재료 이름/수
    for( int i = 0; i < 4; ++i )
    {
        GlobalFunc::SetCaption( m_ctlMatName[i], _T( "" ) );
        GlobalFunc::SetCaption( m_ctlMatNum[i], _T( "" ) );
    }

    // 확률(25%, 50%, 75%, 100%)
    for( i = 0; i < 4; ++i )
    {
        m_ctlProbability[i]->SetCheckState( ( i == m_probabilityIdx ) );
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::EnableDialog( BOOL enable )
{
    bool enableWz = ( enable == TRUE );

    for( POSTYPE pos = ENCHANT_BTN_SLOT_1; pos <= ENCHANT_CHK_100_PER; ++pos )
    {
        CControlWZ* ctl = getControl( pos );
        assert( ctl );

        ctl->EnableWindowWZ( enableWz );
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::UpdateMouseOverObj( void )
{
    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;

    int mx = Mouse::Instance()->GetMouseX();
    int my = Mouse::Instance()->GetMouseY();

    for( POSTYPE pos = ENCHANT_BTN_SLOT_1; pos <= ENCHANT_BTN_SLOT_5; ++pos )
    {
        CControlWZ * ctl = getControl( pos );
        assert( ctl );

        RECT rc = ctl->GetSizeRect();

        if( rc.left <= mx && rc.right >= mx && rc.top <= my && rc.bottom >= my)
        {
            int slotPos = ( pos - ENCHANT_BTN_SLOT_1 );

            if( !IsEmpty( slotPos ) )
            {
                SCSlot& rSlot = GetSlot( slotPos );
                m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
                m_MouseOverObject.pSlot = &rSlot;

                return;
            }
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::UpdateTooltip( void )
{
    if( m_MouseOverObject.eMouseOverType == eMOUSE_OVER_SLOT )
    {
        uiToolTipMan* uiMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager( UIMAN_TOOLTIP );
        assert( uiMan );

        if( uiMan )
        {
            assert( m_MouseOverObject.pSlot );
            uiMan->RegisterTooltipItem( m_MouseOverObject.pSlot, FALSE, kLimitStatCompositeEnchant );
			uiMan->SetTooltipDialogID(GetDialogKey());
        }

        CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
    }
}

//----------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT EnchantDialog::InsertSlot( POSTYPE atPos, SCSlot& slot )
{
    assert( atPos >= 0 && atPos < GetMaxSlotNum() );

    RC::eSLOT_INSERT_RESULT ret = SCSlotContainer::InsertSlot( atPos, slot );

    if( ret == RC_INSERT_SLOT_SUCCESS )
    {
        // 인챈트할 아이템 슬롯에 넣는 경우
        if( atPos == 0 )
        {
            // 인벤토리에서 온다는 가정( SCSlot -> SCItemSlot 변환 가능)
            SCItemSlot& oriSlot = (SCItemSlot&)slot;
            oriSlot.SetLock( TRUE );

            m_lockInfoMap.insert( LockInfoMap::value_type( atPos, slot.GetPos() ) );
        }
        
        DBSERIAL serial = g_SlotKeyGenerator.GetKey();

        SCSlot& dstSlot = GetSlot( atPos );
        dstSlot.SetSerial( serial );

        CControlWZ* pCtrl = getControl( atPos );
        assert( pCtrl );

        RECT rc = pCtrl->GetSizeRect();

        assert(m_itemRenderer);
        m_itemRenderer->AddItemUnit( serial, dstSlot.GetCode(), &rc, &dstSlot, pCtrl );
    }

    return ret;
}

//----------------------------------------------------------------------------
/**
*/
BOOL EnchantDialog::DeleteSlot( POSTYPE atPos, SCSlot* slotOut )
{
    assert( atPos >= 0 && atPos < GetMaxSlotNum() );

    // 삭제할 슬롯을 얻고
    SCSlot& dstSlot = GetSlot( atPos );

    // 해당 슬롯의 키를 반납
    g_SlotKeyGenerator.Restore( (DWORD)dstSlot.GetSerial() );

    SUN_ASSERT(m_itemRenderer);
    if (m_itemRenderer)
    {
        m_itemRenderer->RemoveItemUnit(dstSlot.GetSerial());
    }

    LockInfoMap::iterator it = m_lockInfoMap.find( atPos );
    if( it != m_lockInfoMap.end() )
    {
        // 원본은 인벤토리라고 가정
        SCSlotContainer* oriContainer =  ItemManager::Instance()->GetContainer( SI_INVENTORY );
        assert( oriContainer );

        if( oriContainer )
        {
            POSTYPE oriPos = it->second;

            int numOriSlots = oriContainer->GetMaxSlotNum();
            assert( oriPos >= 0 && oriPos < numOriSlots );

            if( oriPos >= 0 && oriPos < numOriSlots )
            {
                SCItemSlot& oriSlot = (SCItemSlot&)oriContainer->GetSlot( oriPos );
                oriSlot.SetLock( FALSE );
            }           
        }       

        m_lockInfoMap.erase( it );
    }

    return SCSlotContainer::DeleteSlot( atPos, slotOut );
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::DeleteAllSlots( void )
{
    int numSlots = GetMaxSlotNum();

    for( int i = 0; i < numSlots; ++i )
    {
        if( !IsEmpty( i ) )
        {
            DeleteSlot( i, NULL );
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::RefreshDefEnchantInfo( void )
{
    assert( IsValidEnchant() );
    assert( m_enchantLevel >= ENCHANT_LOW && m_enchantLevel <= ENCHANT_HIGH );

    TCHAR buf[256];

    // 메인 슬롯(슬롯0)을 얻고
    SCItemSlot& slot = (SCItemSlot&)GetSlot( 0 );

    // 아이템 이름
    g_InterfaceManager.GetItemName( slot.GetCode(), buf, sizeof( buf ) );
    GlobalFunc::SetCaption( m_ctlItemName, buf );

    // 인챈트 종류(상/중/하)
    const int strId[3] = { eST_LOW_ENCHANT_TEXT, eST_MID_ENCHANT_TEXT, eST_HIGH_ENCHANT_TEXT };
    int idx = m_enchantLevel - ENCHANT_LOW;

    g_InterfaceManager.GetInterfaceString( strId[idx], buf, sizeof( buf ) );
    GlobalFunc::SetCaption( m_ctlEnchantLvl, buf );
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::RefreshEnchantMatInfo( void )
{
    assert( IsValidEnchant() );

    // 스크롤 범위/위치 (4는 슬롯수)
    int size = m_matList.size() - 4;
    int rangeMax = max( 0, size );

    m_ctlMatListScroll->SetScrollRangeWZ( 0, rangeMax );
    m_ctlMatListScroll->SetScrollPosWZ( 0 );

    // 재료 목록 갱신
    RefreshEnchantMatList();

    // 인챈트 비용 표시
    TCHAR buf[256], buf2[256];
    Sprintf( buf, _T("%I64u"), m_enchantCost );

    NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
    ::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, buf, &nFmt, buf2, sizeof( buf2 ) );

    GlobalFunc::SetCaption( m_ctlEnchantCost, buf2 );

    if( HaveEnchantCost() )
    {
        m_ctlEnchantCost->SetTextColor( RGBA( 255, 255, 255, 255 ) );
    }
    else
    {
        m_ctlEnchantCost->SetTextColor( RGBA( 255, 0, 0, 255 ) );
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::RefreshEnchantMatList( void )
{
    // 재료 목록 제거
    ClearEnchantMatList();

    // 스크롤 위치
    assert( m_ctlMatListScroll );
    int scrollPos = m_ctlMatListScroll->GetScrollPosWZ();

    // 재료 목록 수
    int listSize = m_matList.size();

    // 재료 슬롯 수만큼 반복
    for( int i = 0; i < 4; ++i )
    {
        int idx = scrollPos + i;
        if( idx >= listSize )
        {
            break;
        }

        // 재료
        const sMATERIALS& mats = m_matList[idx];

        // 아이템 코드/수
        SLOTCODE itemCode = mats.m_MaterialItemCode;
        int itemNum = mats.m_MaterialItemNum;

        assert( itemCode != 0 );

        // 임시 슬롯을 만들고
        SCItemSlot tempSlot;
        tempSlot.SetCode( (SLOTCODE)itemCode );
        tempSlot.SetNum( itemNum );

        // 재료 슬롯에 추가(복사)
        POSTYPE slotPos = i + 1;
        RC::eSLOT_INSERT_RESULT rt = InsertSlot( slotPos, tempSlot );

        if( rt == RC_INSERT_SLOT_SUCCESS )
        {
            TCHAR buf[256];

            // 추가된 슬롯 다시 얻고(시리얼을 알아오기 위해서)
            SCSlot& slot = GetSlot( slotPos );

            // 재료 이름
            g_InterfaceManager.GetItemName( slot.GetCode(), buf, sizeof( buf ) );
            GlobalFunc::SetCaption( m_ctlMatName[i], buf );

            // 재료 수
            Sprintf( buf, _T( "%d" ), slot.GetNum() );
            GlobalFunc::SetCaption( m_ctlMatNum[i], buf );

            // 렌더 유닛
            SUN_ASSERT(m_itemRenderer);
            ItemUnitRenderer::RenderUnit* renderUnit = NULL;

            if (m_itemRenderer)
            {
                renderUnit = m_itemRenderer->GetItemUnit( slot.GetSerial() );
                SUN_ASSERT(renderUnit);
            }

            // 필요 아이템이 인벤에 있는지 검사
            BOOL exist = ItemManager::Instance()->IsExistItem( SI_INVENTORY, itemCode, itemNum );

            if( exist )
            {
                if (renderUnit)
                {
                    renderUnit->SetState( SLOT_RENDER_STATE_ACTIVATED );
                }

                m_ctlMatName[i]->SetTextColor( RGBA( 255, 255, 255, 255 ) );
                m_ctlMatNum[i]->SetTextColor( RGBA( 255, 255, 255, 255 ) );
            }
            else
            {
                if (renderUnit)
                {
                    renderUnit->SetState( SLOT_RENDER_STATE_DEACTIVATED );
                }

                m_ctlMatName[i]->SetTextColor( RGBA( 255, 0, 0, 255 ) );
                m_ctlMatNum[i]->SetTextColor( RGBA( 255, 0, 0, 255 ) );
            }
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::ClearEnchantMatList( void )
{
    // 재료 슬롯 제거(1부터 시작)
    int numSlots = GetMaxSlotNum();

    for( int i = 1; i < numSlots; ++i )
    {
        if( !IsEmpty( i ) )
        {
            DeleteSlot( i, NULL );
        }
    }

    // 재료 이름/수
    for( int i = 0; i < 4; ++i )
    {
        GlobalFunc::SetCaption( m_ctlMatName[i], _T( "" ) );
        GlobalFunc::SetCaption( m_ctlMatNum[i], _T( "" ) );
    }
}

//----------------------------------------------------------------------------
/**
*/
BOOL EnchantDialog::HaveEnchantMaterials( void )
{
    assert( g_pHero );

    MaterialList::iterator it = m_matList.begin();
    for( ; it != m_matList.end(); ++it )
    {
        if( !ItemManager::Instance()->IsExistItem( 
            SI_INVENTORY, it->m_MaterialItemCode, it->m_MaterialItemNum ) )
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
    assert( g_pHero );

    return ( g_pHero->GetMoney() >= m_enchantCost );
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::RemoveEnchantMatsFromInven( void )
{
    assert( g_pHero );

    // 하임 소비
    MONEY money = g_pHero->GetMoney() - m_enchantCost;
    assert( money >= 0 );

    g_pHero->SetMoney( money );

    // 재료 소비
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
    assert( index >= 0 && index < 4 );

    for( int i = 0; i < 4; ++i )
    {
        m_ctlProbability[i]->SetCheckState( ( i == index ) );
    }

    m_probabilityIdx = index;

    if( IsValidEnchant() )
    {
        // 인챈트 재료 재설정
        ResetEnchantMatList();

        // 인챈트 비용 재계산
        RecalcEnchantCost();

        // 인챈트 재료 정보 갱신
        RefreshEnchantMatInfo();
    }
}

//----------------------------------------------------------------------------
/**
*/
int EnchantDialog::GetValidProbabilityIdx( int defIdx )
{
    assert( defIdx >= 0 && defIdx < 4 );
    assert( IsValidEnchant() );

    SCItemSlot& slot = (SCItemSlot&)GetSlot( 0 );

    const BASE_ITEMINFO* itemInfo = slot.GetItemInfo();
    assert( itemInfo );

    // 아이템 타입별 처리
    switch( (eITEMSUBTYPE)itemInfo->m_wSubType )
    {
    case eITEMSUBTYPE_ELITE_SSQ:
    case eITEMSUBTYPE_ELITE:    // 엘리트 아이템이면 무조건 50%
        return 1;

    case eITEMSUBTYPE_UNIQUE:   // 유니크 아이템이면 무조건 25%
        return 0;
    }

    // 인챈트 차수별 처리
    int newEnchant = slot.GetEnchant() + 1;
    assert( newEnchant <= MAX_ENCHANT_GRADE );

    // 6인챈까지는 무조건 100%
    if( newEnchant <= 6 )
    {
        return 3;
    }

    // 캐시 인챈이 아닌 경우
    if( !IsCashEnchant() )
    {
        int maxIdx = 3;

        if( newEnchant >= 7 && newEnchant < 10 )
        {
            // 7 ~ 9는 최대 75%
            maxIdx = 2;
        }
        else if( newEnchant >= 10 )
        {
            // 10인챈 이상은 최대 50%
            maxIdx = 1;
        }

        // 디폴트 값이 최대값보다 큰 경우
        if( defIdx > maxIdx )
        {
            // 기존 값과 최대값 중 작은 값으로 선택
            return max( 0, min( maxIdx, m_probabilityIdx ) );
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
    assert( IsValidEnchant() );

    SCItemSlot& slot = (SCItemSlot&)GetSlot( 0 );

    int newEnchant = slot.GetEnchant() + 1;
    assert( newEnchant <= MAX_ENCHANT_GRADE );

    switch( newEnchant )
    {
    case 7: case 8: case 9: 
        m_enchantLevel = ENCHANT_MIDDLE;
        break;

    case 10: case 11: case 12:
        m_enchantLevel = ENCHANT_HIGH;
        break;

    default:
        m_enchantLevel = ENCHANT_LOW;
        break;
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::ResetEnchantMatsForLv( void )
{
    assert( IsValidEnchant() );
    assert( !m_matsForLv );

    // 메인 슬롯(슬롯0)을 얻고
    SCItemSlot& slot = (SCItemSlot&)GetSlot( 0 );

    // 아이템 정보를 얻고
    const BASE_ITEMINFO* itemInfo = slot.GetItemInfo();
    assert( itemInfo );

    // 인챈트 재료 정보를 얻고
    m_matsForLv = EnchantParser::Instance()->GetEnchantMatInfo(
        itemInfo->m_LV, slot.GetEnchant(), itemInfo->IsWeapon(), slot.IsLimited(), m_enchantOpt);

    assert( m_matsForLv );
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::ResetEnchantMatList( void )
{
    assert( IsValidEnchant() );
    assert( m_matsForLv );

    // 재료 리스트 초기화
    m_matList.clear();

    // 재료 할인 비율
    eENCHANT_OPTION opt = ProbabilityIdxToOpt( m_probabilityIdx );

	//safecode
	if(!m_matsForLv) return;

    const sFARE_RATIO* ratio = m_matsForLv->GetFareRatio( opt );
    assert( ratio );

    // 최대 재료 수만큼 반복
    for( int i = 0; i < MATERIALS_FOR_LEVEL::kMaxMaterialNumber; ++i )
    {
        // 재료
        const sMATERIALS* mats = m_matsForLv->GetMaterials( i );
        if( mats )
        {
            WORD matNum = (WORD)( mats->m_MaterialItemNum * ratio->m_MaterialFare );

            // 재료 리스트에 추가
            sMATERIALS mat;
            mat.m_MaterialItemCode = mats->m_MaterialItemCode;
            mat.m_MaterialItemNum = max( 1, matNum );

            m_matList.push_back( mat );
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::RecalcEnchantCost( void )
{
    assert( IsValidEnchant() );
    assert( m_matsForLv );

    // 메인 슬롯(슬롯0)을 얻고
    SCItemSlot& slot = (SCItemSlot&)GetSlot( 0 );

    // 아이템 정보를 얻고
    const BASE_ITEMINFO* itemInfo = slot.GetItemInfo();
    assert( itemInfo );

    eENCHANT_OPTION opt = ProbabilityIdxToOpt( m_probabilityIdx );

	//safecode
	if(!m_matsForLv) return;

    const sFARE_RATIO* ratio = m_matsForLv->GetFareRatio( opt );
    assert( ratio );

#ifdef _NA_006589_20120415_ENCHANT_PRICE_MODIFY
    // 하임 계산
    LEVELTYPE touch_item_level = itemInfo->m_LV;
    if (touch_item_level > 22) {
        touch_item_level = 23;
    }
    else if (touch_item_level > 12) {
        touch_item_level = 13;
    }
    else {
        touch_item_level = 3;
    }
    m_enchantCost = (MONEY)( ( slot.GetPriceForUpGradeFare() * touch_item_level ) * ratio->m_HaimFare );
#else
    // 하임 계산
    m_enchantCost = (MONEY)( ( slot.GetPriceForUpGradeFare() * itemInfo->m_LV ) * ratio->m_HaimFare );
#endif // _NA_006589_20120415_ENCHANT_PRICE_MODIFY/

#ifdef _NA_20100307_BETAKEY_SYSTEM
    uiLoginMan* login_manager = static_cast<uiLoginMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
    if (login_manager && m_matsForLv->GetBetaKeyInfo())
    {
        m_enchantCost = BetaKeyImpl::GetEnchantMoney(login_manager->GetBetaKeyInfo(), 
            m_matsForLv->GetBetaKeyInfo(), m_enchantCost);
    }
#endif//_NA_20100307_BETAKEY_SYSTEM
}

//----------------------------------------------------------------------------
/**
*/
void EnchantDialog::OnLButtonClick( SI_MESSAGE* pMsg )
{
    assert( pMsg );
    assert( pMsg->eResultMsg == RT_MSG_LBUTTONCLICK );

    POSTYPE pos = getCtrlIDToPos( pMsg->dwCtrlID );

    if( pos == ENCHANT_BTN_SLOT_1 )
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
void EnchantDialog::OnEnchantItemSlotClick( void )
{
    BOOL ret = MouseHandler::Instance()->ItemCompositeNewEnchant( SI_ENCHANT, 0 );
    MouseHandler::Instance()->RollbackItemAtHand();

    if (ret == TRUE)
    {
        OnEnchantItem();
    }

//    if( IsValidEnchant() )
//    {
//        // 인챈트 레벨 재설정
//        ResetEnchantLevel();
//
//        // 인챈트 재료 재설정
//        ResetEnchantMatsForLv();
//
//        // 기본 정보 갱신
//        RefreshDefEnchantInfo();
//
//        // 확률탭 활성
//#ifdef RESERVE_PREV_PROBABILITY_IDX
//        // nothing to do
//#else
//        m_probabilityIdx = 3;
//#endif
//        int idx = GetValidProbabilityIdx( m_probabilityIdx );
//
//        ActivateProbabilityTab( idx );
//    }
}

//------------------------------------------------------------------------------
void EnchantDialog::OnEnchantItem(void)
{
    if (IsValidEnchant() == FALSE)
    {
        return;
    }

    // 인챈트 레벨 재설정
    ResetEnchantLevel();

    // 인챈트 재료 재설정
    ResetEnchantMatsForLv();

    // 기본 정보 갱신
    RefreshDefEnchantInfo();

    // 확률탭 활성
#ifdef RESERVE_PREV_PROBABILITY_IDX
    // nothing to do
#else
    m_probabilityIdx = 3;
#endif

    ActivateProbabilityTab( GetValidProbabilityIdx(m_probabilityIdx) );
}


//----------------------------------------------------------------------------
void EnchantDialog::OnEnchantItem_InventoryRightClick(const SLOTIDX from_container_index, 
                                                      const POSTYPE from_container_position)
{
    bool is_result = MouseHandler::Instance()->ItemCompositeNewEnchant_InventoryRightClick(from_container_index,
                                                                                            SI_ENCHANT,
                                                                                            from_container_position,
                                                                                            0);
    if (is_result)
    {
        OnEnchantItem();
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

    // 인벤에서 인챈트 재료 제거
    RemoveEnchantMatsFromInven();

    // 인챈트 창 아이템
    SCItemSlot& dstSlot = (SCItemSlot&)GetSlot( 0 );
    dstSlot.SetEnchant( dstSlot.GetEnchant() + 1 );

    // 실제 주 아이템
    SCSlotContainer* oriContainer = ItemManager::Instance()->GetContainer( SI_INVENTORY );
    assert( oriContainer );

    SCItemSlot& oriSlot = (SCItemSlot&)oriContainer->GetSlot( m_pendingTargetPos );
    oriSlot.SetEnchant( dstSlot.GetEnchant() );

    const BASE_ITEMINFO* itemInfo = oriSlot.GetItemInfo();
    assert( itemInfo );

    // 메시지 출력
    TCHAR itemName[128];
    TCHAR szMessage[MAX_MESSAGE_LENGTH];

    GameFunc::GetItemName(&oriSlot, itemInfo, itemName, 128);
    
    // [%s] 아이템의 인첸트가 성공 하였습니다.
    g_InterfaceManager.GetInterfaceString( eST_SUCCESS_ITEM_ENCHANT, szMessage, sizeof( szMessage ) );

    GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage, itemName );

    // 화면 위/중앙 표시
    GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage, itemName );

    // 사운드 출력
    g_SoundEffect.PlayUI( UI_SOUND_COMPOSE_ENCHANT_SUCCESS );

    // 대화상자 활성/초기화
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

    // 인벤에서 인챈트 재료 제거
    RemoveEnchantMatsFromInven();

    TCHAR szMessage[MAX_MESSAGE_LENGTH];

    // 결과 코드별 처리
    if( pRecvMsg->m_byErrorCode == RC_ITEM_ENCHANT_FAILED_AND_ENCHNAT_DOWN )
    {
        // 인챈트 실패 다운 레벨
        int enchant_fail_down_level = 1;
        if (m_matsForLv)
        {
            enchant_fail_down_level = m_matsForLv->GetDownLV();
        }
        // 인챈트 창 아이템
        SCItemSlot& dstSlot = (SCItemSlot&)GetSlot( 0 );
        dstSlot.SetEnchant( dstSlot.GetEnchant() - enchant_fail_down_level );

        // 실제 주 아이템
        SCSlotContainer* oriContainer = ItemManager::Instance()->GetContainer( SI_INVENTORY );
        assert( oriContainer );

        SCItemSlot& oriSlot = (SCItemSlot&)oriContainer->GetSlot( m_pendingTargetPos );
        oriSlot.SetEnchant( dstSlot.GetEnchant() );

        const BASE_ITEMINFO* itemInfo = oriSlot.GetItemInfo();
        assert( itemInfo );

        // 메시지 출력
        TCHAR szItemName[128], szItemName2[128], buf1[64], buf2[64];
        TCHAR szTempMessage[MAX_MESSAGE_LENGTH] = {0,};

        g_InterfaceManager.GetInterfaceString( itemInfo->m_NCode, buf1, sizeof( buf1 ) );
        g_InterfaceManager.GetInterfaceString( itemInfo->m_SCNCode, buf2, sizeof( buf2 ) );

        Sprintf( szItemName, "%s %s %+d", buf1, buf2, oriSlot.GetEnchant() + enchant_fail_down_level );
        Sprintf( szItemName2, "%s %s %+d", buf1, buf2, oriSlot.GetEnchant());

        // 5318 인챈트에 실패하여, %s가 %s로 다운되었습니다.
        g_InterfaceManager.GetInterfaceString( 5318, szTempMessage );
        Sprintf( szMessage, szTempMessage, szItemName, szItemName2 );
    }
	else if (pRecvMsg->m_byErrorCode == RC_ITEM_ENCHANT_FAILED_AND_DESTROY)
	{
		SCSlotContainer* oriContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
		assert(oriContainer);
		SCItemSlot& oriSlot = (SCItemSlot&)oriContainer->GetSlot(m_pendingTargetPos);

		TCHAR szTemp1[64]	  = {0,};
		TCHAR szTemp2[64]	  = {0,};
		TCHAR szItemName[256] = {0,};
		g_InterfaceManager.GetInterfaceString(oriSlot.GetItemInfo()->m_NCode, szTemp1, 63);
		g_InterfaceManager.GetInterfaceString(oriSlot.GetItemInfo()->m_SCNCode, szTemp2, 63);
		Snprintf(szItemName, 255, "%s %s +%d", szTemp1, szTemp2, oriSlot.GetEnchant());

		oriContainer->DeleteSlot(m_pendingTargetPos, NULL);

		// 5319 인챈트에 실패 하여, %s가 소멸 되었습니다.
		TCHAR szTempMessage[MAX_MESSAGE_LENGTH] = { '\0', };
		g_InterfaceManager.GetInterfaceString(5319, szTempMessage);
		Snprintf(szMessage, INTERFACE_STRING_LENGTH - 1, szTempMessage, szItemName);	
	}
    else
    {
        // 인첸트에 실패 하였습니다.
        g_InterfaceManager.GetInterfaceString(eST_FAILED_ITEM_ENCHANT, szMessage);
    }

    // 메시지 출력
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

    // 화면 위/중앙 표시
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);

    // 사운드 출력
    g_SoundEffect.PlayUI(UI_SOUND_COMPOSE_ENCHANT_FAIL);

    // 대화상자 활성/초기화
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



#endif //__NA_001358_ENCHANT_UNIFICATION

#endif // _NA_003966_20111227_ADD_ENCHANT
