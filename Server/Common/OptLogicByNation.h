#pragma once

//=============================================================================================================================
/// OptLogicByNation Ŭ����
/**
	@author	Waverix Lee <waverix@webzen.co.kr>
	@since	2007. 05. 06
	@remark
		- �������� �ʰ� ��ũ��Ʈ�� ���� ����� ���� ��ġ�� ������ �������� Ʋ���� ��� �װ��� ������ �� ���� �ڵ忡
		  �����ϱ� ���ٴ� �ش� �ڵ忡���� �׳� Ư�� �Լ����� ȣ�������ν� ������ �޶��� �� �ֵ��� ����
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
	// PC �� ������ �־�� �ϴ��� ����
	inline BOOL		CanApplyPCBangBenefit() { return m_INFO.m_bCanApplyPCBangBenefit; }

private:	
	struct sOptInfo
	{
		BOOL	m_bCanApplyPCBangBenefit;	// PC �� ������ �־�� �ϴ��� ����
	} m_INFO;
	friend class OptLogicByNation;
};

class OptLogicGame
{
public:	
	OptLogicGame() {}
	~OptLogicGame() {}

public:		
	// PC �� ������ �־�� �ϴ��� ����
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
		BOOL	m_bCanApplyPCBangBenefit;	// PC �� ������ �־�� �ϴ��� ����
		FLOAT	m_fMasterBonusExp;			// ���۹��� ���� �߰� ����ġ
		FLOAT	m_fNoMasterBonusExp;		// ���۹��� ��Ƽ�� �߰� ����ġ 
		BOOL	m_bCheckLevelOwnership;		// �����Ǽ����� �������̸� üũ�ϴ���
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
