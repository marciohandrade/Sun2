/*!
 *	\file		ssqscene.h file
 *	\brief		타락한 사원 씬 파일
 *	\author		김경표 (lammy@webzen.co.kr)
 *	\version	2009/01/12	김경표 - doxygen command 작성
 */

#pragma once

#include "scenebase.h"
#include "battlescene.h"
#include <ssq/ssqinfostruct.h>

class TerrainObject;

/*!
 *	\class		ssqscene class
 *	\brief		타락한 사원 씬 관련 클래스
 *	\author		김경표 (lammy@webzen.co.kr)
 *	\version	2009/01/12	김경표 - doxygen command 작성
 */

class SsqScene : public BattleScene
{
public:
	/// 생성자
	SsqScene(void);
	/// 소멸자
	virtual ~SsqScene(void);

	///	초기화, 로딩
	virtual BOOL		Init( void );
	/// 메인 계산
	virtual BOOL		Process( DWORD dwTick );		
	///	종료
	virtual void		Release( void );
    /// 네트워크 패킷 처리
	virtual void		ParsePacket( MSG_BASE * pMsg );
	virtual void		OnRecvCG_STATUS( MSG_BASE * pMsg );

	///	히어로 관련 인풋 처리 
	virtual void		ProcessKeyInput_Hero( DWORD dwTick );

	///	지진 조짐을 보일 타일을 추가한다.
	/*! 
	 *	\param[in]	index	타일 인덱스
	 */
	void				AddEarthQuakeIndex( int index );

	///	조짐을 보이던 타일에 지진을 진행시킨다.
	void				ProcessEarthQuake();

	///	인덱스로 터레인 오브젝트를 얻어온다
	/*! 
	 *	\param[in]	index	타일 인덱스
	 *	\return		성공	TerrainObject 리턴
	 *  \return		실패	NULL 리턴
	 */
	TerrainObject *		FindTerrainObject( int index );

	///	번개를 생성한다.
	/*! 
	 *	\param[in]	pos		번개 생성 위치
	 */
	void				CreateThunderBolt( const WzVector & pos );

	///	대미지를 입은 플레이어의 대미지를 계산한다.
	/*! 
	 *	\param[in]	DamageInfo	대미지 관련 정보
	 *	\param[in]	dwSkillCode	스킬 코드
	 */
	void				ProcessDamage( const DAMAGE_INFO & DamageInfo, DWORD dwSkillCode  = 0 );

	///	가운데 둠을 지운다.
	void				DeleteDoomEffect( void );

	///	렌덤하게 타일 포지션을 얻어온다. (싱글용)
	/*! 
	 *	\return		성공	WzVector
	 */
	WzVector			GetRandomPosition();

	///	터레인 오브젝트의 인덱스를 추가한다.
	/*! 
	 *	\param[in]	iIndex	터레인 인덱스
	 */
	void				AddTerrainObjIndex( int iIndex );

	///	ssq씬에서 플레어어간 관계 설정
	/*! 
	 *	\param[in]	pObject		관계를 알아올 Object
	 *	\param[in]	bForceEnemy	강제 공격 설정
	 */
	virtual RELATIONKIND::TYPE GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy );
    bool is_ssq_state();
private:
	///	번개를 칠 시간 (싱글용)
	DWORD			m_dwThunderboltTime;
	/// 붕괴를 일으킬 시간 (싱글용)
	DWORD			m_dwEarthQuakeTime;

	/// 둠 이펙트 핸들
	HANDLE			m_hDoomEffect;

	///	둠을 없애는 시간 체크용 변수 (싱글용)
	DWORD			m_dwOverCoatingTime;

	///	붕괴할 터레인 오브젝트 (싱글용)
	TerrainObject *	m_pTerrain;
	///	터레인 오브젝트 인덱스를 관리하는 배열
	nArray<int>     m_iTerrainObjIndexArray;
	/// 조짐을 보이던 터레인오브젝트를 붕괴 시킬 벡터
	std::vector<int>	m_EarthQuakeVec;
	/// 블러 올드 값
	float			m_fOldBlurRatio;

public:
	///	게임 시작 관련 함수		
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
