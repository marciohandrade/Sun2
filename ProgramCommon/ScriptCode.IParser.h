#ifndef __SCRIPTCODE_IPARSER_H
#define __SCRIPTCODE_IPARSER_H

//==================================================================================================
//  (History)
//  - 06.09.27, waverix, Create for SolarAuth Script Loading Mechanism
//  - 07.11.28 ~ 08.09.03, JsCho, update for New Script Loading System
//                              , __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
//  - 09.07.12, waverix, code rearrangement
//                     , delete working macro '__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM'
//                     , change 'TCHAR' to 'char' - 현 시점 사실상 wchar_t가 될 일 없다.
//                       wchar_t사용 사례. packfile관련... 사용은 하나?
//                     , minor fix for more stable
//  - 09.07.18, waverix, changed parsing algorithm by '__NA000000_090718_SCRIPT_PARSING_SYSTEM_MODIFICATION2'
//  - 09.07.19, waverix, update specifiction by changed INIParser::GetData interface
//  - 10.02.01, i4u4me, Init(DWORD dwMaxSize) 가상 함수 추가

#include <SolarFileReader.h>
#include <INIParser.h>
#include "./TableData.h"
#include <boost/lexical_cast.hpp>

// (WAVERIX) (090712) (CHANGES) 자료형 변경 + nested type, (일부 작업 매크로제거 차 변경)
//typedef std::map< eSCRIPT_CODE, std::string >     FILE_NAME_MAP;
//typedef FILE_NAME_MAP::iterator                   FILE_NAME_MAP_ITER;


class IParser
{
public:
    typedef STLX_MAP<eSCRIPT_CODE, std::string> FILE_NAME_MAP;
    IParser();
    virtual ~IParser();

public:
    virtual void Init(DWORD dwMaxSize) { __UNUSED(dwMaxSize); }
    virtual void Release() = 0;

    const char* GetVersion(){ return m_szCurVersion; }
    void SetCurFileName(const char* pszCurFileName);
    const char* GetCurFileName(){ return m_szCurFileName; }
    void SetPackFileName(const char * pszPackFileName);
    void SetPackFileName(const wchar_t * pszPackFileName);

protected:
    BOOL    OpenFile(util::SolarFileReader& sr, const char* pszFileName, eSEPERATOR_MODE mode);
    BOOL    _LoadVersion(util::SolarFileReader& sr);

    BOOL    CreateColumn(util::SolarFileReader& sr);
    BOOL    _LoadTable(util::SolarFileReader& sr);
    int     GetRowSize() const { return m_pTableData->GetRowSize(); }
    int     GetColSize() const { return m_pTableData->GetColSize(); }

    //자료 얻는 멤버함수.
public: 
    //일반 테이블형 자료
    const char* GetDataString(const char* pszColName, const int nRow) const;
    double  GetDataDouble(const char* pszColName, const int nRow) const;
    float   GetDataFloat(const char* pszColName, const int nRow) const;
    int     GetDataInt(const char* pszColName, const int nRow) const;
    __int64 GetDataInt64(const char* pszColName, const int nRow) const;
    BYTE    GetDataBYTE(const char* pszColName, const int nRow) const;
    WORD    GetDataWORD(const char* pszColName, const int nRow) const;
    DWORD   GetDataDWORD(const char* pszColName, const int nRow) const;
    double  GetDataNumber(const char* pszColName, const int nRow) const;
    DWORD64 GetDataDWORD64(const char* pszColName, const int nRow) const;
    bool    GetDataBOOL(const char* pszColName, const int nRow) const;

    //ini 파일형 자료.
    const char* GetINIDataString(const char* pszApp, const char* pszKey);
    double  GetINIDataDouble(const char* pszApp, const char* pszKey);
    float   GetINIDataFloat(const char* pszApp, const char* pszKey);
    int     GetINIDataInt(const char* pszApp, const char* pszKey);
    BYTE    GetINIDataBYTE(const char* pszApp, const char* pszKey);
    WORD    GetINIDataWORD(const char* pszApp, const char* pszKey);
    DWORD   GetINIDataDWORD(const char* pszApp, const char* pszKey);
    double  GetINIDataNumber(const char* pszApp, const char* pszKey);

#ifdef __NA_0_20090407_QUESTSCRIPT_LOAD_MODIFY
    template<typename TYPE>
    BOOL    _GetData(int nRow, OUT TYPE& OutValue, const char* pszColNameformat, ...);

    BOOL    GetData(int nRow, OUT int& OutValue, const char* pszColNameformat, ...);
    BOOL    GetData(int nRow, OUT BYTE& OutValue, const char* pszColNameformat, ...);
    BOOL    GetData(int nRow, OUT WORD& OutValue, const char* pszColNameformat, ...);
    BOOL    GetData(int nRow, OUT DWORD& OutValue, const char* pszColNameformat, ...);
    BOOL    GetData(int nRow, OUT float& OutValue, const char* pszColNameformat, ...);
    BOOL    GetData(int nRow, OUT double& OutValue, const char* pszColNameformat, ...);
    BOOL    GetData64(int nRow, OUT DWORD64& OutValue, const char* pszColNameformat, ...);
    BOOL    GetData64(int nRow, OUT __int64& OutValue, const char* pszColNameformat, ...);
    BOOL    GetDataWZID(int nRow, OUT DWORD& OutValue, const char* pszColNameformat, ...);
    BOOL    GetData(int nRow, int size, OUT char* OutValue, const char* pszColNameformat, ...);
#endif

    // 칼럼이 존재하는지 검사해서 안전하게 숫자형 값을 얻는다.
    template <typename DATA_T>
    bool GetSafeNumericData(
        const char* function_str, 
        const int line_number, 
        const int row, 
        DATA_T& data, 
        const char* column_name_format, ...)
    {
        char col_name[MAX_FIELD_NAME_SIZE] = {0, };

        va_list args; 
        va_start(args, column_name_format); 
        _vsnprintf(col_name, _countof(col_name), column_name_format, args);
        col_name[_countof(col_name)-1] = '\0';
        va_end(args);

        const char* data_str = m_pTableData->FindData(col_name, row);
        if (data_str == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|칼럼값을 찾을 수 없습니다.|FileName = %s, Row = %d, ColumnName = %s|"), 
                function_str, 
                line_number, 
                m_szCurFileName, 
                row, 
                col_name
            );
            return false;
        }
        
        try
        {
            data = boost::lexical_cast<DATA_T>(data_str);
        }
        catch (boost::bad_lexical_cast&)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|칼럼값 문자열을 숫자로 변환할 수 없습니다.|FileName = %s, Row = %d, ColumnName = %s, ColumnValue = %s|"), 
                function_str, 
                line_number, 
                m_szCurFileName, 
                row, 
                col_name, 
                data_str
            );
            return false;
        }
        
        return true;
    }

public:
    BOOL    InsertFileName(eSCRIPT_CODE scriptCode, const char* pszFileName);
    const char* FindFileName(eSCRIPT_CODE scriptCode);

private:
    //Load함수
public:
    virtual BOOL    Load(eSCRIPT_CODE scriptCode, const char* pszFileName,
                         BOOL bBigData, eSEPERATOR_MODE mode,
                         const char* pszPackFileName, BOOL bReload); //File용,.
    virtual BOOL    SolarLoad(eSCRIPT_CODE scriptCode, BYTE* pStream, DWORD dwSize, BOOL bBigData);
    virtual BOOL    LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload) = 0;

private:
    TableData*      m_pTableData;
    util::INIParser* m_pIniParser;
    FILE_NAME_MAP   m_mapFileName;

    char m_szCurFileName[MAX_PATH + 1];
    char m_szPackFileName[MAX_PATH + 1];
    char m_szCurVersion[MAX_PATH + 1];
};

#endif //__SCRIPTCODE_IPARSER_H

