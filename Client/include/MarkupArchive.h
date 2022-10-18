// PGLMarkup.h: interface for the CMarkupArchive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PGLMARKUP_H__3BB8B027_BBD6_4DCF_ACDE_7F1922437BEB__INCLUDED_)
#define AFX_PGLMARKUP_H__3BB8B027_BBD6_4DCF_ACDE_7F1922437BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "STLXMLParser.h"
//#include <afxdisp.h>
#include "STL2String.h"

// Comment out these definitions if you don't want to use it as a .lib
#ifndef _XML_PARSER
	#ifndef _UNICODE
		#ifdef _DEBUG
			#pragma message("     _Adding library: XMLParserd.lib (Debug version - Please read License)" ) 
			#pragma comment(lib, "XMLParserd.lib")
		#else
			#pragma message("     _Adding library: XMLParser.lib (Release version - Please read License)" ) 
			#pragma comment(lib, "XMLParser.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma message("     _Adding library: XMLParserud.lib (Debug version UNICODE - Please read License)" ) 
			#pragma comment(lib, "XMLParserud.lib")
		#else
			#pragma message("     _Adding library: XMLParseru.lib (Release version UNICODE - Please read License)" ) 
			#pragma comment(lib, "XMLParseru.lib")
		#endif
	#endif
#endif


#include "MarkupSTL.h"

namespace markup
{

/*! \brief Class wrapper for CMarkupSTL adding useful features.

This class mimics the well-known CArchive class and implements a few useful helpers:
<ul>
<li>File handling with Open and Close functions (zipping using zlib)
<li>Classic IsStoring() function.
<li>Added Namespace Support
<li>Helpers to write and read bool, int, float, double, UINT, DWORD, STL vectors
<li>FindChildElemAttrib( LPCTSTR szName , LPCTSTR szAttrib, LPCTSTR szAttribValue )
	Finds a child elem with tag szName and an attribute szAttrib of value szAttribValue
<li>Helper functions FindGetElem, FindGetChildElem : Search for tag, if found loads corresponding value.
</ul>

*/
class CMarkupArchive : public CMarkupSTL  
{
public:
	//! Archive flags	
	static enum EArchiveMode
	{	
		//!	Create mode : when loading object, previous data is erased. (used in PGL)
		ArchiveModeStoring =		0x0001,
		//!	Create mode : when loading object, previous data is erased. (used in PGL)
		ArchiveModeCreate =			0x0002,
		//! Must find name : load data, only if element with same name ( name is an attribute)
		ArchiveModeMustFindName =	0x0004,
		//! Loads and stores non-zipped information
		ArchiveModeNotZipped =		0x0008,
		//! Working with string only, no files
		ArchiveModeString =			0x0010
	};

	//! \name Constructors
	//@{
	/*! Default constructor

  \param dwMode the archive mode. Can be a combination (with |) of the EArchiveMode Enum. Default is AchiveModeCreate | ArchiveModeStoring
	*/
	CMarkupArchive(DWORD dwMode = ArchiveModeCreate | ArchiveModeStoring): CMarkupSTL() {	m_dwMode = dwMode; m_bOpened=false; m_strEncodingString = _T("ISO-8859-1");};
	virtual ~CMarkupArchive();
	//@}

	//! \name File Handling
	//@{
	/*! Open a file

	\param szFileName the xml file name
	\param bStoring true if storing xml to a file, false if loading
	*/
	bool Open(LPCTSTR szFileName);
	/*! Open a xml string

	\param szXMLString the xml string
	\return true if success, false otherwize
	*/
	bool OpenString(LPCTSTR szXMLString);
	//! Close the file
	bool Close();	
	/*! Sets the xml encoding string.

	When the xml is stored, a first line is appended to the file:
	<pre>
	<?xml version="1.0" encoding="'Here the encoding string'" ?>
	</pre>

	The default value is ISO-8859-1
	*/
	void SetEncodingString(LPCTSTR lpszEncodingString) { if (lpszEncodingString != NULL) {m_strEncodingString = lpszEncodingString;}};
	//@}

	//! \name Flags
	//@{
	//! Returns true if the file is currently opened
	bool IsOpen() const					{	return m_bOpened;};
	//! Returns true if storing xml
	bool IsStoring() const				{	return m_dwMode & ArchiveModeStoring;};
	//! Returns true if loading xml
	bool IsLoading() const				{	return !(m_dwMode & ArchiveModeStoring);};
	//! Returns true if zipping
	bool IsZipping() const				{	return !(m_dwMode & ArchiveModeNotZipped);};
	bool IsCreateMode() const			{	return ((m_dwMode & ArchiveModeCreate) != 0);};
	bool IsMustFindNameMode() const		{	return ((m_dwMode & ArchiveModeMustFindName) != 0);};
	/*! Sets the archive mode.

	\param _dwMode a combination of the members of EArchiveMode
	*/
	void SetModeFlag( DWORD _dwMode = ArchiveModeCreate | ArchiveModeStoring) {	m_dwMode = _dwMode;};
	//@}

	//! \name Namespace
	//@{
	/*! Sets the xml namespace

	\param szNameSpace the namespace

	The namespace, if non null, is appended to every xml entry: <a>...</a> becomes <namespace:a>...</namespace:a>
	*/
	bool SetNameSpace(LPCTSTR szNameSpace)	
	{	
		m_sNameSpace = szNameSpace; 
		if (!m_sNameSpace.empty()) 
			m_sNameSpace+=_T(":"); 
		return true;
	};
	//! Return the namespace
	std::string GetNameSpace() const			
	{	
		if (m_sNameSpace.empty())
			return m_sNameSpace;
		else
			return m_sNameSpace.substr(0, m_sNameSpace.length()-1);
	};
	//@}

	//! \name Specialized getters
	//@{
	bool GetDataBool() const													{ return GetData() == _T("true"); };
	int GetDataInt() const														{ int val=0; _stscanf(GetData().c_str(),_T("%d"),&val); return  val; };
	USHORT GetDataUSHORT() const													{ USHORT val=0; _stscanf(GetData().c_str(),_T("%u"),&val); return  val; };
	UINT GetDataUINT() const													{ UINT val=0; _stscanf(GetData().c_str(),_T("%u"),&val); return  val; };
	BYTE GetDataBYTE() const													{ BYTE val=0; _stscanf(GetData().c_str(),_T("%u"),&val); return  val; };
	DWORD GetDataDWORD() const													{ DWORD val=0; _stscanf(GetData().c_str(),_T("%u"),&val); return  val; };
	float GetDataFloat() const													{ float val=0; _stscanf(GetData().c_str(),_T("%f"),&val); return  val; };
	double GetDataDouble() const												{ double val=0; _stscanf(GetData().c_str(),_T("%lf"),&val); return  val; };
//	COleDateTime GetDataDateTime() const										{	COleDateTime t; t.ParseDateTime(GetData().c_str()); return t;};
//	COleDateTimeSpan GetDataDateTimeSpan() const;
	void GetDataEx(std::vector<double>& _v) const								{ CSTL2String<double>::ToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::vector<float>& _v) const								{ CSTL2String<float>::ToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::vector<int>& _v) const									{ CSTL2String<int>::ToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::vector<BYTE>& _v) const									{ CSTL2String<BYTE>::ByteToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::vector<short>& _v) const								{ CSTL2String<short>::ToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::vector<long>& _v) const									{ CSTL2String<long>::ToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::valarray<double>& _v) const								{ CSTL2String<double>::ToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::valarray<float>& _v) const								{ CSTL2String<float>::ToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::valarray<int>& _v) const								{ CSTL2String<int>::ToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::valarray<BYTE>& _v) const								{ CSTL2String<BYTE>::ByteToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::valarray<short>& _v) const								{ CSTL2String<short>::ToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::valarray<long>& _v) const								{ CSTL2String<long>::ToSTL(GetData().c_str(), _v);};
	void GetDataEx(std::list<UINT>& _l) const									{ CSTL2String<UINT>::ToSTL(GetData().c_str(), _l);};
	void GetDataEx(std::list<double>& _l) const									{ CSTL2String<double>::ToSTL(GetData().c_str(), _l);};
	void GetDataEx(std::list<float>& _l) const									{ CSTL2String<float>::ToSTL(GetData().c_str(), _l);};
	void GetDataEx(std::list<int>& _l) const									{ CSTL2String<int>::ToSTL(GetData().c_str(), _l);};
	void GetDataEx(std::list<BYTE>& _l) const									{ CSTL2String<BYTE>::ByteToSTL(GetData().c_str(), _l);};

	bool GetChildDataBool() const												{ return GetChildData() == _T("true"); };
	int GetChildDataInt() const													{ int val=0; _stscanf(GetChildData().c_str(),_T("%d"),&val); return  val; };
	long GetChildDataLong() const												{ int val=0; _stscanf(GetChildData().c_str(),_T("%d"),&val); return  (long)val; };
	UINT GetChildDataUINT() const												{ UINT val=0; _stscanf(GetChildData().c_str(),_T("%u"),&val); return  val; };
	BYTE GetChildDataUSHORT() const												{ USHORT val=0; _stscanf(GetChildData().c_str(),_T("%u"),&val); return  (BYTE)val; };
	BYTE GetChildDataBYTE() const												{ BYTE val=0; _stscanf(GetChildData().c_str(),_T("%u"),&val); return  val; };
	DWORD GetChildDataDWORD() const												{ DWORD val=0; _stscanf(GetChildData().c_str(),_T("%u"),&val); return  val; };
	float GetChildDataFloat() const												{ float val=0; _stscanf(GetChildData().c_str(),_T("%f"),&val); return  val; };
	double GetChildDataDouble() const											{ double val=0; _stscanf(GetChildData().c_str(),_T("%lf"),&val); return  val; };
//	COleDateTime GetChildDataDateTime() const									{	COleDateTime t; t.ParseDateTime(GetChildData().c_str()); return t;};
//	COleDateTimeSpan GetChildDataDateTimeSpan() const;
	void GetChildDataEx(std::vector<double>& _v) const							{ CSTL2String<double>::ToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::vector<float>& _v) const							{ CSTL2String<float>::ToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::vector<int>& _v) const								{ CSTL2String<int>::ToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::vector<BYTE>& _v) const							{ CSTL2String<BYTE>::ByteToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::vector<short>& _v) const							{ CSTL2String<short>::ToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::vector<long>& _v) const							{ CSTL2String<long>::ToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::valarray<double>& _v) const						{ CSTL2String<double>::ToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::valarray<float>& _v) const							{ CSTL2String<float>::ToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::valarray<int>& _v) const							{ CSTL2String<int>::ToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::valarray<BYTE>& _v) const							{ CSTL2String<BYTE>::ByteToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::valarray<short>& _v) const							{ CSTL2String<short>::ToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::valarray<long>& _v) const							{ CSTL2String<long>::ToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::list<UINT>& _l) const								{ CSTL2String<UINT>::ToSTL(GetChildData().c_str(),_l);};
	void GetChildDataEx(std::list<double>& _l) const							{ CSTL2String<double>::ToSTL(GetChildData().c_str(),_l);};
	void GetChildDataEx(std::list<float>& _l) const								{ CSTL2String<float>::ToSTL(GetChildData().c_str(),_l);};
	void GetChildDataEx(std::list<int>& _v) const								{ CSTL2String<int>::ToSTL(GetChildData().c_str(),_v);};
	void GetChildDataEx(std::list<BYTE>& _v) const								{ CSTL2String<BYTE>::ByteToSTL(GetChildData().c_str(),_v);};

	bool GetAttribBool(LPCTSTR szAttrib ) const									{ return GetAttrib(szAttrib) == _T("true"); };
	int GetAttribInt(LPCTSTR szAttrib ) const									{ int val=0; _stscanf(GetAttrib(szAttrib).c_str(),_T("%d"),&val); return  val; };
	UINT GetAttribUINT(LPCTSTR szAttrib ) const									{ UINT val=0; _stscanf(GetAttrib(szAttrib).c_str(),_T("%u"),&val); return  val; };
	DWORD GetAttribDWORD(LPCTSTR szAttrib ) const								{ DWORD val=0; _stscanf(GetAttrib(szAttrib).c_str(),_T("%u"),&val); return  val; };
	float GetAttribFloat(LPCTSTR szAttrib ) const								{ float val=0; _stscanf(GetAttrib(szAttrib).c_str(),_T("%f"),&val); return  val; };
	double GetAttribDouble(LPCTSTR szAttrib ) const								{ double val=0; _stscanf(GetAttrib(szAttrib).c_str(),_T("%lf"),&val); return  val; };
//	COleDateTime GetAttribDateTime(LPCTSTR szAttrib ) const						{ 	COleDateTime t; t.ParseDateTime(GetAttrib(szAttrib).c_str()); return t;};
	void GetAttribEx(LPCTSTR szAttrib, std::vector<double>& _v) const			{ CSTL2String<double>::ToSTL(GetAttrib(szAttrib).c_str(),_v);};
	void GetAttribEx(LPCTSTR szAttrib, std::vector<float>& _v) const			{ CSTL2String<float>::ToSTL(GetAttrib(szAttrib).c_str(),_v);};
	void GetAttribEx(LPCTSTR szAttrib, std::vector<int>& _v) const				{ CSTL2String<int>::ToSTL(GetAttrib(szAttrib).c_str(),_v);};
	void GetAttribEx(LPCTSTR szAttrib, std::vector<BYTE>& _v) const				{ CSTL2String<BYTE>::ByteToSTL(GetAttrib(szAttrib).c_str(),_v);};
	void GetAttribEx(LPCTSTR szAttrib, std::vector<short>& _v) const			{ CSTL2String<short>::ToSTL(GetAttrib(szAttrib).c_str(),_v);};
	void GetAttribEx(LPCTSTR szAttrib, std::vector<long>& _v) const				{ CSTL2String<long>::ToSTL(GetAttrib(szAttrib).c_str(),_v);};
	void GetAttribEx(LPCTSTR szAttrib, std::valarray<double>& _l) const			{ CSTL2String<double>::ToSTL(GetAttrib(szAttrib).c_str(),_l);};
	void GetAttribEx(LPCTSTR szAttrib, std::valarray<float>& _l) const			{ CSTL2String<float>::ToSTL(GetAttrib(szAttrib).c_str(),_l);};
	void GetAttribEx(LPCTSTR szAttrib, std::valarray<int>& _l) const			{ CSTL2String<int>::ToSTL(GetAttrib(szAttrib).c_str(),_l);};
	void GetAttribEx(LPCTSTR szAttrib, std::valarray<BYTE>& _l) const			{ CSTL2String<BYTE>::ByteToSTL(GetAttrib(szAttrib).c_str(),_l);};
	void GetAttribEx(LPCTSTR szAttrib, std::list<double>& _l) const				{ CSTL2String<double>::ToSTL(GetAttrib(szAttrib).c_str(),_l);};
	void GetAttribEx(LPCTSTR szAttrib, std::list<float>& _l) const				{ CSTL2String<float>::ToSTL(GetAttrib(szAttrib).c_str(),_l);};
	void GetAttribEx(LPCTSTR szAttrib, std::list<int>& _l) const				{ CSTL2String<int>::ToSTL(GetAttrib(szAttrib).c_str(),_l);};
	void GetAttribEx(LPCTSTR szAttrib, std::list<BYTE>& _l) const				{ CSTL2String<BYTE>::ByteToSTL(GetAttrib(szAttrib).c_str(),_l);};

	bool GetChildAttribBool(LPCTSTR szAttrib ) const							{ return GetChildAttrib(szAttrib) == _T("true"); };
	int GetChildAttribInt(LPCTSTR szAttrib ) const								{ int val=0; _stscanf(GetChildAttrib(szAttrib).c_str(),_T("%d"),&val); return  val; };
	UINT GetChildAttribUINT(LPCTSTR szAttrib ) const							{ UINT val=0; _stscanf(GetChildAttrib(szAttrib).c_str(),_T("%u"),&val); return  val; };
	DWORD GetChildAttribDWORD(LPCTSTR szAttrib ) const							{ DWORD val=0; _stscanf(GetChildAttrib(szAttrib).c_str(),_T("%u"),&val); return  val; };
	float GetChildAttribFloat(LPCTSTR szAttrib ) const							{ float val=0; _stscanf(GetChildAttrib(szAttrib).c_str(),_T("%f"),&val); return  val; };
	double GetChildAttribDouble(LPCTSTR szAttrib ) const						{ double val=0; _stscanf(GetChildAttrib(szAttrib).c_str(),_T("%lf"),&val); return  val; };
//	COleDateTime GetChildAttribDateTime(LPCTSTR szAttrib ) const				{ 	COleDateTime t; t.ParseDateTime(GetChildAttrib(szAttrib).c_str()); return t;};
	void GetChildAttribEx(LPCTSTR szAttrib, std::vector<double>& _v) const		{ CSTL2String<double>::ToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::vector<float>& _v) const		{ CSTL2String<float>::ToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::vector<int>& _v) const			{ CSTL2String<int>::ToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::vector<BYTE>& _v) const		{ CSTL2String<BYTE>::ByteToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::vector<short>& _v) const		{ CSTL2String<short>::ToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::vector<long>& _v) const		{ CSTL2String<long>::ToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::valarray<double>& _v) const	{ CSTL2String<double>::ToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::valarray<float>& _v) const		{ CSTL2String<float>::ToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::valarray<int>& _v) const		{ CSTL2String<int>::ToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::valarray<BYTE>& _v) const		{ CSTL2String<BYTE>::ByteToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::valarray<short>& _v) const		{ CSTL2String<short>::ToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::valarray<long>& _v) const		{ CSTL2String<long>::ToSTL(GetChildAttrib(szAttrib).c_str(),_v);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::list<double>& _l) const		{ CSTL2String<double>::ToSTL(GetChildAttrib(szAttrib).c_str(),_l);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::list<float>& _l) const			{ CSTL2String<float>::ToSTL(GetChildAttrib(szAttrib).c_str(),_l);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::list<int>& _l) const			{ CSTL2String<int>::ToSTL(GetChildAttrib(szAttrib).c_str(),_l);};
	void GetChildAttribEx(LPCTSTR szAttrib, std::list<BYTE>& _l) const			{ CSTL2String<BYTE>::ByteToSTL(GetChildAttrib(szAttrib).c_str(),_l);};	
	//@}

	//! \name Specialized Adders
	//@{
	bool AddElemEx(LPCTSTR szName, bool _bool )									{	return AddElem(szName, (_bool) ? _T("true"):_T("false"));};
	bool AddElemEx(LPCTSTR szName, int _int )									{	m_sTemp.Format(_T("%d"),_int);		return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, UINT _UINT )									{	m_sTemp.Format(_T("%u"),_UINT);		return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, DWORD _DWORD )								{	m_sTemp.Format(_T("%u"),_DWORD);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, long _long )									{	m_sTemp.Format(_T("%d"),_long);		return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, float _float )								{	m_sTemp.Format(_T("%g"),_float);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, double _double )								{	m_sTemp.Format(_T("%g"),_double);	return AddElem(szName,m_sTemp);};
//	bool AddElemEx(LPCTSTR szName, const COleDateTime& _t, LPCTSTR szFormat = _T("%m/%d/%Y %H:%M:%S"))		{	return AddElem(szName,_t.Format(szFormat));};
//	bool AddElemEx(LPCTSTR szName, const COleDateTimeSpan& _ts)					{	return AddElem(szName,_ts.Format(_T("%D,%H:%M:%S")));};
	bool AddElemEx(LPCTSTR szName, const std::vector<double>& _v )				{	CSTL2String<double>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::vector<float>& _v )				{	CSTL2String<float>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::vector<int>& _v )					{	CSTL2String<int>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::vector<BYTE>& _v )				{	CSTL2String<BYTE>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::vector<short>& _v )				{	CSTL2String<short>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::vector<long>& _v )				{	CSTL2String<long>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::valarray<double>& _v )			{	CSTL2String<double>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::valarray<float>& _v )				{	CSTL2String<float>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::valarray<int>& _v )				{	CSTL2String<int>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::valarray<BYTE>& _v )				{	CSTL2String<BYTE>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::valarray<short>& _v )				{	CSTL2String<short>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::valarray<long>& _v )				{	CSTL2String<long>::ToString(_v,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::list<UINT>& _l )					{	CSTL2String<UINT>::ToString(_l,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::list<double>& _l )				{	CSTL2String<double>::ToString(_l,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::list<float>& _l )					{	CSTL2String<float>::ToString(_l,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::list<int>& _l )					{	CSTL2String<int>::ToString(_l,m_sTemp);	return AddElem(szName,m_sTemp);};
	bool AddElemEx(LPCTSTR szName, const std::list<BYTE>& _l )					{	CSTL2String<BYTE>::ToString(_l,m_sTemp);	return AddElem(szName,m_sTemp);};

	bool AddChildElemEx(LPCTSTR szName, bool _bool )							{	return AddChildElem(szName, (_bool) ? _T("true"):_T("false"));};
	bool AddChildElemEx(LPCTSTR szName, int _int )								{	m_sTemp.Format(_T("%d"),_int);		return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, UINT _UINT )							{	m_sTemp.Format(_T("%u"),_UINT);		return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, DWORD _DWORD )							{	m_sTemp.Format(_T("%u"),_DWORD);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, long _long )							{	m_sTemp.Format(_T("%d"),_long);		return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, float _float )							{	m_sTemp.Format(_T("%g"),_float);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, double _double )						{	m_sTemp.Format(_T("%g"),_double);	return AddChildElem(szName,m_sTemp);};
//	bool AddChildElemEx(LPCTSTR szName, const COleDateTime& _t, LPCTSTR szFormat = _T("%m/%d/%Y %H:%M:%S"))		{	return AddChildElem(szName,_t.Format(szFormat));};
//	bool AddChildElemExn(LPCTSTR szName, const COleDateTimeSpan& _ts)			{	return AddChildElem(szName,_ts.Format(_T("%D,%H:%M:%S")));};
	bool AddChildElemEx(LPCTSTR szName, const std::vector<double>& _v)			{	CSTL2String<double>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::vector<float>& _v)			{	CSTL2String<float>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::vector<int>& _v)				{	CSTL2String<int>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::vector<BYTE>& _v)			{	CSTL2String<BYTE>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::vector<short>& _v)			{	CSTL2String<short>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::vector<long>& _v)			{	CSTL2String<long>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::valarray<double>& _v )		{	CSTL2String<double>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::valarray<float>& _v )		{	CSTL2String<float>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::valarray<int>& _v )			{	CSTL2String<int>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::valarray<BYTE>& _v )			{	CSTL2String<BYTE>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::valarray<short>& _v )		{	CSTL2String<short>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::valarray<long>& _v )			{	CSTL2String<long>::ToString(_v,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::list<UINT>& _l)				{	CSTL2String<UINT>::ToString(_l,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::list<double>& _l)			{	CSTL2String<double>::ToString(_l,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::list<float>& _l)				{	CSTL2String<float>::ToString(_l,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::list<int>& _l)				{	CSTL2String<int>::ToString(_l,m_sTemp);	return AddChildElem(szName,m_sTemp);};
	bool AddChildElemEx(LPCTSTR szName, const std::list<BYTE>& _l)				{	CSTL2String<BYTE>::ToString(_l,m_sTemp);	return AddChildElem(szName,m_sTemp);};

	bool AddAttribEx(LPCTSTR szName, bool _bool )								{	return AddAttrib(szName, (_bool) ? _T("true"):_T("false"));};
	bool AddAttribEx(LPCTSTR szName, int _int )									{	m_sTemp.Format(_T("%d"),_int);			return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, UINT _UINT )								{	m_sTemp.Format(_T("%u"),_UINT);		return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, DWORD _DWORD )								{	m_sTemp.Format(_T("%u"),_DWORD);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, long _long )								{	m_sTemp.Format(_T("%d"),_long);		return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, float _float )								{	m_sTemp.Format(_T("%g"),_float);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, double _double )							{	m_sTemp.Format(_T("%g"),_double);	return AddAttrib(szName,m_sTemp);};
//	bool AddAttribEx(LPCTSTR szName, const COleDateTime& _t, LPCTSTR szFormat = _T("%m/%d/%Y %H:%M:%S"))		{	return AddAttrib(szName,_t.Format(szFormat));};
//	bool AddAttribEx(LPCTSTR szName, const COleDateTimeSpan& _ts)				{	return AddAttrib(szName,_ts.Format(_T("%D,%H:%M:%S")));};
	bool AddAttribEx(LPCTSTR szName, const std::vector<double>& _v )			{	CSTL2String<double>::ToString(_v,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::vector<float>& _v )				{	CSTL2String<float>::ToString(_v,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::vector<int>& _v )				{	CSTL2String<int>::ToString(_v,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::vector<BYTE>& _v )				{	CSTL2String<BYTE>::ToString(_v,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::vector<short>& _v )				{	CSTL2String<short>::ToString(_v,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::vector<long>& _v )				{	CSTL2String<long>::ToString(_v,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::valarray<double>& _v )			{	CSTL2String<double>::ToString(_v,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::valarray<float>& _v )			{	CSTL2String<float>::ToString(_v,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::valarray<int>& _v )				{	CSTL2String<int>::ToString(_v,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::valarray<BYTE>& _v )			{	CSTL2String<BYTE>::ToString(_v,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::list<UINT>& _l )				{	CSTL2String<UINT>::ToString(_l,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::list<double>& _l )				{	CSTL2String<double>::ToString(_l,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::list<float>& _l )				{	CSTL2String<float>::ToString(_l,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::list<int>& _l )					{	CSTL2String<int>::ToString(_l,m_sTemp);	return AddAttrib(szName,m_sTemp);};
	bool AddAttribEx(LPCTSTR szName, const std::list<BYTE>& _l )				{	CSTL2String<BYTE>::ToString(_l,m_sTemp);	return AddAttrib(szName,m_sTemp);};

	bool AddChildAttribEx(LPCTSTR szName, bool _bool )							{	return AddChildAttrib(szName, (_bool) ? _T("true"):_T("false"));};
	bool AddChildAttribEx(LPCTSTR szName, int _int )							{	m_sTemp.Format(_T("%d"),_int);		return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, UINT _UINT )							{	m_sTemp.Format(_T("%u"),_UINT);		return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, DWORD _DWORD )						{	m_sTemp.Format(_T("%u"),_DWORD);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, long _long )							{	m_sTemp.Format(_T("%d"),_long);		return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, float _float )						{	m_sTemp.Format(_T("%g"),_float);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, double _double )						{	m_sTemp.Format(_T("%g"),_double);	return AddChildAttrib(szName,m_sTemp);};
//	bool AddChildAttribEx(LPCTSTR szName, const COleDateTime& _t, LPCTSTR szFormat = _T("%m/%d/%Y %H:%M:%S"))		{	return AddChildAttrib(szName,_t.Format(szFormat));};
//	bool AddChildAttribEx(LPCTSTR szName, const COleDateTimeSpan& _ts)			{	return AddChildAttrib(szName,_ts.Format(_T("%D,%H:%M:%S")));};
	bool AddChildAttribEx(LPCTSTR szName, const std::vector<double>& _v )		{	CSTL2String<double>::ToString(_v,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::vector<float>& _v )		{	CSTL2String<float>::ToString(_v,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::vector<int>& _v )			{	CSTL2String<int>::ToString(_v,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::vector<BYTE>& _v )			{	CSTL2String<BYTE>::ToString(_v,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::vector<short>& _v )		{	CSTL2String<short>::ToString(_v,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::vector<long>& _v )			{	CSTL2String<long>::ToString(_v,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::valarray<double>& _v )		{	CSTL2String<double>::ToString(_v,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::valarray<float>& _v )		{	CSTL2String<float>::ToString(_v,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::valarray<int>& _v )		{	CSTL2String<int>::ToString(_v,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::valarray<BYTE>& _v )		{	CSTL2String<BYTE>::ToString(_v,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::list<UINT>& _l )			{	CSTL2String<UINT>::ToString(_l,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::list<double>& _l )			{	CSTL2String<double>::ToString(_l,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::list<float>& _l )			{	CSTL2String<float>::ToString(_l,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::list<int>& _l )			{	CSTL2String<int>::ToString(_l,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	bool AddChildAttribEx(LPCTSTR szName, const std::list<BYTE>& _l )			{	CSTL2String<BYTE>::ToString(_l,m_sTemp);	return AddChildAttrib(szName,m_sTemp);};
	//@}

	//! \name Finders + getters
	//@{
	bool FindGetData(LPCTSTR szName, std::string& _string);
	bool FindGetData(LPCTSTR szName, bool& _bool);
	bool FindGetData(LPCTSTR szName, int& _int);
	bool FindGetData(LPCTSTR szName, USHORT& _USHORT);
	bool FindGetData(LPCTSTR szName, UINT& _UINT);
	bool FindGetData(LPCTSTR szName, BYTE& _BYTE);
	bool FindGetData(LPCTSTR szName, DWORD& _DWORD);
	bool FindGetData(LPCTSTR szName, float& _float);
	bool FindGetData(LPCTSTR szName, double& _double);
//	bool FindGetData(LPCTSTR szName, COleDateTime& _t);
//	bool FindGetData(LPCTSTR szName, COleDateTimeSpan& _ts);
	bool FindGetData(LPCTSTR szName, std::vector<double>& _vDouble);
	bool FindGetData(LPCTSTR szName, std::vector<float>& _vFloat);
	bool FindGetData(LPCTSTR szName, std::vector<int>& _vInt);
	bool FindGetData(LPCTSTR szName, std::vector<BYTE>& _vBYTE);
	bool FindGetData(LPCTSTR szName, std::vector<short>& _vShort);
	bool FindGetData(LPCTSTR szName, std::vector<long>& _vLong);
	bool FindGetData(LPCTSTR szName, std::valarray<double>& _vDouble);
	bool FindGetData(LPCTSTR szName, std::valarray<float>& _vFloat);
	bool FindGetData(LPCTSTR szName, std::valarray<int>& _vInt);
	bool FindGetData(LPCTSTR szName, std::valarray<BYTE>& _vBYTE);
	bool FindGetData(LPCTSTR szName, std::valarray<short>& _vShort);
	bool FindGetData(LPCTSTR szName, std::valarray<long>& _vLong);
	bool FindGetData(LPCTSTR szName, std::list<UINT>& _lUINT);
	bool FindGetData(LPCTSTR szName, std::list<double>& _lDouble);
	bool FindGetData(LPCTSTR szName, std::list<float>& _lFloat);
	bool FindGetData(LPCTSTR szName, std::list<int>& _lInt);
	bool FindGetData(LPCTSTR szName, std::list<BYTE>& _lByte);

	bool FindGetChildData(LPCTSTR szName, std::string& _string);
	bool FindGetChildData(LPCTSTR szName, bool& _bool);
	bool FindGetChildData(LPCTSTR szName, int& _int);
	bool FindGetChildData(LPCTSTR szName, UINT& _UINT);
	bool FindGetChildData(LPCTSTR szName, USHORT& _USHORT);
	bool FindGetChildData(LPCTSTR szName, BYTE& _BYTE);
	bool FindGetChildData(LPCTSTR szName, DWORD& _DWORD);
	bool FindGetChildData(LPCTSTR szName, float& _float);
	bool FindGetChildData(LPCTSTR szName, long& _long);
	bool FindGetChildData(LPCTSTR szName, double& _double);
//	bool FindGetChildData(LPCTSTR szName, COleDateTime& _t);
//	bool FindGetChildData(LPCTSTR szName, COleDateTimeSpan& _ts);
	bool FindGetChildData(LPCTSTR szName, std::vector<double>& _vDouble);
	bool FindGetChildData(LPCTSTR szName, std::vector<float>& _vFloat);
	bool FindGetChildData(LPCTSTR szName, std::vector<int>& _vInt);
	bool FindGetChildData(LPCTSTR szName, std::vector<BYTE>& _vBYTE);
	bool FindGetChildData(LPCTSTR szName, std::vector<short>& _vShort);
	bool FindGetChildData(LPCTSTR szName, std::vector<long>& _vLong);
	bool FindGetChildData(LPCTSTR szName, std::valarray<double>& _vDouble);
	bool FindGetChildData(LPCTSTR szName, std::valarray<float>& _vFloat);
	bool FindGetChildData(LPCTSTR szName, std::valarray<int>& _vInt);
	bool FindGetChildData(LPCTSTR szName, std::valarray<BYTE>& _vByte);
	bool FindGetChildData(LPCTSTR szName, std::valarray<short>& _vShort);
	bool FindGetChildData(LPCTSTR szName, std::valarray<long>& _vLong);
	bool FindGetChildData(LPCTSTR szName, std::list<UINT>& _lUINT);
	bool FindGetChildData(LPCTSTR szName, std::list<double>& _lDouble);
	bool FindGetChildData(LPCTSTR szName, std::list<float>& _lFloat);
	bool FindGetChildData(LPCTSTR szName, std::list<int>& _lInt);
	bool FindGetChildData(LPCTSTR szName, std::list<BYTE>& _lBYTE);
	//@}

	//! \name Base class override to handle namespace
	//@{
	virtual bool AddElem( LPCTSTR szName, LPCTSTR szData=NULL )				
	{	
		if (m_sNameSpace.empty()) 
			return CMarkupSTL::AddElem( szName, szData); 
		else 
			return CMarkupSTL::AddElem( m_sNameSpace+szName, szData);
	};

	virtual bool AddChildElem( LPCTSTR szName, LPCTSTR szData=NULL )		
	{	
		if (m_sNameSpace.empty()) 
			return CMarkupSTL::AddChildElem( szName, szData); 
		else 
			return CMarkupSTL::AddChildElem( m_sNameSpace+szName, szData);
	};

	virtual bool FindElem( LPCTSTR szName=NULL )							
	{	
		if (szName==NULL) 
			return CMarkupSTL::FindElem( NULL); 
		else 
			if (m_sNameSpace.empty())
				return CMarkupSTL::FindElem(szName);
			else
				return CMarkupSTL::FindElem( m_sNameSpace+szName);
	};

	virtual bool FindChildElem( LPCTSTR szName=NULL )
	{	
		if (szName==NULL) 
			return CMarkupSTL::FindElem( NULL); 
		else 
			if (m_sNameSpace.empty())
				return CMarkupSTL::FindChildElem(szName);
			else
				return CMarkupSTL::FindChildElem( m_sNameSpace+szName);
	};

	bool FindElemAttrib( LPCTSTR szName, LPCTSTR szAttrib, LPCTSTR szAttribValue );
	bool FindChildElemAttrib( LPCTSTR szName , LPCTSTR szAttrib, LPCTSTR szAttribValue );
	//@}

protected:
	//! \name Attributes
	//@{
	//! Falgs
	DWORD m_dwMode;
	//! encoding string
	CString m_strEncodingString;
	//! true if already opened
	bool m_bOpened;
	//! Output-input filename
	CString m_sFileName;
	//! Current namespace, can be empty
	CString m_sNameSpace;
	//@}
private:
//	std::string m_sTemp;

	CString m_sTemp;
};

};

#endif // !defined(AFX_PGLMARKUP_H__3BB8B027_BBD6_4DCF_ACDE_7F1922437BEB__INCLUDED_)
