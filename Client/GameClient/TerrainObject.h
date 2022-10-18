//------------------------------------------------------------------------------
/** TerrainObject

    @author
		±èÁö¿Ï
    @since

    @remarks
        - 
*/
#pragma once
#include "mapobject.h"


class TerrainObject
	: public MapObject
{
public:
	TerrainObject();
	virtual ~TerrainObject();

	BOOL    IsFallDown();
	void    SetFallDown(BOOL bStart);

	BOOL    AddTileIndex(int iIndex);

private:
	virtual BOOL	Create(DWORD dwObjectKey, APPEARANCEINFO * pAppearInfo);
	virtual void	Destroy( void );

	virtual BOOL	Render();
	virtual void    RenderInfo();
	virtual	void    RenderFocus();
	virtual WzID	GetCurrentIdleAnimation();


			

			BOOL    AddFallDownPlayer(DWORD dwId);
			BOOL    RemoveAllFallDownPlayer();

			BOOL    DeleteTileIndex(int iIndex);
			void    DeleteAllTileIndex();
	inline	int     GetTileNum(){ return m_iTileIndexArray.Size();}
	inline  int     GetTileIndex(int iIndex){return m_iTileIndexArray[iIndex];}

private:
	nArray<int>     m_iTileIndexArray;
	nArray<DWORD>   m_dwPlayerIdArray;

	BOOL            m_bStartFallDown;
};
