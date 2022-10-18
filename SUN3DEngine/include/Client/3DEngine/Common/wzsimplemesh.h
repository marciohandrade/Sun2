// wzsimplemesh.h

#ifndef _COMMON_WZSIMPLEMESH_H_
#define _COMMON_WZSIMPLEMESH_H_

#include "wzuvvert.h"

//------------------------------------------------------------------------------
/**
    @struct WzSimpleMesh
*/
struct WzSimpleMesh
{
    HANDLE m_hTexDiffuse;
    int m_iNumMesh;
    WzVector* m_pVertices;
    WzUVVert* m_pUVVerts;
};

//------------------------------------------------------------------------------
/**
*/
inline WzSimpleMesh* New_SimpleMesh( int iNumMesh )
{
    WzSimpleMesh* pwsm = new WzSimpleMesh;
    WzAssert( pwsm );

    if( pwsm )
    {
        pwsm->m_hTexDiffuse = INVALID_HANDLE_VALUE;
        pwsm->m_iNumMesh = iNumMesh;

        pwsm->m_pVertices = new WzVector[3 * iNumMesh];
        WzAssert( pwsm->m_pVertices );

        pwsm->m_pUVVerts = new WzUVVert[3 * iNumMesh];
        WzAssert( pwsm->m_pUVVerts );
    }

    return pwsm;
}

//------------------------------------------------------------------------------
/**
*/
inline void Delete_SimpleMesh( WzSimpleMesh* pwsm )
{
    if( pwsm )
    {
        if( pwsm->m_pVertices )
        {
            delete [] pwsm->m_pVertices;
            pwsm->m_pVertices = NULL;
        }

        if( pwsm->m_pUVVerts )
        {
            delete [] pwsm->m_pUVVerts;
            pwsm->m_pUVVerts = NULL;
        }

        delete pwsm;
        pwsm = NULL;
    }
}

#endif // _COMMON_WZSIMPLEMESH_H_