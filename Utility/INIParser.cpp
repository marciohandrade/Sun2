#include "UtilityCommon.h"
#include "./INIParser.h"
#include <SolarFileReader.h>

namespace util {


INIParser::INIParser(void)
    : m_CurrentToken(INI_ERROR)
    , m_pszBuffer(0)
    , m_dwBufferPos(0)
    , m_dwBufferSize(0)
    , overrun_checker_00_(OverrunCheckMark)
    , overrun_checker_01_(OverrunCheckMark)
    , overrun_checker_02_(OverrunCheckMark)
{
    ZeroMemory(m_szApp, sizeof(m_szApp));
    ZeroMemory(m_szKey, sizeof(m_szKey));
    ZeroMemory(m_szValue, sizeof(m_szValue));
}

INIParser::~INIParser(void)
{
    Release();
}

void
INIParser::Release()
{
    APP_MAP::iterator iter;
    for(iter = m_mapApp.begin(); iter != m_mapApp.end(); ++iter)
    {
        KEY_MAP* pKey = iter->second;
        if(pKey)
        {
            pKey->clear();
            delete pKey;
        }
    }

    m_mapApp.clear();

    if(m_pszBuffer)
    {
        delete[] m_pszBuffer;
        m_pszBuffer = NULL;
    }

    // (WAVERIX) (090712) (ADDIN) 해당 INIParser가 수행한 OverrunCheck에 대해서만 검사하기로 한다.
    if(overrun_checker_00_ != OverrunCheckMark ||
       overrun_checker_01_ != OverrunCheckMark ||
       overrun_checker_02_ != OverrunCheckMark)
       __debugbreak();
}

bool
INIParser::OpenFile(const char* pszFileName, const char* pszPackFileName /*= NULL*/)
{
    if(m_pszBuffer != NULL)
        return false;

    ZeroMemory(m_szFileName, MAX_PATH);

	if ( pszPackFileName && strlen(pszPackFileName) > 0 )
	{
		util::SolarFileReader sr;

		BOOL bResult = sr.OpenFileFromPack(const_cast<char*>(pszPackFileName), pszFileName, SEPERATOR_WHITESPACE_ONLY_STRING);
		if (bResult)
		{
			m_dwBufferSize = sr.GetBufferSize();
			if (m_dwBufferSize < 0)
				return false;

			m_pszBuffer = new char[m_dwBufferSize+1];
			ZeroMemory(m_pszBuffer, m_dwBufferSize+1);
			m_dwBufferPos = 0;

			memcpy(m_pszBuffer, sr.GetBuffer(), m_dwBufferSize);			
		}
		sr.CloseFile();
	}
	else
	{
		HANDLE hFile = ::CreateFile(pszFileName,
                                GENERIC_READ,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile)
			return false;

		m_dwBufferSize = GetFileSize(hFile, NULL);
		if (m_dwBufferSize < 0)
			return false;

		m_pszBuffer = new char[m_dwBufferSize+1];
		ZeroMemory(m_pszBuffer, m_dwBufferSize+1);
		m_dwBufferPos = 0;

		DWORD dwNumber = 0;
		BOOL rt = ::ReadFile(hFile, m_pszBuffer, m_dwBufferSize, &dwNumber, NULL);
		if(!rt) 
			return false;

		::CloseHandle(hFile);
	}

    strncpy(m_szFileName, pszFileName, MAX_PATH);
    return true;
}

bool
INIParser::SetBuffer(const char* pszBuffer, DWORD dwSize)
{
    if(m_pszBuffer != NULL || pszBuffer == NULL)
        return false;

    m_dwBufferSize = dwSize;
    m_pszBuffer = new char[dwSize];
    strncpy(m_pszBuffer, pszBuffer, m_dwBufferSize);
    m_dwBufferPos = 0;

    return true;
}


//현재 읽어 드린 파일의 한개의 약속된 기호가 나올때의 값 전달
INIParser::INIToken
INIParser::GetToken()
{
    if(!m_pszBuffer)
        return INI_ERROR;

    _skipComment();
    _undo_getchar(); //_skipComment에서 현재 위치의 위치까지 계산하였으므로 한개만큼 빽~ 한다.

    char ch = _getchar();
    int nCnt = 0;
    if(ch == INI_TOKEN_END)
        return INI_TOKEN_END;

    switch(ch)
    {   
    case '#':
    case ';':
    case ',':
    case '{':
    case '}':
    case '=':
        return m_CurrentToken = INI_ERROR;

    case '[':
        nCnt = 0;
        while((ch = _getchar()) != ']')
        {
            if(ch == INI_TOKEN_END || isspace(ch) || ch == '\n')
                return INI_ERROR;  //APP값이 끝나기 전에 종료되거나 빈칸이 있으면 종료된다.

            if(nCnt >= INI_DATA_SIZE)
                return INI_ERROR;

            m_szApp[nCnt] = ch;
            nCnt++;
        }

        m_szApp[nCnt] = '\0';
        return m_CurrentToken = INI_APP;

    default:
        //문자열인 경우(항상 KEY값은 문자열로 시작한다.)
        if (isalpha(ch))    
        {
            //1. KEY를 구한다.
            nCnt = 0;

            while(!_IsSpace(ch))
            {
                if(ch == INI_TOKEN_END || ch == '\n')
                    return INI_ERROR;  //APP값이 끝나기 전에 종료되거나 빈칸이 있으면 종료된다.

                if(nCnt >= INI_DATA_SIZE)
                    return INI_ERROR;

                m_szKey[nCnt] = ch;
                nCnt++;
                ch = _getchar();
            }

            m_szKey[nCnt] = '\0';

            if(_skipSpace() == INI_TOKEN_END)
                return INI_ERROR;

            _undo_getchar(); 

            //2. '='이 나올때 까지 돈다. 다음 문자가 '='이 아니면 리턴한다.
            ch = _getchar();
            if((ch != '=') || (ch == INI_TOKEN_END))
                return INI_ERROR;

            // '='이 나왔으므로, 그 다음을 가지고 온다.
            _getchar();

            if(_skipSpace() == INI_TOKEN_END)
                return INI_ERROR;

            _undo_getchar(); //_skipSpace에서 현재 위치의 위치까지 계산하였으므로 한개만큼 빽~ 한다.

            //3. Value를 구한다.  무조건 문자열로 넣는다.
            nCnt = 0;
            ch = _getchar();

            while(!_IsSpace(ch))        
            {
                if(ch == INI_TOKEN_END || ch == '\n' || _IsCRLF(ch))
                {
                    if(nCnt > 0)
                        break;
                    else
                        return INI_ERROR;  //APP값이 끝나기 전에 종료되거나 빈칸이 있으면 종료된다.
                }

                if(nCnt >= INI_DATA_SIZE)
                    return INI_ERROR;

                m_szValue[nCnt] = ch;
                nCnt++;
                ch = _getchar();
            }

            m_szValue[nCnt] = '\0';
            return m_CurrentToken = INI_KEY;
        }

        return m_CurrentToken = INI_ERROR;
    }
}

//현재 m_dwBufferPos의 위치의 문자열을 리턴하고, m_dwBufferPos의 위치를 하나 증가 시킨다.
char
INIParser::_getchar()
{
    if(m_dwBufferPos < m_dwBufferSize)
        return m_pszBuffer[m_dwBufferPos++];
    else 
    {
        ++m_dwBufferPos;
        return INI_TOKEN_END;
    }
}

void
INIParser::_undo_getchar()
{
    m_dwBufferPos--;
}

bool
INIParser::INILoad()
{
    KEY_MAP* pKeyMap = NULL;
    std::string strPreAppName;

    while(m_pszBuffer[m_dwBufferPos] != EOF)
    {
        INIToken Token = GetToken();

        if(Token == INI_APP)
        {
            //APP 값을 얻음.
            pKeyMap = new KEY_MAP;
            strPreAppName = m_szApp;
            if(!m_mapApp.insert(make_pair(strPreAppName, pKeyMap)).second)
            {
                delete pKeyMap;
                return false;
            }
        }
        else if(Token == INI_KEY)
        {
            if(!pKeyMap)
                return false;

            if(!pKeyMap->insert(std::make_pair(m_szKey, m_szValue)).second)
                return false;

        }
        else if(Token == INI_TOKEN_END)
            return true;
        else if(Token == INI_ERROR)
            return false;
    }

    return true;
}


INIParser::INIToken
INIParser::_skipComment()
{   
    char ch;

    do
    {
        // 현재 파일의 끝이면 END 리턴
        if((ch = (char)_getchar()) == INI_TOKEN_END)
            return INI_TOKEN_END;

        // 만일 // 값이면 '\n'이 나올때 까지 돌아라.
        if(ch == '/' && (ch = (char)_getchar()) == '/') 
        {
            while((ch = (char)_getchar()) != '\n');
        }
    } while(isspace(ch));

    return INI_SUCCESS;
}


INIParser::INIToken
INIParser::_skipSpace()
{
    //보통 현재 가지고 있는 문자를 ch에 넣고, m_dwBufferPos는 그 다음 문자를 가지고 있다.
    m_dwBufferPos--;
    char ch = m_pszBuffer[m_dwBufferPos];

    //do
    //{
    //  // 현재 파일의 끝이면 END 리턴
    //  if((ch = (char)_getchar()) == INI_TOKEN_END)
    //      return INI_TOKEN_END;

    //} while(isspace(ch));

    //현재 가르키는 Pos부터 빈칸 체크 한다.
    //탭이면 9다.
    while(_IsSpace(ch))
    {
        // 현재 파일의 끝이면 END 리턴
        if((ch = (char)_getchar()) == INI_TOKEN_END)
            return INI_TOKEN_END;
    }

    return INI_SUCCESS;
}

void
INIParser::Display()
{
    //APP_MAP::iterator iter;
    //KEY_MAP::iterator KeyIter;
    //for(iter = m_mapApp.begin(); iter != m_mapApp.end(); ++iter)
    //{
    //  KEY_MAP* pKeyMap = iter->second;
    //  if(pKeyMap)
    //  {
    //      for(KeyIter = pKeyMap->begin(); KeyIter != pKeyMap->end(); ++KeyIter)
    //      {
    //          std::cout << "APP : " << iter->first << ", Key : " << KeyIter->first << ", value : " << KeyIter->second << std::endl;
    //      }
    //  }
    //}
}

// (WAVERIX) (090719) (CHANGES)
const char*
INIParser::GetData(const char* pszApp, const char* pszKey) const
{
    APP_MAP::const_iterator it_app = m_mapApp.find(pszApp);
    if(it_app != m_mapApp.end())
    {
        const KEY_MAP* pKeys = it_app->second;
        if(pKeys == 0)
            return 0;

        KEY_MAP::const_iterator it_key = pKeys->find(pszKey);
        if(it_key != pKeys->end()) {
            const std::string& key = it_key->second;
            return key.c_str();
        }
        else {
            // 이 경우는 특정 Attrbitue가 존재하지 않는 상황이다... 어캐 정책을 정할까? 일단
            // empty string으로 처리하기로 한다.
            return "";
        }
    }
    return 0;
}

//bool
//INIParser::GetData(const char* pszApp, const char* pszKey, OUT std::string& strData)
//{
//    APP_MAP::iterator   app_it;
//    KEY_MAP::iterator   key_it;
//
//    app_it = m_mapApp.find(pszApp);
//    if(app_it != m_mapApp.end())
//    {
//        KEY_MAP* pKeyMap = app_it->second;
//        if(pKeyMap)
//        {
//            key_it = pKeyMap->find(pszKey);
//            if(key_it != pKeyMap->end())
//            {
//                strData = key_it->second.c_str();
//                return TRUE;
//            }
//        }
//    }
//
//    //FASSERT(!"Not Found Data");
//    return FALSE;
//}

bool
INIParser::_IsSpace(char ch)
{
    //첫번째는 스페이스고 두번째는 탭이다.)
    if(ch == ' ' || ch == '\t')
        return true;

    return false;
}

// "\r\n" 체크
bool
INIParser::_IsCRLF(char ch)
{
    return (ch==13 && _getchar()==10) ? true:false;
}

}; //end of namespace 'util'
