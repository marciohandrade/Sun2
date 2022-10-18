//=======================================================================================================================
/** PVP Scene(맵 에디트 모드)
	@author
			김지완
	@since

	@remarks
*/

#pragma once
#include "battlescene.h"



class MapEditScene : public BattleScene
{
public:

	MapEditScene(void);
	virtual ~MapEditScene(void);
#ifdef MAP_EDIT_MODE 
	virtual BOOL	Init( void );						//	초기화, 로딩
	virtual BOOL	Process( DWORD dwTick );			//	메인 계산
	virtual BOOL	Render( DWORD dwTick );				//	메인 랜더링
	virtual void	Release( void );					//	종료

	virtual void	ProcessKeyInput_Hero(DWORD dwTick);
			void    AddTerrainObjIndex(int iIndex);
			void    SetSelectObject(int iObjKey);
			void    Reset();
			void    ResetSelected();
			void    AutoCalCulate();

	inline  BOOL    IsShowAllTile(){ return m_bShowAllTile; }
	inline  BOOL    IsHideAllObject(){ return m_bHideObject;}
	inline  BOOL    IsShowAllLayer(){ return m_bShowAllLayer;}
	
protected:
			void    RenderEditText();
			void    SaveWAT();

private:
	nArray<int>     m_iTerrainObjIndexArray;
	int             m_iCurSelectObject;
	int             m_iNextSelectObject;
	int             m_iPreSelectObject;
	BOOL            m_bShowAllTile;
	BOOL            m_bHideObject;
	BOOL            m_bShowAllLayer;
#endif 
};

extern MapEditScene		g_mapEditScene;
