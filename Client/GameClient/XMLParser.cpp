#include "SunClientPrecompiledHeader.h"
#ifdef _USE_XMLLOG

#include ".\xmlparser.h"

#define MAX_QUERY_STRING 512

XMLParser::XMLParser(void)
{
}

XMLParser::~XMLParser(void)
{
}


BOOL	XMLParser::Init()
{
	CoInitialize( NULL );

	HRESULT	hr;

	hr = m_pDoc.CreateInstance( __uuidof(MSXML2::DOMDocument) );
	if( FAILED(hr) )	return FALSE;

	m_pPI = m_pDoc->createProcessingInstruction( L"xml", L"version=\"1.0\" encoding=\"euc-kr\"" );	//PI생성
	hr = m_pDoc->appendChild( m_pPI );
	if( FAILED(hr) )	return FALSE;

	return TRUE;
}

BOOL	XMLParser::Load( char* pszFile )
{
	m_varXMLFile = pszFile;

	if( m_pDoc->load( m_varXMLFile ) != VARIANT_TRUE )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL	XMLParser::Save()
{
	HRESULT hr = m_pDoc->save( m_varXMLFile );
	if( FAILED(hr) )	return FALSE;

	return TRUE;
}

BOOL    XMLParser::Save(char * pszFile)
{
    m_varXMLFile = pszFile;
    HRESULT hr = m_pDoc->save( m_varXMLFile );
    if( FAILED(hr) )	return FALSE;

    return TRUE;
}


/************************************************************************/
/* Find Nodes Methods                                                   */
/************************************************************************/

//Element Name 으로 검색
NODELISTPtr			XMLParser::FindNodeList( BSTR strElem, long& nNodeCount )
{
	NODELISTPtr pNodeList = NULL;
	pNodeList = m_pDoc->selectNodes( strElem );
	nNodeCount = pNodeList->Getlength();
	return pNodeList;
}

//return  Atrributes of Element
ATTRIMAPPtr			XMLParser::FindAttriMap( NODELISTPtr pNodeList, long nIndex )
{
	if( pNodeList == NULL )	return NULL;
	NODEPtr pNode = pNodeList->Getitem( nIndex );
	if( pNode == NULL )	return NULL;
	return pNode->Getattributes();
}


NODEPtr			XMLParser::FindNode( NODELISTPtr pNodeList, long nIndex )
{
	if( pNodeList == NULL )	return NULL;
	NODEPtr pNode = pNodeList->Getitem( nIndex );
	if( pNode == NULL )	return NULL;
	return pNode;
}

//현재 노드의 바로 아래 단계의 노드들의 리스트를 가져온다.
NODELISTPtr			XMLParser::FindChildNodeList( NODEPtr pNode, long& nNodeCount )
{
	if( pNode == NULL )	return NULL;
	NODELISTPtr	pNodeList = pNode->GetchildNodes();
	if( pNodeList == NULL )		return NULL;
	nNodeCount = pNodeList->Getlength();
	return pNodeList;
}


/************************************************************************/
/* 이름으로 속성값 얻기                                                 */
/************************************************************************/
BOOL			XMLParser::GetAttriData( NODEPtr	pNode, _bstr_t strAttriName, _bstr_t& bstrData )
{
	bstrData = L"";
	if( pNode == NULL )								return FALSE;
	NODEPtr	pAttriNode = GetAttriNode( pNode, strAttriName );
	if( pAttriNode == NULL )						return FALSE;
	if( !GetNodeDataBSTR( pAttriNode, bstrData ) )	return FALSE;
	return TRUE;
}

BOOL			XMLParser::GetAttriDataStr( NODEPtr	pNode, _bstr_t strAttriName, std::string& strData )
{
	strData.clear();
	if( pNode == NULL )								return FALSE;
	NODEPtr	pAttriNode = GetAttriNode( pNode, strAttriName );
	if( pAttriNode == NULL )						return FALSE;
	if( !GetNodeDataStr( pAttriNode, strData ) )	return FALSE;
	return TRUE;
}

BOOL			XMLParser::GetAttriDataInt( NODEPtr	pNode, _bstr_t strAttriName, int& nData )
{
	nData = 0;
	if( pNode == NULL )							return FALSE;
	NODEPtr	pAttriNode = GetAttriNode( pNode, strAttriName );
	if( pAttriNode == NULL )					return FALSE;
	if( !GetNodeDataInt( pAttriNode, nData ) )	return FALSE;

	return TRUE;
}

BOOL			XMLParser::GetAttriDataDWORD( NODEPtr	pNode, _bstr_t strAttriName, DWORD& dwData )
{
	dwData = 0;
	if( pNode == NULL )								return FALSE;
	NODEPtr	pAttriNode = GetAttriNode( pNode, strAttriName );
	if( pAttriNode == NULL )						return FALSE;
	if( !GetNodeDataDWORD( pAttriNode, dwData ) )	return FALSE;

	return TRUE;
}

BOOL			XMLParser::GetAttriDataFloat( NODEPtr	pNode, _bstr_t strAttriName, float& fData )
{
	fData = 0;
	if( pNode == NULL )								return FALSE;
	NODEPtr	pAttriNode = GetAttriNode( pNode, strAttriName );
	if( pAttriNode == NULL )						return FALSE;
	if( !GetNodeDataFloat( pAttriNode, fData ) )	return FALSE;

	return TRUE;
}

BOOL			XMLParser::GetAttriDataBYTE( NODEPtr	pNode, _bstr_t strAttriName, BYTE& byData )
{
	byData = 0;
	if( pNode == NULL )								return FALSE;
	NODEPtr	pAttriNode = GetAttriNode( pNode, strAttriName );
	if( pAttriNode == NULL )						return FALSE;
	if( !GetNodeDataBYTE( pAttriNode, byData ) )	return FALSE;

	return TRUE;
}


NODEPtr			XMLParser::GetAttriNode( NODEPtr pNode, _bstr_t strAttriName )
{
	if( pNode == NULL )	return NULL;

	ATTRIMAPPtr	pAttriMap = pNode->Getattributes();
	if( pAttriMap == NULL )	return NULL;
	NODEPtr pAttriNode = pAttriMap->getNamedItem( strAttriName );
	if( pNode == NULL )	return NULL;
	return pAttriNode;
}

/************************************************************************/
/* Node에서 해당 자료형의 값을 얻어온다.                                */
/************************************************************************/
BOOL			XMLParser::GetNodeDataBSTR( NODEPtr pNode, _bstr_t& strData )
{
	m_varData.Clear();

	if( pNode == NULL )	return FALSE;
	m_varData = pNode->GetnodeValue();
	m_varData.ChangeType( VT_BSTR );
	strData = m_varData.bstrVal;

	return TRUE;
}


BOOL			XMLParser::GetNodeDataStr( NODEPtr pNode, std::string& strData )
{
	m_varData.Clear();

	if( pNode == NULL )	return FALSE;
	m_varData = pNode->GetnodeValue();
	m_varData.ChangeType( VT_BSTR );
	strData = _com_util::ConvertBSTRToString( m_varData.bstrVal );

	return TRUE;
}

BOOL			XMLParser::GetNodeDataInt( NODEPtr pNode, int& nData )
{
	m_varData.Clear();

	if( pNode == NULL )	return FALSE;

	m_varData = pNode->GetnodeValue();
	m_varData.ChangeType( VT_I4 );
	nData = m_varData.intVal;

	return TRUE;
}

BOOL			XMLParser::GetNodeDataDWORD( NODEPtr pNode, DWORD& dwData )
{
	m_varData.Clear();

	if( pNode == NULL )	return FALSE;

	m_varData = pNode->GetnodeValue();
	m_varData.ChangeType( VT_UI4 );
	dwData = m_varData.ulVal;

	return TRUE;
}

BOOL			XMLParser::GetNodeDataFloat( NODEPtr pNode, float& fData )
{
	m_varData.Clear();

	if( pNode == NULL )	return FALSE;

	m_varData = pNode->GetnodeValue();
	m_varData.ChangeType( VT_R4 );
	fData = m_varData.fltVal;

	return TRUE;
}

BOOL			XMLParser::GetNodeDataBYTE( NODEPtr pNode, BYTE& byData )
{
	m_varData.Clear();

	if( pNode == NULL )	return FALSE;

	m_varData = pNode->GetnodeValue();
	m_varData.ChangeType( VT_UI1 );
	byData = m_varData.bVal;

	return TRUE;
}


/************************************************************************/
/* 일련번호로 속성값 얻기                                               */
/************************************************************************/
BOOL			XMLParser::GetAttriData( NODEPtr	pNode, long nIndex, _bstr_t& bstrData )
{
	bstrData = L"";
	if( pNode == NULL )									return FALSE;
	NODEPtr	pAttriNode = GetAttriNode( pNode, nIndex );
	if( pAttriNode == NULL )							return FALSE;
	if( !GetNodeDataBSTR( pAttriNode, bstrData ) )		return FALSE;
	return TRUE;
}

BOOL			XMLParser::GetAttriDataStr( NODEPtr	pNode, long nIndex, std::string& strData )
{
	strData.clear();
	if( pNode == NULL )									return FALSE;
	NODEPtr	pAttriNode = GetAttriNode( pNode, nIndex );
	if( pAttriNode == NULL )							return FALSE;
	if( !GetNodeDataStr( pAttriNode, strData ) )		return FALSE;
	return TRUE;
}

BOOL			XMLParser::GetAttriDataInt( NODEPtr	pNode, long nIndex, int& nData )
{
	nData = 0;
	if( pNode == NULL )									return FALSE;
	NODEPtr	pAttriNode = GetAttriNode( pNode, nIndex );
	if( pAttriNode == NULL )							return FALSE;
	if( !GetNodeDataInt( pAttriNode, nData ) )			return FALSE;
	return TRUE;
}

BOOL			XMLParser::GetAttriDataDWORD( NODEPtr	pNode, long nIndex, DWORD& dwData )
{
	dwData = 0;
	if( pNode == NULL )									return FALSE;
	NODEPtr	pAttriNode = GetAttriNode( pNode, nIndex );
	if( pAttriNode == NULL )		return FALSE;
	if( !GetNodeDataDWORD( pAttriNode, dwData ) )		return FALSE;
	return TRUE;
}

BOOL			XMLParser::GetAttriDataFloat( NODEPtr	pNode, long nIndex, float& fData )
{
	fData = 0;
	if( pNode == NULL )									return FALSE;		
	NODEPtr	pAttriNode = GetAttriNode( pNode, nIndex );
	if( pAttriNode == NULL )							return FALSE;
	if( !GetNodeDataFloat( pAttriNode, fData ) )		return FALSE;
	return TRUE;
}

BOOL			XMLParser::GetAttriDataBYTE( NODEPtr	pNode, long nIndex, BYTE& byData )
{
	byData = 0;
	if( pNode == NULL )									return FALSE;
	NODEPtr	pAttriNode = GetAttriNode( pNode, nIndex );
	if( pAttriNode == NULL )							return FALSE;
	if( !GetNodeDataBYTE( pAttriNode, byData ) )		return FALSE;
	return TRUE;
}


NODEPtr			XMLParser::GetAttriNode( NODEPtr pNode, long nIndex )
{
	nIndex = 0;
	if( pNode == NULL )									return NULL;
	ATTRIMAPPtr	pAttriMap = pNode->Getattributes();
	if( pAttriMap == NULL )								return NULL;
	NODEPtr pAttriNode = pAttriMap->Getitem( nIndex );
	if( pNode == NULL )									return NULL;
	return pAttriNode;
}

/************************************************************************/
/* Insert New Element                                                   */
/************************************************************************/


ELEMENTPtr		XMLParser::InsertNewElement( BSTR strParent, BSTR strNewElem )
{
	ELEMENTPtr		pParent = m_pDoc->selectSingleNode( strParent );
	ELEMENTPtr		pNewElem = m_pDoc->createElement( strNewElem );
	pParent->appendChild( pNewElem );

	return pNewElem;
}

/************************************************************************/
/* 엘레먼트에 속성값 설정하는 함수들                                    */
/************************************************************************/
BOOL			XMLParser::SetAttribute( ELEMENTPtr pElem, BSTR strName, _variant_t value )
{
	HRESULT hr = pElem->setAttribute( strName, value );
	if( FAILED(hr) )	return FALSE;

	return TRUE;
}

BOOL			XMLParser::SetAttributeStr( ELEMENTPtr pElem, BSTR strName, std::string strValue )
{
	m_varAttrValue.Clear();
	m_varAttrValue.vt = VT_BSTR;
	m_varAttrValue.bstrVal = _com_util::ConvertStringToBSTR( strValue.c_str() );
	
	HRESULT hr = pElem->setAttribute( strName, m_varAttrValue );
	if( FAILED(hr) )	return FALSE;

	return TRUE;
}

BOOL			XMLParser::SetAttributeInt( ELEMENTPtr pElem, BSTR strName, int nValue )
{
	m_varAttrValue.Clear();
	m_varAttrValue.vt = VT_I4;
	m_varAttrValue.intVal = nValue;

	HRESULT hr = pElem->setAttribute( strName, m_varAttrValue );
	if( FAILED(hr) )	return FALSE;

	return TRUE;
}

BOOL			XMLParser::SetAttributeDWORD( ELEMENTPtr pElem, BSTR strName, DWORD dwValue )
{
	m_varAttrValue.Clear();
	m_varAttrValue.vt = VT_UI4;
	m_varAttrValue.ulVal = dwValue;

	HRESULT hr = pElem->setAttribute( strName, m_varAttrValue );
	if( FAILED(hr) )	return FALSE;

	return TRUE;

}

BOOL			XMLParser::SetAttributeFloat( ELEMENTPtr pElem, BSTR strName, float fValue )
{
	m_varAttrValue.Clear();
	m_varAttrValue.vt = VT_R4;
	m_varAttrValue.fltVal = fValue;

	HRESULT hr = pElem->setAttribute( strName, m_varAttrValue );
	if( FAILED(hr) )	return FALSE;

	return TRUE;
}

BOOL			XMLParser::SetAttributeBYTE( ELEMENTPtr pElem, BSTR strName, BYTE byValue )
{
	m_varAttrValue.Clear();
	m_varAttrValue.vt = VT_UI1;
	m_varAttrValue.bVal = byValue;

	HRESULT hr = pElem->setAttribute( strName, m_varAttrValue );
	if( FAILED(hr) )	return FALSE;

	return TRUE;
}

//엘레멘트 이름과 속성값으로 삭제
BOOL			XMLParser::DelElement( BSTR strElem, _bstr_t strAttriName, _variant_t varAttriValue )
{

	return TRUE;
}

BOOL			XMLParser::ModyfyAttribute( TCHAR* pszElem, TCHAR* pszAttri, TCHAR* pszOldValue, TCHAR* pszNewValue )
{
	if( !pszElem || !pszAttri || !pszOldValue || !pszNewValue )
		return FALSE;

	TCHAR szQuery[MAX_QUERY_STRING];
	_snprintf( szQuery, MAX_QUERY_STRING, "//%s[@%s==%s]", pszElem, pszAttri, pszOldValue ); 
	BSTR strQuery = _com_util::ConvertStringToBSTR( szQuery );
	ELEMENTPtr	pElement = m_pDoc->selectNodes( strQuery );
	if( pElement )
	{
		BSTR strAttri = _com_util::ConvertStringToBSTR( pszAttri );
		BSTR strValue = _com_util::ConvertStringToBSTR( pszNewValue );
		return SetAttribute( pElement, strAttri, pszNewValue );
	}

	return FALSE;
}

#endif // _USE_XMLLOG