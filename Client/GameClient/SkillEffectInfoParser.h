//----------------------------------------------------------------------------
/**
@desc : 스킬 이펙트 스크립트
@author : 김지완
@remark :
*/
#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

enum eEffect_Type
{
	eEffect_Type_Start,
	eEffect_Type_End,
	eEffect_Type_Buff,
	eEffect_Type_Damage,
	eEffect_Type_Max,
};



struct EffectInfo 
{
	tagWZID			    m_wzidEffectCode;	         //시작시 스킬 이펙트 코드
	BOOL                m_bChase;                    //캐릭터를 따라 다닐것인가 유무  
	DWORD               m_dwLifeTime;				 //시작 이펙트 라이프 타임 		
	eSKILLEFFECT_POS    m_dwPositionType;            //시작 이펙트 포지션 타입  
	WORD                m_wBoneIndex;                //포지션 타입이 본일 경우 만 사용 대상의 본 인덱스 
	BOOL                m_bRot;				         //회전 값을 적용 할것 인가 
};

struct SkillEffectInfo 
{
	SLOTCODE		m_SkillClassCode;				          //스킬 클래스 코드
	char  			m_szSkillName[MAX_SKILLNAME_LENGTH];      //스킬 이름
	BYTE			m_byWeaponType;							  //웨폰 타입
	EffectInfo      m_EffectInfo[eEffect_Type_Max];
	tagWZID			m_wzidEffectCode1;
	tagWZID			m_wzidEffectCode2;

};


class SkillEffectInfoParser	: public Singleton<SkillEffectInfoParser>, public IParser
{
public:
	SkillEffectInfoParser();
	~SkillEffectInfoParser();

	VOID	            Init(DWORD dwPoolSize);
	VOID	            Init(DWORD dwPoolSize, char * pszPackFileName);
	VOID	            Release();
	virtual	BOOL		LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	SkillEffectInfo*	GetSkillEffectInfo(SLOTCODE SkillClassCode);
   
private:
	BOOL				_Load(BOOL bReload);

	VOID				Unload();

	util::SolarHashTable<SkillEffectInfo *>		*m_pDataTable;
};
