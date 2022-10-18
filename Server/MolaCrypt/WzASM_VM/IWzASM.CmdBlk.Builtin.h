#pragma once

#pragma warning(push)
#pragma warning(disable:4067)
#pragma pack(push, 1)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
struct sBUILTIN_001_A_PLUS_B_RET
{
	sIL_MOV_EAX_B4<0>								M01;	// EAX=B4
	sIL_OP_EAX_PLUS_B4<0>							M02;	// EBX=B4, EAX+=EBX
	sIL_MOV_ERP_EAX									M03;	// ERP = EAX
	sRET											M99_RET;

	inline WORD				GetCmdStreamSize()	{ return sizeof(*this); }

#if SERVER_CODE || CODE_TEST(__MOLA_IwASM_CMD_SERVER_CODE_TEST__)
	//
	//	DWORD dwARG[2] = { A, B }
	inline BOOL				SetArgument( PARGB_T pARG )
	{
		DWORD* dwARG = (DWORD*)pARG;
		M01.ChangeVAL( dwARG[0] );
		M02.M01.ChangeVAL( dwARG[1] );

		return TRUE;
	}
	inline sRETURN_PTR_SZ	GetResult()
	{
		sRETURN_PTR_SZ sRET;
		sRET.RET_TYPE = sRETURN_PTR_SZ::VALUE;
		sRET.RET_EX = M01.M02.VALUE + M02.M01.M02.VALUE;

		return sRET;
	}
#endif //
};

//////////////////////////////////////////////////////////////////////////
struct sBUILTIN_002_A_PLUS_B_PLUS_C_RET
{
	sIL_MOV_EBX_B4<0>								M01;	// EBX=B4
	sIL_OP_EBX_PLUS_B4<0>							M02;	// EAX=B4, EBX+=EAX
	sIL_OP_EBX_PLUS_B4<0>							M03;	// EAX=B4, EBX+=EAX
	sIL_MOV_ERP_EBX									M04;	// ERP = EBX
	sRET											M99_RET;

	inline WORD				GetCmdStreamSize()	{ return sizeof(*this); }

#if SERVER_CODE || CODE_TEST(__MOLA_IwASM_CMD_SERVER_CODE_TEST__)
	//
	//	DWORD dwARG[2] = { A, B, C }
	inline BOOL				SetArgument( PARGB_T pARG )
	{
		DWORD* dwARG = (DWORD*)pARG;
		M01.ChangeVAL( dwARG[0] );
		M02.M01.ChangeVAL( dwARG[1] );
		M03.M01.ChangeVAL( dwARG[2] );

		return TRUE;
	}
	inline sRETURN_PTR_SZ	GetResult()
	{
		sRETURN_PTR_SZ sRET;
		sRET.RET_TYPE = sRETURN_PTR_SZ::VALUE;
		sRET.RET_EX = M01.M02.VALUE + M02.M01.M02.VALUE + M03.M01.M02.VALUE;

		return sRET;
	}
#endif //
};


//////////////////////////////////////////////////////////////////////////
struct sBUILTIN_002_A_PLUS_B_PLUS_C_PLUS_D_RET
{
	sIL_MOV_EBX_B4<0>								M01;	// EBX=B4
	sIL_OP_EBX_PLUS_B4<0>							M02;	// EAX=B4, EBX+=EAX
	sIL_OP_EBX_PLUS_B4<0>							M03;	// EAX=B4, EBX+=EAX
	sIL_OP_EBX_PLUS_B4<0>							M04;	// EAX=B4, EBX+=EAX
	sIL_MOV_ERP_EBX									M05;	// ERP = EBX
	sRET											M99_RET;

	inline WORD				GetCmdStreamSize()	{ return sizeof(*this); }
#if SERVER_CODE || CODE_TEST(__MOLA_IwASM_CMD_TEST__)
	//
	//	DWORD dwARG[2] = { A, B, C, D }
	inline BOOL				SetArgument( PARGB_T pARG )
	{
		DWORD* dwARG = (DWORD*)pARG;
		M01.ChangeVAL( dwARG[0] );
		M02.M01.ChangeVAL( dwARG[1] );
		M03.M01.ChangeVAL( dwARG[2] );
		M04.M01.ChangeVAL( dwARG[3] );

		return TRUE;
	}
	inline sRETURN_PTR_SZ	GetResult()
	{
		sRETURN_PTR_SZ sRET;
		sRET.RET_TYPE = sRETURN_PTR_SZ::VALUE;
		sRET.RET_EX = M01.M02.VALUE + M02.M01.M02.VALUE + M03.M01.M02.VALUE + M04.M01.M02.VALUE;

		return sRET;
	}
#	endif //
};

#pragma pack(pop)
#pragma warning(pop)


