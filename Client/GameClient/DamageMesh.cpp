//------------------------------------------------------------------------------
//  DamageMesh.cpp
//  (C) 2009 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "DamageMesh.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "GameFunc.h"


//�ؽ�����ǥ���� �迭 �ε���
const int          MAX_NUMBER_COORDINATE	=72;
const int          XP_MARK_INDEX = 11;
const int          CRITICAL_MARK_INDEX = 12;
const int          BLOCK_MARK_INDEX = 13;
const int          MISS_MARK_INDEX = 14;
const int          RELOAD_MARK_INDEX = 15;
const int          DOUBLE_MARK_INDEX = 16;
const int          HIT_MARK_INDEX = 17;
const int          LINE_CHANGE_INDEX = 18;

//������ �̵��׶�����Ÿ�Ӱ���
const float        l_fUpSpeed = 0.00080f;
const int          iDamageShrinkTime = 300;
const int          iDamageVanishTime = 300;    
const int          iDamageDeleteTime = 500;

//------------------------------------------------------------------------------
/**  �ؽ��� UV��ǥ
*/
static const float NumberCoordinate[MAX_NUMBER_COORDINATE] = 
{
#ifdef _CHINA
    //left			   //top			     //right			    //bottom
    ( 0.0f / 256.0f),      ( 0.0f / 256.0f),     (18.0f / 256.0f),   (22.0f / 256.0f),	// 0
    (19.0f / 256.0f),      ( 0.0f / 256.0f),     (31.0f / 256.0f),   (22.0f / 256.0f),	// 1
    (32.0f / 256.0f),      ( 0.0f / 256.0f),     (48.0f / 256.0f),   (22.0f / 256.0f),	// 2
    (49.0f / 256.0f),      ( 0.0f / 256.0f),     (65.0f / 256.0f),   (22.0f / 256.0f),	// 3
    (66.0f / 256.0f),      ( 0.0f / 256.0f),     (84.0f / 256.0f),   (22.0f / 256.0f),	// 4
    (85.0f / 256.0f),      (  0.0f / 256.0f),    (101.0f / 256.0f),  (22.0f / 256.0f),	// 5
    (102.0f / 256.0f),     ( 0.0f / 256.0f),     (118.0f / 256.0f),  (22.0f / 256.0f),	// 6
    (119.0f / 256.0f),     ( 0.0f / 256.0f),     (134.0f / 256.0f),  (22.0f / 256.0f),	// 7
    (135.0f / 256.0f),     ( 0.0f / 256.0f),     (152.0f / 256.0f),  (22.0f / 256.0f),	// 8
    (153.0f / 256.0f),     ( 0.0f / 256.0f),     (169.0f / 256.0f),  (22.0f / 256.0f),	// 9
    (170.0f / 256.0f),     ( 0.0f / 256.0f),     (179.0f / 256.0f),  (22.0f / 256.0f),	// !
    (180.0f / 256.0f),     ( 0.0f / 256.0f),     (249.0f / 256.0f),  (22.0f / 256.0f),	// XP
    ( 0.0f / 256.0f),      ( 23.0f / 256.0f),    (91.0f  / 256.0f),  (45.0f / 256.0f),	// Critical
    (105.0f / 256.0f),     ( 23.0f / 256.0f),    (153.0f / 256.0f),  (45.0f / 256.0f),	// Block
    ( 0.0f / 256.0f),      ( 48.0f / 256.0f),    (45.0f / 256.0f),   (69.0f / 256.0f),	// Miss
    (57.0f / 256.0f),      ( 48.0f / 256.0f),    (103.0f / 256.0f),  (69.0f / 256.0f),  // Reload
    (0.0f / 256.0f),       ( 74.0f / 256.0f),    (45.0f / 256.0f),   (95.0f / 256.0f),	// Double
    (57.0f / 256.0f),      ( 74.0f / 256.0f),    (103.0f / 256.0f),  (95.0f / 256.0f),	// Double
#else
    //left			   //top			     //right			    //bottom
    ( 0.0f / 256.0f),      ( 0.0f / 256.0f),     (18.0f / 256.0f),   (22.0f / 256.0f),	// 0
    (19.0f / 256.0f),      ( 0.0f / 256.0f),     (31.0f / 256.0f),   (22.0f / 256.0f),	// 1
    (32.0f / 256.0f),      ( 0.0f / 256.0f),     (48.0f / 256.0f),   (22.0f / 256.0f),	// 2
    (49.0f / 256.0f),      ( 0.0f / 256.0f),     (65.0f / 256.0f),   (22.0f / 256.0f),	// 3
    (66.0f / 256.0f),      ( 0.0f / 256.0f),     (84.0f / 256.0f),   (22.0f / 256.0f),	// 4
    (85.0f / 256.0f),      (  0.0f / 256.0f),    (101.0f / 256.0f),  (22.0f / 256.0f),	// 5
    (102.0f / 256.0f),     ( 0.0f / 256.0f),     (118.0f / 256.0f),  (22.0f / 256.0f),	// 6
    (119.0f / 256.0f),     ( 0.0f / 256.0f),     (134.0f / 256.0f),  (22.0f / 256.0f),	// 7
    (135.0f / 256.0f),     ( 0.0f / 256.0f),     (152.0f / 256.0f),  (22.0f / 256.0f),	// 8
    (153.0f / 256.0f),     ( 0.0f / 256.0f),     (169.0f / 256.0f),  (22.0f / 256.0f),	// 9
    (170.0f / 256.0f),     ( 0.0f / 256.0f),     (179.0f / 256.0f),  (22.0f / 256.0f),	// !
    (180.0f / 256.0f),     ( 0.0f / 256.0f),     (221.0f / 256.0f),  (27.0f / 256.0f),	// XP 	
    ( 0.0f / 256.0f),      ( 28.0f / 256.0f),    (112.0f / 256.0f),  (48.0f / 256.0f),	// Critical
    (114.0f / 256.0f),     ( 28.0f / 256.0f),    (195.0f / 256.0f),  (48.0f / 256.0f),	// Block
    ( 0.0f / 256.0f),      ( 51.0f / 256.0f),    (64.0f / 256.0f),   (73.0f / 256.0f),	// Miss
    (65.0f / 256.0f),      ( 51.0f / 256.0f),    (161.0f / 256.0f),  (74.0f / 256.0f),	// Reload	
    (0.0f / 256.0f),       ( 76.0f / 256.0f),    (89.0f / 256.0f),   (95.0f / 256.0f),	// Double	
    (93.0f / 256.0f),      ( 75.0f / 256.0f),    (137.0f / 256.0f),  (95.0f / 256.0f),	// Double	
#endif // _CHINA
};
//------------------------------------------------------------------------------
/** 
*/
DamageMesh::DamageMesh( void ):
m_dwInitMaxFace(0)
{
    memset(&m_Meshinfo,NULL,sizeof(m_Meshinfo));
    m_hNumberTexture = INVALID_HANDLE_VALUE;
}
//------------------------------------------------------------------------------
/** 
*/
DamageMesh::~DamageMesh( void )
{
    Release();
}
//------------------------------------------------------------------------------
/** �ʱ�ȭ(�ؽ��ķε�)
*/
void DamageMesh::InitDamage(DWORD dwMaxFace)
{
    m_dwInitMaxFace  = dwMaxFace;

    m_Meshinfo.m_pVertices  = new WzVector[m_dwInitMaxFace];   // 1.vertex
    m_Meshinfo.m_pUVVerts   = new WzUVVert[m_dwInitMaxFace];   // 2.uv
    m_Meshinfo.m_pColor     = new WzColor[m_dwInitMaxFace];    // 3.color
    m_Meshinfo.m_piIndexMap = new int[m_dwInitMaxFace];        // 4.idx

    for(unsigned int i=0;i<dwMaxFace;++i)
    {
        m_Meshinfo.m_piIndexMap[i] = i;
    }
    
    m_hNumberTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\number.tga",TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
}
//------------------------------------------------------------------------------
/** ���ҽ�����
*/
void DamageMesh::Release()
{
    if ( m_hNumberTexture != INVALID_HANDLE_VALUE) 
    {
        if(g_pSunRenderer && g_pSunRenderer->x_pManagerTexture)
        {
            g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hNumberTexture );
            m_hNumberTexture = INVALID_HANDLE_VALUE;
        }
    }

    delete [] m_Meshinfo.m_pVertices;  // 1.vertex
    delete [] m_Meshinfo.m_pUVVerts;   // 2.uv
    delete [] m_Meshinfo.m_pColor;     // 3.color
    delete [] m_Meshinfo.m_piIndexMap; // 4.idx
}
//------------------------------------------------------------------------------
/** 
*/
int DamageMesh::MakeDigitArray(INT64 value,BYTE *digitarray)
{
    int iDigitcount = 0;
    // 0�̾ �ѹ��� ������������
    while (value >= 0)
    {
        BYTE number = (BYTE)(value % 10);
        digitarray[iDigitcount] = number;
        value /= 10;
        iDigitcount++;
        if (value == 0) break;
        assert(iDigitcount < 30);
    }

    // ������~
    int startindex = 0;
    int backindex = iDigitcount - 1;

    while(startindex < backindex)
    {
        digitarray[startindex] ^= digitarray[backindex];
        digitarray[backindex] ^= digitarray[startindex];
        digitarray[startindex] ^= digitarray[backindex];

        startindex++;
        backindex--;
    }
    return iDigitcount;
}
//------------------------------------------------------------------------------
/** ������ ����
*/
void DamageMesh::AddDamageList(DAMAGE_POPUP &popup)
{
    WzVector wzVisualPos = popup.vPos;		
    popup.byNumberCount = 1;
    popup.iMeshCount = 2;		

    if( GlobalFunc::IsSSQScene() )
    {
        switch( popup.eKind) 
        {
        case eDAMAGEPOPUP_BLOCK:                                                 // ��   
        case eDAMAGEPOPUP_MISS:   popup.arNumber[0] = MISS_MARK_INDEX;   break;  // �̽�
        case eDAMAGEPOPUP_RELOAD: popup.arNumber[0] = RELOAD_MARK_INDEX; break;  // ���ε�
        default:                  popup.arNumber[0] = HIT_MARK_INDEX;    break;  // ������
        }	
    }
    else
    {
        switch( popup.eKind) 
        {
        case eDAMAGEPOPUP_MISS: popup.arNumber[0] = MISS_MARK_INDEX;      break;  // �̽�
        case eDAMAGEPOPUP_BLOCK:popup.arNumber[0] = BLOCK_MARK_INDEX;     break;  // ��
        case eDAMAGEPOPUP_RELOAD:popup.arNumber[0] = RELOAD_MARK_INDEX;   break;  // ���ε�
            // ����ġ
        case eDAMAGEPOPUP_EXP:
            {
                // ���ڹ迭�� �����
                popup.byNumberCount = MakeDigitArray(popup.iNumber,popup.arNumber);											
                popup.iMeshCount = popup.byNumberCount * 2;
                // XP ��ũ �ٿ��ֱ�	
                popup.arNumber[popup.byNumberCount] = XP_MARK_INDEX;
                popup.byNumberCount++;
                popup.iMeshCount += 2;
            }
            break;
            // ������
        default:
            {
                int inumbercount = 0;
                popup.iMeshCount  = popup.byNumberCount = 0;
                if (popup.iNumber2 > 0)
                {
                    inumbercount = MakeDigitArray(popup.iNumber2,&popup.arNumber[popup.byNumberCount]);
                    popup.byNumberCount += inumbercount;												
                    popup.iMeshCount += inumbercount * 2;
                    popup.arNumber[popup.byNumberCount] = LINE_CHANGE_INDEX;
                    popup.byNumberCount += 1;						
                }

                //���ڹ迭�� �����					
                inumbercount = MakeDigitArray(popup.iNumber,&popup.arNumber[popup.byNumberCount]);
                popup.byNumberCount += inumbercount;												
                popup.iMeshCount += inumbercount * 2;

                //ũ��Ƽ���̸鼭 �����ϰ�쿡�� ����ǥ�ø� �Ѵ�.
                if(popup.bDoubleDamage || popup.bCritical)
                {
                    popup.arNumber[popup.byNumberCount] = LINE_CHANGE_INDEX;
                    popup.arNumber[popup.byNumberCount + 1] = (popup.bDoubleDamage) ? (DOUBLE_MARK_INDEX):(CRITICAL_MARK_INDEX);
                    popup.byNumberCount += 2;
                    popup.iMeshCount += 2;
                }
            }
            break;
        }	
    }

    m_DamageList.push_back(popup);
}
//------------------------------------------------------------------------------
/** ������ ����Ʈ����
*/
void DamageMesh::ResetDamageList()
{
    m_DamageList.clear();
}
//------------------------------------------------------------------------------
/** �޽��� ���ؽ��� UV���������Ѵ�.
*/
void DamageMesh::MakeDamageMesh(int Index,WzVector &vBasePos,int digitcount,BYTE *digitarray,WzColor color,float fWidth,float fHeight)
{
    POPUPMESH_SIZE size;
    int count = 0;
    int vindex;

    float fSentenceWidth = 0.0f;
    float fDown = 0.0f;

    int iStartIndex = 0;
    int iCurrentIndex = 0;

    WzVector vLetterPos = {0.f,0.f,0.f};

    //1 Face is 3Vertices
    int nVertexIdx = Index*3;

    while(count < digitcount) 
    {
        //�����ϳ��� 2���Ǹ�(vertex*2ea)
        vindex = iCurrentIndex * 6;

        // �ٹٲ� ���ڰ��ƴϸ� �������� �����Ѵ�.
        if (digitarray[count] != LINE_CHANGE_INDEX)
        {
            //�ѹ��� 6���������Ѵ�.
            size = MakeNumberUVVertice(digitarray[count],vLetterPos,nVertexIdx+vindex,color,fWidth,fHeight);
            fSentenceWidth += size.fWidth;
            vLetterPos = vLetterPos + (m_vDamageMeshRight * size.fWidth);
            iCurrentIndex++;
        }
        else
        {
            WzVector vNewBasePos =  vBasePos - (m_vDamageMeshRight * fSentenceWidth * 0.5f) - (m_vDamageMeshDown * fDown);
            MoveVertexMesh(vNewBasePos,nVertexIdx+(iStartIndex * 6),(iCurrentIndex - iStartIndex)  * 6);
            fDown += size.fHeight;
            iStartIndex = iCurrentIndex;
            fSentenceWidth = 0.0f;
            ZeroMemory(&vLetterPos,sizeof(vLetterPos));
        }
        count++;		
    }

    if (iCurrentIndex > iStartIndex )
    {
        WzVector vNewBasePos =  vBasePos - (m_vDamageMeshRight * fSentenceWidth * 0.5f) - (m_vDamageMeshDown * fDown);
        MoveVertexMesh(vNewBasePos,nVertexIdx+(iStartIndex * 6),(iCurrentIndex - iStartIndex)  * 6);

    }
}
//------------------------------------------------------------------------------
/**  POSITION & UV  & COLOR ����
*/
POPUPMESH_SIZE DamageMesh::MakeNumberUVVertice(unsigned int number,WzVector &vBasePos,DWORD index,WzColor color,float fNumberWidth,float fNumberHeight)
{
    POPUPMESH_SIZE size;
    unsigned int texindex = number * 4;

    float fTexWidth = (NumberCoordinate[texindex + 2] - NumberCoordinate[texindex]) * 200.f;
    float fTexHeight = (NumberCoordinate[texindex + 3] - NumberCoordinate[texindex + 1]) * 200.f;

    fNumberWidth *= fTexWidth;
    fNumberHeight *= fTexHeight;

    // left top
    m_Meshinfo.m_pVertices[index]  = vBasePos;
    m_Meshinfo.m_pUVVerts[index].u = NumberCoordinate[texindex];
    m_Meshinfo.m_pUVVerts[index].v = NumberCoordinate[texindex + 1];
    m_Meshinfo.m_pColor[index]     = color;

    // right top
    m_Meshinfo.m_pVertices[index+1] = vBasePos + (m_vDamageMeshRight * fNumberWidth);
    m_Meshinfo.m_pUVVerts[index+1].u = NumberCoordinate[texindex + 2];
    m_Meshinfo.m_pUVVerts[index+1].v = NumberCoordinate[texindex + 1];
    m_Meshinfo.m_pColor[index+1]     = color;

    // left bottom
    m_Meshinfo.m_pVertices[index+2]  = vBasePos + (m_vDamageMeshDown * fNumberHeight);
    m_Meshinfo.m_pUVVerts[index+2].u = NumberCoordinate[texindex];
    m_Meshinfo.m_pUVVerts[index+2].v = NumberCoordinate[texindex + 3];
    m_Meshinfo.m_pColor[index+2]     = color;

    // right top 
    m_Meshinfo.m_pVertices[index+3]   = vBasePos + (m_vDamageMeshRight * fNumberWidth);
    m_Meshinfo.m_pUVVerts[index+3].u = NumberCoordinate[texindex + 2];
    m_Meshinfo.m_pUVVerts[index+3].v = NumberCoordinate[texindex + 1];
    m_Meshinfo.m_pColor[index+3]     = color;

    // left bottom
    m_Meshinfo.m_pVertices[index+4]   = vBasePos + (m_vDamageMeshDown * fNumberHeight);
    m_Meshinfo.m_pUVVerts[index+4].u= NumberCoordinate[texindex];
    m_Meshinfo.m_pUVVerts[index+4].v= NumberCoordinate[texindex + 3];
    m_Meshinfo.m_pColor[index+4]     = color;

    // right bottom
    m_Meshinfo.m_pVertices[index+5]   = vBasePos + (m_vDamageMeshRight * fNumberWidth) + (m_vDamageMeshDown * fNumberHeight);
    m_Meshinfo.m_pUVVerts[index+5].u = NumberCoordinate[texindex + 2];
    m_Meshinfo.m_pUVVerts[index+5].v = NumberCoordinate[texindex + 3];
    m_Meshinfo.m_pColor[index+5]     = color;

    size.fWidth = fNumberWidth;
    size.fHeight = fNumberHeight;
    return size;
}
//------------------------------------------------------------------------------
/** 
*/
void DamageMesh::MoveVertexMesh(WzVector &vOffset,int iVertexStart,int iVertexCount)
{	
    for (int a = 0; a < iVertexCount; a++)
    {
        m_Meshinfo.m_pVertices[iVertexStart + a] += vOffset;
    }
}
//------------------------------------------------------------------------------
/** ������ �������޽�
*/
void DamageMesh::RenderDamageList(DWORD dwTick)
{	
    //������üũ
    if(m_DamageList.empty()) 
    {
        return;
    }

    //���ӿɼ�üũ
    BOOL bRender = g_InterfaceManager.GetShowFlag();
    
    //���İ���
    DWORD alpharef;
    ENUM_CMP_FUNC alphafunc;	
    g_pSunRenderer->GetAlphaTestMode(&alpharef,&alphafunc);
    g_pSunRenderer->SetAlphaTestMode(0,CF_ALWAYS);
    ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
    g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );

    //�ø����
    ENUM_CULL_FACE      eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
    g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
    
    //z����
    ENUM_CMP_FUNC eZFunc = g_pSunRenderer->GetZFunc();
    BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
    BOOL bZTest = g_pSunRenderer->IsEnableZTest();
    g_pSunRenderer->SetZBufferWrite( FALSE );	
    g_pSunRenderer->SetZBufferTest( FALSE);

    DMG_LIST::iterator temp;
    DMG_LIST::iterator itr = m_DamageList.begin();

    //=================================
    // ������ ���� ����(Right.Normal)
    //=================================
    St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
    m_vDamageMeshRight = pCameraInfo->m_wvRight;
    m_vDamageMeshDown = -pCameraInfo->m_wvUp;

    DWORD dwDrawFace = 0;
    
    while (itr != m_DamageList.end())
    {
        temp = itr;
        WzVector wzVisualPos = (*temp).vPos;		

        // ó���� �� ũ�� ����ش�.
        float fSizeMultiplier = 1.0f;

        //=================================
        //  �ð�������ũ�⺯ȭ
        //=================================
        if ((*temp).dTime <= iDamageShrinkTime)
        {
            fSizeMultiplier *= LerfFloat(1.5f,1.0f,(float)(*temp).dTime / (float)iDamageShrinkTime);
        }
        float fWidth = 0.f;
        float fHeight = 0.f;
        
        //=================================
        //  ���ΰ��ϋ��� ũ�� ����
        //=================================
        if ( eDAMAGEPOPUP_BLOCK == (*temp).eKind && TRUE == (*temp).bHero )
        {
            // ����ΰ� ���� �������� �ٸ��������.(�����۰�)
            fWidth = 0.01f * fSizeMultiplier * (*temp).fMultiplier;
            fHeight = 0.01f * fSizeMultiplier * (*temp).fMultiplier;
        }
        else
        {
            fWidth = 0.01f * fSizeMultiplier * (*temp).fMultiplier;
            fHeight = 0.01f * fSizeMultiplier * (*temp).fMultiplier;
        }

        //=================================
        //  �ð��� �������İ���ȭ
        //=================================
        BYTE firstalpha = Alpha_WzColor((*temp).Color);
        BYTE alpha = firstalpha;

        bool bVanishTime = (*temp).dTime >= iDamageVanishTime;		
        if (bVanishTime)
        {
            float fRate = 1.0f - (float)((*temp).dTime - iDamageVanishTime) / (float)(iDamageDeleteTime - iDamageVanishTime);
            if (fRate <= 0.0f)
            {
                fRate = 0.0f;
            }
            alpha = (BYTE)float(firstalpha * fRate);
        }

        //=================================
        //  �����޽�����(pos.uv.color)
        //=================================
        if (bRender) 
        {
            WzColor color = WzColor_RGBA( Red_WzColor((*temp).Color),Green_WzColor((*temp).Color),Blue_WzColor((*temp).Color),alpha);
            
            DWORD dwCurFace = dwDrawFace+(*temp).iMeshCount;
            
            //Max ���ؽ��� �����ʵ��� üũ
            if( (dwCurFace*3) < m_dwInitMaxFace)
            {
                //�ѹ��� 6�����ؽ�(2Face)���� �����Ѵ�
                MakeDamageMesh(dwDrawFace,wzVisualPos, (*temp).byNumberCount,(*temp).arNumber, color,fWidth, fHeight );
                dwDrawFace = dwCurFace;
            }
        }
        
        // ������Ÿ��üũ
        if ( (*temp).dTime >= iDamageDeleteTime)
        {
            //itr--;
            //m_DamageList.erase(temp);							
            itr = m_DamageList.erase(itr);							
        }
        else
        {
            (*temp).dTime += dwTick;
            (*temp).vPos.z += (float)dwTick * l_fUpSpeed;			
            ++itr;
        }
    }

    //=================================
    //  �̺κп��� �ѹ��� DrawCall�Ѵ�
    //=================================
    if(bRender && dwDrawFace>0 && dwDrawFace<m_dwInitMaxFace) 
    {
        g_pSunRenderer->RealizeRenderTrianglesColoredIndexed( m_hNumberTexture,            //�ؽ���
                                                                dwDrawFace,                //face����  
                                                                m_Meshinfo.m_pVertices,    //���ؽ�����Ʈ
                                                                m_Meshinfo.m_piIndexMap,   //���ؽ�idx
                                                                m_Meshinfo.m_pColor,       //�÷�����Ʈ
                                                                m_Meshinfo.m_piIndexMap,   //�÷�idx
                                                                m_Meshinfo.m_pUVVerts,     //uv��ǥ
                                                                m_Meshinfo.m_piIndexMap);  //uv idx
    }

    g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
    g_pSunRenderer->SetZFunc(eZFunc);
    g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
    g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
    g_pSunRenderer->SetZBufferTest( bZTest );
    g_pSunRenderer->SetZBufferWrite( bZWrite );
}
