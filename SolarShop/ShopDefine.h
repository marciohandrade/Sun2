
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL




#pragma once

#include <list>

// 공통 typedef 
typedef INT		ITEMGUID;
typedef INT		PRICEGUID;
typedef INT		GIFTGUID;


class ShopItem;
class ShopItemPrice;
class PackageItem;
typedef std::list<ShopItem*>						SHOPITEM_LIST;
typedef std::list<ShopItemPrice*>					SHOPITEMPRICE_LIST;
typedef std::list<PackageItem*>						PACKITEM_LIST;





//#ifdef _NA_0_20101015_SOLAR_SHOP_REFACTORING
//  모든 아이템  : 전체보기/세일/이벤트
//  방어구       : 전체보기/헬멧/갑옷/장갑/바지/부츠/프로텍터/벨트/셔츠
//  스킨         : 전체보기/헬멧/갑옷/장갑/바지/부츠/프로텍터/벨트/셔츠
//  커스터마이징 : 전체보기/헤어/얼굴/기타
//  아이템       : 전체보기/행운류/재료류/길드류/입장권류/능력치강화류/기능성류/편의성류
//  펫           : 전체보기/사료/펫/스킨
//  탈것         : 전체보기/탈것/머리파츠/몸통파츠/다리파츠
//  PC방 전용    : 전체보기/탈것/머리파츠/몸통파츠/다리파츠



enum eSHOP_PARTITION
{
    SHOP_PARTITION_SINGLEITEM,         // 캐시 아이템
    SHOP_PARTITION_PACKAGEITEM,        // 패키지 아이템
    SHOP_PARTITION_MILEAGEITEM,        // 포인트 아이템
//    SHOP_PARTITION_PRESENTITEM,        // 선물 아이템
    SHOP_PARTITION_MAXCOUNT,
};


enum eSHOP_FILTER
{
    SHOP_FILTER_NONE               = 0,
    SHOP_FILTER_CHAR_BERSERKER     = (1 << 0),
    SHOP_FILTER_CHAR_ELEMENTALIST  = (1 << 1),
    SHOP_FILTER_CHAR_SHADOW        = (1 << 2),
    SHOP_FILTER_CHAR_PALADIN       = (1 << 3),
    SHOP_FILTER_CHAR_VALKYRIE      = (1 << 4),

    SHOP_FILTER_EVENT              = (1 << 9),  // 이벤트
    SHOP_FILTER_DISCOUNT           = (1 << 10),  // 세일
    SHOP_FILTER_NEWARRIVAL         = (1 << 11),  // 신규
    SHOP_FILTER_POPULAR            = (1 << 12),  // 인기

    SHOP_FILTER_CHAR_ALL           = (SHOP_FILTER_CHAR_BERSERKER | SHOP_FILTER_CHAR_ELEMENTALIST |
                                     SHOP_FILTER_CHAR_SHADOW | SHOP_FILTER_CHAR_PALADIN | SHOP_FILTER_CHAR_VALKYRIE),         // => SHOP_FILTER_CHAR_NONE

    SHOP_FILTER_SLOGAN_ALL         = (SHOP_FILTER_EVENT | SHOP_FILTER_DISCOUNT | SHOP_FILTER_NEWARRIVAL | SHOP_FILTER_POPULAR),
};

//#endif //_NA_0_20101015_SOLAR_SHOP_REFACTORING


// 주의!
// 카테고리 번호는 정해져 있기때문에
// 샵 DB와 맞춰야 한다
enum eSHOP_CATEGORY
{
	CT_ALLITEM			= 0,			//< 모든 아이템
	
	// 뷰 타입
	CT_NEWARRIVAL		= 1,			//< 신규 상품
	CT_POPULARITEM		= 2,			//< 인기 상품
	CT_DISCOUNT			= 3,			//< 세일
	CT_EVENT			= 4,			//< 이벤트
	CT_TEMP				= 5,			//< 기존에 PC방이었지만 이젠 사용안하는 뷰 타입
	
	// 카테고리
	CT_WASTEITEM		= 6,			//< 소모성
	CT_ARMOR			= 7,			//< 방어구
	CT_FUNCTIONAL		= 8,			//< 기능성 
	CT_ETC				= 9,			//< 기타
	CT_PACKAGE			= 10,			//< 패키지

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	CT_MILEAGE			= 11,			//< 마일리지
#endif

	CT_PERIOD_EXTEND	= 12,			//< 기간연장
	CT_WEAPON			= 13,			//< 무기
	CT_PCROOM			= 14,			//< PC방

    CT_PPCASH           = 15,           //< PP cash purchase defined by _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_

	CT_MAX,


//#ifdef _NA_0_20101015_SOLAR_SHOP_REFACTORING
    SHOP_CATEGORY_INVALID = CT_MAX + 1, // 유효하지 않은 카테고리

    SHOP_CATEGORY_ROOT,                 // Root 카테고리

    SHOP_CATEGORY_ALL,                  // 모든 아이템

    SHOP_CATEGORY_EQUIP_ALL,            // 장비
    SHOP_CATEGORY_EQUIP_HELMET,         // 헬멧
    SHOP_CATEGORY_EQUIP_ARMOR,          // 갑옷
    SHOP_CATEGORY_EQUIP_GLOVES,         // 장갑
    SHOP_CATEGORY_EQUIP_PANTS,          // 바지
    SHOP_CATEGORY_EQUIP_BOOTS,          // 부츠
    SHOP_CATEGORY_EQUIP_PROTECTOR,      // 프로텍터
    SHOP_CATEGORY_EQUIP_BELT,           // 벨트
    SHOP_CATEGORY_EQUIP_SHIRTS,         // 셔츠
    SHOP_CATEGORY_EQUIP_WEAPON,         // 무기

    SHOP_CATEGORY_SKIN_ALL,             // 스킨
    SHOP_CATEGORY_SKIN_HELMET,          // 헬멧
    SHOP_CATEGORY_SKIN_ARMOR,           // 갑옷
    SHOP_CATEGORY_SKIN_GLOVES,          // 장갑
    SHOP_CATEGORY_SKIN_PENTS,           // 바지
    SHOP_CATEGORY_SKIN_BOOTS,           // 부츠
    SHOP_CATEGORY_SKIN_PROTECTOR,       // 프로텍터
    SHOP_CATEGORY_SKIN_BELT,            // 벨트
    SHOP_CATEGORY_SKIN_SHIRTS,          // 셔츠
    SHOP_CATEGORY_SKIN_WEAPON,          // 무기

    SHOP_CATEGORY_CUSTOMIZING_ALL,      // 커스터마이징 전체
    SHOP_CATEGORY_CUSTOMIZING_HAIR,     // 머리
    SHOP_CATEGORY_CUSTOMIZING_FACE,     // 얼굴
    SHOP_CATEGORY_CUSTOMIZING_ETC,      // 기타

    SHOP_CATEGORY_ITEM_ALL,            // 아이템 전체
    SHOP_CATEGORY_ITEM_LUCKY,          // 행운류
    SHOP_CATEGORY_ITEM_MATERIAL,       // 재료류
    SHOP_CATEGORY_ITEM_GUILD,          // 길드류
    SHOP_CATEGORY_ITEM_TICKET,         // 입장권류
    SHOP_CATEGORY_ITEM_STRENGTHEN,     // 능력치 강화류
    SHOP_CATEGORY_ITEM_FUNCTIONALITY,  // 기능성류
    SHOP_CATEGORY_ITEM_CONVENIENCE,    // 편의성류

    SHOP_CATEGORY_PET_ALL,              // 펫 전체
    SHOP_CATEGORY_PET,                  // 펫
    SHOP_CATEGORY_PET_FEED,             // 사료
    SHOP_CATEGORY_PET_SKIN,             // 스킨

    SHOP_CATEGORY_VEHICLE_ALL,          // 탈것 전체
    SHOP_CATEGORY_VEHICLE,              // 탈것
    SHOP_CATEGORY_VEHICLE_HEAD,         // 머리 파츠
    SHOP_CATEGORY_VEHICLE_BODY,         // 몸통 파츠
    SHOP_CATEGORY_VEHICLE_LEGS,         // 다리 파츠


    SHOP_CATEGORY_PCROOM,               // PC방
    SHOP_CATEGORY_PPCASH,               // PP Cash 전용 아이템.
    SHOP_CATEGORY_PERIOD_EXTEND,        // 기간 연장


    SHOP_CATEGORY_END,                  // 사용할수없다. 카테고리 갯수 측정을 위한 마지막 위치 표시이다.

    SHOP_CATEGORY_MAXCOUNT = SHOP_CATEGORY_END - SHOP_CATEGORY_INVALID,
//#endif // _NA_0_20101015_SOLAR_SHOP_REFACTORING
};


enum eSHOP_CATEGORY_SEPERATE
{
	SHOP_ITEM = 0,
	SHOP_PACKAGE,
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	SHOP_MILEAGE,
#endif
	SHOP_SEPERATE_MAX,
};

// Specification
//  MONEY_CASH_TYPE = { cash, pp cash }
enum eSHOP_MONEY_TYPE
{
	MONEY_CASH_TYPE=1,  // shop은 ppcash를 아이템별 구분 처리를 수행하지 않는다.
	MONEY_HIME_TYPE,
	MONEY_MILEAGE_TYPE, // useful for __CN_0_20070507_SHOP_MILEAGE
};

enum eSHOP_SERIALIZE
{
	eSHOP_SERIALIZE_LOAD,
	eSHOP_SERIALIZE_SAVE,

	eSHOP_SERIALIZE_CLIENT_LOAD,
	eSHOP_SERIALIZE_CLIENT_SAVE,

	// 추가된것
	eSHOP_SERIALIZE_ITEM_LIST_LOAD,
	eSHOP_SERIALIZE_ITEM_LIST_SAVE,

	eSHOP_SERIALIZE_ITEM_LOAD,
	eSHOP_SERIALIZE_ITEM_SAVE,

	eSHOP_SERIALIZE_PACKAGE_LOAD,
	eSHOP_SERIALIZE_PACKAGE_SAVE,
};

namespace RC
{
enum eSHOP_RESULT
{
	RC_SHOP_SUCCESS,
	RC_SHOP_FAILED,
	RC_SHOP_PENDING,
	RC_SHOP_CANNOTBUYITEM,							//< 판매하지 않는 아이템입니다.
	RC_SHOP_CANNOTBUYITEMPRICE,						//< 판매하지 않는 아이템 가격입니다.
	RC_SHOP_USERISNOTFOUND,							//< 유저를 찾을 수 없습니다.
	RC_SHOP_SHOPSERVERDISCONNECTED,					//< 샵서버가 연결되어있지않습니다.(잠시뒤 이용해주세요)
	RC_SHOP_NOTENOUGHCASH,							//< 캐쉬가 부족합니다.
	RC_SHOP_NOTENOUGHMONEY,							//< 하임이 부족합니다.
	RC_SHOP_DB_ERROR,								//< 샵 DB에러
	RC_SHOP_NOTENOUGHINVENTORYSPACE,				//< 인벤토리 공간이 부족합니다.
	RC_SHOP_CRITICAL_ERROR,							//< 치명적인 오류입니다. 운영팀에 문의하세요!
	RC_SHOP_INVALID_CHAR_NAME_LEN,					//< 캐릭터 이름 길이를 지원하지 않습니다.
	RC_SHOP_STANDBY_SERVICE,						//< 샵 서비스 준비중입니다. 잠시뒤에 이용하세요.(정보쿼리중)
	RC_SHOP_ALREADY_BUYING_TRANSACTION,				//< 이미 구매 트랜잭션이 진행중입니다. 잠시뒤 이용하세요.
	RC_SHOP_SERVICE_STOP,							//< 서비스 중지중입니다. 잠시뒤에 이용하세요.
	RC_SHOP_CANNOTBUYBYLIMITCOUNT,					//< 구매 개수 제한을 넘어서서 구입할 수 없습니다.
	RC_SHOP_NOTEXPIREDCOOLTIME,						//< 구매 쿨타임이 지나지 않아서 구입할 수 없습니다.
	RC_SHOP_CANNOT_USE_SHOP,						//< 아이템 샵을 이용할 수 없습니다.

    //2007/1/17 오후 4:31:52
	RC_SHOP_OVER_BUY_COUNT,							//< 한번에 구매할 수 있는 개수를 넘어섰습니다.
	RC_SHOP_INVALID_GIFT_GUID,						//< 유효하지 않는 선물 일련번호입니다.
	RC_SHOP_CANNOT_OBTAIN_GIFT,						//< 선물을 받을 수 없습니다.
	RC_SHOP_CANNOT_REJECT_GIFT,						//< 선물을 거절할 수 없습니다
	RC_SHOP_CANNOT_DISCARD_GIFT,					//< 선물기록을 삭제할 수 없습니다.
	RC_SHOP_CANNOT_FIND_CHARNAME,					//< 캐릭터 이름에 해당하는 캐릭터를 찾을 수 없습니다.
	RC_SHOP_CANNOT_SEND_GIFT_MYSELF,				//< 자기자신에게 선물을 보낼 수 없습니다.
	RC_SHOP_CANNOT_REJECT_GIFT_MYSELF,				//< 자기자신에게 선물을 반송할 수없습니다.
#ifdef __NA_00387_20070605_SHOP_MILEAGE2			
	RC_SHOP_NOTENOUGHMILEAGE,						//< 마일리지가 부족합니다.
#endif

	RC_SHOP_NOT_EXIST_EXTEND_ITEM,					//< 기간을 연장할 아이템이 존재하지 않는다.
	RC_SHOP_CANNOT_BUY_PC_ROOM_ITEM_GENERAL_USER,	//일반 유저는 PC방 아이템 구입 불가
	RC_SHOP_CANNOT_PRESENT_ITEM,					//< 해당 아이템은 선물할 수 없다. __NA001087_080618_APPEND_CASHITEM_PRESENT_PROTECTION_FUNTION__

    //_US_SHOP_GIFT_ERRORCONTROL_ADD 북미 선물하기 관련 결과 값
    RC_SHOP_INVALID_BILLING_PARAM,                  ///>  빌링 파라메터 에러
    RC_SHOP_BILLING_NETWORK_ERROR,                  ///>  빌링 서버 네트워크 장애
    RC_SHOP_BILLING_SERVER_LOGIC_ERROR,             ///>  빌링 서버 로직 장애
    RC_SHOP_NOT_EXIST_BILLING_ITEM_CODE,            ///>  빌링 서버에 등록되지 않은 상품코드
    RC_SHOP_NOT_EXIST_BILLING_TRADE_CODE,           ///>  빌링 서버에 등록되지 않은 거래구분코드
    RC_SHOP_EXCEED_LIMIT_CHARGE,                    ///>  충전한도초과
    RC_SHOP_ABNORMALITY_CHARGE_TRANSACTION,         ///>  비정상적인 충전 트랜잭션 
    RC_SHOP_GIFT_ITEM_BLACK_LIST,                   ///>  선물 자격 미달 - black listed
    RC_SHOP_GIFT_ITEM_BANNED_USER,                  ///>  선물 자격 미달 - banned user
    RC_SHOP_GIFT_ITEM_NOT_EXIST_HISTORY,            ///>  선물 자격 미달 - 두달내 두번이상 충전이력이 없을때
    RC_SHOP_EXCEED_LIMIT_GIFT_ITEM,                 ///>  선물 자격 미달 - 선물하기 한도초과
    RC_SHOP_BILLING_SYSTEM_ERROR,                   ///>  빌링 시스템 에러
    RC_SHOP_BILLING_ETC_ERROR,                      ///>  빌링 기타에러
    RC_SHOP_USERLEVEL_LOW,                          ///>  유저 레벨 이 낮아 선물하기 불가능

    //북미 지원 C/S 에러 추가
    RC_SHOP_CANNOT_SEND_GIFT_MYACCOUNT,             ///>  자기 계정으로 선물을 보낼 수 없습니다.
    RC_SHOP_CAN_BUY_PP_CASH_ONLY,       ///> 이 상품은 선불 결재만으로 구매가 가능합니다. (for Global)
    RC_SHOP_CAN_PRESENT_PP_CASH_ONLY,   ///> 선물은 선불 결재로만 가능합니다. (for Global) _NA000424_GLOBAL_CASH_ITEM_RENEWAL_

    RC_SHOP_NOT_FRIEND_ME,                          //선물 받는 사람이 내 친구 목록에 없다.                 
    RC_SHOP_NOT_FRINED_YOUR,                        //선불 받는 사람의 친구목록에 내가 없다.
};

};

//==================================================================================================

namespace nsShop {
};







#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL




