//	Cryptmanager.ISecureWorkNode.CmdStream.h
#pragma once

#pragma pack(push, 1)

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
//

class ISecureQueryCmdStreamInit : public ISecureQueryCmdStreamBase
{
public:		virtual VOID						Release()
			{
			}
#if SERVER_CODE || CODE_TEST(__MOLA_IwASM_CMD_SERVER_CODE_TEST__)
			virtual VOID						Init()
			{
			}
			virtual BYTE*						GetCmdStream( PARGB_T pARG, WORD& rwSIZE )
			{
				rwSIZE = 0;
				return NULL;
			}
			virtual sRETURN_PTR_SZ				ExecQueryNResult()
			{
				sRETURN_PTR_SZ info;
				info.RET_TYPE = sRETURN_PTR_SZ::PTR;
				info.RET_STREAM = NULL;
				info.RET_SZ = 0;

				return info;
			}
#endif //SERVER_CODE

			// 주어진 포인터의 가상함수테이블 타입주소를 통해 새롭게 복사본을 만들어낸다.
			virtual ISecureQueryCmdStreamBase*	GetInstance( BYTE* IN pAllocedPtr )
			{
				ISecureQueryCmdStreamInit* pInstance = new (pAllocedPtr) ISecureQueryCmdStreamInit();
				return pInstance;
			}
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	Builtin으로 해결되는 코드들을 여기에 입력하기로 한다.
//
//	BuiltinCode Definition
//	[CMD STREAM]
//		WORD			GetCmdStreamSize();
//		BOOL			SetArgument( PARGB_T pARG );
//		sRETURN_PTR_SZ	GetResult();
//
template<typename _CMDBLK, typename BYTE _TMP>
class ISecureQueryCmdStreamBuiltinCmd : public ISecureQueryCmdStreamBase
{
public:		virtual VOID						Release()
			{
			}
#if SERVER_CODE || CODE_TEST(__MOLA_IwASM_CMD_SERVER_CODE_TEST__)
			virtual VOID						Init()
			{
			}
			virtual BYTE*						GetCmdStream( PARGB_T pARG, WORD& rwSIZE )
			{
				if( pARG )
					m_CMDSTREAM.SetArgument( pARG );
				rwSIZE = m_CMDSTREAM.GetCmdStreamSize();
				return (BYTE*)&m_CMDSTREAM;
			}
			virtual sRETURN_PTR_SZ				ExecQueryNResult()
			{
				sRETURN_PTR_SZ sRET = m_CMDSTREAM.GetResult();
				return sRET;
			}

#endif //SERVER_CODE

			// 주어진 포인터의 가상함수테이블 타입주소를 통해 새롭게 복사본을 만들어낸다.
			virtual ISecureQueryCmdStreamBase*	GetInstance( BYTE* IN pAllocedPtr )
			{
				ISecureQueryCmdStreamBuiltinCmd* pInstance = new (pAllocedPtr) ISecureQueryCmdStreamBuiltinCmd();
				return pInstance;
			}

private:	_CMDBLK		m_CMDSTREAM;
};





#pragma pack(pop)


