#pragma once
//------------------------------------------------------------------------------
/**
	�ζ� npc

	@author		����

	@since		09.07.24

	@remarks	09.08.05  �ҽ�����

*/
#include "NPC.h"


//�����ڸ���(15�ڸ�)
#define MAX_BONE_POS		15 

//�������ְ���(0~9 + ����)
#define NUM_MAX_UNIT		11 


//�ζǸ��ҽ�
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

	//�ζǼ����� Ȱ��.��Ȱ��(��÷�߿��� ��Ȱ��)
	void			SetLottoActive(BOOL bActive);
	
private:
	
	//���������� �迭�� ���
	void SetparateNumber(INT64 Number);

	//���ڰ���(UnitDraw�� Unit����)
	void RefreshNumber();

	//���ε��� ����
	void ExtractBoneIndex();

	//������ ����
	void SetBillBoard();

	//���ھִϸ��̼�
	void AnimationNumber(DWORD dwTick);

	//���ھִϸ��̼�
	void AnimationNumberChaos(DWORD dwTick);

	//���ҽ��ε�
	void LoadNumberObject();	
	
	//���ҽ�����
	void FreeNumberObject();
	

private:

	//���ҽ�����
	RC_LOTTO_INFO			m_ResInfo;

	//�����Ƕ���
	CWzUnitDraw*			m_pBasePart;
	
	//�ڸ����� ����Draw
	CWzUnitDraw*			m_ppRenderPart[MAX_BONE_POS];

	//��������
	CWzUnit	 *				m_ppNumberUnit[NUM_MAX_UNIT];

	//�����Ƕ����� �ڸ�������ġ
	int						m_arBonPosition[MAX_BONE_POS];

	//���ھִϸ� ���Ѻ���
	int						m_arCurNumber[MAX_BONE_POS];
	int						m_arOldNumber[MAX_BONE_POS];
	INT64					m_iOldNumber;
	BOOL					m_bNumberAni;

	//�ζǼ����� Ȱ��ȭ����
	BOOL					m_bLottoActive;

};

