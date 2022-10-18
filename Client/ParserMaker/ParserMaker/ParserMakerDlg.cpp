// ParserMakerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include <SolarFileReader.h>
#include "ParserMaker.h"
#include "ParserMakerDlg.h"
#include ".\parsermakerdlg.h"
#include <vector>

using namespace std;

#pragma comment( lib, "WzFilePacker_static.lib")
#pragma comment( lib, "Utility_d.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CParserMakerDlg 대화 상자

enum eDataType
{
	DATATYPE_DWORD = 0,
	DATATYPE_WORD,    
	DATATYPE_BYTE,
	DATATYPE_INT, 
	DATATYPE_STRING,
	DATATYPE_FLOAT,	

	DATATYPE_MAX
	
};

const char *strDataType[DATATYPE_MAX] =
{
	"DWORD",
	"WORD",
	"BYTE",
	"int",
	"char",
	"float"
};


typedef struct
{
	char szFieldName[256];
	eDataType DataType;

} FIELD;



CParserMakerDlg::CParserMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParserMakerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CParserMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CParserMakerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MAKE, OnBnClickedMake)
	ON_BN_CLICKED(IDC_BROWSEBUTTON, OnBnClickedBrowsebutton)
END_MESSAGE_MAP()


// CParserMakerDlg 메시지 처리기

BOOL CParserMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CParserMakerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CParserMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CParserMakerDlg::OnBnClickedMake()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strFileName;
	CString strClassName;

	CEdit *pEditFileName = (CEdit *)GetDlgItem(IDC_EDIT_FILENAME);
	CEdit *pEditClassName = (CEdit *)GetDlgItem(IDC_EDIT_CLASSNAME);

	pEditFileName->GetWindowText(strFileName);
	pEditClassName->GetWindowText(strClassName);

	char szFileName[256];
	char szClassName[256];


	strcpy(szFileName,strFileName);
	strcpy(szClassName,strClassName);

	int stringcount = 0;

	if (strlen(szClassName) < 1)
	{
		MessageBox("클래스 이름을 넣으세요","이런!");
		return;

	}

	vector<FIELD> fieldlist;

	fieldlist.clear();


	

	// 파일 열기
	util::SolarFileReader sr;
	if ( !sr.OpenFile( szFileName, SEPERATOR_WHITESPACE ) )
	{
		MessageBox("파일을 열수 없습니다","이런!");
		return;
	}

	char szField[256];

	while( TRUE )
	{
		eTOKEN_TYPE ettType = sr.GetNextTokenType(); 
		if ( TOKEN_END == ettType )
		{
			break;
		}
		else if ( TOKEN_STRING != ettType )
		{
			MessageBox("잘못된 헤더 파일입니다.","이런!");
			sr.CloseFile();
			return;
		}

		FIELD field;
		
		strncpy(field.szFieldName,sr.GetTokenString(),256);

		switch (field.szFieldName[0])
		{
		default:
		case 'd':
			field.DataType = DATATYPE_DWORD;
			break;
		case 'w':
			field.DataType = DATATYPE_WORD;
			break;
		case 'b':
			field.DataType = DATATYPE_BYTE;
			break;
		case 'i':
			field.DataType = DATATYPE_INT;
			break;		
		case 'f':
			field.DataType = DATATYPE_FLOAT;
			break;
		case 's':
			{
				field.DataType = DATATYPE_STRING;
				if (stringcount)
				{
					return;
				}
				stringcount++;
			}
			break;

		}

		fieldlist.push_back(field);
		
	}

	sr.CloseFile();

	char szHeaderFileName[256];
	char szSourceFileName[256];
	char szParserName[256];
	char szStructName[256];
	char szKeyName[256];

	sprintf(szParserName,"%sParser",szClassName);
	sprintf(szHeaderFileName,"%s.h",szParserName);
	sprintf(szSourceFileName,"%s.cpp",szParserName);
	sprintf(szStructName,"BASE_%s",szClassName);

	// 헤더 생성
	FILE *fp = fopen(szHeaderFileName,"wt");

	vector<FIELD>::iterator itr;

	itr = fieldlist.begin();

	sprintf(szKeyName,"m_%s",(*itr).szFieldName);

	if (fp)
	{
		

		fprintf(fp,"#pragma once\n\n");

		fprintf(fp,"#include <SolarHashTable.h>\n");
		fprintf(fp,"#include <Singleton.h>\n\n\n");

		// struct 만들기
		fprintf(fp,"struct %s {\n\n",szStructName);

		itr = fieldlist.begin();
		

		while(itr != fieldlist.end())
		{			
			// 해당하는 자료형으로 변수선언
			// string은 길이를 알수없으므로
			if ((*itr).DataType == DATATYPE_STRING) {
				fprintf(fp,"     %s     m_%s[REPLACE_ME_WITH_LENGTH];\n",strDataType[(*itr).DataType],(*itr).szFieldName);			
			}
			else
			{
				fprintf(fp,"     %s     m_%s;\n",strDataType[(*itr).DataType],(*itr).szFieldName);			

			}
			itr++;
		}

		fprintf(fp,"\n};\n\n\n");


		// class 생성
		fprintf(fp,"class %s	: public Singleton<%s> ,public BaseParser\n{\n",szParserName,szParserName);
		fprintf(fp,"public:\n");
		fprintf(fp,"   %s();\n",szParserName);
		fprintf(fp,"   ~%s();\n\n\n",szParserName);

		fprintf(fp,"   VOID           Init(DWORD dwPoolSize , TCHAR *szPackfileName);\n");
		fprintf(fp,"   VOID           Release();\n");
		fprintf(fp,"   BOOL           Load(char *pszFilename);\n\n");

		fprintf(fp,"   %s*            Get%s(DWORD dwKey);\n\n",szStructName,szClassName);
		
		fprintf(fp,"private:\n");

		fprintf(fp,"VOID		   Unload();\n");
		fprintf(fp,"util::SolarHashTable<%s *>		*m_pDataTable;\n",szStructName);



		fprintf(fp,"};\n");

		fclose(fp);

	}


	fp = fopen(szSourceFileName,"wt");

	

	if (fp)
	{
		
		

		fprintf(fp,"//------------------------------------------------------------------------------\
				    \n//\
                   	\n//	(C) 2005 y2jinc\
					\n//------------------------------------------------------------------------------\n");

		fprintf(fp,"#include \"SunClientPrecompiledHeader.h\"\n");
		fprintf(fp,"#include \"%s\"\n",szHeaderFileName);
		fprintf(fp,"#include <Struct.h>\n");
		fprintf(fp,"#include <SolarFileReader.h>\n\n\n");

		// 생성자
		fprintf(fp,"//----------------------------------------------------------------------------\
                   	\n/**\
                    \n*/\n");
		fprintf(fp,"%s::%s():	m_pDataTable ( NULL )\n{\n}\n\n",szParserName,szParserName);

		// 소멸자
		fprintf(fp,"//----------------------------------------------------------------------------\
                   	\n/**\
                    \n*/\n");
		fprintf(fp,"%s::~%s()\n{\n   ASSERT( m_pDataTable == NULL);\n}\n\n",szParserName,szParserName);

		// Release
		fprintf(fp,"//----------------------------------------------------------------------------\
                   	\n/**\
                    \n*/\n");
		fprintf(fp,"VOID %s::Release()\n{\n   Unload();\n   delete m_pDataTable;\n   m_pDataTable = NULL;\n}\n\n",szParserName);

		// Init
		fprintf(fp,"//----------------------------------------------------------------------------\
                   	\n/**\
                    \n*/\n");
		fprintf(fp,"VOID %s::Init(DWORD dwPoolSize , TCHAR *pszPackFileName)\n{\n   ASSERT( m_pDataTable == NULL );\n   m_pDataTable = new util::SolarHashTable<%s*>;\n   m_pDataTable->Initialize( dwPoolSize );\n SetPackFileName( pszPackFileName );\n}\n\n",szParserName,szStructName);

		// Unload
		fprintf(fp,"//----------------------------------------------------------------------------\
                   	\n/**\
                    \n*/\n");
		fprintf(fp,"VOID %s::Unload()\n{\n\
			%s * pInfo = NULL;\n\
			if( m_pDataTable == NULL )\n\
			    {\n\
				return;\n\
				}\n\
			m_pDataTable->SetFirst();\n\
			while( pInfo = m_pDataTable->GetNext() )\n\
			    {\n\
				     delete pInfo;\n\
			    }\n \
			m_pDataTable->RemoveAll();\n\
	        }\n\n",szParserName,szStructName);

		// Get함수
		fprintf(fp,"//----------------------------------------------------------------------------\
                   	\n/**\
                    \n*/\n");
		fprintf(fp,"%s* %s::Get%s(DWORD dwKey)\n{\n\n",szStructName,szParserName,szClassName);
		fprintf(fp,"return m_pDataTable->GetData( dwKey );\n");
		fprintf(fp,"}\n\n",szStructName,szParserName,szClassName);

		// Load 함수!!
		fprintf(fp,"#pragma warning ( push )\n");
		fprintf(fp,"#pragma warning ( disable : 4244)\n");

		fprintf(fp,"//----------------------------------------------------------------------------\
                   	\n/**\
                    \n*/\n");
		fprintf(fp,"BOOL %s::Load( char * pszFileName )\n",szParserName);
		fprintf(fp,"{\n\n");

		fprintf(fp,"ASSERT( m_pDataTable != NULL );\n");
		fprintf(fp,"Unload();\n\n\n");
	
		fprintf(fp,"util::SolarFileReader sr;\n");
		fprintf(fp,"if ( !OpenFile( sr , pszFileName, SEPERATOR_WHITESPACE ) )\n");
		fprintf(fp,"{\n\n");
		fprintf(fp,"ASSERT(0);\n");
		fprintf(fp,"return (FALSE);\n");
		fprintf(fp,"}\n\n");

		// 핵심 로드 부분

		fprintf(fp,"while( TRUE )\n");
	    fprintf(fp,"{\n\n");

		fprintf(fp,"eTOKEN_TYPE ettType = sr.GetNextTokenType();\n");
		fprintf(fp,"if ( TOKEN_END == ettType )\n");
		fprintf(fp,"{\n\n");
		fprintf(fp,"     break;\n");
		fprintf(fp,"}\n\n");

		fprintf(fp,"%s * pInfo = new %s;\n",szStructName,szStructName);

		itr = fieldlist.begin();

		fprintf(fp,"pInfo->m_%s = sr.GetTokenNumber();\n",(*itr).szFieldName);

		itr++;

		while (itr != fieldlist.end())
		{
			if (itr->DataType == DATATYPE_STRING) {
				fprintf(fp,"sr.GetNextTokenType(TOKEN_STRING); strncpy( pInfo->m_%s, sr.GetTokenString(), REPLACE_ME_WITH_LENGTH );\n",(*itr).szFieldName);
			}
			else
			{
				fprintf(fp,"sr.GetNextTokenType(TOKEN_NUMBER); pInfo->m_%s = sr.GetTokenNumber();\n",(*itr).szFieldName);
			}
			itr++;
		}

		fprintf(fp,"ASSERT( NULL == m_pDataTable->GetData( pInfo->%s ) );\n",szKeyName);
		fprintf(fp,"m_pDataTable->Add( pInfo, pInfo->%s );\n\n",szKeyName);

		fprintf(fp,"}\n\n"); // end while

		fprintf(fp,"sr.CloseFile();\n\n",szKeyName);
		
		fprintf(fp,"return TRUE;\n");

		fprintf(fp,"}\n\n\n\n"); // end unload

		fprintf(fp,"#pragma warning ( pop )\n");
		
		fclose(fp);

	}
	


	fieldlist.clear();

	MessageBox("파서를 성공적으로 작성하였습니다.","만세~");


	
}

void CParserMakerDlg::OnBnClickedBrowsebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		CEdit *pEditFileName = (CEdit *)GetDlgItem(IDC_EDIT_FILENAME);
		pEditFileName->SetWindowText(dlg.GetPathName());
	}


}
