#pragma once

#include <slm/IncludeSLM.h>

namespace ConvenienceStore
{
    class PropertyInfo;
    class ProductInfo;
    class ProductItem;
    class PurchaseItem;

    namespace Define
    {
        enum ShopStringCode
        {
            //코드	스트링	분류
            //2500	전체보기	모든아이템
            //2501	세일	    모든아이템
            //2502	이벤트	모든아이템
            //2503	전체보기	장비
            //2504	헬멧	장비
            //2505	갑옷	장비
            //2506	장갑	장비
            //2507	바지	장비
            //2508	부츠	장비
            //2509	프로텍터	장비
            //2510	벨트	장비
            //2511	셔츠	장비
            //2512	무기	장비
            //2513	전체보기	스킨
            //2514	헬멧	스킨
            //2515	갑옷	스킨
            //2516	장갑	스킨
            //2517	바지	스킨
            //2518	부츠	스킨
            //2519	프로텍터	스킨
            //2520	벨트	스킨
            //2521	셔츠	스킨
            //2522	무기	스킨
            //2523	전체보기	커스터마이징
            //2524	헤어	커스터마이징
            //2525	얼굴	커스터마이징
            //2526	기타	커스터마이징
            //2527	전체보기	아이템 
            //2528	행운류	아이템 
            //2529	재료류	아이템 
            //2530	길드류	아이템 
            //2531	입장권류	아이템 
            //2532	능력치강화류	아이템 
            //2533	기능성류	아이템 
            //2534	편의성류 	아이템 
            //2535	전체보기	펫
            //2536	사료	펫
            //2537	펫	펫
            //2538	스킨	펫
            //2539	전체보기	탈것
            //2540	탈것	탈것
            //2541	머리파츠	탈것
            //2542	몸통파츠	탈것
            //2543	다리파츠	탈것
            //2544	전체 클레스	캐시상점 필터
            //2545	버서커	캐시상점 필터
            //2546	드래곤	캐시상점 필터
            //2547	발키리 	캐시상점 필터
            //2548	엘리멘탈	캐시상점 필터
            //2549	새도우	캐시상점 필터
            //2550	전체 세트	패키지상점 필터
            //2551	버서커 세트	패키지상점 필터
            //2552	드래곤 세트	패키지상점 필터
            //2553	발키리 세트	패키지상점 필터
            //2554	엘리멘탈 세트	패키지상점 필터
            //2555	새도우 세트	패키지상점 필터
            //2556	기타 아이템 세트	패키지상점 필터

            //155527	담기
            //155528	미리보기
            //155529	선물
            //155530	구입
            //155531	장착해제
            //155532	장착(보기)
            //155533	삭제
            //155534	확인
            //155535	이동


            //155495	%s
            //155496	총 %s개의 아이템을 선택 하셨습니다.
            //155497	%s
            //155498	현재 소지금
            //155499	구입 가격
            //155500	남은 소지금
            //155501	선물 하기
            //155502	하단의 구입 버튼을  누르면 위 아이템이 구입됩니다.\n잘 확인 후 결제 하시길 바랍니다.
            //155503	하단의 선물 버튼을  누르면 위 아이템이 구입됩니다.\n잘 확인 후 결제 하시길 바랍니다.
            //155504	현재 보유하신 소지금이 부족하여 정상적인 구입을\n하실 수 없습니다. 캐시 충전 후에 구입 하시길 바랍니다.
            //155505	구입 
            //155506	선물
            //155507	정말 구입 하시겠습니까?
            //155508	정말 선물 하시겠습니까?
            //155509	구매 후 7일 이내 구매 철회가 가능하며,\n사용된 아이템 및 거래로 주고 받은 아이템은\n구매 철회가 불가능합니다.
            //155510	선물 보내기로 전송된 아이템은 구매 철회가\n불가능 합니다.
            //155511	총 %s개의 아이템을 구입 하셨습니다.
            //155512	총 %s개의 아이템을 선물 하셨습니다.
            //155513	총 %s개의 패키지 아이템을 구입 하셨습니다.
            //155514	총 %s개의 패키지 아이템을 선물 하셨습니다.
            //155515	받는이:
            //155516	구매 확인
            //    155517	선물 확인
            //    155518	받는분의 캐릭터 명을 입력해 주세요.
            //    155519	위의 분에게 선물을 보냅니다.
            //    155520	체크
            //    155521	해당 캐릭터 명이 존재하지 않습니다.
            //    155522	친구 리스트
            //    155523	길드 리스트
            //    155524	%s
            //    155525	%s개
            //    155526	%s일

            //2557	신상품 순
            //2558	오래된 순
            //2559	인기 순
            //2560	가격 순
            //2561	가격 역순
            //2562	이름 순
            //2563	이름 역순
            //2564	날짜 순
            //2565	날짜 역순

            //샵 문서
            //3.30.	이미지태그 유형(ImageTagType)
            //1	680	NEW
            //2	681	HOT
            //3	682	SALE
            //4	683	EVENT

            //155009	신규상품
            //155010	인기상품
            //155011	세일
            //155012	이벤트

            //155549	%s%s / %s%s

            kShopStringCode_AllItem = 2500,
            kShopStringCode_Armor = 2503,  
            kShopStringCode_Skin = 2513,    
            kShopStringCode_Customize = 2523,
            kShopStringCode_Item = 2527,     
            kShopStringCode_Pet = 2535,      
            kShopStringCode_Rider = 2539,     
            kShopStringCode_Filter_Cash = 2544, 
            kShopStringCode_Filter_ALL = 2544, 
            kShopStringCode_Filter_Berserker = 2545, 
            kShopStringCode_Filter_Dragon = 11385, //2546, 
            kShopStringCode_Filter_Shadow = 2549, 
            kShopStringCode_Filter_Valkyrie = 2547, 
            kShopStringCode_Filter_Magician = 11384,//2548, 
            kShopStringCode_Filter_Mystic = 2568, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            kShopStringCode_Filter_Hellroid = 2569,
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            kShopStringCode_Filter_WitchBladae = 10008,
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

            kShopStringCode_Filter_Package = 2550,

            //정렬
            kShopStringCode_Filter_Sort = 2557,
            kShopStringCode_Filter_New_Inc = 2557,    //신상품 순
            kShopStringCode_Filter_New_Desc = 2558,    //오래된 순
            kShopStringCode_Filter_Hit_Desc = 2559, 	//인기 순
            kShopStringCode_Filter_Price_Inc = 2561,    //높은 가격 순
            kShopStringCode_Filter_Price_Desc = 2560,    //낮은 가격 역순
            kShopStringCode_Filter_Name_Inc = 2562,    //이름 순
            kShopStringCode_Filter_Name_Desc = 2563,    //이름 역순
            kShopStringCode_Filter_Date_Inc = 2564,    //날짜 순
            kShopStringCode_Filter_Date_Desc = 2565,    //날짜 역순
            kShopStringCode_Filter_End = 2565,


            kShopStringCode_Shop_Popup_Pickup = 155527,
            kShopStringCode_Shop_Popup_Preview = 155528,
            kShopStringCode_Shop_Popup_Buy = 155530,
            kShopStringCode_Shop_Popup_Gift = 155529,

            //155554	메시지
            //155555	받기
            kShopStringCode_Shop_Popup_Msg = 155554,
            kShopStringCode_Shop_Popup_Recv = 155555,

            kShopStringCode_Basket_Popup_Buy = 155530,
            kShopStringCode_Basket_Popup_Gift = 155529,
            kShopStringCode_Basket_Popup_Equip = 155532,
            kShopStringCode_Basket_Popup_Unequip = 155531,
            kShopStringCode_Basket_Popup_Delete = 155533,
            kShopStringCode_Basket_Popup_Preview = 155528,  //!<미리보기

            kShopStringCode_End = 2563
        };

        //! 샾 메뉴 종류
        enum ShopMenuType
        {
            kShopMenuType_Cash = 0,
            kShopMenuType_PPCard,
            kShopMenuType_Mileage,
            kShopMenuType_Mypage,
            kShopMenuType_Size,
        };

        enum MyPageType
        {
            kMyPageType_BuyBox = 0, //!< 내가 구매한 목록
            kMyPageType_RecvBox,    //!< 선물 받은 목록
            kMyPageType_Size,
        };

        enum PopupMenuKind
        {
            kPopupMenuKind_Two = 0,
            kPopupMenuKind_Three,
            kPopupMenuKind_Four,
            kPopupMenuKind_Five,
            kPopupMenuKind_Size,
        };



        //------------------------------------------------------------------------------ 
        enum CashType
        {
            kCashType_Cash = 0,
            kCashType_PPCard,
            kCashType_Mileage,
            kCashType_Size,
        };

        enum ShopPropertyType
        {
            kShopProperty_Price = 0,
            kShopProperty_SellKind,
            kShopProperty_Inchant,
            kShopProperty_Socket,
            kShopProperty_Size,
        };

        enum ShopProductType
        {
            kShopProduct_Waste = 0,
            kShopProduct_Infinite,
            kShopProduct_Date,
            kShopProduct_Size,
        };


        //------------------------------------------------------------------------------ 
        //138	소모성 아이템	111	썬-기본속성	146	선택	141	아이템 속성
        //138	소모성 아이템	114	썬-인챈트	146	선택	141	아이템 속성
        //138	소모성 아이템	117	썬-소켓	146	선택	141	아이템 속성
        //139	영구 아이템	112	썬-기본속성	146	선택	141	아이템 속성
        //139	영구 아이템	115	썬-인챈트	146	선택	141	아이템 속성
        //139	영구 아이템	118	썬-소켓	146	선택	141	아이템 속성
        //140	기간제 아이템	113	썬-기본속성	146	선택	141	아이템 속성
        //140	기간제 아이템	116	썬-인챈트	146	선택	141	아이템 속성
        //140	기간제 아이템	119	썬-소켓	146	선택	141	아이템 속성
        //소모성     갯수 property_seq = 7
        //영구성     갯수 property_seq = 8
        //기간       날자 property_seq = 10, 갯수 9

        static const int kProductType[kShopProduct_Size]            = {138, 139, 140}; 
        static const int kPropertySeq_Waste[kShopProperty_Size]     = {7,  111, 114, 117};     
        static const int kPropertySeq_Infinite[kShopProperty_Size]  = {8,  112, 115, 118};     
        static const int kPropertySeq_Date[kShopProperty_Size]      = {10, 113, 116, 119};    
        static const int kPropertyType_Price = 142;     //!< 아이템 가격 타입
        static const int kPropertyType_Attribute = 141; //!< 아이템 속성 타입
        static const int kCapsuleFlag_Normal = 177; //!< 일반 상품
        static const int kCapsuleFlag_Lotto = 176; //!< 로또 상품

    }


    Define::ShopProductType GetProductType(IN CShopProduct* const shop_product);

    //! 단일 상품만 가능하다.
    bool FindShopProduct(CShopPackage* shop_package, int price_seq, 
        OUT CShopProduct*& shop_product, 
        OUT CShopProductLink*& shop_product_link);


    //------------------------------------------------------------------------------ 
    bool GetDisplayPriceStringByProduct(TCHAR* msg, int str_len, ProductItem* product);
    bool GetDisplayPriceStringByProductInfo(TCHAR* msg, int str_len, ProductInfo* product_info, bool buy_mileage);
    bool GetDisplayPriceStringByProductLink(TCHAR* msg, int str_len, Define::ShopProductType shop_product_type, CShopProductLink* shop_product_link, bool buy_mileage);

    //------------------------------------------------------------------------------ 
    CShopCategoryList* category_list();
    CShopPackageList* package_list();
    CShopProductList* product_list();
    CShopPropertyList* property_list();
    CShopProductLinkList* productlink_list(); 

    //------------------------------------------------------------------------------
    //! Returns:   void
    //! Qualifier: 해당 카타고리에 있는 모든 패키지를 검사해서 , hot, new, sale, event 테그가 있는지 검사한다.
    //! Parameter: CShopCategory * category
    //! Parameter: DWORD new_val        //1	680	NEW
    //! Parameter: DWORD hot_val        //2	681	HOT
    //! Parameter: DWORD sale_val       //3	682	SALE
    //! Parameter: DWORD event_val      //4	683	EVENT
    //------------------------------------------------------------------------------
    void GetImageTag(CShopCategory* shop_category, std::set<int>& image_tag_vector);

    void test_shop_script_list();
    void test_shop_script_list_category(FILE* fp, CShopCategory &category, int deapth);
    void test_shop_script_list_package(FILE* fp, CShopCategory &subcategory, int deapth);
}
