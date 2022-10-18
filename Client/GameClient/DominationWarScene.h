#ifndef _DominationWarScene_h_
#define _DominationWarScene_h_

#include "scenebase.h"
#include "FieldScene.h"

class DominationWarScene : public FieldScene
{
public:
	DominationWarScene();
	virtual ~DominationWarScene();

	virtual BOOL		Init();
	virtual BOOL		Process( DWORD dwTick );		
	virtual void		Release();

	virtual void		ParsePacket( MSG_BASE * pMsg );
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    virtual BOOL        PostInterfaceRender(DWORD tick);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	virtual RELATIONKIND::TYPE GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy );

	BOOL  m_bOldShadowVal;
	//BOOL  m_bOldTileBlendingVal;
	int   m_iOldEffectLevel;
	float m_fOldCullingRange;
};

extern DominationWarScene g_DominationWarScene;


#endif // _DominationWarScene_h_
