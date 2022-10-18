#pragma once

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
// <DECLARATION>
#define MAX_STREAM_KEY_BLOCK	256

#pragma pack(push, 1)
#pragma warning(push)
#pragma warning(disable:4200)

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<1010 STRUCTUREs>
union uSTREAM_KEY_BLOCK
{
	struct
	{
		BYTE		blank0[HEX_KEY_1_GENERATOR_008+1];
		BYTE		m_byResKey1;
		BYTE		blank1[OCT_KEY_1_GENERATOR_001+1];
		BYTE		m_byResKeyThreeQuater[3];
		BYTE		blank2[OCT_KEY_1_GENERATOR_006+1];
		HSEL_INITIAL	m_HSEL_ENC_KEY_BLOCK;
		BYTE		blank3[OCT_KEY_1_GENERATOR_002+1];
		DWORD		m_dwUserKey;
		BYTE		blank4[HEX_KEY_1_GENERATOR_003+1];
		WORD		m_wResKey2;
		BYTE		blank5[OCT_KEY_1_GENERATOR_004+1];
		BYTE		m_byStartIDX;
		BYTE		blank6[HEX_KEY_1_GENERATOR_005+1];
		DWORD		m_byResKey3;
		HSEL_INITIAL	m_HSEL_DEC_KEY_BLOCK;
	};
	BYTE	m_byCryptData[MAX_STREAM_KEY_BLOCK];
};

struct sSECURE_PACKET_INFO_BASE
{
	BYTE		blank0[DWORD_KEY_1_GENERATOR_001+1];
	BYTE		bySecureType;
};

#pragma warning(pop)
#pragma pack(pop)


