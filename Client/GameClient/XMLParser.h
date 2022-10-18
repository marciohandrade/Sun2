#ifdef _USE_XMLLOG
#pragma once

/************************************************************************/
/* MS XML �� ���� ���� ���� RAPPER CLASS                                */
/* description :  DB TABLE ���Ŀ� �����.
/* date : 2006/7/12
/* creator : �����
/************************************************************************/

#import <msxml4.dll>
#include <string>
#include <comutil.h>

typedef		MSXML2::IXMLDOMDocument2Ptr					XMLDOMPtr;
typedef		MSXML2::IXMLDOMProcessingInstructionPtr		XMLPIPtr;
typedef		MSXML2::IXMLDOMNodeListPtr					NODELISTPtr;
typedef		MSXML2::IXMLDOMNamedNodeMapPtr				ATTRIMAPPtr;	//NamedNodeMap�� �ַ� �Ӽ��� ã�µ� ���Ǿ �̷��� �����ߴ�.
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
// �б� �Լ���..

//////////////////////////////////////////////////////////////////////////
// ������Ʈ�� ��� �Լ���, ������ ������Ʈ�� �Ӽ� ���� ��� �Լ�

	//���� �ϳ��� �ڷ�ǥ�� �����̴�.
	//���� ������Ʈ�� �ɼ��� �ְ�, �Ӽ��� �ɼ� �ִ�.

	//��� �˻� �Լ���..
	NODELISTPtr			FindNodeList( BSTR strElem, long& nNodeCount );						
	ATTRIMAPPtr			FindAttriMap( NODELISTPtr pNodeList, long nIndex );
	NODEPtr				FindNode( NODELISTPtr pNodeList, long nIndex );
	NODELISTPtr			FindChildNodeList( NODEPtr pNode, long& nNodeCount );	//���� ����� �ٷ� �Ʒ� �ܰ��� ������ ����Ʈ�� �����´�.

	//�̸����� �Ӽ��� ���.(NODE)
	BOOL			GetAttriData( NODEPtr pNode, _bstr_t strAttriName, _bstr_t& bstrData );
	BOOL			GetAttriDataStr( NODEPtr	pNode, _bstr_t strAttriName, std::string& strData );
	BOOL			GetAttriDataInt( NODEPtr	pNode, _bstr_t strAttriName, int& nData );
	BOOL			GetAttriDataDWORD( NODEPtr	pNode, _bstr_t strAttriName, DWORD& dwData );
	BOOL			GetAttriDataFloat( NODEPtr	pNode, _bstr_t strAttriName, float& fData );
	BOOL			GetAttriDataBYTE( NODEPtr	pNode, _bstr_t strAttriName, BYTE& byData );

	//�Ӽ� ��ȣ�� �Ӽ��� ���
	BOOL			GetAttriData( NODEPtr	pNode, long nIndex, _bstr_t& bstrData  );
	BOOL			GetAttriDataStr( NODEPtr	pNode, long nIndex, std::string& strData  );
	BOOL			GetAttriDataInt( NODEPtr	pNode, long nIndex, int& nData );
	BOOL			GetAttriDataDWORD( NODEPtr	pNode, long nIndex, DWORD& dwData );
	BOOL			GetAttriDataFloat( NODEPtr	pNode, long nIndex, float& fData );
	BOOL			GetAttriDataBYTE( NODEPtr	pNode, long nIndex, BYTE& byData );

private:
	//�Ӽ� ��� ���� ��´�.
	NODEPtr			GetAttriNode( NODEPtr pNode, _bstr_t strAttriName );
	NODEPtr			GetAttriNode( NODEPtr pNode, long nIndex );

	//�ش� �ڷ����� ���� ���´�.
	BOOL			GetNodeDataBSTR( NODEPtr pNode, _bstr_t& strData );
	BOOL			GetNodeDataStr( NODEPtr pNode, std::string& strData );
	BOOL			GetNodeDataInt( NODEPtr pNode, int& nData );
	BOOL			GetNodeDataDWORD( NODEPtr pNode, DWORD& dwData );
	BOOL			GetNodeDataFloat( NODEPtr pNode, float& fData );
	BOOL			GetNodeDataBYTE( NODEPtr pNode, BYTE& byData );


//////////////////////////////////////////////////////////////////////////
// ���� �Լ���...
public:
	//�Ӽ� ����.
	BOOL			SetAttribute( ELEMENTPtr pElem, BSTR strName, _variant_t value );
	BOOL			SetAttributeStr( ELEMENTPtr pElem, BSTR strName, std::string strValue );
	BOOL			SetAttributeInt( ELEMENTPtr pElem, BSTR strName, int nValue );
	BOOL			SetAttributeDWORD( ELEMENTPtr pElem, BSTR strName, DWORD dwValue );
	BOOL			SetAttributeFloat( ELEMENTPtr pElem, BSTR strName, float fValue );
	BOOL			SetAttributeBYTE( ELEMENTPtr pElem, BSTR strName, BYTE byValue );

//////////////////////////////////////////////////////////////////////////
//  �����Լ�
	ELEMENTPtr		InsertNewElement( BSTR strParent, BSTR strNewElem );

//////////////////////////////////////////////////////////////////////////
// ���� �Լ���...
public:
	BOOL			DelElement( BSTR strElem, _bstr_t strAttriName, _variant_t varAttriValue );		//������Ʈ �̸��� �Ӽ������� ����
	BOOL			ModyfyAttribute( TCHAR* pszElem, TCHAR* pszAttri, TCHAR* pszOldValue, TCHAR* pszNewValue );
};

#endif // _USE_XMLLOG