#pragma once
//------------------------------------------------------------------------------
/**
	@author		광수

	@since		

	@remarks	
            데미지메쉬 한번에 모아서 DrawCall하기
 
*/
using namespace util;

typedef STLX_LIST<DAMAGE_POPUP> DMG_LIST; 

typedef struct _MESH_INFO
{
    WzVector    *m_pVertices;  //버텍스
    WzUVVert    *m_pUVVerts;   //버텍스uv
    WzColor     *m_pColor;     //버텍스color
    int         *m_piIndexMap; //인덱스map

}MESH_INFO;

class DamageMesh
{
public:
	DamageMesh( void );
	
    ~DamageMesh( void );

    void            InitDamage(DWORD dwMaxFace);
    
    void            Release();
	
public:
    void            AddDamageList(DAMAGE_POPUP &popup);

    void            ResetDamageList();

    void            RenderDamageList(DWORD dwTick);

private:
    int             MakeDigitArray(INT64 value,BYTE *digitarray);

    void            MakeDamageMesh(int Index,WzVector &vBasePos,int digitcount,BYTE *digitarray,WzColor color,float fWidth,float fHeight);

    POPUPMESH_SIZE  MakeNumberUVVertice(unsigned int number,WzVector &vBasePos,DWORD index,WzColor color,float fNumberWidth,float fNumberHeight);

    void            MoveVertexMesh(WzVector &vOffset,int iVertexStart,int iVertexCount);
    
private:
    
    //텍스쳐
    HANDLE       m_hNumberTexture;

    //최대면갯수(2개면이있어야하나숫자를찍는다)
    DWORD        m_dwInitMaxFace;

    //메쉬정보
    MESH_INFO    m_Meshinfo;

    //데미지리스트
    DMG_LIST    m_DamageList;
    
    //카메라벡터(right)
    WzVector    m_vDamageMeshRight;

    WzVector    m_vDamageMeshDown;
};
