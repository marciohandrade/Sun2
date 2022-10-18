#include <wininet.h>

#pragma comment(lib, "wininet")

// for InternetConnect
typedef struct 
{
	HINTERNET* pInternetSession;
	HINTERNET* pFtpConnection;
	LPCTSTR szUrl;
	int nPort;
	LPCTSTR szId;
	LPCTSTR szPassword;
} FTP_PARM;

class CUploader
{
public:
	const static int BUFFER_SIZE = 256;
	const static DWORD TIME_OUT = 3000;

	CUploader(LPCTSTR szUrl, int nPort, LPCTSTR szId, LPCTSTR szPassword, int nUploadCount);
	~CUploader();

	int			UploadFTP(LPCTSTR szLocalFile, LPCTSTR szServerFile);
	int			UploadHTTP();

private:
	TCHAR		m_szUrl[BUFFER_SIZE];		// ftp url
	int			m_nPort;				// ftp port
	TCHAR		m_szId[BUFFER_SIZE];		// ftp id
	TCHAR		m_szPassword[BUFFER_SIZE];	// ftp password
	int			m_nUploadCount;			// upload
};