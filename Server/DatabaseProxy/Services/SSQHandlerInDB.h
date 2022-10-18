#pragma once

#include <Struct.h>
#include <SSQ/SSQHandlerBase.h>
#include "QueryObjects/GameServerQuery.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class ISSQHandlerInDB;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SSQHandlerInDB>
//
class SSQHandlerInDB : public SSQHandlerBase< SSQHandlerInDB >
{
	friend class ISSQHandlerInDB;

public:		SSQHandlerInDB();
			~SSQHandlerInDB();

public:		VOID		SetSerializedInfo( EventStatusQuery::sQUERY& IN rSource );
			//BOOLEAN		Rewarded( const eEVENT_MODE_MULTIUSE mode );	// 보상,복구 처리 완료상태로 전환
			//BOOLEAN		GetRewardInfo( SSQ_RESERVED_REWARD_INFO& OUT rInfo ) const;
			//BOOLEAN		SetRewardInfo( const SSQ_RESERVED_REWARD_INFO& IN rInfo );	// Game,Agent등에서 변경된 정보를 설정할 것이다.
			BOOLEAN		MakeQuery( STRING_SAFER_QUERY256& OUT rQueryBuff, const DWORD char_guid );
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
class ISSQHandlerInDB
{
public:		ISSQHandlerInDB()
				: m_pHandler(0)
			{}

			~ISSQHandlerInDB() 
            {
                Release();
            }
public:
	inline static BOOLEAN		IsSSQEventID( const DWORD eventID ) { return SSQHandlerCommon::IsSSQEventID( eventID ); }
	inline	BOOLEAN				IsValidState() const { return !!m_pHandler ; }
	inline	SSQHandlerInDB*		operator->() { return m_pHandler; }
			VOID				Init();
			VOID				Release();

private:	SSQHandlerInDB*		m_pHandler;
};

