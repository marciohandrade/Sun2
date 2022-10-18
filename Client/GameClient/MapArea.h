#pragma once

class AREA_INFO
{
public:
	AREA_INFO():byBoundingType(0), byAreaType(0), wzAreaID(0){}
	virtual ~AREA_INFO(){}

	enum { AREA_AABB = 0, AREA_TILE = 1 };
	enum { AREA_TYPE_NORMAL =0, AREA_TYPE_START, AREA_TYPE_WARP, };
	BYTE				byBoundingType;
	WzID				wzAreaID;
	BYTE				byAreaType;
};

class AABB_AREA_INFO : public AREA_INFO
{
public:
	AABB_AREA_INFO(){}
	virtual ~AABB_AREA_INFO(){}

	BvAABB	bvAABB;
};

class TILE_AREA_INFO : public AREA_INFO
{
public:
	TILE_AREA_INFO():wNumberOfTiles(0),piTileIndex(NULL) {}
	virtual ~TILE_AREA_INFO(){ if( piTileIndex ) delete [] piTileIndex; }
	WORD				wNumberOfTiles;	
	int *				piTileIndex;
};