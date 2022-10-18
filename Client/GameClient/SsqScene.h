/*!
 *	\file		ssqscene.h file
 *	\brief		Ÿ���� ��� �� ����
 *	\author		���ǥ (lammy@webzen.co.kr)
 *	\version	2009/01/12	���ǥ - doxygen command �ۼ�
 */

#pragma once

#include "scenebase.h"
#include "battlescene.h"
#include <ssq/ssqinfostruct.h>

class TerrainObject;

/*!
 *	\class		ssqscene class
 *	\brief		Ÿ���� ��� �� ���� Ŭ����
 *	\author		���ǥ (lammy@webzen.co.kr)
 *	\version	2009/01/12	���ǥ - doxygen command �ۼ�
 */

class SsqScene : public BattleScene
{
public:
	/// ������
	SsqScene(void);
	/// �Ҹ���
	virtual ~SsqScene(void);

	///	�ʱ�ȭ, �ε�
	virtual BOOL		Init( void );
	/// ���� ���
	virtual BOOL		Process( DWORD dwTick );		
	///	����
	virtual void		Release( void );
    /// ��Ʈ��ũ ��Ŷ ó��
	virtual void		ParsePacket( MSG_BASE * pMsg );
	virtual void		OnRecvCG_STATUS( MSG_BASE * pMsg );

	///	����� ���� ��ǲ ó�� 
	virtual void		ProcessKeyInput_Hero( DWORD dwTick );

	///	���� ������ ���� Ÿ���� �߰��Ѵ�.
	/*! 
	 *	\param[in]	index	Ÿ�� �ε���
	 */
	void				AddEarthQuakeIndex( int index );

	///	������ ���̴� Ÿ�Ͽ� ������ �����Ų��.
	void				ProcessEarthQuake();

	///	�ε����� �ͷ��� ������Ʈ�� ���´�
	/*! 
	 *	\param[in]	index	Ÿ�� �ε���
	 *	\return		����	TerrainObject ����
	 *  \return		����	NULL ����
	 */
	TerrainObject *		FindTerrainObject( int index );

	///	������ �����Ѵ�.
	/*! 
	 *	\param[in]	pos		���� ���� ��ġ
	 */
	void				CreateThunderBolt( const WzVector & pos );

	///	������� ���� �÷��̾��� ������� ����Ѵ�.
	/*! 
	 *	\param[in]	DamageInfo	����� ���� ����
	 *	\param[in]	dwSkillCode	��ų �ڵ�
	 */
	void				ProcessDamage( const DAMAGE_INFO & DamageInfo, DWORD dwSkillCode  = 0 );

	///	��� ���� �����.
	void				DeleteDoomEffect( void );

	///	�����ϰ� Ÿ�� �������� ���´�. (�̱ۿ�)
	/*! 
	 *	\return		����	WzVector
	 */
	WzVector			GetRandomPosition();

	///	�ͷ��� ������Ʈ�� �ε����� �߰��Ѵ�.
	/*! 
	 *	\param[in]	iIndex	�ͷ��� �ε���
	 */
	void				AddTerrainObjIndex( int iIndex );

	///	ssq������ �÷��� ���� ����
	/*! 
	 *	\param[in]	pObject		���踦 �˾ƿ� Object
	 *	\param[in]	bForceEnemy	���� ���� ����
	 */
	virtual RELATIONKIND::TYPE GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy );
    bool is_ssq_state();
private:
	///	������ ĥ �ð� (�̱ۿ�)
	DWORD			m_dwThunderboltTime;
	/// �ر��� ����ų �ð� (�̱ۿ�)
	DWORD			m_dwEarthQuakeTime;

	/// �� ����Ʈ �ڵ�
	HANDLE			m_hDoomEffect;

	///	���� ���ִ� �ð� üũ�� ���� (�̱ۿ�)
	DWORD			m_dwOverCoatingTime;

	///	�ر��� �ͷ��� ������Ʈ (�̱ۿ�)
	TerrainObject *	m_pTerrain;
	///	�ͷ��� ������Ʈ �ε����� �����ϴ� �迭
	nArray<int>     m_iTerrainObjIndexArray;
	/// ������ ���̴� �ͷ��ο�����Ʈ�� �ر� ��ų ����
	std::vector<int>	m_EarthQuakeVec;
	/// �� �õ� ��
	float			m_fOldBlurRatio;

public:
	///	���� ���� ���� �Լ�		
	void			Func_SendPacket_();
	void			Func_OnRecv_(const MSG_CG_ETC_GET_BATTLE_OPEN_TIME_ACK* pMsg);
	bool			Func_Output_with_internal_timer(TCHAR* szMessage, bool is_only_time = false);
    void            GetSSQTimeInfo(OUT SYSTEMTIME& system_time, OUT nsSSQ::SSQTicketTimeInfo::eTM& current_state);
private:
	void			Func_CSQuery_Check();
	nsSSQ::SSQTicketTimeInfo		m_SSQTicketTimeInfoCtrl;



	BOOL  m_bOldShadowVal;
	int   m_iOldEffectLevel;
};

extern SsqScene		g_SsqScene;
