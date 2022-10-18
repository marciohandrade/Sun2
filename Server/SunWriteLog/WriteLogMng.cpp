#include "stdafx.h"
#include ".\writelogmng.h"

//#include <afxcom_.h>

#include "ActionData.h"
#include "AdminData.h"
#include "ChatData.h"
#include "ExpData.h"
#include "ItemData.h"
#include "MoneyData.h"
#include "SessionData.h"
#include "SkillData.h"
#include "SnapShotData.h"
#include "WareHouseData.h"
#include "SolarLog.h"
#include "GuildData.h"

#include "AccumulatePointData.h"

#ifdef __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
static int SearchForRecord(void* buf, size_t len);
#endif // __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY

//---------------------------------------------------------------------------------------------------------------------

#ifdef _NA_0_20110305_BUG_READ_BIG_LOGFILE

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//---------------------------------------------------------------------------------------------------------------------

struct CWriteLogMng::_MEMBER
{	
    SessionData		m_SessionData;
    ItemData		m_ItemData;
    ExpData			m_ExpData;
    ActionData		m_ActionData;
    SkillData		m_SkillData;
    SnapShotData	m_SnapShotData;
    ChatData		m_ChatData;
    AdminData		m_AdminData;
    MoneyData		m_MoneyData;
    WareHouseData   m_WareHouseData;
    GuildData		m_GuildData;

    AccumulatePointData m_AccumulatePointData;

    _MEMBER()
    {
        Init();
    }
    void Init()
    {
        m_SessionData.Init();
        m_ItemData.Init();
        m_ExpData.Init();
        m_ActionData.Init();
        m_SkillData.Init();
        m_SnapShotData.Init();
        m_ChatData.Init();
        m_AdminData.Init();
        m_MoneyData.Init();
        m_GuildData.Init();

        m_AccumulatePointData.Init();
    }
};

CWriteLogMng::CWriteLogMng(VOID)
{
    m_bTextOut = FALSE;
    file_handle_ = Null_File;
    m_pSolarLog = new SolarLog ;
    m_pSolarLogText = new SolarLog;	
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    m_pSpoolBuffer = new BYTE[SPOOL_BUFFER_SIZE]; // ��Ǯ ���� �Ҵ�, Read ��忡���� ��ȿ�� �˻�� ���۷� �̿�
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    m_pMEMBER = new _MEMBER;
}

CWriteLogMng::~CWriteLogMng(VOID)
{
    delete m_pSolarLog;
    delete m_pSolarLogText;
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    if (m_pSpoolBuffer != NULL) {
        delete[] m_pSpoolBuffer; // ��Ǯ ���� ����
    }
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    delete m_pMEMBER;
}

BOOL CWriteLogMng::Init( TCHAR* pszLogFilePrefix, TCHAR* pszBinDirName, TCHAR* pszTextDirName, BOOL bTextOut )
{	
    m_bTextOut = bTextOut;
    // �ݵ�� ���̳ʸ� �ؽ�Ʈ ������ �α����� ��δ� �־�� �Ѵ�.
    if( !pszLogFilePrefix || !pszBinDirName )
        return FALSE;

    // Bin �α� ����
    if( !m_pSolarLog->Create( pszBinDirName, pszLogFilePrefix ) )
        return FALSE;

    // Text �� �α� ����
    if( bTextOut )
    {
        if( !m_pSolarLogText->Create( pszTextDirName, pszLogFilePrefix ) )
            return FALSE;
    }

    return TRUE;
}

VOID CWriteLogMng::Serialize( BaseLogData& IN rData )
{
    CLogArchive	Archive(m_pSolarLog, m_pSolarLogText);

#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    if (m_pSpoolBuffer != NULL) {
        Archive.InitSpooling(m_pSpoolBuffer, SPOOL_BUFFER_SIZE);
        Archive.BeginSpooling();
    }
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT

    rData.Serialize( Archive, m_bTextOut );	

#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    if (m_pSpoolBuffer != NULL) {
        Archive.Flush();
    }
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
}

BOOL CWriteLogMng::Open( TCHAR* pszFileName, fpos_t& end_offset)
{
    if (Open(pszFileName))
    {
        end_offset = end_offset_;
        return TRUE;
    }
    return FALSE;
}

BOOL CWriteLogMng::Open( TCHAR* pszFileName)
{	
    //�̹� ������ �����ִ� ����
    if (file_handle_ != Null_File)
        return FALSE;
    file_handle_ = _open(pszFileName, _O_BINARY|_O_RDONLY, _S_IREAD);
    if(file_handle_ == Null_File)
        return FALSE;

    end_offset_ = _lseeki64(file_handle_, 0L, SEEK_END);
    if (end_offset_ == -1L)
    {
        Close();
        return FALSE;
    }

    _lseeki64(file_handle_, 0L, SEEK_SET);

    m_pMEMBER->Init();

    return TRUE;
}

BOOL CWriteLogMng::Close()
{
    //������ ���������� �ʾҴ�
    if (file_handle_ == Null_File)
        return FALSE;

    _close(file_handle_);
    file_handle_ = Null_File;
    return TRUE;
}

BOOL CWriteLogMng::SetLogFileOffset(fpos_t Pos)
{
    //������ ���������� �ʾҴ�
    if (file_handle_ == Null_File)
        return FALSE;

    if (_lseeki64(file_handle_, Pos, SEEK_SET) == -1L)
        return FALSE;

    return TRUE;
}

fpos_t CWriteLogMng::GetLogFileOffset()
{
    //������ ���������� �ʾҴ�
    if (file_handle_ == Null_File)
        return 0;

    return _lseeki64(file_handle_, 0, SEEK_CUR);
}
   
BaseLogData* CWriteLogMng::Read(int& error_code)
{
    error_code = CWriteLogMng::error_unknown;
    fpos_t prev_pos = GetLogFileOffset();
    if (prev_pos >= end_offset_)
    {
        error_code = CWriteLogMng::error_eof;
        return NULL;
    }

    eLOG_CATEGORY read_category;
    if (_read(file_handle_, &read_category, sizeof(read_category)) <= 0)
    {
        SetLogFileOffset(prev_pos);
        return NULL;
    }

    ushort base_version;
    if(_read(file_handle_, &base_version, sizeof(base_version)) <= 0)
    {
        SetLogFileOffset(prev_pos);
        return NULL;
    }

    int log_type;
    if(_read(file_handle_, &log_type, sizeof(log_type)) <= 0)
    {
        SetLogFileOffset(prev_pos);
        return NULL;
    }

    if (!SetLogFileOffset(prev_pos))
        return NULL;

    BaseLogData* base_log_data = NULL;
    size_t sample_length = 0;
    switch(read_category)
    {
    case LOG_SESSION:
        base_log_data = &(m_pMEMBER->m_SessionData);
        sample_length = sizeof(m_pMEMBER->m_SessionData);
        break;
    case LOG_ITEM:
        base_log_data = &(m_pMEMBER->m_ItemData);
        sample_length = sizeof(m_pMEMBER->m_ItemData);
        break;
    case LOG_EXP:
        base_log_data = &(m_pMEMBER->m_ExpData);
        sample_length = sizeof(m_pMEMBER->m_ExpData);
        break;
    case LOG_ACTION:
        base_log_data = &(m_pMEMBER->m_ActionData);
        sample_length = sizeof(m_pMEMBER->m_ActionData);
        break;
    case LOG_SKILL:
        base_log_data = &(m_pMEMBER->m_SkillData);
        sample_length = sizeof(m_pMEMBER->m_SkillData);
        break;
    case LOG_SNAPSHOT:
        switch(log_type)
        {
        case SNAPSHOT_REGULAR:
            base_log_data = &(m_pMEMBER->m_SnapShotData);
            sample_length = sizeof(m_pMEMBER->m_SnapShotData);
            break;
        case SNAPSHOT_WAREHOUSE_OEPN:
        case SNAPSHOT_WAREHOUSE_CLOSE:
            base_log_data = &(m_pMEMBER->m_WareHouseData);
            sample_length = sizeof(m_pMEMBER->m_WareHouseData);
            break;
        default:
            ASSERT(FALSE);
        }

        break;
    case LOG_CHAT:
        base_log_data = &(m_pMEMBER->m_ChatData);
        sample_length = sizeof(m_pMEMBER->m_ChatData);
        break;
    case LOG_EVENT:
        return NULL;
    case LOG_ADMIN:
        base_log_data = &(m_pMEMBER->m_AdminData);
        sample_length = sizeof(m_pMEMBER->m_AdminData);
        break;
    case LOG_MONEY:
        base_log_data = &(m_pMEMBER->m_MoneyData);
        sample_length = sizeof(m_pMEMBER->m_MoneyData);
        break;
    case LOG_GUILD:
        base_log_data = &(m_pMEMBER->m_GuildData);
        sample_length = sizeof(m_pMEMBER->m_GuildData);
        break;
    case LOG_ACCUMULATEPOINT:
        base_log_data = &(m_pMEMBER->m_AccumulatePointData);
        sample_length = sizeof(m_pMEMBER->m_AccumulatePointData);
        break;
    default:
        error_code = CWriteLogMng::error_category;
        return NULL;
    }

    CLogArchive	Archive(file_handle_);
    base_log_data->Read(Archive);
    error_code = CWriteLogMng::error_success;
    return base_log_data;
}

#ifdef __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
BaseLogData* CWriteLogMng::Read(bool autoSkip)
{
retry:
    fpos_t prev_pos = GetLogFileOffset();
    eLOG_CATEGORY read_category;
    if (_read(file_handle_, &read_category, sizeof(read_category)) <= 0)
    {
        SetLogFileOffset(prev_pos);
        return NULL;
    }

    ushort base_version;
    if(_read(file_handle_, &base_version, sizeof(base_version)) <= 0)
    {
        SetLogFileOffset(prev_pos);
        return NULL;
    }

    int log_type;
    if(_read(file_handle_, &log_type, sizeof(log_type)) <= 0)
    {
        SetLogFileOffset(prev_pos);
        return NULL;
    }

    if (!SetLogFileOffset(prev_pos))
        return NULL;

    BaseLogData* base_log_data = NULL;
    size_t sample_length = 0;
    switch(read_category)
    {
    case LOG_SESSION:
        base_log_data = &(m_pMEMBER->m_SessionData);
        sample_length = sizeof(m_pMEMBER->m_SessionData);
        break;
    case LOG_ITEM:
        base_log_data = &(m_pMEMBER->m_ItemData);
        sample_length = sizeof(m_pMEMBER->m_ItemData);
        break;
    case LOG_EXP:
        base_log_data = &(m_pMEMBER->m_ExpData);
        sample_length = sizeof(m_pMEMBER->m_ExpData);
        break;
    case LOG_ACTION:
        base_log_data = &(m_pMEMBER->m_ActionData);
        sample_length = sizeof(m_pMEMBER->m_ActionData);
        break;
    case LOG_SKILL:
        base_log_data = &(m_pMEMBER->m_SkillData);
        sample_length = sizeof(m_pMEMBER->m_SkillData);
        break;
    case LOG_SNAPSHOT:
        switch(log_type)
        {
        case SNAPSHOT_REGULAR:
            base_log_data = &(m_pMEMBER->m_SnapShotData);
            sample_length = sizeof(m_pMEMBER->m_SnapShotData);
            break;
        case SNAPSHOT_WAREHOUSE_OEPN:
        case SNAPSHOT_WAREHOUSE_CLOSE:
            base_log_data = &(m_pMEMBER->m_WareHouseData);
            sample_length = sizeof(m_pMEMBER->m_WareHouseData);
            break;
        default:
            ASSERT(FALSE);
        }

        break;
    case LOG_CHAT:
        base_log_data = &(m_pMEMBER->m_ChatData);
        sample_length = sizeof(m_pMEMBER->m_ChatData);
        break;
    case LOG_EVENT:
        return NULL;
    case LOG_ADMIN:
        base_log_data = &(m_pMEMBER->m_AdminData);
        sample_length = sizeof(m_pMEMBER->m_AdminData);
        break;
    case LOG_MONEY:
        base_log_data = &(m_pMEMBER->m_MoneyData);
        sample_length = sizeof(m_pMEMBER->m_MoneyData);
        break;
    case LOG_GUILD:
        base_log_data = &(m_pMEMBER->m_GuildData);
        sample_length = sizeof(m_pMEMBER->m_GuildData);
        break;
    case LOG_ACCUMULATEPOINT:
        base_log_data = &(m_pMEMBER->m_AccumulatePointData);
        sample_length = sizeof(m_pMEMBER->m_AccumulatePointData);
        break;
    default:
        return NULL;
    }

    if (autoSkip) 
    {
        // ���ڵ� ��ȿ�� �˻� �� ���� ���ڵ� Skip
        // �� ���ڵ��� ���̴� �ּ��� sizeof(LogData)���ٴ� Ŭ ���̶�� ���� �Ͽ�
        // ���ø� ������ ������ ������������ �� ���ڵ尡 �����ϴ��� �˻�
        __int64 offset_next = -1;
        if (!SearchForNextRecord(sample_length, offset_next)) 
            return NULL;

        if (offset_next >= 0) 
        {
            _lseeki64(file_handle_, offset_next, SEEK_CUR);
            goto retry;
        }
    }

    CLogArchive	Archive(file_handle_);
    base_log_data->Read(Archive);
    return base_log_data;
}
// ���� ��Ʈ���� ���� sampleLength ���� ���� ���ڵ� ������ �ִ��� �˻��Ѵ�.
bool CWriteLogMng::SearchForNextRecord(size_t sampleLength, __int64& offset_next)
{
    static size_t baseLength = sizeof(BaseLogData);
    if (NULL == m_pSpoolBuffer) {
        return true; // Not supported
    }
    if (sampleLength < baseLength) {
        return false;
    }
    if (_read(file_handle_, m_pSpoolBuffer, sampleLength) <= 0) {
        return false; // Unexpected EOF
    }
    size_t length = sampleLength - baseLength; // ������ �˻��� ����
    int result = SearchForRecord(m_pSpoolBuffer + baseLength, length);
    __int64 offset_i64 = (__int64)0 - (__int64)sampleLength;
    _lseeki64(file_handle_, offset_i64, SEEK_CUR);
    if (result >= 0)
    {
        offset_next = result + (int)baseLength;
    }
    else
    {
        offset_next = (__int64)result;
    }
    return true;
}

static inline int HexTo2DigitInt(BYTE* p)
{
    char s[3];
    s[0] = *p++;
    s[1] = *p;
    s[2] = '\0';
    return atoi(s);
}

// �־��� ���� ���� ������ ���� �α� ���ڵ� ������ �����ϴ��� �˻���
// ���� ���̽��κ����� �������� �����Ѵ�. �߰����� �������� -1�� �����Ѵ�.
// ���� ���ڵ� ��踦 �ĺ��� �� �ִ� Sentinel�� �������� �����Ƿ�, 
// �˻�� �ִ� �˻� ����Ʈ ���� �޾� Heuristics�� �����Ѵ�.
// �̷� �Ѱ�� ����, �ĺ� ������ ���ɼ��� ������ �� �ۿ� ����.
int SearchForRecord(void* buf, size_t len)
{
    BYTE* p = (BYTE*)buf;
    // ���� (10 bytes)... 0F YYMMDDhhmmssSSS (IDLen) ID (NameLen) Name (IPLen) IP
    // �ϴ� ȿ���� ������� �ʰ� ������ �Ѵ�.
    // �翬�� �ӵ��� ��Ȯ���� ����ų ������ ���ù���������, 
    // ���� ���⿡ ������ �ʿ�� ���� ���� �� ����.
    int i = 10 - 1;
    while ( ++i < (int)len && i < (int)len - 16 ) {
        if (0x0F == p[i]) {
            int j = 0;
            for (; j < 15; ++j) {
                BYTE c = p[i + j + 1];
                if (c < 0x30 || 0x39 < c ) {
                    break;
                }
            }
            if (15 == j) {
                int month = HexTo2DigitInt(&p[i + 2 + 1]);
                if ( month < 1 || 12 < month ) {
                    continue;
                }
                int day = HexTo2DigitInt(&p[i + 4 + 1]);
                if ( day < 1 || 31 < day ) {
                    continue;
                }
                int hour = HexTo2DigitInt(&p[i + 6 + 1]);
                if ( hour < 0 || 23 < hour ) {
                    continue;
                }
                int minute = HexTo2DigitInt(&p[i + 8 + 1]);
                if ( minute < 0 || 59 < minute ) {
                    continue;
                }
                int second = HexTo2DigitInt(&p[i + 10 + 1]);
                if ( second < 0 || 59 < second ) {
                    continue;
                }
                BYTE c = p[i + 15 + 1];
                if (c <= 24) { // ID �ִ� ����
                    // �� ���� �α� ī�װ� ��ȿ���� �˻��ϸ� ������
                    return (i - 10); // ��Ī �Ǵ�
                }
            }
        }
    }
    return -1;
}
#else // __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
BaseLogData* CWriteLogMng::Read()
{
    ASSERT(0);
    return NULL;
}
#endif // __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY





//---------------------------------------------------------------------------------------------------------------------

#else //_NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------


struct CWriteLogMng::_MEMBER
{	
	SessionData		m_SessionData;
	ItemData		m_ItemData;
	ExpData			m_ExpData;
	ActionData		m_ActionData;
	SkillData		m_SkillData;
	SnapShotData	m_SnapShotData;
	ChatData		m_ChatData;
	AdminData		m_AdminData;
	MoneyData		m_MoneyData;
	WareHouseData   m_WareHouseData;
	GuildData		m_GuildData;

    AccumulatePointData m_AccumulatePointData;

	_MEMBER()
	{
		m_SessionData.Init();
		m_ItemData.Init();
		m_ExpData.Init();
		m_ActionData.Init();
		m_SkillData.Init();
		m_SnapShotData.Init();
		m_ChatData.Init();
		m_AdminData.Init();
		m_MoneyData.Init();
		m_GuildData.Init();

        m_AccumulatePointData.Init();
	}
};

CWriteLogMng::CWriteLogMng(VOID)
{
	m_bTextOut = FALSE;
	m_pFileStream = NULL;

	m_pSolarLog = new SolarLog ;
	m_pSolarLogText = new SolarLog;	
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    m_pSpoolBuffer = new BYTE[SPOOL_BUFFER_SIZE]; // ��Ǯ ���� �Ҵ�, Read ��忡���� ��ȿ�� �˻�� ���۷� �̿�
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
}

CWriteLogMng::~CWriteLogMng(VOID)
{
	delete m_pSolarLog;
	delete m_pSolarLogText;
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    if (m_pSpoolBuffer != NULL) {
        delete[] m_pSpoolBuffer; // ��Ǯ ���� ����
    }
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
}

BOOL	CWriteLogMng::Init( TCHAR* pszLogFilePrefix, TCHAR* pszBinDirName, TCHAR* pszTextDirName, BOOL bTextOut )
{	
	m_bTextOut = bTextOut;
	// �ݵ�� ���̳ʸ� �ؽ�Ʈ ������ �α����� ��δ� �־�� �Ѵ�.
	if( !pszLogFilePrefix || !pszBinDirName )
		return FALSE;

	// Bin �α� ����
	if( !m_pSolarLog->Create( pszBinDirName, pszLogFilePrefix ) )
		return FALSE;

	// Text �� �α� ����
	if( bTextOut )
	{
		if( !m_pSolarLogText->Create( pszTextDirName, pszLogFilePrefix ) )
			return FALSE;
	}

	return TRUE;
}

VOID CWriteLogMng::Serialize( BaseLogData& IN rData )
{
	CLogArchive	Archive(m_pSolarLog, m_pSolarLogText);

#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    if (m_pSpoolBuffer != NULL) {
        Archive.InitSpooling(m_pSpoolBuffer, SPOOL_BUFFER_SIZE);
        Archive.BeginSpooling();
    }
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT

	rData.Serialize( Archive, m_bTextOut );	

#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    if (m_pSpoolBuffer != NULL) {
        Archive.Flush();
    }
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
}

BOOL CWriteLogMng::Open( TCHAR* pszFileName, fpos_t& end_offset)
{
    if (Open(pszFileName))
    {
        if (fseek(m_pFileStream, 0L, SEEK_END) == 0)
        {
            fgetpos(m_pFileStream, &end_offset);
            fseek(m_pFileStream, 0L, SEEK_SET);
            return TRUE;
        }
        else
        {
            Close();
            return FALSE;
        }
    }
    return FALSE;
}

BOOL CWriteLogMng::Open( TCHAR* pszFileName)
{	
	//�̹� ������ �����ִ� ����
	if( m_pFileStream != NULL )
		return FALSE;

	m_pFileStream = fopen(pszFileName, "rb");

	if(m_pFileStream == NULL)
	{		
		return FALSE;
	}

	m_pMEMBER = new _MEMBER;

	return TRUE;
}
BOOL CWriteLogMng::Close()
{
	//������ ���������� �ʾҴ�
	if( m_pFileStream == NULL )
		return FALSE;

	fclose( m_pFileStream );
	m_pFileStream = NULL;

	delete m_pMEMBER;

	return TRUE;
}

#ifdef __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
BaseLogData* CWriteLogMng::Read(bool autoSkip)
{
retry:
    eLOG_CATEGORY eReadCategory;	
    if(fread(&eReadCategory, sizeof(eReadCategory), 1, m_pFileStream) <= 0)
        return NULL;

    WORD wBaseVersion;
    if(fread(&wBaseVersion, sizeof(wBaseVersion), 1, m_pFileStream) <= 0)
        return NULL;

    int iLogType;
    if(fread(&iLogType, sizeof(iLogType), 1, m_pFileStream) <= 0)
        return NULL;

    long lDistance = 0 - (sizeof(eReadCategory) + sizeof(wBaseVersion) + sizeof(iLogType));
    fseek(m_pFileStream, lDistance, SEEK_CUR );

    BaseLogData* pBaseData = NULL;
    size_t sampleLength = 0;
    switch(eReadCategory)
    {
    case LOG_SESSION:
        pBaseData = &(m_pMEMBER->m_SessionData);
        sampleLength = sizeof(m_pMEMBER->m_SessionData);
        break;
    case LOG_ITEM:
        pBaseData = &(m_pMEMBER->m_ItemData);
        sampleLength = sizeof(m_pMEMBER->m_ItemData);
        break;
    case LOG_EXP:
        pBaseData = &(m_pMEMBER->m_ExpData);
        sampleLength = sizeof(m_pMEMBER->m_ExpData);
        break;
    case LOG_ACTION:
        pBaseData = &(m_pMEMBER->m_ActionData);
        sampleLength = sizeof(m_pMEMBER->m_ActionData);
        break;
    case LOG_SKILL:
        pBaseData = &(m_pMEMBER->m_SkillData);
        sampleLength = sizeof(m_pMEMBER->m_SkillData);
        break;
    case LOG_SNAPSHOT:
        switch(iLogType)
        {
        case SNAPSHOT_REGULAR:
            pBaseData = &(m_pMEMBER->m_SnapShotData);
            sampleLength = sizeof(m_pMEMBER->m_SnapShotData);
            break;
        case SNAPSHOT_WAREHOUSE_OEPN:
        case SNAPSHOT_WAREHOUSE_CLOSE:
            pBaseData = &(m_pMEMBER->m_WareHouseData);
            sampleLength = sizeof(m_pMEMBER->m_WareHouseData);
            break;
        default:
            ASSERT(FALSE);
        }

        break;
    case LOG_CHAT:
        pBaseData = &(m_pMEMBER->m_ChatData);
        sampleLength = sizeof(m_pMEMBER->m_ChatData);
        break;
    case LOG_EVENT:
        return NULL;
    case LOG_ADMIN:
        pBaseData = &(m_pMEMBER->m_AdminData);
        sampleLength = sizeof(m_pMEMBER->m_AdminData);
        break;
    case LOG_MONEY:
        pBaseData = &(m_pMEMBER->m_MoneyData);
        sampleLength = sizeof(m_pMEMBER->m_MoneyData);
        break;
    case LOG_GUILD:
        pBaseData = &(m_pMEMBER->m_GuildData);
        sampleLength = sizeof(m_pMEMBER->m_GuildData);
        break;
    case LOG_ACCUMULATEPOINT:
        pBaseData = &(m_pMEMBER->m_AccumulatePointData);
        sampleLength = sizeof(m_pMEMBER->m_AccumulatePointData);
        break;
    default:
        return NULL;
    }

    if (autoSkip) {
        // ���ڵ� ��ȿ�� �˻� �� ���� ���ڵ� Skip
        // �� ���ڵ��� ���̴� �ּ��� sizeof(LogData)���ٴ� Ŭ ���̶�� ���� �Ͽ�
        // ���ø� ������ ������ ������������ �� ���ڵ尡 �����ϴ��� �˻�
        int offsetNext;
        if (!SearchForNextRecord(sampleLength, &offsetNext)) {
            return NULL;
        }
        if (offsetNext >= 0) 
        {
            fseek(m_pFileStream, (long)offsetNext, SEEK_CUR );
            goto retry;
        }
    }

    CLogArchive	Archive( m_pFileStream );
    pBaseData->Read( Archive );

    return pBaseData;
}
#else // __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
BaseLogData* CWriteLogMng::Read()
{
    eLOG_CATEGORY eReadCategory;	
    if(fread(&eReadCategory, sizeof(eReadCategory), 1, m_pFileStream) <= 0)
        return NULL;

    WORD wBaseVersion;
    if(fread(&wBaseVersion, sizeof(wBaseVersion), 1, m_pFileStream) <= 0)
        return NULL;

    int iLogType;
    if(fread(&iLogType, sizeof(iLogType), 1, m_pFileStream) <= 0)
        return NULL;

    long lDistance = 0 - (sizeof(eReadCategory) + sizeof(wBaseVersion) + sizeof(iLogType));
    fseek(m_pFileStream, lDistance, SEEK_CUR );

    BaseLogData* pBaseData;
    switch(eReadCategory)
    {
    case LOG_SESSION:
        pBaseData = &(m_pMEMBER->m_SessionData);
        break;
    case LOG_ITEM:
        pBaseData = &(m_pMEMBER->m_ItemData);
        break;
    case LOG_EXP:
        pBaseData = &(m_pMEMBER->m_ExpData);
        break;
    case LOG_ACTION:
        pBaseData = &(m_pMEMBER->m_ActionData);
        break;
    case LOG_SKILL:
        pBaseData = &(m_pMEMBER->m_SkillData);
        break;
    case LOG_SNAPSHOT:
        switch(iLogType)
        {
        case SNAPSHOT_REGULAR:
            pBaseData = &(m_pMEMBER->m_SnapShotData);
            break;
        case SNAPSHOT_WAREHOUSE_OEPN:
        case SNAPSHOT_WAREHOUSE_CLOSE:
            pBaseData = &(m_pMEMBER->m_WareHouseData);
            break;
        default:
            ASSERT(FALSE);
        }

        break;
    case LOG_CHAT:
        pBaseData = &(m_pMEMBER->m_ChatData);
        break;
    case LOG_EVENT:
        break;
    case LOG_ADMIN:
        pBaseData = &(m_pMEMBER->m_AdminData);
        break;
    case LOG_MONEY:
        pBaseData = &(m_pMEMBER->m_MoneyData);
        break;
    case LOG_GUILD:
        pBaseData = &(m_pMEMBER->m_GuildData);
        break;
    case LOG_ACCUMULATEPOINT:
        pBaseData = &(m_pMEMBER->m_AccumulatePointData);
        break;

    default:
        return NULL;
    }

    CLogArchive	Archive( m_pFileStream );
    pBaseData->Read( Archive );

    return pBaseData;
}
#endif // __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY

BOOL CWriteLogMng::SetLogFileOffset(fpos_t Pos)
{
	//������ ���������� �ʾҴ�
	if( m_pFileStream == NULL )
		return FALSE;

	fsetpos(m_pFileStream, &Pos);

	return TRUE;
}

fpos_t CWriteLogMng::GetLogFileOffset()
{
	//������ ���������� �ʾҴ�
	if( m_pFileStream == NULL )
		return 0;

	fpos_t	FilePointer;
	fgetpos(m_pFileStream, &FilePointer);
    
	return FilePointer;
}

#ifdef __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
// ���� ��Ʈ���� ���� sampleLength ���� ���� ���ڵ� ������ �ִ��� �˻��Ѵ�.
bool CWriteLogMng::SearchForNextRecord(size_t sampleLength, int* offset)
{
    *offset = -1;
    static size_t baseLength = sizeof(BaseLogData);
    if (NULL == m_pSpoolBuffer) {
        return true; // Not supported
    }
    if (sampleLength < baseLength) {
        return false;
    }
    if (fread(m_pSpoolBuffer, sampleLength, 1, m_pFileStream) <= 0) {
        return false; // Unexpected EOF
    }
    size_t length = sampleLength - baseLength; // ������ �˻��� ����
    int result = SearchForRecord(m_pSpoolBuffer + baseLength, length);
    fseek(m_pFileStream, 0 - (long)sampleLength, SEEK_CUR);
    *offset = ( result >= 0 ? result + (int)baseLength : result );
    return true;
}

static inline int HexTo2DigitInt(BYTE* p)
{
    char s[3];
    s[0] = *p++;
    s[1] = *p;
    s[2] = '\0';
    return atoi(s);
}

// �־��� ���� ���� ������ ���� �α� ���ڵ� ������ �����ϴ��� �˻���
// ���� ���̽��κ����� �������� �����Ѵ�. �߰����� �������� -1�� �����Ѵ�.
// ���� ���ڵ� ��踦 �ĺ��� �� �ִ� Sentinel�� �������� �����Ƿ�, 
// �˻�� �ִ� �˻� ����Ʈ ���� �޾� Heuristics�� �����Ѵ�.
// �̷� �Ѱ�� ����, �ĺ� ������ ���ɼ��� ������ �� �ۿ� ����.
int SearchForRecord(void* buf, size_t len)
{
    BYTE* p = (BYTE*)buf;
    // ���� (10 bytes)... 0F YYMMDDhhmmssSSS (IDLen) ID (NameLen) Name (IPLen) IP
    // �ϴ� ȿ���� ������� �ʰ� ������ �Ѵ�.
    // �翬�� �ӵ��� ��Ȯ���� ����ų ������ ���ù���������, 
    // ���� ���⿡ ������ �ʿ�� ���� ���� �� ����.
    int i = 10 - 1;
    while ( ++i < (int)len && i < (int)len - 16 ) {
        if (0x0F == p[i]) {
            int j = 0;
            for (; j < 15; ++j) {
                BYTE c = p[i + j + 1];
                if (c < 0x30 || 0x39 < c ) {
                    break;
                }
            }
            if (15 == j) {
                int month = HexTo2DigitInt(&p[i + 2 + 1]);
                if ( month < 1 || 12 < month ) {
                    continue;
                }
                int day = HexTo2DigitInt(&p[i + 4 + 1]);
                if ( day < 1 || 31 < day ) {
                    continue;
                }
                int hour = HexTo2DigitInt(&p[i + 6 + 1]);
                if ( hour < 0 || 23 < hour ) {
                    continue;
                }
                int minute = HexTo2DigitInt(&p[i + 8 + 1]);
                if ( minute < 0 || 59 < minute ) {
                    continue;
                }
                int second = HexTo2DigitInt(&p[i + 10 + 1]);
                if ( second < 0 || 59 < second ) {
                    continue;
                }
                BYTE c = p[i + 15 + 1];
                if (c <= 24) { // ID �ִ� ����
                    // �� ���� �α� ī�װ� ��ȿ���� �˻��ϸ� ������
                    return (i - 10); // ��Ī �Ǵ�
                }
            }
        }
    }
    return -1;
}
#endif // __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY


//---------------------------------------------------------------------------------------------------------------------

#endif //_NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------
