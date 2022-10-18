//----------------------------------------------------------------------------
/**
@desc : ��ų ����Ʈ ��ũ��Ʈ
@author : ������
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
	tagWZID			    m_wzidEffectCode;	         //���۽� ��ų ����Ʈ �ڵ�
	BOOL                m_bChase;                    //ĳ���͸� ���� �ٴҰ��ΰ� ����  
	DWORD               m_dwLifeTime;				 //���� ����Ʈ ������ Ÿ�� 		
	eSKILLEFFECT_POS    m_dwPositionType;            //���� ����Ʈ ������ Ÿ��  
	WORD                m_wBoneIndex;                //������ Ÿ���� ���� ��� �� ��� ����� �� �ε��� 
	BOOL                m_bRot;				         //ȸ�� ���� ���� �Ұ� �ΰ� 
};

struct SkillEffectInfo 
{
	SLOTCODE		m_SkillClassCode;				          //��ų Ŭ���� �ڵ�
	char  			m_szSkillName[MAX_SKILLNAME_LENGTH];      //��ų �̸�
	BYTE			m_byWeaponType;							  //���� Ÿ��
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
