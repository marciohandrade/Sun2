#pragma once

//=============================================================================================================================
/// OptLogicByNation 클래스
/**
	@author	Waverix Lee <waverix@webzen.co.kr>
	@since	2007. 05. 06
	@remark
		- 빌드하지 않고 스크립트에 의한 제어시 각종 수치나 로직이 국가별로 틀려질 경우 그것을 일일히 각 서버 코드에
		  삽입하기 보다는 해당 코드에서는 그냥 특정 함수만을 호출함으로써 값등이 달라질 수 있도록 지원
	@note
		- 
	@history 
		- 
	@usage
*/
//=============================================================================================================================


#ifndef __NA_000000_20071106_SERVER_OPTION_CHANGE

class OptLogicAgent
{
public:		
	OptLogicAgent() {}
	~OptLogicAgent() {}

public:		
	// PC 방 혜택을 주어야 하는지 여부
	inline BOOL		CanApplyPCBangBenefit() { return m_INFO.m_bCanApplyPCBangBenefit; }

private:	
	struct sOptInfo
	{
		BOOL	m_bCanApplyPCBangBenefit;	// PC 방 혜택을 주어야 하는지 여부
	} m_INFO;
	friend class OptLogicByNation;
};

class OptLogicGame
{
public:	
	OptLogicGame() {}
	~OptLogicGame() {}

public:		
	// PC 방 혜택을 주어야 하는지 여부
	inline BOOL		CanApplyPCBangBenefit() { return m_INFO.m_bCanApplyPCBangBenefit; }
	//
	inline FLOAT	GetSuperRoomBonusExp( BOOL bMaster )	
	{ 
		if( bMaster )	return m_INFO.m_fMasterBonusExp; 
		else			return m_INFO.m_fNoMasterBonusExp; 
	}
	inline BOOL		IsCheckLevelOwnership()	{ return m_INFO.m_bCheckLevelOwnership; }

private:	
	struct sOptInfo
	{
		BOOL	m_bCanApplyPCBangBenefit;	// PC 방 혜택을 주어야 하는지 여부
		FLOAT	m_fMasterBonusExp;			// 수퍼방의 방장 추가 경험치
		FLOAT	m_fNoMasterBonusExp;		// 슈퍼방의 파티원 추가 경험치 
		BOOL	m_bCheckLevelOwnership;		// 소유권설정시 레벨차이를 체크하는지
	} m_INFO;

	friend class OptLogicByNation;
};

class OptLogicWorld
{
public:		
	OptLogicWorld() {}
	~OptLogicWorld() {}

private:	
	struct sOptInfo
	{
	} m_INFO;
	friend class OptLogicByNation;
};

class OptLogicDBProxy
{
public:		OptLogicDBProxy() {}
			~OptLogicDBProxy() {}

private:	
	struct sOptInfo
	{
	} m_INFO;
	friend class OptLogicByNation;
};

class OptLogicGuild
{
public:		OptLogicGuild() {}
			~OptLogicGuild() {}
private:	
	struct sOptInfo
	{
	} m_INFO;
	friend class OptLogicByNation;
};

class OptLogicMaster
{
public:		
	OptLogicMaster() {}
	~OptLogicMaster() {}

private:	
	struct sOptInfo
	{
	} m_INFO;
	friend class OptLogicByNation;
};

//////////////////////////////////////////////////////////////////////////
//
class OptLogicByNation
{

public:		
	static VOID				CreateInstance( eNATION_CODE eNationCode );	
	static VOID				DestroyInstance();
	inline static OptLogicByNation*	Instance() { return ms_pInstance; }

public:		
	OptLogicAgent			AGENT;
	OptLogicGame			GAME;
	OptLogicWorld			WORLD;
	OptLogicDBProxy			DBPROXY;
	OptLogicGuild			GUILD;
	OptLogicMaster			MASTER;

public:		
	inline eNATION_CODE		GetNationCode() { return m_eNationCode; }

private:	
	OptLogicByNation();
	~OptLogicByNation();

private:	
	VOID					_InitializeByNationCode( eNATION_CODE eNationCode );
	VOID					__Initialize__KR();
	VOID					__Initialize__JP();
	VOID					__Initialize__TW();
	VOID					__Initialize__CN();
	VOID					__Initialize__UK();

private:	
	static OptLogicByNation*	ms_pInstance;
	eNATION_CODE				m_eNationCode;
};

#endif //__NA_000000_20071106_SERVER_OPTION_CHANGE
