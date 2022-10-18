#pragma once
//=============================================================================================================================
/// VersionControl class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 11. 28
	@purpose
		- SolarSlot의 ConvertStream()을 외부 로직에서 버전 처리하기 위한 클래스
	@note
		- 
	@history 
		- 
*/
//=============================================================================================================================
struct QUESTPART_PROGR_V1;
struct QUESTPART_PROGR_V0;

struct QUESTPART_COMPL_V1;
struct QUESTPART_COMPL_V0;

struct QUESTPART_PROGR_V2;
struct QUESTPART_COMPL_V2;

class VersionControl
{
public:
	VersionControl(){}
	~VersionControl(){}

    static bool ConvertItem(int* converted_version, void* converted_stream,
                            const int from_version, const void* from_stream);
	// 인벤토리 스트림 버전 변환
	static BOOL	ItemConverter( SLOTIDX SlotIdx, int UsedSlotNum IN, int iFromVersion IN, BYTE * IN pFromStream IN, int & iToVersion OUT, BYTE * pToStream OUT );

    //designed by __NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
	static BOOL QuestConverter( OUT int& iDstVersion, OUT BYTE* pDstStreamProgr, OUT BYTE* pDstStreamCompl,
        const int iSrcVersion, const BYTE* pSrcStreamProgr, const BYTE* pSrcStreamCompl );

	static BOOL _QuestProgrConvert( OUT QUESTPART_PROGR_V1* pDstStreamProgr, const QUESTPART_PROGR_V0* pSrcStreamProgr );
	static BOOL _QuestComplConvert( OUT QUESTPART_COMPL_V1* pDstStreamCompl, const QUESTPART_COMPL_V0* pSrcStreamCompl );

    static BOOL _QuestProgrConvert( OUT QUESTPART_PROGR_V2* pDstStreamProgr, const QUESTPART_PROGR_V1* pSrcStreamProgr );
    static BOOL _QuestComplConvert( OUT QUESTPART_COMPL_V2* pDstStreamCompl, const QUESTPART_COMPL_V1* pSrcStreamCompl );
};