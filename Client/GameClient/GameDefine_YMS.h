#pragma once
//////////////////////////////////////////////////////////////////////////
//		����� �۾� ������ YJW, i4u4me
//////////////////////////////////////////////////////////////////////////
//

//#define _YJW_SHOP_USING_PPCARD  //! 2011.8.8 / i4u4me /  ppī��
//#define _YJW_SHOP_USING_MILEAGE  //! 2011.8.8 / i4u4me / ���ϸ���

//#define _YJW_TUTORIAL_SKIP  //! 2011.8.9 / i4u4me / Ʃ�丮�� ��ŵ 

//#define _YJW_CREATECHARACTER_MOVIE //! 2011.8.3 / i4u4me / ĳ���� ������ ������ ����

//#define _NA003109_STYLE_PRESENTATION_IMPROVEMENT  //! 2011.7.28 / i4u4me /  

//#define _YMS_CHANNEL_LIST_COUNT5    //! 2011.7.12 / i4u4me / ���� �ټ��� 

//#if (WZENGINEVER >= 400)
    //#define _YMS_TREECONTROL   //! 2011.6.13 / i4u4me /  Ʈ����Ʈ�� / WZENGINEVER >= 400
//#endif //#if (WZENGINEVER >= 40


//#define _YMS_GENDER_FULLSETITEM //! 2011.5.24 / i4u4me / Ǯ�� ��ũ��Ʈ ���� 

//#define _YMS_WINDOW_MAXSCREEN   //! 2011.4.7 / i4u4me / â��� �ִ�ȭ ��� �߰� 

//#define _YMS_AUTOBOT_ITEM_KIND //! 2011.3.23 / i4u4me / �ڵ� ��� ������ �������� �߰� 

//#define _YMS_FULLSETITEM_RESOURCE_DIVISION_ //! 2011.3.4 / i4u4me / ����Ʈ2 Ǯ�� ��Ʈ ��ȯ ���Ŀ��� �и�

//#define _NA001955_110210_WING_COSTUME   // 2010.02.10 / �����, ������ / �Ĵ��Ĵ� ���� ������ ���� �۾�

//#define _NA001605_EP2_CHANGE_SKILL_BROADCASTER_ //! 2011.3.3 / i4u4me / ��ų ��ũ 

//#define _YMS_SKILL_TOOLTIP_ //! 2011.2.28 / i4u4me / ��ų���� ����: �����߰� �ɼ� �ֱ�

//#define _YMS_PARTY_ERRORCODE_   //! 2011.2.11 / i4u4me / ��Ƽ ���� �ʴ�Ǵ� ���� ���� 

//#define _YMS_DEATH_SYNC_    //! 2011.2.21 / i4u4me / ���� ��� ����ȭ

//#define _NA_0_20110216_STATUS_STAT_POINT_SIMULATION           // 2011.02.16 / �����,����� / ���� ����Ʈ �ùķ��̼� �߰�.
//#if defined(_NA_0_20110216_STATUS_STAT_POINT_SIMULATION)
//    #define _YMS_STAT_SIMULATION_   //! 2011.2.16 / i4u4me / ���� �ùķ��̼� 
//#endif

//#define _YMS_SMOOTH_GAUGE_ //! 2011.2.14 / i4u4me / hp, mp ������ �������ϰ� �����̰�. 


//#define _YMS_LOGIN_RENEWAL_
//#if defined(_YMS_LOGIN_RENEWAL_)
    //#define _YMS_SCREEN_RATIO_  //! 2011.1.24 / i4u4me / 16:10 ���� �߰�  (WZENGINEVER >= 280) �̻�
    //#define _YMS_LOGIN_CHARACTER_CAMERA_RENEWAL_    //! 2011.1.25 / i4u4me / ĳ���� ī�޶� ���� ���� 
    //#define _YMS_LOADING_IMAGE_ //! 2011.1.27 / i4u4me / �ε� �̹��� �ϳ��� ���� 
//#endif 

//#define _NA001605_EP2_CHANGE_SKILL_BROADCASTER_ // ������, ����� / skill broadcaster �̺�Ʈ ������� ����
//#if defined(_NA001605_EP2_CHANGE_SKILL_BROADCASTER_)
//    #define _YMS_EP2_SKILL_PACKET_RENEWAL_ //! 2011.1.10 / i4u4me / ��ų ��Ŷ ����
//#endif

//#define _NA001605_EP2_ATTRIBUTES_RENEWAL_
//#if defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
//    #define _YMS_EP2_ATTRIBUTES_RENEWAL_     //! 2011.1.7 / i4u4me / ĳ���� �Ӽ� ������
//#endif
//
//
////#define _YMS_DIALOG_ANIMATION_ //! 2010.12.13 / i4u4me / ���̾�α� �����̰�  DialogPosInfo.txt  �� ���� �����Ǿ�� ��
////#define _YMS_TEXTURE_ANIMATION_ //! 2010.12.27 / i4u4me / �ؽ��� �ִϸ��̼�  TextureListInfo.txt, TextureAnimationInfo.txt ���� �����Ǿ�� ��
//#if defined(_YMS_DIALOG_ANIMATION_) && defined(_YMS_TEXTURE_ANIMATION_)
//    #define _YMS_LOGIN_RENEWAL_     //! 2010.12.13 / i4u4me / �α��� ȭ�� ������ 
//#endif


//#define ENGINE_LIB_PATH  "../../../SunEngine/trunk/lib"

//#define _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_                         // �����릢000000��2010/0118~01  ����������ĳ��Ÿ��(PPī��) ��ɰ���(����:������)
//#define __NA_00387_20070605_SHOP_MILEAGE2
//#define __CN_0_20070507_SHOP_MILEAGE

//#define _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING   //! 2010.10.13 / i4u4me / �����ۼ� ���� �޴� �߰� , �������� 250 �̻�

//#define _YMS_TRADING_DONOTOPEN_SYSTEMDIALOG //! 2010.10.19 / i4u4me / �ŷ����ϰ�쿡�� �ý��� �޴� ���� �ʴ´� 


//#define _YMS_AUTOBOT_GAME  // [2010-7-1 by i4u4me] �������� �������� 211 ���� �����Ѵ�.

//------------------------------------------------------------------------------
//����
//#define _YMS_ADD_RADAR_ICON_100622                  // 2010.06.17 /  �̴ϸ� ǥ�� ������Ʈ 
//#define _YMS_ESCKEY_CLOSE_DIALOG                    // 2010.06.14 /  �Ϻ� UI�� ESC Ű�� ������ �ʴ� ����
//#define _YMS_SYSTEMINPUTBOX_ONLYNUMBER              // 2010.05.28 /  �ý��� ��ǲ â���� ���ڸ� ����ġ�� ���� ����
//#define _NA_0_20100527_ADD_CHAR_RELATION_MEMO       // 2010.06.10 /  ģ�� �޸��� �߰�
//#define _YMS_PET_UI                                 // 2010.05.18 /  �� UI ����. 
//#define _YMS_GUILD_DETAL_SPEC                       // 2010.05.11 /  ���� �� ���� ��� 
//#define _YMS_REQUEST_PACKET_QUEST_COMPLETE_SYN      // 2010.05.11 /  ��Ÿ��ġ���� �̼� ���� ������ ���� �Ϸ� �ȵǴ� ���� 

//#define _YMS_CHINA_TAIWAN_MERGE                     // 2010.07.15 / ����� / �߱�/�븸 ��ħ 

//#define _YMS_EVENTFRIEND_SCROLLBAR       // [2010-5-14 by i4u4me] ģ���ʴ� �޼��� ��ũ�� �ǰ�

//#define _YMS_LANGUAGE_KIND // [2010-5-6 by i4u4me] ������ ��� 

//#define _YMS_SUMMONSKILLCONTAINER_RESET // [2010-5-7 by i4u4me] ��Ű�� ��ȯ�� ��ųâ ����


//------------------------------------------------------------------------------
//! �ӽ�
//#define _YMS_USING_FROST_LIBRARY_

//#define _APPLY_HACKSHIELD_MONITOR                           // 2010.03.17 / ����� / �߱� �ٽ��� ����͸� ����
//#ifdef _APPLY_HACKSHIELD_MONITOR
//    #define __NA000000_070913_HACKSHIELD_INSTALLATION__
//#endif

//#define _YMS_CONTROL_REDUCE_TEXT    // [2010-3-11 by i4u4me]  _INTERNATIONAL_UI �� ���̻������ ����
//
//#define _YMS_NUMBER_TO_MONEY // [2010-3-2 by i4u4me] ���� õ������ �޸� �Է�
//#define _YMS_ESCKEY_COMPOSITE  // [2010-2-26 by i4u4me]  
//#define _YMS_UI_RBUTTONCLICKI_ENCHANT_CRYSTALLIZATION_RANKUP // [2010-2-17 by i4u4me] [�ѱ�][0032] ��ȭâ ������ ��� ���� ����
//#define _YMS_UI_STATE_REMEMBER          // [2010-2-17 by i4u4me] [�ѱ�][0030]UI Ŭ�� ���� ����
//#define _YMS_AREACONQUEST_MAP_TOOLTIP   // [2010-2-17 by i4u4me] [�ѱ�][0025]AC �������� ���� ��� 
//#define _YMS_SOUND_CACHE
//#define _YMS_GAMEOPTION_LANGUAGETYPE  //< [2010-1-28 by i4u4me] ������ ���� �߰�
//#define _YMS_RANKUP_TOOLTIP   //< [2010-1-26 by i4u4me] ��ũ�� ��������(���� ��� ���� ��ġ �����)
//#define _YMS_CANNOT_EMOTION	//< [2010-1-25 by i4u4me] �Ǿ� ���¿��� ����ǥ�� �ȵǰ�
