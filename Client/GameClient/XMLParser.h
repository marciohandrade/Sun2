#ifdef _USE_XMLLOG
#pragma once

/************************************************************************/
/* MS XML 를 편히 쓰기 위한 RAPPER CLASS                                */
/* description :  DB TABLE 형식에 맞췄다.
/* date : 2006/7/12
/* creator : 조재식
/************************************************************************/

#import <msxml4.dll>
#include <string>
#include <comutil.h>

typedef		MSXML2::IXMLDOMDocument2Ptr					XMLDOMPtr;
typedef		MSXML2::IXMLDOMProcessingInstructionPtr		XMLPIPtr;
typedef		MSXML2::IXMLDOMNodeListPtr					NODELISTPtr;
typedef		MSXML2::IXMLDOMNamedNodeMapPtr				ATTRIMAPPtr;	//NamedNodeMap은 주로 속성값 찾는데 사용되어서 이렇게 정의했다.
typedef		MSXML2::IXMLDOMNodePtr						NODEPtr;
typedef		MSXML2::IXMLDOMElementPtr					ELEMENTPtr;


class XMLParser
{
public:
	XMLParser(void);
	~XMLParser(void);

private:
	XMLDOMPtr		m_pDoc;
	XMLPIPtr		m_pPI;

	_variant_t		m_varData;
	_variant_t		m_varAttrValue;
	_variant_t		m_varXMLFile;

public:
	BOOL			Init();
	BOOL			Load( char* pszFile );
	BOOL			Save();
    BOOL			Save(char* pszFile);

//////////////////////////////////////////////////////////////////////////
// 읽기 함수들..

//////////////////////////////////////////////////////////////////////////
// 엘레먼트를 얻는 함수와, 각각의 엘레먼트의 속성 값을 얻는 함수

	//노드는 하나의 자료표현 단위이다.
	//노드는 엘레멘트가 될수도 있고, 속성도 될수 있다.

	//노드 검색 함수들..
	NODELISTPtr			FindNodeList( BSTR strElem, long& nNodeCount );						
	ATTRIMAPPtr			FindAttriMap( NODELISTPtr pNodeList, long nIndex );
	NODEPtr				FindNode( NODELISTPtr pNodeList, long nIndex );
	NODELISTPtr			FindChildNodeList( NODEPtr pNode, long& nNodeCount );	//현재 노드의 바로 아래 단계의 노드들의 리스트를 가져온다.

	//이름으로 속성값 얻기.(NODE)
	BOOL			GetAttriData( NODEPtr pNode, _bstr_t strAttriName, _bstr_t& bstrData );
	BOOL			GetAttriDataStr( NODEPtr	pNode, _bstr_t strAttriName, std::string& strData );
	BOOL			GetAttriDataInt( NODEPtr	pNode, _bstr_t strAttriName, int& nData );
	BOOL			GetAttriDataDWORD( NODEPtr	pNode, _bstr_t strAttriName, DWORD& dwData );
	BOOL			GetAttriDataFloat( NODEPtr	pNode, _bstr_t strAttriName, float& fData );
	BOOL			GetAttriDataBYTE( NODEPtr	pNode, _bstr_t strAttriName, BYTE& byData );

	//속성 번호로 속성값 얻기
	BOOL			GetAttriData( NODEPtr	pNode, long nIndex, _bstr_t& bstrData  );
	BOOL			GetAttriDataStr( NODEPtr	pNode, long nIndex, std::string& strData  );
	BOOL			GetAttriDataInt( NODEPtr	pNode, long nIndex, int& nData );
	BOOL			GetAttriDataDWORD( NODEPtr	pNode, long nIndex, DWORD& dwData );
	BOOL			GetAttriDataFloat( NODEPtr	pNode, long nIndex, float& fData );
	BOOL			GetAttriDataBYTE( NODEPtr	pNode, long nIndex, BYTE& byData );

private:
	//속성 노드 값을 얻는다.
	NODEPtr			GetAttriNode( NODEPtr pNode, _bstr_t strAttriName );
	NODEPtr			GetAttriNode( NODEPtr pNode, long nIndex );

	//해당 자료형의 값을 얻어온다.
	BOOL			GetNodeDataBSTR( NODEPtr pNode, _bstr_t& strData );
	BOOL			GetNodeDataStr( NODEPtr pNode, std::string& strData );
	BOOL			GetNodeDataInt( NODEPtr pNode, int& nData );
	BOOL			GetNodeDataDWORD( NODEPtr pNode, DWORD& dwData );
	BOOL			GetNodeDataFloat( NODEPtr pNode, float& fData );
	BOOL			GetNodeDataBYTE( NODEPtr pNode, BYTE& byData );


//////////////////////////////////////////////////////////////////////////
// 쓰기 함수들...
public:
	//속성 쓰기.
	BOOL			SetAttribute( ELEMENTPtr pElem, BSTR strName, _variant_t value );
	BOOL			SetAttributeStr( ELEMENTPtr pElem, BSTR strName, std::string strValue );
	BOOL			SetAttributeInt( ELEMENTPtr pElem, BSTR strName, int nValue );
	BOOL			SetAttributeDWORD( ELEMENTPtr pElem, BSTR strName, DWORD dwValue );
	BOOL			SetAttributeFloat( ELEMENTPtr pElem, BSTR strName, float fValue );
	BOOL			SetAttributeBYTE( ELEMENTPtr pElem, BSTR strName, BYTE byValue );

//////////////////////////////////////////////////////////////////////////
//  삽입함수
	ELEMENTPtr		InsertNewElement( BSTR strParent, BSTR strNewElem );

//////////////////////////////////////////////////////////////////////////
// 수정 함수들...
public:
	BOOL			DelElement( BSTR strElem, _bstr_t strAttriName, _variant_t varAttriValue );		//엘레멘트 이름과 속성값으로 삭제
	BOOL			ModyfyAttribute( TCHAR* pszElem, TCHAR* pszAttri, TCHAR* pszOldValue, TCHAR* pszNewValue );
};

#endif // _USE_XMLLOG