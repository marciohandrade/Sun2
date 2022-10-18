#pragma once

#include <boost/static_assert.hpp>

/*---------------------------------------------------------------------------------------

	@Author : Waverix
	@Date : 2007-04-28
	@Reference
		- Utility/Documents/"Safe String Buffer Control Classes.doc"
	@Remark 
		- ���� ����� ��Ʈ��ũ ���ۿ��� ���ڿ� ��Ʈ�� ó���� ���������� ���ؼ�...
		- stl::string�� ���⿡�� ����� ���� ���δ�.
		- (2007/04/19)

		- ������ �۾��� ����, �б����� ���ڿ� ó��, �а��Ⱑ�� ���ڿ� ó��
	@Prototypes

		-> class cSTRING_BUFFER;
		-> class cSTRING_REF
		// ��ϵ� ���۸� �а� ��������� ����� ���
		-> class cSTRING_REF_RW : public cSTRING_REF
		// ���� ���� ���۸� �а� ��������� ����� ���
		-> template<typename BYTE _SZ> class cSTRING_REF_RW_WITH_BUFFER : public cSTRING_REF_RW

//---------------------------------------------------------------------------------------
*/

namespace util
{
	//////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
	// ��Ŷ���� �����ϰ� ����ϱ� ���� �����̳�
	template<typename WORD _SZ>
	class cSTRING_BUFFER
	{
		friend class cSTRING_REF;
    public:
        static const DWORD				SZ_BUFFER_LENGTH = _SZ*sizeof(TCHAR);;
		static const DWORD				SZ_STRING_LENGTH_WITH_NULL = _SZ;
		BOOST_STATIC_ASSERT( SZ_STRING_LENGTH_WITH_NULL >= 1 );
		static const DWORD				SZ_STRING_LENGTH = SZ_STRING_LENGTH_WITH_NULL-1;

	public:		cSTRING_BUFFER() { BUFINFO.pBUFFER[0] = _T('\0'); }
				cSTRING_BUFFER( const TCHAR* ptsSrc ) { CopyFrom( ptsSrc ); }
				template<typename _TYPE_SB>
				explicit cSTRING_BUFFER( const _TYPE_SB& rSrc ) { CopyFrom( rSrc ); }
				~cSTRING_BUFFER() {}
				// <METHODs>
	public:		inline TCHAR*			GetBUFFER() { return BUFINFO.pBUFFER; }
				inline operator TCHAR*() { return BUFINFO.pBUFFER; }
				inline operator TCHAR*() const { return const_cast<TCHAR*>(BUFINFO.pBUFFER); }
				inline WORD				GetBUFFER_LENGTH() { return sizeof(BUFINFO.pBUFFER); }
				inline INT				GetSTRING_LENGTH() { return GetCalcStringLength(); }
	public:		inline const TCHAR*		GetBUFFER() const { return BUFINFO.pBUFFER; }
				inline operator const TCHAR*() const { return BUFINFO.pBUFFER; }
				inline WORD				GetBUFFER_LENGTH() const { return sizeof(BUFINFO.pBUFFER); }
				inline INT				GetSTRING_LENGTH() const { return GetCalcStringLength(); }

				inline BOOL				IsReadOnly() { return FALSE; }
	protected:	INT						GetCalcStringLength()
				{
					DWORD dwLen = 0;
					TCHAR* pOffset = BUFINFO.pBUFFER;

					while( (_SZ > dwLen) && (_T('\0') != *pOffset++) )
						++dwLen;

					return dwLen;
				}

	public:		INT						CopyFrom( const TCHAR* ptsSrc )
				{
					DWORD dwLen = 0;
					const TCHAR* pSrc = ptsSrc;
					for( TCHAR* pTarget = BUFINFO.pBUFFER ; (dwLen<_SZ) && (_T('\0') != *pSrc) ; ++dwLen )
						*pTarget++ = *pSrc++;

					if( _SZ == dwLen )
					{
						BUFINFO.pBUFFER[SZ_STRING_LENGTH] = _T('\0');
						return dwLen-1;
					}

					// ���� ������ �κ� Ŭ����
					if( dwLen )	memset( &(BUFINFO.pBUFFER[dwLen]), 0, ((DWORD)_SZ-dwLen) );
					else		BUFINFO.pBUFFER[0] = _T('\0');

					return dwLen;
				}

				BOOL					MakeSafeString()
				{
					TCHAR& rChr = BUFINFO.pBUFFER[SZ_STRING_LENGTH];
					if( rChr != _T('\0') )
						rChr = _T('\0');

					return TRUE;
				}
				// <METHODs>
	protected:	inline TCHAR*	BUFFER_HDR() { return BUFINFO.pBUFFER; }
				inline WORD		BUFFER_LENGTH() const { return sizeof(BUFINFO.pBUFFER); }
				// <FIELDs>
	private:	struct
				{
					TCHAR	pBUFFER[_SZ];
				} BUFINFO;
	};
#pragma pack(pop)



	//////////////////////////////////////////////////////////////////////////
	// �⺻�� ��ϵ� ������ ���ڿ��� �б��������� ó���ϴ� Ŭ����
	class cSTRING_REF
	{
		// <TYPE DEFINITIONs>
	protected:	struct eMODE
				{
					static const DWORD		NONE			= 0x00;	// �ʱ�ȭ ��
					static const DWORD		CALCULATED		= 0x01;
				};
				// <CONSTRUCTORs>
	public:		cSTRING_REF() { BUFINFO.wBUFFER_LENGTH = 0; BUFINFO.wSTRING_LENGTH = 0; BUFINFO.pBUFFER = NULL; BUFINFO.eMODEs = eMODE::NONE; }
				~cSTRING_REF() {}
				cSTRING_REF( const TCHAR* pStrBuffer, const WORD wBufferLen )
				{ BUFINFO.wBUFFER_LENGTH = wBufferLen; BUFINFO.wSTRING_LENGTH = 0; BUFINFO.pBUFFER = (TCHAR*)pStrBuffer; BUFINFO.eMODEs = eMODE::NONE; }
				cSTRING_REF( const cSTRING_REF* pSOURCE )
				{ memcpy( &BUFINFO, &(pSOURCE->BUFINFO), sizeof(BUFINFO) ); }
				template<typename _cSTRING_BUFFER_TYPE_>
				cSTRING_REF( const _cSTRING_BUFFER_TYPE_& rBUF_SRC )
				{	BUFINFO.wBUFFER_LENGTH = rBUF_SRC.BUFFER_LENGTH();
				BUFINFO.wSTRING_LENGTH = 0;
				BUFINFO.pBUFFER = (TCHAR*)rBUF_SRC.BUFINFO.pBUFFER;
				BUFINFO.eMODEs = eMODE::NONE; }
	protected:	cSTRING_REF( WORD wBufferLen, WORD wStrLen, TCHAR* pBuffer, DWORD eMode )
				{
					BUFINFO.wBUFFER_LENGTH = wBufferLen;
					BUFINFO.wSTRING_LENGTH = wStrLen;
					BUFINFO.pBUFFER = (TCHAR*)pBuffer;
					BUFINFO.eMODEs = eMode; }

				// <METHODs>
	public:		virtual BOOL	IsReadOnly()	{ return TRUE; }

	public:		inline TCHAR*	GetBUFFER() { return BUFINFO.pBUFFER; }
				inline WORD		GetBUFFER_LENGTH() { return BUFINFO.wBUFFER_LENGTH; }
				inline INT		GetSTRING_LENGTH() { return GetCalcStringLength(); }

	protected:	inline TCHAR*	BUFFER_HDR() { return BUFINFO.pBUFFER; }
				inline WORD		BUFFER_LENGTH() { return BUFINFO.wBUFFER_LENGTH; }
				inline WORD&	STRING_LENGTH() { return BUFINFO.wSTRING_LENGTH; }

				// <returns><INT>
				//   -1 : ���� �� ���� ����
				//   n : �����ϰ� ���� �� �ִ� ���ڿ� ����, �̰��� null-terminate ���� ���� ���ڿ��� ��� ����ũ��
	protected:	virtual INT		GetCalcStringLength()
				{
					if( BUFINFO.pBUFFER && IsBadReadPtr( BUFINFO.pBUFFER, sizeof(BYTE) ) )
					{
						BUFINFO.eMODEs = eMODE::NONE;
						return -1;
					}
					if( eMODE::CALCULATED == BUFINFO.eMODEs )
						return (INT)BUFINFO.wSTRING_LENGTH;

					DWORD dwBufferLen = (DWORD)BUFINFO.wBUFFER_LENGTH;
					DWORD dwLen = 0;
					TCHAR* pOffset = BUFINFO.pBUFFER;

					while( (dwBufferLen > dwLen) && (_T('\0') != *pOffset++) )
						++dwLen;

					BUFINFO.wSTRING_LENGTH = (WORD)dwLen;
					BUFINFO.eMODEs = eMODE::CALCULATED;

					return (INT)dwLen;
				}

	public:		virtual BOOL	MakeSafeString() { return FALSE; }

				// <comment> this�� ���ڿ��� ��¹��ۺ��� Ŀ�� tempBuffer.BUFFER_SIZE-1 ��ŭ �����ϰ� NULLó���� �����Ѵ�.
				// <returns> ������ ���ڿ� ����, -1�̸� ���� ����
	public:		template<typename _cSTRING_BUFFER_TYPE_>
				INT		CopyTo( _cSTRING_BUFFER_TYPE_& OUT rOUTBUF )
				{
					if( rOUTBUF.IsReadOnly() )
						return -1;

					INT iRet = GetCalcStringLength();
					if( iRet < 0 )	return iRet;

					if( BUFINFO.pBUFFER == rOUTBUF.BUFINFO.pBUFFER )
					{
						rOUTBUF.BUFINFO.wBUFFER_LENGTH = BUFINFO.wBUFFER_LENGTH;
						rOUTBUF.BUFINFO.wSTRING_LENGTH = BUFINFO.wSTRING_LENGTH;
						rOUTBUF.BUFINFO.eMODEs = BUFINFO.eMODEs;

						return iRet;
					}

					iRet = sizeof(TCHAR)*iRet;
					WORD wTmpBufferLen = rOUTBUF.BUFFER_LENGTH();
					BOOL bLessThanTHIS = (BOOL) !(iRet < (INT)wTmpBufferLen);
					if( bLessThanTHIS )
					{
						memcpy( rOUTBUF.BUFFER_HDR(), BUFINFO.pBUFFER, wTmpBufferLen );
						rOUTBUF.BUFFER_HDR()[wTmpBufferLen-1] = _T('\0');
						rOUTBUF.STRING_LENGTH() = wTmpBufferLen-1;
						rOUTBUF.BUFINFO.eMODEs = eMODE::CALCULATED;

						return (INT)rOUTBUF.STRING_LENGTH();
					}

					if( iRet )
					{
						memcpy( rOUTBUF.BUFFER_HDR(), BUFINFO.pBUFFER, iRet );
						rOUTBUF.BUFFER_HDR()[iRet] = _T('\0');
						rOUTBUF.STRING_LENGTH() = (WORD)(iRet);
						rOUTBUF.BUFINFO.eMODEs = eMODE::CALCULATED;
						return (INT)rOUTBUF.STRING_LENGTH();
					}

					rOUTBUF.BUFFER_HDR()[0] = _T('\0');
					rOUTBUF.STRING_LENGTH() = 0;
					rOUTBUF.BUFINFO.eMODEs = eMODE::CALCULATED;
					return 0;
				}

				template<typename _cSTRING_BUFFER_TYPE_>
				INT		CopyFrom( _cSTRING_BUFFER_TYPE_& IN rINBUF )
				{
					if( IsReadOnly() )
						return -1;

					INT iRet = rINBUF.GetCalcStringLength();
					if( iRet < 0 )	return iRet;

					if( BUFINFO.pBUFFER == rINBUF.BUFINFO.pBUFFER )
					{
						BUFINFO.wBUFFER_LENGTH = rINBUF.BUFINFO.wBUFFER_LENGTH;
						BUFINFO.wSTRING_LENGTH = rINBUF.BUFINFO.wSTRING_LENGTH;
						BUFINFO.eMODEs = rINBUF.BUFINFO.eMODEs;

						return iRet;
					}

					iRet = sizeof(TCHAR)*iRet;
					WORD wTmpBufferLen = BUFFER_LENGTH();
					BOOL bLessThanTHIS = (BOOL) !(iRet < (INT)wTmpBufferLen);
					if( bLessThanTHIS )
					{
						memcpy( BUFFER_HDR(), rINBUF.BUFFER_HDR(), wTmpBufferLen );
						BUFFER_HDR()[wTmpBufferLen-1] = _T('\0');
						STRING_LENGTH() = wTmpBufferLen-1;
						BUFINFO.eMODEs = eMODE::CALCULATED;

						return (INT)STRING_LENGTH();
					}

					if( iRet )
					{
						memcpy( BUFFER_HDR(), rINBUF.BUFFER_HDR(), iRet );
						BUFFER_HDR()[iRet] = _T('\0');
						STRING_LENGTH() = (WORD)(iRet);
						BUFINFO.eMODEs = eMODE::CALCULATED;
						return (INT)STRING_LENGTH();
					}

					BUFFER_HDR()[0] = _T('\0');
					STRING_LENGTH() = 0;
					BUFINFO.eMODEs = eMODE::CALCULATED;
					return 0;
				}

				INT		CopyFromString( const TCHAR* pSrcString )
				{
					if( IsReadOnly() )
						return -1;

					DWORD dwBufferLen = (DWORD)BUFFER_LENGTH();
					DWORD dwLen = 0;
					TCHAR* pSrcOffset = (TCHAR*)pSrcString;
					TCHAR* pOffset = BUFFER_HDR();

					while( (dwBufferLen > dwLen) && (_T('\0') != *pSrcOffset) )
					{
						*pOffset = *pSrcOffset;
						++dwLen;
						++pOffset;
						++pSrcOffset;
					}

					if( dwBufferLen > dwLen )
					{
						BUFFER_HDR()[dwBufferLen-1] = _T('\0');
						BUFFER_HDR()[dwLen] = _T('\0');
					}

					// if( dwBufferLen == dwLen ) no null-terminate
					STRING_LENGTH() = (WORD)dwLen;
					BUFINFO.eMODEs = eMODE::CALCULATED;

					return (INT)STRING_LENGTH();
				}

				// <FIELDs>
	protected:	struct
				{
					WORD	wBUFFER_LENGTH;
					WORD	wSTRING_LENGTH;
					DWORD	eMODEs;
					TCHAR*	pBUFFER;
				} BUFINFO;
	};

	//////////////////////////////////////////////////////////////////////////
	// ��ϵ� ���۸� �а� ��������� ����� ���
	class cSTRING_REF_RW : public cSTRING_REF
	{
		friend class cSTRING_REF;
		// <CONSTRUCTORs>
	public:		cSTRING_REF_RW() : cSTRING_REF() {}
				~cSTRING_REF_RW() {}
				cSTRING_REF_RW( const TCHAR* pStrBuffer, const WORD wBufferLen ) : cSTRING_REF( pStrBuffer, wBufferLen ) {}
				cSTRING_REF_RW( const cSTRING_REF* pSOURCE ) : cSTRING_REF( pSOURCE ) {}
				template<typename _cSTRING_BUFFER_TYPE_>
				cSTRING_REF_RW( const _cSTRING_BUFFER_TYPE_& rBUF_SRC ) : cSTRING_REF( rBUF_SRC ) {}
	protected:	cSTRING_REF_RW( WORD wBufferLen, WORD wStrLen, TCHAR* pBuffer, DWORD eMode ) : cSTRING_REF( wBufferLen, wStrLen, pBuffer, eMode ) {}
				// <METHODs>
	public:		virtual BOOL	IsReadOnly()	{ return FALSE; }

				// <returns><INT>
				//   -1 : ���� �� ���� ����
				//   n : �����ϰ� ���� �� �ִ� ���ڿ� ����, �̰��� null-terminate ���� ���� ���ڿ��� ��� (����ũ��-1)
	protected:	virtual INT		GetCalcStringLength()
				{
					if( (!BUFINFO.pBUFFER)
						|| (0 == BUFINFO.wBUFFER_LENGTH)
						|| IsBadReadPtr( BUFINFO.pBUFFER, sizeof(BYTE) ) )
						return -1;

					if( eMODE::CALCULATED == BUFINFO.eMODEs )
						return (INT)BUFINFO.wSTRING_LENGTH;

					DWORD dwBufferLen = (DWORD)BUFINFO.wBUFFER_LENGTH;
					DWORD dwLen = 0;
					TCHAR* pOffset = BUFINFO.pBUFFER;

					while( (dwBufferLen > dwLen) && (_T('\0') != *pOffset++) )
						++dwLen;

					if( dwBufferLen != dwLen )
						BUFINFO.wSTRING_LENGTH = (WORD)dwLen;
					else
						BUFINFO.wSTRING_LENGTH = (WORD)( dwLen ? dwLen-1 : 0 );

					BUFINFO.eMODEs = eMODE::CALCULATED;

					return (INT)(BUFINFO.wSTRING_LENGTH);
				}

	public:		virtual BOOL	MakeSafeString()
				{
					INT iRet = GetCalcStringLength();
					if( iRet < 0 )
						return FALSE;

					if( IsBadWritePtr( BUFINFO.pBUFFER, BUFINFO.wBUFFER_LENGTH ) )
						return FALSE;

					BUFINFO.pBUFFER[BUFINFO.wSTRING_LENGTH] = _T('\0');
					BUFINFO.pBUFFER[BUFINFO.wBUFFER_LENGTH-1] = _T('\0');

					return TRUE;
				}
	};

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� ���۷� �а� ��������� ����� ���
	template<typename WORD _SZ>
	class cSTRING_REF_RW_WITH_BUFFER : public cSTRING_REF_RW
	{
		// <CONSTRUCTORs>
	public:		cSTRING_REF_RW_WITH_BUFFER() : cSTRING_REF_RW( BUFFER_BLOCK.GetBUFFER(), (WORD)BUFFER_BLOCK.GetBUFFER_LENGTH() ) {}

	protected:	cSTRING_BUFFER<_SZ>		BUFFER_BLOCK;
	};

}; //namespace
