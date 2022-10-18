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
            //�ڵ�	��Ʈ��	�з�
            //2500	��ü����	��������
            //2501	����	    ��������
            //2502	�̺�Ʈ	��������
            //2503	��ü����	���
            //2504	���	���
            //2505	����	���
            //2506	�尩	���
            //2507	����	���
            //2508	����	���
            //2509	��������	���
            //2510	��Ʈ	���
            //2511	����	���
            //2512	����	���
            //2513	��ü����	��Ų
            //2514	���	��Ų
            //2515	����	��Ų
            //2516	�尩	��Ų
            //2517	����	��Ų
            //2518	����	��Ų
            //2519	��������	��Ų
            //2520	��Ʈ	��Ų
            //2521	����	��Ų
            //2522	����	��Ų
            //2523	��ü����	Ŀ���͸���¡
            //2524	���	Ŀ���͸���¡
            //2525	��	Ŀ���͸���¡
            //2526	��Ÿ	Ŀ���͸���¡
            //2527	��ü����	������ 
            //2528	����	������ 
            //2529	����	������ 
            //2530	����	������ 
            //2531	����Ƿ�	������ 
            //2532	�ɷ�ġ��ȭ��	������ 
            //2533	��ɼ���	������ 
            //2534	���Ǽ��� 	������ 
            //2535	��ü����	��
            //2536	���	��
            //2537	��	��
            //2538	��Ų	��
            //2539	��ü����	Ż��
            //2540	Ż��	Ż��
            //2541	�Ӹ�����	Ż��
            //2542	��������	Ż��
            //2543	�ٸ�����	Ż��
            //2544	��ü Ŭ����	ĳ�û��� ����
            //2545	����Ŀ	ĳ�û��� ����
            //2546	�巡��	ĳ�û��� ����
            //2547	��Ű�� 	ĳ�û��� ����
            //2548	������Ż	ĳ�û��� ����
            //2549	������	ĳ�û��� ����
            //2550	��ü ��Ʈ	��Ű������ ����
            //2551	����Ŀ ��Ʈ	��Ű������ ����
            //2552	�巡�� ��Ʈ	��Ű������ ����
            //2553	��Ű�� ��Ʈ	��Ű������ ����
            //2554	������Ż ��Ʈ	��Ű������ ����
            //2555	������ ��Ʈ	��Ű������ ����
            //2556	��Ÿ ������ ��Ʈ	��Ű������ ����

            //155527	���
            //155528	�̸�����
            //155529	����
            //155530	����
            //155531	��������
            //155532	����(����)
            //155533	����
            //155534	Ȯ��
            //155535	�̵�


            //155495	%s
            //155496	�� %s���� �������� ���� �ϼ̽��ϴ�.
            //155497	%s
            //155498	���� ������
            //155499	���� ����
            //155500	���� ������
            //155501	���� �ϱ�
            //155502	�ϴ��� ���� ��ư��  ������ �� �������� ���Ե˴ϴ�.\n�� Ȯ�� �� ���� �Ͻñ� �ٶ��ϴ�.
            //155503	�ϴ��� ���� ��ư��  ������ �� �������� ���Ե˴ϴ�.\n�� Ȯ�� �� ���� �Ͻñ� �ٶ��ϴ�.
            //155504	���� �����Ͻ� �������� �����Ͽ� �������� ������\n�Ͻ� �� �����ϴ�. ĳ�� ���� �Ŀ� ���� �Ͻñ� �ٶ��ϴ�.
            //155505	���� 
            //155506	����
            //155507	���� ���� �Ͻðڽ��ϱ�?
            //155508	���� ���� �Ͻðڽ��ϱ�?
            //155509	���� �� 7�� �̳� ���� öȸ�� �����ϸ�,\n���� ������ �� �ŷ��� �ְ� ���� ��������\n���� öȸ�� �Ұ����մϴ�.
            //155510	���� ������� ���۵� �������� ���� öȸ��\n�Ұ��� �մϴ�.
            //155511	�� %s���� �������� ���� �ϼ̽��ϴ�.
            //155512	�� %s���� �������� ���� �ϼ̽��ϴ�.
            //155513	�� %s���� ��Ű�� �������� ���� �ϼ̽��ϴ�.
            //155514	�� %s���� ��Ű�� �������� ���� �ϼ̽��ϴ�.
            //155515	�޴���:
            //155516	���� Ȯ��
            //    155517	���� Ȯ��
            //    155518	�޴º��� ĳ���� ���� �Է��� �ּ���.
            //    155519	���� �п��� ������ �����ϴ�.
            //    155520	üũ
            //    155521	�ش� ĳ���� ���� �������� �ʽ��ϴ�.
            //    155522	ģ�� ����Ʈ
            //    155523	��� ����Ʈ
            //    155524	%s
            //    155525	%s��
            //    155526	%s��

            //2557	�Ż�ǰ ��
            //2558	������ ��
            //2559	�α� ��
            //2560	���� ��
            //2561	���� ����
            //2562	�̸� ��
            //2563	�̸� ����
            //2564	��¥ ��
            //2565	��¥ ����

            //�� ����
            //3.30.	�̹����±� ����(ImageTagType)
            //1	680	NEW
            //2	681	HOT
            //3	682	SALE
            //4	683	EVENT

            //155009	�űԻ�ǰ
            //155010	�α��ǰ
            //155011	����
            //155012	�̺�Ʈ

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

            //����
            kShopStringCode_Filter_Sort = 2557,
            kShopStringCode_Filter_New_Inc = 2557,    //�Ż�ǰ ��
            kShopStringCode_Filter_New_Desc = 2558,    //������ ��
            kShopStringCode_Filter_Hit_Desc = 2559, 	//�α� ��
            kShopStringCode_Filter_Price_Inc = 2561,    //���� ���� ��
            kShopStringCode_Filter_Price_Desc = 2560,    //���� ���� ����
            kShopStringCode_Filter_Name_Inc = 2562,    //�̸� ��
            kShopStringCode_Filter_Name_Desc = 2563,    //�̸� ����
            kShopStringCode_Filter_Date_Inc = 2564,    //��¥ ��
            kShopStringCode_Filter_Date_Desc = 2565,    //��¥ ����
            kShopStringCode_Filter_End = 2565,


            kShopStringCode_Shop_Popup_Pickup = 155527,
            kShopStringCode_Shop_Popup_Preview = 155528,
            kShopStringCode_Shop_Popup_Buy = 155530,
            kShopStringCode_Shop_Popup_Gift = 155529,

            //155554	�޽���
            //155555	�ޱ�
            kShopStringCode_Shop_Popup_Msg = 155554,
            kShopStringCode_Shop_Popup_Recv = 155555,

            kShopStringCode_Basket_Popup_Buy = 155530,
            kShopStringCode_Basket_Popup_Gift = 155529,
            kShopStringCode_Basket_Popup_Equip = 155532,
            kShopStringCode_Basket_Popup_Unequip = 155531,
            kShopStringCode_Basket_Popup_Delete = 155533,
            kShopStringCode_Basket_Popup_Preview = 155528,  //!<�̸�����

            kShopStringCode_End = 2563
        };

        //! �� �޴� ����
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
            kMyPageType_BuyBox = 0, //!< ���� ������ ���
            kMyPageType_RecvBox,    //!< ���� ���� ���
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
        //138	�Ҹ� ������	111	��-�⺻�Ӽ�	146	����	141	������ �Ӽ�
        //138	�Ҹ� ������	114	��-��æƮ	146	����	141	������ �Ӽ�
        //138	�Ҹ� ������	117	��-����	146	����	141	������ �Ӽ�
        //139	���� ������	112	��-�⺻�Ӽ�	146	����	141	������ �Ӽ�
        //139	���� ������	115	��-��æƮ	146	����	141	������ �Ӽ�
        //139	���� ������	118	��-����	146	����	141	������ �Ӽ�
        //140	�Ⱓ�� ������	113	��-�⺻�Ӽ�	146	����	141	������ �Ӽ�
        //140	�Ⱓ�� ������	116	��-��æƮ	146	����	141	������ �Ӽ�
        //140	�Ⱓ�� ������	119	��-����	146	����	141	������ �Ӽ�
        //�Ҹ�     ���� property_seq = 7
        //������     ���� property_seq = 8
        //�Ⱓ       ���� property_seq = 10, ���� 9

        static const int kProductType[kShopProduct_Size]            = {138, 139, 140}; 
        static const int kPropertySeq_Waste[kShopProperty_Size]     = {7,  111, 114, 117};     
        static const int kPropertySeq_Infinite[kShopProperty_Size]  = {8,  112, 115, 118};     
        static const int kPropertySeq_Date[kShopProperty_Size]      = {10, 113, 116, 119};    
        static const int kPropertyType_Price = 142;     //!< ������ ���� Ÿ��
        static const int kPropertyType_Attribute = 141; //!< ������ �Ӽ� Ÿ��
        static const int kCapsuleFlag_Normal = 177; //!< �Ϲ� ��ǰ
        static const int kCapsuleFlag_Lotto = 176; //!< �ζ� ��ǰ

    }


    Define::ShopProductType GetProductType(IN CShopProduct* const shop_product);

    //! ���� ��ǰ�� �����ϴ�.
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
    //! Qualifier: �ش� īŸ���� �ִ� ��� ��Ű���� �˻��ؼ� , hot, new, sale, event �ױװ� �ִ��� �˻��Ѵ�.
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
