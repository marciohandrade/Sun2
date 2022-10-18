#pragma once

//=======================================================================================================================
/// VarMsg class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 1. 11
	@remarks
			- 가변 패킷을 보낼때 사용하는 클래스
	@note
			- 
	@history
			- 2006/01/12 : StartTypePtr(Type) 함수 추가 : 생성자를 호출할 있게
			- 2006/04/04 : (WAVERIX) VarMsg 대체하는 VarMsg2 구현 및 적용
			- 2007/09/10 : (WAVERIX) VARMSG_EX 구현 PacketStruct_AG.h
						(위의 것들이 사용하기 힘들다 및 패킷 브로드 캐스터 구현을 목적으로 함)
	@todo
			- 
	@usage
			- ex)

			*1번째 방법 : 생성자를 호출하지 않아서 반드시 초기화를 해야 함!
			MSG_CG_PACKET * pMsgEx = (MSG_CG_PACKET *)m_VarMsg.StartPtr();
			pMsgEx->m_byCategory = 0;
			pMsgEx->m_byProtocol = 1;
			m_VarMsg.SetSize( pMsgEx->GetSize() );
			BASEINFO *pResultMsg = (BASEINFO*)m_VarMsg.NextPtr();
			m_VarMsg.SetSize(pResultMsg->GetSize());
			pPlayer->SendExPacket( m_VarMsg.GetMsgCount(), m_VarMsg.GetMsgPPtr(), m_VarMsg.GetMsgSizePtr() );
			*2번째 방법 : 생성자를 호출해줌
			MSG_CG_PACKET * pMsgEx = m_VarMsg.StartTypePtr(Param2Class<MSG_CG_PACKET>());
			m_VarMsg.SetSize( pMsgEx->GetSize() );
			BASEINFO *pResultMsg = (BASEINFO*)m_VarMsg.NextPtr();
			m_VarMsg.SetSize(pResultMsg->GetSize());
			pPlayer->SendExPacket( m_VarMsg.GetMsgCount(), m_VarMsg.GetMsgPPtr(), m_VarMsg.GetMsgSizePtr() );

//////////////////////////////////////////////////////////////////////////


*/
//=======================================================================================================================
#if SUN_CODE_BACKUP

#define _MAX_MSG_NUM			(500)
#define _MAX_MSG_BUFFER_SIZE	(1024)

class VarMsg
{
public:
	VarMsg(void);
	~VarMsg(void);

	inline BYTE * StartPtr()			{ m_wCout = 0; return m_ppMsg[m_wCout++]; }

	template< class PacketType >
	inline PacketType * StartTypePtr( Param2Class<PacketType> )	{ m_wCout = 0; return new (m_ppMsg[m_wCout++])(PacketType); }
	inline BYTE * NextPtr()				{ ASSERT(m_wCout<_MAX_MSG_NUM); return m_ppMsg[m_wCout++]; }
	inline VOID SetSize( WORD wSize )	{ ASSERT(m_wCout>0); m_pwSize[m_wCout-1] = wSize; }
	

	inline BYTE ** GetMsgPPtr()			{ return m_ppMsgOut; }
	inline WORD * GetMsgSizePtr()		{ return m_pwSize; }
	inline WORD	GetMsgCount()			{ return m_wCout;	}

private:
	BYTE	m_ppMsg[_MAX_MSG_NUM][_MAX_MSG_BUFFER_SIZE];
	BYTE *	m_ppMsgOut[_MAX_MSG_NUM];
	WORD	m_pwSize[_MAX_MSG_NUM];
	WORD	m_wCout;
};
#endif //SUN_CODE_BACKUP

//////////////////////////////////////////////////////////////////////////
// <2006-04-02><Waverix>
// <설명>
//   VarMsg를 스트림의 연속형으로 변환시킨 것으로 VarMsg와 사용방법은 같다. (추가적인 인터페이스 하나 존재)
// <개선점>
//   GetMsgCount()는 항상 1로 유지시켜 네트워크 버퍼로의 처리 복잡도를 O(1)으로 만들어 준다.
//   공간 복잡도를 기존에 비해 1/10로 줄여준다. (사용자 버퍼에 기준할 필요는 있을 것이다. 일단 64KByte)
// <개선 방안>
//   VarMsg와 같은 성격의 기능과 합치고, Network Module단의 개선이 이뤄진다면,
//   처리량의 감소의 이점을 얻을 수 있을 것이며, 부가적으로
//   물리 네트워크 환경의 성능도 높일 수 있을 거라 기대한다.
//   예) TCP packet(20B)+SUN_ACK(5B) --- TCP packet(20B)+SUN_ACK(5B) == 50B
//       개선된다면) TCP packet(20B)+SUN_ACK(5B)*2 = 30B
//       -> 이 경우 TCP가 가지는 ACK과 같은 추가적 오버헤드도 줄일 것이다.
//   물론 위와 같은 예는 (_CMD+_CMD, _CMD+_ACK, _CMD+_NAK)(반대도 가능) 연계되도 무방한 부분에 대해 사용이 가능할지도...
//   또한 정책적인 면도 고려되어야 하고, 무엇보다 특정 크기 이내에서 이뤄져야 할 것이다.

#define _MAX_MSG_BUFFER_TOTAL_SIZE	((((WORD)-1)+1)-(sizeof(DWORD)+sizeof(BYTE**)))

class VarMsg2
{
public:
	VarMsg2() {}
	~VarMsg2() {}

	inline BYTE* StartPtr()				{ /*m_wCout=0;*/ m_ppMsgOut[0]=m_pMsgBlock; m_dwSize=0; return m_pMsgBlock; };
	template<typename PacketType>
	inline PacketType* StartTypePtr( Param2Class<PacketType> )
	{
		m_ppMsgOut[0]=m_pMsgBlock;
		m_dwSize /*= m_wCout*/ = 0;
		return new (m_pMsgBlock)(PacketType);
	}

	// 다음은 Network 단의 개선이 있다면 사용이 가능할 것이다
	//inline PacketType* StartTypePtrNext( Param2Class<PacketType> ) { ++m_wCout; return new (m_pMsgBlock+m_dwSize)(PacketType); }

	inline BYTE* NextPtr() { return (m_pMsgBlock+m_dwSize); }

	// 사용하기는 편하지만 최적화 레벨이 높아도 (Release) 템플릿의 특성상 inline 계열의 생성자 조차도 함수 호출로 변환된다.
	// 굳이 생성자 호출이 필요없거나 사용자 레벨의 호출로 변환할 거라면 위 프로토타입이 더 좋을 것이다.
	template<typename PacketType>
	inline PacketType* NextPtrWithCTOR( Param2Class<PacketType> )
	{
		return new (m_pMsgBlock+m_dwSize)(PacketType);
	}

	inline VOID SetSize( WORD wSize )
	{
		DWORD conv_wSize = (DWORD)wSize;
		ASSERT( !((m_dwSize+conv_wSize)>_MAX_MSG_BUFFER_TOTAL_SIZE) );
		m_dwSize += conv_wSize;
	}

	inline BYTE** GetMsgPPtr()			{ return (BYTE**)(m_ppMsgOut); }
	inline WORD* GetMsgSizePtr()		{ return (WORD*)&m_dwSize; }
	inline WORD	GetMsgCount()			{ return 1;	}

private:
#pragma pack(push, 1)
	BYTE	m_pMsgBlock[_MAX_MSG_BUFFER_TOTAL_SIZE];
	BYTE*	m_ppMsgOut[1];
	DWORD	m_dwSize;
#pragma pack(pop)
	/*WORD	m_wCout;*/ 
};

//	- (WAVERIX)(080317) 적용 했던 부분 제거 (실제 적용은 된 적 없음)
#ifdef __NA000000_070928_PACKET_UNICAST_UPDATE__

template<DWORD _ROUTECOUNT>
class VARMSG_EX_ASSMBLER_ROUTER
{
	typedef VARMSG_EX_ASSMBLER_ROUTER<_ROUTECOUNT>	router_type;
	static const MAX_NODE_COUNT						= 5;
	typedef VARMSG_ASSEMBLER0						node0_type;
	typedef VARMSG_ASSEMBLER1						node1_type;
	typedef VARMSG_ASSEMBLER2						node2_type;
	typedef VARMSG_ASSEMBLER3						node3_type;
	typedef VARMSG_ASSEMBLER4						node4_type;

	struct IVARMSG_EX_ROUTING_NODE
	{
	public:
		virtual ~IVARMSG_EX_ROUTING_NODE() { IRelease(); }

		virtual VOID		Init() = 0;
		virtual PVOID		IGetPtr() = 0;
		virtual VOID		IRelease() {}
		virtual INT			IAddOffset( INT iOffset ) = 0;
		virtual INT			IControlOffset( BOOL bIsPlus, INT iOffset ) = 0;
		virtual PVOID		IGetFirstPacketPtr() = 0;
		virtual VOID		ISetTotalSize( INT iTotalSize ) = 0;
		virtual INT			IGetTotalSize() = 0;
	};

	template<typename _TYPE>
	struct VARMSG_EX_ROUTING_NODE_W : public IVARMSG_EX_ROUTING_NODE
	{
		typedef _TYPE		node_type;

		VARMSG_EX_ROUTING_NODE_W() { m_node.Clear(); }

		virtual VOID		Init() { m_node.Clear(); }
		virtual PVOID		IGetPtr() { return (PVOID)&(m_node.TypeOf<BYTE>()); }
		virtual VOID		IRelease() { m_node.Clear(); }
		virtual INT			IAddOffset( INT iOffset ) { return m_node.AddOffset( iOffset ); }
		virtual INT			IControlOffset( BOOL bIsPlus, INT iOffset ) { return m_node.ControlOffset( bIsPlus, iOffset ); }
		virtual PVOID		IGetFirstPacketPtr() { return m_node.GetFirstPacketPtr(); }
		virtual VOID		ISetTotalSize( INT iTotalSize ) { m_node.SetTotalSize( iTotalSize ); }
		virtual INT			IGetTotalSize() { return m_node.GetTotalSize(); }

		node_type			m_node;
	};

	typedef IVARMSG_EX_ROUTING_NODE						super_node_type;
	typedef VARMSG_EX_ROUTING_NODE_W<VARMSG_ASSEMBLER0>	wnode0_type;
	typedef VARMSG_EX_ROUTING_NODE_W<VARMSG_ASSEMBLER1>	wnode1_type;
	typedef VARMSG_EX_ROUTING_NODE_W<VARMSG_ASSEMBLER2>	wnode2_type;
	typedef VARMSG_EX_ROUTING_NODE_W<VARMSG_ASSEMBLER3>	wnode3_type;
	typedef VARMSG_EX_ROUTING_NODE_W<VARMSG_ASSEMBLER4>	wnode4_type;

public:
	VARMSG_EX_ASSMBLER_ROUTER()
	{
		ZeroMemory( m_pRouter, sizeof(m_pRouter) );
		ZeroMemory( m_pStream, sizeof(m_pStream) );

		INT iOffset = 0;
		if( _ROUTECOUNT > 0 ) { m_pRouter[0] = new (&m_pStream[iOffset]) wnode0_type; iOffset += sizeof(wnode0_type); }
		if( _ROUTECOUNT > 1 ) { m_pRouter[1] = new (&m_pStream[iOffset]) wnode1_type; iOffset += sizeof(wnode0_type); }
		if( _ROUTECOUNT > 2 ) { m_pRouter[2] = new (&m_pStream[iOffset]) wnode2_type; iOffset += sizeof(wnode0_type); }
		if( _ROUTECOUNT > 3 ) { m_pRouter[3] = new (&m_pStream[iOffset]) wnode3_type; iOffset += sizeof(wnode0_type); }
		if( _ROUTECOUNT > 4 ) { m_pRouter[4] = new (&m_pStream[iOffset]) wnode4_type; iOffset += sizeof(wnode0_type); }
	}

	template<typename _TYPE>
	inline _TYPE&				InstanceOf( DWORD dwRouterIdx )
	{
		if( _ROUTECOUNT > dwRouterIdx )
		{
			return *( new (m_pRouter[dwRouterIdx]->IGetPtr()) _TYPE );
		}

		return *(_TYPE*)NULL;
	}

	template<typename _TYPE>
	inline _TYPE&				TypeOf( DWORD dwRouterIdx )
	{
		if( _ROUTECOUNT > dwRouterIdx )
		{
			return *(_TYPE*)( m_pRouter[dwRouterIdx]->IGetPtr());
		}

		return *(_TYPE*)NULL;
	}

	inline VOID					Clear( DWORD dwRouterIdx )
	{
		if( _ROUTECOUNT > dwRouterIdx )
		{
			m_pRouter[dwRouterIdx]->IRelease();
		}
	}

	inline INT					AddOffset( DWORD dwRouterIdx, INT iOffset )
	{
		if( _ROUTECOUNT > dwRouterIdx )
		{
			return ( m_pRouter[dwRouterIdx]->IAddOffset( iOffset ) );
		}

		return 0;
	}

	inline INT					ControlOffset( DWORD dwRouterIdx, BOOL bIsPlus, SIZE_T szSize )
	{
		if( _ROUTECOUNT > dwRouterIdx )
		{
			return ( m_pRouter[dwRouterIdx]->IControlOffset( bIsPlus, iOffset ) );
		}

		return 0;
	}

	inline MSG_BASE_FORWARD*	GetFirstPacketPtr( DWORD dwRouterIdx )
	{
		if( _ROUTECOUNT > dwRouterIdx )
		{
			return (MSG_BASE_FORWARD*)( m_pRouter[dwRouterIdx]->IGetFirstPacketPtr() );
		}

		return 0;
	}

	inline VOID					SetTotalSize( DWORD dwRouterIdx, INT iTotalSize )
	{
		if( _ROUTECOUNT > dwRouterIdx )
		{
			return ( m_pRouter[dwRouterIdx]->ISetTotalSize( iTotalSize ) );
		}

		return 0;
	}

	inline INT					GetTotalSize( DWORD dwRouterIdx )
	{
		if( _ROUTECOUNT > dwRouterIdx )
		{
			return ( m_pRouter[dwRouterIdx]->IGetTotalSize() );
		}

		return 0;
	}

private:
	IVARMSG_EX_ROUTING_NODE*	m_pRouter[MAX_NODE_COUNT];
	BYTE						m_pStream[sizeof(wnode0_type)*MAX_NODE_COUNT];
};

#endif //

