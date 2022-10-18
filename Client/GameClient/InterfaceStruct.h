//------------------------------------------------------------------------------
/**

    인터페이스 관련 struct 를 모아 두는곳 

    @author
		유재영
    @since

    @remarks
        -
        - 

*/

#ifndef INTERFACESTRUCT_H
#define INTERFACESTRUCT_H

struct sScope
{
	int iMin;
	int iMax;
};

struct sScrollInfo
{
	int			iStartDrawIndex;
	int			iCurPos;
	sScope		ScrollRange;
};

#define MAX_MESSAGE_LINE			15
#define MAX_MISSION_AREA_LINE		2
#define MAX_SHOWMESSAGE_LENGTH		1024
#define INTERFACE_STRING_LENGTH		2048

enum eDAMAGEPOPUP_KIND
{
	eDAMAGEPOPUP_ENEMYHPDAMAGE = 0,
	eDAMAGEPOPUP_ENEMYMPDAMAGE,
	eDAMAGEPOPUP_PLAYERHPDAMAGE,
	eDAMAGEPOPUP_PLAYERMPDAMAGE,
	eDAMAGEPOPUP_HEAL,
	eDAMAGEPOPUP_EXP,
	eDAMAGEPOPUP_MISS,
	eDAMAGEPOPUP_BLOCK,
	eDAMAGEPOPUP_RELOAD,
	eDAMAGEPOPUP_MAX
	
};


typedef struct _DAMAGE_POPUP
{
	WzVector vPos;
	DWORD    dTime;
	DWORD	 SkillCode;
	BOOL	 bHero;	// 나 자신이냐?
	INT64      iNumber;	
	BOOL     bCritical;
	eDAMAGEPOPUP_KIND eKind;
	WzColor  Color;
	int      iNumber2;
	WzColor  Color2;
	float    fMultiplier;
	BOOL	 bDoubleDamage;
    BYTE	 arNumber[30];
    BYTE	 byNumberCount;    
    int	     iMeshCount;

} DAMAGE_POPUP;

struct BASE_STATEINFO;
class SKILL_EFFECT;
typedef struct _SKILL_EFFECT_IMAGE
{

	int iEffectID;
	DWORD dwStatusID;
	DWORD dwOwnerID;
    SKILL_EFFECT* pSkillEffect;
    BASE_STATEINFO* pStateInfo;
    RECT rcDrawRect;
    BYTE m_byAlpha;   
} SKILL_EFFECT_IMAGE;


typedef struct _POPUPMESH_SIZE
{
	float fWidth;
	float fHeight;
} POPUPMESH_SIZE;
#endif 
// INTERFACESTRUCT_H