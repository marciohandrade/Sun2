#pragma once
#ifdef _NA_008334_20150608_SONNENSCHEIN

#include "scenebase.h"
#include "FieldScene.h"

class SonnenScheinScene : public FieldScene
{
public:
	SonnenScheinScene();
	virtual ~SonnenScheinScene();

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

extern SonnenScheinScene g_SonnenScheinScene;


#endif //_NA_008334_20150608_SONNENSCHEIN