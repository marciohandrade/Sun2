#pragma once

#include <PacketStruct.h>
#include <PacketStruct_CG.h>


namespace nsPacket
{
	template< BYTE category, BYTE category_pack >
	struct CATEGORY_LINK_PAIR
	{
		static const BYTE			value		= category;
		static const BYTE			value_pack	= category_pack;
	};

#ifdef _NA000000_CW_PROTOCOL_UNIFICATION
    #define DECL_CATEGORY_LIST_CW() \
        DECL_CATEGORY_NODE(CW_CONNECTION   , CW_CONNECTION_PACK   ) \
        DECL_CATEGORY_NODE(CW_CHAT         , CW_CHAT_PACK         ) \
        DECL_CATEGORY_NODE(CW_GM           , CW_GM_PACK           ) \
        DECL_CATEGORY_NODE(CW_VIEWPORT     , CW_VIEWPORT_PACK     ) \
        DECL_CATEGORY_NODE(CW_ARMORKIT     , CW_ARMORKIT_PACK     ) \
        DECL_CATEGORY_NODE(CW_FRIEND       , CW_FRIEND_PACK       ) \
        DECL_CATEGORY_NODE(CW_GUILD        , CW_GUILD_PACK        ) \
        DECL_CATEGORY_NODE(CW_CRYPTOGRAPHY , CW_CRYPTOGRAPHY_PACK ) \
        DECL_CATEGORY_NODE(CW_COMMUNITY    , CW_COMMUNITY_PACK    ) \
        DECL_CATEGORY_NODE(CW_REVENGE      , CW_REVENGE_PACK      ) \
        DECL_CATEGORY_NODE(CW_MEMO         , CW_MEMO_PACK         ) \
        DECL_CATEGORY_NODE(CW_GROUP        , CW_GROUP_PACK        ) \
        DECL_CATEGORY_NODE(CW_LOTTO        , CW_LOTTO_PACK        ) \
        DECL_CATEGORY_NODE(CW_PARTY        , CW_PARTY_PACK        )
#else
    #define DECL_CATEGORY_LIST_CW()
#endif

//#define DECL_CATEGORY_NODE( cate, cate_pack )
#define DECL_CATEGORY_LIST()										\
	DECL_CATEGORY_NODE( CG_CONNECTION	, CG_CONNECTION_PACK	)	\
	DECL_CATEGORY_NODE( CG_CHARINFO		, CG_CHARINFO_PACK		)	\
	DECL_CATEGORY_NODE( CG_MAP			, CG_MAP_PACK			)	\
	DECL_CATEGORY_NODE( CG_SYNC			, CG_SYNC_PACK			)	\
	DECL_CATEGORY_NODE( CG_BATTLE		, CG_BATTLE_PACK		)	\
	DECL_CATEGORY_NODE( CG_STYLE		, CG_STYLE_PACK			)	\
	DECL_CATEGORY_NODE( CG_SKILL		, CG_SKILL_PACK			)	\
	DECL_CATEGORY_NODE( CG_CONVERSATION	, CG_CONVERSATION_PACK	)	\
	DECL_CATEGORY_NODE( CG_ZONE			, CG_ZONE_PACK			)	\
	DECL_CATEGORY_NODE( CG_ITEM			, CG_ITEM_PACK			)	\
	DECL_CATEGORY_NODE( CG_TRADE		, CG_TRADE_PACK			)	\
	DECL_CATEGORY_NODE( CG_PARTY		, CG_PARTY_PACK			)	\
	DECL_CATEGORY_NODE( CG_TRIGGER		, CG_TRIGGER_PACK		)	\
	DECL_CATEGORY_NODE( CG_GM			, CG_GM_PACK			)	\
	DECL_CATEGORY_NODE( CG_ETC			, CG_ETC_PACK			)	\
	DECL_CATEGORY_NODE( CG_STATUS		, CG_STATUS_PACK		)	\
	DECL_CATEGORY_NODE( CG_VENDOR		, CG_VENDOR_PACK		)	\
	DECL_CATEGORY_NODE( CG_WAREHOUSE	, CG_WAREHOUSE_PACK		)	\
	DECL_CATEGORY_NODE( CG_PVP			, CG_PVP_PACK			)	\
	DECL_CATEGORY_NODE( CG_SUMMON		, CG_SUMMON_PACK		)	\
	DECL_CATEGORY_NODE( CG_GUILD		, CG_GUILD_PACK			)	\
	DECL_CATEGORY_NODE( CG_EVENT		, CG_EVENT_PACK			)	\
	DECL_CATEGORY_NODE( CG_QUEST		, CG_QUEST_PACK			)	\
	DECL_CATEGORY_NODE( CG_CRYPTOGRAPHY	, CG_CRYPTOGRAPHY_PACK	)	\
	DECL_CATEGORY_NODE( CG_ARMORKIT		, CG_ARMORKIT_PACK		)	\
	DECL_CATEGORY_NODE( CG_CHUNTING		, CG_CHUNTING_PACK		)	\
	DECL_CATEGORY_NODE( CG_AREACONQUEST	, CG_AREACONQUEST_PACK	)	\
	DECL_CATEGORY_NODE( CG_MISSION		, CG_MISSION_PACK		)	\
	DECL_CATEGORY_NODE( CG_WAYPOINT		, CG_WAYPOINT_PACK		)	\
	DECL_CATEGORY_NODE( CG_CHAO			, CG_CHAO_PACK			)	\
	DECL_CATEGORY_NODE( CG_SHOP			, CG_SHOP_PACK			)	\
	DECL_CATEGORY_NODE( CG_ZONE_EX		, CG_ZONE_EX_PACK		)   \
    DECL_CATEGORY_NODE( CG_COLLECT		, CG_COLLECT_PACK		)   \
    DECL_CATEGORY_LIST_CW()
	//DECL_CATEGORY_NODE( CG_MAX			, CG_MAX_PACK			)

#define DECL_CATEGORY_NODE( cate, cate_pack )	\
	typedef CATEGORY_LINK_PAIR<cate, cate_pack>		PAIR_##cate;

	DECL_CATEGORY_LIST();	//type definitions

#ifdef _NA000000_CW_PROTOCOL_UNIFICATION
    BOOST_STATIC_ASSERT(CATEGORY_GENERATOR_065 == CG_MAX);
    BOOST_STATIC_ASSERT(CATEGORY_GENERATOR_165 == CG_MAX_PACK);
#else
    BOOST_STATIC_ASSERT(CATEGORY_GENERATOR_036 == CG_MAX);
    BOOST_STATIC_ASSERT(CATEGORY_GENERATOR_136 == CG_MAX_PACK);
#endif
    //==================================================================================================
	//
	struct CategoryConverter
	{
		//////////////////////////////////////////////////////////////////////////
		//	
		static WORD			GetCategoryValue( const BYTE any_category )
		{
			switch( any_category )
			{
		#undef DECL_CATEGORY_NODE
		#define DECL_CATEGORY_NODE( cate, cate_pack )	\
			case PAIR_##cate::value:			return PAIR_##cate::value;

				DECL_CATEGORY_LIST();

		#undef DECL_CATEGORY_NODE
		#define DECL_CATEGORY_NODE( cate, cate_pack )	\
			case PAIR_##cate::value_pack:		return PAIR_##cate::value;

				DECL_CATEGORY_LIST();
			}

			return (WORD)-1;
		}

		//////////////////////////////////////////////////////////////////////////
		//	
		static WORD			GetCategoryPackValue( const BYTE any_category )
		{
			switch( any_category )
			{
			#undef DECL_CATEGORY_NODE
			#define DECL_CATEGORY_NODE( cate, cate_pack )	\
				case PAIR_##cate::value:		return PAIR_##cate::value_pack;

				DECL_CATEGORY_LIST();

			#undef DECL_CATEGORY_NODE
			#define DECL_CATEGORY_NODE( cate, cate_pack )	\
				case PAIR_##cate::value_pack:	return PAIR_##cate::value_pack;

				DECL_CATEGORY_LIST();
			}

			return (WORD)-1;
		}

		//////////////////////////////////////////////////////////////////////////
		//	
		static BOOLEAN		IsCategoryPacked( const BYTE check_category )
		{
			switch( check_category )
			{
			#undef DECL_CATEGORY_NODE
			#define DECL_CATEGORY_NODE( cate, cate_pack )	\
				case PAIR_##cate::value_pack:	return TRUE;

				DECL_CATEGORY_LIST();
			}

			return FALSE;
		}

		//////////////////////////////////////////////////////////////////////////
		//	
		static BOOLEAN		SetPackInfo( MSG_BASE* const pMsg )
		{
			const BYTE category = pMsg->m_byCategory;
        #ifdef _DEBUG
			const BOOLEAN isPacketPacked = IsCategoryPacked( category );
			if( isPacketPacked )
			{
				__debugbreak();
				return FALSE;
			}
        #endif

			switch( category )
			{
			#undef DECL_CATEGORY_NODE
			#define DECL_CATEGORY_NODE( cate, cate_pack )	\
				case PAIR_##cate::value:	pMsg->m_byCategory = PAIR_##cate::value_pack;	return TRUE;

				DECL_CATEGORY_LIST();
			}

			return FALSE;
		}

		//////////////////////////////////////////////////////////////////////////
		//	
		static BOOLEAN		SetUnPackInfo( MSG_BASE* const pMsg )
		{
			const BYTE category = pMsg->m_byCategory;
        #ifdef _DEBUG
			const BOOLEAN isPacketPacked = IsCategoryPacked( category );
			if( !isPacketPacked )
			{
				__debugbreak();
				return FALSE;
			}
        #endif
			switch( category )
			{
			#undef DECL_CATEGORY_NODE
			#define DECL_CATEGORY_NODE( cate, cate_pack )	\
				case PAIR_##cate::value_pack:	pMsg->m_byCategory = PAIR_##cate::value;	return TRUE;

				DECL_CATEGORY_LIST();
			}

			return FALSE;
		}
	};
};


