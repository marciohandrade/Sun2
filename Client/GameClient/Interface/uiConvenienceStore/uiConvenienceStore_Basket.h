#pragma once

#include "uiBase.h"
#include "uiConvenienceStore_Dialog.h"
#include "CashShopHeader.h"
#include "TabCheckButtonControl.h"
#include "InterfaceManager.h"
#include "SCSlotStruct.h"

class uiConvenienceStoreMan;
class uiConvenienceStore_Dialog;

//------------------------------------------------------------------------------ 
class uiConvenienceStore_Basket : public uiConvenienceStore_Dialog
{
public:
    static const DWORD kDialog_ID;


private:
    TabCheckButtonControl preview_tab_;
    TabCheckButtonControl skin_class_tab_;

    std::vector<ConvenienceStore::ProductItem*> basket_list_;


public:
    std::vector<ConvenienceStore::ProductItem*>& basket_list() { return basket_list_; }
    TabCheckButtonControl& preview_tab() { return preview_tab_; }
    TabCheckButtonControl& skin_class_tab() { return skin_class_tab_; }


public:
    //------------------------------------------------------------------------------
    static void     Callback_Preview(CControlWZ* control_ptr, void* parameter);
    static bool     IsPossiblePreview(const SLOTCODE item_code);
    static bool     IsPossiblePreview(const BASE_ITEMINFO* item_info);
    static bool     HasPreviewItem(ConvenienceStore::ProductItem* product);
    static bool     IsPossibleEquipPreview(const Player* player, const BASE_ITEMINFO* item_info);
    static bool     IsPossibleEquipPreview(const Player* player, ConvenienceStore::ProductItem* const basket);
    static bool     IsPossibleEquipPreview(const BASE_ITEMINFO* rider_item_info, const BASE_ITEMINFO* parts_item_info);
    static bool     IsPossibleEquipPreview(const BASE_ITEMINFO* rider_item_info, ConvenienceStore::ProductItem* const basket);
    //------------------------------------------------------------------------------

public:
    uiConvenienceStore_Basket(InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_Basket(void);

    CASH GetMyMoney();

    bool InsertBasket(ConvenienceStore::ProductItem* product);
    bool RemoveBasket(int basket_index);
    bool RemoveBasket(CShopCategory* shop_category, CShopPackage* shop_package);
    void RemoveBasket();
    void UpdateCashInfo();

    void Cleanup();
    void ClearRider();
    void ClearProduct();
    bool CreatePlayer();
    bool CreateRider();
    bool CreateProduct(SLOTCODE item_code, eCHAR_TYPE skin_class_type);
    bool CreateProduct(ConvenienceStore::ProductItem* product);
    void UpdateEquipControl();

    void SetActiveProductTab();

    int FindBasketIndex(const ConvenienceStore::ProductItem* basket);
    int FindBasketIndex(const CShopCategory* shop_category, const CShopPackage* shop_package);
#ifdef _NA_000000_20130114_RENEWER_UI
	void SetChangeBackgroundIsClass(eCHAR_TYPE charater_type);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI


private:
    virtual DWORD player_key() { return CONVENIENCESTORE_CASH_CHARACTER_KEY; }

    virtual void Init(CDrawBase* drawbase);
    virtual void Release(void);
    virtual void OnShowWindow(BOOL val);
    virtual void Process(DWORD tick);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog(); 
    virtual void NetworkProc(MSG_BASE* msgbase){}
    virtual void MessageProc(SI_MESSAGE* message);

private:
    enum PreviewType
    {
        kPreview_Default = 0,
        kPreview_Player = 0,
        kPreview_Rider,
#ifdef _NA_004035_20120227_COSTUME_ITEM
        kPreview_Costum,
#endif//_NA_004035_20120227_COSTUME_ITEM
        kPreview_Product,
        kPreview_Size,
    };
    PreviewType uiConvenienceStore_Basket::GetCurrentPreviewType();


    virtual Player* player();
    virtual bool IsPossibleShopPopup();
    virtual bool UpdatePopupMenu(bool is_possible, bool is_now_show_tooltip);
    virtual void UpdateTooltip(bool is_possible);


private:
    //------------------------------------------------------------------------------ 
    virtual bool OnLButtonClick(SI_MESSAGE* message);
    virtual bool OnLDButtonClick(SI_MESSAGE* message);
    virtual bool OnLButtonDown(SI_MESSAGE* message);
    virtual bool OnRButtonClick(SI_MESSAGE* message);
    virtual bool OnMouseOver(SI_MESSAGE* message){ return false; }
    virtual bool OnLButtonClick_Basket(SI_MESSAGE* message);
    virtual bool OnLButtonClick_Equip(SI_MESSAGE* message);

    virtual bool CreateProductPart(const BASE_ITEMINFO* item_info, eCHAR_TYPE skin_class_type);
    virtual bool CreateProductPet(const BASE_ITEMINFO* item_info);
#ifdef _NA_004035_20120227_COSTUME_ITEM
    virtual bool CreateProductCostume(const BASE_ITEMINFO* item_info, eCHAR_TYPE skin_class_type);
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	virtual bool CreateProductDecorateCostume(const BASE_ITEMINFO* item_info, eCHAR_TYPE skin_class_type);
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
    virtual bool CreateProductRider(const BASE_ITEMINFO* item_info);
    virtual bool CreateProductPlayer(ConvenienceStore::ProductItem* basket, 
                                    eCHAR_TYPE char_type, 
                                    GenderType::Value genter_type); //!< 패키지 아이템 미리보기

    bool OnClickPopupMenu(const uiConvenienceStore_Dialog::PopupCommandType command_type, const DWORD user_data);

    //PreviewProduct::product 기준으로 상품 미리보기 성별을 바꾼다.
    void ChangeProductBySkinClass();
    void ProcessPlayer(DWORD tick);
    void ProcessRider(DWORD tick);
    void ProcessProduct(DWORD tick);
    void RenderPlayer(CControlWZ* picture);
    void RenderRider(CControlWZ* picture);
    void RenderProduct(CControlWZ* picture);

    bool IsEquipedPlayerParts(const ConvenienceStore::ProductItem* basket);
    CControlWZ* GetPlayerEquipUIControl(const BASE_ITEMINFO* item_info);
    //------------------------------------------------------------------------------
    //! Returns:   bool
    //! Qualifier:
    //! Parameter: Player * player
    //! Parameter: const BASE_ITEMINFO * item_info : 부착될 아이템 정보
    //! Parameter: PreviewType preiview_type : UI control에 정보를 저장할지 여부 : 
    //!             PreviewType == kPreview_Rider 일때만 CControlWZ::SetUserData를 한다.
    //------------------------------------------------------------------------------
    bool EquipPlayerParts(Player* player, const BASE_ITEMINFO* item_info, PreviewType preiview_type);
    bool EquipPlayerParts(Player* player, ConvenienceStore::ProductItem* basket, PreviewType preiview_type);
    bool UnequipPlayerParts(const BASE_ITEMINFO* item_info);
    void UnequipPlayerParts(ConvenienceStore::ProductItem* basket);
    void UnequipPlayerParts();

    bool IsEquipedRiderParts(const ConvenienceStore::ProductItem* basket);
    CControlWZ* GetRiderEquipUIControl(const BASE_ITEMINFO* item_info);
    //------------------------------------------------------------------------------
    //! Returns:   bool
    //! Qualifier:
    //! Parameter: Rider * rider
    //! Parameter: const BASE_ITEMINFO * rider_item_info : 탈것 아이템 정보
    //! Parameter: const BASE_ITEMINFO * parts_item_info : 탈것에 부탁될 파츠 아이템 정보
    //! Parameter: PreviewType preiview_type : UI control에 정보를 저장할지 여부 : 
    //!             PreviewType == kPreview_Rider 일때만 CControlWZ::SetUserData를 한다.
    //------------------------------------------------------------------------------
    bool EquipRiderParts(Rider* rider, 
                        const BASE_ITEMINFO* rider_item_info, 
                        const BASE_ITEMINFO* parts_item_info, 
                        PreviewType preiview_type);
    bool EquipRiderParts(Rider* rider, 
                        const BASE_ITEMINFO* rider_item_info, 
                        ConvenienceStore::ProductItem* parts_basket, 
                        PreviewType preiview_type);
    bool UnequipRiderParts(const BASE_ITEMINFO* item_info);
    void UnequipRiderParts(ConvenienceStore::ProductItem* basket);
    void UnequipRiderParts();

    void UpdateUI();
    void TurnLeft(float radin = 0.2f);
    void TurnRight(float radin = 0.2f);

    CASH GetBasketPrice();

private:

    //------------------------------------------------------------------------------ 
    struct PreviewScreenInfo
    {
        float rot;  //! 회전 radin_value 애니메이션값.
        float zoom;
        FPOINT org;

        POINT mouse_pos;
        void reset()
        {
            mouse_pos.x = 0;
            mouse_pos.y = 0;
            rot = PI;
            zoom = 1.0f;
            org.x = 0;
            org.y = 0;
        }
        void check_valid_value()
        {
            if (rot > (PI * 2.0f))  rot -= (PI * 2.0f);
            if (rot < 0.0f)         rot += (PI * 2.0f);
            if (zoom > 1.0f)        zoom = 1.0f;
            if (zoom < 0.15f)       zoom = 0.15f;

            if (org.x >  1.5f)      org.x =  1.5f;
            if (org.x < -1.5f)      org.x = -1.5f;
            if (org.y >  1.5f)      org.y =  1.5f;
            if (org.y < -1.5f)      org.y = -1.5f;
        }
    };

    //------------------------------------------------------------------------------ 
    PreviewScreenInfo& screen_info() { return screen_info_; }

private:
    PreviewScreenInfo screen_info_;
};

//#endif //#ifdef _YMS_TREECONTROL