#pragma once
//------------------------------------------------------------------------------
/**
	@author		����

	@since		

	@remarks	
            �������޽� �ѹ��� ��Ƽ� DrawCall�ϱ�
 
*/
using namespace util;

typedef STLX_LIST<DAMAGE_POPUP> DMG_LIST; 

typedef struct _MESH_INFO
{
    WzVector    *m_pVertices;  //���ؽ�
    WzUVVert    *m_pUVVerts;   //���ؽ�uv
    WzColor     *m_pColor;     //���ؽ�color
    int         *m_piIndexMap; //�ε���map

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
    
    //�ؽ���
    HANDLE       m_hNumberTexture;

    //�ִ�鰹��(2�������־���ϳ����ڸ���´�)
    DWORD        m_dwInitMaxFace;

    //�޽�����
    MESH_INFO    m_Meshinfo;

    //����������Ʈ
    DMG_LIST    m_DamageList;
    
    //ī�޶���(right)
    WzVector    m_vDamageMeshRight;

    WzVector    m_vDamageMeshDown;
};
