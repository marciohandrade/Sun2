#include "SunClientPrecompiledHeader.h"
#ifdef __CN_269_MATRIX_CARD_AUTH
#include "uiMatrixCard/uiMatrixCardMan.h"
#include "InterfaceManager.h"
#include "MatrixCardAuth.h"

const char  c_strDlgFname_MatrixCardAuth[]           = ("Data\\Interface\\1_7_electronic_authentication.iwz");
const DWORD c_dwCompareID_MatrixCardAuth             = StrToWzID("0107");

//------------------------------------------------------------------------------
/**
*/
uiMatrixCardMan::uiMatrixCardMan(InterfaceManager * pUIMAN) 
: uiBaseMan(pUIMAN)
{
}


void uiMatrixCardMan::OnInitialize()
{
    m_pMatrixCardAuth = CreateUIDialog<MatrixCardAuth>(c_dwCompareID_MatrixCardAuth
        , c_strDlgFname_MatrixCardAuth  
        , this
        ,TRUE);
}


#endif // __CN_269_MATRIX_CARD_AUTH