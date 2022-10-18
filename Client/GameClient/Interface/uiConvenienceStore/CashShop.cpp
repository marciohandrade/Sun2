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
                _T("./log"),    //_T("D:/Perforce/test_Dev_release/2.�ֽŰ��߹���/SUN_Client/log"),
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
                _T("./log"),    //_T("D:/Perforce/test_Dev_release/2.�ֽŰ��߹���/SUN_Client/log"),
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
                _T("./log"),    //_T("D:/Perforce/test_Dev_release/2.�ֽŰ��߹���/SUN_Client/log"),
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
                    // ������ ������ �����˸� �÷��״� �ϴ� ����
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

                //! ĳ�� ���� ��û
                SEND_CG_SHOP_CASH_SYN();

                //! ��Ű�� ������ ������ �־ �ٽ� ÷���� ��û�Ѵ�.
                SEND_CG_SHOP_BUY_STORAGE_LIST_SYN(1,10);

                MSG_CG_SHOP_BUY_ITEM_ACK* ack = static_cast<MSG_CG_SHOP_BUY_ITEM_ACK*>(pk);
                OnSuccessBuyItem( ack->item_info_, ack->item_count_ );
            }
            break;
        case CG_SHOP_SEND_GIFT_ACK:
            {
                OnEndWaiting();

                //! ĳ�� ���� ��û
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

                // ���� �������� �̸��� ����
                if( StorageItemInfo* buy_info = ItemData.FindStorageItem(ack->use_item_.storage_sequence_, ack->use_item_.storage_item_sequence_, ack->use_item_.item_type_) )
                {
                    CShopProduct* shop_product = NULL;
                    if ( shop_list_manager_->GetListPtr()->GetProductListPtr()->GetValueByKey(buy_info->product_sequence_, shop_product) == true)
                    {
                        Snprintf( message, 1024-1, "%s %d%s", shop_product->ProductName, ack->inventory_info_.m_InvenCount, g_InterfaceManager.GetInterfaceString(155565) );
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
                    }

                    // ����ҿ��� ����
                    ItemData.EraseStorageItem( ack->use_item_.storage_sequence_, ack->use_item_.storage_item_sequence_, ack->use_item_.item_type_ );
                }

                bool is_continue = false;

                if( is_all_item_receive_ )
                {
                    // ��� �ޱ��� ��� ���� �ִٸ� ��� ������
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
                    //! �� �޾����� ����� �ٽ� ��û�Ѵ�.
                    int page_index = ItemData.GetPage();
                    if( ItemData.GetCountPerPage() == 0 )
                    {
                        // ���� �������� 0���� �Ǿ��ٸ�,
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


                //! ����� �ٽ� ��û�Ѵ�.
                StorageItemList& ItemData = is_my_item_receive_ ? GetStorageItemData() : GetGiftStorageItemData();
                int page_index = ItemData.GetPage();
                if( ItemData.GetCountPerPage() == 0 )
                {
                    // ���� �������� 0���� �Ǿ��ٸ�,
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


        // ��Ʈ�� �ڵ�� 6791���� �������
        DWORD string_code = 6791 + error_code - (DWORD)(RC::RC_SHOP_PROCESS_TRANSACTION);


        if( error_code == RC::RC_SHOP_EVENT_ITEM_GIFT_IMPOSSIBLE )
            string_code = 6822;
        else if( error_code == RC::RC_SHOP_EVENT_ITEM_GIFT_COUNT_OVER )
            string_code = 6823;
        else if( error_code == RC::RC_SHOP_LIMIT_CASH_TO_GIFT )
            string_code = 6824;
		else if( error_code == RC::RC_SHOP_FAILED )
			string_code = 6827;

        // ������ ���� ����
        WzColor text_color = WzColor_RGBA(129,254,82,255);   // default color

        if( error_code == RC::RC_SHOP_ALREADY_EXSIT_USER ||
            error_code == RC::RC_SHOP_INVENTORY_FULL ||
            error_code == RC::RC_SHOP_EVENT_ITEM_BUY_IMPOSSIBLE ||
            error_code == RC::RC_SHOP_NOT_ENOUGH_CASH 
            )
        {
            // ������ ����
            text_color = WzColor_RGB(197, 47, 47);
        }



        // ��Ÿ �߰� �۾�
        if( error_code == RC::RC_SHOP_ALREADY_EXSIT_USER )
        {
            //! �� ������ ����.
            SEND_CG_SHOP_CLOSE_NTF();
            OnHideCashShop();
        }



        if( string_color != NULL )
        {
            *string_color = text_color;
        }

        // ���� ���
        {
            TCHAR szMessage[INTERFACE_STRING_LENGTH] ={NULL, };
            g_InterfaceManager.GetInterfaceString(string_code, szMessage, INTERFACE_STRING_LENGTH);
            _tcscat(output_msg, szMessage);

            //! ���� �ڵ� ���
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
        //! ��Ŷ 2�������� �ʰ� üũ
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

            syn.item_info_[syn.item_count_].product_display_sequence_ = purchase->GetShopCategory()->ProductDisplaySeq;  // ī�װ� ��ȣ
            syn.item_info_[syn.item_count_].package_product_sequence_ = purchase->GetShopPackage()->PackageProductSeq;          // ��Ű�� ��ȣ
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
                ack.item_info_[i].product_display_sequence_ = syn.item_info_[i].product_display_sequence_;  // ī�װ� ��ȣ
                ack.item_info_[i].package_product_sequence_ = syn.item_info_[i].package_product_sequence_;          // ��Ű�� ��ȣ
                ack.item_info_[i].price_sequence_ = syn.item_info_[i].price_sequence_;            // ���� ��ȣ
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
        //! ��Ŷ 2�������� �ʰ� üũ
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
            syn.item_info_[syn.item_count_].product_display_sequence_ = purchase->GetDisplaySequence(); // ī�װ� ��ȣ
            syn.item_info_[syn.item_count_].package_product_sequence_ = purchase->GetPackageSequence(); // ��Ű�� ��ȣ
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
        //! ��Ŷ 2�������� �ʰ� üũ
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
        syn.item_count_per_page_ = request_count;   // MAX_STORAGE_ITEM_PER_PAGE ����.
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
                //ack.item_info_[i].storage_sequence_;                         // ������ ����
                //ack.item_info_[i].storage_item_sequence_;				    // ������ ��ǰ ���� or ĳ�� �׸� ����
                //ack.item_info_[i].product_sequence_;					        // ���� ��ǰ �ڵ�
                //ack.item_info_[i].price_sequence_;                           // ���� ��ȣ
                //ack.item_info_[i].storage_item_type_;					    // ��ǰ ĳ�� ���� (P:��ǰ, C:ĳ��)
                //ack.item_info_[i].buy_character_name_[MAX_CHARNAME_LENGTH+1];
                ack.item_info_[i].product_sequence_ = 7191 + i;
                ack.item_info_[i].price_sequence_ = 6157 + i;                           // ���� ��ȣ
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
        //! ��Ŷ 2�������� �ʰ� üũ
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
        syn.item_count_per_page_ = request_count;   // MAX_STORAGE_ITEM_PER_PAGE ����.
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
                //ack.item_info_[i].storage_sequence_;                         // ������ ����
                //ack.item_info_[i].storage_item_sequence_;				    // ������ ��ǰ ���� or ĳ�� �׸� ����
                //ack.item_info_[i].product_sequence_;					        // ���� ��ǰ �ڵ�
                //ack.item_info_[i].price_sequence_;                           // ���� ��ȣ
                //ack.item_info_[i].storage_item_type_;					    // ��ǰ ĳ�� ���� (P:��ǰ, C:ĳ��)
                //ack.item_info_[i].buy_character_name_[MAX_CHARNAME_LENGTH+1];
                ack.item_info_[i].product_sequence_ = 7195 + i;
                ack.item_info_[i].price_sequence_ = 6171 + i;                           // ���� ��ȣ

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
        //! ��Ŷ 2�������� �ʰ� üũ
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
        //! ��Ŷ 2�������� �ʰ� üũ
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
        //! ��Ŷ 2�������� �ʰ� üũ
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
        //! ��Ŷ 2�������� �ʰ� üũ
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
        //! ��Ŷ 2�������� �ʰ� üũ
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

