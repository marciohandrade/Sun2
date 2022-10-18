#if !defined(AFX_SQLAPI_H__BC064F47_40FC_45B6_AA67_2E6D2568E219__INCLUDED_)
#define AFX_SQLAPI_H__BC064F47_40FC_45B6_AA67_2E6D2568E219__INCLUDED_

#define sa_min(x, y) ((x) < (y)? (x) : (y))
#define sa_max(x, y) ((x) > (y)? (x) : (y))

#ifdef SQLAPI_EXPORTS
	#define SQLAPI_API __declspec(dllexport)
#else
	#define SQLAPI_API
#endif

#include <time.h>
#include <stdarg.h>

class ISAClient;
class ISAConnection;
class ISACursor;

class SAMutex;

class SACommand;
struct sa_Commands;
class saOptions;
class SAParam;
class SAField;

class saPlaceHolder;

class SABytes;
class SALongBinary;
class SALongChar;
class SABLob;
class SACLob;
class SAValueRead;

typedef
enum
{
	SA_Client_NotSpecified,
	SA_ODBC_Client,
	SA_Oracle_Client,
	SA_SQLServer_Client,
	SA_InterBase_Client,
	SA_SQLBase_Client,
	SA_DB2_Client,
	SA_Informix_Client,
	SA_Sybase_Client,
	SA_MySQL_Client,
	SA_PostgreSQL_Client,
	_SA_Client_Reserverd = (int)(((unsigned int)(-1))/2)
} SAClient_t;

typedef
enum
{
	SA_No_Error,
	SA_UserGenerated_Error,
	SA_Library_Error,
	SA_DBMS_API_Error,
	_SA_ErrorClass_Reserved = (int)(((unsigned int)(-1))/2)
} SAErrorClass_t;

typedef
enum
{
	SA_LevelUnknown = -1,	//  the default
	SA_ANSILevel0,
	SA_ANSILevel1,
	SA_ANSILevel2,
	SA_ANSILevel3,
	SA_ReadUncommitted = SA_ANSILevel0,
	SA_ReadCommitted = SA_ANSILevel1,
	SA_RepeatableRead = SA_ANSILevel2,
	SA_Serializable = SA_ANSILevel3,
	_SA_IsolationLevel_Reserved = (int)(((unsigned int)(-1))/2)
} SAIsolationLevel_t;

typedef
enum
{
	SA_AutoCommitUnknown = -1,
	SA_AutoCommitOff,
	SA_AutoCommitOn,
	_SA_AutoCommit_Reserved = (int)(((unsigned int)(-1))/2)
} SAAutoCommit_t;

typedef
enum
{
	SA_dtUnknown,
	SA_dtBool,
	SA_dtShort,
	SA_dtLong,
	SA_dtDouble,
	SA_dtNumeric,
	SA_dtDateTime,
	SA_dtString,
	SA_dtBytes,
	SA_dtLongBinary,
	SA_dtLongChar,
	SA_dtBLob,
	SA_dtCLob,
	SA_dtCursor,
	SA_dtSpecificToDBMS,
	_SA_DataType_Reserved = (int)(((unsigned int)(-1))/2)
} SADataType_t;

typedef
enum
{
	SA_CmdUnknown,
	SA_CmdSQLStmt,
	SA_CmdStoredProc,
	_SA_Cmd_Reserved = (int)(((unsigned int)(-1))/2)
}
SACommandType_t;

typedef
enum
{
	SA_ParamInput,
	SA_ParamInputOutput,
	SA_ParamOutput,
	SA_ParamReturn,
	_SA_ParamDirType_Reserved = (int)(((unsigned int)(-1))/2)
} SAParamDirType_t;

typedef
enum
{
	SA_LongOrLobReaderDefault,
	SA_LongOrLobReaderManual,
	_SA_LongOrLobReaderModes_Reserved = (int)(((unsigned int)(-1))/2)
} SALongOrLobReaderModes_t;

typedef
enum {
	SA_FirstPiece = 1,
	SA_NextPiece = 2,
	SA_LastPiece = 3,
    SA_OnePiece = 4,
	_SA_Reserved_PieceType = (int)(((unsigned int)(-1))/2)
} SAPieceType_t;

// Long or Lob writer callback, use for parameter binding
typedef unsigned int (*saLongOrLobWriter_t)
	(SAPieceType_t &ePieceType, void *pBuf, unsigned int nLen, void *pAddlData);

// Long or Lob reader callback, use for field fetching
typedef void (*saLongOrLobReader_t)
	(SAPieceType_t ePieceType, void *pBuf, unsigned int nLen, unsigned int nBlobSize, void *pAddlData);

class SQLAPI_API saAPI
{
};

class SQLAPI_API saConnectionHandles
{
};

class SQLAPI_API saCommandHandles
{
};

#if defined(SA_UNICODE)
typedef wchar_t SAChar;
#define _TSA(x)      L ## x
#define sa_strlen(x) wcslen(x)
#define sa_strstr(x, y) wcsstr(x, y)
#define sa_strchr(x, y) wcschr(x, y)
#define sa_isspace(x) iswspace(x)
#define sa_isdigit(x) iswdigit(x)
#define sa_isalpha(x) iswalpha(x)
#define sa_strcmp(x, y) wcscmp(x, y)
#define sa_strncmp(x, y, z) wcsncmp(x, y, z)
#define sa_tolower(x) towlower(x)
#define sa_strcoll(x, y) wcscoll(x, y)
#define sa_strpbrk(x, y) wcspbrk(x, y)
#define sa_strrchr(x, y) wcsrchr(x, y)
#define sa_strtol(x, y, z) wcstol(x, y, z)
#define sa_strtoul(x, y, z) wcstoul(x, y, z)
#define sa_strtod(x, y) wcstod(x, y)
#define sa_toi(x) (int)wcstol(x, NULL, 10)
#define sa_tol(x) wcstol(x, NULL, 10)
#define sa_vsprintf(x, y, z) vswprintf(x, y, z)
#define sa_sprintf swprintf
#define sa_csinc(x) (++(x))
#define sa_clen(x) (1)
#else
typedef char SAChar;
#define _TSA(x)      x
#define sa_strlen(x) strlen(x)
#define sa_strstr(x, y) strstr(x, y)
#define sa_strchr(x, y) strchr(x, y)
#define sa_isspace(x) isspace((unsigned char)x)
#define sa_isdigit(x) isdigit((unsigned char)x)
#define sa_isalpha(x) isalpha((unsigned char)x)
#define sa_strcmp(x, y) strcmp(x, y)
#define sa_strncmp(x, y, z) strncmp(x, y, z)
#define sa_tolower(x) tolower((unsigned char)x)
#define sa_strcoll(x, y) strcoll(x, y)
#define sa_strpbrk(x, y) strpbrk(x, y)
#define sa_strrchr(x, y) strrchr(x, y)
#define sa_strtol(x, y, z) strtol(x, y, z)
#define sa_strtoul(x, y, z) strtoul(x, y, z)
#define sa_strtod(x, y) strtod(x, y)
#define sa_toi(x) atoi(x)
#define sa_tol(x) atol(x)
#define sa_vsprintf(x, y, z) vsprintf(x, y, z)
#define sa_sprintf sprintf
#define sa_csinc(x) (++(x))
#define sa_clen(x) (1)
#endif

struct SAStringConvertedData
{
	int nDataLength;        // length of converted data (including terminator)
	// !SAChar ConvertedData[nDataLength+1]

#ifdef SA_UNICODE
	char *data()		// char * to converted data
		{ return (char *)(this+1); }
#else	// !SA_UNICODE
	wchar_t *data()	// wchar_t * to converted data
		{ return (wchar_t *)(this+1); }
#endif	//!SA_UNICODE
};

struct SAStringData
{
	SAStringConvertedData	*pConvertedData;	// pointer to converted data, if any

#ifdef SA_UNICODE
	// nDataLength*sizeof(SAChar) - real length of binary data
	// also true: nLengthInBytes % sizeof(SAChar)
	// In bytes [0..sizeof(SAChar)-1]
	int nBinaryDataLengthDiff;
#endif	// SA_UNICODE

	long nRefs;				// reference count
	int nDataLength;		// length of data (including terminator)
	int nAllocLength;		// length of allocation
	// SAChar data[nAllocLength]

	SAChar *data()           // SAChar * to managed data
		{ return (SAChar *)(this+1); }
};

class SQLAPI_API SAString
{
public:
// Constructors
	// constructs empty SAString
	SAString();
	// copy constructor
	SAString(const SAString &stringSrc);
	// from a single character
	SAString(SAChar ch, int nRepeat = 1);
	// from an ANSI (multibyte) string (converts to SAChar)
	SAString(const char *lpsz);
	// from a UNICODE string (converts to SAChar)
	SAString(const wchar_t *lpsz);
	// subset of characters from an ANSI (multibyte) string (converts to SAChar)
	SAString(const char *lpch, int nLength);
	// subset of characters from a UNICODE string (converts to SAChar)
	SAString(const wchar_t *lpch, int nLength);
	// from unsigned characters (converts to SAChar)
	SAString(const unsigned char *psz);
	// special constructor for binary data (no converion to SAChar)
	SAString(const void *pBuffer, int nLengthInBytes);

// Attributes & Operations

	// get data length (in characters)
	int GetLength() const;
	// true if zero length
	bool IsEmpty() const;
	// clear contents to empty
	void Empty();

	// return pointer to const string
	operator const SAChar *() const;

	// overloaded assignment

	// ref-counted copy from another SAString
	const SAString &operator =(const SAString &sSrc);
	// set string content to single character
	const SAString &operator=(SAChar ch);
#ifdef SA_UNICODE
	const SAString &operator=(char ch);
#endif
	// copy string content from ANSI (multibyte) string (converts to SAChar)
	const SAString &operator=(const char *lpsz);
	// copy string content from UNICODE string (converts to SAChar)
	const SAString &operator=(const wchar_t *lpsz);
	// copy string content from unsigned chars
	const SAString &operator=(const unsigned char *psz);

	// string concatenation

	// concatenate from another SAString
	const SAString &operator+=(const SAString &string);
	// concatenate a single character
	const SAString &operator+=(SAChar ch);
#ifdef SA_UNICODE
	// concatenate an ANSI character after converting it to SAChar
	const SAString &operator+=(char ch);
#endif
	// concatenate from a SAChar string
	const SAString &operator+=(const SAChar *lpsz);

	friend SAString SQLAPI_API operator+(const SAString &string1, const SAString &string2);
	friend SAString SQLAPI_API operator+(const SAString &string, SAChar ch);
	friend SAString SQLAPI_API operator+(SAChar ch, const SAString &string);
#ifdef SA_UNICODE
	friend SAString SQLAPI_API operator+(const SAString &string, char ch);
	friend SAString SQLAPI_API operator+(char ch, const SAString &string);
#endif
	friend SAString SQLAPI_API operator+(const SAString &string, const SAChar *lpsz);
	friend SAString SQLAPI_API operator+(const SAChar *lpsz, const SAString &string);

	// string comparison

	// straight character comparison
	int Compare(const SAChar *lpsz) const;
	// compare ignoring case
	int CompareNoCase(const SAChar *lpsz) const;
	// NLS aware comparison, case sensitive
	int Collate(const SAChar *lpsz) const;

	// simple sub-string extraction

	// return all characters starting at zero-based nFirst
	SAString Mid(int nFirst) const;
	// return nCount characters starting at zero-based nFirst
	SAString Mid(int nFirst, int nCount) const;
	// return first nCount characters in string
	SAString Left(int nCount) const;
	// return nCount characters from end of string
	SAString Right(int nCount) const;

	// trimming whitespace (either side)

	// remove whitespace starting from right edge
	void TrimRight();
	// remove whitespace starting from left side
	void TrimLeft();

	// trimming anything (either side)

	// remove continuous occurrences of chTarget starting from right
	void TrimRight(SAChar chTarget);
	// remove continuous occcurrences of characters in passed string,
	// starting from right
	void TrimRight(const SAChar *lpszTargets);
	// remove continuous occurrences of chTarget starting from left
	void TrimLeft(SAChar chTarget);
	// remove continuous occcurrences of characters in
	// passed string, starting from left
	void TrimLeft(const SAChar *lpszTargets);

	// advanced manipulation

	// replace occurrences of substring lpszOld with lpszNew;
	// empty lpszNew removes instances of lpszOld
	int Replace(const SAChar *lpszOld, const SAChar *lpszNew);
	// insert character at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, SAChar ch);
	// insert substring at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, const SAChar *pstr);
	// delete nCount characters starting at zero-based index
	int Delete(int nIndex, int nCount = 1);

	// searching

	// find character starting at left, -1 if not found
	int Find(SAChar ch) const;
	// find character starting at right
	int ReverseFind(SAChar ch) const;
	// find character starting at zero-based index and going right
	int Find(SAChar ch, int nStart) const;
	// find first instance of any character in passed string
	int FindOneOf(const SAChar *lpszCharSet) const;
	// find first instance of substring
	int Find(const SAChar *lpszSub) const;
	// find first instance of substring starting at zero-based index
	int Find(const SAChar *lpszSub, int nStart) const;

	// simple formatting

	// printf-like formatting using passed string
	void Format(const SAChar *lpszFormat, ...);
	// printf-like formatting using variable arguments parameter
	void FormatV(const SAChar *, va_list argList);

	// Access to string implementation buffer as "C" character array

	// get pointer to modifiable buffer at least as long as nMinBufLength
	SAChar *GetBuffer(int nMinBufLength);
	// release buffer, setting length to nNewLength (or to first nul if -1)
	void ReleaseBuffer(int nNewLength = -1);

	// Use LockBuffer/UnlockBuffer to turn refcounting off

	// turn refcounting off
	SAChar *LockBuffer();
	// turn refcounting back on
	void UnlockBuffer();

// Special buffer access routines to manipulate binary data

	// get binary data length (in bytes)
	int GetBinaryLength() const;
	// return pointer to const binary data buffer
	operator const void *() const;
	// get pointer to modifiable binary data buffer at least as long as nMinBufLengthInBytes
	void *GetBinaryBuffer(int nMinBufLengthInBytes);
	// release buffer, setting length to nNewLength (or to first nul if -1)
	void ReleaseBinaryBuffer(int nNewLengthInBytes);

// Special conversion functions (multibyte <-> Unicode)
#ifdef SA_UNICODE
	// return pointer to const multibyte string, convert if needed
	const char *GetMultiByteChars() const;
	// get string length (in multibyte characters)
	int GetMultiByteCharsLength() const;
#define GetWideChars operator const SAChar *
#define GetWideCharsLength GetLength
#else	// !SA_UNICODE
#define GetMultiByteChars operator const SAChar *
#define GetMultiByteCharsLength GetLength
	// return pointer to const Unicode string, convert if needed
	const wchar_t *GetWideChars() const;
	// get string length (in Unicode characters)
	int GetWideCharsLength() const;
#endif	// !SA_UNICODE

// Implementation
public:
	~SAString();

protected:
	SAChar *m_pchData;	// pointer to ref counted string data

	// implementation helpers
	SAStringData *GetData() const;
	void Init();
	void AllocBuffer(int nLen);
#ifdef SA_UNICODE
	void AssignBinaryCopy(int nSrcLenInBytes, const void *pSrcData);
	void ConcatBinaryCopy(int nSrc1LenInBytes, const void *pSrc1Data, int nSrc2LenInBytes, const void *pSrc2Data);
	void ConcatBinaryInPlace(int nSrcLen, const void *pData);
#endif	// SA_UNICODE
	void AssignCopy(int nSrcLen, const SAChar *lpszSrcData);
	void ConcatCopy(int nSrc1Len, const SAChar *lpszSrc1Data, int nSrc2Len, const SAChar *lpszSrc2Data);
	void ConcatInPlace(int nSrcLen, const SAChar *lpszSrcData);

	void CopyBeforeWrite();
	void AllocBeforeWrite(int nLen);
	void Release();
	static void Release(SAStringData *pData);
	static int SafeStrlen(const SAChar *lpsz);
	static void FreeData(SAStringData *pData);
};

// Compare helpers
bool SQLAPI_API operator==(const SAString &s1, const SAString &s2);
bool SQLAPI_API operator==(const SAString &s1, const SAChar *s2);
bool SQLAPI_API operator==(const SAChar *s1, const SAString &s2);
bool SQLAPI_API operator!=(const SAString &s1, const SAString &s2);
bool SQLAPI_API operator!=(const SAString &s1, const SAChar *s2);
bool SQLAPI_API operator!=(const SAChar *s1, const SAString &s2);
bool SQLAPI_API operator<(const SAString &s1, const SAString &s2);
bool SQLAPI_API operator<(const SAString &s1, const SAChar *s2);
bool SQLAPI_API operator<(const SAChar *s1, const SAString &s2);
bool SQLAPI_API operator>(const SAString &s1, const SAString &s2);
bool SQLAPI_API operator>(const SAString &s1, const SAChar *s2);
bool SQLAPI_API operator>(const SAChar *s1, const SAString &s2);
bool SQLAPI_API operator<=(const SAString &s1, const SAString &s2);
bool SQLAPI_API operator<=(const SAString &s1, const SAChar *s2);
bool SQLAPI_API operator<=(const SAChar *s1, const SAString &s2);
bool SQLAPI_API operator>=(const SAString &s1, const SAString &s2);
bool SQLAPI_API operator>=(const SAString &s1, const SAChar *s2);
bool SQLAPI_API operator>=(const SAChar *s1, const SAString &s2);


class SQLAPI_API SANull
{
};

#define SA_NUMERIC_MANTISSA_SIZE 32
class SQLAPI_API SANumeric
{
	void InitZero();
	void setFromPlainString(const SAChar *s);
	void setFromExpString(const SAString &s);

public:
	SANumeric();	// default constructor, initializes to zero

	SANumeric(double d);			// initializes from double
	SANumeric &operator=(double);	// reinitializes from double
	operator double() const;		// converts to double

	SANumeric &operator=(const SAChar *s);	// reinitializes from string
	operator SAString() const;	// converts to string

public:
	unsigned char precision;	// the maximum number of digits in base 10
	unsigned char scale;		// the number of digits to the right of the decimal point
	unsigned char sign;			// the sign: 1 for positive numbers, 0 for negative numbers

	// a number stored as SA_NUMERIC_MANTISSA_SIZE-byte scaled integer, with the least-significant byte on the left
	unsigned char val[SA_NUMERIC_MANTISSA_SIZE];
};

class SQLAPI_API SADateTime
{
	static int m_saMonthDays[13];

protected:
	static bool DateFromTm(
		unsigned short wYear, unsigned short wMonth, unsigned short wDay,
		unsigned short wHour, unsigned short wMinute, unsigned short wSecond,
		unsigned long nNanoSecond,
		double &dtDest);
	static bool TmFromDate(
		double dtSrc,
		struct tm &tmDest, unsigned long &nNanoSecond);

protected:
	void Init_Tm();
	struct tm m_tm;
	unsigned long m_nFraction;	// 0..999999999

	int	nReserved;

public:
	SADateTime();
	SADateTime(const SADateTime &other);

	SADateTime(const struct tm &tmValue);
	operator struct tm &();
	operator struct tm() const;

	SADateTime(double dt);
	operator double() const;

	SADateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);

	int GetYear() const;		// year, f.ex., 1999, 2000
	int GetMonth() const;		// 1..12
	int GetDay() const;			// 1..31
	int GetHour() const;		// 0..23
	int GetMinute() const;		// 0..59
	int GetSecond() const;		// 0..59
	int GetDayOfWeek() const;	// 1..7, 1=Sunday, 2=Monday, and so on
	int GetDayOfYear() const;	// 1..366, where January 1 = 1

	unsigned long &Fraction();
	unsigned long Fraction() const;

	// return the current date/time value
	static SADateTime currentDateTime();
};

class SQLAPI_API SAPos
{
	friend class SACommand;

	SAString m_sName;

public:
	SAPos(int nByID);
	SAPos(const SAString& sByName);
};

class SQLAPI_API saOptions
{
	int		m_nOptionCount;
	SAParam	**m_ppOptions;

private:
	// disable copy constructor
	saOptions(const saOptions &);
	// disable assignment operator
	saOptions &operator = (const saOptions &);

public:
	saOptions();
	virtual ~saOptions();

	SAString &operator[](const SAString &sOptionName);
	SAString operator[](const SAString &sOptionName) const;
};

typedef void (*EnumCursors_t)(ISACursor *, void *);

class SQLAPI_API SAConnection
{
	friend class SACommand;
	friend class SAField;
	friend class SAParam;
	friend class ISAConnection;

private:
	// disable copy constructor
	SAConnection(const SAConnection &);
	// disable assignment operator
	SAConnection &operator = (const SAConnection &);

	SAClient_t m_eSAClient;
	ISAConnection *m_pISAConnection;
	SAMutex *m_pCommandsMutex;
	sa_Commands *m_pCommands;

	SAIsolationLevel_t m_eIsolationLevel;
	SAAutoCommit_t m_eAutoCommit;

	saOptions m_Options;

	int	nReserved;

protected:
	void EnumCursors(EnumCursors_t fn, void *pAddlData);
	void RegisterCommand(SACommand *pCommand);
	void UnRegisterCommand(SACommand *pCommand);
	ISACursor *GetISACursor(SACommand *pCommand);

public:
	SAConnection();
	virtual ~SAConnection();

	void setClient(SAClient_t eSAClient);
	SAClient_t Client() const;
	long ClientVersion() const;
	long ServerVersion() const;
	SAString ServerVersionString() const;

	bool isConnected() const;
	void Connect(
		const SAString &sDBString,
		const SAString &sUserID,
		const SAString &sPassword,
		SAClient_t eSAClient = SA_Client_NotSpecified);
	void Disconnect();

	void setIsolationLevel(SAIsolationLevel_t eIsolationLevel);
	SAIsolationLevel_t IsolationLevel() const;
	void setAutoCommit(SAAutoCommit_t eAutoCommit);
	SAAutoCommit_t AutoCommit() const;
	void Commit();
	void Rollback();

	SAString &setOption(const SAString &sOptionName);
	SAString Option(const SAString &sOptionName) const;

	saAPI *NativeAPI() const;
	saConnectionHandles *NativeHandles();
};

class SQLAPI_API SACommand
{
	friend class SAConnection;
	friend class IibCursor;
	friend class IsybCursor;
	friend class IssDBLibCursor;
	friend class IsbCursor;

private:
	// disable copy constructor
	SACommand(const SACommand &);
	// disable assignment operator
	SACommand &operator = (const SACommand &);

	SAConnection	*m_pConnection;

	SACommandType_t	m_eCmdType;
	SAString		m_sCmd;
	bool			m_bPrepared;
	bool			m_bExecuted;
	bool			m_bFieldsDescribed;
	bool			m_bSelectBuffersSet;

	bool			m_bParamsKnown;
	int				m_nPlaceHolderCount;
	saPlaceHolder	**m_ppPlaceHolders;
	int				m_nParamCount;
	SAParam			**m_ppParams;
	int				m_nCurParamID;
	SAString		m_sCurParamName;
	
	int				m_nFieldCount;
	SAField			**m_ppFields;

	saOptions		m_Options;

	int	nReserved;

	void Init();
	SAParam &CreateParam(
		const SAString &sName,
		SADataType_t eParamType,
		int nNativeType,
		int nParamSize,
		int	nParamPrecision,
		int	nParamScale,
		SAParamDirType_t eDirType,
		const SAString &sFullName,
		int nStart,	// param position in SQL statement
		int nEnd);	// param end position in SQL statemen
	void GetParamsSP();
	void UnDescribeParams();
	void ParseInputMarkers(
		const SAString &sCmd,
		bool *pbSpacesInCmd);

	void DescribeFields();
	void CreateField(
		const SAString &sName,
		SADataType_t eFieldType,
		int nNativeType,
		int nFieldSize,
		int nFieldPrecision,
		int nFieldScale,
		bool bFieldRequired);
	void DestroyFields();

	// parses sql statement and create bind parameters array if any (In)
	// also cancels previous stsement if any
	void ParseCmd(
		const SAString &sSQL,
		SACommandType_t eCmdType);

	void UnSetCommandText();
	void UnPrepare();
	void UnExecute();

public:
	SACommand();	// construct command with no associated connection and SQL
	SACommand(		// construct command based on the given connection and SQL
		SAConnection *pConnection,
		const SAString &sCmd = SAString(),
		SACommandType_t eCmdType = SA_CmdUnknown);
	
	virtual ~SACommand();

	SAConnection *Connection() const;
	void setConnection(SAConnection *pConnection);

	virtual void Open();
	virtual bool isOpened();
	virtual void Close();

	void setCommandText(
		const SAString &sSQL,
		SACommandType_t eCmdType = SA_CmdUnknown);
	SAString CommandText() const;
	SACommandType_t CommandType() const;
	virtual void Prepare();
	virtual void Execute();
	bool isResultSet();
	long RowsAffected();	// returns number of rows affected by last DML operation
	bool FetchNext();	// returns true if new row is fetched
	void Cancel();

	SAParam &CreateParam(
		const SAString &sName,
		SADataType_t eParamType,
		int nNativeType,
		int nParamSize,
		int	nParamPrecision,
		int	nParamScale,
		SAParamDirType_t eDirType);
	void DestroyParams();
	int ParamCount();
	SAParam &ParamByIndex(int i);	// zero based index of C array
	SAParam &Param(int nParamByID);	// id in SQL statement, not in C array
	SAParam &Param(const SAString& sParamByName);
	SACommand &operator << (const SAPos &pos);
	SACommand &operator << (const SANull &null);
	SACommand &operator << (bool Value);
	SACommand &operator << (short Value);
	SACommand &operator << (long Value);
	SACommand &operator << (double Value);
	SACommand &operator << (const SANumeric &Value);
	SACommand &operator << (const SADateTime &Value);
	SACommand &operator << (const SAChar *Value);	// special overload for string constants
	SACommand &operator << (const SAString &Value);
	SACommand &operator << (const SABytes &Value);
	SACommand &operator << (const SALongBinary &Value);
	SACommand &operator << (const SALongChar &Value);
	SACommand &operator << (const SABLob &Value);
	SACommand &operator << (const SACLob &Value);
	SACommand &operator << (const SAValueRead &Value);

	int FieldCount();
	SAField &Field(int nField);	// 1-based field number in result set
	SAField &Field(const SAString &sField);
	SAField &operator[](int nField);	// 1-based field number in result set
	SAField &operator[](const SAString &sField);

	SAString &setOption(const SAString &sOptionName);
	SAString Option(const SAString &sOptionName) const;

	saCommandHandles *NativeHandles();
};

class SQLAPI_API SAValueRead
{
	friend class ISACursor;
	friend class IibCursor;
	friend class Iora7Cursor;
	friend class Iora8Cursor;
	friend class IsbCursor;
	friend class IodbcCursor;
	friend class IssDBLibCursor;
	friend class IssOleDbCursor;
	friend class Idb2Cursor;
	friend class IinfCursor;
	friend class IsybCursor;
	friend class ImyCursor;
	friend class IpgCursor;

protected:
	SALongOrLobReaderModes_t m_eReaderMode;
	
	saLongOrLobReader_t	m_fnReader;
	unsigned int	m_nReaderWantedPieceSize;
	void			*m_pReaderAddlData;

	unsigned char	*m_pReaderBuf;
	unsigned int	m_nReaderAlloc;
	unsigned int	m_nExpectedSizeMax;
	unsigned int	m_nReaderRead;
	unsigned int	m_nPieceSize;

	unsigned int PrepareReader(
		unsigned int nExpectedSizeMax,	// to optimaze buf allocation for internal buffer, 0 = unknown
		unsigned int nCallerMaxSize,	// max Piece that can be proceeced by API
		unsigned char *&pBuf,
		saLongOrLobReader_t fnReader,
		unsigned int nReaderWantedPieceSize,
		void *pReaderAddlData,
		bool bAddSpaceForNull = false);
	void InvokeReader(
		SAPieceType_t ePieceType,
		unsigned char *&pBuf,
		unsigned int nPieceLen);
	SAString asLongOrLob() const;

protected:
	SADataType_t	m_eDataType;

	// null indicator
	bool	*m_pbNull;
	// scalar types
	void	*m_pScalar;
	// an exact numeric value with a fixed precision and scale
	SANumeric	*m_pNumeric;
	// Date time
	SADateTime	*m_pDateTime;
	// variable length types (string, bytes, Longs and Lobs)
	SAString	*m_pString;
	// Cursor
	SACommand	*m_pCursor;
private:
	// null indicator
	bool	m_bInternalNull;
	// scalar types
	union
	{
		bool m_Bool;
		short m_Short;
		long m_Long;
		double m_Double;
	} m_InternalScalar;
	SANumeric	m_InternalNumeric;
	SADateTime	m_InternalDateTime;
	// variable length types (string, bytes, Longs and Lobs)
	SAString	m_InternalString;
	// Cursor
	SACommand	m_InternalCursor;

public:
	SAValueRead(SADataType_t eDataType);
	SAValueRead(const SAValueRead &vr);
	virtual ~SAValueRead();

	SAValueRead &operator =(const SAValueRead &vr);

public:
	SADataType_t DataType() const;

	// Null type
	bool isNull() const;

	// scalar types
	bool asBool() const;
	short asShort() const;
	long asLong() const;
	double asDouble() const;

	// numeric
	SANumeric asNumeric() const;

	// date/time
	SADateTime asDateTime() const;

	// variable length types
	SAString asString() const;
	SAString asBytes() const;
	SAString asLongBinary() const;
	SAString asLongChar() const;
	SAString asBLob() const;
	SAString asCLob() const;

	// cursor
	SACommand *asCursor() const;

	void setLongOrLobReaderMode(SALongOrLobReaderModes_t eMode);
	SALongOrLobReaderModes_t LongOrLobReaderMode() const;

	// operators for quick accessing values
	// do not inline to prevent varnings
	operator bool() const;
	operator short() const;
	operator long() const;
	operator double() const;
	operator SANumeric() const;
	operator SADateTime() const;
	operator SAString() const;
	operator SACommand *() const;

	// data and indicator storage
	void setIndicatorStorage(bool *pStorage);
private:
	void setDataStorage(void *pStorage, SADataType_t eDataType);
};

class SQLAPI_API SAValue : public SAValueRead
{
	friend class ISACursor;
	friend class IibCursor;
	friend class IssDBLibCursor;
	friend class Iora7Cursor;
	friend class Iora8Cursor;
	friend class IsbCursor;
	friend class IodbcCursor;
	friend class Idb2Cursor;
	friend class IinfCursor;
	friend class IsybCursor;
	friend class ImyCursor;
	friend class IpgCursor;

private:
	bool	m_bInternalUseDefault;

protected:
	bool	*m_pbUseDefault;

	saLongOrLobWriter_t	m_fnWriter;
	int	m_nWriterSize;
	void			*m_pWriterAddlData;
	void			*m_pWriterBuf;
	unsigned int	m_nWriterAlloc;
	unsigned int	m_nWriterWritten;

	unsigned int InvokeWriter(
		SAPieceType_t &ePieceType,
		int nCallerMaxSize,
		void *&pBuf);

public:
	SAValue(SADataType_t eDataType);
	virtual ~SAValue();

	// Sets NULL value
	void setAsNull();
	// Sets a flag to use default value
	void setAsDefault();
	// queries if "default value" flag is set
	bool isDefault() const;

	void setAsUnknown();

	// scalar types
	bool &setAsBool();
	short &setAsShort();
	long &setAsLong();
	double &setAsDouble();

	// numeric
	SANumeric &setAsNumeric();

	// date/time
	SADateTime &setAsDateTime();

	// variable length types
	SAString &setAsString();
	SAString &setAsBytes();
	SAString &setAsLongBinary(
		saLongOrLobWriter_t fnWriter = NULL,
		unsigned int nWriterSize = 0, void *pAddlData = NULL);
	SAString &setAsLongChar(
		saLongOrLobWriter_t fnWriter = NULL,
		unsigned int nWriterSize = 0, void *pAddlData = NULL);
	SAString &setAsBLob(
		saLongOrLobWriter_t fnWriter = NULL,
		unsigned int nWriterSize = 0, void *pAddlData = NULL);
	SAString &setAsCLob(
		saLongOrLobWriter_t fnWriter = NULL,
		unsigned int nWriterSize = 0, void *pAddlData = NULL);

	// cursor
	SACommand *&setAsCursor();

	// special set function(s)
	SAValueRead &setAsValueRead();
};

class SQLAPI_API SAParam : public SAValue
{
	friend class SACommand;
	friend class saPlaceHolder;
	friend class saOptions;

	SACommand *m_pCommand;

	SAString			m_sName;
	SADataType_t		m_eParamType;
	int					m_nNativeType;
	int					m_nParamSize;
	int					m_nParamPrecision;
	int					m_nParamScale;
	SAParamDirType_t	m_eDirType;

	saOptions m_Options;

private:
	// disable copy constructor
	SAParam(const SAParam &);
	// disable assignment operator
	SAParam &operator = (const SAParam &);

protected:
	SAParam(
		SACommand *pCommand,
		const SAString &sName,
		SADataType_t eParamType,
		int nNativeType,
		int nParamSize,
		int	nParamPrecision,
		int	nParamScale,
		SAParamDirType_t eDirType);
	virtual ~SAParam();

public:
	const SAString &Name() const;
	SADataType_t ParamType() const;
	void setParamType(SADataType_t eParamType);
	int ParamNativeType() const;
	void setParamNativeType(int nNativeType);
	int ParamSize() const;
	void setParamSize(int nParamSize);
	SAParamDirType_t ParamDirType() const;
	void setParamDirType(SAParamDirType_t eParamDirType);
	int ParamPrecision() const;
	void setParamPrecision(int nParamPrecision);
	int ParamScale() const;
	void setParamScale(int nParamScale);

	void ReadLongOrLob(
		saLongOrLobReader_t fnReader,
		unsigned int nReaderWantedSize,
		void *pAddlData);

	SAString &setOption(const SAString &sOptionName);
	SAString Option(const SAString &sOptionName) const;
};

class SQLAPI_API saPlaceHolder
{
	friend class SACommand;

	SAString	m_sFullName;
	int			m_nStart;
	int			m_nEnd;

	SAParam		*m_pParamRef;

private:
	saPlaceHolder(
		const SAString &sFullName,
		int nStart,
		int nEnd,
		SAParam *pParamRef);
	virtual ~saPlaceHolder();

public:
	const SAString &getFullName() const;
	int getStart() const;
	int getEnd() const;
	SAParam *getParam() const;
};

class SQLAPI_API SABytes : public SAString
{
public:
	SABytes(const SAString &sData);
};

class SQLAPI_API SALongOrLob : public SAString
{
	friend class SACommand;

protected:
	saLongOrLobWriter_t	m_fnWriter;
	unsigned int		m_nWriterPieceSize;
	void				*m_pAddlData;

	SALongOrLob(const SAString &sData);
	SALongOrLob(
		saLongOrLobWriter_t fnWriter,
		unsigned int nWriterPieceSize,
		void *pAddlData);
};

class SQLAPI_API SALongBinary : public SALongOrLob
{
public:
	SALongBinary(const SAString &sData);
	SALongBinary(
		saLongOrLobWriter_t fnWriter,
		unsigned int nWriterPieceSize,
		void *pAddlData);
};

class SQLAPI_API SALongChar : public SALongOrLob
{
public:
	SALongChar(const SAString &sData);
	SALongChar(
		saLongOrLobWriter_t fnWriter,
		unsigned int nWriterPieceSize,
		void *pAddlData);
};

class SQLAPI_API SABLob : public SALongOrLob
{
public:
	SABLob(const SAString &sData);
	SABLob(
		saLongOrLobWriter_t fnWriter,
		unsigned int nWriterPieceSize,
		void *pAddlData);
};

class SQLAPI_API SACLob : public SALongOrLob
{
public:
	SACLob(const SAString &sData);
	SACLob(
		saLongOrLobWriter_t fnWriter,
		unsigned int nWriterPieceSize,
		void *pAddlData);
};

class SQLAPI_API SAField : public SAValueRead
{
	friend class SACommand;

	SACommand *m_pCommand;

	// as reported by describe API
	int					m_nPos;	// 1-based
	SAString			m_sName;
	SADataType_t		m_eFieldType;
	int					m_nNativeType;
	int					m_nFieldSize;
	int					m_nFieldPrecision;
	int					m_nFieldScale;
	bool				m_bFieldRequired;

	saOptions m_Options;

private:
	// disable copy constructor
	SAField(const SAField &);
	// disable assignment operator
	SAField &operator = (const SAField &);

protected:
	SAField(
		SACommand *pCommand,
		int nPos,	// 1-based
		const SAString &sName,
		SADataType_t eFieldType,
		int nNativeType,
		int nFieldSize,
		int nFieldPrecision,
		int nFieldScale,
		bool bFieldRequired);
	virtual ~SAField();

public:
	int Pos() const;
	const SAString &Name() const;
	SADataType_t FieldType() const;
	int FieldNativeType() const;
	int FieldSize() const;
	int FieldPrecision() const;
	int FieldScale() const;
	bool isFieldRequired() const;

	void ReadLongOrLob(
		saLongOrLobReader_t fnReader,
		unsigned int nReaderWantedSize,
		void *pAddlData);

	SAString &setOption(const SAString &sOptionName);
	SAString Option(const SAString &sOptionName) const;
};

class SQLAPI_API SAException
{
	friend class SAConnection;
	friend class SACommand;
	friend class SAStoredProc;
	friend class Iora7Connection;
	friend class Iora8Connection;
	friend class IibConnection;
	friend class IodbcConnection;
	friend class IsbConnection;
	friend class Iss6Connection;

public:
	SAException(
		SAErrorClass_t eError,
		int nNativeError,
		int nErrPos,
		const SAString &sMsg);
	SAException(
		SAErrorClass_t eError,
		int nNativeError,
		int nErrPos,
		const SAChar *lpszFormat, ...);

	SAException(const SAException &other);

public:
	virtual ~SAException();

	static void throwUserException(
		int nUserCode, const SAChar *lpszFormat, ...);

	SAErrorClass_t ErrClass() const;
	int ErrNativeCode() const;
	int ErrPos() const;
	SAString ErrText() const;

protected:
	SAErrorClass_t	m_eErrorClass;
	int				m_nNativeError;
	int				m_nErrPos;
	SAString		m_sMsg;

	int	nReserved;
};

class SQLAPI_API SAUserException : public SAException
{
	friend class SAException;

protected:
	SAUserException(int nUserCode, const SAString &sMsg);

public:
	virtual ~SAUserException();
};

#define SQLAPI_VER_MAJOR	3
#define SQLAPI_VER_MINOR	7
#define SQLAPI_VER_BUILD	9

// we use class to avoid namespace using
class SQLAPI_API SAGlobals
{
public:
	static char *setlocale(int category, const char *locale);

	static int GetVersionMajor();
	static int GetVersionMinor();
	static int GetVersionBuild();
};

#endif // !defined(AFX_SQLAPI_H__BC064F47_40FC_45B6_AA67_2E6D2568E219__INCLUDED_)
