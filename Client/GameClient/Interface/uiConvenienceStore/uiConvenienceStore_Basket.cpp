#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_Basket.h"
#include "uiConvenienceStore_Ask.h"
#include "uiConvenienceStore_PopupMenuFour.h"
#include "uiConvenienceStore_PopupMenuFive.h"
#include "uiConvenienceStore_PopupMenuTwo.h"
#include "uiConvenienceStoreMan.h"
#include "InterfaceManager.h"
#include "Player.h"
#include "Hero.h"
#include "uiConvenienceStore_Purchase.h"
#include "uiConvenienceStore_Gift.h"
#include "ItemManager.h"
#include "Character.h"
#include "ItemInfoParser.h"
#include "EquipmentContainer.h"
#include "Mouse.h"
#include "SolarDialog.h"
#include "ObjectManager.h"
#include "SCItemSlot.h"
#include "uiConvenienceStore_Counter.h"
#include "uiConvenienceStore_PopupPackage.h"
#include "uiTargetMan\uiTargetMan.h"
#include "uiRadarMan\uiRadarMan.h"
#include "GlobalFunc.h"
#include "uiSkillMan\uiNewSkillMan.h"
#include "uiToolTipMan\uiToolTipMan.h"
#include "ResourceManager.h"
#include "GameFunc.h"
#include "PetInfoParser.h"
#include "AppearanceResourceCode.h"
#include "Application.h"
#include "uiConvenienceStore_PopupMenuThree.h"
#include "CharacterCreationInfoParser.h"
#include "ProductInfo.h"
#include "Map.h"


//------------------------------------------------------------------------------ 
const DWORD uiConvenienceStore_Basket::kDialog_ID = StrToWzID("4720");;
const int MAX_BASKET_SLOT_COUNT = 30;   // 장바구니 슬롯의 최대 갯수

//------------------------------------------------------------------------------ 
struct
{   
    //PreviewType == kPreview_Rider
    Rider* rider;
    const BASE_ITEMINFO* item_info;   //!< rider item info
}PreviewRider;

struct
{
   //PreviewType == kPreview_Product
    CWzUnitDraw* parts;
    Pet* pet;
    Rider* rider;
//    DWORD character_key;
    DWORD character_key;

    //! 성별에 따라서 미리보기가 달라야하는 상품 때문에 백업본을 하나 가지고 있음.
    //! 종료시 delete 필요.
    //! 단일(캐릭터에 입큅 가능한 : parts) 상품과, 패키지 상품(캐릭터 : character_key)에만 사용됨
    //eCHAR_TYPE skin_class_type;
    ConvenienceStore::ProductItem* product;
}PreviewProduct;

//------------------------------------------------------------------------------ 
//155485	(총액: %s%s)
enum UIControlPos
{
    kControl_CheckBT_C000 = 0,
    kControl_CheckBT_C002,
    kControl_CheckBT_C003,

    // 클래스 버튼
    kControl_CheckBT_C001,
    kControl_CheckBT_C004,
    kControl_CheckBT_C005,
    kControl_CheckBT_C006,
    kControl_CheckBT_C007,
    kControl_CheckBT_C008, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    kControl_CheckBT_C009,
    kControl_CheckBT_C010,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#ifdef _NA_20111026_CASHSHOP_CHECKBUTTON_TOOLTIP
    kControl_Dummy_001,     // 체크박스에 툴팁을 표시하기 위한 더미 이미지
    kControl_Dummy_002,
    kControl_Dummy_003,
    kControl_Dummy_004,
    kControl_Dummy_005,
#endif

    kControl_Button_BT12,   //<! 장바구니 합계
    kControl_Button_BT13,   //<! 소지 캐쉬
    kControl_Button_B004,   //<! 닫기
    kControl_Button_B000,   //<! 캐쉬 충전하기
    kControl_Button_BT07,   //<! 해방군의훈장마크
    kControl_Button_B001,   //<! 모두 선물
    kControl_Button_B002,   //<! 모두 받기
    kControl_Button_B021,   //<! 장바구니 비우기
    kControl_Button_B003,   //<! 장바구니 슬롯 시작
    kControl_Button_B034,
    kControl_Button_B035,
    kControl_Button_B036,
    kControl_Button_B037,
    kControl_Button_B038,
    kControl_Button_B039,
    kControl_Button_B040,
    kControl_Button_B041,
    kControl_Button_B042,
    kControl_Button_B043,
    kControl_Button_B044,
    kControl_Button_B045,
    kControl_Button_B046,
    kControl_Button_B047,
    kControl_Button_B048,
    kControl_Button_B049,
    kControl_Button_B050,
    kControl_Button_B051,
    kControl_Button_B052,
    kControl_Button_B053,
    kControl_Button_B054,
    kControl_Button_B055,
    kControl_Button_B056,
    kControl_Button_B057,
    kControl_Button_B058,
    kControl_Button_B059,
    kControl_Button_B060,
    kControl_Button_B061,
    kControl_Button_B062,   //<! 장바구니 슬롯 끝

    //! 장착
    kControl_Button_B005,
    kControl_Button_B006,
    kControl_Button_B007,
    kControl_Button_B008,
    kControl_Button_B009,
    kControl_Button_B010,
    kControl_Button_B011,
    kControl_Button_B012,
    kControl_Button_B013, //!< 패키지 전용

    kControl_Button_B014,

    // 탈것 장착 버튼
    kControl_Button_B015,
    kControl_Button_B016,
    kControl_Button_B017,
    kControl_Button_B018, //!<패키지

    kControl_Text_ST05, //<! 소지금 위
    kControl_Text_ST02, //<! 소지금 아래
    kControl_Text_ST04, //<! 소지금 아래
    kControl_Text_T057, //<! 소지금 아래

    kControl_Text_ST20, //<! 소지금 위
    kControl_Text_ST21, //<! 소지금 dkfo
    kControl_Text_ST03, //!< 해방군의 훈장
    kControl_Text_ST12, //!< 소지금

    kControl_Text_S001,
    kControl_Text_S002,
    kControl_Text_S000,
    kControl_Picture_P002,
    kControl_Picture_PI11, //<! 소지금 위
    kControl_Picture_PI12, //<! 소지금 아래
    kControl_Picture_PI13,
    kControl_Picture_P005,
    kControl_Picture_P000,
    kControl_Picture_P001,
    kControl_Picture_P008,
    kControl_Picture_P003,
    kControl_Picture_P004,

    kControl_Picture_P009, //<! 미리보기를 표시할 수 없는경우 표시되는 이미지
    kControl_Text_S003, //<! 미리보기를 표시할 수 없는경우 표시되는 안내문
#ifdef _NA_000000_20130114_RENEWER_UI
	//배경이미지
	kControl_Picture_006,//버서커
	kControl_Picture_012,//드나
	kControl_Picture_013,//엘리
	kControl_Picture_014,//미스틱
	kControl_Picture_018,//새도우
	kControl_Picture_019,//발키리
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

    kControl_Size,
};

//------------------------------------------------------------------------------
//! static function
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::IsPossibleEquipPreview(const Player* player, const BASE_ITEMINFO* item_info)
{
    if (item_info == NULL) 
    {
        return false;
    } 
    if ((item_info->IsWeapon() == true) || //				= 11,	//	오브1
        (item_info->m_wType == eITEMTYPE_ARMOR) || //				= 101,	//	갑옷
        (item_info->m_wType == eITEMTYPE_HELMET) || //			= 301,	//	헬멧
        (item_info->m_wType == eITEMTYPE_PANTS) || //				= 401,	//	바지
        (item_info->m_wType == eITEMTYPE_BOOTS) || //				= 501,	//	부츠
        (item_info->m_wType == eITEMTYPE_GLOVE) ||
#ifdef _NA_004035_20120227_COSTUME_ITEM
        (item_info->m_wType == eITEMTYPE_COSTUME) || //코스튬
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
		(item_info->m_wType == eITEMTYPE_COSTUME_DECORATE) || //데코코스튬
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
        (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_FACE) || //				= 501,	//	부츠
        (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_HAIR))
    {
        if (ItemManager::Instance()->CanEquipClass((Player*)player, (BASE_ITEMINFO*)item_info) == TRUE)
        {
            return true;
        }
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::IsPossibleEquipPreview(const Player* player, ConvenienceStore::ProductItem* const basket)
{
    if ((basket == NULL) || (basket->is_lotto() == true))
    {
        return false;
    }
    if (basket->is_package() == false)
    {
        return uiConvenienceStore_Basket::IsPossibleEquipPreview(player, basket->first_info()->GetItemInfo());
    }

    std::vector<ConvenienceStore::ProductInfo>::iterator begin = basket->GetInfoVector().begin();
    std::vector<ConvenienceStore::ProductInfo>::iterator end = basket->GetInfoVector().end();
    for (; begin != end; ++begin)
    {
        if (uiConvenienceStore_Basket::IsPossibleEquipPreview(player, begin->GetItemInfo()) == true)
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::IsPossibleEquipPreview(const BASE_ITEMINFO* rider_item_info, 
                                                       const BASE_ITEMINFO* parts_item_info)
{
    if (rider_item_info == NULL || parts_item_info == NULL) 
    {
        return false;
    } 
    if (rider_item_info->IsRiderItem() && parts_item_info->IsRiderPartsItem())
    {
        return (rider_item_info->m_RidingClass.value == parts_item_info->m_RidingClass.value);
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::IsPossibleEquipPreview(const BASE_ITEMINFO* rider_item_info, 
                                                       ConvenienceStore::ProductItem* const basket)
{
    if ((basket == NULL) || (basket->is_lotto() == true))
    {
        return false;
    }
    if (basket->is_package() == false)
    {
        return uiConvenienceStore_Basket::IsPossibleEquipPreview(rider_item_info, basket->first_info()->GetItemInfo());
    }

    std::vector<ConvenienceStore::ProductInfo>::iterator begin = basket->GetInfoVector().begin();
    std::vector<ConvenienceStore::ProductInfo>::iterator end = basket->GetInfoVector().end();
    for (; begin != end; ++begin)
    {
        if (uiConvenienceStore_Basket::IsPossibleEquipPreview(rider_item_info, begin->GetItemInfo()) == true)
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::IsPossiblePreview(const SLOTCODE item_code)
{
    const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code); 

    return uiConvenienceStore_Basket::IsPossiblePreview(item_info);
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::IsPossiblePreview(const BASE_ITEMINFO* item_info)
{
    if (item_info == NULL)
    {
        return false;
    } 
    if ((item_info->m_wType == eITEMTYPE_TWOHANDAXE) || //		= 1,	//	양손도끼1
        (item_info->m_wType == eITEMTYPE_TWOHANDSWORD) || //		= 2,	//	양손검1
        (item_info->m_wType == eITEMTYPE_ONEHANDSWORD) || //		= 3,	//	한손검1
        (item_info->m_wType == eITEMTYPE_SPEAR) || //				= 4,	//	창1
        (item_info->m_wType == eITEMTYPE_DAGGER) || //			= 5,	//	단검1
        (item_info->m_wType == eITEMTYPE_WHIP) || //				= 6,	//	채찍1
        (item_info->m_wType == eITEMTYPE_ONEHANDCROSSBOW) || //	= 7,	//	한손석궁1
        (item_info->m_wType == eITEMTYPE_ETHERWEAPON) || //		= 8,	//	양손석궁1,에테르웨폰
        (item_info->m_wType == eITEMTYPE_SCIMITAR) || //			= 9,	//	곡도1
        (item_info->m_wType == eITEMTYPE_STAFF) || //				= 10,	//	지팡이1
        (item_info->m_wType == eITEMTYPE_ORB) || //				= 11,	//	오브1
        (item_info->m_wType == eITEMTYPE_FISHING_POLE) || //		= 12,	//  낚시대
        (item_info->m_wType == eITEMTYPE_ARMOR) || //				= 101,	//	갑옷
        //(item_info->m_wType == eITEMTYPE_PROTECTOR) || //			= 201,	//	프로텍터
        (item_info->m_wType == eITEMTYPE_HELMET) || //			= 301,	//	헬멧
        (item_info->m_wType == eITEMTYPE_PANTS) || //				= 401,	//	바지
        (item_info->m_wType == eITEMTYPE_BOOTS) || //				= 501,	//	부츠
        (item_info->m_wType == eITEMTYPE_GLOVE) || //				= 601,	//	장갑
        //(item_info->m_wType == eITEMTYPE_BELT) || //				= 701,	//	벨트
        //(item_info->m_wType == eITEMTYPE_SHIRTS) || //			= 801,	//	셔츠
        (item_info->m_wType == eITEMTYPE_BERSERKER_SACCESSORY) || //	= 831,	//	망토
        (item_info->m_wType == eITEMTYPE_DRAGON_SACCESSORY) || //		= 841,	//	날개
        (item_info->m_wType == eITEMTYPE_VALKYRIE_SACCESSORY) || //	= 851,	//	속성석
        (item_info->m_wType == eITEMTYPE_SHADOW_SACCESSORY) || //		= 861,	//	수호정령, 제네레이터
        (item_info->m_wType == eITEMTYPE_ELEMENTALIST_SACCESSORY) || // = 871,	//	문장
        (item_info->m_wType == eITEMTYPE_BERSERKER_KALBUS_ACCESSORY) || //	= 881,
        (item_info->m_wType == eITEMTYPE_DRAGON__KALBUS_ACCESSORY) || //		= 882,
        (item_info->m_wType == eITEMTYPE_VALKYRIE__KALBU_ACCESSORY) || //		= 883,
        (item_info->m_wType == eITEMTYPE_ELEMENTALIST_KALBUS_ACCESSORY) || // = 884,
        (item_info->m_wType == eITEMTYPE_SHADOW_KALBU_ACCESSORY) || //		= 888,
        (item_info->m_wType == eITEMTYPE_WINGS) || 
        (item_info->m_wType == eITEMTYPE_PET) || 
        (item_info->m_wType == eITEMTYPE_RIDER) || 
        (item_info->m_wType == eITEMTYPE_RIDER_PARTS) || 
        (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_FACE) || 
        (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_HAIR) || 
#ifdef _NA_004035_20120227_COSTUME_ITEM
        (item_info->m_wType == eITEMTYPE_COSTUME) || //코스튬
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
		(item_info->m_wType == eITEMTYPE_COSTUME_DECORATE) || //데코코스튬
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
        //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        (item_info->m_wType == eITEMTYPE_HELLROID_1) || 
        (item_info->m_wType == eITEMTYPE_HELLROID_SPECIAL_ACCESSORY1) || 
        (item_info->m_wType == eITEMTYPE_HELLROID_SPECIAL_ACCESSORY2) ||
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        (item_info->m_wType == eITEMTYPE_ARCBLADE) || 
        (item_info->m_wType == eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY1) || 
        (item_info->m_wType == eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY2) ||
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        (item_info->m_wType == eITEMTYPE_GUNTLET) || 
        (item_info->m_wType == eITEMTYPE_POLEARM) || 
        (item_info->m_wType == eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY1) || 
        (item_info->m_wType == eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY2))

    {
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::HasPreviewItem(ConvenienceStore::ProductItem* product)
{
    std::vector<ConvenienceStore::ProductInfo>::iterator begin = product->GetInfoVector().begin();
    std::vector<ConvenienceStore::ProductInfo>::iterator end = product->GetInfoVector().end();
    for (; begin != end; ++ begin)
    {
        if (uiConvenienceStore_Basket::IsPossiblePreview(begin->GetItemInfo()))
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------ 
uiConvenienceStore_Basket::PreviewType uiConvenienceStore_Basket::GetCurrentPreviewType()
{
    DWORD data;
    preview_tab().Current( data );
    return (uiConvenienceStore_Basket::PreviewType)data;
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::Callback_Preview(CControlWZ* control_ptr, void* parameter)
{
    uiConvenienceStore_Basket* onwer = static_cast<uiConvenienceStore_Basket*>(parameter);
    if ((control_ptr != NULL) && (onwer != NULL))
    {
        DWORD user_data = 0;
        if (onwer->preview_tab().Current(user_data) != NULL)
        {
            PreviewType preiview = static_cast<PreviewType>(user_data);
            if (preiview == uiConvenienceStore_Basket::kPreview_Player)
            {
                onwer->RenderPlayer(control_ptr);
            }
            else if (preiview == uiConvenienceStore_Basket::kPreview_Rider)
            {
                onwer->RenderRider(control_ptr);
            }
            else if (preiview == uiConvenienceStore_Basket::kPreview_Product)
            {
                onwer->RenderProduct(control_ptr);
            }
        }
    }

}

//------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------ 
uiConvenienceStore_Basket::uiConvenienceStore_Basket(InterfaceManager* ui_man) : uiConvenienceStore_Dialog(ui_man)
{
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);

    // 클래스별 버튼
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
    RegisterControlPosition(StrToWzID("C007"), kControl_CheckBT_C007);
    RegisterControlPosition(StrToWzID("C008"), kControl_CheckBT_C008); //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C009"), kControl_CheckBT_C009); 
    RegisterControlPosition(StrToWzID("C010"), kControl_CheckBT_C010); 
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    RegisterControlPosition(StrToWzID("BT12"), kControl_Button_BT12);
    RegisterControlPosition(StrToWzID("BT13"), kControl_Button_BT13);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("BT07"), kControl_Button_BT07);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B021"), kControl_Button_B021);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B034"), kControl_Button_B034);
    RegisterControlPosition(StrToWzID("B035"), kControl_Button_B035);
    RegisterControlPosition(StrToWzID("B036"), kControl_Button_B036);
    RegisterControlPosition(StrToWzID("B037"), kControl_Button_B037);
    RegisterControlPosition(StrToWzID("B038"), kControl_Button_B038);
    RegisterControlPosition(StrToWzID("B039"), kControl_Button_B039);
    RegisterControlPosition(StrToWzID("B040"), kControl_Button_B040);
    RegisterControlPosition(StrToWzID("B041"), kControl_Button_B041);
    RegisterControlPosition(StrToWzID("B042"), kControl_Button_B042);
    RegisterControlPosition(StrToWzID("B043"), kControl_Button_B043);
    RegisterControlPosition(StrToWzID("B044"), kControl_Button_B044);
    RegisterControlPosition(StrToWzID("B045"), kControl_Button_B045);
    RegisterControlPosition(StrToWzID("B046"), kControl_Button_B046);
    RegisterControlPosition(StrToWzID("B047"), kControl_Button_B047);
    RegisterControlPosition(StrToWzID("B048"), kControl_Button_B048);
    RegisterControlPosition(StrToWzID("B049"), kControl_Button_B049);
    RegisterControlPosition(StrToWzID("B050"), kControl_Button_B050);
    RegisterControlPosition(StrToWzID("B051"), kControl_Button_B051);
    RegisterControlPosition(StrToWzID("B052"), kControl_Button_B052);
    RegisterControlPosition(StrToWzID("B053"), kControl_Button_B053);
    RegisterControlPosition(StrToWzID("B054"), kControl_Button_B054);
    RegisterControlPosition(StrToWzID("B055"), kControl_Button_B055);
    RegisterControlPosition(StrToWzID("B056"), kControl_Button_B056);
    RegisterControlPosition(StrToWzID("B057"), kControl_Button_B057);
    RegisterControlPosition(StrToWzID("B058"), kControl_Button_B058);
    RegisterControlPosition(StrToWzID("B059"), kControl_Button_B059);
    RegisterControlPosition(StrToWzID("B060"), kControl_Button_B060);
    RegisterControlPosition(StrToWzID("B061"), kControl_Button_B061);
    RegisterControlPosition(StrToWzID("B062"), kControl_Button_B062);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("B013"), kControl_Button_B013);
    RegisterControlPosition(StrToWzID("B014"), kControl_Button_B014);
    RegisterControlPosition(StrToWzID("B015"), kControl_Button_B015);
    RegisterControlPosition(StrToWzID("B016"), kControl_Button_B016);
    RegisterControlPosition(StrToWzID("B017"), kControl_Button_B017);
    RegisterControlPosition(StrToWzID("B018"), kControl_Button_B018);

    RegisterControlPosition(StrToWzID("ST05"), kControl_Text_ST05);
    RegisterControlPosition(StrToWzID("ST02"), kControl_Text_ST02);
    RegisterControlPosition(StrToWzID("ST04"), kControl_Text_ST04);
    RegisterControlPosition(StrToWzID("T057"), kControl_Text_T057);
    RegisterControlPosition(StrToWzID("ST20"), kControl_Text_ST20);
    RegisterControlPosition(StrToWzID("ST21"), kControl_Text_ST21);
    RegisterControlPosition(StrToWzID("ST03"), kControl_Text_ST03);
    RegisterControlPosition(StrToWzID("ST12"), kControl_Text_ST12);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("PI11"), kControl_Picture_PI11);
    RegisterControlPosition(StrToWzID("PI12"), kControl_Picture_PI12);
    RegisterControlPosition(StrToWzID("PI13"), kControl_Picture_PI13);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);

    RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);

#ifdef _NA_20111026_CASHSHOP_CHECKBUTTON_TOOLTIP
    RegisterControlPosition(StrToWzID("P010"), kControl_Dummy_001);
    RegisterControlPosition(StrToWzID("P011"), kControl_Dummy_002);
    RegisterControlPosition(StrToWzID("P012"), kControl_Dummy_003);
    RegisterControlPosition(StrToWzID("P013"), kControl_Dummy_004);
    RegisterControlPosition(StrToWzID("P014"), kControl_Dummy_005);
#endif

#ifdef _NA_000000_20130114_RENEWER_UI
	RegisterControlPosition(StrToWzID("P006"), kControl_Picture_006);
	RegisterControlPosition(StrToWzID("P012"), kControl_Picture_012);
	RegisterControlPosition(StrToWzID("P013"), kControl_Picture_013);
	RegisterControlPosition(StrToWzID("P014"), kControl_Picture_014);
	RegisterControlPosition(StrToWzID("P018"), kControl_Picture_018);
	RegisterControlPosition(StrToWzID("P019"), kControl_Picture_019);
#endif //#ifdef _NA_000000_20130114_RENEWER_UI

    basket_list().clear();

    screen_info().reset();

    //------------------------------------------------------------------------------ 
    PreviewRider.rider = NULL;
    PreviewRider.item_info = NULL;
    PreviewProduct.parts = NULL;
    PreviewProduct.pet = NULL;
    PreviewProduct.rider = NULL;
    PreviewProduct.product = NULL;
    //PreviewProduct.skin_class_type = eCHAR_NONE;
    PreviewProduct.character_key = CONVENIENCESTORE_PREVIEW_CHARACTER_KEY;
}

//------------------------------------------------------------------------------ 
uiConvenienceStore_Basket::~uiConvenienceStore_Basket( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
Player* uiConvenienceStore_Basket::player()
{
    return g_ObjectManager.GetPlayer(player_key());
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::Init(CDrawBase* drawbase)
{
    if (GetControlID_byPosition(kControl_CheckBT_C001) &&
        GetControlID_byPosition(kControl_CheckBT_C004) &&
        GetControlID_byPosition(kControl_CheckBT_C005) &&
        GetControlID_byPosition(kControl_CheckBT_C006) &&
        GetControlID_byPosition(kControl_CheckBT_C007) &&
        GetControlID_byPosition(kControl_CheckBT_C008) &&
        GetControlID_byPosition(kControl_CheckBT_C009) &&
        GetControlID_byPosition(kControl_CheckBT_C010) &&
        GetControlID_byPosition(kControl_CheckBT_C000) &&
        GetControlID_byPosition(kControl_CheckBT_C002) &&
        GetControlID_byPosition(kControl_CheckBT_C003))
    {
        skin_class_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C001)), eCHAR_BERSERKER);
        skin_class_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C004)), eCHAR_DRAGON);
        skin_class_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C005)), eCHAR_SHADOW);
        skin_class_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C006)), eCHAR_VALKYRIE);
        skin_class_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C007)), eCHAR_ELEMENTALIST);
        skin_class_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C008)), eCHAR_MYSTIC); //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        skin_class_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C009)), eCHAR_HELLROID);
        skin_class_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C010)), eCHAR_WITCHBLADE);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        skin_class_tab().Click(eCHAR_NONE, true);

        preview_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C000)), uiConvenienceStore_Basket::kPreview_Player);
        preview_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C002)), uiConvenienceStore_Basket::kPreview_Product);
        preview_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C003)), uiConvenienceStore_Basket::kPreview_Rider);
    }

    preview_tab().Click(uiConvenienceStore_Basket::kPreview_Player);
    UpdateEquipControl();

    if (GetControlWZ_byPosition(kControl_Text_ST05) &&
        GetControlWZ_byPosition(kControl_Text_ST02) &&
        GetControlWZ_byPosition(kControl_Text_ST05) &&
        GetControlWZ_byPosition(kControl_Text_ST02) &&
        GetControlWZ_byPosition(kControl_Text_ST04) &&
        GetControlWZ_byPosition(kControl_Text_T057) &&
        GetControlWZ_byPosition(kControl_Text_ST20) &&
        GetControlWZ_byPosition(kControl_Text_ST21) &&
        GetControlWZ_byPosition(kControl_Text_ST03) &&
        GetControlWZ_byPosition(kControl_Text_ST12) &&
        GetControlWZ_byPosition(kControl_Picture_PI12) &&
        GetControlWZ_byPosition(kControl_Button_BT07) &&
        GetControlWZ_byPosition(kControl_Picture_P009) &&
        GetControlWZ_byPosition(kControl_Text_S003))
    {
        GetControlWZ_byPosition(kControl_Text_ST05)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Text_ST02)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Text_ST05)->SetMoney(0);
        GetControlWZ_byPosition(kControl_Text_ST02)->SetMoney(0);

        GetControlWZ_byPosition(kControl_Text_ST04)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Text_T057)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Text_ST20)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Text_ST21)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Text_ST03)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Text_ST12)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Picture_PI12)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Button_BT07)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Picture_P009)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Text_S003)->ShowWindowWZ(WZ_HIDE);
    }

    GetDialogWZ()->SetCallbackTooltip(uiToolTipMan::Callback_Tooltip);

    if (GetControlWZ_byPosition(kControl_Picture_P003))
        GetControlWZ_byPosition(kControl_Picture_P003)->SetCallbackDraw(uiConvenienceStore_Basket::Callback_Preview, this);

#ifdef _NA_000000_20130114_RENEWER_UI
    if (GetControlWZ_byPosition(kControl_Picture_006) &&
        GetControlWZ_byPosition(kControl_Picture_012) &&
        GetControlWZ_byPosition(kControl_Picture_013) &&
        GetControlWZ_byPosition(kControl_Picture_014) &&
        GetControlWZ_byPosition(kControl_Picture_018) &&
        GetControlWZ_byPosition(kControl_Picture_019))
    {
        GetControlWZ_byPosition(kControl_Picture_006)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Picture_012)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Picture_013)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Picture_014)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Picture_018)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Picture_019)->ShowWindowWZ(WZ_HIDE);
    }
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

    ASSERT(true && "uiConvenienceStore_Basket::Init false");
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::Release(void)
{
    Cleanup();
    preview_tab().Release();
    skin_class_tab().Release();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::Cleanup()
{
    if (IsLoaded() == FALSE)
    {
        return;
    }
    
    UnequipPlayerParts(); 
    UnequipRiderParts(); 
    RemoveBasket();

    screen_info().reset(); 
    preview_tab().Click(uiConvenienceStore_Basket::kPreview_Player);
    UpdateEquipControl();

    g_ObjectManager.Delete(player_key());

    //------------------------------------------------------------------------------ 
    for (int i = kControl_Button_B003; i < kControl_Button_B062; ++i)
    {
        if (GetControlWZ_byPosition(i))
            GetControlWZ_byPosition(i)->SetUserData(NULL);
    }
    for (int i = kControl_Button_B005; i < kControl_Button_B013; ++i)
    {
        if (GetControlWZ_byPosition(i))
            GetControlWZ_byPosition(i)->SetUserData(NULL);
    }
    for (int i = kControl_Button_B015; i < kControl_Button_B018; ++i)
    {
        if (GetControlWZ_byPosition(i))
            GetControlWZ_byPosition(i)->SetUserData(NULL);
    }

    //------------------------------------------------------------------------------ 
    ClearRider();
    ClearProduct();

}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::OnShowWindow(BOOL val)
{
    if (val == TRUE) 
    {
        if (GetControlWZ_byPosition(kControl_Text_ST05) &&
            GetControlWZ_byPosition(kControl_Text_ST02) &&
            GetControlWZ_byPosition(kControl_Text_ST04) &&
            GetControlWZ_byPosition(kControl_Text_T057) &&
            GetControlWZ_byPosition(kControl_Text_ST20) &&
            GetControlWZ_byPosition(kControl_Text_ST21) &&
            GetControlWZ_byPosition(kControl_Text_ST03) &&
            GetControlWZ_byPosition(kControl_Text_ST12) &&
            GetControlWZ_byPosition(kControl_Picture_PI11) &&
            GetControlWZ_byPosition(kControl_Picture_PI12) &&
            GetControlWZ_byPosition(kControl_Button_BT07))
        {
            GetControlWZ_byPosition(kControl_Text_ST05)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Text_ST02)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Text_ST04)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Text_T057)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Text_ST20)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Text_ST21)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Text_ST03)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Text_ST12)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Picture_PI11)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Picture_PI12)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Button_BT07)->ShowWindowWZ(WZ_HIDE);
        }

        // PPCard 와 Mileage 등을 사용할 경우 표시
#if defined(_GLOBAL)
        if (GetControlWZ_byPosition(kControl_Picture_PI12) &&
            GetControlWZ_byPosition(kControl_Picture_PI11) &&
            GetControlWZ_byPosition(kControl_Text_ST02) &&
            GetControlWZ_byPosition(kControl_Text_ST05) &&
            GetControlWZ_byPosition(kControl_Text_ST20) &&
            GetControlWZ_byPosition(kControl_Text_ST21))
        {
            GetControlWZ_byPosition(kControl_Picture_PI12)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Picture_PI11)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_ST02)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_ST05)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_ST20)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_ST21)->ShowWindowWZ(WZ_SHOW);
        }
#elif defined(_CHINA)
        if (GetControlWZ_byPosition(kControl_Picture_PI12) &&
            GetControlWZ_byPosition(kControl_Picture_PI11) &&
            GetControlWZ_byPosition(kControl_Text_ST02) &&
            GetControlWZ_byPosition(kControl_Text_ST05) &&
            GetControlWZ_byPosition(kControl_Text_ST20) &&
            GetControlWZ_byPosition(kControl_Text_ST21))
        {
            GetControlWZ_byPosition(kControl_Picture_PI12)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Picture_PI11)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_ST02)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_ST05)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_ST20)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_ST21)->ShowWindowWZ(WZ_SHOW);
        }
#else //_KOREA
        if (GetControlWZ_byPosition(kControl_Picture_PI12) &&
            GetControlWZ_byPosition(kControl_Text_ST02) &&
            GetControlWZ_byPosition(kControl_Text_ST12))
        {
            GetControlWZ_byPosition(kControl_Picture_PI12)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_ST02)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_ST12)->ShowWindowWZ(WZ_SHOW);
        }
#endif //

        CreatePlayer();
        CreateRider();
        UpdateEquipControl();

#ifdef _NA_000000_20130114_RENEWER_UI
        //캐릭터에 맞는 배경 선택
        SetChangeBackgroundIsClass(g_pHero->GetClass());
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
    }
    else
    {
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::UpdateCashInfo()
{
    if (IsLoaded() == FALSE)
    {
        return;
    }

#if defined(_GLOBAL)
    if (GetControlWZ_byPosition(kControl_Text_ST05))
        GetControlWZ_byPosition(kControl_Text_ST05)->SetMoney(static_cast<ULONGLONG>(ConvenienceStore::CashShop::instance()->GetMyCash()));
    if (GetControlWZ_byPosition(kControl_Text_ST02))
        GetControlWZ_byPosition(kControl_Text_ST02)->SetMoney(static_cast<ULONGLONG>(ConvenienceStore::CashShop::instance()->GetMyPPCash()));
#elif defined(_CHINA)
    if (GetControlWZ_byPosition(kControl_Text_ST02))
        GetControlWZ_byPosition(kControl_Text_ST02)->SetMoney(static_cast<ULONGLONG>(ConvenienceStore::CashShop::instance()->GetMyCash()));
    if (GetControlWZ_byPosition(kControl_Text_ST05))
        GetControlWZ_byPosition(kControl_Text_ST05)->SetMoney(static_cast<ULONGLONG>(ConvenienceStore::CashShop::instance()->GetMyMileage()));
#else //_KOREA
    if (GetControlWZ_byPosition(kControl_Text_ST02))
        GetControlWZ_byPosition(kControl_Text_ST02)->SetMoney(static_cast<ULONGLONG>(ConvenienceStore::CashShop::instance()->GetMyCash()));
#endif //
}

CASH uiConvenienceStore_Basket::GetMyMoney()
{
    return static_cast<CASH>(ConvenienceStore::CashShop::instance()->GetMyCash());
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::OnUpdateSolarDialog()
{
    bool is_possible = IsPossibleShopPopup();
    UpdatePopupMenu(is_possible, false);
    UpdateTooltip(is_possible);

    UpdateUI();

    if (GetDialogWZ()->HaveFocus() == true)
    {
        GlobalFunc::MoveToTopDialog(GetManager()->counter()->GetDialogWZ()->GetDlgID());
    }

    ////------------------------------------------------------------------------------ 
    //if ((GetControlWZ_byPosition(kControl_Button_BT12)->GetState() == WZ_SHOW) && 
    //    (IsOverControl(GetControlWZ_byPosition(kControl_Button_BT12)) == true) && 
    //    (Mouse::Instance()->LButtonPressed() == TRUE))
    //{
    //    TurnLeft();
    //}
    //else if ((GetControlWZ_byPosition(kControl_Button_BT13)->GetState() == WZ_SHOW) && 
    //    (IsOverControl(GetControlWZ_byPosition(kControl_Button_BT13)) == true) &&
    //    (Mouse::Instance()->LButtonPressed() == TRUE))
    //{
    //    TurnRight();
    //}

#if defined(_GLOBAL) || defined(_CHINA)
    if (GetControlWZ_byPosition(kControl_Button_B001))
    {
        ENUM_DRAWSTATE draw_state = GetControlWZ_byPosition(kControl_Button_B001)->GetDrawState();
        if( basket_list().empty() )
        {
            if (draw_state != E_DISABLE)
            {
                if (GetControlWZ_byPosition(kControl_Button_B001))
                    GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(false);
                if (GetControlWZ_byPosition(kControl_Button_B002))
                    GetControlWZ_byPosition(kControl_Button_B002)->EnableWindowWZ(false);
            }
        }
        else
        {
            if (draw_state == E_DISABLE)
            {
                if (GetControlWZ_byPosition(kControl_Button_B001))
                    GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(true);
                if (GetControlWZ_byPosition(kControl_Button_B002))
                    GetControlWZ_byPosition(kControl_Button_B002)->EnableWindowWZ(true);
            }
        }
    }
#else
    //------------------------------------------------------------------------------ 
    // 선물 구매 버튼을 가격에 따라 활성화/비활성화 시킬 것인가
    CASH basket_price = GetBasketPrice();
    CASH my_price = GetMyMoney();
    if (GetControlWZ_byPosition(kControl_Button_B001))
    {
        ENUM_DRAWSTATE draw_state = GetControlWZ_byPosition(kControl_Button_B001)->GetDrawState();
        if ((my_price < basket_price) || (basket_list().size() <= 0)) 
        {
            if (draw_state != E_DISABLE)
            {
                if (GetControlWZ_byPosition(kControl_Button_B001))
                    GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(false);
                if (GetControlWZ_byPosition(kControl_Button_B002))
                    GetControlWZ_byPosition(kControl_Button_B002)->EnableWindowWZ(false);
            }
        }
        else
        {
            if (draw_state == E_DISABLE)
            {
                if (GetControlWZ_byPosition(kControl_Button_B001))
                    GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(true);
                if (GetControlWZ_byPosition(kControl_Button_B002))
                    GetControlWZ_byPosition(kControl_Button_B002)->EnableWindowWZ(true);
            }
        }
    }
#endif
        
    //------------------------------------------------------------------------------ 
    if (GetControlWZ_byPosition(kControl_Picture_P003) &&
        (IsOverControl(GetControlWZ_byPosition(kControl_Picture_P003)) == true) &&
        (g_InterfaceManager.IsLockDlg() == FALSE))
    {
        RECT area = GetControlWZ_byPosition(kControl_Picture_P003)->GetSizeRect(); 
        BYTE wheel_msg = uiBase::GetMouseWheelStatus(&area);

        switch (wheel_msg)
        {
            case MOUSE_WHEEL_UP:    screen_info().zoom -= (0.1f * screen_info().zoom); break;
            case MOUSE_WHEEL_DOWN:  screen_info().zoom += (0.1f * screen_info().zoom); break;
        } 
        if (Mouse::Instance()->LButtonPressed() == TRUE)
        {
            screen_info().org.x -= ((Mouse::Instance()->GetMouseX() - screen_info().mouse_pos.x) * 0.005f);
            screen_info().org.y += ((Mouse::Instance()->GetMouseY() - screen_info().mouse_pos.y) * 0.005f);
        }
        screen_info().check_valid_value();
    }
    screen_info().mouse_pos.x = Mouse::Instance()->GetMouseX();
    screen_info().mouse_pos.y = Mouse::Instance()->GetMouseY();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::Process(DWORD tick)
{
    //------------------------------------------------------------------------------ 
    if (GetControlWZ_byPosition(kControl_Button_BT12) &&
        (GetControlWZ_byPosition(kControl_Button_BT12)->GetState() == WZ_SHOW) && 
        (IsOverControl(GetControlWZ_byPosition(kControl_Button_BT12)) == true) && 
        (Mouse::Instance()->LButtonPressed() == TRUE))
    {
        TurnLeft(tick * 0.002f);
    }
    else if (GetControlWZ_byPosition(kControl_Button_BT13) &&
        (GetControlWZ_byPosition(kControl_Button_BT13)->GetState() == WZ_SHOW) && 
        (IsOverControl(GetControlWZ_byPosition(kControl_Button_BT13)) == true) &&
        (Mouse::Instance()->LButtonPressed() == TRUE))
    {
        TurnRight(tick * 0.002f);
    }

    //------------------------------------------------------------------------------ 
    DWORD user_data = 0;
    if (preview_tab().Current(user_data) != NULL)
    {
        PreviewType preiview = static_cast<PreviewType>(user_data);
        if (preiview == kPreview_Player)
        {
            ProcessPlayer(tick);
        }
        else if (preiview == kPreview_Rider)
        {
            ProcessRider(tick);
        }
        else if (preiview == kPreview_Product)
        {
            ProcessProduct(tick);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::OnRenderSolarDialog()
{
    DWORD user_data = 0;
    if (preview_tab().Current(user_data) != NULL)
    {
        PreviewType preiview = static_cast<PreviewType>(user_data);
        if (preiview == kPreview_Player)
        {
            //RenderPlayer(GetControlWZ_byPosition(kControl_Picture_P003));
            for (int i = kControl_Button_B005; i <= kControl_Button_B013; ++i)
            {
                CControlWZ* control = GetControlWZ_byPosition(i);
                if (control == NULL)
                {
                    continue;
                }

                ConvenienceStore::ProductItem* basket = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData());
                if (basket != NULL && basket->IsProduct())
                {
                    if (basket->GetIconImage() != NULL)
                    {
                        basket->GetIconImage()->Render(&(control->GetSizeRect()));
                    }
                }
            }
        }
        else if (preiview == kPreview_Rider)
        {
            //RenderRider(GetControlWZ_byPosition(kControl_Picture_P003));
            for (int i = kControl_Button_B015; i <= kControl_Button_B018; ++i)
            {
                CControlWZ* control = GetControlWZ_byPosition(i);
                ConvenienceStore::ProductItem* product = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData());
                if (product != NULL)
                {
                    product->GetIconImage()->Render(&(control->GetSizeRect()));
                }
            }
        }
        else if (preiview == kPreview_Product)
        {
            //RenderProduct(GetControlWZ_byPosition(kControl_Picture_P003));
        }
    }

    //------------------------------------------------------------------------------ 
    std::vector<ConvenienceStore::ProductItem*>::iterator begin = basket_list().begin();
    std::vector<ConvenienceStore::ProductItem*>::iterator end = basket_list().end();
    int control_index = 0;
    for (; (begin != end) && (control_index < MAX_BASKET_SLOT_COUNT); ++begin, ++control_index)
    {
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_B003 + control_index));
        if (control != NULL && (*begin)->GetIconImage() != NULL)
        {
            (*begin)->GetIconImage()->Render(&(control->GetSizeRect()));
        }
    }
}
//------------------------------------------------------------------------------
void uiConvenienceStore_Basket::MessageProc(SI_MESSAGE* message)
{
    if (IsLock()) 
    {
        return;
    }
    if (GetManager() == NULL)
    {
        return;
    }

    if (preview_tab().MessageProc(message))
    {
        UpdateEquipControl();
        return;
    }
    if (skin_class_tab().MessageProc(message))
    {
        ChangeProductBySkinClass();
        return;
    }
    
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_LBUTTONDCLICK:
        OnLDButtonClick(message);
        break;
    case RT_MSG_LBUTTONDOWN:
        break;
    case RT_MSG_RBUTTONCLICK:
        OnRButtonClick(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------
bool uiConvenienceStore_Basket::OnLButtonDown(SI_MESSAGE* message)
{
    bool result = true; 
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Picture_P003:
        screen_info().mouse_pos.x = Mouse::Instance()->GetMouseX();
        screen_info().mouse_pos.y = Mouse::Instance()->GetMouseY();
        break;
    }

    return result;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::OnLDButtonClick(SI_MESSAGE* message)
{
    bool result = true; 
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B003:
    case kControl_Button_B034:
    case kControl_Button_B035:
    case kControl_Button_B036:
    case kControl_Button_B037:
    case kControl_Button_B038:
    case kControl_Button_B039:
    case kControl_Button_B040:
    case kControl_Button_B041:
    case kControl_Button_B042:
    case kControl_Button_B043:
    case kControl_Button_B044:
    case kControl_Button_B045:
    case kControl_Button_B046:
    case kControl_Button_B047:
    case kControl_Button_B048:
    case kControl_Button_B049:
    case kControl_Button_B050:
    case kControl_Button_B051:
    case kControl_Button_B052:
    case kControl_Button_B053:
    case kControl_Button_B054:
    case kControl_Button_B055:
    case kControl_Button_B056:
    case kControl_Button_B057:
    case kControl_Button_B058:
    case kControl_Button_B059:
    case kControl_Button_B060:
    case kControl_Button_B061:
    case kControl_Button_B062:
        {
            ////! 바로 구매
            //CControlWZ* control = GetControlWZ(message->dwCtrlID);
            //ConvenienceStore::ProductItem* basket = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData()); 
            //if (basket != NULL)
            //{
            //    GetManager()->purchase()->ShowInterface(true, 
            //        uiConvenienceStore_Purchase::kPurchase_Buy,
            //        (TCHAR*)(g_pHero->GetName()),
            //        _T(""),
            //        basket_type(),
            //        basket);
            //}
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
bool uiConvenienceStore_Basket::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true; 
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B003:
    case kControl_Button_B034:
    case kControl_Button_B035:
    case kControl_Button_B036:
    case kControl_Button_B037:
    case kControl_Button_B038:
    case kControl_Button_B039:
    case kControl_Button_B040:
    case kControl_Button_B041:
    case kControl_Button_B042:
    case kControl_Button_B043:
    case kControl_Button_B044:
    case kControl_Button_B045:
    case kControl_Button_B046:
    case kControl_Button_B047:
    case kControl_Button_B048:
    case kControl_Button_B049:
    case kControl_Button_B050:
    case kControl_Button_B051:
    case kControl_Button_B052:
    case kControl_Button_B053:
    case kControl_Button_B054:
    case kControl_Button_B055:
    case kControl_Button_B056:
    case kControl_Button_B057:
    case kControl_Button_B058:
    case kControl_Button_B059:
    case kControl_Button_B060:
    case kControl_Button_B061:
    case kControl_Button_B062:
        {
            OnLButtonClick_Basket(message);
            if (GetManager()->popup_menu_five()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupMenuFive::kDialog_ID);
            }
            if (GetManager()->popup_menu_three()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupMenuThree::kDialog_ID);
            }
            if (GetManager()->popup_menu_four()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupMenuFour::kDialog_ID);
            }
            if (GetManager()->popup_menu_two()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupMenuTwo::kDialog_ID);
            }
        }
        break;

    case kControl_CheckBT_C000:
        {
        }
        break;
    case kControl_CheckBT_C002:
        {
        }
        break;
    case kControl_CheckBT_C003:
        {
        }
        break;
    case kControl_Button_B004:
        {
            ConvenienceStore::CashShop::instance()->HideCashShop();
        }
        break;
    case kControl_Button_B000:
        {
#if defined(_GLOBAL) || defined(_CHINA)
            //! 캐시 충전     //155566	http://www.sunonline.co.kr
            GlobalFunc::ExecWebSite(155566);
            ////GetManager()->SEND_CG_SHOP_CLOSE_NTF();

#else
            // 6825	준비중입니다. 웹페이지에서 확인해주세요.
            GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_Message);
            GetManager()->ask()->AppendFText(g_InterfaceManager.GetInterfaceString( 6825 ));
            GetManager()->ask()->EnableCloseButton();
#endif
        }
        break;
    case kControl_Button_BT07:
        {
        }
        break;
    case kControl_Button_B001:
        {
            //모두 선물

            bool poosible_gift = false;

            for( size_t i=0;i!=basket_list().size();++i)
            {
                ConvenienceStore::ProductItem* item = basket_list().at(i);
                if( item->is_possible_gift() == true )
                {
                    poosible_gift = true;
                    break;
                }
            }

            if( poosible_gift == true )
            {
                GetManager()->gift()->ShowInterface(TRUE, basket_list());
            }
            else
            {
                GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_Message );
                GetManager()->ask()->AppendFText(g_InterfaceManager.GetInterfaceString(155457));
                GetManager()->ask()->EnableCloseButton();
            }
        }
        break;
    case kControl_Button_B021:
        {
            //모두 비우기
            RemoveBasket();
        }
        break;
    case kControl_Button_B002:
        {
            // 모두 구입

#ifdef _GLOBAL
            // 글로벌은 결제 방식을 먼저 선택한다.
            GetManager()->cashtype()->SetBuyProductInfo(NULL,-1);
            GetManager()->cashtype()->OpenSelectCashType();
#else
            GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Buy);

            for( size_t i=0;i!=basket_list().size();++i)
            {
                GetManager()->purchase()->InsertPurchase(basket_list().at(i), 0);
            }
#endif

            //std::vector<ConvenienceStore::ProductItem*>::iterator begin = basket_list().begin();
            //std::vector<ConvenienceStore::ProductItem*>::iterator end = basket_list().end();
            //int index = 0;
            //for (; begin != end; ++begin, ++index)
            //{
            //    GetManager()->purchase()->InsertPurchase(*begin, index);
            //}
        }
        break;
    case kControl_Button_B005:
    case kControl_Button_B006:
    case kControl_Button_B007:
    case kControl_Button_B008:
    case kControl_Button_B009:
    case kControl_Button_B010:
    case kControl_Button_B011:
    case kControl_Button_B012:
    case kControl_Button_B013:
    case kControl_Button_B015:
    case kControl_Button_B016:
    case kControl_Button_B017:
    case kControl_Button_B018:
        {
            OnLButtonClick_Equip(message);
            if (GetManager()->popup_menu_five()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupMenuFive::kDialog_ID);
            }
            if (GetManager()->popup_menu_three()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupMenuThree::kDialog_ID);
            }
            if (GetManager()->popup_menu_four()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupMenuFour::kDialog_ID);
            }
            if (GetManager()->popup_menu_two()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupMenuTwo::kDialog_ID);
            }
        }
        break;

    case kControl_Button_B014:
        {
            DWORD user_data = 0;
            if (preview_tab().Current(user_data) != NULL)
            {
                PreviewType preiview = static_cast<PreviewType>(user_data);
                if (preiview == uiConvenienceStore_Basket::kPreview_Player)
                {
                    UnequipPlayerParts(); 
                }
                else if (preiview == uiConvenienceStore_Basket::kPreview_Rider)
                {
                    UnequipRiderParts();
                }
            }
        }
        break;

    //case kControl_Button_B015:
    //case kControl_Button_B016:
    //case kControl_Button_B017:
    //    {
    //        //탈것 장착 버튼
    //    }
    //    break;

    case kControl_Text_ST05:
        {
        }
        break;
    case kControl_Text_ST02:
        {
        }
        break;
    case kControl_Text_ST04:
        {
        }
        break;
    case kControl_Text_T057:
        {
        }
        break;
    case kControl_Text_ST20:
        {
        }
        break;
    case kControl_Text_ST21:
        {
        }
        break;
    case kControl_Text_ST03:
        {
        }
        break;
    case kControl_Text_ST12:
        {
        }
        break;
    case kControl_Text_S001:
        {
        }
        break;
    case kControl_Text_S002:
        {
        }
        break;
    case kControl_Text_S000:
        {
        }
        break;
    case kControl_Picture_P002:
        {
        }
        break;
    case kControl_Picture_PI11:
        {
        }
        break;
    case kControl_Picture_PI12:
        {
        }
        break;
    case kControl_Picture_PI13:
        {
        }
        break;
    case kControl_Picture_P005:
        {
        }
        break;
    case kControl_Picture_P000:
        {
        }
        break;
    case kControl_Picture_P001:
        {
        }
        break;
    case kControl_Picture_P003:
        {
        }
        break;
    case kControl_Picture_P004:
        {
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
bool uiConvenienceStore_Basket::OnRButtonClick(SI_MESSAGE* message)
{
    if (GetControlPosition_byID(message->dwCtrlID) == NULL)
    {
        return false;
    }

    bool result = true; 
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B003:
    case kControl_Button_B034:
    case kControl_Button_B035:
    case kControl_Button_B036:
    case kControl_Button_B037:
    case kControl_Button_B038:
    case kControl_Button_B039:
    case kControl_Button_B040:
    case kControl_Button_B041:
    case kControl_Button_B042:
    case kControl_Button_B043:
    case kControl_Button_B044:
    case kControl_Button_B045:
    case kControl_Button_B046:
    case kControl_Button_B047:
    case kControl_Button_B048:
    case kControl_Button_B049:
    case kControl_Button_B050:
    case kControl_Button_B051:
    case kControl_Button_B052:
    case kControl_Button_B053:
    case kControl_Button_B054:
    case kControl_Button_B055:
    case kControl_Button_B056:
    case kControl_Button_B057:
    case kControl_Button_B058:
    case kControl_Button_B059:
    case kControl_Button_B060:
    case kControl_Button_B061:
    case kControl_Button_B062:
        {
            //장바구니
            int index = GetControlPosition_byID(message->dwCtrlID) - kControl_Button_B003;
            if (index < static_cast<int>(basket_list().size()))
            {
                std::vector<ConvenienceStore::ProductItem*>::const_reference basket = basket_list().at(index);

                if (basket->is_lotto() == false)
                {
                    if (IsEquipedRiderParts(basket) || IsEquipedPlayerParts(basket))
                    {
                        UnequipPlayerParts(basket);
                        UnequipRiderParts(basket);
                    }
                    else
                    {
                        if (basket->has_preiview_player_equip_parts() && 
                            uiConvenienceStore_Basket::IsPossibleEquipPreview(player(), basket))
                        {
                            OnClickPopupMenu(uiConvenienceStore_Dialog::kPopupCommand_Basket_Equip, (DWORD)basket);
                        }
                        else if (basket->has_preiview_rider_equip_parts() && 
                            uiConvenienceStore_Basket::IsPossibleEquipPreview(PreviewRider.item_info, basket))
                        {
                            OnClickPopupMenu(uiConvenienceStore_Dialog::kPopupCommand_Basket_Equip, (DWORD)basket);
                        }
                    }
                    //! 팝업 메뉴가 떠 있으면 없앤다.
                    GetManager()->popup_menu_five()->ShowInterface(FALSE);
                }
            }
        }
        break;

    case kControl_Button_B005:
    case kControl_Button_B006:
    case kControl_Button_B007:
    case kControl_Button_B008:
    case kControl_Button_B009:
    case kControl_Button_B010:
    case kControl_Button_B011:
    case kControl_Button_B012:
    case kControl_Button_B013:
    case kControl_Button_B015:
    case kControl_Button_B016:
    case kControl_Button_B017:
    case kControl_Button_B018:
        {
            //장착해제
            CControlWZ* control = GetControlWZ(message->dwCtrlID);
            if (control != NULL && control->GetUserData() != NULL)
            {
                ConvenienceStore::ProductItem* basket = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData());
                UnequipPlayerParts(basket);
                UnequipRiderParts(basket);

                //! 팝업 메뉴가 떠 있으면 없앤다.
                GetManager()->popup_menu_three()->ShowInterface(FALSE);
            }
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
bool uiConvenienceStore_Basket::InsertBasket(ConvenienceStore::ProductItem* product)
{
    if (static_cast<int>(basket_list().size()) >= MAX_BASKET_SLOT_COUNT)
    {
        return false;
    }

    ConvenienceStore::ProductItem* basket = ConvenienceStore::ProductItem::CreateProduct(product);
    if (basket != NULL)
    {
        basket_list().push_back(basket);
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::RemoveBasket()
{
    std::vector<ConvenienceStore::ProductItem*>::iterator begin = basket_list().begin();
    std::vector<ConvenienceStore::ProductItem*>::iterator end = basket_list().end();
    for (; begin != end; ++ begin)
    {
        ConvenienceStore::ProductItem* basket = (*begin);
        UnequipRiderParts(basket); 
        UnequipPlayerParts(basket); 
        SAFE_DELETE(basket)
    }

    basket_list().clear();
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::RemoveBasket(int basket_index)
{
    if (basket_index < 0 || basket_index >= static_cast<int>(basket_list().size()))
    {
        return false; 
    }

    std::vector<ConvenienceStore::ProductItem*>::iterator iter = basket_list().begin() + basket_index;
    {
        ConvenienceStore::ProductItem* basket = (*iter);
        UnequipRiderParts(basket); 
        UnequipPlayerParts(basket); 

        SAFE_DELETE(basket)
    }
    basket_list().erase(iter);

    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::RemoveBasket(CShopCategory* shop_category, CShopPackage* shop_package)
{
    int index = FindBasketIndex(shop_category, shop_package);
    return RemoveBasket(index);
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::UpdateTooltip(bool is_possible)
{ 
    if (is_possible == false)
    {
        return; 
    }

    if (GetManager()->is_show_popup_menu())
    {
        if ((GetManager()->popup_package()->IsVisible() == TRUE) && 
            (GetManager()->popup_package()->owner() == this))
        {
            GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
        }
        return;
    }

    //! 묶음 상품 툴팁이 떠 있는지 확인한다.
    if (GetManager()->popup_package()->IsVisible() == TRUE)
    {
        if ((GetManager()->popup_package()->InterSectRect(Mouse::Instance()->CheckMousePos()) == TRUE) ||
            (IsOverControl(GetManager()->popup_package()->owner_control()) == true))
        {
            return;
        }
    }

    if ( skin_class_tab().IsVisible() == true )
    {
        // 클래스 체크
        // kControl_CheckBT_C008 //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        for (int i = kControl_CheckBT_C001; i <= kControl_CheckBT_C010; ++i )
#else
        for (int i = kControl_CheckBT_C001; i <= kControl_CheckBT_C008; ++i )
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        {
            if( CControlWZ* control = GetControlWZ_byPosition(i) )
            {
                if( IsOverControl(control) == true )
                {
                    uiToolTipMan* man = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
                    if (man != NULL)
                    {
                        static int iClassNameCode[eCHAR_TYPE_MAX - eCHAR_BERSERKER] = { 
                            11382, //(B)
                            11385, //(D)
                            11388, //(S)
                            11383, //(V)
                            11384, //(E)
                            2568,  //(M) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
                            2569, //(H)
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                            10008, //(W)
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                        };

                        int class_index = i-kControl_CheckBT_C001;
                        man->RegisterTooltipWithStringCode( iClassNameCode[class_index] );
						man->SetTooltipDialogID(GetDialogKey());

                    }
                }
            }
        }
    }

    if (GetCurrentPreviewType() == kPreview_Player )
    {
        for (int i = kControl_Button_B005; i <= kControl_Button_B013; ++i)
        {
            if( CControlWZ* control = GetControlWZ_byPosition(i) )
            {
                if( IsOverControl(control) == true )
                {
                    if( ConvenienceStore::ProductItem* basket = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData()) )
                    {
                        if( basket->IsProduct() )
                        {
                            GetManager()->RegisterTooltip(basket);
                            GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
                        }
                    }
                }
            }
        }
    }


    //------------------------------------------------------------------------------ 
    std::vector<ConvenienceStore::ProductItem*>::iterator begin = basket_list().begin();
    std::vector<ConvenienceStore::ProductItem*>::iterator end = basket_list().end();
    int control_index = 0;
    for (; (begin != end) && (control_index < MAX_BASKET_SLOT_COUNT); ++begin, ++control_index)
    {
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_B003 + control_index));
        if ((control != NULL) && (IsOverControl(control) == true))
        {
            ConvenienceStore::ProductItem* basket = *begin;
            if (basket->is_package() == true)
            {
                GetManager()->popup_package()->ShowInterface(TRUE, basket);
                GetManager()->popup_package()->Reset();
                GetManager()->popup_package()->set_owner(this, control);

                int cx = control->GetSizeRect().left;
                int cy = control->GetSizeRect().bottom;
                POINT screen_pos = g_pApplication->Get2DModeMapping();
                RECT dlg_rect;
                GetManager()->popup_package()->GetDialogWZ()->GetDlgSize(&dlg_rect);
                if (cx + dlg_rect.right > screen_pos.x)
                {
                    cx = screen_pos.x - dlg_rect.right;
                }
                GetManager()->popup_package()->MoveWindowWZ(cx, cy);
            }
            else
            {
                GetManager()->RegisterTooltip(basket);
                GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
            }
            return;
        }
    }
    GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::UpdatePopupMenu(bool is_possible, bool is_now_show_tooltip)
{
    if (uiConvenienceStore_Dialog::UpdatePopupMenu(is_possible, is_now_show_tooltip) == false)
    {
        return false;
    }

    GetManager()->popup_menu_five()->ShowInterface(FALSE);
    GetManager()->popup_menu_four()->ShowInterface(FALSE);
    GetManager()->popup_menu_three()->ShowInterface(FALSE);

    return true;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::UpdateEquipControl()
{
    if (IsLoaded() == FALSE)
    {
        return;
    }

    DWORD user_data = 0;
    if (preview_tab().Current(user_data) == NULL)
    {
        return;
    }
    PreviewType preiview_type = static_cast<PreviewType>(user_data);

    eCHAR_TYPE char_class = static_cast<eCHAR_TYPE>(skin_class_tab().CurrentUserData());


    //------------------------------------------------------------------------------ 
    if (GetControlWZ_byPosition(kControl_Picture_P001))
        GetControlWZ_byPosition(kControl_Picture_P001)->ShowWindowWZ(WZ_HIDE);
    if (GetControlWZ_byPosition(kControl_Picture_P008))
        GetControlWZ_byPosition(kControl_Picture_P008)->ShowWindowWZ(WZ_HIDE);
    for (int i = kControl_Button_B005; i <= kControl_Button_B013; ++i)
    {
        if (GetControlWZ_byPosition(i))
        {
            GetControlWZ_byPosition(i)->ShowWindowWZ(WZ_HIDE);
        }
    }
    if (GetControlWZ_byPosition(kControl_Picture_P004))
        GetControlWZ_byPosition(kControl_Picture_P004)->ShowWindowWZ(WZ_HIDE);
    for (int i = kControl_Button_B015; i <= kControl_Button_B018; ++i)
    {
        if (GetControlWZ_byPosition(i))
        {
            GetControlWZ_byPosition(i)->ShowWindowWZ(WZ_HIDE);
        }
    }

    if (GetControlWZ_byPosition(kControl_Button_B014) &&
        GetControlWZ_byPosition(kControl_Button_BT12) &&
        GetControlWZ_byPosition(kControl_Button_BT13) &&
        GetControlWZ_byPosition(kControl_Picture_P009) &&
        GetControlWZ_byPosition(kControl_Text_S003))
    {
        GetControlWZ_byPosition(kControl_Button_B014)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Button_BT12)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Button_BT13)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Picture_P009)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Text_S003)->ShowWindowWZ(WZ_HIDE);
    }


    //// 클래스 버튼
    skin_class_tab().SetVisible(false);
#ifdef _NA_20111026_CASHSHOP_CHECKBUTTON_TOOLTIP
    if (GetControlWZ_byPosition(kControl_Dummy_001) &&
        GetControlWZ_byPosition(kControl_Dummy_002) &&
        GetControlWZ_byPosition(kControl_Dummy_003) &&
        GetControlWZ_byPosition(kControl_Dummy_004) &&
        GetControlWZ_byPosition(kControl_Dummy_005))
    {
        GetControlWZ_byPosition(kControl_Dummy_001)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Dummy_002)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Dummy_003)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Dummy_004)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Dummy_005)->ShowWindowWZ(WZ_HIDE);
    }
#endif

    //------------------------------------------------------------------------------ 
    if (preiview_type == kPreview_Player)
    {
        if (GetControlWZ_byPosition(kControl_Button_B014))
            GetControlWZ_byPosition(kControl_Button_B014)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Button_BT12))
            GetControlWZ_byPosition(kControl_Button_BT12)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Button_BT13))
            GetControlWZ_byPosition(kControl_Button_BT13)->ShowWindowWZ(WZ_SHOW);

        if (GetControlWZ_byPosition(kControl_Picture_P001))
            GetControlWZ_byPosition(kControl_Picture_P001)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Picture_P008))
            GetControlWZ_byPosition(kControl_Picture_P008)->ShowWindowWZ(WZ_SHOW);
        for (int i = kControl_Button_B005; i <= kControl_Button_B013; ++i)
        {
            if (GetControlWZ_byPosition(i))
            {
                GetControlWZ_byPosition(i)->ShowWindowWZ(WZ_SHOW);
            }
        }
    }
    else if (preiview_type == kPreview_Rider)
    {
        if (GetControlWZ_byPosition(kControl_Button_B014))
            GetControlWZ_byPosition(kControl_Button_B014)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Button_BT12))
            GetControlWZ_byPosition(kControl_Button_BT12)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Button_BT13))
            GetControlWZ_byPosition(kControl_Button_BT13)->ShowWindowWZ(WZ_SHOW);

        if (GetControlWZ_byPosition(kControl_Picture_P004))
            GetControlWZ_byPosition(kControl_Picture_P004)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Picture_P008))
            GetControlWZ_byPosition(kControl_Picture_P008)->ShowWindowWZ(WZ_SHOW);
        for (int i = kControl_Button_B015; i <= kControl_Button_B018; ++i)
        {
            if (GetControlWZ_byPosition(i))
            {
                GetControlWZ_byPosition(i)->ShowWindowWZ(WZ_SHOW);
            }
        }
    }
    else //if (preiview_type == kPreview_Product)
    {
        Player* player = g_ObjectManager.GetPlayer(PreviewProduct.character_key);

        // 좌우버튼 표시
        if (PreviewProduct.pet != NULL || 
            PreviewProduct.rider != NULL || 
            player != NULL ||
            GetControlWZ_byPosition(kControl_Button_BT12) != NULL ||
            GetControlWZ_byPosition(kControl_Button_BT13) != NULL
            )
        {
            GetControlWZ_byPosition(kControl_Button_BT12)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Button_BT13)->ShowWindowWZ(WZ_SHOW);
        }

        if (player != NULL || PreviewProduct.parts != NULL && eCHAR_TYPE_MAX != char_class )
        {
            // 클래스 버튼
            DWORD backup_user_data = skin_class_tab().CurrentUserData();

            if( player && player->GetCostumeItemCode() != 0 )
            {
                if( BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(player->GetCostumeItemCode()) )
                {

                    eCHAR_TYPE Type1 = ResourceCode::GetImageClassType( player->GetClass(), GenderType::kMale );
                    eCHAR_TYPE Type2 = ResourceCode::GetImageClassType( player->GetClass(), GenderType::kFemale );

                    for (int i = eCHAR_BERSERKER; i < eCHAR_TYPE_MAX; ++i)
                    {
                        skin_class_tab().SetEnable(i,false);
                    }
                    eCHAR_TYPE sel = eCHAR_NONE;
                    
                    if( pItemInfo->m_wWICode != 0 )
                    {
                        skin_class_tab().SetEnable(Type1, true );
                        sel = Type1;
                    }

                    if( pItemInfo->m_wWICode2 != 0 )
                    {
                        skin_class_tab().SetEnable(Type2, true );
                        sel = Type2;
                    }

                    skin_class_tab().SetVisible(true);
                    if (sel != eCHAR_NONE)
                    {
                        skin_class_tab().Click(sel, true);
                        ChangeProductBySkinClass();
                    }
                    else
                    {
                        skin_class_tab().Click(backup_user_data, true);
                    }
                }
            }
            else
            {
                bool is_skin_class[eCHAR_TYPE_MAX] = {0, };
                int skin_class_count = CharacterCreationInfoParser::Instance()->GetSkinClass(char_class, OUT is_skin_class);
                for (int i = eCHAR_BERSERKER; i < eCHAR_TYPE_MAX; ++i)
                {
                    skin_class_tab().SetEnable(i, is_skin_class[i]);
                }
            }

#ifdef _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG
            if( player && player->GetCostumeItemCode() != 0 )
            {
                skin_class_tab().SetVisible(false);
            }
            else
            {
                skin_class_tab().SetVisible(true);
            }
#else
            skin_class_tab().SetVisible(true);
#endif // _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG
#ifdef _NA_20111026_CASHSHOP_CHECKBUTTON_TOOLTIP
            if (GetControlWZ_byPosition(kControl_Dummy_001) &&
                GetControlWZ_byPosition(kControl_Dummy_002) &&
                GetControlWZ_byPosition(kControl_Dummy_003) &&
                GetControlWZ_byPosition(kControl_Dummy_004) &&
                GetControlWZ_byPosition(kControl_Dummy_005))
            {
                GetControlWZ_byPosition(kControl_Dummy_001)->ShowWindowWZ(WZ_SHOW);
                GetControlWZ_byPosition(kControl_Dummy_002)->ShowWindowWZ(WZ_SHOW);
                GetControlWZ_byPosition(kControl_Dummy_003)->ShowWindowWZ(WZ_SHOW);
                GetControlWZ_byPosition(kControl_Dummy_004)->ShowWindowWZ(WZ_SHOW);
                GetControlWZ_byPosition(kControl_Dummy_005)->ShowWindowWZ(WZ_SHOW);
            }
#endif

            skin_class_tab().Click(backup_user_data, false);
        }


        // 아무것도 볼 수 없을때 미리보기 불가
        if (PreviewProduct.parts == NULL &&
            PreviewProduct.rider == NULL &&
            PreviewProduct.pet == NULL &&
            player == NULL &&
            GetControlWZ_byPosition(kControl_Picture_P009) &&
            GetControlWZ_byPosition(kControl_Text_S003))
        {
            GetControlWZ_byPosition(kControl_Picture_P009)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_S003)->ShowWindowWZ(WZ_SHOW);
        }
    }

    //------------------------------------------------------------------------------ 
    screen_info().reset();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::TurnLeft(float radin)
{
    screen_info().rot += radin;
    screen_info().check_valid_value();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::TurnRight(float radin)
{
    screen_info().rot -= radin;
    screen_info().check_valid_value();
}
//------------------------------------------------------------------------------
bool uiConvenienceStore_Basket::OnLButtonClick_Basket(SI_MESSAGE* message)
{
    //장바구니
    int index = GetControlPosition_byID(message->dwCtrlID) - kControl_Button_B003;
    if (index < static_cast<int>(basket_list().size()))
    {
        if (GetManager()->popup_menu_five()->IsVisible() == FALSE)
        {
            GetManager()->popup_menu_five()->ShowInterface(TRUE);
        }

        std::vector<ConvenienceStore::ProductItem*>::const_reference basket = basket_list().at(index);

        bool is_possible_preview = false;
        bool is_possible_equip = false;
        bool is_equiped = false;

        if (basket->is_package() == true)
        {
            is_possible_preview = uiConvenienceStore_Basket::HasPreviewItem(basket);
        }
        else if (basket->is_lotto() == false)
        {
            // 단품 아이템
            ConvenienceStore::ProductInfo* product_info = basket->first_info();
            if (product_info != NULL && 
                product_info->is_etheria() == false && 
                product_info->is_etheria_with_divine() == false)
            {
                is_possible_preview = uiConvenienceStore_Basket::IsPossiblePreview(product_info->GetItemInfo()->m_Code);
            }
        }
        if (basket->is_lotto() == false)
        {
            if (IsEquipedRiderParts(basket) || IsEquipedPlayerParts(basket))
            {
                is_equiped = true;
            }
            else
            {
                if (basket->has_preiview_player_equip_parts())
                {
                    is_possible_equip = uiConvenienceStore_Basket::IsPossibleEquipPreview(player(), basket);
                }
                else if (basket->has_preiview_rider_equip_parts())
                {
                    is_possible_equip = uiConvenienceStore_Basket::IsPossibleEquipPreview(PreviewRider.item_info, basket);
                }
            }
        }

        CControlWZ* control = GetControlWZ(message->dwCtrlID);
        GetManager()->popup_menu_five()->Reset();
        GetManager()->popup_menu_five()->set_owner(this, control);
        GetManager()->popup_menu_five()->SetMenu(0, //uiConvenienceStore_PopupMenu::kBasketPopup_Buy, 
                                            ConvenienceStore::Define::kShopStringCode_Basket_Popup_Buy, 
                                            basket->is_possible_sales(), 
                                            uiConvenienceStore_Dialog::kPopupCommand_Basket_Buy,
                                            reinterpret_cast<DWORD>(basket));
        GetManager()->popup_menu_five()->SetMenu(1, //uiConvenienceStore_PopupMenu::kBasketPopup_Gift, 
                                            ConvenienceStore::Define::kShopStringCode_Basket_Popup_Gift, 
                                            basket->is_possible_gift(), 
                                            uiConvenienceStore_Dialog::kPopupCommand_Basket_Gift,
                                            reinterpret_cast<DWORD>(basket));

        GetManager()->popup_menu_five()->SetMenu(2, //uiConvenienceStore_PopupMenu::kBasketPopup_Preview, 
                                            ConvenienceStore::Define::kShopStringCode_Basket_Popup_Preview, 
                                            is_possible_preview, 
                                            uiConvenienceStore_Dialog::kPopupCommand_Basket_Preview,
                                            reinterpret_cast<DWORD>(basket));

        if (is_equiped)
        {
            GetManager()->popup_menu_five()->SetMenu(3, //uiConvenienceStore_PopupMenu::kBasketPopup_Equip, 
                                            ConvenienceStore::Define::kShopStringCode_Basket_Popup_Unequip, 
                                            is_equiped, 
                                            uiConvenienceStore_Dialog::kPopupCommand_Equip_Unequip,
                                            reinterpret_cast<DWORD>(basket));
        }
        else
        {
            if (is_possible_equip)
            {
                GetManager()->popup_menu_five()->SetMenu(3, //uiConvenienceStore_PopupMenu::kBasketPopup_Equip, 
                                                ConvenienceStore::Define::kShopStringCode_Basket_Popup_Equip, 
                                                is_possible_equip, 
                                                uiConvenienceStore_Dialog::kPopupCommand_Basket_Equip,
                                                reinterpret_cast<DWORD>(basket));
            }
            else
            {
                // 장착이 안되는것들은 기본이 장착(해제) 로 되어 있어야 함
                GetManager()->popup_menu_five()->SetMenu(3, //uiConvenienceStore_PopupMenu::kBasketPopup_Equip, 
                                                ConvenienceStore::Define::kShopStringCode_Basket_Popup_Unequip, 
                                                is_possible_equip, 
                                                uiConvenienceStore_Dialog::kPopupCommand_Equip_Unequip,
                                                reinterpret_cast<DWORD>(basket));
            }
        }

        GetManager()->popup_menu_five()->SetMenu(4, //uiConvenienceStore_PopupMenu::kBasketPopup_Delete, 
                                            ConvenienceStore::Define::kShopStringCode_Basket_Popup_Delete, 
                                            true, 
                                            uiConvenienceStore_Dialog::kPopupCommand_Basket_Delete,
                                            reinterpret_cast<DWORD>(basket));

        if (control != NULL)
        { 
            RECT control_area = control->GetSizeRect();
            int cx = static_cast<int>(control_area.left + (control_area.right - control_area.left) / 2);
            int cy = static_cast<int>(control_area.top);
            GetManager()->popup_menu_five()->MoveWindowWZ(cx, cy);
        }
    }
    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::OnLButtonClick_Equip(SI_MESSAGE* message)
{
    // 장착 아이템
    CControlWZ* control = GetControlWZ(message->dwCtrlID);
    if (control != NULL && control->GetUserData() != NULL)
    {
        ConvenienceStore::ProductItem* basket = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData());

        if (GetManager()->popup_menu_three()->IsVisible() == FALSE)
        {
            GetManager()->popup_menu_three()->ShowInterface(TRUE);
        }

        GetManager()->popup_menu_three()->Reset();
        GetManager()->popup_menu_three()->set_owner(this, control);
        GetManager()->popup_menu_three()->SetMenu(0, //uiConvenienceStore_PopupMenu::kEquipPopup_Buy, 
            ConvenienceStore::Define::kShopStringCode_Basket_Popup_Buy,
            true, 
            uiConvenienceStore_Dialog::kPopupCommand_Equip_Buy,
            reinterpret_cast<DWORD>(basket));
        GetManager()->popup_menu_three()->SetMenu(1, //uiConvenienceStore_PopupMenu::kEquipPopup_Gift, 
            ConvenienceStore::Define::kShopStringCode_Basket_Popup_Gift, 
            true, 
            uiConvenienceStore_Dialog::kPopupCommand_Equip_Gift,
            reinterpret_cast<DWORD>(basket));
        GetManager()->popup_menu_three()->SetMenu(2, //uiConvenienceStore_PopupMenu::kEquipPopup_Unequip, 
            ConvenienceStore::Define::kShopStringCode_Basket_Popup_Unequip, 
            true, 
            uiConvenienceStore_Dialog::kPopupCommand_Equip_Unequip,
            reinterpret_cast<DWORD>(basket));

        if (control != NULL)
        {
            RECT control_area = control->GetSizeRect();
            int cx = static_cast<int>(control_area.left + (control_area.right - control_area.left) / 2);
            int cy = static_cast<int>(control_area.top);
            GetManager()->popup_menu_three()->MoveWindowWZ(cx, cy);
        }
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::OnClickPopupMenu(const uiConvenienceStore_Dialog::PopupCommandType command_type, const DWORD user_data)
{
    ConvenienceStore::ProductItem* basket = reinterpret_cast<ConvenienceStore::ProductItem*>(user_data); 
    if (basket == NULL)
    {
        return false;
    }
    int basket_index = FindBasketIndex(basket);
    if (basket_index >= static_cast<int>(basket_list().size()))
    {
        return false;
    }

    if ((command_type == uiConvenienceStore_Dialog::kPopupCommand_Basket_Buy) ||
        (command_type == uiConvenienceStore_Dialog::kPopupCommand_Equip_Buy))
    {

#ifdef _GLOBAL
        if( basket->is_cash_only() )
        {
            GetManager()->purchase()->SetCashType( ConvenienceStore::Define::kCashType_Cash );
            GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Buy);
            GetManager()->purchase()->InsertPurchase(basket, basket_index);
        }
        else if( basket->is_pcash_only() )
        {
            GetManager()->purchase()->SetCashType( ConvenienceStore::Define::kCashType_PPCard );
            GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Buy);
            GetManager()->purchase()->InsertPurchase(basket, basket_index);
        }
        else
        {
            GetManager()->cashtype()->SetBuyProductInfo(basket, basket_index);
            GetManager()->cashtype()->OpenSelectCashType();
        }
#elif defined(_CHINA)
        if( basket->is_mileage_only() )
        {
            GetManager()->purchase()->SetCashType( ConvenienceStore::Define::kCashType_Mileage );
            GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Buy);
            GetManager()->purchase()->InsertPurchase(basket, basket_index);
        }
        else
        {
            GetManager()->purchase()->SetCashType( ConvenienceStore::Define::kCashType_Cash );
            GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Buy);
            GetManager()->purchase()->InsertPurchase(basket, basket_index);
        }
#else
        GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Buy);
        GetManager()->purchase()->InsertPurchase(basket, basket_index);
#endif
    }
    else if (command_type == uiConvenienceStore_Dialog::kPopupCommand_Basket_Gift)
    {
        GetManager()->gift()->ShowInterface(true, basket);
    }
    else if (command_type == uiConvenienceStore_Dialog::kPopupCommand_Basket_Equip)
    {
        if (basket->has_preiview_player_equip_parts())
        {
            if (basket->is_package())
            {
                //단품 아이템이 장착되어 있으면 다 내린다.
                UnequipPlayerParts();
            }
            else
            {
                // 단품 아이템이면 패키지 아이템에 장착되어 있는것을 내린다.
                CControlWZ* control = GetControlWZ_byPosition(kControl_Button_B013);
                if (control != NULL)
                {
                    ConvenienceStore::ProductItem* basket = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData());
                    UnequipPlayerParts(basket);
                }
            }

            EquipPlayerParts(player(), basket, uiConvenienceStore_Basket::kPreview_Player);
            preview_tab().Click(uiConvenienceStore_Basket::kPreview_Player);
            UpdateEquipControl();
        }
        else if (basket->has_preiview_rider_equip_parts())
        {
            if (basket->is_package())
            {
                //단품 아이템이 장착되어 있으면 다 내린다.
                UnequipRiderParts();
            }
            else
            {
                // 단품 아이템이면 패키지 아이템에 장착되어 있는것을 내린다.
                CControlWZ* control = GetControlWZ_byPosition(kControl_Button_B018);
                if (control != NULL)
                {
                    ConvenienceStore::ProductItem* basket = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData());
                    UnequipRiderParts(basket);
                }
            }

            EquipRiderParts(PreviewRider.rider, PreviewRider.item_info, basket, uiConvenienceStore_Basket::kPreview_Rider);
            preview_tab().Click(uiConvenienceStore_Basket::kPreview_Rider); 
            UpdateEquipControl();
        }
    }
    else if (command_type == uiConvenienceStore_Dialog::kPopupCommand_Basket_Preview)
    {
        CreateProduct(basket);
    }
    else if (command_type == uiConvenienceStore_Dialog::kPopupCommand_Basket_Delete)
    {
        UnequipRiderParts(basket); 
        UnequipPlayerParts(basket); 
        RemoveBasket(basket_index);
    }
    //else if (command_type == uiConvenienceStore_Dialog::kPopupCommand_Equip_Buy)
    //{
    //    GetManager()->purchase()->ShowInterface(true, 
    //        uiConvenienceStore_Purchase::kPurchase_Buy,
    //        (TCHAR*)(g_pHero->GetName()),
    //        _T(""),
    //        basket_type());

    //   GetManager()->purchase()->InsertPurchase(basket, basket_index);

    //}
    else if (command_type == uiConvenienceStore_Dialog::kPopupCommand_Equip_Gift)
    {
        GetManager()->gift()->ShowInterface(true, basket);
    }
    else if (command_type == uiConvenienceStore_Dialog::kPopupCommand_Equip_Unequip)
    {
        if (basket->has_preiview_rider_equip_parts())
        {
            UnequipRiderParts(basket);
            preview_tab().Click(uiConvenienceStore_Basket::kPreview_Rider); 
            UpdateEquipControl();
        }
        else
        {
            UnequipPlayerParts(basket); 
            preview_tab().Click(uiConvenienceStore_Basket::kPreview_Player); 
            UpdateEquipControl();
        }
        GetManager()->popup_menu_three()->ShowInterface(FALSE);
    }

    GetManager()->popup_menu_three()->ShowInterface(FALSE);
    GetManager()->popup_menu_four()->ShowInterface(FALSE);
    GetManager()->popup_menu_five()->ShowInterface(FALSE);
    return true;
}
//------------------------------------------------------------------------------ 
int uiConvenienceStore_Basket::FindBasketIndex(const ConvenienceStore::ProductItem* basket)
{
    //ConvenienceStore::BasketVector::iterator iter = find_if(basket_list().begin(), 
    //                                basket_list.end(), 
    //                                std::bind1st(std::mem_fun_ref(&ConvenienceStore::Basket::is_equl), product));
    int index = 0;
    std::vector<ConvenienceStore::ProductItem*>::iterator begin = basket_list().begin();
    std::vector<ConvenienceStore::ProductItem*>::iterator end = basket_list().end();
    for (; begin != end; ++ begin, ++index)
    {
        if (*begin == basket)
        {
            return index;
        }
    }
    return -1;
}
//------------------------------------------------------------------------------ 
int uiConvenienceStore_Basket::FindBasketIndex(const CShopCategory* shop_category, 
                                               const CShopPackage* shop_package)
{
    int index = 0;
    std::vector<ConvenienceStore::ProductItem*>::iterator begin = basket_list().begin();
    std::vector<ConvenienceStore::ProductItem*>::iterator end = basket_list().end();
    for (; begin != end; ++ begin, ++index)
    {
        if (((*begin)->GetShopCategory() == shop_category) && ((*begin)->GetShopPackage() == shop_package))
        {
            return index;
        }
    }
    return -1;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::IsEquipedPlayerParts(const ConvenienceStore::ProductItem* basket)
{
    for (int i = kControl_Button_B005; i <= kControl_Button_B013; ++i)
    {
        if (GetControlWZ_byPosition(i) &&
            basket == reinterpret_cast<ConvenienceStore::ProductItem*>(GetControlWZ_byPosition(i)->GetUserData()))
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::IsEquipedRiderParts(const ConvenienceStore::ProductItem* basket)
{
    for (int i = kControl_Button_B015; i <= kControl_Button_B018; ++i)
    {
        if (GetControlWZ_byPosition(i) &&
            basket == reinterpret_cast<ConvenienceStore::ProductItem*>(GetControlWZ_byPosition(i)->GetUserData()))
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------ 
CControlWZ* uiConvenienceStore_Basket::GetRiderEquipUIControl(const BASE_ITEMINFO* item_info)
{
    if (item_info->IsRiderPartsItem())
    {
        int gab = item_info->m_wEqPos - eRIDER_PARTS_POS::HeadPart;
        return GetControlWZ_byPosition(kControl_Button_B015 + gab);
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
CControlWZ* uiConvenienceStore_Basket::GetPlayerEquipUIControl(const BASE_ITEMINFO* item_info)
{
    //헬멧/갑옷/장갑/바지/부츠/무기/헤어/얼굴/패키지
    //    kControl_Weapon = kControl_Button_B005,
    //    kControl_Armor  = kControl_Button_B006,
    //    kControl_Helmet = kControl_Button_B007,
    //    kControl_Pants  = kControl_Button_B008,
    //    kControl_Boots  = kControl_Button_B009,
    //    kControl_Glove  = kControl_Button_B010,
    //    kControl_Hair   = kControl_Button_B011,
    //    kControl_Face   = kControl_Button_B012,
    //      패키지 = kControl_Button_B013

    if (item_info == NULL)
    {
        return NULL;
    }
    if (item_info->m_wEqPos == eEQUIPCONTAINER_HELMET)  return GetControlWZ_byPosition(kControl_Button_B005);
    if (item_info->m_wEqPos == eEQUIPCONTAINER_ARMOR)   return GetControlWZ_byPosition(kControl_Button_B006);
    if (item_info->m_wEqPos == eEQUIPCONTAINER_GLOVE)   return GetControlWZ_byPosition(kControl_Button_B007);
    if (item_info->m_wEqPos == eEQUIPCONTAINER_PANTS)   return GetControlWZ_byPosition(kControl_Button_B008);
    if (item_info->m_wEqPos == eEQUIPCONTAINER_BOOTS)   return GetControlWZ_byPosition(kControl_Button_B009);
    if (item_info->m_wEqPos == eEQUIPCONTAINER_WEAPON)  return GetControlWZ_byPosition(kControl_Button_B010);
    if (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_HAIR) return GetControlWZ_byPosition(kControl_Button_B011);
    if (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_FACE) return GetControlWZ_byPosition(kControl_Button_B012);
    //if (item_info->m_wEqPos == eITEMTYPE_CHANGE_CHARACTER_HAIR) return GetControlWZ_byPosition(kControl_Button_B011);
    //if (item_info->m_wEqPos == eITEMTYPE_CHANGE_CHARACTER_FACE) return GetControlWZ_byPosition(kControl_Button_B012);

    return NULL;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::EquipRiderParts(Rider* rider, 
                                                const BASE_ITEMINFO* rider_item_info, 
                                                const BASE_ITEMINFO* parts_item_info,
                                                PreviewType preiview_type)
{
    if (rider == NULL)
    {
        return false;
    }
    if (uiConvenienceStore_Basket::IsPossibleEquipPreview(rider_item_info, parts_item_info) == false)
    {
        return false;
    }
               
    int pos = parts_item_info->m_wEqPos - eRIDER_PARTS_POS::StartIndex;
    rider->DetachRiderPart(pos);
    rider->AttachRiderPart(pos, parts_item_info->m_Code);
    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::EquipRiderParts(Rider* rider, 
                                                const BASE_ITEMINFO* rider_item_info, 
                                                ConvenienceStore::ProductItem* parts_basket,
                                                PreviewType preiview_type)
{
    if (rider == NULL)
    {
        return false;
    }
    if (uiConvenienceStore_Basket::IsPossibleEquipPreview(rider_item_info, parts_basket) == false)
    {
        return false;
    }

    CControlWZ* control = NULL;
    if (parts_basket->is_package())
    {
        //! 패키지 일경우에는 모두 언이킵한다.
        UnequipRiderParts();

        std::vector<ConvenienceStore::ProductInfo>::iterator begin = parts_basket->GetInfoVector().begin();
        std::vector<ConvenienceStore::ProductInfo>::iterator end = parts_basket->GetInfoVector().end();
        for (; begin != end; ++ begin)
        {
            if (EquipRiderParts(rider, rider_item_info, begin->GetItemInfo(), preiview_type) == true)
            {
            }
        }
        rider->RefreshApperRider(NULL);

        if (preiview_type == uiConvenienceStore_Basket::kPreview_Rider)
        {
            control = GetControlWZ_byPosition(kControl_Button_B018);
        }
    }
    else
    {
        // 패키지가 장착되어 있으면 언이킵한다.
        DWORD user_data = 0;
        if (GetControlWZ_byPosition(kControl_Button_B018))
        {
            user_data = GetControlWZ_byPosition(kControl_Button_B018)->GetUserData();
        }
        
        if (user_data != 0)
        {
            UnequipRiderParts(reinterpret_cast<ConvenienceStore::ProductItem*>(user_data));
        }

        if (EquipRiderParts(rider, rider_item_info, parts_basket->first_info()->GetItemInfo(), preiview_type) == true)
        {
            rider->RefreshApperRider(NULL);
        }

        if (preiview_type == uiConvenienceStore_Basket::kPreview_Rider)
        {
            control = GetRiderEquipUIControl(parts_basket->first_info()->GetItemInfo());
        }
    }
    if (control != NULL)
    {
        control->SetUserData(reinterpret_cast<DWORD>(parts_basket));
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::UnequipRiderParts(const BASE_ITEMINFO* item_info)
{
    if (PreviewRider.rider == NULL || item_info == NULL)
    {
        return false;
    }
    if (item_info->IsRiderPartsItem())
    {
        int pos = item_info->m_wEqPos - eRIDER_PARTS_POS::StartIndex;
        PreviewRider.rider->DetachRiderPart(pos);
    }

    return true;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::UnequipRiderParts(ConvenienceStore::ProductItem* basket)
{
    if (basket == NULL || PreviewRider.rider == NULL)
    {
        return;
    }
    if (basket->is_lotto())
    {
        return;
    }
    if (IsEquipedRiderParts(basket) == false)
    {
        return;
    }
    CControlWZ* control = NULL;
    if (basket->is_package())
    {
        for (int i = RP_BODY; i <= RP_HELM; ++i)
        {
            PreviewRider.rider->DetachPart(static_cast<RiderParts>(i)); 
        }
        PreviewRider.rider->RefreshApperRider(NULL);

        control = GetControlWZ_byPosition(kControl_Button_B018);
    }
    else
    {
        if (UnequipRiderParts(basket->first_info()->GetItemInfo()) == true)
        {
            PreviewRider.rider->RefreshApperRider(NULL);
        }
        control = GetRiderEquipUIControl(basket->first_info()->GetItemInfo());
    }
    if (control != NULL)
    {
        control->SetUserData(NULL);
    }

}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::UnequipRiderParts()
{
    for (int i = kControl_Button_B015; i <= kControl_Button_B018; ++i)
    {
        CControlWZ* control = GetControlWZ_byPosition(i);
        if (control != NULL)
        {
            ConvenienceStore::ProductItem* basket = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData());
            UnequipRiderParts(basket);
        }
    }
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::EquipPlayerParts(Player* player, 
                                                 const BASE_ITEMINFO* item_info, 
                                                 PreviewType preiview_type)
{
    if (uiConvenienceStore_Basket::IsPossibleEquipPreview(player, item_info) == false)
    {
        return false;
    }

    if (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_FACE)
    {
        player->SetPartVariation(PLAYER_VARIATION_FACE, item_info->m_byWasteType);
    }
    else if (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_HAIR)
    {
        player->SetPartVariation(PLAYER_VARIATION_HAIR, item_info->m_byWasteType);
    }
#ifdef _NA_004035_20120227_COSTUME_ITEM
    else if (item_info->m_wType == eITEMTYPE_COSTUME)
    {
        player->SetCostumeItemCode(item_info->m_Code);
    }
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	else if (item_info->m_wType == eITEMTYPE_COSTUME_DECORATE)
	{
		player->SetDecorateCostumeItemCode(item_info->m_Code);
	}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
    else
    {
        SCSlot& slot = player->GetEquipmentContainer()->GetSlot(item_info->m_wEqPos);
        if (slot.GetCode() == item_info->m_Code)
        {
            return false;
        }

        player->GetEquipmentContainer()->DeleteSlot(item_info->m_wEqPos, NULL);

        SCItemSlot item_slot;
        item_slot.SetCode(item_info->m_Code);
        player->GetEquipmentContainer()->InsertSlot(item_info->m_wEqPos, item_slot);
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::EquipPlayerParts(Player* player, 
                                                 ConvenienceStore::ProductItem* basket,
                                                 PreviewType preiview_type)
{
    if (basket == NULL || player == NULL)
    {
        return false;
    }
    if (basket->is_lotto())
    {
        return false;
    }
    CControlWZ* control = NULL;
    if (basket->is_package())
    {
        std::vector<ConvenienceStore::ProductInfo>::iterator begin = basket->GetInfoVector().begin();
        std::vector<ConvenienceStore::ProductInfo>::iterator end = basket->GetInfoVector().end();
        for (; begin != end; ++ begin)
        {
            if (EquipPlayerParts(player, begin->GetItemInfo(), preiview_type) == true)
            {
            }
        }
        player->RefreshAppearRender();

        if (preiview_type == uiConvenienceStore_Basket::kPreview_Player)
        {
            control = GetControlWZ_byPosition(kControl_Button_B013);
        }
    }
    else
    {
        if (EquipPlayerParts(player, basket->first_info()->GetItemInfo(), preiview_type) == true)
        {
            player->RefreshAppearRender();
        }
        if (preiview_type == uiConvenienceStore_Basket::kPreview_Player)
        {
            control = GetPlayerEquipUIControl(basket->first_info()->GetItemInfo());
        }
    }
    if (control != NULL)
    {
        control->SetUserData(reinterpret_cast<DWORD>(basket));
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::UnequipPlayerParts(const BASE_ITEMINFO* item_info)
{
    if (item_info == NULL)
    {
        return false;
    }

    if (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_FACE)
    {
        int face = g_pHero ? g_pHero->GetPartVariation(PLAYER_VARIATION_FACE) : 1;
        player()->SetPartVariation(PLAYER_VARIATION_FACE, face);
    }
    else if (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_HAIR)
    {
        int hair = g_pHero ? g_pHero->GetPartVariation(PLAYER_VARIATION_HAIR) : 1;
        player()->SetPartVariation(PLAYER_VARIATION_HAIR, hair);
    }
#ifdef _NA_004035_20120227_COSTUME_ITEM
    else if (item_info->m_wType == eITEMTYPE_COSTUME)
    {
        player()->SetCostumeItemCode(0);
    }
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	else if (item_info->m_wType == eITEMTYPE_COSTUME_DECORATE)
	{
		player()->SetDecorateCostumeItemCode(0);
	}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
    else
    {
        player()->GetEquipmentContainer()->DeleteSlot(item_info->m_wEqPos, NULL);
    }

    return true;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::UnequipPlayerParts(ConvenienceStore::ProductItem* basket)
{
    if (basket == NULL || (player() == NULL))
    {
        return;
    }
    if (basket->is_lotto())
    {
        return;
    }
    if (IsEquipedPlayerParts(basket) == false)
    {
        return;
    }
    if (basket->is_package())
    {
        int face = g_pHero ? g_pHero->GetPartVariation(PLAYER_VARIATION_FACE) : 1;
        int hair = g_pHero ? g_pHero->GetPartVariation(PLAYER_VARIATION_HAIR) : 1;
        player()->SetPartVariation(PLAYER_VARIATION_FACE, face);
        player()->SetPartVariation(PLAYER_VARIATION_HAIR, hair);
        for (int i = 0; i < eEQUIPCONTAINER_MAX; ++i)
        {
            player()->GetEquipmentContainer()->DeleteSlot(i, NULL);
        }
        player()->RefreshAppearRender();
            
        CControlWZ* control = GetControlWZ_byPosition(kControl_Button_B013);
        if (control != NULL)
        {
            control->SetUserData(NULL);
        }
    }
    else
    {
        if (UnequipPlayerParts(basket->first_info()->GetItemInfo()) == true)
        {
            player()->RefreshAppearRender();
        }
        CControlWZ* control = GetPlayerEquipUIControl(basket->first_info()->GetItemInfo());
        if (control != NULL)
        {
            control->SetUserData(NULL);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::UnequipPlayerParts()
{
    for (int i = kControl_Button_B005; i <= kControl_Button_B013; ++i)
    {
        CControlWZ* control = GetControlWZ_byPosition(i);
        if (control != NULL)
        {
            ConvenienceStore::ProductItem* basket = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData());
            UnequipPlayerParts(basket);
        }
    }
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::IsPossibleShopPopup()
{
    //BOOL is_res = FALSE;
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_Counter::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_Basket::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_BasketMypage::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_BasketPPCard::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_BasketMileage::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_PopupPackage::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_PopupMenuFour::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_PopupMenuFive::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_PopupMenuTwo::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(StrToWzID("0210"));                       //HeroGaugeDialog
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(StrToWzID("0230"));                       //QuickContainer
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(IM_SKILL_MANAGER::SKILL_PERK_SLOT_DLG);   //uiPerkSlot
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(IM_TARGET_MANAGER::TARGET_DIALOG);        //TargetDialog
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(IM_RADAR_MANAGER::RADAR_DIALOG);          //RadarDialog
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);      //RadarPlusDialog

    //return (is_res == TRUE);

#if WZENGINEVER >= 410
    if ((g_InterfaceManager.GetDialogManager()->GetLockDlgID() != 0) && 
        ((g_InterfaceManager.GetDialogManager()->GetLockDlgID() != uiConvenienceStore_Basket::kDialog_ID) )
       )
    {
        return false; 
    }

    STD_LIST::reverse_iterator rbegin = g_InterfaceManager.GetDialogManager()->GetDlgManager()->GetDrawList().rbegin();
    STD_LIST::reverse_iterator rend = g_InterfaceManager.GetDialogManager()->GetDlgManager()->GetDrawList().rend();
    for (; rbegin != rend; ++rbegin)
    {
        DWORD dlg_id = (*rbegin);
        if ((dlg_id == uiConvenienceStore_Basket::kDialog_ID))
        {
            break;
        }
        if ((dlg_id == uiConvenienceStore_Counter::kDialog_ID) || 
            (dlg_id == uiConvenienceStore_PopupPackage::kDialog_ID) || 
            (dlg_id == uiConvenienceStore_PopupMenuFour::kDialog_ID) || 
            (dlg_id == uiConvenienceStore_PopupMenuThree::kDialog_ID) || 
            (dlg_id == uiConvenienceStore_PopupMenuFive::kDialog_ID) || 
            (dlg_id == uiConvenienceStore_PopupMenuTwo::kDialog_ID) ||
            (dlg_id == StrToWzID("0210")) ||                        //HeroGaugeDialog
            (dlg_id == StrToWzID("0230")) ||                        //QuickContainer
#ifdef _NA_000000_20130114_RENEWER_UI
#else
            (dlg_id == IM_SKILL_MANAGER::SKILL_PERK_SLOT_DLG) ||    //uiPerkSlot
#endif
            (dlg_id == IM_TARGET_MANAGER::TARGET_DIALOG) ||         //TargetDialog
            (dlg_id == IM_RADAR_MANAGER::RADAR_DIALOG) ||           //RadarDialog
            (dlg_id == IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)      //RadarPlusDialog
           )
        {
            continue;
        }
        SolarDialog* dlg = g_InterfaceManager.GetDialog(dlg_id);  
        if (dlg == NULL || dlg->IsVisible() == FALSE)
        {
            continue;
        }
        return false;
    }
#endif //#if WZENGINEVER >= 410

    return true;
}
//------------------------------------------------------------------------------ 
CASH uiConvenienceStore_Basket::GetBasketPrice()
{
    std::vector<ConvenienceStore::ProductItem*>::iterator begin = basket_list().begin();
    std::vector<ConvenienceStore::ProductItem*>::iterator end = basket_list().end();

    CASH price = 0;;
    for (; begin != end; ++begin)
    {
        price += (*begin)->GetPrice();
    }

    return price;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::UpdateUI()
{
    TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
    GlobalFunc::SetPointText(msg, GetBasketPrice());

    //155485	(총액: %s%s)
    GlobalFunc::SetTextFromStringTable(GetControlWZ_byPosition(kControl_Text_S002), 
        155485, 
        msg, 
        (TCHAR*)g_InterfaceManager.GetInterfaceString(155304));


}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::ClearRider()
{
    if (PreviewRider.rider)
    {
        PreviewRider.rider->Destroy();
        SAFE_DELETE(PreviewRider.rider);
    }
    PreviewRider.item_info = NULL;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::ClearProduct()
{
    if (PreviewProduct.parts)
    {
        ResourceManager::Instance()->FreeResourceUnitDraw(PreviewProduct.parts);
        PreviewProduct.parts = NULL;
    }
    if (PreviewProduct.pet)
    {
        PreviewProduct.pet->Destroy();
        SAFE_DELETE(PreviewProduct.pet);
    }
    if (PreviewProduct.rider)
    {
        PreviewProduct.rider->Destroy();
        SAFE_DELETE(PreviewProduct.rider);
    }
    SAFE_DELETE(PreviewProduct.product);
    skin_class_tab().Click(eCHAR_NONE, true);
    g_ObjectManager.Delete(PreviewProduct.character_key);
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::CreatePlayer()
{
    //GetManager()->counter()->CurrentShopMenuType() == ConvenienceStore::Define::kShopMenuType_Mypage
    Player* player = GameFunc::MakePlayer(player_key(), g_pHero, false);
    if (player != NULL)
    {
        player->SetShow(FALSE);

        WzVector Light_Direction = g_pMap->GetWorldBase()->GetSunLightDir();
        WzVector position = player->GetVisiblePos();
        position = position;
    }
    return true; 
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::CreateRider()
{
    //------------------------------------------------------------------------------ 
    ClearRider();

    //------------------------------------------------------------------------------ 
    if ((g_pHero->IsRiding() == FALSE) || (g_pHero->rider() == NULL))
    {
        return false;
    }

    SCSlotContainer* inventory_container = ItemManager::Instance()->GetContainer(SI_INVENTORY);
    if (inventory_container == NULL)
    {
        return false;
    }
    SCSlot& slot = inventory_container->GetSlot(g_pHero->m_RiderInfo.m_RiderItemPos);
    if (slot.GetSlotType() != ST_ITEM)
    {
        return false;
    }
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(slot);
    
    PreviewRider.rider = new Rider;
    PreviewRider.rider->CopyRider(g_pHero->rider(), false /*(basket_type() == kBasket_Mypage)*/);
    PreviewRider.rider->SetAnimation(GlobalFunc::GetRiderAnimationByPlayerAni(eANI_STAND, false), TRUE, TRUE); 
    PreviewRider.item_info = item_slot.GetItemInfo();
    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::CreateProduct(SLOTCODE item_code, eCHAR_TYPE skin_class_type)
{
    if (uiConvenienceStore_Basket::IsPossiblePreview(item_code) == false)
    {
        return false;
    }

    //------------------------------------------------------------------------------ 
    const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code); 
    if (item_info != NULL)
    {
        if (item_info->m_wType == eITEMTYPE_PET)
        {
            return CreateProductPet(item_info);
        }
        else if (item_info->m_wType == eITEMTYPE_RIDER)
        {
            return CreateProductRider(item_info);
        }
#ifdef _NA_004035_20120227_COSTUME_ITEM
        else if (item_info->m_wType == eITEMTYPE_COSTUME)
        {
            return CreateProductCostume(item_info, skin_class_type);
        }
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
		else if (item_info->m_wType == eITEMTYPE_COSTUME_DECORATE)
		{
			return CreateProductDecorateCostume(item_info, skin_class_type);
		}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
        else //if (SCItemSlot::CanEquip(item_info) == true)
        {
            return CreateProductPart(item_info, skin_class_type);
        }
    }
    //if (is_res == true)
    //{
    //    preview_tab().Click(uiConvenienceStore_Basket::kPreview_Product);
    //    UpdateEquipControl(uiConvenienceStore_Basket::kPreview_Product);
    //}

    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::CreateProduct(ConvenienceStore::ProductItem* product)
{
    if ((product == NULL) || (product->is_lotto() == true))
    {
        return false;
    }

    //------------------------------------------------------------------------------ 
    ClearProduct();

    //------------------------------------------------------------------------------ 
    bool is_res = false;
    if (product->is_package() == false)
    {
        eCHAR_TYPE char_type = eCHAR_NONE;
        product->has_preiview_player_equip_parts(OUT &char_type);

        if (char_type == g_pHero->GetClass())
        {
            char_type = ResourceCode::GetImageClassType(char_type, g_pHero->GetGender());
        }
         is_res = CreateProduct(product->item_code(0), char_type);
    }
    else
    {
        //!패키지 일경우 에만 이쪽 루터
        if (uiConvenienceStore_Basket::HasPreviewItem(product) == false)
        {
            return false;
        }

        //------------------------------------------------------------------------------ 
        eCHAR_TYPE char_type = eCHAR_NONE;
        if (product->has_preiview_player_equip_parts(OUT &char_type))
        {
            GenderType::Value gender_type = GenderType::kAll;
            if (char_type == g_pHero->GetClass())
            {
                gender_type = g_pHero->GetGender();
            }
            else
            {
                gender_type = ResourceCode::GetBaseGenderOfClass(char_type);
            }
            is_res = CreateProductPlayer(product, char_type, gender_type);
            if (is_res)
            {
                // 클래스 버튼
                bool is_skin_class[eCHAR_TYPE_MAX] = {0, };
                int skin_class_count = CharacterCreationInfoParser::Instance()->GetSkinClass(char_type, OUT is_skin_class);
                for (int i = eCHAR_BERSERKER; i < eCHAR_TYPE_MAX; ++i)
                {
                    skin_class_tab().SetEnable(i, is_skin_class[i]);
                }
                skin_class_tab().Click(ResourceCode::GetImageClassType(char_type, gender_type), false);
            }

        }
        else if (product->has_preiview_rider_equip_parts())
        {
            BASE_ITEMINFO* rider_item_info = NULL;
            std::vector<ConvenienceStore::ProductInfo>::iterator begin = product->GetInfoVector().begin();
            std::vector<ConvenienceStore::ProductInfo>::iterator end = product->GetInfoVector().end();
            for (; begin != end; ++begin)
            {
                if (begin->GetItemInfo()->m_wType == eITEMTYPE_RIDER)
                {
                    rider_item_info = begin->GetItemInfo();
                    is_res = CreateProductRider(rider_item_info);
                    break;
                }
            }
            if (is_res)
            {
                // 라이더에 파쳐를 붙인다
                EquipRiderParts(PreviewProduct.rider, rider_item_info, product, kPreview_Product);
            }
        }
    }
        
    if (is_res == true)
    {
        SAFE_DELETE(PreviewProduct.product);
        PreviewProduct.product = ConvenienceStore::ProductItem::CreateProduct(product);

        preview_tab().Click(uiConvenienceStore_Basket::kPreview_Product);
        UpdateEquipControl();
    }

    return is_res;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::ChangeProductBySkinClass()
{
    DWORD user_data = 0;
    if (skin_class_tab().Current(user_data) != NULL)
    {
        eCHAR_TYPE skin_class_type = static_cast<eCHAR_TYPE>(user_data);

        if (PreviewProduct.product == NULL || PreviewProduct.product->is_lotto())
        {
            return;
        }
        if (PreviewProduct.product->is_package())
        {
            if (PreviewProduct.product->has_preiview_player_equip_parts())
            {
                Player* player = g_ObjectManager.GetPlayer(PreviewProduct.character_key);
                if (player != NULL)
                {
                    player->SetGender(ResourceCode::GetBaseGenderOfClass(skin_class_type));
                    player->RefreshAppearRender(TRUE);
                }

                // 클래스 버튼
                bool is_skin_class[eCHAR_TYPE_MAX] = {0, };
                int skin_class_count = CharacterCreationInfoParser::Instance()->GetSkinClass(player->GetClass(), OUT is_skin_class);
                for (int i = eCHAR_BERSERKER; i < eCHAR_TYPE_MAX; ++i)
                {
                    skin_class_tab().SetEnable(i, is_skin_class[i]);
                }
                skin_class_tab().Click(skin_class_type, false);
            }
        }
        else
        {
            BASE_ITEMINFO* pItemInfo = PreviewProduct.product->first_info()->GetItemInfo();
            if( pItemInfo && pItemInfo->m_wType == eITEMTYPE_COSTUME )
            {
                Player* player = g_ObjectManager.GetPlayer(PreviewProduct.character_key);
                if (player != NULL)
                {
                    player->SetGender(ResourceCode::GetBaseGenderOfClass(skin_class_type));
                    player->RefreshAppearRender(TRUE);
                }

                // 클래스 버튼

                eCHAR_TYPE Type1 = ResourceCode::GetImageClassType( skin_class_type, GenderType::kMale );
                eCHAR_TYPE Type2 = ResourceCode::GetImageClassType( skin_class_type, GenderType::kFemale );

                for (int i = eCHAR_BERSERKER; i < eCHAR_TYPE_MAX; ++i)
                {
                    skin_class_tab().SetEnable(i,false);
                }

                if( pItemInfo->m_wWICode != 0 )
                {
                    skin_class_tab().SetEnable(Type1, true );
                }
                if( pItemInfo->m_wWICode2 != 0 )
                {
                    skin_class_tab().SetEnable(Type2, true );
                }

                skin_class_tab().SetVisible(true);
                skin_class_tab().Click(skin_class_type, false);
            }
            else
            {
                CreateProductPart(pItemInfo, skin_class_type);
            }
        }
    }


}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::CreateProductPart(const BASE_ITEMINFO* item_info, eCHAR_TYPE skin_class_type)
{
    if (item_info == NULL)
    {
        return false;
    }

    eCHAR_TYPE char_class_type = eCHAR_NONE;
    if (item_info->m_wEqClass1) char_class_type = eCHAR_BERSERKER;
    else if (item_info->m_wEqClass2) char_class_type = eCHAR_DRAGON;
    else if (item_info->m_wEqClass3) char_class_type = eCHAR_SHADOW;
    else if (item_info->m_wEqClass4) char_class_type = eCHAR_VALKYRIE;
    else if (item_info->m_wEqClass5) char_class_type = eCHAR_MAGICIAN; 
    else if (item_info->m_wEqClass6) char_class_type = eCHAR_MYSTIC; //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    else if (item_info->m_wEqClass7) char_class_type = eCHAR_HELLROID; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	else if (item_info->m_wEqClass8) char_class_type = eCHAR_WITCHBLADE;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    RESOURCECODE skin_code = 0;
    GenderType::Value gender = ResourceCode::GetBaseGenderOfClass(skin_class_type);

    if (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_FACE)
    {
        skin_code = ResourceManager::Instance()->GetCharacterPartSkinResourceCode(char_class_type, BP_FACE, item_info->m_byWasteType, 0, gender);
    }
    else if (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_HAIR)
    {
        int variation_set = (gender == GenderType::kMale) ? item_info->m_bHeadType : item_info->m_bHeadType1;
        skin_code = ResourceManager::Instance()->GetCharacterPartSkinResourceCode(char_class_type, BP_HAIR, item_info->m_byWasteType, variation_set, gender);
    }
    else if (item_info->m_wType == eITEMTYPE_RIDER_PARTS)
    {
        skin_code = ResourceManager::Instance()->GetItemSkinResourceCode(item_info->m_Code);
    }
    else if (SCItemSlot::CanEquip(item_info) == true)
    {
        BodyPart bodypart_index = GameFunc::EquipToBodyPart(static_cast<eEQUIPCONTAINER_POS>(item_info->m_wEqPos)); 
        if (bodypart_index == BP_COUNT)
        {
            return false;
        }
        if (bodypart_index < BP_SPECIAL_ACCESSORY1)
        {
            skin_code = ResourceManager::Instance()->GetItemSkinResourceCode(item_info->m_Code, char_class_type, bodypart_index, gender);
        }
        else
        {
            gender = ResourceCode::CheckValidGender(char_class_type, gender);

            skin_code = ResourceManager::Instance()->GetItemSkinResourceCode(item_info->m_Code, gender);
        }
    }


    if (skin_code == 0 || skin_code == ENGINE_RESOURCE_ID_OFFSET)
    {
        return false;
    }

    ResourceManager::Instance()->FreeResourceUnitDraw(PreviewProduct.parts);
    PreviewProduct.parts = ResourceManager::Instance()->AllocResourceUnitDraw(skin_code, -1.0f, false);
    if (PreviewProduct.parts == NULL)
    {
        return false;
    }

    // 클래스 버튼
    bool is_skin_class[eCHAR_TYPE_MAX] = {0, };
    int skin_class_count = CharacterCreationInfoParser::Instance()->GetSkinClass(char_class_type, OUT is_skin_class);
    for (int i = eCHAR_BERSERKER; i < eCHAR_TYPE_MAX; ++i)
    {
        skin_class_tab().SetEnable(i, is_skin_class[i]);
    }

    skin_class_tab().Click(skin_class_type, false);

    return true;
}
//------------------------------------------------------------------------------ 
#ifdef _NA_004035_20120227_COSTUME_ITEM
bool uiConvenienceStore_Basket::CreateProductCostume(const BASE_ITEMINFO* item_info, eCHAR_TYPE skin_class_type)
{
    if (item_info == NULL)
    {
        return false;
    }

    eCHAR_TYPE char_class_type = eCHAR_NONE;
    if (item_info->m_wEqClass1) char_class_type = eCHAR_BERSERKER;
    else if (item_info->m_wEqClass2) char_class_type = eCHAR_DRAGON;
    else if (item_info->m_wEqClass3) char_class_type = eCHAR_SHADOW;
    else if (item_info->m_wEqClass4) char_class_type = eCHAR_VALKYRIE;
    else if (item_info->m_wEqClass5) char_class_type = eCHAR_MAGICIAN; 
    else if (item_info->m_wEqClass6) char_class_type = eCHAR_MYSTIC; //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    else if (item_info->m_wEqClass7) char_class_type = eCHAR_HELLROID; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	else if (item_info->m_wEqClass8) char_class_type = eCHAR_WITCHBLADE;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    RESOURCECODE skin_code = 0;
    GenderType::Value gender = GenderType::kAll;
#ifdef _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG
    if (GenderType::kAll == item_info->gender_type)
    {
        gender = ResourceCode::GetBaseGenderOfClass(char_class_type);
    }
    else
    {
        gender = static_cast<GenderType::Value>(item_info->gender_type);
    }
#else
    GenderType::Value gender = ResourceCode::GetBaseGenderOfClass(char_class_type);
#endif //_NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG

    Player* player = g_ObjectManager.GetPlayer(PreviewProduct.character_key);
    if (player == NULL)
    {
        ClassCodeEx class_code_ex;
        class_code_ex.class_type = char_class_type;
        class_code_ex.gender = gender;//GenderType::kMale;

        player = (Player*)g_ObjectManager.Add(PreviewProduct.character_key, PLAYER_OBJECT, class_code_ex.class_code, FALSE);
    }
    if (player != NULL)
    {
        player->SetHeightVariation(2);

        // 이킵 정보 셋팅한다.
        EquipPlayerParts(player, item_info, kPreview_Product);
        player->RefreshAppearRender();
        player->SetShow(FALSE);
    }


    eCHAR_TYPE Type1 = ResourceCode::GetImageClassType( char_class_type, GenderType::kMale );
    eCHAR_TYPE Type2 = ResourceCode::GetImageClassType( char_class_type, GenderType::kFemale );

    for (int i = eCHAR_BERSERKER; i < eCHAR_TYPE_MAX; ++i)
    {
        skin_class_tab().SetEnable(i,false);
    }

    if( item_info->m_wWICode != 0 )
    {
        skin_class_tab().SetEnable(Type1, true );
    }
    if( item_info->m_wWICode2 != 0 )
    {
        skin_class_tab().SetEnable(Type2, true );
    }

    
#ifdef _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG
    skin_class_tab().SetVisible(false);
#else
    skin_class_tab().SetVisible(true);
#endif // _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG
    skin_class_tab().Click(char_class_type, false);

    return (player!=NULL);
}
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
bool uiConvenienceStore_Basket::CreateProductDecorateCostume(const BASE_ITEMINFO* item_info, eCHAR_TYPE skin_class_type)
{
	if (item_info == NULL)
	{
		return false;
	}

	eCHAR_TYPE char_class_type = eCHAR_NONE;
	if (item_info->m_wEqClass1) char_class_type = eCHAR_BERSERKER;
	else if (item_info->m_wEqClass2) char_class_type = eCHAR_DRAGON;
	else if (item_info->m_wEqClass3) char_class_type = eCHAR_SHADOW;
	else if (item_info->m_wEqClass4) char_class_type = eCHAR_VALKYRIE;
	else if (item_info->m_wEqClass5) char_class_type = eCHAR_MAGICIAN; 
	else if (item_info->m_wEqClass6) char_class_type = eCHAR_MYSTIC; //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
	else if (item_info->m_wEqClass7) char_class_type = eCHAR_HELLROID; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	else if (item_info->m_wEqClass8) char_class_type = eCHAR_WITCHBLADE;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	RESOURCECODE skin_code = 0;
	GenderType::Value gender = GenderType::kAll;
#ifdef _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG
	if (GenderType::kAll == item_info->gender_type)
	{
		gender = ResourceCode::GetBaseGenderOfClass(char_class_type);
	}
	else
	{
		gender = static_cast<GenderType::Value>(item_info->gender_type);
	}
#else
	GenderType::Value gender = ResourceCode::GetBaseGenderOfClass(char_class_type);
#endif //_NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG

	Player* player = g_ObjectManager.GetPlayer(PreviewProduct.character_key);
	if (player == NULL)
	{
		ClassCodeEx class_code_ex;
		class_code_ex.class_type = char_class_type;
		class_code_ex.gender = gender;//GenderType::kMale;

		player = (Player*)g_ObjectManager.Add(PreviewProduct.character_key, PLAYER_OBJECT, class_code_ex.class_code, FALSE);
	}
	if (player != NULL)
	{
		player->SetHeightVariation(2);

		// 이킵 정보 셋팅한다.
		EquipPlayerParts(player, item_info, kPreview_Product);
		player->RefreshAppearRender();
		player->SetShow(FALSE);
	}


	eCHAR_TYPE Type1 = ResourceCode::GetImageClassType( char_class_type, GenderType::kMale );
	eCHAR_TYPE Type2 = ResourceCode::GetImageClassType( char_class_type, GenderType::kFemale );

	for (int i = eCHAR_BERSERKER; i < eCHAR_TYPE_MAX; ++i)
	{
		skin_class_tab().SetEnable(i,false);
	}

	if( item_info->m_wWICode != 0 )
	{
		skin_class_tab().SetEnable(Type1, true );
	}
	if( item_info->m_wWICode2 != 0 )
	{
		skin_class_tab().SetEnable(Type2, true );
	}


#ifdef _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG
	skin_class_tab().SetVisible(false);
#else
	skin_class_tab().SetVisible(true);
#endif // _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG
	skin_class_tab().Click(char_class_type, false);

	return (player!=NULL);
}
#endif//_NA_008633_20160104_ADD_DECORATECOSTUME
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::CreateProductPet(const BASE_ITEMINFO* item_info)
{
    if (item_info == NULL)
    {
        return false;
    }
    sPet_Script_Info* pet_info = PetInfoParser::Instance()->GetPetInfo(item_info->m_wSkillCode);
    if (pet_info == NULL)
    {
        return false;
    }

    PreviewProduct.pet = new Pet;
    PreviewProduct.pet->Create(0, pet_info->m_MonsterCode); 
    PreviewProduct.pet->RefreshApperPet(NULL);    
    PreviewProduct.pet->SetFadeIn(0);
    PreviewProduct.pet->SetFreeMove(TRUE);

    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::CreateProductRider(const BASE_ITEMINFO* item_info)
{
    if (item_info == NULL)
    {
        return false;
    }

    PreviewProduct.rider = new Rider;
    PreviewProduct.rider->Create(item_info->m_wSkillCode, NULL, FALSE);  
    PreviewProduct.rider->SetAnimation(GlobalFunc::GetRiderAnimationByPlayerAni(eANI_STAND, false), TRUE, TRUE); 

    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Basket::CreateProductPlayer(ConvenienceStore::ProductItem* basket, 
                                                    eCHAR_TYPE char_type, 
                                                    GenderType::Value genter_type)
{
    Player* player = g_ObjectManager.GetPlayer(PreviewProduct.character_key);
    if (player == NULL)
    {
        ClassCodeEx class_code_ex;
        class_code_ex.class_type = char_type;
        class_code_ex.gender = genter_type;//GenderType::kMale;

        player = (Player*)g_ObjectManager.Add(PreviewProduct.character_key, PLAYER_OBJECT, class_code_ex.class_code, FALSE);
    }
    if (player != NULL)
    {
        player->SetHeightVariation(2);

        // 이킵 정보 셋팅한다.
        EquipPlayerParts(player, basket, kPreview_Product);
        player->RefreshAppearRender();
        player->SetShow(FALSE);
    }
    return (player != NULL);
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::ProcessPlayer(DWORD tick)
{
    //ObjectManager에서 한다.
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::ProcessRider(DWORD tick)
{
    if (PreviewRider.rider)
    { 
        PreviewRider.rider->Process(tick, NULL);
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::ProcessProduct(DWORD tick)
{
    Player* player = g_ObjectManager.GetPlayer(PreviewProduct.character_key);
    if (player != NULL)
    {
        //ObjectManager에서 한다.
    }
    else if (PreviewProduct.pet)
    { 
        PreviewProduct.pet->Process(tick); 
    }
    else if (PreviewProduct.rider)
    { 
        PreviewProduct.rider->Process(tick, NULL);
    }
    else
    {
        TurnRight(tick * 0.001f);
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::RenderPlayer(CControlWZ* picture)
{
    if (picture == NULL || player() == NULL)
    {
        return;
    }

    player()->SetShow(TRUE);
    g_InterfaceManager.RenderPlayerOnRect(player(), picture->GetSizeRect(), screen_info().rot, screen_info().org, screen_info().zoom);
    player()->SetShow(FALSE);

    //ConvenienceStore::ProductItem* basket = NULL;
    //for (int i = kControl_Button_B005; i <= kControl_Button_B013; ++i)
    //{
    //    CControlWZ* control = GetControlWZ_byPosition(i);
    //    if (control == NULL)
    //    {
    //        continue;
    //    }
    //    basket = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData());
    //    if (basket != NULL && basket->compare_type(ConvenienceStore::Define::kObjType_Product))
    //    {
    //        if (basket->package_image != NULL)
    //        {
    //            basket->package_image->Render(&(control->GetSizeRect()));
    //        }
    //    }
    //}
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::RenderRider(CControlWZ* picture)
{
    if (picture == NULL)
    {
        return;
    }

    if (PreviewRider.rider != NULL)
    {
        g_InterfaceManager.RenderRiderOnRect(PreviewRider.rider, picture->GetSizeRect(), screen_info().rot, screen_info().org, screen_info().zoom);
    }

    //for (int i = kControl_Button_B015; i <= kControl_Button_B018; ++i)
    //{
    //    CControlWZ* control = GetControlWZ_byPosition(i);
    //    ConvenienceStore::ProductItem* product = reinterpret_cast<ConvenienceStore::ProductItem*>(control->GetUserData());
    //    if (product != NULL)
    //    {
    //        product->package_image->Render(&(control->GetSizeRect()));
    //    }
    //}
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Basket::RenderProduct(CControlWZ* picture)
{
    if (picture == NULL)
    {
        return;
    }

    Player* player = g_ObjectManager.GetPlayer(PreviewProduct.character_key);
    if (player != NULL)
    {
        player->SetShow(TRUE);
        g_InterfaceManager.RenderPlayerOnRect(player, picture->GetSizeRect(), screen_info().rot, screen_info().org, screen_info().zoom);
        player->SetShow(FALSE);
    }
    else if (PreviewProduct.parts != NULL)
    {
        g_InterfaceManager.RenderUnitDrawOnRect(PreviewProduct.parts, picture->GetSizeRect(), screen_info().rot, screen_info().org, screen_info().zoom);
    }
    else if (PreviewProduct.pet != NULL)
    {
        g_InterfaceManager.RenderPetOnRect(PreviewProduct.pet, 
                                        NULL,
                                        picture->GetSizeRect(), 
                                        screen_info().rot, 
                                        screen_info().org, 
                                        screen_info().zoom);
    }
    else if (PreviewProduct.rider != NULL)
    {
        g_InterfaceManager.RenderRiderOnRect(PreviewProduct.rider, picture->GetSizeRect(), screen_info().rot, screen_info().org, screen_info().zoom);
    }

}

void uiConvenienceStore_Basket::SetActiveProductTab()
{
    preview_tab().Click( kPreview_Product );
}
#ifdef _NA_000000_20130114_RENEWER_UI
void uiConvenienceStore_Basket::SetChangeBackgroundIsClass( eCHAR_TYPE charater_type )
{
	switch (charater_type)
	{
	case eCHAR_BERSERKER:
        {
            if (GetControlWZ_byPosition(kControl_Picture_006) &&
                GetControlWZ_byPosition(kControl_Picture_012) &&
                GetControlWZ_byPosition(kControl_Picture_013) &&
                GetControlWZ_byPosition(kControl_Picture_014) &&
                GetControlWZ_byPosition(kControl_Picture_018) &&
                GetControlWZ_byPosition(kControl_Picture_019))
            {
                GetControlWZ_byPosition(kControl_Picture_006)->ShowWindowWZ(WZ_SHOW);
                GetControlWZ_byPosition(kControl_Picture_012)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_013)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_014)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_018)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_019)->ShowWindowWZ(WZ_HIDE);
            }
		}
		break;
	case eCHAR_DRAGON:
		{
            if (GetControlWZ_byPosition(kControl_Picture_006) &&
                GetControlWZ_byPosition(kControl_Picture_012) &&
                GetControlWZ_byPosition(kControl_Picture_013) &&
                GetControlWZ_byPosition(kControl_Picture_014) &&
                GetControlWZ_byPosition(kControl_Picture_018) &&
                GetControlWZ_byPosition(kControl_Picture_019))
            {
                GetControlWZ_byPosition(kControl_Picture_006)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_012)->ShowWindowWZ(WZ_SHOW);
                GetControlWZ_byPosition(kControl_Picture_013)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_014)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_018)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_019)->ShowWindowWZ(WZ_HIDE);
            }
		}
		break;
	case eCHAR_ELEMENTALIST:
		{
            if (GetControlWZ_byPosition(kControl_Picture_006) &&
                GetControlWZ_byPosition(kControl_Picture_012) &&
                GetControlWZ_byPosition(kControl_Picture_013) &&
                GetControlWZ_byPosition(kControl_Picture_014) &&
                GetControlWZ_byPosition(kControl_Picture_018) &&
                GetControlWZ_byPosition(kControl_Picture_019))
            {
                GetControlWZ_byPosition(kControl_Picture_006)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_012)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_013)->ShowWindowWZ(WZ_SHOW);
                GetControlWZ_byPosition(kControl_Picture_014)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_018)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_019)->ShowWindowWZ(WZ_HIDE);
            }
		}
		break;
	case eCHAR_MYSTIC:
        {
            if (GetControlWZ_byPosition(kControl_Picture_006) &&
                GetControlWZ_byPosition(kControl_Picture_012) &&
                GetControlWZ_byPosition(kControl_Picture_013) &&
                GetControlWZ_byPosition(kControl_Picture_014) &&
                GetControlWZ_byPosition(kControl_Picture_018) &&
                GetControlWZ_byPosition(kControl_Picture_019))
            {
                GetControlWZ_byPosition(kControl_Picture_006)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_012)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_013)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_014)->ShowWindowWZ(WZ_SHOW);
                GetControlWZ_byPosition(kControl_Picture_018)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_019)->ShowWindowWZ(WZ_HIDE);
            }
		}
		break;
	case eCHAR_SHADOW:
		{
            if (GetControlWZ_byPosition(kControl_Picture_006) &&
                GetControlWZ_byPosition(kControl_Picture_012) &&
                GetControlWZ_byPosition(kControl_Picture_013) &&
                GetControlWZ_byPosition(kControl_Picture_014) &&
                GetControlWZ_byPosition(kControl_Picture_018) &&
                GetControlWZ_byPosition(kControl_Picture_019))
            {
                GetControlWZ_byPosition(kControl_Picture_006)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_012)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_013)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_014)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_018)->ShowWindowWZ(WZ_SHOW);
                GetControlWZ_byPosition(kControl_Picture_019)->ShowWindowWZ(WZ_HIDE);
            }
		}
		break;
	case eCHAR_VALKYRIE:
		{
            if (GetControlWZ_byPosition(kControl_Picture_006) &&
                GetControlWZ_byPosition(kControl_Picture_012) &&
                GetControlWZ_byPosition(kControl_Picture_013) &&
                GetControlWZ_byPosition(kControl_Picture_014) &&
                GetControlWZ_byPosition(kControl_Picture_018) &&
                GetControlWZ_byPosition(kControl_Picture_019))
            {
                GetControlWZ_byPosition(kControl_Picture_006)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_012)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_013)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_014)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_018)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_019)->ShowWindowWZ(WZ_SHOW);
            }
		}
		break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eCHAR_HELLROID:
        {
            if (GetControlWZ_byPosition(kControl_Picture_006) &&
                GetControlWZ_byPosition(kControl_Picture_012) &&
                GetControlWZ_byPosition(kControl_Picture_013) &&
                GetControlWZ_byPosition(kControl_Picture_014) &&
                GetControlWZ_byPosition(kControl_Picture_018) &&
                GetControlWZ_byPosition(kControl_Picture_019))
            {
                GetControlWZ_byPosition(kControl_Picture_006)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_012)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_013)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_014)->ShowWindowWZ(WZ_SHOW);
                GetControlWZ_byPosition(kControl_Picture_018)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_019)->ShowWindowWZ(WZ_HIDE);
            }
        }
        break;
    case eCHAR_WITCHBLADE:
        {
            if (GetControlWZ_byPosition(kControl_Picture_006) &&
                GetControlWZ_byPosition(kControl_Picture_012) &&
                GetControlWZ_byPosition(kControl_Picture_013) &&
                GetControlWZ_byPosition(kControl_Picture_014) &&
                GetControlWZ_byPosition(kControl_Picture_018) &&
                GetControlWZ_byPosition(kControl_Picture_019))
            {
                GetControlWZ_byPosition(kControl_Picture_006)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_012)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_013)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_014)->ShowWindowWZ(WZ_SHOW);
                GetControlWZ_byPosition(kControl_Picture_018)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ_byPosition(kControl_Picture_019)->ShowWindowWZ(WZ_HIDE);
            }
        }
        break;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	}
}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI