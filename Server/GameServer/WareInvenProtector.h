#pragma once

#include <BitsetOperation.h>

class Player;

struct MSG_DG_WAREHOUSE_PWD_SETTING_ACK;

//////////////////////////////////////////////////////////////////////////
//
struct sPACKET_PROTECT_BASE_INFO
{
	enum
	{	// Password ���� �۾��� �ʿ��� ���� ���
		CHECK_NONE			= 4,	// 
		CHECK_CMP_PWD		= 1,	// PWD �˻���
		CHECK_CMP_SSN		= 2,	// SSN �˻���
		APPLY_TRANSACTION	= 3,	// DB���� �ݿ�, ��������
		CHECK_END			= 4,	// Ʈ������ ���� ����
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

	// ����� �Ҵ�� ���¿��� ������μ� ��� �������� ���� Ÿ��
	static BOOL		IsAcceptableFormat( CHAR* pwd );
};

//������������������������������������������������������������������������������
// â��/�κ��丮 ��� ��� ����
//---------------------------------------------------------------------------
class ProtectorWarehouse
{
private:	sPACKET_PROTECT_BASE_INFO&		m_rINFO;
			sPASSWORD_BLOCK					m_Password;

#pragma warning(push)
#pragma warning(disable : 4201)
			struct
			{	// Addin-INFO
				CODETYPE					NPCCode;	// ����,�κ� ���� ��� ������ �ӽ� �����
			};
#pragma warning(pop)
public:
			// ��� ���� �ʱ�ȭ�� DB�κ��� ���� �޾ƿ� �� ����ϱ�� ����.
			ProtectorWarehouse( sPACKET_PROTECT_BASE_INFO& rINFO ) : m_rINFO(rINFO) { }

			// ���� ������ �������� ������ ��� ���� Ȯ�� �� �� ��ƾ�� �����Ѵ�.
			VOID							Block( BOOL bIsBlock, BOOL bDBUpdate = TRUE );
			// ��� �̼����� �� ��� �Է� �ް� �����Ǿ��� ��, ����� �� �ֵ��� ����
			inline VOID						UnBlock();
			// â�� �ʱ�ȭ �Ǿ�����? (�ּ� �ѹ��� ��������?)
			inline BOOL						IsInitialized();
			// â�� �ʱ�ȭ �Ǿ���.
			inline VOID						Initialized();
			// ��� ��� �ߴ��� ����
			inline BOOL						IsUsable();
			// ��� �ִ���
			inline BOOL						IsEmptyPassword();
			// ��� ��
			inline BOOL						IsEqualPassword( CHAR pszPwd[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] );
			// ��� ����
			inline BOOL						SetPasswordFromDB( CHAR pszPwdinDB[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] );
			//
			inline CHAR*					GetPassword() { return PASSWORD().Get().GetBUFFER(); }

			inline BYTE&					PasswordTransactionMode() { return m_rINFO.byPwdConfigureMode; }

			//
			//
			BOOL							TEST_Transaction_PWD_CONFIG( BOOL bIsEnable );
			// ��� Ȯ������ ���, ��� ���� ���� ���
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

			// ���� ������ �������� ������ ��� ���� Ȯ�� �� �� ��ƾ�� �����Ѵ�.
			VOID							Block( BOOL bIsBlock, BOOL bDBUpdate = TRUE );
			// ��� ��� �ߴ��� ����
			inline BOOL						IsUsable() const;
			// ��� �ִ���
			inline BOOL						IsEmptyPassword();
			// ��� �� (������ �Է��� ��ȣ�� �� DB�� �����Ǿ� ���� ��ȣ�� ��)
			inline BOOL						IsEqualPassword( CHAR pszPwd[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH+1] );
			// ��� ���� (DB���� ���� �о���� ��, ����)
			inline BOOL						SetPasswordFromDB( CHAR pszPwdinDB[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] );
			//
			inline CHAR*					GetPassword() { return PASSWORD().Get().GetBUFFER(); }

			inline BYTE&					PasswordTransactionMode() { return m_rINFO.byPwdConfigureMode; }

			//
			//
			BOOL							TEST_Transaction_PWD_CONFIG( BOOL bIsEnable );
			// ��� Ȯ������ ���, ��� ���� ���� ���
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
			// <â��, �κ�> ��� ���濡 ���� ���
			BOOL	OnRecvDG_WAREHOUSE_PWD_SETTING_ACK( MSG_DG_WAREHOUSE_PWD_SETTING_ACK* pMsg, WORD wSize );
			// <â��>
			BOOL	OnRecvCG_WAREHOUSE_PWD_LOCKING_SYN( MSG_CG_WAREHOUSE_PWD_LOCKING_SYN* pMsg, WORD wSize );
			BOOL	OnRecvCG_WAREHOUSE_PWD_SETTING_SYN( MSG_CG_WAREHOUSE_PWD_SETTING_SYN* pMsg, WORD wSize );
			BOOL	OnRecvCG_WAREHOUSE_PWD_CHECK_SYN( MSG_CG_WAREHOUSE_PWD_CHECK_SYN* pMsg, WORD wSize );
			// <�κ��丮>
			BOOL	OnRecvCG_ITEM_INVENTORY_PWD_LOCKING_SYN( MSG_CG_ITEM_INVENTORY_PWD_LOCKING_SYN* pMsg, WORD wSize );
			BOOL	OnRecvCG_ITEM_INVENTORY_PWD_SETTING_SYN( MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN* pMsg, WORD wSize );
			BOOL	OnRecvCG_ITEM_INVENTORY_PWD_CHECK_SYN( MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN* pMsg, WORD wSize );
    __DISABLE_COPY(WarehouseInventoryProtector);
};
//
//������������������������������������������������������������������������������

// â�� �ʱ�ȭ �Ǿ�����? (�ּ� �ѹ��� ��������?)
inline BOOL		ProtectorWarehouse::IsInitialized()
{
	return CONTROL_FIELD().IsSet( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_INITIALIZE );
}

inline VOID		ProtectorWarehouse::Initialized()
{
	return CONTROL_FIELD().Set( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_INITIALIZE );
}

// ��� ��� �ߴ��� ����
inline BOOL		ProtectorWarehouse::IsUsable()
{
	return CONTROL_FIELD().IsSet( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_USABLE );
}

// ��� �ִ���
inline BOOL		ProtectorWarehouse::IsEmptyPassword()
{
	return PASSWORD().IsEmpty();
}

// ��� ��
inline BOOL		ProtectorWarehouse::IsEqualPassword( CHAR pszPwd[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] )
{
	if( 0 == strncmp( PASSWORD().Get().GetBUFFER(), pszPwd, MAX_WAREHOUSE_INVENTORY_PWD_LENGTH ) )
		return TRUE;
	return FALSE;
}

// ��� ����
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

// ��� �̼����� �� ��� �Է� �ް� �����Ǿ��� ��, ����� �� �ֵ��� ����
//inline VOID		ProtectorInventory::UnBlock()

// ��� ��� �ߴ��� ����
inline BOOL		ProtectorInventory::IsUsable() const
{
	return CONTROL_FIELD().IsSet( eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_USABLE );
}

// ��� �ִ���
inline BOOL		ProtectorInventory::IsEmptyPassword()
{
	return PASSWORD().IsEmpty();
}

// ��� �� (������ �Է��� ��ȣ�� �� DB�� �����Ǿ� ���� ��ȣ�� ��)
inline BOOL		ProtectorInventory::IsEqualPassword( CHAR pszPwd[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH+1] )
{
	if( 0 == strncmp( PASSWORD().Get().GetBUFFER(), pszPwd, MAX_WAREHOUSE_INVENTORY_PWD_LENGTH ) )
		return TRUE;
	return FALSE;
}

// ��� ���� (DB���� ���� �о���� ��, ����)
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




