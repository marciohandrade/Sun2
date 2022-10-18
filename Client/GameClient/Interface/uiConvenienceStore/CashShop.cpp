#include "SunClientPrecompiledHeader.h"

#include "CashShop.h"
#include "StorageItem.h"
#include "PacketRequestManager.h"
#include "GameParameter.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "ItemManager.h"
#include "PurchaseItem.h"


namespace ConvenienceStore
{
    CashShop* CashShop::static_instance_ = NULL;

    CashShop::CashShop()
    : shop_list_manager_(NULL)
    , has_new_present_(false)
    , is_open_with_gift_menu_(false)
    {
        if( static_instance_ != NULL )
        {
            assert(false);
        }
        static_instance_ = this;

        cash_info_.cash_ = 0;
        cash_info_.pp_cash_ = 0;
        cash_info_.mileage_ = 0;

    };

    CashShop::~CashShop()
    {
        if( shop_list_manager_ != NULL )
        {
            delete shop_list_manager_;
            shop_list_manager_ = NULL;
        }
    }

    void CashShop::ToggleOpenKey()
    {
        if( IsShowShop() == true )
        {
            SEND_CG_SHOP_CLOSE_NTF();
            HideCashShop();
        }
        else
        {
            ShowCashShop();
        }
    }

    void CashShop::OpenCashShopWithGiftMenu()
    {
        if( IsShowShop() == true )
        {
            SEND_CG_SHOP_CLOSE_NTF();
            HideCashShop();
        }
        else
        {
            is_open_with_gift_menu_ = true;
            ShowCashShop();
        }
    }

    void CashShop::ShowCashShop()
    {
        SEND_CG_SHOP_VERSION_SYN();
    }

    bool CashShop::IsShowShop()
    {
        return false;
    }

    void CashShop::HideCashShop()
    {
        SEND_CG_SHOP_CLOSE_NTF();
        OnHideCashShop();
    }

    bool CashShop::BuildShopList()
    {
        if( builded_sales_zone_script_version_.IsEqual( sale_zone_script_version_ ))
            return true;

        product_list_.ClearProductArray();

        if( shop_list_manager_ != NULL )
        {
            delete shop_list_manager_;
        }

        //------------------------------------------------------------------------------ 
        shop_list_manager_ = new CShopListManager;

        CListVersionInfo version_info;
        version_info.Zone = sale_zone_script_version_.sales_zone_;
        version_info.year = sale_zone_script_version_.year_;
        version_info.yearId = sale_zone_script_version_.year_identity_;

        bool is_always_download = CGeneralGameParam::Instance()->conveniencestore_always_download;
        unsigned short port_num = CGeneralGameParam::Instance()->conveniencestore_port_num;

        //------------------------------------------------------------------------------ 
        if ( port_num == 21)
        {
            shop_list_manager_->SetListManagerInfoVer6(FTP,  
                (TCHAR*)CGeneralGameParam::Instance()->conveniencestore_connect_ip.c_str(),
                CGeneralGameParam::Instance()->conveniencestore_port_num,
                _T(""),		                                                        
                _T(""),		                                                        
                (TCHAR*)CGeneralGameParam::Instance()->conveniencestore_remote_path.c_str(),
                _T("./log"),    //_T("D:/Perforce/test_Dev_release/2.최신개발버전/SUN_Client/log"),
                false,
                version_info);
        }
        else if ( port_num > 0)
        {
            shop_list_manager_->SetListManagerInfoVer6(HTTP,  
                (TCHAR*)CGeneralGameParam::Instance()->conveniencestore_connect_ip.c_str(),
                CGeneralGameParam::Instance()->conveniencestore_port_num,
                _T(""),		                                                        
                _T(""),		                                                        
                (TCHAR*)CGeneralGameParam::Instance()->conveniencestore_remote_path.c_str(),
                _T("./log"),    //_T("D:/Perforce/test_Dev_release/2.최신개발버전/SUN_Client/log"),
                false,
                version_info);
        }
        else
        {
            shop_list_manager_->SetListManagerInfoVer6(HTTP,  
                (TCHAR*)CGeneralGameParam::Instance()->conveniencestore_connect_ip.c_str(),
                _T(""),		                                                        
                _T(""),		                                                        
                (TCHAR*)CGeneralGameParam::Instance()->conveniencestore_remote_path.c_str(),
                _T("./log"),    //_T("D:/Perforce/test_Dev_release/2.최신개발버전/SUN_Client/log"),
                version_info);
        }

        WZResult res = shop_list_manager_->LoadScriptList(is_always_download);
        if( res.IsSuccess() == false )
        {
            WZLOG(WZERR, "File Download fail: %s[%d]", res.GetErrorMessage(), res.GetErrorCode());
            return false;
        }

#if _DEV_VER
        test_shop_script_list();
#endif //DEV_VER

        builded_sales_zone_script_version_ = sale_zone_script_version_;

        OnUpdateShopItemList();

        return true;
    }

    void CashShop::NetworkProc(MSG_BASE* pk)
    {
        if (pk->m_byCategory != CG_SHOP)
        {
            return;
        }

    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL

        switch (pk->m_byProtocol)
        {
        case CG_SHOP_VERSION_ACK:
            {
                MSG_CG_SHOP_VERSION_ACK* ack = static_cast<MSG_CG_SHOP_VERSION_ACK*>(pk); 

                bool is_always_download = CGeneralGameParam::Instance()->conveniencestore_always_download;

                sale_zone_script_version_ = ack->sales_version_;
                banner_zone_script_version_ = ack->banner_version_;

                SEND_CG_SHOP_OPEN_SYN();
            }
            break;
        case CG_SHOP_VERSION_NAK:
            {
                MSG_CG_SHOP_VERSION_NAK* nak = static_cast<MSG_CG_SHOP_VERSION_NAK*>(pk);
                TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
                DWORD msg_color;
                GetErrorStringByErrorCode(nak->error_code_, msg, INTERFACE_STRING_LENGTH, &msg_color);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, msg);
                g_InterfaceManager.ShowCenterMessageBox(msg, 5000, &msg_color, FALSE);
            }
            break;
        case CG_SHOP_OPEN_ACK:
            {
                if( BuildShopList() )
                {
                    // 상점이 열리면 선물알림 플래그는 일단 끈다
                    has_new_present_ = false;

                    SEND_CG_SHOP_CASH_SYN();
                    SEND_CG_SHOP_BUY_STORAGE_LIST_SYN(1, 10);
                    SEND_CG_SHOP_GIFT_STORAGE_LIST_SYN(1, 10);
                    OnShowCashShop();
                }
                else
                {
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("counter open failure!!"));
                }
            }
            break;
        case CG_SHOP_OPEN_NAK:
            {
                MSG_CG_SHOP_OPEN_NAK* nak = static_cast<MSG_CG_SHOP_OPEN_NAK*>(pk);
                TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
                DWORD msg_color;
                GetErrorStringByErrorCode(nak->error_code_, msg, INTERFACE_STRING_LENGTH, &msg_color);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, msg);
                g_InterfaceManager.ShowCenterMessageBox(msg, 5000, &msg_color, FALSE);
            }
            break;
        case CG_SHOP_CLOSE_NTF:
            {
                MSG_CG_SHOP_CLOSE_NTF* ntf = static_cast<MSG_CG_SHOP_CLOSE_NTF*>(pk);
                TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
                DWORD msg_color;
                GetErrorStringByErrorCode(ntf->error_code_, msg, INTERFACE_STRING_LENGTH, &msg_color);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, msg);
                g_InterfaceManager.ShowCenterMessageBox(msg, 5000, &msg_color, FALSE);

                OnHideCashShop();
            }
            break;
        case CG_SHOP_CASH_ACK:
            {
                MSG_CG_SHOP_CASH_ACK* ack = static_cast<MSG_CG_SHOP_CASH_ACK*>(pk);

#ifdef _GLOBAL
#ifdef _DEBUG
                if( ack->cash_.cash_ == 0 && ack->cash_.pp_cash_ == 0 )
                {
                    ack->cash_.cash_ = 10000;
                    ack->cash_.pp_cash_ = 10000;
                }
#endif
#endif

                CashShop::instance()->SetCashInfo( ack->cash_ );


                OnUpdateCash();
            }
            break;
        case CG_SHOP_CASH_NAK:
            {
                MSG_CG_SHOP_OPEN_NAK* nak = static_cast<MSG_CG_SHOP_OPEN_NAK*>(pk);
                TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
                DWORD msg_color;
                GetErrorStringByErrorCode(nak->error_code_, msg, INTERFACE_STRING_LENGTH, &msg_color);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, msg);
                g_InterfaceManager.ShowCenterMessageBox(msg, 5000, &msg_color, FALSE);
            }
            break;

        case CG_SHOP_BUY_STORAGE_LIST_ACK:
            {
                MSG_CG_SHOP_BUY_STORAGE_LIST_ACK* ack = static_cast<MSG_CG_SHOP_BUY_STORAGE_LIST_ACK*>(pk);

                CashShop::instance()->GetStorageItemData().SetStorageData( ack );
                OnUpdateStorageBuyList();

            }
            break;
        case CG_SHOP_BUY_STORAGE_LIST_NAK:
            {
                MSG_CG_SHOP_BUY_STORAGE_LIST_NAK* nak = static_cast<MSG_CG_SHOP_BUY_STORAGE_LIST_NAK*>(pk);
                OutputErrorMessage( nak->error_code_ );
            }
            break;
        case CG_SHOP_GIFT_STORAGE_LIST_ACK:
            {
                MSG_CG_SHOP_GIFT_STORAGE_LIST_ACK* ack = static_cast<MSG_CG_SHOP_GIFT_STORAGE_LIST_ACK*>(pk);
                CashShop::instance()->GetGiftStorageItemData().SetStorageData( ack );

                OnUpdateStorageGiftList();
            }
            break;
        case CG_SHOP_GIFT_STORAGE_LIST_NAK:
            {
                MSG_CG_SHOP_GIFT_STORAGE_LIST_NAK* nak = static_cast<MSG_CG_SHOP_GIFT_STORAGE_LIST_NAK*>(pk);
                OutputErrorMessage( nak->error_code_ );
            }
            break;

        case CG_SHOP_BUY_ITEM_ACK:
            {
                OnEndWaiting();

                //! 캐시 정보 요청
                SEND_CG_SHOP_CASH_SYN();

                //! 패키지 같은게 있을수 있어서 다시 첨부터 요청한다.
                SEND_CG_SHOP_BUY_STORAGE_LIST_SYN(1,10);

                MSG_CG_SHOP_BUY_ITEM_ACK* ack = static_cast<MSG_CG_SHOP_BUY_ITEM_ACK*>(pk);
                OnSuccessBuyItem( ack->item_info_, ack->item_count_ );
            }
            break;
        case CG_SHOP_SEND_GIFT_ACK:
            {
                OnEndWaiting();

                //! 캐시 정보 요청
                SEND_CG_SHOP_CASH_SYN();

                MSG_CG_SHOP_SEND_GIFT_ACK* ack = static_cast<MSG_CG_SHOP_SEND_GIFT_ACK*>(pk);
                OnSuccessGiftItem( ack->item_info_, ack->item_count_ );
            }
            break;
        case CG_SHOP_BUY_ITEM_NAK:
            {
                MSG_CG_SHOP_BUY_ITEM_NAK* nak = static_cast<MSG_CG_SHOP_BUY_ITEM_NAK*>(pk);
                OutputErrorMessage( nak->error_code_ );
                OnEndWaiting();
            }
            break;
        case CG_SHOP_SEND_GIFT_NAK:
            {
                MSG_CG_SHOP_SEND_GIFT_NAK* nak = static_cast<MSG_CG_SHOP_SEND_GIFT_NAK*>(pk);
                OutputErrorMessage( nak->error_code_ );
                OnEndWaiting();
            }
            break;

        case CG_SHOP_USE_STORAGE_ITEM_ACK:
            {
                MSG_CG_SHOP_USE_STORAGE_ITEM_ACK* ack = static_cast<MSG_CG_SHOP_USE_STORAGE_ITEM_ACK*>(pk);
                ItemManager::Instance()->InsertInventoryTotalInfo(SHOP_STORAGE, ack->inventory_info_);

                StorageItemList& ItemData = is_my_item_receive_ ? GetStorageItemData() : GetGiftStorageItemData();

                TCHAR message[1024] = "";

                // 받은 아이템의 이름을 구함
                if( StorageItemInfo* buy_info = ItemData.FindStorageItem(ack->use_item_.storage_sequence_, ack->use_item_.storage_item_sequence_, ack->use_item_.item_type_) )
                {
                    CShopProduct* shop_product = NULL;
                    if ( shop_list_manager_->GetListPtr()->GetProductListPtr()->GetValueByKey(buy_info->product_sequence_, shop_product) == true)
                    {
                        Snprintf( message, 1024-1, "%s %d%s", shop_product->ProductName, ack->inventory_info_.m_InvenCount, g_InterfaceManager.GetInterfaceString(155565) );
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
                    }

                    // 저장소에서 삭제
                    ItemData.EraseStorageItem( ack->use_item_.storage_sequence_, ack->use_item_.storage_item_sequence_, ack->use_item_.item_type_ );
                }

                bool is_continue = false;

                if( is_all_item_receive_ )
                {
                    // 모두 받기인 경우 남아 있다면 계속 보낸다
                    if( is_my_item_receive_ == true )/*&& IsEmptyBuyStorage() == false )*/
                    {
                        if( IsEmptyBuyStorage() == false )
                        {
                            SendUseageStorageBuyItemOnce();
                            OnReceveStorageItem( message, true, is_all_item_receive_, is_my_item_receive_, true );
                            is_continue = true;
                        }
                    }
                    else if( is_my_item_receive_ == false )//&& IsEmptyGiftStorage() == false )
                    {
                        if( IsEmptyGiftStorage() == false )
                        {
                            SendUseageStorageGiftItemOnce();
                            OnReceveStorageItem( message, true, is_all_item_receive_, is_my_item_receive_, true );
                            is_continue = true;
                        }
                    }
                }


                if( is_continue == false )
                {
                    //! 다 받았으면 목록을 다시 요청한다.
                    int page_index = ItemData.GetPage();
                    if( ItemData.GetCountPerPage() == 0 )
                    {
                        // 현재 페이지에 0개가 되었다면,
                        --page_index;
                        if( page_index < 1 )
                            page_index = 1;
                    }

                    is_my_item_receive_ ? SEND_CG_SHOP_BUY_STORAGE_LIST_SYN(page_index, 10) : SEND_CG_SHOP_GIFT_STORAGE_LIST_SYN(page_index, 10);

                    OnReceveStorageItem( message, true, is_all_item_receive_, is_my_item_receive_, false );
                }
                break;
            }
        case CG_SHOP_USE_STORAGE_ITEM_NAK:
            {
                MSG_CG_SHOP_USE_STORAGE_ITEM_NAK* nak = static_cast<MSG_CG_SHOP_USE_STORAGE_ITEM_NAK*>(pk);

                TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
                DWORD msg_color;
                GetErrorStringByErrorCode(nak->error_code_, msg, INTERFACE_STRING_LENGTH, &msg_color);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, msg);
                g_InterfaceManager.ShowCenterMessageBox(msg, 5000, &msg_color, FALSE);


                //! 목록을 다시 요청한다.
                StorageItemList& ItemData = is_my_item_receive_ ? GetStorageItemData() : GetGiftStorageItemData();
                int page_index = ItemData.GetPage();
                if( ItemData.GetCountPerPage() == 0 )
                {
                    // 현재 페이지에 0개가 되었다면,
                    --page_index;
                    if( page_index < 1 )
                        page_index = 1;
                }

                is_my_item_receive_ ? SEND_CG_SHOP_BUY_STORAGE_LIST_SYN(page_index, 10) : SEND_CG_SHOP_GIFT_STORAGE_LIST_SYN(page_index, 10);

                OnReceveStorageItem( msg, false, is_all_item_receive_, is_my_item_receive_, false );
            }
            break;
        case CG_SHOP_CHAR_NAME_CHECK_ACK:
            {
                MSG_CG_SHOP_CHAR_NAME_CHECK_ACK* ack = static_cast<MSG_CG_SHOP_CHAR_NAME_CHECK_ACK*>(pk);
                OnResultNameCheck( true, ack->character_name_, ack->character_level_, ack->guild_name_ );
            }
            break;
        case CG_SHOP_CHAR_NAME_CHECK_NAK:
            {
                MSG_CG_SHOP_CHAR_NAME_CHECK_NAK* nak = static_cast<MSG_CG_SHOP_CHAR_NAME_CHECK_NAK*>(pk);
                OutputErrorMessage( nak->error_code_ );
                OnResultNameCheck( false, NULL, 0, NULL );
            }
            break;

        case CG_SHOP_EXIST_GIFT_ACK:
        case CG_SHOP_EXIST_GIFT_NTF:
            {
                has_new_present_ = true;
            }
            break;
        case CG_SHOP_EXIST_GIFT_NAK:
            {
                has_new_present_ = false;
            }
            break;
        }
    #endif
    }

    void CashShop::SendBuyItem(std::vector<PurchaseItem*>& purchase_list, Define::CashType cash_type)
    {
        SEND_CG_SHOP_BUY_ITEM_SYN(purchase_list, cash_type); 
        OnBeginWaiting();
    }

    void CashShop::SendGiftItem(std::vector<PurchaseItem*>& purchase_list, Define::CashType cash_type, LPCTSTR recv_name, LPCTSTR recv_msg)
    {
        SEND_CG_SHOP_SEND_GIFT_SYN(purchase_list, cash_type, recv_name, recv_msg); 
        OnBeginWaiting();
    }


    void CashShop::ReceiveStorageItem( bool bAllReceive, bool bIsMyItem )
    {
        is_my_item_receive_ = bIsMyItem;
        is_all_item_receive_ = bAllReceive;

        OnStartReceiveStorageItem( bAllReceive, bIsMyItem );
    }

    void CashShop::OutputErrorMessage( int error_code )
    {
        TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
        DWORD msg_color;
        GetErrorStringByErrorCode(error_code, msg, INTERFACE_STRING_LENGTH, &msg_color);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, msg);
        g_InterfaceManager.ShowCenterMessageBox(msg, 5000, &msg_color, FALSE);

    }

    void CashShop::GetErrorStringByErrorCode(int error_code, OUT TCHAR* output_msg, int string_size, DWORD* string_color/* = NULL*/)
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        if( error_code == RC::RC_SHOP_SUCCESS )
        {
            return;
        }


        // 스트링 코드는 6791부터 순서대로
        DWORD string_code = 6791 + error_code - (DWORD)(RC::RC_SHOP_PROCESS_TRANSACTION);


        if( error_code == RC::RC_SHOP_EVENT_ITEM_GIFT_IMPOSSIBLE )
            string_code = 6822;
        else if( error_code == RC::RC_SHOP_EVENT_ITEM_GIFT_COUNT_OVER )
            string_code = 6823;
        else if( error_code == RC::RC_SHOP_LIMIT_CASH_TO_GIFT )
            string_code = 6824;
		else if( error_code == RC::RC_SHOP_FAILED )
			string_code = 6827;

        // 문자의 색상 결정
        WzColor text_color = WzColor_RGBA(129,254,82,255);   // default color

        if( error_code == RC::RC_SHOP_ALREADY_EXSIT_USER ||
            error_code == RC::RC_SHOP_INVENTORY_FULL ||
            error_code == RC::RC_SHOP_EVENT_ITEM_BUY_IMPOSSIBLE ||
            error_code == RC::RC_SHOP_NOT_ENOUGH_CASH 
            )
        {
            // 붉은색 문자
            text_color = WzColor_RGB(197, 47, 47);
        }



        // 기타 추가 작업
        if( error_code == RC::RC_SHOP_ALREADY_EXSIT_USER )
        {
            //! 샵 연결을 끊자.
            SEND_CG_SHOP_CLOSE_NTF();
            OnHideCashShop();
        }



        if( string_color != NULL )
        {
            *string_color = text_color;
        }

        // 문자 출력
        {
            TCHAR szMessage[INTERFACE_STRING_LENGTH] ={NULL, };
            g_InterfaceManager.GetInterfaceString(string_code, szMessage, INTERFACE_STRING_LENGTH);
            _tcscat(output_msg, szMessage);

            //! 에러 코드 출력
            _tcscat(output_msg, _T("["));
            {
                TCHAR temp[32] = {NULL, };
                Snprintf(temp, 32, "%d", error_code);
                _tcscat(output_msg, temp);
            }
            _tcscat(output_msg, _T("]"));
        }
    #endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    }


    //------------------------------------------------------------------------------ 
    void CashShop::SEND_CG_SHOP_BUY_ITEM_SYN(std::vector<PurchaseItem*>& purchase_list, Define::CashType cash_type)
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        //------------------------------------------------------------------------------ 
        //! 패킷 2번보내지 않게 체크
        if (PacketRequestManager::Instance()->is_sent(CG_SHOP, CG_SHOP_BUY_ITEM_SYN))
        {
            return;
        }
        PacketRequestManager::Instance()->PushPacket((new MSG_CG_SHOP_BUY_ITEM_SYN),
            (new MSG_CG_SHOP_BUY_ITEM_ACK), 
            (new MSG_CG_SHOP_BUY_ITEM_NAK),
            sizeof(MSG_CG_SHOP_BUY_ITEM_SYN), 
            PACKET_WAITING_MILLISECOND, 
            false);
        //------------------------------------------------------------------------------ 

        MSG_CG_SHOP_BUY_ITEM_SYN syn;
        syn.item_count_ = 0;

        for( size_t i=0;i!=purchase_list.size();++i)
        {
            PurchaseItem* purchase = purchase_list[i];
            if ((purchase->is_lotto() == false) && (purchase->is_package() == false) && (purchase->GetPriceSequence() <= 0))
            {
                continue;
            }

            syn.item_info_[syn.item_count_].product_display_sequence_ = purchase->GetShopCategory()->ProductDisplaySeq;  // 카테고리 번호
            syn.item_info_[syn.item_count_].package_product_sequence_ = purchase->GetShopPackage()->PackageProductSeq;          // 패키지 번호
            syn.item_info_[syn.item_count_].price_sequence_ = purchase->GetPriceSequence();
            syn.item_info_[syn.item_count_].cash_type_ = cash_type;
            syn.item_count_ += 1;

            if (syn.item_count_ >= SHOP_MAX_ITEM_BUY_COUNT)
            {
                GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));
                syn.item_count_ = 0;
            }
        }

        if (syn.item_count_ > 0 && syn.item_count_ < SHOP_MAX_ITEM_BUY_COUNT)
        {
            GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));
        }

        //------------------------------------------------------------------------------ 
        if (CGeneralGameParam::Instance()->IsNet() == FALSE)
        {
            MSG_CG_SHOP_BUY_ITEM_ACK ack;

            ack.item_count_ = syn.item_count_;
            for (int i = 0; i < (int)syn.item_count_; ++i)
            {
                ack.item_info_[i].product_display_sequence_ = syn.item_info_[i].product_display_sequence_;  // 카테고리 번호
                ack.item_info_[i].package_product_sequence_ = syn.item_info_[i].package_product_sequence_;          // 패키지 번호
                ack.item_info_[i].price_sequence_ = syn.item_info_[i].price_sequence_;            // 가격 번호
                ack.item_info_[i].error_code_ = i;
            }
            NetworkProc(&ack);
        }
    #endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    }

    //------------------------------------------------------------------------------ 
    void CashShop::SEND_CG_SHOP_SEND_GIFT_SYN(std::vector<PurchaseItem*>& purchase_list, Define::CashType cash_type, LPCTSTR recv_name, LPCTSTR recv_msg )
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        //------------------------------------------------------------------------------ 
        //! 패킷 2번보내지 않게 체크
        if (PacketRequestManager::Instance()->is_sent(CG_SHOP, CG_SHOP_SEND_GIFT_SYN))
        {
            return;
        }
        PacketRequestManager::Instance()->PushPacket((new MSG_CG_SHOP_SEND_GIFT_SYN),
            (new MSG_CG_SHOP_SEND_GIFT_ACK), 
            (new MSG_CG_SHOP_SEND_GIFT_NAK),
            sizeof(MSG_CG_SHOP_SEND_GIFT_SYN), 
            PACKET_WAITING_MILLISECOND, 
            false);
        //------------------------------------------------------------------------------ 

        MSG_CG_SHOP_SEND_GIFT_SYN syn;
        memset( syn.recv_character_name_, 0, sizeof(char) * (MAX_CHARNAME_LENGTH +1) );
        memset( syn.gift_message_, 0, sizeof(char) * GIFT_MESSAGE_LENGTH );

        syn.item_count_ = 0;
        StrnCopy(syn.recv_character_name_, recv_name, MAX_CHARNAME_LENGTH);
        StrnCopy(syn.gift_message_, recv_msg, GIFT_MESSAGE_LENGTH - 1);

        for( size_t i=0;i!=purchase_list.size();++i)
        {
            PurchaseItem* purchase = purchase_list[i];
            if ((purchase->is_lotto() == false) && (purchase->is_package() == false) && (purchase->GetPriceSequence() <= 0))
            {
                continue;
            }
            syn.item_info_[syn.item_count_].product_display_sequence_ = purchase->GetDisplaySequence(); // 카테고리 번호
            syn.item_info_[syn.item_count_].package_product_sequence_ = purchase->GetPackageSequence(); // 패키지 번호
            syn.item_info_[syn.item_count_].price_sequence_ = purchase->GetPriceSequence();   
            syn.item_info_[syn.item_count_].cash_type_ = cash_type;
            syn.item_count_ += 1;

            if (syn.item_count_ >= SHOP_MAX_ITEM_BUY_COUNT)
            {
                GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));
                syn.item_count_ = 0;
            }
        }
        if (syn.item_count_ > 0 && syn.item_count_ < SHOP_MAX_ITEM_BUY_COUNT)
        {
            GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));
        }

        //------------------------------------------------------------------------------ 
        if (CGeneralGameParam::Instance()->IsNet() == FALSE) 
        {
            MSG_CG_SHOP_SEND_GIFT_ACK ack;
            ack.item_count_ = 0;
            NetworkProc(&ack);
        }
    #endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    }

    //------------------------------------------------------------------------------ 
    void CashShop::SEND_CG_SHOP_BUY_STORAGE_LIST_SYN( int page_index, int request_count)
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        //------------------------------------------------------------------------------ 
        //! 패킷 2번보내지 않게 체크
        if (PacketRequestManager::Instance()->is_sent(CG_SHOP, CG_SHOP_BUY_STORAGE_LIST_SYN))
        {
            return;
        }
        PacketRequestManager::Instance()->PushPacket((new MSG_CG_SHOP_BUY_STORAGE_LIST_SYN),
            (new MSG_CG_SHOP_BUY_STORAGE_LIST_ACK), 
            (new MSG_CG_SHOP_BUY_STORAGE_LIST_NAK),
            sizeof(MSG_CG_SHOP_BUY_STORAGE_LIST_SYN), 
            PACKET_WAITING_MILLISECOND, 
            false);
        //------------------------------------------------------------------------------ 

        if (request_count <= 0)
        {
            return;
        }
        if (request_count > BUY_STORAGE_ITEM_PER_PAGE)
        {
            request_count = BUY_STORAGE_ITEM_PER_PAGE;
        }

        MSG_CG_SHOP_BUY_STORAGE_LIST_SYN syn;
        syn.page_ = page_index;
        syn.item_count_per_page_ = request_count;   // MAX_STORAGE_ITEM_PER_PAGE 이하.
        GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));

        if (CGeneralGameParam::Instance()->IsNet() == FALSE)
        {
            MSG_CG_SHOP_BUY_STORAGE_LIST_ACK ack;
            ack.page_ = 1;
            ack.total_page_ = 1;
            ack.total_item_count_ = 1;
            ack.item_count_ = BUY_STORAGE_ITEM_PER_PAGE;
            for (int i = 0; i < ack.item_count_; ++i)
            {
                //ack.item_info_[i].storage_sequence_;                         // 보관함 순번
                //ack.item_info_[i].storage_item_sequence_;				    // 보관함 상품 순번 or 캐시 항목 순번
                //ack.item_info_[i].product_sequence_;					        // 단위 상품 코드
                //ack.item_info_[i].price_sequence_;                           // 가격 번호
                //ack.item_info_[i].storage_item_type_;					    // 상품 캐시 구분 (P:상품, C:캐시)
                //ack.item_info_[i].buy_character_name_[MAX_CHARNAME_LENGTH+1];
                ack.item_info_[i].product_sequence_ = 7191 + i;
                ack.item_info_[i].price_sequence_ = 6157 + i;                           // 가격 번호
            }
            NetworkProc(&ack);
        }
    #endif
    }
    //------------------------------------------------------------------------------ 
    void CashShop::SEND_CG_SHOP_GIFT_STORAGE_LIST_SYN(int page_index, int request_count)
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        //------------------------------------------------------------------------------ 
        //! 패킷 2번보내지 않게 체크
        if (PacketRequestManager::Instance()->is_sent(CG_SHOP, CG_SHOP_GIFT_STORAGE_LIST_SYN))
        {
            return;
        }
        PacketRequestManager::Instance()->PushPacket((new MSG_CG_SHOP_GIFT_STORAGE_LIST_SYN),
            (new MSG_CG_SHOP_GIFT_STORAGE_LIST_ACK), 
            (new MSG_CG_SHOP_GIFT_STORAGE_LIST_NAK),
            sizeof(MSG_CG_SHOP_GIFT_STORAGE_LIST_SYN), 
            PACKET_WAITING_MILLISECOND, 
            false);
        //------------------------------------------------------------------------------ 

        if (request_count <= 0)
        {
            return;
        }
        if (request_count > BUY_STORAGE_ITEM_PER_PAGE)
        {
            request_count = BUY_STORAGE_ITEM_PER_PAGE;
        }

        MSG_CG_SHOP_GIFT_STORAGE_LIST_SYN syn;
        syn.page_ = page_index;
        syn.item_count_per_page_ = request_count;   // MAX_STORAGE_ITEM_PER_PAGE 이하.
        GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));

        if (CGeneralGameParam::Instance()->IsNet() == FALSE)
        {
            MSG_CG_SHOP_GIFT_STORAGE_LIST_ACK ack;
            ack.page_ = 1;
            ack.total_page_ = 1;
            ack.total_item_count_ = 1;
            ack.item_count_ = BUY_STORAGE_ITEM_PER_PAGE;
            for (int i = 0; i < ack.item_count_; ++i)
            {
                //ack.item_info_[i].storage_sequence_;                         // 보관함 순번
                //ack.item_info_[i].storage_item_sequence_;				    // 보관함 상품 순번 or 캐시 항목 순번
                //ack.item_info_[i].product_sequence_;					        // 단위 상품 코드
                //ack.item_info_[i].price_sequence_;                           // 가격 번호
                //ack.item_info_[i].storage_item_type_;					    // 상품 캐시 구분 (P:상품, C:캐시)
                //ack.item_info_[i].buy_character_name_[MAX_CHARNAME_LENGTH+1];
                ack.item_info_[i].product_sequence_ = 7195 + i;
                ack.item_info_[i].price_sequence_ = 6171 + i;                           // 가격 번호

                StrnCopy(ack.item_info_[i].send_character_name_, "aaa", MAX_CHARNAME_LENGTH);
                StrnCopy(ack.item_info_[i].recv_character_name_, "bbb", MAX_CHARNAME_LENGTH);
                StrnCopy(ack.item_info_[i].gift_message_, "afasdfasdfasdfasdfsfd", GIFT_MESSAGE_LENGTH);
            }
            NetworkProc(&ack);
        }
    #endif
    }

    //------------------------------------------------------------------------------ 
    void CashShop::SEND_CG_SHOP_EXIST_GIFT_SYN()
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL 
        MSG_CG_SHOP_EXIST_GIFT_SYN syn;
        GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));
    #endif
    }

    //------------------------------------------------------------------------------ 
    void CashShop::SEND_CG_SHOP_CASH_SYN()
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        //------------------------------------------------------------------------------ 
        //! 패킷 2번보내지 않게 체크
        if (PacketRequestManager::Instance()->is_sent(CG_SHOP, CG_SHOP_CASH_SYN))
        {
            return;
        }
        PacketRequestManager::Instance()->PushPacket((new MSG_CG_SHOP_CASH_SYN),
            (new MSG_CG_SHOP_CASH_ACK), 
            (new MSG_CG_SHOP_CASH_NAK),
            sizeof(MSG_CG_SHOP_CASH_SYN), 
            PACKET_WAITING_MILLISECOND, 
            false);
        //------------------------------------------------------------------------------ 

        MSG_CG_SHOP_CASH_SYN syn;
        GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));

        if (CGeneralGameParam::Instance()->IsNet() == FALSE)
        {
            MSG_CG_SHOP_CASH_ACK ack;
            ack.cash_.cash_ = 100000;
            ack.cash_.pp_cash_ = 900000;
            ack.cash_.mileage_ = 800000;
            NetworkProc(&ack);
        }
    #endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    }


    void CashShop::SEND_CG_SHOP_VERSION_SYN()
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        //------------------------------------------------------------------------------ 
        //! 패킷 2번보내지 않게 체크
        if (PacketRequestManager::Instance()->is_sent(CG_SHOP, CG_SHOP_VERSION_SYN))
        {
            return;
        }
        PacketRequestManager::Instance()->PushPacket((new MSG_CG_SHOP_VERSION_SYN),
            (new MSG_CG_SHOP_VERSION_ACK), 
            (new MSG_CG_SHOP_VERSION_NAK),
            sizeof(MSG_CG_SHOP_VERSION_SYN), 
            PACKET_WAITING_MILLISECOND, 
            false);
        //------------------------------------------------------------------------------ 

        MSG_CG_SHOP_VERSION_SYN syn;
        GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));

        if (CGeneralGameParam::Instance()->IsNet() == FALSE)
        {
            MSG_CG_SHOP_VERSION_ACK ack;
            ack.sales_version_.sales_zone_ = 329;
            ack.sales_version_.year_ = 2011;
            ack.sales_version_.year_identity_ = 5; 

            NetworkProc(&ack);
        }
    #endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    }

    void CashShop::SEND_CG_SHOP_OPEN_SYN()
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        //------------------------------------------------------------------------------ 
        //! 패킷 2번보내지 않게 체크
        if (PacketRequestManager::Instance()->is_sent(CG_SHOP, CG_SHOP_OPEN_SYN))
        {
            return;
        }
        PacketRequestManager::Instance()->PushPacket((new MSG_CG_SHOP_OPEN_SYN),
            (new MSG_CG_SHOP_OPEN_ACK), 
            (new MSG_CG_SHOP_OPEN_NAK),
            sizeof(MSG_CG_SHOP_OPEN_SYN), 
            PACKET_WAITING_MILLISECOND, 
            false);
        //------------------------------------------------------------------------------ 

        MSG_CG_SHOP_OPEN_SYN syn;
        syn.sales_version_ = sale_zone_script_version_;
        GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));

        if (CGeneralGameParam::Instance()->IsNet() == FALSE)
        {
            MSG_CG_SHOP_OPEN_ACK ack;
            //ack.cash_info_.cash_ = 100000;
            //ack.cash_info_.pp_cash_ = 900000;
            //ack.cash_info_.mileage_ = 800000;
            NetworkProc(&ack);
        }
    #endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    }

    void CashShop::SEND_CG_SHOP_CLOSE_NTF()
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        MSG_CG_SHOP_CLOSE_NTF ntf;
        GlobalFunc::SendPacketToGameServer(&ntf, sizeof(ntf));
    #endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    }


    //------------------------------------------------------------------------------ 
    void CashShop::SEND_CG_SHOP_USE_STORAGE_ITEM_SYN(StorageItem* pItemData)
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        if( pItemData == NULL )
            return;

        //------------------------------------------------------------------------------ 
        //! 패킷 2번보내지 않게 체크
        if (PacketRequestManager::Instance()->is_sent(CG_SHOP, CG_SHOP_USE_STORAGE_ITEM_SYN))
        {
            return;
        }
        PacketRequestManager::Instance()->PushPacket((new MSG_CG_SHOP_USE_STORAGE_ITEM_SYN),
            (new MSG_CG_SHOP_USE_STORAGE_ITEM_ACK), 
            (new MSG_CG_SHOP_USE_STORAGE_ITEM_NAK),
            sizeof(MSG_CG_SHOP_USE_STORAGE_ITEM_SYN), 
            PACKET_WAITING_MILLISECOND, 
            false);
        //------------------------------------------------------------------------------ 

        if (pItemData == NULL)
        {
            return;
        }

        MSG_CG_SHOP_USE_STORAGE_ITEM_SYN syn;
        syn.use_item_.storage_sequence_ = pItemData->GetItemInfo()->storage_sequence_;
        syn.use_item_.storage_item_sequence_ = pItemData->GetItemInfo()->storage_item_sequence_;
        syn.use_item_.item_type_ = pItemData->GetItemInfo()->storage_item_type_;

        GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));

        if (CGeneralGameParam::Instance()->IsNet() == FALSE) 
        {
            MSG_CG_SHOP_USE_STORAGE_ITEM_ACK ack;
            NetworkProc(&ack);
        }
    #endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    }

    //------------------------------------------------------------------------------ 
    void CashShop::SEND_CG_SHOP_CHECK_CHAR_NAME_SYN(LPCTSTR name) 
    {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        //------------------------------------------------------------------------------ 
        //! 패킷 2번보내지 않게 체크
        if (PacketRequestManager::Instance()->is_sent(CG_SHOP, CG_SHOP_CHAR_NAME_CHECK_SYN))
        {
            return;
        }
        PacketRequestManager::Instance()->PushPacket((new MSG_CG_SHOP_CHAR_NAME_CHECK_SYN),
            (new MSG_CG_SHOP_CHAR_NAME_CHECK_ACK), 
            (new MSG_CG_SHOP_CHAR_NAME_CHECK_NAK),
            sizeof(MSG_CG_SHOP_CHAR_NAME_CHECK_SYN), 
            PACKET_WAITING_MILLISECOND, 
            false);
        //------------------------------------------------------------------------------ 

        MSG_CG_SHOP_CHAR_NAME_CHECK_SYN syn; 
        memset( syn.character_name_, 0, sizeof(char) * MAX_CHARNAME_LENGTH+1 );
        StrnCopy(syn.character_name_, name, MAX_CHARNAME_LENGTH);
        GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn)); 

        if (CGeneralGameParam::Instance()->IsNet() == FALSE)
        {
            static int temp = 0;
            ++temp;
            if (temp % 2)
            {
                MSG_CG_SHOP_CHAR_NAME_CHECK_ACK ack;
                NetworkProc(&ack);
            }
            else
            {
                MSG_CG_SHOP_CHAR_NAME_CHECK_NAK nak;
                NetworkProc(&nak);
            }
        }
    #endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    }

    void CashShop::SendNameCheck( LPCTSTR name )
    {
        SEND_CG_SHOP_CHECK_CHAR_NAME_SYN( name );
    }
};

