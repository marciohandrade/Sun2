#pragma once
//------------------------------------------------------------------------------
/**
	로또 npc

	@author		광수

	@since		09.07.24

	@remarks	09.08.05  소스정리

*/
#include "NPC.h"


//숫자자릿수(15자리)
#define MAX_BONE_POS		15 

//숫자유닛갯수(0~9 + 공백)
#define NUM_MAX_UNIT		11 


//로또리소스
typedef struct _RC_LOTTO_INFO
{
	RESOURCECODE m_rNumberCode;
	RESOURCECODE m_rBaseCode;
}RC_LOTTO_INFO;

class LottoNPC : public NPC
{
public:
	LottoNPC( void );
	virtual ~LottoNPC( void );
 
	virtual BOOL	Create( DWORD dwObjectKey, DWORD dwID = 0 );

	virtual void	Destroy( void );

	virtual BOOL	Process( DWORD dwTick );

	virtual BOOL    Render();

	void			SetDrawNumber( INT64 Number);

	virtual void	SetExtraInfo( EXTRA_NPCINFO	* pExtraInfo);

	//로또숫자판 활성.비활성(추첨중에는 비활성)
	void			SetLottoActive(BOOL bActive);
	
private:
	
	//숫자추출해 배열에 담기
	void SetparateNumber(INT64 Number);

	//숫자갱신(UnitDraw에 Unit셋팅)
	void RefreshNumber();

	//본인덱스 추출
	void ExtractBoneIndex();

	//빌보드 적용
	void SetBillBoard();

	//숫자애니메이션
	void AnimationNumber(DWORD dwTick);

	//숫자애니메이션
	void AnimationNumberChaos(DWORD dwTick);

	//리소스로드
	void LoadNumberObject();	
	
	//리소스해제
	void FreeNumberObject();
	

private:

	//리소스정보
	RC_LOTTO_INFO			m_ResInfo;

	//숫자판때기
	CWzUnitDraw*			m_pBasePart;
	
	//자릿수의 숫자Draw
	CWzUnitDraw*			m_ppRenderPart[MAX_BONE_POS];

	//숫자유닛
	CWzUnit	 *				m_ppNumberUnit[NUM_MAX_UNIT];

	//숫자판때기의 자릿수본위치
	int						m_arBonPosition[MAX_BONE_POS];

	//숫자애니를 위한변수
	int						m_arCurNumber[MAX_BONE_POS];
	int						m_arOldNumber[MAX_BONE_POS];
	INT64					m_iOldNumber;
	BOOL					m_bNumberAni;

	//로또숫자판 활성화상태
	BOOL					m_bLottoActive;

};

