#pragma once

namespace util
{
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//	BIT Operation
	//	Fix Array Control
	//
	template<DWORD _MAX_SIZE>
	class BitSetOperation
	{
	public:
		inline VOID		ClearAll() { memset( m_Array, 0, sizeof(m_Array) ); }
		inline BOOL		operator [] ( DWORD dwIndex ) { return IsSet( dwIndex ); }

		inline BOOL		IsSet( DWORD dwIndex )
		{
			//ASSERT( dwIndex < max_size );
			if( !(dwIndex<_MAX_SIZE) )
				return FALSE;

			return (BOOL)( !!(m_Array[dwIndex>>5] & (1UL<<(dwIndex&31))) );
		}

		inline VOID		Set( DWORD dwIndex )
		{
			//ASSERT( dwIndex < max_size );
			if( !(dwIndex<_MAX_SIZE) )
				return;

			(m_Array)[dwIndex >> 5] |= (1UL << (dwIndex & 31));
		}

		inline VOID		UnSet( DWORD dwIndex )
		{
			//ASSERT( dwIndex < max_size );
			if( !(dwIndex<_MAX_SIZE) )
				return;

			(m_Array)[dwIndex >> 5] &= ~(1UL << (dwIndex & 31));
		}

		//	(WAVERIX)(071114)
		//	Append Interface 'Serialize'
		enum eSERIAL_CMD { LOAD = 0, SAVE = 1, };
		inline BOOL		Serialize( const eSERIAL_CMD eCMD, DWORD& INOUT rBITSTREAM, const DWORD IDXofARRAY )
		{
			if( IDXofARRAY < sizeof(m_Array) )
			{
				switch( eCMD )
				{
				case eSERIAL_CMD::LOAD:
					rBITSTREAM = m_Array[IDXofARRAY];
					return TRUE;
				case eSERIAL_CMD::SAVE:
					m_Array[IDXofARRAY] = rBITSTREAM;
					return TRUE;
				}
			}
			rBITSTREAM = 0;
			return FALSE;
		}

	private:
		DWORD	m_Array[ ( (_MAX_SIZE)>(1<<5) ) ? ( (_MAX_SIZE)>>5 )+1 : 1 ];
	};

	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//	BIT Operation
	//
	template<>
	class BitSetOperation<32>
	{
	#pragma push_macro("BITSET_OP")
	#define BITSET_OP(index)	(1UL<<(index))
	private:
		static const DWORD	_MAX_SIZE	= 32;
	public:
		BitSetOperation() : m_Array(0) {}
		inline	VOID		ClearAll() { m_Array = 0; }
		inline	BOOL		operator [] ( const DWORD index )
		{
			return (index < _MAX_SIZE) ? !!(BITSET_OP(index)&m_Array) : 0;
		}

		inline	BOOL		IsSet( const DWORD index )
		{
			return (*this)[index];
		}

		inline	VOID		Set( const DWORD index )
		{
			if(index < _MAX_SIZE)
				m_Array |= BITSET_OP(index);
		}

		inline	VOID		UnSet( const DWORD index )
		{
			if(index < _MAX_SIZE)
				m_Array &= ~BITSET_OP(index);
		}

		//	(WAVERIX)(071114)
		//	Append Interface 'Serialize'
		enum eSERIAL_CMD { LOAD = 0, SAVE = 1, };
		inline	BOOL		Serialize( const eSERIAL_CMD eCMD, DWORD& INOUT rBITSTREAM, const DWORD IDXofARRAY = 0 )
		{
            __UNUSED(IDXofARRAY);
			switch( eCMD )
			{
			case LOAD:
				rBITSTREAM = m_Array;
				return TRUE;
			case SAVE:
				m_Array = rBITSTREAM;
				return TRUE;
			}
			rBITSTREAM = 0;
			return FALSE;
		}

		inline	BOOL		SerializeStore( const DWORD rBITSTREAM, const DWORD IDXofARRAY = 0 )
		{
            __UNUSED(IDXofARRAY);
            m_Array = rBITSTREAM;
            return TRUE;
        }
		inline	BOOL		SerializeLoad( DWORD& OUT rBITSTREAM, const DWORD IDXofARRAY = 0 )
		{
            __UNUSED(IDXofARRAY);
            rBITSTREAM = m_Array;
            return TRUE;
        }

	private:	DWORD	m_Array;

	#undef BITSET_OP
	#pragma pop_macro("BITSET_OP")
	};

	template<>
	class BitSetOperation<31> : public BitSetOperation<32>
	{
	};



	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//	BIT Operation
	//	Variable Array
	//	-	History
	//	-	080822 - waverix : addin
	//
	class BitSetOperationV
	{
	public:		BitSetOperationV()
					: MaxBitSize( m_MaxBitSize )
					, ArraySize( m_ArraySize )
					, m_MaxBitSize(0)
					, m_ArraySize(0)
					, m_pArray(0)
				{}

				~BitSetOperationV()
				{
					if( m_pArray )
						TAllocFree( m_pArray );
					m_pArray = 0;
				}

		inline	VOID		Create( const DWORD max_index )
				{
					m_MaxBitSize	= max_index;
					m_ArraySize		= ( (max_index)>(1<<5) ) ? ( (max_index)>>5 )+1 : 1 ;
					if( m_pArray )
						TAllocFree( m_pArray );

					const DWORD byte_numbers = (DWORD)(m_ArraySize*sizeof(DWORD));
					m_pArray		= (DWORD*)TAllocMalloc( byte_numbers );
					ZeroMemory( m_pArray, byte_numbers );
				}

		inline	VOID		ClearAll()
				{
					if( m_pArray )
					{
						const DWORD byte_numbers = (DWORD)(m_ArraySize*sizeof(DWORD));
						ZeroMemory( m_pArray, byte_numbers );
					}
				}
		inline	BOOLEAN		operator [] ( const DWORD index ) const { return IsSet( index ); }

		inline	BOOLEAN		IsSet( const DWORD index ) const
				{
					//ASSERT( index < max_size );
					if( !(index<m_MaxBitSize) )
						return FALSE;

					return (BOOLEAN)( !!(m_pArray[index>>5] & (1UL<<(index&31))) );
				}

		inline	VOID		Set( const DWORD index )
				{
					//ASSERT( index < max_size );
					if( !(index<m_MaxBitSize) )
						return;

					(m_pArray)[index >> 5] |= (1UL << (index & 31));
				}

		inline	VOID		UnSet( const DWORD index )
				{
					//ASSERT( index < max_size );
					if( !(index<m_MaxBitSize) )
						return;

					(m_pArray)[index >> 5] &= ~(1UL << (index & 31));
				}

				//	(WAVERIX)(071114)
				//	Append Interface 'Serialize'
				enum eSERIAL_CMD { LOAD = 0, SAVE = 1, };

		inline	BOOLEAN		Serialize( const eSERIAL_CMD eCMD, DWORD& INOUT rBITSTREAM, const DWORD IDXofARRAY )
				{
					if( IDXofARRAY < m_ArraySize )
					{
						switch( eCMD )
						{
						case LOAD:
							rBITSTREAM = m_pArray[IDXofARRAY];
							return TRUE;
						case SAVE:
							m_pArray[IDXofARRAY] = rBITSTREAM;
							return TRUE;
						}
					}
					rBITSTREAM = 0;
					return FALSE;
				}

	public:
		const	DWORD&		MaxBitSize;		//	Property
		const	DWORD&		ArraySize;		//	Property

	private:	DWORD		m_MaxBitSize;
				DWORD		m_ArraySize;
				DWORD*		m_pArray;
                //
                __DISABLE_COPY(BitSetOperationV);
	};


} // namespace util

