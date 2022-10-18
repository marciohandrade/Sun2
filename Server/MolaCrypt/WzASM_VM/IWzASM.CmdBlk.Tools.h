#pragma once

#include "IWzASM.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
typedef BYTE*	BYTE_ARRAY_T;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
template<typename DWORD _RESIDX, DWORD _SZ>
class WzCMDBLK
{
private:	typedef STLX_VECTOR<BYTE>		CMD_STREAM;
			typedef CMD_STREAM::iterator				CMD_STREAM_IT;

public:		WzCMDBLK() {}
			~WzCMDBLK() {}

public:		VOID	SetDefaultBlk( BYTE* IN pCmdBlk, WORD IN wSize )
			{
				m_CMD.clear();
				m_CMD.assign( (&pCmdBlk[0]), (&pCmdBlk[wSize]) );
				//copy<BYTE_ARRAY_T, CMD_STREAM_IT>( (&pCmdBlk[0]), (&pCmdBlk[wSize]), m_CMD.begin() );
			}

			// 고정형, 정해진 인덱스에 정해진 크기를 치환시킨다.
			VOID	Replace( BYTE* IN pReplace )
			{
				BYTE* pDEST = &m_CMD[_RESIDX];
				CopyMemory( pDEST, pReplace, _SZ );
			}

			// 정해진 INDEX와 SZ를 무시하고 치환
			// 주의해서 사용해야 할 것이다.
			VOID	Replace( BYTE* IN pCmdBlk, WORD IN wSize, WORD IN wIdx )
			{
				BYTE* pDEST = &m_CMD[wIdx];
				CopyMemory( pDEST, pReplace, wSize );
			}

			VOID	Insert( BYTE* IN pCmdBlk, WORD IN wSize, WORD IN wIdx )
			{
				CMD_STREAM_IT it = m_CMD.begin();
				m_CMD.insert( it+wIdx, (&pCmdBlk[0]), (&pCmdBlk[wSize]) );

				//CMD_STREAM tmp;
				//tmp.assign( wSize, 0 );
				//copy<BYTE_ARRAY_T, CMD_STREAM_IT>( (&pCmdBlk[0]), (&pCmdBlk[wSize]), tmp.begin() );

				//CMD_STREAM_IT it = m_CMD.begin();
				////m_CMD.swap();
				//m_CMD.insert( it+wIdx, tmp.begin(), tmp.end() );
			}

			BOOL	Output( BYTE* OUT pCmdBlk, WORD IN wSize )
			{
				SIZE_T iSize = m_CMD.size();
				if( !pCmdBlk || (wSize < (WORD)iSize) )
					return FALSE;

				copy<CMD_STREAM_IT, BYTE_ARRAY_T>( m_CMD.begin(), m_CMD.end(), pCmdBlk );
				return TRUE;
			}

private:	CMD_STREAM		m_CMD;
};



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <IBF ARGUMENT MAKER>
struct sUREG_NODE
{
	uREG_NODE	NODE;
	sUREG_NODE()
	{
		NODE.EXA[sARG::ARG0].PTR = NULL;
		NODE.EXA[sARG::ARG1].PTR = NULL;
		NODE.EXA[sARG::ARG2].PTR = NULL;
		NODE.EXA[sARG::ARG3].PTR = NULL;
	};

	//template<typename WzASM_EIP* _ARG0, typename BYTE* _ARG1, typename WORD _ARG2, typename DWORD _ARG3>
	//template<typename _DECLARG1, typename _DECLARG2, typename _DECLARG3, typename DWORD _ARG4>
	//template<typename WzASM_EIP* _ARG0, typename BYTE* _ARG1, typename WORD _ARG2, typename DWORD _ARG3>

	sUREG_NODE( WzASM_EIP* _ARG0 )
	{
		NODE.EXA[sARG::ARG0].PTR = (WzPTR)_ARG0;
		NODE.EXA[sARG::ARG1].PTR = NULL;
		NODE.EXA[sARG::ARG2].PTR = NULL;
		NODE.EXA[sARG::ARG3].PTR = NULL;
	}

	sUREG_NODE( WzASM_EIP* _ARG0, BYTE* _ARG1, WORD _ARG2, DWORD _ARG3 )
	{
		NODE.EXA[sARG::ARG0].PTR = (WzPTR)_ARG0;
		NODE.EXA[sARG::ARG1].PTRB = _ARG1;
		NODE.EXA[sARG::ARG2].X = _ARG2;
		NODE.EXA[sARG::ARG3].EX = _ARG3;
	}

	sUREG_NODE( WzASM_EIP* _ARG0, BYTE* _ARG1, WORD _ARG2 )
	{
		NODE.EXA[sARG::ARG0].PTR = (WzPTR)_ARG0;
		NODE.EXA[sARG::ARG1].PTRB = _ARG1;
		NODE.EXA[sARG::ARG2].X = _ARG2;
		NODE.EXA[sARG::ARG3].PTR = NULL;
	}

	sUREG_NODE( WzASM_EIP* _ARG0, DWORD _ARG1 )
	{
		NODE.EXA[sARG::ARG0].PTR = (WzPTR)_ARG0;
		NODE.EXA[sARG::ARG1].EX = _ARG1;
		NODE.EXA[sARG::ARG2].PTR = NULL;
		NODE.EXA[sARG::ARG3].PTR = NULL;
	}

	sUREG_NODE( WzASM_EIP* _ARG0, WzPTR _ARG1, DWORD _ARG2 )
	{
		NODE.EXA[sARG::ARG0].PTR = (WzPTR)_ARG0;
		NODE.EXA[sARG::ARG1].PTR = _ARG1;
		NODE.EXA[sARG::ARG2].EX = _ARG2;
		NODE.EXA[sARG::ARG3].PTR = NULL;
	}

	sUREG_NODE( WzASM_EIP* _ARG0, WzPTR _ARG1, WzPTR _ARG2, DWORD _ARG3 )
	{
		NODE.EXA[sARG::ARG0].PTR = (WzPTR)_ARG0;
		NODE.EXA[sARG::ARG1].PTR = _ARG1;
		NODE.EXA[sARG::ARG2].PTR = _ARG2;
		NODE.EXA[sARG::ARG3].EX = _ARG3;
	}

	////template<typename _DECLARG0, typename _DECLARG1, typename _DECLARG2>
	//template<typename _DECLT0, typename _DECLT1, typename _DECLT2, typename _DECL3>
	//sUREG_NODE( _DECLT0 _ARG0, _DECLT0 _ARG0, _DECLT0 _ARG0, _DECLT0 _ARG0,  )
	//{
	//	NODE.EXA[sARG::ARG0].PTR = (WzPTR)_DECLARG0;
	//	NODE.EXA[sARG::ARG1].PTR = (WzPTR)_DECLARG;
	//	NODE.EXA[sARG::ARG2].PTR = (WzPTR)_DECLARG;
	//	NODE.EXA[sARG::ARG3].EX = _ARG3;
	//}
};


