#include "stdafx.h"

VOID Test( int selector )
{
	switch( selector )
	{
		case IWzASM::NOP						:
		case IWzASM::MOV_EAX_HASH			:	
		case IWzASM::MOV_EAX_B1				:
		case IWzASM::MOV_EAX_B2				:
		case IWzASM::MOV_EAX_B4				:
		case IWzASM::MOV_EAX_PTR				:

		case IWzASM::MOV_EBX_HASH			:
		case IWzASM::MOV_EBX_B1				:
		case IWzASM::MOV_EBX_B2				:
		case IWzASM::MOV_EBX_B4				:
		case IWzASM::MOV_EBX_PTR				:

		case IWzASM::MOV_EAX_EBX				:
		case IWzASM::MOV_EAX_ECX				:
		case IWzASM::MOV_EAX_EDX				:

		case IWzASM::MOV_EBX_EAX				:
		case IWzASM::MOV_EBX_ECX				:
		case IWzASM::MOV_EBX_EDX				:

		case IWzASM::MOV_ECX_EAX				:
		case IWzASM::MOV_ECX_EBX				:
		case IWzASM::MOV_ECX_EDX				:

		case IWzASM::MOV_EDX_EAX				:
		case IWzASM::MOV_EDX_EBX				:
		case IWzASM::MOV_EDX_ECX				:

		case IWzASM::MOV_EAX_ERP				:
		case IWzASM::MOV_EBX_ERP				:
		case IWzASM::MOV_ECX_ERP				:
		case IWzASM::MOV_EDX_ERP				:

		case IWzASM::MOV_ERP_EAX				:
		case IWzASM::MOV_ERP_EBX				:
		case IWzASM::MOV_ERP_ECX				:
		case IWzASM::MOV_ERP_EDX				:

		case IWzASM::MOV_EAX0_B1				:
		case IWzASM::MOV_EAX1_B1				:
		case IWzASM::MOV_EAX2_B1				:
		case IWzASM::MOV_EAX3_B1				:

		case IWzASM::MOV_EAX0_B2				:
		case IWzASM::MOV_EAX1_B2				:
		case IWzASM::MOV_EAX2_B2				:
		case IWzASM::MOV_EAX3_B2				:

		case IWzASM::MOV_EAX0_B4				:
		case IWzASM::MOV_EAX1_B4				:
		case IWzASM::MOV_EAX2_B4				:
		case IWzASM::MOV_EAX3_B4				:

		case IWzASM::MOV_EAX0_HASH			:
		case IWzASM::MOV_EAX1_HASH			:
		case IWzASM::MOV_EAX2_HASH			:
		case IWzASM::MOV_EAX3_HASH			:

		case IWzASM::MOV_EAX0_PTR			:
		case IWzASM::MOV_EAX1_PTR			:
		case IWzASM::MOV_EAX2_PTR			:
		case IWzASM::MOV_EAX3_PTR			:

		case IWzASM::MOV_EAX_EXA4B1			:
		case IWzASM::MOV_EAX_EXA4B2			:
		case IWzASM::MOV_EAX_EXA4B4			:
		case IWzASM::MOV_EAX_EXA4HS		:

		case IWzASM::MOV_EAX_EXA3B1			:
		case IWzASM::MOV_EAX_EXA3B2			:
		case IWzASM::MOV_EAX_EXA3B4			:
		case IWzASM::MOV_EAX_EXA3HS		:

		case IWzASM::MOV_EAX_EXA2B1			:
		case IWzASM::MOV_EAX_EXA2B2			:
		case IWzASM::MOV_EAX_EXA2B4			:
		case IWzASM::MOV_EAX_EXA2HS		:

		case IWzASM::MOV_EAX0_EAX1			:
		case IWzASM::MOV_EAX0_EAX2			:
		case IWzASM::MOV_EAX0_EAX3			:

		case IWzASM::MOV_EAX0_EBX0			:
		case IWzASM::MOV_EAX0_EBX1			:
		case IWzASM::MOV_EAX0_EBX2			:
		case IWzASM::MOV_EAX0_EBX3			:
		case IWzASM::MOV_EAX0_ERP0			:
		case IWzASM::MOV_EAX0_ERP1			:
		case IWzASM::MOV_EAX0_ERP2			:
		case IWzASM::MOV_EAX0_ERP3			:

		case IWzASM::MOV_EAX1_EAX0			:
		case IWzASM::MOV_EAX1_EAX2			:
		case IWzASM::MOV_EAX1_EAX3			:

		case IWzASM::MOV_EAX1_EBX0			:
		case IWzASM::MOV_EAX1_EBX1			:
		case IWzASM::MOV_EAX1_EBX2			:
		case IWzASM::MOV_EAX1_EBX3			:
		case IWzASM::MOV_EAX1_ERP0			:
		case IWzASM::MOV_EAX1_ERP1			:
		case IWzASM::MOV_EAX1_ERP2			:
		case IWzASM::MOV_EAX1_ERP3			:

		case IWzASM::MOV_EAX2_EAX0			:
		case IWzASM::MOV_EAX2_EAX1			:
		case IWzASM::MOV_EAX2_EAX3			:

		case IWzASM::MOV_EAX2_EBX0			:
		case IWzASM::MOV_EAX2_EBX1			:
		case IWzASM::MOV_EAX2_EBX2			:
		case IWzASM::MOV_EAX2_EBX3			:
		case IWzASM::MOV_EAX2_ERP0			:
		case IWzASM::MOV_EAX2_ERP1			:
		case IWzASM::MOV_EAX2_ERP2			:
		case IWzASM::MOV_EAX2_ERP3			:

		case IWzASM::MOV_EAX3_EAX0			:
		case IWzASM::MOV_EAX3_EAX1			:
		case IWzASM::MOV_EAX3_EAX2			:

		case IWzASM::MOV_EAX3_EBX0			:
		case IWzASM::MOV_EAX3_EBX1			:
		case IWzASM::MOV_EAX3_EBX2			:
		case IWzASM::MOV_EAX3_EBX3			:
		case IWzASM::MOV_EAX3_ERP0			:
		case IWzASM::MOV_EAX3_ERP1			:
		case IWzASM::MOV_EAX3_ERP2			:
		case IWzASM::MOV_EAX3_ERP3			:

		case IWzASM::MOV_ERP0_EAX0			:
		case IWzASM::MOV_ERP0_EAX1			:
		case IWzASM::MOV_ERP0_EAX2			:
		case IWzASM::MOV_ERP0_EAX3			:
		case IWzASM::MOV_ERP0_EBX0			:
		case IWzASM::MOV_ERP0_EBX1			:
		case IWzASM::MOV_ERP0_EBX2			:
		case IWzASM::MOV_ERP0_EBX3			:

		case IWzASM::MOV_ERP1_EAX0			:
		case IWzASM::MOV_ERP1_EAX1			:
		case IWzASM::MOV_ERP1_EAX2			:
		case IWzASM::MOV_ERP1_EAX3			:
		case IWzASM::MOV_ERP1_EBX0			:
		case IWzASM::MOV_ERP1_EBX1			:
		case IWzASM::MOV_ERP1_EBX2			:
		case IWzASM::MOV_ERP1_EBX3			:

		case IWzASM::MOV_ERP2_EAX0			:
		case IWzASM::MOV_ERP2_EAX1			:
		case IWzASM::MOV_ERP2_EAX2			:
		case IWzASM::MOV_ERP2_EAX3			:
		case IWzASM::MOV_ERP2_EBX0			:
		case IWzASM::MOV_ERP2_EBX1			:
		case IWzASM::MOV_ERP2_EBX2			:
		case IWzASM::MOV_ERP2_EBX3			:

		case IWzASM::MOV_ERP3_EAX0			:
		case IWzASM::MOV_ERP3_EAX1			:
		case IWzASM::MOV_ERP3_EAX2			:
		case IWzASM::MOV_ERP3_EAX3			:
		case IWzASM::MOV_ERP3_EBX0			:
		case IWzASM::MOV_ERP3_EBX1			:
		case IWzASM::MOV_ERP3_EBX2			:
		case IWzASM::MOV_ERP3_EBX3			:

		case IWzASM::MOV_EAX_EOP				:
		case IWzASM::MOV_EBX_EOP				:
		case IWzASM::MOV_ECX_EOP				:
		case IWzASM::MOV_EDX_EOP				:
		case IWzASM::MOV_ERP_EOP				:
		case IWzASM::MOV_EOP_EAX				:
		case IWzASM::MOV_EOP_EBX				:
		case IWzASM::MOV_EOP_ECX				:
		case IWzASM::MOV_EOP_EDX				:
		case IWzASM::MOV_EOP_ERP				:
		case IWzASM::MOV_EOP_OFF			:

		case IWzASM::MOV_EFP_EAX				:
		case IWzASM::MOV_EFP_EBX				:
		case IWzASM::MOV_EFP_ECX				:
		case IWzASM::MOV_EFP_EDX				:
		case IWzASM::MOV_EFP_ERP				:

		case IWzASM::MOV_EAXE_EABCDX			:
		case IWzASM::MOV_EBXE_EABCDX			:
		case IWzASM::MOV_ECXE_EABCDX			:
		case IWzASM::MOV_EDXE_EABCDX			:
		case IWzASM::MOV_ERPE_EABCDX			:

		case IWzASM::MOV_EABCDX_EAXE			:
		case IWzASM::MOV_EABCDX_EBXE			:
		case IWzASM::MOV_EABCDX_ECXE			:
		case IWzASM::MOV_EABCDX_EDXE			:
		case IWzASM::MOV_EABCDX_ERPE			:

		case IWzASM::MOVP_EAX_B1				:
		case IWzASM::MOVP_EAX_B2				:
		case IWzASM::MOVP_EAX_B4				:
		case IWzASM::MOVP_EAX_EBX			:
		case IWzASM::MOVP_EAX_BX				:
		case IWzASM::MOVP_EAX_BL				:
		case IWzASM::MOVP_EAX_ECX			:
		case IWzASM::MOVP_EAX_CX				:
		case IWzASM::MOVP_EAX_CL				:
		case IWzASM::MOVP_EAX_EDX			:
		case IWzASM::MOVP_EAX_DX				:
		case IWzASM::MOVP_EAX_DL				:
		case IWzASM::MOVP_EAX_ERP			:
		case IWzASM::MOVP_EAX_PX				:
		case IWzASM::MOVP_EAX_PL				:

		case IWzASM::MOVP_EBX_B1				:
		case IWzASM::MOVP_EBX_B2				:
		case IWzASM::MOVP_EBX_B4				:
		case IWzASM::MOVP_EBX_EAX			:
		case IWzASM::MOVP_EBX_AX				:
		case IWzASM::MOVP_EBX_AL				:
		case IWzASM::MOVP_EBX_ECX			:
		case IWzASM::MOVP_EBX_CX				:
		case IWzASM::MOVP_EBX_CL				:
		case IWzASM::MOVP_EBX_EDX			:
		case IWzASM::MOVP_EBX_DX				:
		case IWzASM::MOVP_EBX_DL				:
		case IWzASM::MOVP_EBX_ERP			:
		case IWzASM::MOVP_EBX_PX				:
		case IWzASM::MOVP_EBX_PL				:

		case IWzASM::MOVP_ECX_B1				:
		case IWzASM::MOVP_ECX_B2				:
		case IWzASM::MOVP_ECX_B4				:
		case IWzASM::MOVP_ECX_EAX			:
		case IWzASM::MOVP_ECX_AX				:
		case IWzASM::MOVP_ECX_AL				:
		case IWzASM::MOVP_ECX_EBX			:
		case IWzASM::MOVP_ECX_BX				:
		case IWzASM::MOVP_ECX_BL				:
		case IWzASM::MOVP_ECX_EDX			:
		case IWzASM::MOVP_ECX_DX				:
		case IWzASM::MOVP_ECX_DL				:
		case IWzASM::MOVP_ECX_ERP			:
		case IWzASM::MOVP_ECX_PX				:
		case IWzASM::MOVP_ECX_PL				:

		case IWzASM::MOVP_EDX_B1				:
		case IWzASM::MOVP_EDX_B2				:
		case IWzASM::MOVP_EDX_B4				:
		case IWzASM::MOVP_EDX_EAX			:
		case IWzASM::MOVP_EDX_AX				:
		case IWzASM::MOVP_EDX_AL				:
		case IWzASM::MOVP_EDX_EBX			:
		case IWzASM::MOVP_EDX_BX				:
		case IWzASM::MOVP_EDX_BL				:
		case IWzASM::MOVP_EDX_ECX			:
		case IWzASM::MOVP_EDX_CX				:
		case IWzASM::MOVP_EDX_CL				:
		case IWzASM::MOVP_EDX_ERP			:
		case IWzASM::MOVP_EDX_PX				:
		case IWzASM::MOVP_EDX_PL				:

		case IWzASM::MOVP_ERP_B1				:
		case IWzASM::MOVP_ERP_B2				:
		case IWzASM::MOVP_ERP_B4				:
		case IWzASM::MOVP_ERP_EAX			:
		case IWzASM::MOVP_ERP_AX				:
		case IWzASM::MOVP_ERP_AL				:
		case IWzASM::MOVP_ERP_EBX			:
		case IWzASM::MOVP_ERP_BX				:
		case IWzASM::MOVP_ERP_BL				:
		case IWzASM::MOVP_ERP_ECX			:
		case IWzASM::MOVP_ERP_CX				:
		case IWzASM::MOVP_ERP_CL				:
		case IWzASM::MOVP_ERP_EDX			:
		case IWzASM::MOVP_ERP_DX				:
		case IWzASM::MOVP_ERP_DL				:

		case IWzASM::MOVP_EOP_B1				:
		case IWzASM::MOVP_EOP_B2				:
		case IWzASM::MOVP_EOP_B4				:
		case IWzASM::MOVP_EOP_EAX			:
		case IWzASM::MOVP_EOP_AX				:
		case IWzASM::MOVP_EOP_AL				:
		case IWzASM::MOVP_EOP_EBX			:
		case IWzASM::MOVP_EOP_BX				:
		case IWzASM::MOVP_EOP_BL				:
		case IWzASM::MOVP_EOP_ECX			:
		case IWzASM::MOVP_EOP_CX				:
		case IWzASM::MOVP_EOP_CL				:
		case IWzASM::MOVP_EOP_EDX			:
		case IWzASM::MOVP_EOP_DX				:
		case IWzASM::MOVP_EOP_DL				:
		case IWzASM::MOVP_EOP_ERP			:
		case IWzASM::MOVP_EOP_PX				:
		case IWzASM::MOVP_EOP_PL				:

		case IWzASM::MOVX_EAX_EAX			:
		case IWzASM::MOVX_EAX_AX				:
		case IWzASM::MOVX_EAX_AL				:
		case IWzASM::MOVX_EAX_EBX			:
		case IWzASM::MOVX_EAX_BX				:
		case IWzASM::MOVX_EAX_BL				:
		case IWzASM::MOVX_EAX_ECX			:
		case IWzASM::MOVX_EAX_CX				:
		case IWzASM::MOVX_EAX_CL				:
		case IWzASM::MOVX_EAX_EDX			:
		case IWzASM::MOVX_EAX_DX				:
		case IWzASM::MOVX_EAX_DL				:
		case IWzASM::MOVX_EAX_ERP			:
		case IWzASM::MOVX_EAX_PX				:
		case IWzASM::MOVX_EAX_PL				:
		case IWzASM::MOVX_EAX_EOP			:

		case IWzASM::MOVX_EBX_EAX			:
		case IWzASM::MOVX_EBX_AX				:
		case IWzASM::MOVX_EBX_AL				:
		case IWzASM::MOVX_EBX_EBX			:
		case IWzASM::MOVX_EBX_BX				:
		case IWzASM::MOVX_EBX_BL				:
		case IWzASM::MOVX_EBX_ECX			:
		case IWzASM::MOVX_EBX_CX				:
		case IWzASM::MOVX_EBX_CL				:
		case IWzASM::MOVX_EBX_EDX			:
		case IWzASM::MOVX_EBX_DX				:
		case IWzASM::MOVX_EBX_DL				:
		case IWzASM::MOVX_EBX_ERP			:
		case IWzASM::MOVX_EBX_PX				:
		case IWzASM::MOVX_EBX_PL				:
		case IWzASM::MOVX_EBX_EOP			:

		case IWzASM::MOVX_ECX_EAX			:
		case IWzASM::MOVX_ECX_AX				:
		case IWzASM::MOVX_ECX_AL				:
		case IWzASM::MOVX_ECX_EBX			:
		case IWzASM::MOVX_ECX_BX				:
		case IWzASM::MOVX_ECX_BL				:
		case IWzASM::MOVX_ECX_ECX			:
		case IWzASM::MOVX_ECX_CX				:
		case IWzASM::MOVX_ECX_CL				:
		case IWzASM::MOVX_ECX_EDX			:
		case IWzASM::MOVX_ECX_DX				:
		case IWzASM::MOVX_ECX_DL				:
		case IWzASM::MOVX_ECX_ERP			:
		case IWzASM::MOVX_ECX_PX				:
		case IWzASM::MOVX_ECX_PL				:
		case IWzASM::MOVX_ECX_EOP			:

		case IWzASM::MOVX_EDX_EAX			:
		case IWzASM::MOVX_EDX_AX				:
		case IWzASM::MOVX_EDX_AL				:
		case IWzASM::MOVX_EDX_EBX			:
		case IWzASM::MOVX_EDX_BX				:
		case IWzASM::MOVX_EDX_BL				:
		case IWzASM::MOVX_EDX_ECX			:
		case IWzASM::MOVX_EDX_CX				:
		case IWzASM::MOVX_EDX_CL				:
		case IWzASM::MOVX_EDX_EDX			:
		case IWzASM::MOVX_EDX_DX				:
		case IWzASM::MOVX_EDX_DL				:
		case IWzASM::MOVX_EDX_ERP			:
		case IWzASM::MOVX_EDX_PX				:
		case IWzASM::MOVX_EDX_PL				:
		case IWzASM::MOVX_EDX_EOP			:

		case IWzASM::PUSH_EAX				:
		case IWzASM::PUSH_EBX				:
		case IWzASM::PUSH_ECX				:
		case IWzASM::PUSH_EDX				:
		case IWzASM::PUSH_ERP				:
		case IWzASM::PUSH_EFP				:
		case IWzASM::PUSH_EOP				:

		case IWzASM::PUSH_GPR				:
		case IWzASM::PUSH_EABCDX				:

		case IWzASM::PUSH_B1					:
		case IWzASM::PUSH_B2					:
		case IWzASM::PUSH_B4					:
		case IWzASM::PUSH_HASH				:

		case IWzASM::POP_EAX					:
		case IWzASM::POP_EBX					:
		case IWzASM::POP_ECX					:
		case IWzASM::POP_EDX					:
		case IWzASM::POP_ERP					:
		case IWzASM::POP_EFP					:
		case IWzASM::POP_EOP					:

		case IWzASM::POP_GPR					:
		case IWzASM::POP_EABCDX				:

// 피연산 대상 및 결과 공간은 EAX
		case IWzASM::CLR_EAX					:
		case IWzASM::CLR_EBX					:
		case IWzASM::CLR_ECX					:
		case IWzASM::CLR_EDX					:
		case IWzASM::CLR_ERP					:
		case IWzASM::CLR_EOP					:
		case IWzASM::CLR_EABCDX				:

		case IWzASM::XOR_EAX_EAX				:
		case IWzASM::XOR_EAX_EBX				:
		case IWzASM::XOR_EAX_ECX				:
		case IWzASM::XOR_EAX_EDX				:
		case IWzASM::AND_EAX_EBX				:
		case IWzASM::AND_EAX_ECX				:
		case IWzASM::AND_EAX_EDX				:
		case IWzASM::OR_EAX_EBX				:
		case IWzASM::OR_EAX_ECX				:
		case IWzASM::OR_EAX_EDX				:
		case IWzASM::NOT_EAX_EAX				:
		case IWzASM::NOT_EAX_EBX				:
		case IWzASM::NOT_EAX_ECX				:
		case IWzASM::NOT_EAX_EDX				:
		case IWzASM::CMPL_EAX_EAX			:
		case IWzASM::CMPL_EAX_EBX			:
		case IWzASM::CMPL_EAX_ECX			:
		case IWzASM::CMPL_EAX_EDX			:

		case IWzASM::SHR_EAX_B1				:
		case IWzASM::SHR_EAX_EBX				:
		case IWzASM::SHR_EAX_ECX				:
		case IWzASM::SHR_EAX_EDX				:
		case IWzASM::SHL_EAX_B1				:
		case IWzASM::SHL_EAX_EBX				:
		case IWzASM::SHL_EAX_ECX				:
		case IWzASM::SHL_EAX_EDX				:

		case IWzASM::ADD_EAX					:
		case IWzASM::ADD_EAX_EBX				:
		case IWzASM::ADD_EAX_B1				:
		case IWzASM::ADD_EAX_B2				:
		case IWzASM::ADD_EAX_B4				:

		case IWzASM::ADD_EBX_EAX				:
		case IWzASM::ADD_EBX_B1				:
		case IWzASM::ADD_EBX_B2				:
		case IWzASM::ADD_EBX_B4				:

		case IWzASM::ADD_ECX_EAX				:
		case IWzASM::ADD_ECX_EBX				:
		case IWzASM::ADD_ECX_EDX				:

		case IWzASM::ADD_ECX_B2				:
		case IWzASM::ADD_EDX_B2				:

		case IWzASM::SUB_ECX_EAX				:
		case IWzASM::SUB_ECX_EBX				:
		case IWzASM::SUB_ECX_EDX				:

		case IWzASM::MUL_ECX_EAX				:
		case IWzASM::MUL_ECX_EBX				:
		case IWzASM::MUL_ECX_EDX				:

		case IWzASM::DIV_ECX_EAX				:
		case IWzASM::DIV_ECX_EBX				:
		case IWzASM::DIV_ECX_EDX				:

		case IWzASM::SUB_EAX					:
		case IWzASM::SUB_EAX_EBX				:
		case IWzASM::SUB_EAX_B1				:
		case IWzASM::SUB_EAX_B2				:
		case IWzASM::SUB_EAX_B4				:

		case IWzASM::SUB_EBX_EAX				:
		case IWzASM::SUB_EBX_B1				:
		case IWzASM::SUB_EBX_B2				:
		case IWzASM::SUB_EBX_B4				:

		case IWzASM::SUB_ECX_B2				:
		case IWzASM::SUB_EDX_B2				:

		case IWzASM::MUL_EAX					:
		case IWzASM::MUL_EAX_EBX				:
		case IWzASM::MUL_EAX_B1				:
		case IWzASM::MUL_EAX_B2				:
		case IWzASM::MUL_EAX_B4				:

		case IWzASM::DIV_EAX					:
		case IWzASM::DIV_EAX_EBX				:
		case IWzASM::DIV_EAX_B1				:
		case IWzASM::DIV_EAX_B2				:
		case IWzASM::DIV_EAX_B4				:

		case IWzASM::INC_EAX					:
		case IWzASM::INC_EBX					:

		case IWzASM::DEC_EAX					:
		case IWzASM::DEC_EBX					:

		case IWzASM::TEST_EAX				:
		case IWzASM::TEST_EBX				:
		case IWzASM::TEST_ECX				:
		case IWzASM::TEST_EDX				:
		case IWzASM::TEST_ERP				:
		case IWzASM::TEST_EOP				:

		case IWzASM::CMP_EAX_EBX				:
		case IWzASM::CMP_EAX_ECX				:
		case IWzASM::CMP_EAX_EDX				:
		case IWzASM::CMP_EAX_ERP				:

		case IWzASM::CMP_ECX_EAX				:
		case IWzASM::CMP_ECX_EBX				:
		case IWzASM::CMP_ECX_EDX				:
		case IWzASM::CMP_ECX_ERP				:

		case IWzASM::CMP_EAX_B1				:
		case IWzASM::CMP_EAX_B2				:
		case IWzASM::CMP_EAX_B4				:
		case IWzASM::CMP_ECX_B1				:
		case IWzASM::CMP_ECX_B2				:
		case IWzASM::CMP_ECX_B4				:

		case IWzASM::JZEROEQUAL				:
		case IWzASM::JZERONOTEQUAL			:

		case IWzASM::JBELOW					:
		case IWzASM::JNOTBELOW				:
		case IWzASM::JABOVE					:
		case IWzASM::JNOTABOVE				:
		case IWzASM::JEQUAL					:
		case IWzASM::JNOTEQUAL				:

		case IWzASM::JUMP					:
		case IWzASM::JUMP1					:
		case IWzASM::JUMP2					:
		case IWzASM::JUMP3					:
		case IWzASM::JUMP4					:
		case IWzASM::JUMP5					:
		case IWzASM::JUMP6					:
		case IWzASM::JUMP7					:
		case IWzASM::JUMP8					:
		case IWzASM::JUMP9					:
		case IWzASM::JUMP10					:

		case IWzASM::JUMP11					:
		case IWzASM::JUMP12					:
		case IWzASM::JUMP13					:
		case IWzASM::JUMP14					:
		case IWzASM::JUMP15					:
		case IWzASM::JUMP16					:
		case IWzASM::JUMP17					:
		case IWzASM::JUMP18					:
		case IWzASM::JUMP19					:


//		case IWzASM::LEA_ECX					:
//		case IWzASM::LEA_ECX_HASH			:
//		case IWzASM::LEA_EDX					:
//		case IWzASM::LEA_EDX_HASH			:
		case IWzASM::LEA_EFP_HASH			:
		case IWzASM::LEA_REF_HASH			:

		case IWzASM::LEA_EAX_EOP				:
		case IWzASM::LEA_EBX_EOP				:
		case IWzASM::LEA_ECX_EOP				:
		case IWzASM::LEA_EDX_EOP				:
		case IWzASM::LEA_EOP_HASH			:
		case IWzASM::LEA_EOP_B4				:
		case IWzASM::LEA_EOP_HASHB4			:
		case IWzASM::LEA_EOP_EXA2B4			:

		case IWzASM::CALL_RC					:
//		case IWzASM::CALL_RD					:
		case IWzASM::CALL_IBF				:
		case IWzASM::CALL_IBF_EFP			:
		case IWzASM::RET						:

		case IWzASM::MARK01					:
		case IWzASM::MARK02					:
		case IWzASM::MARK03					:
		case IWzASM::MARK04					:
		case IWzASM::MARK05					:
		case IWzASM::MARK06					:
		case IWzASM::MARK07					:
		case IWzASM::MARK08					:
		case IWzASM::MARK09					:
		case IWzASM::MARK10					:

// TEMPLATE으로 구성된 명령어 집합에
		case IWzASM::MARK11					:
		case IWzASM::MARK12					:
		case IWzASM::MARK13					:
		case IWzASM::MARK14					:
		case IWzASM::MARK15					:
		case IWzASM::MARK16					:
		case IWzASM::MARK17					:
		case IWzASM::MARK18					:
		case IWzASM::MARK19					:
			break;
	};
}