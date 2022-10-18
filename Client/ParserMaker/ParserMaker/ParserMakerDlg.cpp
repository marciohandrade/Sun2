// ParserMakerDlg.cpp : ���� ����
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


// CParserMakerDlg ��ȭ ����

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


// CParserMakerDlg �޽��� ó����

BOOL CParserMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CParserMakerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CParserMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CParserMakerDlg::OnBnClickedMake()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
		MessageBox("Ŭ���� �̸��� ��������","�̷�!");
		return;

	}

	vector<FIELD> fieldlist;

	fieldlist.clear();


	

	// ���� ����
	util::SolarFileReader sr;
	if ( !sr.OpenFile( szFileName, SEPERATOR_WHITESPACE ) )
	{
		MessageBox("������ ���� �����ϴ�","�̷�!");
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
			MessageBox("�߸��� ��� �����Դϴ�.","�̷�!");
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

	// ��� ����
	FILE *fp = fopen(szHeaderFileName,"wt");

	vector<FIELD>::iterator itr;

	itr = fieldlist.begin();

	sprintf(szKeyName,"m_%s",(*itr).szFieldName);

	if (fp)
	{
		

		fprintf(fp,"#pragma once\n\n");

		fprintf(fp,"#include <SolarHashTable.h>\n");
		fprintf(fp,"#include <Singleton.h>\n\n\n");

		// struct �����
		fprintf(fp,"struct %s {\n\n",szStructName);

		itr = fieldlist.begin();
		

		while(itr != fieldlist.end())
		{			
			// �ش��ϴ� �ڷ������� ��������
			// string�� ���̸� �˼������Ƿ�
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


		// class ����
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

		// ������
		fprintf(fp,"//----------------------------------------------------------------------------\
                   	\n/**\
                    \n*/\n");
		fprintf(fp,"%s::%s():	m_pDataTable ( NULL )\n{\n}\n\n",szParserName,szParserName);

		// �Ҹ���
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

		// Get�Լ�
		fprintf(fp,"//----------------------------------------------------------------------------\
                   	\n/**\
                    \n*/\n");
		fprintf(fp,"%s* %s::Get%s(DWORD dwKey)\n{\n\n",szStructName,szParserName,szClassName);
		fprintf(fp,"return m_pDataTable->GetData( dwKey );\n");
		fprintf(fp,"}\n\n",szStructName,szParserName,szClassName);

		// Load �Լ�!!
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

		// �ٽ� �ε� �κ�

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

	MessageBox("�ļ��� ���������� �ۼ��Ͽ����ϴ�.","����~");


	
}

void CParserMakerDlg::OnBnClickedBrowsebutton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		CEdit *pEditFileName = (CEdit *)GetDlgItem(IDC_EDIT_FILENAME);
		pEditFileName->SetWindowText(dlg.GetPathName());
	}


}
