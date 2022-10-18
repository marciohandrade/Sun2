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

    // (WAVERIX) (090712) (ADDIN) �ش� INIParser�� ������ OverrunCheck�� ���ؼ��� �˻��ϱ�� �Ѵ�.
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


//���� �о� �帰 ������ �Ѱ��� ��ӵ� ��ȣ�� ���ö��� �� ����
INIParser::INIToken
INIParser::GetToken()
{
    if(!m_pszBuffer)
        return INI_ERROR;

    _skipComment();
    _undo_getchar(); //_skipComment���� ���� ��ġ�� ��ġ���� ����Ͽ����Ƿ� �Ѱ���ŭ ��~ �Ѵ�.

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
                return INI_ERROR;  //APP���� ������ ���� ����ǰų� ��ĭ�� ������ ����ȴ�.

            if(nCnt >= INI_DATA_SIZE)
                return INI_ERROR;

            m_szApp[nCnt] = ch;
            nCnt++;
        }

        m_szApp[nCnt] = '\0';
        return m_CurrentToken = INI_APP;

    default:
        //���ڿ��� ���(�׻� KEY���� ���ڿ��� �����Ѵ�.)
        if (isalpha(ch))    
        {
            //1. KEY�� ���Ѵ�.
            nCnt = 0;

            while(!_IsSpace(ch))
            {
                if(ch == INI_TOKEN_END || ch == '\n')
                    return INI_ERROR;  //APP���� ������ ���� ����ǰų� ��ĭ�� ������ ����ȴ�.

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

            //2. '='�� ���ö� ���� ����. ���� ���ڰ� '='�� �ƴϸ� �����Ѵ�.
            ch = _getchar();
            if((ch != '=') || (ch == INI_TOKEN_END))
                return INI_ERROR;

            // '='�� �������Ƿ�, �� ������ ������ �´�.
            _getchar();

            if(_skipSpace() == INI_TOKEN_END)
                return INI_ERROR;

            _undo_getchar(); //_skipSpace���� ���� ��ġ�� ��ġ���� ����Ͽ����Ƿ� �Ѱ���ŭ ��~ �Ѵ�.

            //3. Value�� ���Ѵ�.  ������ ���ڿ��� �ִ´�.
            nCnt = 0;
            ch = _getchar();

            while(!_IsSpace(ch))        
            {
                if(ch == INI_TOKEN_END || ch == '\n' || _IsCRLF(ch))
                {
                    if(nCnt > 0)
                        break;
                    else
                        return INI_ERROR;  //APP���� ������ ���� ����ǰų� ��ĭ�� ������ ����ȴ�.
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

//���� m_dwBufferPos�� ��ġ�� ���ڿ��� �����ϰ�, m_dwBufferPos�� ��ġ�� �ϳ� ���� ��Ų��.
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
            //APP ���� ����.
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
        // ���� ������ ���̸� END ����
        if((ch = (char)_getchar()) == INI_TOKEN_END)
            return INI_TOKEN_END;

        // ���� // ���̸� '\n'�� ���ö� ���� ���ƶ�.
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
    //���� ���� ������ �ִ� ���ڸ� ch�� �ְ�, m_dwBufferPos�� �� ���� ���ڸ� ������ �ִ�.
    m_dwBufferPos--;
    char ch = m_pszBuffer[m_dwBufferPos];

    //do
    //{
    //  // ���� ������ ���̸� END ����
    //  if((ch = (char)_getchar()) == INI_TOKEN_END)
    //      return INI_TOKEN_END;

    //} while(isspace(ch));

    //���� ����Ű�� Pos���� ��ĭ üũ �Ѵ�.
    //���̸� 9��.
    while(_IsSpace(ch))
    {
        // ���� ������ ���̸� END ����
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
            // �� ���� Ư�� Attrbitue�� �������� �ʴ� ��Ȳ�̴�... ��ĳ ��å�� ���ұ�? �ϴ�
            // empty string���� ó���ϱ�� �Ѵ�.
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
    //ù��°�� �����̽��� �ι�°�� ���̴�.)
    if(ch == ' ' || ch == '\t')
        return true;

    return false;
}

// "\r\n" üũ
bool
INIParser::_IsCRLF(char ch)
{
    return (ch==13 && _getchar()==10) ? true:false;
}

}; //end of namespace 'util'
