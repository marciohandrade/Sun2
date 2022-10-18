#pragma once

#include <BitsetOperation.h>

class Player;

struct MSG_DG_WAREHOUSE_PWD_SETTING_ACK;

//////////////////////////////////////////////////////////////////////////
//
struct sPACKET_PROTECT_BASE_INFO
{
	enum
	{	// Password 관련 작업중 필요한 세부 모드
		CHECK_NONE			= 4,	// 
		CHECK_CMP_PWD		= 1,	// PWD 검사중
		CHECK_CMP_SSN		= 2,	// SSN 검사중
		APPLY_TRANSACTION	= 3,	// DB에의 반영, 응답대기중
		CHECK_END			= 4,	// 트랜젝션 상태 종료
	};
	Player*							pPlayer;
	WAREINVEN_FILTER_BITFLAGS		CONTROL_FIELD;
	BYTE							byPwdConfigureMode;
	sSSN_ENCODED_STREAM				SSN_ENCODED_STREAM;
};

struct sPASSWORD_BLOCK
{
	sSSN_STREAM		PWDnSSN;

	inline VOID				Init() { memset( PWDnSSN.GetBUFFER(), 0, PWDnSSN.GetBUFFER_LENGTH() ); }
	inline BOOL				IsEmpty() { return (BOOL)(PWDnSSN.GetBUFFER()[0] == '\0'); }

	inline sSSN_STREAM&		Get()		{ return PWDnSSN; }
	inline VOID				Set( CHAR* pStrSrc )
	{
		PWDnSSN.CopyFrom( pStrSrc );
		PWDnSSN.MakeSafeString();
	}

	// 비번이 할당된 상태에서 비번으로서 허용 가능한지 여부 타전
	static BOOL		IsAcceptableFormat( CHAR* pwd );
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
// 창고/인벤토리 잠금 기능 관련
//---------------------------------------------------------------------------
class ProtectorWarehouse
{
private:	sPACKET_PROTECT_BASE_INFO&		m_rINFO;
			sPASSWORD_BLOCK					m_Password;

#pragma warning(push)
#pragma warning(disable : 4201)
			struct
			{	// Addin-INFO
				CODETYPE					NPCCode;	// 상점,인벤 관련 비번 설정시 임시 저장소
			};
#pragma warning(pop)
public:
			// 비번 공간 초기화는 DB로부터 값을 받아올 때 사용하기로 하자.
			ProtectorWarehouse( sPACKET_PROTECT_BASE_INFO& rINFO ) : m_rINFO(rINFO) { }

			// 유저 정보를 가져오는 시점에 비번 유무 확인 후 이 루틴을 수행한다.
			VOID							Block( BOOL bIsBlock, BOOL bDBUpdate = TRUE );
			// 비번 미설정시 나 비번 입력 받고 인증되었을 때, 사용할 수 있도록 설정
			inline VOID						UnBlock();
			// 창고 초기화 되었는지? (최소 한번은 열었는지?)
			inline BOOL						IsInitialized();
			// 창고 초기화 되었다.
			inline VOID						Initialized();
			// 비번 통과 했는지 여부
			inline BOOL						IsUsable();
			// 비번 있는지
			inline BOOL						IsEmptyPassword();
			// 비번 비교
			inline BOOL						IsEqualPassword( CHAR pszPwd[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] );
			// 비번 설정
			inline BOOL						SetPasswordFromDB( CHAR pszPwdinDB[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] );
			//
			inline CHAR*					GetPassword() { return PASSWORD().Get().GetBUFFER(); }

			inline BYTE&					PasswordTransactionMode() { return m_rINFO.byPwdConfigureMode; }

			//
			//
			BOOL							TEST_Transaction_PWD_CONFIG( BOOL bIsEnable );
			// 비번 확인중일 경우, 비번 설정 중일 경우
			BOOL							TESTnSET_Transaction_PWD_CONFIG( BOOL bIsEnable );
			//
			inline DWORD					GetControlFieldStream();
			inline VOID						SetControlFieldStream( DWORD dwFlagStream );

private:
			inline Player&						PLAYER() { return *m_rINFO.pPlayer; }
			inline WAREINVEN_FILTER_BITFLAGS&	CONTROL_FIELD() { return m_rINFO.CONTROL_FIELD; }
			inline sPASSWORD_BLOCK&				PASSWORD() { return m_Password; }
    __DISABLE_COPY(ProtectorWarehouse);
};

class ProtectorInventory
{
private:	sPACKET_PROTECT_BASE_INFO&		m_rINFO;
			sPASSWORD_BLOCK					m_Password;

public:		ProtectorInventory( sPACKET_PROTECT_BASE_INFO& rINFO ) : m_rINFO(rINFO) { }

			// 유저 정보를 가져오는 시점에 비번 유무 확인 후 이 루틴을 수행한다.
			VOID							Block( BOOL bIsBlock, BOOL bDBUpdate = TRUE );
			// 비번 통과 했는지 여부
			inline BOOL						IsUsable() const;
			// 비번 있는지
			inline BOOL						IsEmptyPassword();
			// 비번 비교 (유저가 입력한 암호로 현 DB에 설정되어 있을 암호와 비교)
			inline BOOL						IsEqualPassword( CHAR pszPwd[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH+1] );
			// 비번 설정 (DB에서 값을 읽어들일 때, 설정)
			inline BOOL						SetPasswordFromDB( CHAR pszPwdinDB[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] );
			//
			inline CHAR*					GetPassword() { return PASSWORD().Get().GetBUFFER(); }

			inline BYTE&					PasswordTransactionMode() { return m_rINFO.byPwdConfigureMode; }

			//
			//
			BOOL							TEST_Transaction_PWD_CONFIG( BOOL bIsEnable );
			// 비번 확인중일 경우, 비번 설정 중일 경우
			BOOL							TESTnSET_Transaction_PWD_CONFIG( BOOL bIsEnable );
			//
			inline DWORD					GetControlFieldStream();
			inline VOID						SetControlFieldStream( DWORD dwFlagStream );

private:	inline Player&						PLAYER() { return *m_rINFO.pPlayer; }
			inline WAREINVEN_FILTER_BITFLAGS&	CONTROL_FIELD() const { return m_rINFO.CONTROL_FIELD; }

			inline sPASSWORD_BLOCK&				PASSWORD() { return m_Password; }
    __DISABLE_COPY(ProtectorInventory);
};

//
//////////////////////////////////////////////////////////////////////////
//

class WarehouseInventoryProtector
{
private:	sPACKET_PROTECT_BASE_INFO		m_BASE_INFO;
			ProtectorWarehouse				m_WAREHOUSE;
			ProtectorInventory				m_INVENTORY;
public:		WarehouseInventoryProtector() : m_WAREHOUSE( m_BASE_INFO ), m_INVENTORY( m_BASE_INFO ) {}
			~WarehouseInventoryProtector() {}

			inline VOID						Init( Player* pPlayer )
			{
				m_BASE_INFO.pPlayer = pPlayer;
				m_BASE_INFO.CONTROL_FIELD.ClearAll();
				m_BASE_INFO.byPwdConfigureMode = sPACKET_PROTECT_BASE_INFO::CHECK_NONE;
				CONTROL_FIELD().UnSet( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_USABLE );
			}

public:		ProtectorWarehouse&				WAREHOUSE() { return m_WAREHOUSE; }
			ProtectorInventory&				INVENTORY() { return m_INVENTORY; }

public:		BOOL							IsEqualSSN( BYTE psSSN[MAX_SSN_STREAM_LENGTH] );

public:		inline Player&						USER() { return *m_BASE_INFO.pPlayer; }
			inline WAREINVEN_FILTER_BITFLAGS&	CONTROL_FIELD() { return m_BASE_INFO.CONTROL_FIELD; }
			inline sSSN_ENCODED_STREAM&			SSN_ENCODED_STREAM() { return m_BASE_INFO.SSN_ENCODED_STREAM; }

// <PACKET HANDLER>
public:
			// <창고, 인벤> 비번 변경에 대한 결과
			BOOL	OnRecvDG_WAREHOUSE_PWD_SETTING_ACK( MSG_DG_WAREHOUSE_PWD_SETTING_ACK* pMsg, WORD wSize );
			// <창고>
			BOOL	OnRecvCG_WAREHOUSE_PWD_LOCKING_SYN( MSG_CG_WAREHOUSE_PWD_LOCKING_SYN* pMsg, WORD wSize );
			BOOL	OnRecvCG_WAREHOUSE_PWD_SETTING_SYN( MSG_CG_WAREHOUSE_PWD_SETTING_SYN* pMsg, WORD wSize );
			BOOL	OnRecvCG_WAREHOUSE_PWD_CHECK_SYN( MSG_CG_WAREHOUSE_PWD_CHECK_SYN* pMsg, WORD wSize );
			// <인벤토리>
			BOOL	OnRecvCG_ITEM_INVENTORY_PWD_LOCKING_SYN( MSG_CG_ITEM_INVENTORY_PWD_LOCKING_SYN* pMsg, WORD wSize );
			BOOL	OnRecvCG_ITEM_INVENTORY_PWD_SETTING_SYN( MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN* pMsg, WORD wSize );
			BOOL	OnRecvCG_ITEM_INVENTORY_PWD_CHECK_SYN( MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN* pMsg, WORD wSize );
    __DISABLE_COPY(WarehouseInventoryProtector);
};
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫

// 창고 초기화 되었는지? (최소 한번은 열었는지?)
inline BOOL		ProtectorWarehouse::IsInitialized()
{
	return CONTROL_FIELD().IsSet( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_INITIALIZE );
}

inline VOID		ProtectorWarehouse::Initialized()
{
	return CONTROL_FIELD().Set( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_INITIALIZE );
}

// 비번 통과 했는지 여부
inline BOOL		ProtectorWarehouse::IsUsable()
{
	return CONTROL_FIELD().IsSet( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_USABLE );
}

// 비번 있는지
inline BOOL		ProtectorWarehouse::IsEmptyPassword()
{
	return PASSWORD().IsEmpty();
}

// 비번 비교
inline BOOL		ProtectorWarehouse::IsEqualPassword( CHAR pszPwd[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] )
{
	if( 0 == strncmp( PASSWORD().Get().GetBUFFER(), pszPwd, MAX_WAREHOUSE_INVENTORY_PWD_LENGTH ) )
		return TRUE;
	return FALSE;
}

// 비번 설정
inline BOOL		ProtectorWarehouse::SetPasswordFromDB( CHAR pszPwdinDB[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] )
{
	PASSWORD().Set( pszPwdinDB );
	return TRUE;
}

inline DWORD	ProtectorWarehouse::GetControlFieldStream()
{
	const DWORD dwAcceptField = eWARE_INVEN_FILTER_FLAG_INFO::MASK_WAREHOUSE_FLAGS
		& (~(1<<eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_PWD_TRANSACTION));

	DWORD dwMaskedFields = dwAcceptField & (*(DWORD*)&m_rINFO.CONTROL_FIELD);

	//const DWORD dwMASK_FIELD = (1<<eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_INITIALIZE)
	//	| (1<<eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_USABLE);
	//DWORD dwMaskedFields = dwMASK_FIELD & (*(DWORD*)&m_rINFO.CONTROL_FIELD);
	return dwMaskedFields;
}

inline VOID		ProtectorWarehouse::SetControlFieldStream( DWORD dwFlagStream )
{
	const DWORD dwMASK_ACCEPT =
		(1<<eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_INITIALIZE)
        | (1<<eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_USABLE);

	dwFlagStream = dwMASK_ACCEPT & dwFlagStream;

	DWORD& rFIELD = (*(DWORD*)&m_rINFO.CONTROL_FIELD);
	rFIELD = rFIELD & dwFlagStream;

	//if( (dwMASK_INIT|dwMASK_USABLE) & (dwFlagStream) )
	//{
	//	if( dwMASK_INIT & dwFlagStream )
	//		CONTROL_FIELD().Set( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_INITIALIZE );
	//	else
	//		CONTROL_FIELD().UnSet( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_INITIALIZE );
	//	if( dwMASK_USABLE & dwFlagStream )
	//		CONTROL_FIELD().Set( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_USABLE );
	//	else
	//		CONTROL_FIELD().UnSet( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_USABLE );
	//}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// 비번 미설정시 나 비번 입력 받고 인증되었을 때, 사용할 수 있도록 설정
//inline VOID		ProtectorInventory::UnBlock()

// 비번 통과 했는지 여부
inline BOOL		ProtectorInventory::IsUsable() const
{
	return CONTROL_FIELD().IsSet( eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_USABLE );
}

// 비번 있는지
inline BOOL		ProtectorInventory::IsEmptyPassword()
{
	return PASSWORD().IsEmpty();
}

// 비번 비교 (유저가 입력한 암호로 현 DB에 설정되어 있을 암호와 비교)
inline BOOL		ProtectorInventory::IsEqualPassword( CHAR pszPwd[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH+1] )
{
	if( 0 == strncmp( PASSWORD().Get().GetBUFFER(), pszPwd, MAX_WAREHOUSE_INVENTORY_PWD_LENGTH ) )
		return TRUE;
	return FALSE;
}

// 비번 설정 (DB에서 값을 읽어들일 때, 설정)
inline BOOL		ProtectorInventory::SetPasswordFromDB( CHAR pszPwdinDB[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] )
{
	PASSWORD().Set( pszPwdinDB );
	return TRUE;
}

inline DWORD	ProtectorInventory::GetControlFieldStream()
{
	DWORD dwMaskedFields =
		(1<<eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_USABLE) & (*(DWORD*)&m_rINFO.CONTROL_FIELD);
	return dwMaskedFields;
}

inline VOID		ProtectorInventory::SetControlFieldStream( DWORD dwFlagStream )
{
	if( (1<<eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_USABLE) & (dwFlagStream) )
		CONTROL_FIELD().Set( eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_USABLE );
	else
		CONTROL_FIELD().UnSet( eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_USABLE );
}




