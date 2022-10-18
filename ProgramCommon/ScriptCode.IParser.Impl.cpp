#include "stdafx.h"
#include "scriptCode.IParser.h"

IParser::IParser()
{
    m_pTableData = 0;
    m_pIniParser = 0;
    ZeroMemory(m_szCurFileName, sizeof(m_szCurFileName));
    ZeroMemory(m_szPackFileName, sizeof(m_szPackFileName));
    ZeroMemory(m_szCurVersion, sizeof(m_szCurVersion));
}

IParser::~IParser()
{
    // 'no - check' for memory lead detect
    m_mapFileName.clear();
}


void
IParser::SetCurFileName(const char* pszCurFileName)
{
    strncpy(m_szCurFileName, pszCurFileName, _countof(m_szCurFileName));
    m_szCurFileName[_countof(m_szCurFileName) - 1] = '\0';
}

void
IParser::SetPackFileName(const char* pszPackFileName)
{
    if(pszPackFileName)
    {
        strncpy(m_szPackFileName, pszPackFileName, _countof(m_szPackFileName));
        m_szPackFileName[_countof(m_szPackFileName) - 1] = '\0';
    }
    else
    {
        ZeroMemory(m_szPackFileName, sizeof(m_szPackFileName));
    }
}

void
IParser::SetPackFileName(const wchar_t* pszPackFileName)
{
    char szPackFileName[MAX_PATH + 1] = {0,};
    // (WAVERIX) (090712) (NOTE) 찜찜한 연산을 수행 중... 함수 파악 뒤, 정확한 크기 입력하자.
    WideCharToMultiByte(CP_ACP, MB_PRECOMPOSED, pszPackFileName, -1, szPackFileName, -1 , NULL ,NULL);

    // (WAVERIX) (CHANGES) old code : if(szPackFileName)
    //           (NOTE) WideCharToMultiByte는 어떤 에러를 낼 것인가?
    if(pszPackFileName)
    {
        strncpy(m_szPackFileName, szPackFileName, _countof(m_szPackFileName));
        m_szPackFileName[_countof(m_szPackFileName) - 1] = '\0';
    }
    else
    {
        ZeroMemory(m_szPackFileName, sizeof(m_szPackFileName));
    }
}

BOOL
IParser::SolarLoad(eSCRIPT_CODE scriptCode, BYTE* pStream, DWORD dwSize, BOOL bBigData)
{
    __UNUSED(bBigData);
    m_pTableData = NULL;
    m_pIniParser = 0;

    const char* pszFileName = FindFileName(scriptCode);
    if(!pszFileName)
        return FALSE;

    //분기조건. 확장자가 txt냐 ini냐에 따라서..
    char szFileName[MAX_PATH], szExt[MAX_PATH];
    _splitpath(pszFileName, NULL, NULL, szFileName, szExt);

    //대문자 변경.
    _strupr(szExt);
    //DWORD dwLength = (DWORD)strlen(szExt);
    //char* pOffset = (char*)szExt;
    //for(DWORD dwIdx = 0; dwIdx < dwLength; ++dwIdx, ++pOffset)
    //    *pOffset = (char)toupper(*pOffset);

    //분기조건. 확장자가 txt냐 ini냐에 따라서..
    if(strcmp(szExt, ".TXT") == 0)
    {
        m_pTableData = new TableData;
        if(m_pTableData->LoadBuffer(scriptCode, pStream, dwSize) == 0)
        {
            ASSERT(!"OpenBuffer");
            return (FALSE);
        }
    }
    else if(strcmp(szExt, ".INI") == 0)
    {
        m_pIniParser = new util::INIParser;
        m_pIniParser->SetBuffer((char*)pStream, dwSize);
        if(m_pIniParser->INILoad() == 0) {
            FASSERT(!"[INILoad] 파일 읽기 실패!!");
            return FALSE;
        }
    }
    else
    {
        FASSERT(!"알수없는 확장자.");
        return FALSE;
    }

    BOOL bLoad = LoadScript(scriptCode, FALSE);

    if(bLoad)
        SUNLOG(eDEV_LOG, "Script File Load Complete, File = %s", GetCurFileName());
    else
        SUNLOG(eDEV_LOG, "Script File Load Fail, File = %s", GetCurFileName());

    if(m_pTableData)
    {
        m_pTableData->Release();
        delete m_pTableData;
    }
    SAFE_DELETE(m_pIniParser);
    

    return bLoad;
}

BOOL
IParser::Load(eSCRIPT_CODE scriptCode, const char* pszFileName,
              BOOL bBigData, eSEPERATOR_MODE mode,
              const char* pszPackFileName, BOOL bReload)
{
    __UNUSED((mode, bBigData));
    m_pTableData =NULL;
    m_pIniParser = 0;
    DWORD dwFirstTick = GetTickCount();

    if(!bReload)
    {
        SetCurFileName(pszFileName);
        SetPackFileName(pszPackFileName);
    }

    //분기조건. 확장자가 txt냐 ini냐에 따라서..
    char szFileName[MAX_PATH], szExt[MAX_PATH];
    _splitpath(pszFileName, NULL, NULL, szFileName, szExt);

    //대문자 변경.
    _strupr(szExt);
    //DWORD dwLength = (DWORD)strlen(szExt);
    //char* pOffset = (char*)szExt;
    //for(DWORD dwIdx=0 ; dwIdx<dwLength ; ++dwIdx, ++pOffset)
    //    *pOffset = (char)toupper(*pOffset);

    if(strcmp(szExt, ".TXT") == 0)
    {
        m_pTableData = new TableData;
        SUNLOG(eDEV_LOG, "Script File Load Start, File = %s", m_szCurFileName);
        if(m_pTableData->Load(scriptCode, m_szCurFileName,
                              m_szPackFileName[0] != '\0', m_szPackFileName) == 0)
        {
            return FALSE;
        }
    }
    else if(strcmp(szExt, ".INI") == 0)
    {
        m_pIniParser = new util::INIParser;
        if(m_pIniParser->OpenFile(pszFileName, m_szPackFileName) == 0 || m_pIniParser->INILoad() == 0) {
            FASSERT(!"[INILoad] 파일 읽기 실패!!");
            return FALSE;
        }
    }
    else
    {
        FASSERT(!"알수없는 확장자.");
        return FALSE;
    }

    BOOL bLoad = LoadScript(scriptCode, bReload);
    SUNLOG(eDEV_LOG, "Script File Load %s, File = %s", bLoad ? "Complete" : "Fail", m_szCurFileName);

    if(m_pTableData)
    {
        m_pTableData->Release();
        delete m_pTableData;
    }
    SAFE_DELETE(m_pIniParser);

    DWORD dwLastTick = GetTickCount();
    SUNLOG(eDEV_LOG, "%s, Load Time %d Mili Second", GetCurFileName(), dwLastTick - dwFirstTick);

    return bLoad;
}

BOOL
IParser::InsertFileName(eSCRIPT_CODE scriptCode, const char* pszFileName)
{
    if(FindFileName(scriptCode))
        return FALSE;

    if(!m_mapFileName.insert(std::make_pair(scriptCode, pszFileName)).second)
        return FALSE;

    return TRUE;
}

const char*
IParser::FindFileName(eSCRIPT_CODE scriptCode)
{
    FILE_NAME_MAP::iterator  iter;
    iter = m_mapFileName.find(scriptCode);
    if(iter != m_mapFileName.end())
        return iter->second.c_str();

    return NULL;
}


//////////////////////////////////////////////////////////////////////////


BOOL
IParser::OpenFile(util::SolarFileReader& sr, const char* pszFileName, eSEPERATOR_MODE mode)
{
    BOOL bResult = FALSE;

    if(strlen(m_szPackFileName) > 0) // pack
    {
        bResult = sr.OpenFileFromPack(m_szPackFileName, pszFileName, mode);

    }
    else //UnPack
    {
        bResult = sr.OpenFile((char*)pszFileName, mode);
    }

    if(!bResult)
    {
        // (WAVERIX) (090712) (CHANGES)
        SUNLOG(eCRITICAL_LOG, "File[%s] Open Error", pszFileName);
        //char szError[MAX_PATH + 1];
        //_snprintf(szError, MAX_PATH, "File[%s] Open Error \n", pszFileName);
        //SUNLOG(eCRITICAL_LOG, szError);
        ASSERT(!"File Open Error");
    }

    return bResult;
}

BOOL
IParser::_LoadVersion(util::SolarFileReader& sr)
{
    while(TRUE)
    {
        eTOKEN_TYPE ettType = sr.GetNextTokenType(); 
        if(TOKEN_END == ettType)
            break;

        char /*szVersion[MAX_PATH],*/ szName[MAX_PATH];
        //ZeroMemory(szName, MAX_PATH);

        strncpy(szName, sr.GetTokenString(), _countof(szName));
        szName[_countof(szName) - 1] = '\0';

        if(strcmp(szName, "Version") != 0 && strcmp(szName, "version") != 0)
            continue;

        //ZeroMemory(szVersion, MAX_PATH);
        sr.GetNextTokenType(TOKEN_STRING);
        strncpy(m_szCurVersion, sr.GetTokenString(), _countof(m_szCurVersion));
        m_szCurVersion[_countof(m_szCurVersion) - 1] = '\0';
        //m_szCurVersion[MAX_PATH] = '\0';
        return TRUE;
    }

    return FALSE;
}

template<typename TYPE>
BOOL
IParser::_GetData(int nRow, OUT TYPE& OutValue, const char* pszColNameformat, ...)
{
    /*static*/char pszColName[MAX_FIELD_NAME_SIZE] = {0,};

    va_list arglist; 
    va_start(arglist, pszColNameformat); 
    _vsnprintf(pszColName, _countof(pszColName), pszColNameformat, arglist);
    pszColName[_countof(pszColName) - 1] = '\0';
    //pszColName[MAX_FIELD_NAME_SIZE-1] = '\0';
    va_end(arglist);

    switch(typeid(TYPE))
    {
    case typeid(int):
    case typeid(byte):
    case typeid(WORD):
    case typeid(DWORD):
        OutValue = (TYPE)atoi(m_TableData->FindData(pszColName, nRow));
        return TRUE;

    case typeid(double):
    case typeid(float):
        OutValue = (TYPE)atof(m_TableData->FindData(pszColName, nRow));
        return TRUE;

    case typeid(__int64):
    case typeid(DWORD64):
        OutValue = (TYPE)_atoi64(m_TableData->FindData(pszColName, nRow));
        return TRUE;
    }

    return FALSE;
}

BOOL
IParser::GetData(int nRow, int size, OUT char* OutValue, const char* pszColNameformat, ...)
{
    /*static*/char pszColName[MAX_FIELD_NAME_SIZE] = {0,};

    va_list arglist; 
    va_start(arglist, pszColNameformat); 
    _vsnprintf(pszColName, _countof(pszColName), pszColNameformat, arglist);
    pszColName[_countof(pszColName) - 1] = '\0';
    //pszColName[MAX_FIELD_NAME_SIZE-1] = '\0';
    va_end(arglist);

    strncpy(OutValue, GetDataString(pszColName,nRow), size);

    return TRUE;
}

BOOL
IParser::GetData(int nRow, OUT int& OutValue, const char* pszColNameformat, ...)
{   
    /*static*/char pszColName[MAX_FIELD_NAME_SIZE] = {0,};

    va_list arglist; 
    va_start(arglist, pszColNameformat); 
    _vsnprintf(pszColName, _countof(pszColName), pszColNameformat, arglist);
    pszColName[_countof(pszColName) - 1] = '\0';
    //pszColName[MAX_FIELD_NAME_SIZE-1] = '\0';
    va_end(arglist);

    OutValue = GetDataInt(pszColName,nRow);

    return TRUE;
}

BOOL
IParser::GetData(int nRow, OUT BYTE& OutValue, const char* pszColNameformat, ...)
{   
    /*static*/char pszColName[MAX_FIELD_NAME_SIZE] = {0,};

    va_list arglist; 
    va_start(arglist, pszColNameformat); 
    _vsnprintf(pszColName, _countof(pszColName), pszColNameformat, arglist);
    pszColName[_countof(pszColName) - 1] = '\0';
    //pszColName[MAX_FIELD_NAME_SIZE-1] = '\0';
    va_end(arglist);

    OutValue = GetDataBYTE(pszColName,nRow);

    return TRUE;
}

BOOL
IParser::GetData(int nRow, OUT WORD& OutValue, const char* pszColNameformat, ...)
{   
    /*static*/char pszColName[MAX_FIELD_NAME_SIZE] = {0,};

    va_list arglist; 
    va_start(arglist, pszColNameformat); 
    _vsnprintf(pszColName, _countof(pszColName), pszColNameformat, arglist);
    pszColName[_countof(pszColName) - 1] = '\0';
    //pszColName[MAX_FIELD_NAME_SIZE-1] = '\0';
    va_end(arglist);

    OutValue = GetDataWORD(pszColName,nRow);

    return TRUE;
}

BOOL
IParser::GetData(int nRow, OUT DWORD& OutValue, const char* pszColNameformat, ...)
{   
    /*static*/char pszColName[MAX_FIELD_NAME_SIZE] = {0,};

    va_list arglist; 
    va_start(arglist, pszColNameformat); 
    _vsnprintf(pszColName, _countof(pszColName), pszColNameformat, arglist);
    pszColName[_countof(pszColName) - 1] = '\0';
    //pszColName[MAX_FIELD_NAME_SIZE-1] = '\0';
    va_end(arglist);

    OutValue = GetDataDWORD(pszColName,nRow);

    return TRUE;
}

BOOL
IParser::GetData(int nRow, OUT float& OutValue, const char* pszColNameformat, ...)
{   
    /*static*/char pszColName[MAX_FIELD_NAME_SIZE] = {0,};

    va_list arglist; 
    va_start(arglist, pszColNameformat); 
    _vsnprintf(pszColName, _countof(pszColName), pszColNameformat, arglist);
    pszColName[_countof(pszColName) - 1] = '\0';
    //pszColName[MAX_FIELD_NAME_SIZE-1] = '\0';
    va_end(arglist);

    OutValue = GetDataFloat(pszColName,nRow);

    return TRUE;
}

BOOL
IParser::GetData(int nRow, OUT double& OutValue, const char* pszColNameformat, ...)
{   
    /*static*/char pszColName[MAX_FIELD_NAME_SIZE] = {0,};

    va_list arglist; 
    va_start(arglist, pszColNameformat); 
    _vsnprintf(pszColName, _countof(pszColName), pszColNameformat, arglist);
    pszColName[_countof(pszColName) - 1] = '\0';
    //pszColName[MAX_FIELD_NAME_SIZE-1] = '\0';
    va_end(arglist);

    OutValue = GetDataDouble(pszColName,nRow);

    return TRUE;
}

BOOL
IParser::GetData64(int nRow, OUT __int64& OutValue, const char* pszColNameformat, ...)
{   
    /*static*/char pszColName[MAX_FIELD_NAME_SIZE] = {0,};

    va_list arglist; 
    va_start(arglist, pszColNameformat); 
    _vsnprintf(pszColName, _countof(pszColName), pszColNameformat, arglist);
    pszColName[_countof(pszColName) - 1] = '\0';
    //pszColName[MAX_FIELD_NAME_SIZE-1] = '\0';
    va_end(arglist);

    OutValue = GetDataInt64(pszColName,nRow);

    return TRUE;
}

BOOL
IParser::GetData64(int nRow, OUT DWORD64& OutValue, const char* pszColNameformat, ...)
{   
    /*static*/char pszColName[MAX_FIELD_NAME_SIZE] = {0,};

    va_list arglist; 
    va_start(arglist, pszColNameformat); 
    _vsnprintf(pszColName, _countof(pszColName), pszColNameformat, arglist);
    pszColName[_countof(pszColName) - 1] = '\0';
    //pszColName[MAX_FIELD_NAME_SIZE-1] = '\0';
    va_end(arglist);

    OutValue = GetDataDWORD64(pszColName,nRow);

    return TRUE;
}

BOOL
IParser::GetDataWZID(int nRow, OUT DWORD& OutValue, const char* pszColNameformat, ...)
{
    char pszColName[MAX_FIELD_NAME_SIZE] = {0,};

    va_list arglist; 
    va_start(arglist, pszColNameformat); 
    _vsnprintf(pszColName, _countof(pszColName), pszColNameformat, arglist);
    pszColName[_countof(pszColName) - 1] = '\0';
    va_end(arglist);

    char szTemp[5]={0,};
    GetData(nRow, 4, szTemp, pszColName );

    OutValue = StrToWzID(szTemp);
    if (48 == OutValue) //48="NULL"
        OutValue = 0;

    return TRUE;
}

const char*
IParser::GetDataString(const char* pszColName, const int nRow) const
{
    return m_pTableData->FindData(pszColName, nRow);
}

double
IParser::GetDataDouble(const char* pszColName, const int nRow) const
{
    return atof(m_pTableData->FindData(pszColName, nRow));
}

float
IParser::GetDataFloat(const char* pszColName, const int nRow) const
{
    return (float)atof(m_pTableData->FindData(pszColName, nRow));
}


int
IParser::GetDataInt(const char* pszColName, const int nRow) const
{
    return atoi(m_pTableData->FindData(pszColName, nRow));
}

__int64
IParser::GetDataInt64(const char* pszColName, const int nRow) const
{
    return _atoi64(m_pTableData->FindData(pszColName, nRow));
}

BYTE
IParser::GetDataBYTE(const char* pszColName, const int nRow) const
{
    return (BYTE)(atoi(m_pTableData->FindData(pszColName, nRow)));
}

WORD
IParser::GetDataWORD(const char* pszColName, const int nRow) const
{
    return (WORD)(atoi(m_pTableData->FindData(pszColName, nRow)));
}

DWORD
IParser::GetDataDWORD(const char* pszColName, const int nRow) const
{
    return (DWORD)(atoi(m_pTableData->FindData(pszColName, nRow)));
}

DWORD64
IParser::GetDataDWORD64(const char* pszColName, const int nRow) const
{
    return (DWORD64)(_atoi64(m_pTableData->FindData(pszColName, nRow)));
}

double
IParser::GetDataNumber(const char* pszColName, const int nRow) const
{
    return atof(m_pTableData->FindData(pszColName, nRow));
}

bool
IParser::GetDataBOOL(const char* pszColName, const int nRow) const
{
    return !!(atoi(m_pTableData->FindData(pszColName, nRow)));
}
//==================================================================================================
// (WAVERIX) (090719) (NOTE)
// - 현 파서 구조는 없으면 뻗게 되어 있다... 장기적으로 이게 나을 수 있으므로 그대로 지원한다.

//ini 파일형 자료.
const char*
IParser::GetINIDataString(const char* pszApp, const char* pszKey)
{
    const char* value = m_pIniParser->GetData(pszApp, pszKey);
    if(value != 0) {
        // (TEST) 현 시점 GM 명령어 관련 빼고는 없는 듯...
        if(value[0] == '\0')
            SUNLOG(eDEV_LOG, "Empty String Key:Value = {%s:%s}", pszApp, pszKey);
        return value;
    }

    SUNLOG(eCRITICAL_LOG, "Can't find value Key:Value = {%s:%s}", pszApp, pszKey);
    __debugbreak();
    return 0;
}

double
IParser::GetINIDataDouble(const char* pszApp, const char* pszKey)
{
    const char* value = m_pIniParser->GetData(pszApp, pszKey);
    if(value != 0) {
        return atof(value);
    }

    SUNLOG(eCRITICAL_LOG, "Can't find value Key:Value = {%s:%s}", pszApp, pszKey);
    __debugbreak();
    return 0;
}

float
IParser::GetINIDataFloat(const char* pszApp, const char* pszKey)
{
    const char* value = m_pIniParser->GetData(pszApp, pszKey);
    if(value != 0) {
        return (float)atof(value);
    }

    SUNLOG(eCRITICAL_LOG, "Can't find value Key:Value = {%s:%s}", pszApp, pszKey);
    __debugbreak();
    return 0;
}

int
IParser::GetINIDataInt(const char* pszApp, const char* pszKey)
{
    const char* value = m_pIniParser->GetData(pszApp, pszKey);
    if(value != 0) {
        return atoi(value);
    }

    SUNLOG(eCRITICAL_LOG, "Can't find value Key:Value = {%s:%s}", pszApp, pszKey);
    __debugbreak();
    return 0;
}

BYTE
IParser::GetINIDataBYTE(const char* pszApp, const char* pszKey)
{
    const char* value = m_pIniParser->GetData(pszApp, pszKey);
    int numeric = 0;
    if(value != 0) {
        numeric = atoi(value);
        if(SAFE_NUMERIC_TYPECAST(int, numeric, char))
            return BYTE(numeric);
    }

    SUNLOG(eCRITICAL_LOG, "Can't find value Key:Value = {%s:%s} or Invalid value %d",
           pszApp, pszKey, numeric);
    __debugbreak();
    return 0;
}

WORD
IParser::GetINIDataWORD(const char* pszApp, const char* pszKey)
{
    const char* value = m_pIniParser->GetData(pszApp, pszKey);
    int numeric = 0;
    if(value != 0) {
        numeric = atoi(value);
        if(SAFE_NUMERIC_TYPECAST(int, numeric, short))
            return WORD(numeric);
    }

    SUNLOG(eCRITICAL_LOG, "Can't find value Key:Value = {%s:%s} or Invalid value %d",
           pszApp, pszKey, numeric);
    __debugbreak();
    return 0;
}

DWORD
IParser::GetINIDataDWORD(const char* pszApp, const char* pszKey)
{
    const char* value = m_pIniParser->GetData(pszApp, pszKey);
    int numeric = 0;
    if(value != 0) {
        numeric = atoi(value);
        return DWORD(numeric);
    }

    SUNLOG(eCRITICAL_LOG, "Can't find value Key:Value = {%s:%s}", pszApp, pszKey);
    __debugbreak();
    return 0;
}

double
IParser::GetINIDataNumber(const char* pszApp, const char* pszKey)
{
    const char* value = m_pIniParser->GetData(pszApp, pszKey);
    if(value != 0) {
        return atof(value);
    }

    SUNLOG(eCRITICAL_LOG, "Can't find value Key:Value = {%s:%s}", pszApp, pszKey);
    __debugbreak();
    return 0;
}

//==================================================================================================

