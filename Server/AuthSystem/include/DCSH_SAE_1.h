/***************************************************************************
* �ļ�:     DCSH_SAE_1.h
*
* ����˵��: ������̬��ȫ��֤����1.0ͷ�ļ�
*
* ��Ȩ���У��Ϻ�������Ϣ�������޹�˾
*
* ���ߣ�    �
* ʱ�䣺    2008��5��13��
* �ص㣺    �Ϻ� ����
*
***************************************************************************/
/*-------------------------------------------------------------------------
*�޸ļ�¼*
*�޸�ʱ��*             *�޸�����*
*
-------------------------------------------------------------------------
*��ע��
*     ��ͷ�ļ���ֻ�������������ɶ���
*     ��ͷ�ļ������иĶ�������ϸ˵��
*
-------------------------------------------------------------------------*/



#ifndef _DCSH_SAE_1_H
#define _DCSH_SAE_1_H


#ifdef DCSHCOREAPI_EXPORTS
#define DCSHCOREAPI_API __declspec(dllexport)
#else
#define DCSHCOREAPI_API __declspec(dllimport)
#endif
/*

#define DCSHCOREAPI_API 
*/
#ifdef  __cplusplus
extern "C" {
#endif

#define int8		char
#define int16		short
#define int32		long
#define uint8		unsigned char
#define uint16		unsigned short
#define uint32		unsigned int
#define uint		unsigned int 
#define BYTE		unsigned char
#define WORD		unsigned short
#define DWORD		unsigned long
#define UCHAR		unsigned char
#define UINT		unsigned int
#define LONG		long
#define ULONG		unsigned long
#define VOID		void
#define PVOID		void *

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

//��̬��֤�����ϵͳĬ��ֵ
#define SYSTEMPIN        "888888"		/* ϵͳĬ�ϵ�PIN*/	
#define TOTALSECONDS     0x47791200		/* 2008��1��1��00��00ʱ00�֣���ʧ����*/
#define DCPVTLEN         232			/* ˽�����ݳ���*/
#define SEEDLEN	         16				/* ���ӵĳ��ȣ�����ʹ��128λ������������Կ*/
#define EX_SEEDLEN		 4				/* ���ӵ���չ��֧��20�ֽ�*/
#define MAXSEEDLEN		 20				/* ϵͳ֧�ֵ�������ӳ��ȣ�����64��Ŀǰʹ��20*/
#define MAXOFFSETSECONDS 43200			/* (12*60*60)�룬 ƫ��������12Сʱ�������Ƿ���֤ͨ����Ӧ�����ý���*/
#define ONEDAYSECONDS	 86400			/* (24*60*60)�룬 һ������������ڻ�ö�̬���� */
#define TWOWEEKSECONDS   1209600		/* (14*24*60*60)�룬���2��δ��½���򴰿��Զ���չ1������󲻳����󴰿�*/
#define ONEWEEKSECONDS   604800			/* (7*24*60*60)�룬���1��δ��½��������Ҫʱ�䲹��*/
#define MAXRESYNCHTIMES	 1000			//�¼�����������ͬ�����Ѱ�Ҵ���
										//�¼�������������ͬ���Ƿ����ַ�ʽ��
										//1��������1��ʼ����MAXRESYNCHTIMES ��Ѱ��
										//2���ӵ�ǰ���һ����Ч��������MAXRESYNCHTIMES��Ѱ��
#define SM3HASHSTRINGLENGTH	64			//SM3�㷨�����Ƶ�������װ�ռ䣬��ʽ�ο���ʱ��ͬ��һ���Կ�����֤ϵͳ��׼0702��
#define DATETIMESTRINGLEN	13			//SM3�㷨������ת����12�ֽڣ���ĩβ��13�ֽ�Ϊ�������
#define THANDELTATIMES		3			//ǰһ��ʹ�õ�ʱ�������Ǵ˴�ʱ����3��������ʱ�䲹��
#define	GETTOKENCODESTRHEAD	"%010u:%03u:%03u:%010u:"	//"��ʼʱ��:���Ƽ��:��̬�����:��̬�������:��̬�����"


/*�����㷨*/
#define ALGORITHM_SHA1		0x01 /* ����20�ֽ�ժҪ */
#define ALGORITHM_RC6		0x02 /* ����ܣ��ٶȿ�*/
#define ALGORITHM_SM3		0x03 /* SM3������SHA1������΢���� */
#define ALGORITHM_AES		0x04 /* ����ܣ���׼�㷨*/
#define ALGORITHM_ATM		0x00 /* ֧��ATM����ʽ����սӦ�𣬴�ʱ��*/
#define ALGORITHM_ATM_MD5	0x05 /* ֧��ATM����ʽ����սӦ�𣬴�ʱ�䣬�û�����ʹ��MD5ժҪ*/
#define ALGORITHM_ATM_SHA1	0x06 /* ֧��ATM����ʽ����սӦ�𣬴�ʱ�䣬�û�����ʹ��SHA1ժҪ*/
#define ALGORITHM_HMAC		0x08 /* OTP������20�ֽ�ժҪ */
#define ALGORITHM_YQ		0x10 /* AES��ʹ���µĽ�ȡ��ʽ */
#define ALGORITHM_COS		0x20 /* HMAC��cos��sim����ʽ */
#define ALGORITHM_ASP		0x30 /* ׿��SIM�����ƣ�ʹ��HMAC */
#define ALGORITHM_UNI		0x07 /* ��ͨSIM�����ƣ�ʹ��HMAC�����ǽ�ȡ����һ�� */
#define ALGORITHM_TOTP		0x09 /* ��������ʹ�õ�ʱ����OTP��OTP��ʱ���ͬALGORITHM_HMAC����ս��׷��ʱ�� */
#define ALGORITHM_SSM3		0x11 /* SM3�㷨�����ܰ�չ��������㷨����Ȼ����ԭ�ȵĹ��� */

#define STRLENLOGID      48		/*LoginId�ĳ���*/
#define STRLENAME        24		/*UserName�ĳ���*/
#define GMTSTRLEN        24		/*ʱ���ַ��ĳ��� GMT 2008-05-14 04:09:22*/
#define URGENCYPWLEN	 20		/*�����������󳤶�*/

#define DCSH_MIN_PIN_LEN     4  /*Pin����С����*/
#define DCSH_MAX_PIN_LEN     8  /*Pin����󳤶�*/
#define DCSH_PIN_LEN         12 /*Pin�ĳ��ȿռ䣬����Ϊ4-8*/
#define DCSH_SN_LEN          12 /*���к���󳤶�*/
#define DCSH_PRN_LEN         12 /*tokencode��󳤶�*/
#define DCSH_RESPONSE_LEN	 16 /*�û���Ӧ�룬���ܰ���PIN*/
#define DCSH_CHALLENGE_LEN	 16 /*��ս�룬����8�ֽ�*/
#define UNLOCKCODELEN		 8  /*������ĳ���Ϊ8������*/	
#define HMACQUESTIONLEN		 8	/*��սӦ��HMAC�㷨��question����һ��Ϊ8*/	
#define HOSTBYTELEN			 12	/*ȡ12���ֽڣ�ʹ�����4/6λ*/
#define UNICOMMODLEN		 1000 /*ȡ�������3�����֣�������ǰ�˲��� */
#define UNICOMNUMBER		 3    /*ÿ��Shortȡģ���������Ϊ3��*/

#define OCRASUITE		"OCRA-1:HOTP-SHA1-6:QN08"   /* ��սӦ��Ĺ��� */
#define OCRASUITE_ICBC	"OCRA-1:HOTP-SHA1-6:QA64-T" /* ����������սӦ��ʱ�������ƵĹ��� */
#define SM3OCRASUITE	"OCRA-1:HOTP-SM3-8:QN08"    /* SM3�㷨����սӦ����� */

//��������
#define DCSH_TOKEN_TYPE_TIME	0 /*ʱ�����͵�����*/
#define DCSH_TOKEN_TYPE_EVENT	1 /*�¼����͵�����*/
#define DCSH_TOKEN_TYPE_MOBILE	2 /*�ֻ����͵�����, ����ר��*/

//���Ƶ�Ӧ������
#define DCSH_HARDWARE_TOKEN_DC		1 /*����������Ӳ������*/ //Ŀǰ���������ģ�ר����ָK5����
#define DCSH_HARDWARE_TOKEN_DC_EX	2 /*�������������ƺ� �������̵���չ*/ 
									  //��Ҫʹ������һ�ִ��ݲ�����ʽ
									  //֧�ֺ���Ԧǿ������ --AES
									  //֧��cos��SIM������ --COS
									  //֧�ָ���΢���ӵ�SM3���ƣ����ܳ�Ϊ��ҵ��׼ --SM3
									  //�����Լ�������
									  //׿������ͨ��SIM������

#define DCSH_HARDWARE_TOKEN_CHR		3 /*�첽���͵�Ӳ������*/ //��սӦ��RSA��һ��
#define DCSH_SOFTWARE_TOKEN_DC		4 /*�����������������*/ //��־λ���֣���ͬ��DCSH_HARDWARE_TOKEN_DC_EX
#define DCSH_SOFTWARE_TOKEN_PHONE	5 /*�ֻ�����          */ //��־λ���֣���ͬ��DCSH_HARDWARE_TOKEN_DC_EX
#define DCSH_SOFTWARE_TOKEN_CHR		6 /*�첽���͵��������*/ //�����ֻ��첽����
#define DCSH_SOFTWARE_TOKEN_SMS		7 /*�������͵��������*/ //��������
#define DCSH_HARDWARE_TOKEN_COVER	8 /*�ιο�����*/
#define DCSH_HARDWARE_TOKEN_ATM		9 /*�����첽���͵�����*/ //Ϊ����ATM�����û����뵽������
#define DCSH_SOFTWARE_TOKEN_ATM		10 /*�����첽�������*/ //Ϊ����ATM������ƣ��������ֻ������
#define DCSH_HARDWARE_TOKEN_SIM		11 /*׿������ͨ��SIM������Ҳ������ΪӦ�����ͣ�Ҳ������DC_EX*/
#define DCSH_HARDWARE_TOKEN_GETDC	12 /*K5 ����������֧�ֻ�ȡ��̬�����������Ͳ�������֤*/
#define DCSH_HARDWARE_TOKEN_GETEX	13 /*K8 ����������֧�ֻ�ȡ��̬�����������Ͳ�������֤*/


//��֤����
#define DCSH_PRN_TYPE						0 /*ֱ��ʹ��tokencode��֤*/
#define DCSH_PASSCODE_TYPE					1 /*��PIN��tokencode��֤ */
#define DCSH_CHALLENGE_RESPONSE_TYPE		2 /*��ս��Ӧ����֤��ʽ */
#define DCSH_PASSCODE_NEXT_TYPE				3 /*��Ҫ������һ��tokencode����֤��ʽ*/
#define DCSH_SMS_GETPRN_TYPE				4 /*����Ϣ��֤�����tokencode*/
#define DCSH_SMS_PRN_TYPE					5 /*����Ϣ��֤����֤tokencode*/
#define DCSH_MSCHAP2_PASSCODE_TYPE			6 /*ʹ��MS-Chap v2����֤��ʽ�����������tokencode����֤*/
#define DCSH_MSCHAP2_CHANGE_PW_TYPE			7 /*�޸����Ƶ�PIN��*/
#define DCSH_SIGN_PRN_TYPE					8 /*����ǩ���Ķ�̬��֤��ʽ*/
#define DCSH_GET_SIGN_PRN_TYPE				9 /*���ǩ����������ڶ�������ǩ����ʽ*/
#define DCSH_COVER_GET_HOSTCODE_TYPE		10 /*��ùιο��ĵ�ǰ������������*/
#define DCSH_COVER_HOST_PRN_TYPE			11 /*�ιο�����֤�����������ַ� ��̬���� PIN*/
#define DCSH_CHALLENGE_RESPONSE_ATM_TYPE	12 /*ATM�û��������սӦ����֤ģʽ���û�����֧��ժҪ�㷨 */

//��֤״̬����֤ģʽ
#define DCSH_LOGON_STATE_FIRST_LOGIN         1
#define DCSH_LOGON_STATE_NORMAL              2
#define DCSH_LOGON_STATE_NEXT_TOKENCODE      3
#define DCSH_LOGON_STATE_NEXT_TOKENCODE_2    4

#define DCSH_MAX_PINTIMES	3    /*���Pin����������*/
#define DCSH_MAX_THRESOLD	10   /*���tokencode�����������������nextģʽ*/
#define DCSH_MAX_REPEATED	5    /*�����طŴ��������������*/

#define ICBC_ACTIVECHECKCODELEN	12	/* �����������Ӽ�����ĳ���12��ǰ6�����6У�� */
#define ICBC_ACTIVECODELEN		6	/* ����������������������6 */
#define ICBC_ACTIVECODECALCLEN	8	/* ����������������������6��������ʱ����ʵ����8 */
#define ICBC_CHECKCODELEN		6	/* ����������������У���볤��6 */
#define ICBC_PINLOCKCODELEN		8	/* ��������PIN������������������볤�� */
#define ICBC_PINUNLOCKCODELEN	6	/* �������ƽ����볤�� */


#define		DCSHEMSUCCESS			0 //��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
#define     DCSHEMFIRST				1 //��һ��ʹ�����Ƶ�½���ɹ�����ͬ��EMSUCCESS
#define		DCSHEMBADARG			2 //�������󣬲�������Ϊ��ָ��
#define		DCSHEMBADREC			3 //���ش���PDATA�ڲ����������⣬��ǩ����
#define		DCSHEMPRNLEN			4 //Tokencode�ĳ��ȴ���
#define		DCSHEMTIMRNG			5 //passcodeʱ����󣬲�������������
#define		DCSHEMNOTASSIGNED		6 //���ƻ�û�з���
#define     DCSHEMDISABLE			7 //���ƴ��ڽ���״̬
#define		DCSHEMFOREVERDISABLE	8 //���Ʊ����ý��ã�������ʱ��ƫ�Ƴ���12Сʱ
#define		DCSHEMNOPIN				9 //û������pin��
#define		DCSHEMPINLEN			10 //�����PIN���ȴ���
#define		DCSHEMBADPIN			11 //��������PIN
#define		DCSHEMSTOLEN			12 //��ΪPIN����������Ｋ�ޣ�Ĭ��Ϊ��ʧ
#define		DCSHEMOUTWIND			13 //���κδ������Ҳ���tokencode��������Ҫ��ͬ��
#define		DCSHEMREPPRN			14 //�����طţ���ҪС�ģ�Ӧ��������¼�������ѹ���Ա
#define		DCSHEMREPDISABLE		15 //�طŵ�һ���Ĵ��������
#define		DCSHEMNEXTPRN			16 //����Next��ģʽ
#define		DCSHEMTHRESHOLDNXT		17 //����tokencode�Ĵ���������Ｋ�ޣ��Ժ����NEXTģʽ
#define		DCSHEMBADNEX			18 //��Nextģʽ���������tokencode
#define		DCSHEMNEXTDISABLE		19 //��Nextģʽ���������������Ｋ�ޣ�������
#define		DCSHEMSYN1OK			20 //����ͬ��ģʽ�£���һ��ƥ��tokencode�Ѿ��ҵ�
#define     DCSHEMFIRSTDISABLE      21 //��Firstģʽ�£��������ʧ�ܣ������
#define 	DCSHEMBADNEX2           22 //��Next2����������tokencode
#define 	DCSHEMBADINPUT          23 //�ļ��޷���
#define 	DCSHEMNOMEM             24 //��ȡBIN�ļ���û���㹻���ڴ�ռ�
#define 	DCSHEMBADINPFIL         25 //BIN���ļ����ݴ������ش���
#define		DCSHEMBADSIG            26 //BIN�ļ�ǩ���������ش���
#define 	DCSHEMEMPNAME           27 //�ļ����ƴ���
#define		DCSHEMKEYPAIRERR		28 //��Կ�Դ���ʧ��
#define		DCSHEMCERTERR			29 //��֤�鴴��ʧ��
#define		DCSHEMSEEDERR			30 //��������ʧ��
#define		DCSHEMREADPUBKEYERR		31 //��ȡ��Կʧ��
#define		DCSHEMPUBKEYENCERR		32 //��Կ����ʧ��
#define		DCSHEMREADSEEDERR		33 //���������ļ�ʧ��
#define		DCSHEMREADPRIKEYERR		34 //����˽Կʧ��
#define		DCSHEMPRIKEYDECERR		35 //��˽Կ����ʧ��
#define		DCSHEMREADPKCS10ERR		36 //��ȡPKCS#10�ļ�����
#define		DCSHEMTOKENTYPE			37 //���������Ӧ������
#define		DCSHEMREADROOTCERTERR	38 //��ȡ��֤��ʧ��
#define		DCSHEMDECROOTCERTERR	39 //������֤��ʧ��
#define		DCSHEMDECPKCS10ERR		40 //����PKCS#10ʧ��
#define		DCSHEMUSERCERTERR		41 //�����û�֤��ʧ��
#define		DCSHEMVERSIGDATAERR		42 //��֤ǩ���������ش���
#define		DCSHEMDECEVPDATAERR		43 //���������ŷ�������ش���
#define		DCSHEMREADUSERCERTERR	44 //��ȡ�û�֤��ʧ��
#define		DCSHEMDECUSERCERTERR	45 //�����û�֤��ʧ��
#define		DCSHEMEVPDATAERR		46 //���������ŷ�ʧ��
#define		DCSHEMSIGDATAERR		47 //��������ǩ��ʧ��
#define		DCSHEMREADSEEDFILEERR	48 //��ȡ�����ļ�ʧ��
#define		DCSHEMPKCS10ERR			49 //����PKCS#10ʧ��
#define		DCSHEMBADFILETYPE		50 //����������ļ�����
#define		DCSHEMRESPONSEERR		51 //��ս��֤��֤ʧ��
#define		DCSHEMBADTYPE			52 //�������������
#define		DCSHEMSNASSIGNED		53 //ָ�������к��Ѿ����䣬���Ѿ�ʹ��
#define		DCSHEMBADURGENCYPW		54 //�����������
#define		DCSHEMSMSBADPRN			55 //����Ϣ��֤������Ķ�̬����
#define		DCSHEMSMSTIMEOUT		56 //������ȷ�����Ѿ���ʱ
#define		DCSHEMBADGETURGENCYPW	57 //��ȡ��������ʧ��
#define		DCSHEMPKI128			58 //CryptoAPI�ڹ�Կ����ʱż���ᶪʧ1�ֽ�
#define		DCSHEMLICDEVSNERR		59 //��Ȩ����Ȩ����ʱ�豸���кŴ���
#define		DCSHEMLICDATEEND		60 //��Ȩ����Ȩ�ļ��趨����Ч�ڽ���
#define		DCSHEMLICEKEYERR		61 //��Ȩ��eKey�ڲ��İ�Ȩ��Ϣ��������
#define		DCSHEMLICUSBERROR		62 //��Ȩ��usb��eKey�����豸ԭ������
#define		DCSHEMMSCHAPV2ERROR		63 //ms chap v2 ��֤���ͻ����ύnt_response����
#define		DCSHEMMSCHAPV2ERRORPW	64 //ms chap v2 �޸Ŀ����������쳣
#define		DCSHEMDEVSNERR			65 //��Ȩ��eKey���кŲ�ƥ��
#define		DCSHEMDEVFILEERROR		66 //ekey�ڲ��ļ�������
#define		DCSHEMFORCESYNTYPE		67 //ǿ��ͬ�������ʹ���Ӧ����ֻ�����¼���
#define		DCSHEMNOTCOVERTYPE		68 //���ǹιο��������ͣ��������ɿ�Ƭ
#define		DCSHEMOVERCOVERNUM		69 //�������ɿ�Ƭ��������������������
#define		DCSHEMLASTCOVERNUM		70 //���һ����������������֤���������ڻ���
#define		DCSHEMCOVERNOTUSED		71 //��ǰ�ĸ�������û��������֤
#define		DCSHEMMUSTGETCOVER		72 //�ιο���֤�����Ȼ�������ַ�
#define		DCSHEMHOSTCODEERRO		73 //�����ַ�����
#define		DCSHEMCOVERCODEERR		74 //��������Ķ�̬�������
#define		DCSHEMERRORTODISABLE	75 //�����֤���󣬵������Ʊ�����
#define		DCSHEMUPDATECODEERR		76 //�ιο���������󣬼����һ���ιο������
#define		DCSHEMBASE64CODEERR		77 //base64��������
#define		DCSHEMURGPWDERR			78 //���ķ�����ʧ��
#define		DCSHEMMODIFYSNERR		79 //�޸����к�ʱ�����к��������
#define		DCSHEMPCIOPENERR		80 //��PCI SJK0817E���ܿ�ʧ��
#define		DCSHEMPCISESSIONERR		81 //�����Ựʧ�ܣ�����PCI�жϣ�Ҳ���ܴﵽ���Ự��
#define		DCSHEMPCISM3ERR			82 //SM3�㷨�ڲ�����
#define		DCSHEMNOSUPPORT			83 //��ϵͳ��֧�ָ��㷨��ĿǰSM3������windows
#define		DCSHEMFORCECOUNTERLESS	84 //�ͻ���ǿ��ͬ������˼�����ʱ��������С�ڷ������Ѿ�ʹ�õ�
#define		DCSHEMSHECADLLERR		85 //����sheca�Ŀ����ӿ�ʧ��
#define		DCSHEMSHECAKEYERR		86 //����sheca��usb-eKey�����쳣
#define		DCSHEMPCIOPERERR		87 //���ܿ��������ʧ��
#define		DCSHEMICBCVALUEERR		88 //���н��������������ݽ���ʧ��
#define		DCSHEMICBCHAVEXOR		89 //���е����������Ѿ����õ����µ���������
#define		DCSHEMSEEDLENERR		90 //���ӳ��ȴ��󣬿����ǹ��е�����������
#define		DCSHEMACTIVELENERR		91 //�����볤�ȴ���6���֣����㳤��Ϊ8����0x00,0x00
#define		DCSHEMCHECKLENERR		92 //У���볤�ȴ���6����
#define		DCSHEMACTIVCHECKERR		93 //����У�������12����
#define		DCSHEMBIGTIMELENERR		94 //�����ʱ�䣬��ͷ���򣬳��ȴ���
#define		DCSHEMCHANLENERR		95 //��ս���ַ������ȴ��󣬹��б���С��64
#define		DCSHENUNLOCKPINERR		96 //���ƽ�������ַ������ȴ���8����
#define		DCSHENOUTONEDAY			97 //��ȡ��̬���ʱ���ͣ�������24Сʱ
#define		DCSHENOUTMAXCOUNTER		98 //��ȡ��̬����¼��ͣ�������1000��


typedef struct CARDREC
{
	char ucSerialNumber[DCSH_SN_LEN];		/* Token���к��ַ���ʾ      */
	char ucUrgencyPassWord[URGENCYPWLEN];	/* ��������					*/
	char ucSeedNumber[32];					/* ����ת����26������		*/
} TOKENCARDRECORD, *PTTOKENCARDRECORD;


typedef struct DCSHPDATA {
    unsigned int ulSerialNumber;     /* Token���к�              */
    char ucSerialNumber[DCSH_SN_LEN];/* Token���к��ַ���ʾ      */
    unsigned int ulPRNLen;			 /* Tokencode�ĳ���          */
    unsigned int ulInterval;		 /* ʱ����������tokencode  */
	unsigned int ulBirth;			 /* �������ڣ��� 1/1/2008    */
    unsigned int ulDeath;			 /* ��Ч�ڣ��룬�� 1/1/2008  */
    char cTokenType;				 /* Token����                */	
    char cIsHardwareToken;			 /* �ǲ���Ӳ��               */
	char cAlgorithmSData;			 /* �㷨��˽�����ݵļ����㷨 */
	char cAlgorithmToken;			 /* �㷨��tokencode�Ĳ����㷨*/	
	
	int  iState;					 /* ״ֵ̬������ǳ���Ҫ���ֶ�*/
	char cEnabled;					 /* �Ƿ���ã���һ�ж�Ҫ��    */
    char cAssigned;					 /* �Ƿ���䣬��һ�ж�Ҫ��    */
    char cNextPRNMode;				 /* NextToeknģʽ             */
    char cPINDisabled;				 /* PIN�Ƿ�ʹ��               */
    
    unsigned int ulLastlogin;		 /* �����½ʱ��,���۳ɹ�ʧ�ܣ��룬�� 1/1/2008 */
    
	char ucUserLoginID[STRLENLOGID]; /* */
    char ucUserFirstName[STRLENAME]; /* */
    char ucUserLastName[STRLENAME];  /* */	
	
    int iMaxBadPins;                 /* Pin����tokencode��֤��ÿ�ε�½�ɹ�ǰ�����������������disable */
    int iNextTokencodeModeThreshold; /* tokenode����ļ���ֵ��������ֵ�������Next Tokenģʽ */
	int iMaxRepeateds;               /* ����طŹ�������ֵ�������ύ��ǰ��tokencode������ͬһʱ���ύtokencode�����Ϊ�طţ��ﵽ����ֵ����� */
	int iMaxBadNextTokencodes;		 /* ��Nextģʽ�£�������������� */
	
    int iPrecision;                  /* ���ȣ���ulLastSuccessLoginÿ��һ�����ۼӾ����� */
    
    int iLargeWindowSeconds;		 /* �� */
    int iMediumWindowSeconds;		 /* �� */
    int iSmallWindowSeconds;		 /* �� */

	char ucTokenTypeString[8];		 /* ���Ƶ��������� */
	int iSeedLen;					 /* ���ӳ��ȣ������0����Ĭ��Ϊ16 */
	char ucReserved[4];              /* �����ֶ� */
    unsigned char ucPrivateData[DCPVTLEN]; /* ��������-���� pin�� */ 
} TOKENRECORDDATA, *PTTOKENRECORDDATA; /* 424�ֽ�  Base64Ϊ568�ֽ�*/

typedef struct DCSHSAE_TOKENRECORD_INFO {
    unsigned int ulSerialNumber;     /* Token���к�              */
    char ucSerialNumber[DCSH_SN_LEN];/* Token���к��ַ���ʾ      */
    unsigned int ulPRNLen;			 /* Tokencode�ĳ���          */
    unsigned int ulInterval;		 /* ʱ����������tokencode  */
	unsigned int ulBirth;			 /* �������ڣ��� 1/1/2008    */
    unsigned int ulDeath;			 /* ��Ч�ڣ��룬�� 1/1/2008  */
    char cTokenType;				 /* Token����                */	
    char cIsHardwareToken;			 /* �ǲ���Ӳ��               */
	char cAlgorithmSData;			 /* �㷨��˽�����ݵļ����㷨 */
	char cAlgorithmToken;			 /* �㷨��tokencode�Ĳ����㷨*/	
	
	int  iState;					 /* ״ֵ̬������ǳ���Ҫ���ֶ�*/
	char cEnabled;					 /* �Ƿ���ã���һ�ж�Ҫ��    */
    char cAssigned;					 /* �Ƿ���䣬��һ�ж�Ҫ��    */
    char cNextPRNMode;				 /* NextToeknģʽ             */
    char cPINDisabled;				 /* PIN�Ƿ�ʹ��               */
    
    unsigned int ulLastlogin;		 /* �����½ʱ��,���۳ɹ�ʧ�ܣ��룬�� 1/1/2008 */
    
	char ucUserLoginID[STRLENLOGID]; /* */
    char ucUserFirstName[STRLENAME]; /* */
    char ucUserLastName[STRLENAME];  /* */	
	
    int iMaxBadPins;                 /* Pin����tokencode��֤��ÿ�ε�½�ɹ�ǰ�����������������disable */
    int iNextTokencodeModeThreshold; /* tokenode����ļ���ֵ��������ֵ�������Next Tokenģʽ */
	int iMaxRepeateds;               /* ����طŹ�������ֵ�������ύ��ǰ��tokencode������ͬһʱ���ύtokencode�����Ϊ�طţ��ﵽ����ֵ����� */
	int iMaxBadNextTokencodes;		 /* ��Nextģʽ�£�������������� */
	
    int iPrecision;                  /* ���ȣ���ulLastSuccessLoginÿ��һ�����ۼӾ����� */
    
    int iLargeWindowSeconds;		 /* �� */
    int iMediumWindowSeconds;		 /* �� */
    int iSmallWindowSeconds;		 /* �� */

	char ucTokenTypeString[8];		 /* ���Ƶ��������� */
	int iSeedLen;					 /* ���ӳ��ȣ������0����Ĭ��Ϊ16 */
	char ucReserved[4];              /* �����ֶ� */
} TOKENPUBLICINFO, *PTTOKENPUBLICINFO;     /* 192�ֽ�  */

typedef struct {
        LONG type;					/* PASSCODE_TYPE/PASSCODE_NEXT_TYPE */
		ULONG passc_time;			/* passcodetime */
        char prn[DCSH_PRN_LEN];		/* tokencode */
        char pin[DCSH_PIN_LEN];		/* pin�� */
        char nextPrn[DCSH_PRN_LEN];	/* PASSCODE_NEXT_TYPE�������һ��tokencode */
} DCSHPASSCODE;

typedef struct {
        LONG type;					/* PRN_TYPE */
        ULONG passc_time;			/* passcodetime */
        char prn[DCSH_PRN_LEN];		/* tokencode */
} DCSHPRN;

typedef struct {
        LONG type;						/* CHALLENGE_RESPONSE_TYPE */
        ULONG passc_time;				/* passcodetime */
        char challenge[DCSH_CHALLENGE_LEN];  /* ��ս�� */
        char response[DCSH_RESPONSE_LEN];    /* ��Ӧ�� */
} DCSHCHALLENGE_RESPONSE;

typedef struct {
        LONG type;						/* CHALLENGE_RESPONSE_ATM_TYPE */
        ULONG passc_time;				/* passcodetime */
		int  iPasswdAlg;				/* �û����������ժҪ�㷨��0��ʾֱ��ʹ��*/
        char challenge[DCSH_CHALLENGE_LEN];  /* ��ս�� */
        char response[DCSH_RESPONSE_LEN];    /* ��Ӧ�� */
} DCSHCHALLENGE_RESPONSE_ATM;

typedef struct {
        LONG type;					/* DCSH_SMS_GETPRN_TYPE/DCSH_SMS_PRN_TYPE */
        ULONG valid_time;			/* ��Чʱ�䣬�� */
        char prn[DCSH_PRN_LEN];		/* tokencode������/��֤ */
} DCSHSMSPRN;

/* RFC 2548     MSCHAPV2 Response Vendor type = 25                              */
/* RFC 2759     Section 8 Provides Pseudocode and details                       */
#define DCSH_CHALLENGE_LENGTH			16
#define DCSH_USERNAME_LENGTH			128
#define DCSH_RESPONSERECEIVED_LENGTH	24
#define DCSH_RESPONSE_LENGTH			42
#define DCSH_ENCRYPTED_NEWPW_LENGTH		516
#define DCSH_PASSWORD_LENGTH			256
#define DCSH_ENCRYPTED_OLDPWHASH_LENGTH 16
/* struct to be used with type set to DCSH_MSCHAP2_PASSCODE_TYPE                */
/* allow to pass required parameters for mschapv2 auth                          */
typedef struct {
        LONG type;
        ULONG passc_time;
        /* IN::RFC 2759 Section 8.1 */
        unsigned char authenticatorChallenge[DCSH_CHALLENGE_LENGTH];
        unsigned char peerChallenge[DCSH_CHALLENGE_LENGTH];

        /* IN::RFC 2759 Section 8.1 null terminated string of size less or equal to 128*/
        char userName[DCSH_USERNAME_LENGTH]; 
        
        /* IN::RFC 2759 Section 8.1 Result of*/
        /* GenerateNTResponse(authenticatorChallenge, peerChallenge, userName, Passcode) */
        /* note:: Passcode is used to generate this value */
        unsigned char responseReceived[DCSH_RESPONSERECEIVED_LENGTH];
        
        /* OUT::RFC 2759 Section 8.7 Output from*/
        /* GenerateAuthenticatorResponse(Passcode, responseRecieved, peerChallenge, authenticatorChallenge, userName)*/
        /* note::Passcode is used to generate this value */
        char authenticatorResponse[DCSH_RESPONSE_LENGTH];
} DCSHMSCHAP2_PASSCODE;

/* struct to be used with type set to DCSH_MSCHAP2_CHANGE_PW_TYPE               */
/* allow to pass required parameters for to change password with mschapv2 auth  */
typedef struct {
        LONG type;
        ULONG passc_time;

        /* IN::RFC 2759 Section 8.1 */
        unsigned char authenticatorChallenge[DCSH_CHALLENGE_LENGTH]; 
        unsigned char peerChallenge[DCSH_CHALLENGE_LENGTH];

        /* IN::RFC 2759 Section 8.1 null terminated string of size less or equal to 128*/
        char userName[DCSH_USERNAME_LENGTH];         
     
        /* IN::RFC 2759 Section 8.1 Result of */
        /* GenerateNTResponse(authChallenge, peerChallenge, userName, NewPin) */
        /* note:: NewPin is used to generate this value */
        unsigned char responseReceived[DCSH_RESPONSERECEIVED_LENGTH];

        /* OUT::RFC 2759 Section 8.7 Output from */
        /* GenerateAuthenticatorResponse(NewPin, responseRecieved, peerChallenge, authenticatorChallenge, userName)*/
        /* note:: NewPin is used to generate this value */
        unsigned char authenticatorResponse[DCSH_RESPONSE_LENGTH];

        /* IN::RFC 2759 Section 8.9 provides detail on how to calculate this value*/
        unsigned char encryptedNewPasword[DCSH_ENCRYPTED_NEWPW_LENGTH];

        /* IN::RFC 2759 Section 8.12 provides detail on how to calculate this value*/
        unsigned char encryptedOldPasswordHashed[DCSH_ENCRYPTED_OLDPWHASH_LENGTH];
} DCSHMSCHAP2_CHANGE_PW;

typedef struct
{
	LONG type;					/* DCSH_SIGN_PRN_TYPE / DCSH_GET_SIGN_PRN_TYPE */
    ULONG passc_time;			/* passcodetime */
	char key1[20];				/* ��һ���ؼ���Ϣ������6������  */
	char key2[20];				/* �ڶ����ؼ���Ϣ������6������  */
	char key3[20];				/* �������ؼ���Ϣ������6������  */
	char prn[DCSH_PRN_LEN];		/* DCSH_SIGN_PRN_TYPE Ϊ��֤����ǩ���Ķ�̬����*/
								/* DCSH_GET_SIGN_PRN_TYPE Ϊ���ǩ����̬����*/
	char pin[DCSH_PIN_LEN];		/* pin��	        */
}  DCSH_SIGN_PASSCODE; //����ǩ������֤����

typedef struct
{
	LONG type;						/*DCSH_COVER_GET_HOSTCODE_TYPE / DCSH_COVER_HOST_PRN_TYPE*/
	ULONG passc_time;				/*ϵͳʱ��*/
	char cHostCode[DCSH_PRN_LEN];	/*�����ַ�*/ //��֤ʱ�ύ�����ʱ����
	char cTokenCode[DCSH_PRN_LEN];	/*��̬����*/ //���ʱʵ��
	char pin[DCSH_PRN_LEN];			/*pin��*/	 //��֤ʱʹ��
	int  iCurrentCount;				/*��ǰ������*///���ʱʹ�÷���
} DCSH_COVER_PASSCODE; //�ιο���֤����

/*�������ͣ������������͵���֤*/
typedef union  {
    LONG type;
    DCSHPASSCODE passcode;
    DCSHCHALLENGE_RESPONSE challengeResponse;
    DCSHPRN prn;
	DCSHSMSPRN smsprn;
	DCSHMSCHAP2_PASSCODE mschap2;
	DCSHMSCHAP2_CHANGE_PW mschap2pw;
	DCSH_SIGN_PASSCODE sigprn;
	DCSH_COVER_PASSCODE coverprn;
	DCSHCHALLENGE_RESPONSE_ATM chapATM;
} DCSHCODE2CHECK, *PT_DCSHCODE2CHECK;



/******************************************************************************
**
**��������:	DCSHSAGetPasscodeTime
**
**��������:	����ϵͳʱ��ȡ�õ�ǰ������Ҫ��ʱ��UTC,�����2008/1/1������
**
**����˵��:
		void	[] - 
**
**��������: LONG 
**		�������ͣ���
**		
**		
**����˵��:	�ڽ���������֤ʱ��������ô˺����ṩ��ʱ��
**
**��ʷ:		����		����		��Ϊ
		2008-5-14		�		Created
******************************************************************************/
DCSHCOREAPI_API LONG DCSHSAGetPasscodeTime (void);

/******************************************************************************
**
**��������:	DCSHSAGetPasscodeTimeOTP
**
**��������:	��ȡϵͳ���룬�����1970/1/1
**
**����˵��:
		ULONG PasscodeTime	[in /   ] - DCSHSAGetPasscodeTime ��õ�ʱ��
**
**��������:  LONG 
**		�������ͣ���
**		
**		
**����˵��:	�ڽ���OTP��֤ʱ�����øýӿڻ��ʱ�䣺��
**
**��ʷ:		����		����		��Ϊ
		2011-1-7		�		Created
******************************************************************************/
DCSHCOREAPI_API LONG DCSHSAGetPasscodeTimeOTP (ULONG PasscodeTime);

/******************************************************************************
**
**��������:	DCSHSAFmtTime
**
**��������:	����֤����ʹ�õ�ʱ��ת��Ϊ�ַ��� "GMT 2008-05-14 04:09:22"
**
**����˵��:
		LONG tod		[in /    ] - ����ʱ�䣬��
		char *outstr	[in / out] - ����ַ������ⲿ����ռ䣬����Ϊ GMTSTRLEN
**
**��������: int 
**		EMSUCCESS �ɹ�ת��
**		EMBADARG  �������󣬿���outstrΪ��ָ��
**		
**����˵��:	ʱ��ΪGMTʱ�䣬������UTCʱ�䣬ע��ʱ��
**
**��ʷ:		����		����		��Ϊ
		2008-5-14		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAFmtTime (LONG tod, char *outstr);


/******************************************************************************
**
**��������:	DCSHSASetUserLogin
**
**��������:	�������Ƹ��û�,���Ƶĵ�һ������Ӧ���Ƿ���
**			�����û�ʱ����������PIN�룬�������������״̬λ�ͼ��������ı�
**			���Է�������
**
**����˵��:
		PTTOKENDATA pdata	[in / out] - �������ݰ��������޸�
		char *name			[in /    ] - �û�����
**
**��������: int 
**		EMSUCCESS �ɹ�
**		EMBADARG  �������󣬿���Ϊ��ָ�룬Ҳ����name�ĳ��ȴ���STRLENLOGID
**		EMBADREC  PDATA�ڲ����������⣬��ǩ����
**		EMFOREVERDISABLE ���ý���
**
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-5-14		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetUserLogin(PTTOKENRECORDDATA pdata ,char *name);


/******************************************************************************
**
**��������:	DCSHSASetTokenCodeLength
**
**��������:	�������ö�̬����ĳ��ȣ���󲻳���DCSH_PRN_LEN����Сû������
**			��ΪRFCȡLong�ͣ����ֵ��4�ֽڱ�ʾ������ʵ�ʳ��Ȳ����ܳ���10������
**
**����˵��:
		PTTOKENRECORDDATA pdata	[in / out] - pdata
		int iLen				[in /    ] - �µĳ��ȣ�Ӧ�ò�����DCSH_PRN_LEN
**
**��������:  int 
**		EMSUCCESS �ɹ�
**		EMBADARG  �������󣬿���Ϊ��ָ�룬Ҳ����iLen����DCSH_PRN_LEN
**		EMBADREC  PDATA�ڲ����������⣬��ǩ����
**		EMFOREVERDISABLE ���ý���	
**		
**		
**����˵��:	������ 4 - 8֮��
**
**��ʷ:		����		����		��Ϊ
		2009-8-18		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetTokenCodeLength(PTTOKENRECORDDATA pdata , int iLen);


/******************************************************************************
**
**��������:	DCSHSAEnableToken
**
**��������:	���Ѿ�������������ã�ͬʱ�����������Nextģʽ�����������Ʒ���֮���������
**			���ò������������״̬λ�ͼ����������ǲ�����طź������֤ʱ��
**
**����˵��:
		PTTOKENDATA pdata	[in / out] - �������ݰ�
		char *pin			[in /    ] - pin�������NULL���߳���Ϊ0����ǿ������SYSTEMPIN
**
**��������: int 
**		EMSUCCESS ���ÿ��óɹ�
**		EMBADARG  ��������pdata����Ϊ��ָ�룬Ҳ����pin���ȴ���MAX_PIN_LENС��MIN_PIN_LEN
**		EMBADREC  PDATA�ڲ����������⣬��ǩ����
**		EMNOTASSIGNED ��û�з����û�
**		EMPINLEN  pin�ĳ��������⣬Ӧ�� 4<=pinlen<=8
**		EMFOREVERDISABLE ���ý���
**
**����˵��:	���øú���ǰӦ���ȵ���DCSHSASetUserLogin
**
**��ʷ:		����		����		��Ϊ
		2008-5-14		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAEnableToken (PTTOKENRECORDDATA pdata, char *pin);

/******************************************************************************
**
**��������:	DCSHSADisableToken
**
**��������:	����һ������
**
**����˵��:
		PTTOKENDATA pdata	[in / out] - �������ݰ�
**
**��������: int 
**		EMSUCCESS ���ý��óɹ�
**		EMBADARG  ��������pdata����Ϊ��ָ��
**		EMBADREC  PDATA�ڲ����������⣬��ǩ����
**		EMDISABLE �Ѿ�����Ϊ������
**		EMFOREVERDISABLE ���ý���
**		
**		
**����˵��:	�κ�ʱ���������Ӧ�õ��øú����������ú����޸�
**
**��ʷ:		����		����		��Ϊ
		2008-5-14		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSADisableToken (PTTOKENRECORDDATA pdata);

/******************************************************************************
**
**��������:	DCSHSASetPinMaxErrors
**
**��������:	���û����PIN�����������������ʱ������״̬λ
**
**����˵��:
		PTTOKENDATA pdata	[in / out] - �������ݰ�
		int		iPinErrs	[in /    ] - ���������
**
**��������: int 
**		EMSUCCESS ����pin�ɹ�
**		EMBADARG  ��������pdata����Ϊ��ָ��
**		
**����˵��:	�ڽ���֮ǰ�޸����ֵ�����ã�������ú�Ҳ�����޸ģ��������ú�������
**
**��ʷ:		����		����		��Ϊ
		2008-5-14		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetPinMaxErrors (PTTOKENRECORDDATA pdata, int iPinErrs);


/******************************************************************************
**
**��������:	DCSHSASetPin
**
**��������:	��һ���ѷ�����õ���������PIN��ͬʱ������м�������Nextģʽ��������طź�ʱ��
**			ע�⣬Ҳ���޸�״̬λ�������õĽ���Ȼ����
**
**����˵��:
		PTTOKENDATA pdata	[in / out] - �������ݰ�
		char *pin			[in /    ] - pin�������NULL���߳���Ϊ0����ǿ������SYSTEMPIN
**
**��������: int 
**		EMSUCCESS ����pin�ɹ�
**		EMBADARG  ��������pdata����Ϊ��ָ��
**		EMBADREC  PDATA�ڲ����������⣬��ǩ����
**		EMDISABLE Ϊ����״̬
**		EMPINLEN  pin�ĳ��������⣬Ӧ�� 4<=pinlen<=8
**		EMFOREVERDISABLE ���ý���
**		
**����˵��:	�ڳɹ���½��Ӧ�ṩ�û��޸�PIN�ĵط�
**
**��ʷ:		����		����		��Ϊ
		2008-5-14		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetPin (PTTOKENRECORDDATA pdata, char *pin);


/******************************************************************************
**
**��������:	DCSHSASetPinDisabledFlag
**
**��������:	�����Ƿ�ʹ��PIN�ı�־λ��1 ���� 0 ����
**			δ���õ����Ʋ�������
**
**����˵��:
		PTTOKENDATA pdata	[in / out] - �������ݰ�
		int b				[in /    ] - 1 ����PIN��֤��0 ʹ��PIN��֤ 
**
**��������: int 
**		EMSUCCESS ����Flag�ɹ�
**		EMBADARG  ��������pdata����Ϊ��ָ��
**		EMBADREC  PDATA�ڲ����������⣬��ǩ����
**		EMDISABLE Ϊ����״̬����������
**		EMFOREVERDISABLE ���ý���
**		
**		
**����˵��:	����ʹ��PIN��֤
**
**��ʷ:		����		����		��Ϊ
		2008-5-14		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetPinDisabledFlag (PTTOKENRECORDDATA pdata,int b);


/******************************************************************************
**
**��������:	DCSHSASetNextTokencodeModeThreshold
**
**��������:	����tokencode�������ֵ���ﵽ������������nextģʽ
**			δ���õ����Ʋ�������
**
**����˵��:
		PTTOKENDATA pdata	[in / out] - �������ݰ�
		int index			[in /    ] - ���tokencode������ֵ
**
**��������: int 
**		EMSUCCESS ����Threshol�ɹ�
**		EMBADARG  ��������pdata����Ϊ��ָ�룬����index����(1-10֮��)
**		EMDISABLE Ϊ����״̬����������
**		
**		
**����˵��:	���ڷſ������10�����ƣ������������
**
**��ʷ:		����		����		��Ϊ
		2008-5-14		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetNextTokencodeModeThreshold (PTTOKENRECORDDATA pdata, int index);


/******************************************************************************
**
**��������:	DCSHSAGetTokenInfo
**
**��������:	��ȡ���Ƶ���ϸ��Ϣ
**
**����˵��:
		PTTOKENDATA pdata						[in /    ] - �������ݰ� 
		PTTOKENSECRETDATA tokenInfo	            [in / out] - ��ϸ��Ϣ���������ռ�
**
**��������: int 
**		EMSUCCESS ����Threshol�ɹ�
**		EMBADARG  ��������pdata����Ϊ��ָ��
**		EMBADREC  PDATA�ڲ����������⣬��ǩ����
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-5-14		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetTokenInfo (PTTOKENRECORDDATA pdata, PTTOKENPUBLICINFO tokenInfo);


/******************************************************************************
**
**��������:	DCSHSAUpdateTokenSeed
**
**��������:	��֤�����ڲ��������ӣ�������pdata�ڵ�ԭ�����ӣ�ע�⣬�⽫���������ļ�ʧЧ
			�������Ӻ�����ƽ���Ϊȫ�µ����ƣ���Firtģʽ
**
**����˵��:
		PTTOKENRECORDDATA pdata	[in / out] - pdata���ݽṹ
		int iBitLen		        [in /    ] - ���ӵ�λ����128����160
		char *cSeed				[in / out] - �����ַ�����base64�ַ�����ע��"==",��ʵ����ΪcSeedLen
		int *cSeedLen			[in / out] - base64���ӵ��ַ�������
**
**��������:  int 
**		EMSUCCESS �������ӳɹ�
**		EMBADARG  ��������pdata����Ϊ��ָ��		
**		
**		
**����˵��:	�����������������ϸ�Ĺ����漰���㷨�ͼ��ܿ���
			ͬʱpdata�ṹ�����к�/���������й��������ڷ�α��
			�������ӽ��ǵ��ϸ�Ĺ���ͷ�αУ��ʧЧ��
			��ʹ������Ҫע�⣬һ�����º󣬽����������ļ�BINʧЧ��
**
**��ʷ:		����		����		��Ϊ
		2010-2-4		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAUpdateTokenSeed (PTTOKENRECORDDATA pdata, int iBitLen, char *cSeed, int *cSeedLen);


/******************************************************************************
**
**��������:	DCSHSACheckPasscode
**
**��������:	��̬������֤
**
**����˵��:
		LONG passc_time		[in /    ] - DCSHSAGetPasscodeTimeȡ��ʱ��
		PTTOKENDATA pdata	[in / out] - �������ݰ�
		char *prn			[in /    ] - tokencode
		char *pin			[in /    ] - pin
**
**��������: int 
**		EMSUCCESS			��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMBADREC			���ش���PDATA�ڲ����������⣬��ǩ����
**      EMFIRST				��һ��ʹ�����Ƶ�½���ɹ�����ͬ��EMSUCCESS

**		EMPRNLEN			Tokencode�ĳ��ȴ���
**		EMTIMRNG			passcodeʱ����󣬲�������������
**		EMNOTASSIGNED		���ƻ�û�з���
**      EMDISABLE			���ƴ��ڽ���״̬
**		EMFOREVERDISABLE	���Ʊ����ý��ã�������ʱ��ƫ�Ƴ���12Сʱ
**		EMNOPIN				û������pin��
**		EMPINLEN			�����PIN���ȴ���
**		EMBADPIN			��������PIN
**		EMSTOLEN			��ΪPIN����������Ｋ�ޣ�Ĭ��Ϊ��ʧ
**		EMOUTWIND			���κδ������Ҳ���tokencode��������Ҫ��ͬ��
**		EMREPPRN			�����طţ���ҪС�ģ�Ӧ��������¼�������ѹ���Ա
**		EMREPDISABLE		�طŵ�һ���Ĵ��������
**		EMNEXTPRN			����Next��ģʽ
**		EMTHRESHOLDNXT		����tokencode�Ĵ���������Ｋ�ޣ��Ժ����NEXTģʽ
**		EMBADNEX			��Nextģʽ���������tokencode
**		EMNEXTDISABLE		��Nextģʽ���������������Ｋ�ޣ�������
**		EMBADTYPE			�������������
**		
**		
**����˵��:	����Ҫ����֤�ӿڣ��ر���Ҫע�ⷵ��ֵ
**
**��ʷ:		����		����		��Ϊ
		2008-5-15		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckPasscode (ULONG passc_time, PTTOKENRECORDDATA pdata, char *prn, char *pin);


/******************************************************************************
**
**��������:	DCSHSACheckATMTokenCode
**
**��������:	ATM���Ƶ���֤�ӿڣ���Ҫע���û������ת��
**
**����˵��:
		ULONG passc_time						[in /    ] - ʱ��
		PTTOKENRECORDDATA pdata					[in / out] - pdata
		int iPasswdAlg							[in /    ] - ���û�������㷨
		char *atmpassword						[in /    ] - �û����Ҳ������ժҪ����
		char *prn								[in /    ] - ��̬����
**
**��������:  int 
**		EMSUCCESS			��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMBADREC			���ش���PDATA�ڲ����������⣬��ǩ����
**      EMFIRST				��һ��ʹ�����Ƶ�½���ɹ�����ͬ��EMSUCCESS

**		EMPRNLEN			Tokencode�ĳ��ȴ���
**		EMTIMRNG			passcodeʱ����󣬲�������������
**		EMNOTASSIGNED		���ƻ�û�з���
**      EMDISABLE			���ƴ��ڽ���״̬
**		EMFOREVERDISABLE	���Ʊ����ý��ã�������ʱ��ƫ�Ƴ���12Сʱ
**		EMNOPIN				û������pin��
**		EMPINLEN			�����PIN���ȴ���
**		EMBADPIN			��������PIN
**		EMSTOLEN			��ΪPIN����������Ｋ�ޣ�Ĭ��Ϊ��ʧ
**		EMOUTWIND			���κδ������Ҳ���tokencode��������Ҫ��ͬ��
**		EMREPPRN			�����طţ���ҪС�ģ�Ӧ��������¼�������ѹ���Ա
**		EMREPDISABLE		�طŵ�һ���Ĵ��������
**		EMNEXTPRN			����Next��ģʽ
**		EMTHRESHOLDNXT		����tokencode�Ĵ���������Ｋ�ޣ��Ժ����NEXTģʽ
**		EMBADNEX			��Nextģʽ���������tokencode
**		EMNEXTDISABLE		��Nextģʽ���������������Ｋ�ޣ�������
**		EMBADTYPE			�������������
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-12-7		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckATMTokenCode (ULONG passc_time, PTTOKENRECORDDATA pdata, int iPasswdAlg, char *atmpassword /*�û�������Ϊchallenge*/, char *prn);


/******************************************************************************
**
**��������:	DCSHSACheckSIGPasscode
**
**��������:	ǩ����̬������֤����������ؼ���Ϣ������Ϊ����
**
**����˵��:
		ULONG passc_time		[in /    ] - ��ǰϵͳʱ��
		PTTOKENRECORDDATA pdata	[in / out] - pdata
		char *key1				[in /    ] - �ؼ���Ϣ1������6������
		char *key2				[in /    ] - �ؼ���Ϣ2������6������
		char *key3				[in /    ] - �ؼ���Ϣ3������6������
		char *prn				[in /    ] - ��̬����
		char *pin				[in /    ] - ����PIN��
**
**
**��������: int 
**		EMSUCCESS			��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMBADREC			���ش���PDATA�ڲ����������⣬��ǩ����
**      EMFIRST				��һ��ʹ�����Ƶ�½���ɹ�����ͬ��EMSUCCESS

**		EMPRNLEN			Tokencode�ĳ��ȴ���
**		EMTIMRNG			passcodeʱ����󣬲�������������
**		EMNOTASSIGNED		���ƻ�û�з���
**      EMDISABLE			���ƴ��ڽ���״̬
**		EMFOREVERDISABLE	���Ʊ����ý��ã�������ʱ��ƫ�Ƴ���12Сʱ
**		EMNOPIN				û������pin��
**		EMPINLEN			�����PIN���ȴ���
**		EMBADPIN			��������PIN
**		EMSTOLEN			��ΪPIN����������Ｋ�ޣ�Ĭ��Ϊ��ʧ
**		EMOUTWIND			���κδ������Ҳ���tokencode��������Ҫ��ͬ��
**		EMREPPRN			�����طţ���ҪС�ģ�Ӧ��������¼�������ѹ���Ա
**		EMREPDISABLE		�طŵ�һ���Ĵ��������
**		EMNEXTPRN			����Next��ģʽ
**		EMTHRESHOLDNXT		����tokencode�Ĵ���������Ｋ�ޣ��Ժ����NEXTģʽ
**		EMBADNEX			��Nextģʽ���������tokencode
**		EMNEXTDISABLE		��Nextģʽ���������������Ｋ�ޣ�������
**		EMBADTYPE			�������������
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-2-12		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckSIGPasscode (ULONG passc_time, PTTOKENRECORDDATA pdata, char *key1, char *key2, char *key3, char *prn, char *pin);

/******************************************************************************
**
**��������:	DCSHSAGetSIGPasscode
**
**��������:	��ȡǩ����̬����ڲ�ʹ��
**
**����˵��:
		ULONG passc_time		[in /    ] - ϵͳʱ��
		PTTOKENRECORDDATA pdata	[in /    ] - pdata
		char *key1				[in /    ] - �ؼ���Ϣ1
		char *key2				[in /    ] - �ؼ���Ϣ2
		char *key3				[in /    ] - �ؼ���Ϣ3
		char *prn				[in / out] - ��̬����ⲿ����ռ䣬����ʼ��
**
**��������:  int 
**		EMSUCCESS			��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMBADREC			���ش���PDATA�ڲ����������⣬��ǩ����
**		EMPRNLEN			Tokencode�ĳ��ȴ���
**		EMTIMRNG			passcodeʱ����󣬲�������������
**		EMNOTASSIGNED		���ƻ�û�з���
**      EMDISABLE			���ƴ��ڽ���״̬
**		EMFOREVERDISABLE	���Ʊ����ý��ã�������ʱ��ƫ�Ƴ���12Сʱ
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-2-12		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetSIGPasscode (ULONG passc_time, PTTOKENRECORDDATA pdata, char *key1, char *key2, char *key3, char *prn);

/******************************************************************************
**
**��������:	DCSHSACheckUrgencyPassWord
**
**��������:	����������������е�״̬λ��⣬���ִ�Сд
**			û�з�������Ʋ�����ʹ�ý�������
**
**����˵��:
		PTTOKENDATA pdata		[in / ] - PDATA
		char *ucUrgencyPassWord	[in / ] - �������16�ַ�
**
**��������:  int 
**		EMBADARG		��������
**		EMBADURGENCYPW ����������֤ʧ��
**		EMSUCCESS	����������֤�ɹ�
**
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-7-7		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckUrgencyPassWord (PTTOKENRECORDDATA pdata, char *ucUrgencyPassWord);


/******************************************************************************
**
**��������:	DCSHSAGetUrgencyPassWord
**
**��������:	��ȡ��������������е�״̬λ���
**
**����˵��:
		PTTOKENDATA pdata		[in /    ] - PDATA
		char *ucUrgencyPassWord	[in / out] - �������16�ַ����ⲿ����ռ�
**
**��������:  int 
**		EMBADARG		��������
**		EMBADGETURGENCYPW ����������֤ʧ��
**		EMSUCCESS		��ȡ�ɹ�
**
**����˵��:	�Ѿ��޸ģ���δ����������Ҳ����õ���������
**
**��ʷ:		����		����		��Ϊ
		2008-7-7		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetUrgencyPassWord (PTTOKENRECORDDATA pdata, char *ucUrgencyPassWord);


/******************************************************************************
**
**��������:	DCSHSASetUrgencyPassWord
**
**��������:	�����µķ����룬�������ڲ����ã����óɹ��󣬷����µķ�����
**
**����˵��:
		PTTOKENRECORDDATA pdata		[in / out] - pdata
		char *ucNewUrgencyPassWord	[in / out] - ����µķ����룬16���ֽڣ������ⲿ����20�ֽڵĿռ�
**
**��������:  int 
**		DCSHEMURGPWDERR	���ķ�����ʧ��
**		DCSHEMSUCCESS	���ķ�����ɹ�
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-10-27		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetUrgencyPassWord (PTTOKENRECORDDATA pdata, char *ucNewUrgencyPassWord);


/******************************************************************************
**
**��������:	DCSHSACheckTokenvalue
**
**��������:	�൱��FirstLogin,���ǲ��޸��κ����ݣ�Ҳ���жϱ�־λ
**
**����˵��:
		LONG passc_time		[in /    ] - ʱ��,DCSHSAGetPasscodeTimeȡ��ʱ��
		PTTOKENDATA pdata	[in /    ] - �������ݰ�
		char *prn			[in /    ] - tokencode
**
**��������: int 
**		EMSUCCESS			���ͨ��
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMBADREC			���ش���PDATA�ڲ����������⣬��ǩ����
**	    EMPRNLEN			Tokencode�ĳ��ȴ���
**		EMTIMRNG			passcodeʱ����󣬲�������������
**		EMOUTWIND			���κδ������Ҳ���tokencode��������Ҫ��ͬ��
**		EMBADTYPE			�������������		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-5-16		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckTokenvalue (ULONG passc_time, PTTOKENRECORDDATA pdata, char *prn);


/******************************************************************************
**
**��������:	DCSHSAResynchTokenEx
**
**��������:	����ͬ�����ǳ���ļ�������12Сʱ��Ѱ��
**
**����˵��:
		LONG passc_time		[in /    ] - ʱ��,DCSHSAGetPasscodeTimeȡ��ʱ��
		PTTOKENDATA pdata	[in / out] - �������ݰ�
		char *prn			[in /    ] - tokencode
**
**��������: int 
**		EMSUCCESS			����ͬ��
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMBADREC			���ش���PDATA�ڲ����������⣬��ǩ����
**	    EMPRNLEN			Tokencode�ĳ��ȴ���
**		EMTIMRNG			passcodeʱ����󣬲�������������
**		EMOUTWIND			���κδ������Ҳ���tokencode��������Ҫ��ͬ��
**		EMSYN1OK			����ͬ��ģʽ�£���һ��ƥ��tokencode�Ѿ��ҵ�
**		EMBADTYPE			�������������
**				
**����˵��:	����ͬ���ԡ�����ʧЧ��������
**
**��ʷ:		����		����		��Ϊ
		2008-5-16		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAResynchTokenEx (ULONG passc_time, PTTOKENRECORDDATA pdata, char *prn);


/******************************************************************************
**
**��������:	DCSHSAInitBINRead
**
**��������:	��BIN�ļ������и�ʽ�ֽ⣬���ؿռ���ܼ�¼��
**
**����˵��:
		const char *InputFile	[in /    ] - �ļ�����
		int **inpfil			[in / out] - ���ܵĿռ䣬ʹ��DCSHSAExitBINRead�ͷ�
		int *iRecoeds           [in / out] - �ܹ��ļ�¼��
		char *cType				[in / out] - �����ļ�������
**
**��������: int 
**		EMSUCCESS       �ɹ�
**		EMEMPNAME       �ļ����ƴ���
**		EMBADINPUT      �ļ��޷���
**		EMNOMEM         û���㹻���ڴ�ռ�
**		EMBADINPFIL     BIN���ļ����ݴ������ش���
**		EMBADSIG        �ļ�ǩ���������ش���
**		EMBADFILETYPE	����������ļ�����
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-5-16		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAInitBINRead (const char *InputFile, void **inpfil, unsigned int *iRecoeds, char *cType);


/******************************************************************************
**
**��������:	DCSHSAReadBINRec
**
**��������:	�ӽ��ܵ������ж�ȡ��¼
**
**����˵��:
		char *userPriKeyPath[in /    ] - �û�˽Ҫ�ļ�
		void *inpfil		[in /    ] - ���ܵ����ݿ�
		PTTOKENDATA pdata	[in / out] - ��װ��PDATA
		int i               [in /    ] - ��¼��������,��0��ʼ
		char cType			[in / out] - �����ļ�������
**
**��������: int 
**		EMSUCCESS       �رճɹ�
**      EMBADARG        ����Ĳ����������ǿ�ָ��
**		EMBADFILETYPE   �����ļ������ʹ���
**
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-5-16		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAReadBINRec (char *userPriKeyPath, void *inpfil, PTTOKENRECORDDATA pdata, unsigned int i, char cType);

/******************************************************************************
**
**��������:	DCSHSAExitBINRead
**
**��������:	���BIN�ļ����ܵ��ڴ�ռ�
**
**����˵��:
		void *fc	[in /    ] - �ռ��ַ 
**
**��������: int 
**		EMSUCCESS �رճɹ�
**		EMBADARG  �������󣬿����ǿ�ָ��
**		
**		
**����˵��:	�����ͷŽӿ��ڲ����ڴ�ռ䣬�����ط�Ҳ����ʹ��
**
**��ʷ:		����		����		��Ϊ
		2008-5-16		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAExitBINRead (void *fc);


/******************************************************************************
**
**��������:	DCSHMakeTokenRecord -- ������
**
**��������:	�ֹ�����һ��PDATA,SDATA
**
**����˵��:
		PTTOKENDATA pdata	[in / out] - ��ʼ��������
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-5-17		�		Created
******************************************************************************/
DCSHCOREAPI_API void DCSHSAMakeTokenRecord(PTTOKENRECORDDATA pdata);


/******************************************************************************
**
**��������:	DCSHMakeTokenRecordEvent -- ������
**
**��������:	�ֹ�����һ��PDATA,SDATA
**
**����˵��:
		PTTOKENDATA pdata	[in / out] - ��ʼ��������
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-5-17		�		Created
******************************************************************************/
DCSHCOREAPI_API void DCSHSAMakeTokenRecordEvent(PTTOKENRECORDDATA pdata);


/******************************************************************************
**
**��������:	DCSHMakeTokenRecordMobile -- ������
**
**��������:	����һ���ֻ����ƣ���ָ����ר�õ�
**
**����˵��:
		PTTOKENDATA pdata	[in / out] - ��ʼ��������
**
**��������:  void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API void DCSHSAMakeTokenRecordMobile(PTTOKENRECORDDATA pdata);


/******************************************************************************
**
**��������:	DCSHSASplitBINFile
**
**��������:	�ָ������ļ���֧�ּ��ܸ�ʽ��Ҳ֧�������ŷ��ʽ
**
**����˵��:
		char *pSeedFile				[in /    ] - �����ļ�·��
		unsigned int iNumber		[in /    ] - �ָ���ٸ�����
**
**��������:  int 
**		EMREADSEEDFILEERR	��ȡ�����ļ�ʧ��
**		EMBADFILETYPE		�����ļ����ʹ���
**		EMSUCCESS			�ɹ�
**		
**����˵��:	Դ�ļ�������������ֵ������ļ�
**
**��ʷ:		����		����		��Ϊ
		2008-6-12		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASplitBINFile (char *pSeedFile, unsigned int iNumber);



/******************************************************************************
**
**��������:	DCSHSAGenerateUrgencyPassWord (16�ֽڵĽ�������)
**
**��������:	���ɸ��������������Ȼ��Ը����������Base64���룬��ȡ16�ֽڣ���Ϊ��������
**
**����˵��:
		char *ucUrgencyPassWord	[in / out] - �ⲿ����ռ䣬���ؽ��������ַ�������20���ֽڿռ�
**
**��������:  int 
**			DCSHEMBADARG	��������
**			DCSHEMSEEDERR	BBS���������ʱ���󣬿����³���
**			DCSHEMSUCCESS	������������ɹ�
**
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-7-7		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGenerateUrgencyPassWord(char *ucUrgencyPassWord);


/******************************************************************************
**
**��������:	DCSHSAGetChallenge
**
**��������:	����һ��ָ�����ȵ��������Ϊ��ս��
**
**����˵��:
		unsigned char *randomData	[in / out] - ���������ս�룬�ⲿ����ռ�
		int iNumBytes				[in /    ] - Ҫ������������,�����8
**
**��������: int 
**		EMBADARG		��������
**		EMSEEDERR		�������������
**		EMSUCCESS		�ɹ�
**		
**����˵��:	�����˷���ȫ0���жϣ����������0����������³���
**
**��ʷ:		����		����		��Ϊ
		2008-6-23		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetChallenge(char *randomData, int iNumBytes);


/******************************************************************************
**
**��������:	DCSHSACheckCredential�����ݸ������Ƶ�ͳһ��֤�ӿڣ���CheckPasscode������
**
**��������:	���ݵ�һ��tokencode��֤
			����PIN��+tokencode��֤
			����PIN��+tokencode��֤�������ڳɹ����ȡ��һ����ȷ��tokencode����
			������ս��Ӧģʽ��֤
			����SIM������
			����COS���������̵�����
			����ATM������֤
			���ݹιҿ�������֤
			����MS-Chap V2����֤��ʽ
			���ݶ�������
			����ǩ����֤
**
**����˵��:
		PTTOKENDATA pdata			[in / out] - �������ݽṹ
		PT_CODE2CHECK ptCode2Check	[in / out] - ���ݸ���������װ�����ݽṹ
**
**��������: int 
**		EMSUCCESS			��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMBADREC			���ش���PDATA�ڲ����������⣬��ǩ����
**      EMFIRST				��һ��ʹ�����Ƶ�½���ɹ�����ͬ��EMSUCCESS

**		EMPRNLEN			Tokencode�ĳ��ȴ���
**		EMTIMRNG			passcodeʱ����󣬲�������������
**		EMNOTASSIGNED		���ƻ�û�з���
**      EMDISABLE			���ƴ��ڽ���״̬
**		EMFOREVERDISABLE	���Ʊ����ý��ã�������ʱ��ƫ�Ƴ���12Сʱ
**		EMNOPIN				û������pin��
**		EMPINLEN			�����PIN���ȴ���
**		EMBADPIN			��������PIN
**		EMSTOLEN			��ΪPIN����������Ｋ�ޣ�Ĭ��Ϊ��ʧ
**		EMOUTWIND			���κδ������Ҳ���tokencode��������Ҫ��ͬ��
**		EMREPPRN			�����طţ���ҪС�ģ�Ӧ��������¼�������ѹ���Ա
**		EMREPDISABLE		�طŵ�һ���Ĵ��������
**		EMNEXTPRN			����Next��ģʽ
**		EMTHRESHOLDNXT		����tokencode�Ĵ���������Ｋ�ޣ��Ժ����NEXTģʽ
**		EMBADNEX			��Nextģʽ���������tokencode
**		EMNEXTDISABLE		��Nextģʽ���������������Ｋ�ޣ�������
**		EMBADTYPE			�������������
**		EMRESPONSEERR		��ս��֤��ʽ��֤ʧ��
**		
**����˵��:	��ο���������PT_DCSHCODE2CHECK��ϸ˵��
**
**��ʷ:		����		����		��Ϊ
		2008-6-23		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckCredential (PTTOKENRECORDDATA pdata, PT_DCSHCODE2CHECK ptCode2Check);

/******************************************************************************
**
**��������:	ReadCardInfoFromFile
**
**��������:	��ȡ���ƹο���Ϣ���ļ�����ο�PTTOKENCARDRECORD�ṹ��
**
**����˵��:
		char *CardFileName					[in /    ] - �ļ�����
		PTTOKENCARDRECORD *pTokenCardRecord	[in / out] - ���ݿ飬�ڲ�����ռ䣬DCSHSAExitBINRead�ͷ�
		int *iCounter						[in / out] - �ܼ�¼��
**
**��������: int  
**		EMSUCCESS �ɹ�
**		EMBADARG  ʧ�ܣ����ܲ������ݴ���
**		
**����˵��:	pTokenCardRecord���ڴ�ռ���ʹ��DCSHSAExitBINRead�ͷ�
**
**��ʷ:		����		����		��Ϊ
		2008-8-5		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAReadCardInformation (char *CardFileName, PTTOKENCARDRECORD *pTokenCardRecord, int *iCounter);


/******************************************************************************
**
**��������:	DCSHSAGetSMSPasscode
**
**��������:	��ȡ����Ϣ��֤�Ķ�̬���ͬʱ���øÿ������Чʱ��
**
**����˵��:
		LONG valid_time		[in /    ] - ��Чʱ�䣬��λ��
		PTTOKENDATA pdata	[in / out] - �������ݰ�
		char *prn			[in / out] - ���ص�tokencode���ⲿ��������
**
**��������: int 
**		EMSUCCESS			��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMBADREC			���ش���PDATA�ڲ����������⣬��ǩ����
**		EMTIMRNG			passcodeʱ����󣬲�������������
**		EMNOTASSIGNED		���ƻ�û�з���
**      EMDISABLE			���ƴ��ڽ���״̬
**		EMFOREVERDISABLE	���Ʊ����ý��ã�������ʱ��ƫ�Ƴ���12Сʱ
**		EMBADTYPE			�������������
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-8-15		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetSMSPasscode (ULONG valid_time, PTTOKENRECORDDATA pdata, char *prn);

/******************************************************************************
**
**��������:	DCSHSACheckSMSPasscode
**
**��������:	��֤���Ŷ�̬�������Чʱ����
**
**����˵��:
		PTTOKENDATA pdata	[in / ] - �������ݰ�
		char *prn			[in / ] - ���ص�tokencode���ⲿ��������
**
**��������: int 
**		EMSUCCESS			��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMBADREC			���ش���PDATA�ڲ����������⣬��ǩ����
**		EMTIMRNG			passcodeʱ����󣬲�������������
**		EMNOTASSIGNED		���ƻ�û�з���
**      EMDISABLE			���ƴ��ڽ���״̬
**		EMFOREVERDISABLE	���Ʊ����ý��ã�������ʱ��ƫ�Ƴ���12Сʱ
**		EMSMSTIMEOUT		��ʱ����������ȷ
**		EMSMSBADPRN			�������
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-8-15		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckSMSPasscode (PTTOKENRECORDDATA pdata, char *prn);


/******************************************************************************
**
**��������:	DCSHSACheckPasscodeEx������ʹ��DCSHSACheckCredential����
**
**��������:	����һ��������֤�ӿڣ�������Ϸ�ʽ��Ҳ���Խ�����֤
			��չ�ġ�ͳһ����֤�ӿ�
			����� ʱ�䡢�¼����ֻ� ��֤���� prn=tokencode,NULL,0
			����� �첽��֤�����ֻ�����   �� prn=response,challenge=challenge,0
			����Ƕ�����֤����ö�̬����� prn=�ⲿ����,NULL,valid_time=��Чʱ�䣨�룩
			                ��֤��̬����� prn=tokencode,NULL,0
			�����ATM���ƣ���Ҫ�����㷨�� �� prn=��̬����,challenge=��ս�루�û����Ҳ����ʹժҪ��,valid_time=�㷨��ALGORITHM_ATM/MD5/SH1��
			����ǹιο����ƣ�ֻ������֤���� prn=��̬����,challenge=������,valid_time=0��
**
**����˵��:
		PTTOKENRECORDDATA pdata	[in / out] - �������ݰ�
		char *prn				[in / out] - ����Ҫ����tokencode��������Ӧ�룬���߷���ֵ���ο��Ϸ�˵��
		char *challenge			[in /    ] - �첽ģʽ����Ϊ��ս�룬�ο��Ϸ�˵��
		ULONG valid_time		[in /    ] - ��ö��ſ�������Чʱ�䣬�룬�ο��Ϸ�˵��
**
**��������:  int 
**
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMTOKENTYPE			���������Ӧ������
**		
**		
**����˵��:	ע�⣬���ӿڲ�֧�ִ�PIN����֤
**
**��ʷ:		����		����		��Ϊ
		2008-9-3		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckPasscodeEx (PTTOKENRECORDDATA pdata, char *prn, char *challenge, ULONG valid_time);


/******************************************************************************
**
**��������:	DCSHSAMsChap2CheckAndMakeResponse
**
**��������:	MS CHAP V2���Կͻ����ύ��nt_response������֤
			
			��鿴DCSHMSCHAP2_PASSCODE�ṹ����鿴RFC2759

			����֧��MS CHAP v2���������ڳ���PIN�Ͷ�̬���û���û������������ս�롢�ͻ�����ս���
			���ԣ�Ҫ����øýӿڵ�ʱ���������

			���ص�AuthenticatorResponse�������ĸ�ʽ��S=0123456789ABCDEF0123456789ABCDEF01234567��
**
**����˵��:
		DCSHMSCHAP2_PASSCODE *pDCSHMSCHAP2Block	[in / out] - �ṹ�ڰ�����MS Chap V2�ĸ����ֶΣ�AuthenticatorResponse�ɸú�������
		char *cMsChap2Password					[in /    ] - ���ע�⣺
																		�������PIN�������Ӧ��Ϊ����̬����+PIN�롰
																		���������PIN���������ǡ���̬���

																		ע�⣬�û�Ӧ��ҲӦ����˱���
**
**��������: int 
**		DCSHEMBADARG			��������
**		DCSHEMMSCHAPV2ERROR		mschapv2��֤�ͻ����ύ����Ӧ�����
**		DCSHEMSUCCESS			�ɹ�
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-2-5		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAMsChap2CheckAndMakeResponse(DCSHMSCHAP2_PASSCODE *pDCSHMSCHAP2Block, char *cMsChap2Password);

/******************************************************************************
**
**��������:	DCSHSACheckPasscodeMsChap2
**
**��������:	
**
**����˵��:
		LONG passc_time		[in /    ] - DCSHSAGetPasscodeTimeȡ��ʱ��
		PTTOKENDATA pdata	[in / out] - �������ݰ�
		unsigned char *pMsChap2Block[in / out] - DCSHMSCHAP2_PASSCODE���͵�block
**
**��������: int 
**		EMSUCCESS			��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMBADREC			���ش���PDATA�ڲ����������⣬��ǩ����
**      EMFIRST				��һ��ʹ�����Ƶ�½���ɹ�����ͬ��EMSUCCESS

**		EMPRNLEN			Tokencode�ĳ��ȴ���
**		EMTIMRNG			passcodeʱ����󣬲�������������
**		EMNOTASSIGNED		���ƻ�û�з���
**      EMDISABLE			���ƴ��ڽ���״̬
**		EMFOREVERDISABLE	���Ʊ����ý��ã�������ʱ��ƫ�Ƴ���12Сʱ
**		EMNOPIN				û������pin��
**		EMPINLEN			�����PIN���ȴ���
**		EMBADPIN			��������PIN
**		EMSTOLEN			��ΪPIN����������Ｋ�ޣ�Ĭ��Ϊ��ʧ
**		EMOUTWIND			���κδ������Ҳ���tokencode��������Ҫ��ͬ��
**		EMREPPRN			�����طţ���ҪС�ģ�Ӧ��������¼�������ѹ���Ա
**		EMREPDISABLE		�طŵ�һ���Ĵ��������
**		EMNEXTPRN			����Next��ģʽ
**		EMTHRESHOLDNXT		����tokencode�Ĵ���������Ｋ�ޣ��Ժ����NEXTģʽ
**		EMBADNEX			��Nextģʽ���������tokencode
**		EMNEXTDISABLE		��Nextģʽ���������������Ｋ�ޣ�������
**		EMBADTYPE			�������������
**		
**		
**����˵��:	
			ms-chap v2����֤��ʽ�ǵò����û���PIN��Ͷ�̬�����
			���ǣ����Ǳ���������еõ���Щ��Ȼ������̼�����Ӧ�룬���бȽ�
			��ʱ�����Ǿͻᷢ���޷�����ط�
			����������⣬����ֻ��ǡ�����жϣ�
			1���������Ǽ�ס���4����ȷ��tokencode
			2����������֤����ʷ��tokencode��ͬ������Ϊ�ط�
			3������ڴ�����ɨ�裬һ����������ʷtokencode��ͬ��������

**
**��ʷ:		����		����		��Ϊ
		2008-5-15		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckPasscodeMsChap2 (ULONG passc_time, PTTOKENRECORDDATA pdata, unsigned char *pMsChap2Block/*DCSHMSCHAP2_PASSCODE���͵�block*/);


/******************************************************************************
**
**��������:	DCSHSAMsChap2ChangePassword
**
**��������:	MS CHAP V2�����ܵõ��¿������֤�¿���
			
			��鿴DCSHMSCHAP2_CHANGE_PW�ṹ����鿴RFC2759

			�޸Ŀ���֮ǰ��Ҫ�����֤

			��֤ͨ�����ٽ����¿��������֤�¿����Ƿ���ȷ

			���ص�AuthenticatorResponse�������ĸ�ʽ��S=0123456789ABCDEF0123456789ABCDEF01234567��
**
**����˵��:
		DCSHMSCHAP2_CHANGE_PW *pDCSHMSCHAP2Blockpw[in /    ] - �ṹ�ڰ�����MS Chap V2�ĸ����ֶΣ�AuthenticatorResponse�ɸú�������
		char *oldPinAndTokencode				  [in /    ] - ���ע�⣺
																		�������PIN�������Ӧ��Ϊ����̬����+PIN�롰
																		���������PIN���������ǡ���̬���
	
																		ע�⣬�û�Ӧ��ҲӦ����˱���
**		char *newpin							  [in / out] - �¿���

**��������: int 
**		DCSHEMBADARG			��������
**		DCSHEMMSCHAPV2ERRORPW	mschapv2�ͻ����ύ���¿���У���쳣
**		DCSHEMSUCCESS			�ɹ�
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-2-5		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAMsChap2ChangePassword(DCSHMSCHAP2_CHANGE_PW *pMsChap2Blockpw, char *oldPinAndTokencode, char *newpin);


/******************************************************************************
**
**��������:	DCSHSAChangePINCodeMsChap2
**
**��������:	ms chap v2 ���Ŀ���
**
**����˵��:
		ULONG passc_time				[in /    ] - DCSHSAGetPasscodeTimeȡ��ʱ��
		PTTOKENRECORDDATA pdata			[in /    ] - �������ݰ�
		unsigned char *pMsChap2Blockpw	[in / out] - �ο�DCSHMSCHAP2_CHANGE_PW�ṹ��������authenticatorResponse
**
**��������:  int 
**		DCSHEMBADARG	��������
**		DCSHEMSUCCESS	�ɹ�
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-2-5		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAChangePINCodeMsChap2 (ULONG passc_time, PTTOKENRECORDDATA pdata, unsigned char *pMsChap2Blockpw/*DCSHMSCHAP2_CHANGE_PW���͵�block*/);


/******************************************************************************
**
**��������:	DCSHSAForceSynchronizeEventCounter
**
**��������: ǿ��ͬ���¼������Ƶļ�����
**			��ĳ������£���Ҫ��÷���˵�ǰ�ļ�������Ȼ���ۼ�һ����������Ϊǿ�Ƶ�ͬ���Ĳ�����Ҫ��ͻ���ǿ���޸ļ����������������	
**
**����˵��:
		PTTOKENRECORDDATA pdata		[in / out] - �������ݰ�
		unsigned int CounterAdd		[in /    ] - һ���������������100С��1000��
		unsigned int *SrvCounter	[in / out] - ���õ�ǰ���Ƶļ�������ֵ�����ҷ��ظ�ֵ
**
**��������:  int 
**		DCSHEMBADARG			��������
**		DCSHEMFORCESYNTYPE		ǿ��ͬ���������ʹ��󣬲����¼���
**		DCSHEMSUCCESS			�ɹ�
**
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-6-22		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAForceSynchronizeEventCounter(PTTOKENRECORDDATA pdata, unsigned int CounterAdd, unsigned int *SrvCounter );

/******************************************************************************
**
**��������:	DCSHSAForceSynchronizeEventCounterReverse
**
**��������:	���ݿͻ��˵ļ�����ǿ��ͬ���������м�����
			���������ĵ�ʱ�򣬲�����������ļ�������С�޸ģ��ͻ����ύ�ļ�����������ڷ������ļ�����

**
**����˵��:
		PTTOKENRECORDDATA pdata		[in / out] - �������ݰ�
		unsigned int ClientCounter	[in /    ] - �ͻ��˵ļ�����
**
**��������:  int 
**		DCSHEMBADARG			��������
**		DCSHEMFORCESYNTYPE		ǿ��ͬ���������ʹ��󣬲����¼���
**		DCSHEMFORCECOUNTERLESS	������ʹ�ÿͻ��˵�Сֵ�޸ķ���˵Ĵ�ֵ
**		DCSHEMSUCCESS			�ɹ�
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-1-27		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAForceSynchronizeEventCounterReverse(PTTOKENRECORDDATA pdata, unsigned int ClientCounter );


/******************************************************************************
**
**��������:	DCSHSAUnlockPinpadToken
**
**��������:	���K8�����Ľ����룬��pad�������ڶ������pinʧ�ܺ����������ʱ�ṩ��Ļ�����ֺ����кţ��ɵ�����˽��С���ȡ�����롱
**
**����˵��:
		PTTOKENRECORDDATA pdata	[in /    ] - �������ݰ�
		char *cChallenge		[in /    ] - ������������Ļ�ϵ�8λ����
		char *cUnLockCode		[in / out] - �����룬8λ����
**
**��������:  int 
**		DCSHEMBADARG			��������
**		DCSHEMSUCCESS			�ɹ�
**
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-7-29		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAUnlockPinpadToken(PTTOKENRECORDDATA pdata, char *cChallenge, char *cUnLockCode);

/*
�������������ͣ�[ʱ����Time]			[�¼���Event���ֻ��������¼��ͣ�]

���������ֽ��ʣ�[Ӳ����SIM����Ӳ����]	[������ֻ��ϵ��������]

�������������ࣺ[��ͨ����K5�� K3��]		[��������]	[�ֻ�����] 	[�������]

���Ƶ�Ӧ��ģʽ��[��ͨ��֤ģʽ]	[������֤ģʽ]	[��սӦ��ģʽ]	[����ǩ��ģʽ]	[MSChapV2ģʽ]

ÿ��ģʽ�����Լ�����֤�ӿ�

����Ĺ�ϵ�ǲ����ӵģ�������K8���ƾ���ʱ���͵�Ӳ��K3���ƣ�ͬʱ֧������սӦ�������ǩ��ģʽ

�������������ƿ�����ʱ�����͵ģ�Ҳ�������¼����͵�
      �������ƴ�ͳ��������������ʣ����Ҫ��ΪӲ���ģ�Ҳ����
	  �������Ƶ�Ӧ��ģʽ�ǻ�ȡ����֤������͵�ͬ����ͨ��֤ģʽ
	  ��������Ҳ����֧������ģʽ��ֻҪ���ýӿھͿ���
	  ����ͬK5������ȫ����֧������ǩ��ģʽ��ֻ����K5û�м��̶���

  �������˵���Ĳ�ṹ���ǲ����ӷ�ʽ�ģ�����Ӧ���Խӿڵ��ú�ʵ�����ƾ���

*/

/******************************************************************************
**
**��������:	DCSHSAGetCoverHostCode
**
**��������:	��õ�ǰ�ļ�������Ӧ�������ַ��Ͷ�̬������������ַ��͵�ǰʹ�õĴ���
**
**����˵��:
		ULONG passc_time		[in /    ] - ϵͳʱ��
		PTTOKENRECORDDATA pdata	[in /    ] - pdata
		char *cHostCode			[in / out] - �����ַ����ⲿ����ռ�
		int *iCurrentUseCount	[in / out] - ��ǰʹ�õĴ���
**
**��������:  int 
**		DCSHEMBADARG	��������
**		DCSHEMNOTCOVERTYPE	���ǹιο����͵�����
**		DCSHEMOVERCOVERNUM	�ﵽ���ֵ���ιο��������õ����һ����
**		DCSHEMSUCCESS	�ɹ�
**
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-8-14		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetCoverHostCode (ULONG passc_time, PTTOKENRECORDDATA pdata, char *cHostCode, int *iCurrentUseCount);


/******************************************************************************
**
**��������:	DCSHSACheckCoverTokenCode
**
**��������:	��֤�������Ķ�̬�����Ҫ���������ַ��Ͷ�̬������������PIN������Ҫ����PIN
**
**����˵��:
		ULONG passc_time		[in /    ] - ��ǰϵͳʱ��
		PTTOKENRECORDDATA pdata	[in / out] - pdata
		char *cHostCode			[in /    ] - �����ַ�
		char *cTokenCode		[in /    ] - ��������̬����
		char *pin				[in /    ] - pin��
**
**��������:  int 
**		DCSHEMBADARG	��������
**		DCSHEMNOTCOVERTYPE	���ǹιο�����
**		DCSHEMMUSTGETCOVER	��֤֮ǰӦ�û�������ַ�
**		DCSHEMOVERCOVERNUM	����֤���ֵ���Ժ��
**		DCSHEMHOSTCODEERRO	�����ַ�����
**		DCSHEMERRORTODISABLE	��̬�������̫��Σ�������
**		DCSHEMCOVERCODEERR	��������̬�������
**		DCSHEMSUCCESS		��֤ͨ��
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-8-14		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckCoverTokenCode (ULONG passc_time, PTTOKENRECORDDATA pdata, char *cHostCode, char *cTokenCode, char *pin);


/******************************************************************************
**
**��������:	DCSHSAUpdateNewCoverCard
**
**��������:	
**
**����˵��:
		ULONG passc_time											[in /    ] - ϵͳʱ��
		PTTOKENRECORDDATA pdataO									[in / out] - �ɿ�pdata
		char *cHostCodeO											[in /    ] - �����ַ�
		char *cTokenCodeO											[in /    ] - ��̬����
		char *pinO													[in /    ] - pin		
        PTTOKENRECORDDATA pdataN									[in / out] - �¿�pdata 
		char *cHostCodeN											[in /    ] - �����ַ�
		char *cTokenCodeN											[in /    ] - ��̬����
		char *pinN													[in /    ] - pin
**
**��������: int 
**		DCSHEMBADARG	��������
**		DCSHEMNOTCOVERTYPE	���ǹιο�����
**		DCSHEMMUSTGETCOVER	��֤֮ǰӦ�û�������ַ�
**		DCSHEMOVERCOVERNUM	����֤���ֵ���Ժ��
**		DCSHEMHOSTCODEERRO	�����ַ�����
**		DCSHEMERRORTODISABLE	��̬�������̫��Σ�������
**		DCSHEMCOVERCODEERR	��������̬�������
**		DCSHEMSUCCESS		��֤ͨ�������Ը���		
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-8-16		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAUpdateNewCoverCard (ULONG passc_time, PTTOKENRECORDDATA pdataO, char *cHostCodeO, char *cTokenCodeO, char *pinO,
											                    PTTOKENRECORDDATA pdataN, char *cHostCodeN, char *cTokenCodeN, char *pinN);

/******************************************************************************
**
**��������:	DCSHSAXORStringTo8Char
**
**��������:	���ַ���ͨ�����ת��Ϊ8�ֽڣ�ԭ����8�ֽڷ��飬��Ӧ��λ�����
**
**����˵��:
		const char *cBuffer			[in /    ] - ԭʼ�ַ��������Ȳ��� 
		unsigned char *cDDWBUffer	[in / out] - �����8�ֽڣ������ⲿ�ṩ8�ֽڿռ䣬�ҳ�ʼ��0
**
**��������:  int 
**		EMBADARG  �������󣬲��������ǿ�ָ�룬�����㷨ֵ����
**		EMSUCCESS �ɹ�		
**		
**		
**����˵��:	
**			С�ڵ���8�ֽڵ��ַ�����ֱ��copy
**
**��ʷ:		����		����		��Ϊ
		2009-12-8		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAXORStringTo8Char (const char *cBuffer, unsigned char *cDDWBUffer);



//---------------------------------�ͻ����ƵĿ����ӿ�------------------------------------//


/******************************************************************************
**
**��������:	DCSHSACheckPasscode_for_hexin
**
**��������:	ר��Ϊ���Ķ��Ƶ���֤�ӿ�
			�ýӿ��У�����ֵDCSHEMSUCCESS��DCSHEMFIRST����ȷ�ģ�������ͳͳ����Ϊ�Ǵ���
			����ֵͬʱ����ƫ�����ı仯��update�Ľ��飬���У�
			DCSHEMFIRST����ֵ����update��
			DCSHEMSUCCESSʱ��������Ϊoffset�仯�˾�Ӧ��update,������Ȩ�ɺ��Ŀ��ƣ�����ע�⣬��iAdjustStep�ӽ�С�������ֵʱ������update;
**
**����˵��:
		LONG passc_time		[in /    ] - DCSHSAGetPasscodeTimeȡ��ʱ��
		PTTOKENDATA pdata	[in / out] - �������ݰ�
		char *prn			[in /    ] - tokencode
		char *pin			[in /    ] - pin
		int *iAdjustStep	[in / out] - �˴���֤������ƫ������������0��Ҳ������������Ҳ�����Ǹ���
		int *iAdviseUpdate	[in / out] - ���������Ƿ�Update��0-��ʾ����Ҫ��1-��ʾ��Ҫ
**
**��������: int 
**		EMSUCCESS			��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
**      EMFIRST				��һ��ʹ�����Ƶ�½���ɹ�����ͬ��EMSUCCESS
**		����ֵͳͳ��Ϊʧ��

**		DCSHSACheckPasscode�ķ���ֵ��ͬ��
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-1-4		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckPasscode_for_hexin (ULONG passc_time, PTTOKENRECORDDATA pdata, char *prn, char *pin, int *iAdjustStep, int *iAdviseUpdate);

/******************************************************************************
**
**��������:	DCSHSAModifyTokenDataWindow
**
**��������:	�޸��������ݵ���֤����
**			���飺
					1��ȯ�̡���Ϸ ���ó�Ϊ ʱ���ͣ�120��300��600�����¼��ͣ�2��5��10��
					2�����С���ҵ ���ó�Ϊ ʱ���ͣ�60��180��300�����¼��ͣ� 1��3��5��
**			
**			�ò������������״̬λ����֤ģʽ
**����˵��:
		PTTOKENRECORDDATA pdata		[in / out] - pdata����������
		int iSmallWindowSeconds		[in /    ] - �µ�С����
		int iMediumWindowSeconds	[in /    ] - �µ��д���
		int iLargeWindowSeconds		[in /    ] - �µĴ󴰿�
**
**��������:  int 
**			EMSUCCESS			�޸ĳɹ������뷵�ر��뱣��PDATA�����ݿ�
**			�����ķ���ֵ����ʧ��
**		
**����˵��:	�ýӿ����ڱ��ȵ���ʹ�ã��ͻ��ڶ�������ʱ��Ӧ��Ԥ������������Ƶ���ϸ��������
**
**��ʷ:		����		����		��Ϊ
		2010-8-4		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAModifyTokenDataWindow (PTTOKENRECORDDATA pdata, int iSmallWindowSeconds, int  iMediumWindowSeconds, int iLargeWindowSeconds);


/******************************************************************************
**
**��������:	DCSHSACheckPasscode_for_City9
**
**��������:	ר��Ϊ�ųǶ��Ƶ���֤�ӿ�
			�ýӿ��У�����ֵDCSHEMSUCCESS��DCSHEMFIRST����ȷ�ģ�������ͳͳ����Ϊ�Ǵ���
			����ֵͬʱ����ƫ�����ı仯��update�Ľ��飬���У�
			DCSHEMFIRST����ֵ����update��
			DCSHEMSUCCESSʱ��������Ϊoffset�仯�˾�Ӧ��update,������Ȩ�ɾųǿ��ƣ�����ע�⣬��iAdjustStep�ӽ�С�������ֵʱ������update;
**
**����˵��:
		char *pdataBase64	[in / out] - �������ݰ���base64�ַ���
		char *prn			[in /    ] - tokencode
		char *pin			[in /    ] - pin
		int *iAdjustStep	[in / out] - �˴���֤������ƫ������������0��Ҳ������������Ҳ�����Ǹ���
		int *iAdviseUpdate	[in / out] - ���������Ƿ�Update��0-��ʾ����Ҫ��1-��ʾ��Ҫ
**
**��������: int 
**		EMSUCCESS			��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
**      EMFIRST				��һ��ʹ�����Ƶ�½���ɹ�����ͬ��EMSUCCESS
**		����ֵͳͳ��Ϊʧ��

**		DCSHSACheckPasscode�ķ���ֵ��ͬ��
**		
**		
**����˵��:	
**			�ڲ�������ȡϵͳUTCʱ��
**			�ڲ���״̬λ���������ò���
**			Ϊ�˽��Ϳͻ�Ӧ�õ����ݿ�������ṩ��Update���ݿ�Ľ���
**			����ⲿ�û��д����������������з���ȷ����ֵ�����Բ��������ݿ�
**
**��ʷ:		����		����		��Ϊ
		2010-1-24		�		Created
		2010-6-24		�		��pdata�����ݽ�����Base64ת��
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckPasscode_for_City9 ( char *pdataBase64, char *prn, char *pin, int *iAdjustStep, int *iAdviseUpdate);

/******************************************************************************
**
**��������:	DCSHSAInitPdata_for_City9
**
**��������:	Ϊ9�Ƕ��Ƶ��������ݳ�ʼ���Ľӿڣ��书�ܾ��� ����/����/����PIN
**
**����˵��:
		char *pdataBase64	[in / out] - �������ݰ���base64�ַ���
**
**��������: 
**		EMBADARG  �������󣬲��������ǿ�ָ�룬�����㷨ֵ����
**		EMSUCCESS �ɹ�
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-1-25		�		Created
		2010-6-24		�		��pdata�����ݽ�����Base64ת��
******************************************************************************/
DCSHCOREAPI_API int DCSHSAInitPdata_for_City9 ( char *pdataBase64);


/******************************************************************************
**
**��������:	DCSHSAResynchTokenEx_for_City9
**
**��������:	Ϊ9�Ƕ��Ƶ�����ͬ���ӿ�
**
**����˵��:
		char *pdataBase64	[in / out] - �������ݰ���base64�ַ���
		char *prn1			[in /    ] - ��һ����̬����
		char *prn2			[in /    ] - �����ĵڶ�����̬����
**
**��������: int 
**		DCSHEMBADARG	��������
**		DCSHEMSUCCESS	ͬ���ɹ�
**		����ֵ�ο�������Ϣ�б�
**		
**		
**����˵��:	ֻ�е�����DCSHEMSUCCESSʱ������ҪUpdate���ݿ�
**
**��ʷ:		����		����		��Ϊ
		2010-8-12		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAResynchTokenEx_for_City9(char *pdataBase64, char *prn1, char *prn2);

/******************************************************************************
**
**��������:	DCSHSAGetCustomTokenPasscode
**
**��������:	��ȡ�����������Ƶ���֤�Ķ�̬�������DCSH_HARDWARE_TOKEN_GET���͵�����
			ĿǰΪ�������ƽӿڣ� ���������Ӳ��Կ��ƣ���ʱ�䲻������ȡ

			ע�⣬һ����ȡ��̬����󣬸ö�̬��������ڶ���ϵͳ��֤
			First��ȡ�󣬽�����ΪNormal��������һ���Ѿ���ʹ��
			Normal��ȡ����Ȼ��Normal
			Next/Next2��ȡ�󣬻�ԭ��Normal
**
**����˵��:
		LONG passc_time		[in /    ] - DCSHSAGetPasscodeTimeȡ��ʱ��
		char *pdatabase64	[in / out] - �������ݰ���base64�ַ���
		char *prn			[in / out] - ���ص�tokencode���ⲿ��������
**
**��������: int 
**		EMSUCCESS			��֤ͨ��������EMBADREC�����еķ��ر��뱣��PDATA�����ݿ�
**		EMBADARG			�������󣬲�������Ϊ��ָ��
**		EMBADREC			���ش���PDATA�ڲ����������⣬��ǩ����
**		EMTIMRNG			passcodeʱ����󣬲�������������
**		EMNOTASSIGNED		���ƻ�û�з���
**      EMDISABLE			���ƴ��ڽ���״̬
**		EMFOREVERDISABLE	���Ʊ����ý��ã�������ʱ��ƫ�Ƴ���12Сʱ
**		EMBADTYPE			�������������
**		
**		
**����˵��:	����ÿһ�λ�ö�̬�����Ӧ�ñ���pdatabase64�������������Ҫ��Ҳ���Բ�����
**
**��ʷ:		����		����		��Ϊ
		2010-3-28		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetCustomTokenPasscode (ULONG passc_time, char *pdataBase64, char *prn);


/******************************************************************************
**
**��������:	DCSHSABase64Encode
**
**��������:	�Զ����������б��룬�γɿɼ��ַ�
**
**����˵��:
		unsigned char * binaryDataPtr	[in /    ] - ��������
		int binaryDataLength			[in /    ] - ������������
		char * stringData				[in / out] - �ɼ��ַ������ⲿ����ռ�
		int *stringDataLength			[in / out] - �ɼ��ַ�������
**
**��������: int 
**		DCSHEMSUCCESS			�ɹ�
**		DCSHEMBASE64CODEERR	 ʧ��
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-8-19		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSABase64Encode( unsigned char * binaryDataPtr, int binaryDataLength, char * stringData ,int *stringDataLength );

/******************************************************************************
**
**��������:	DCSHSABase64Decode
**
**��������:	��base64������н��룬��ö�������
**
**����˵��:
		char * stringData				[in /    ] - �ɼ��ַ���
		int stringDataLen				[in /    ] - �ɼ��ַ�������
		unsigned char * binaryDataPtr	[in / out] - �����������ⲿ����ռ�
		int * binaryDataLength			[in / out] - ���������ĳ���
**
**��������:  int 
**		DCSHEMSUCCESS			�ɹ�
**		DCSHEMBASE64CODEERR	 ʧ��
**			
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-8-19		�		Created
******************************************************************************/
DCSHCOREAPI_API int  DCSHSABase64Decode( char * stringData, int stringDataLen, unsigned char * binaryDataPtr, int * binaryDataLength );


/******************************************************************************
**
**��������:	DCSHSAUpdateNewSeedForICBC
**
**��������:	�����������ݣ�����ʹ�������֤��������������������󣬵õ��µ�����
**
**����˵��:
		PTTOKENRECORDDATA pdata	[in / out] - ��������
		char *cID				[in /    ] - �������֣�Ӧ����9byte
**
**��������:  int 
**		DCSHEMBADARG ��������
**		DCSHEMICBCHAVEXOR �Ѿ�������
**		DCSHEMSUCCESS �ɹ�
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-11-19		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAUpdateNewSeedForICBC(PTTOKENRECORDDATA pdata, char *cID );

/******************************************************************************
**
**��������:	DCSHSAICBCActiveUnlockAlg
**
**��������:	�������Ƶ����Ӷ��α����㷨�����ƽ����㷨
**
**����˵��:
		BYTE *bSeed				[in /    ] - ���ӣ�����֮ǰΪ�����ӣ�����֮��Ϊ�����ӣ�20�ֽ�
		int iSeedLen			[in /    ] - ���ӳ��ȣ����й̶�Ϊ20�ֽڣ��̶�Ϊ20
		char *cChallengeCode	[in /    ] - ��ս�룬����ʱΪ������6���֣���0x00,0x00������ʱΪ������8����
		int iChallengeCodeLen	[in /    ] - ��ս��ĳ��ȣ��̶�Ϊ8
		char *cResponseCode		[in / out] - ��Ӧ�룬��У������߽����룬�ⲿ����ռ�
		int iResponseCodeLen	[in /    ] - ��Ӧ�볤�ȣ��̶�Ϊ6
**
**��������: int 
**		DCSHEMBADARG		���������˿�ָ��
**		DCSHEMSEEDLENERR	���ӳ��ȴ���
**      DCSHEMCHECKLENERR	ָ������Ӧ�볤�ȴ���
**      DCSHEMACTIVELENERR	��ս��ĳ��ȴ���
**		DCSHEMSUCCESS		�ɹ�
**		
**����˵��:	�����Msg�����ݳ��Ȳ���8Byte
**
**��ʷ:		����		����		��Ϊ
		2010-12-22		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAICBCActiveUnlockAlg( BYTE *bSeed, int iSeedLen, char *cChallengeCode, int iChallengeCodeLen, char *cResponseCode, int iResponseCodeLen );

/******************************************************************************
**
**��������:	DCSHSAICBCGenerateNewSeedAlg
**
**��������:	�������Ʊ��εõ��������㷨����Ҫ���뼤�����У����
**
**����˵��:
		BYTE *bSeed				[in /    ] - �������ݣ�20�ֽ�
		int iSeedLen			[in /    ] - ���ӳ��ȣ�20
		char *cChallengeCode	[in /    ] - �����룬������Ϊ6�����֣�����ʱ��0x00,0x00
		int iChallengeCodeLen	[in /    ] - ������ĳ��ȣ�8
		char *cResponseCode		[in /    ] - У���룬6�����ַ�
		int iResponseCodeLen	[in /    ] - У����ĳ���6
		BYTE *bNewSeed			[in / out] - �µ����ӣ��ⲿ����ռ䣬����20�ֽ�
		int *iNewSeedLen		[in / out] - �����ӳ��ȣ�20
**
**��������: int 
**		DCSHEMBADARG		���������˿�ָ��
**		DCSHEMSEEDLENERR	���ӳ��ȴ���
**      DCSHEMCHECKLENERR	ָ������Ӧ�볤�ȴ���
**      DCSHEMACTIVELENERR	��ս��ĳ��ȴ���
**		DCSHEMACTIVCHECKERR	������У��ʧ�ܣ������б���
**		DCSHEMSUCCESS		�ɹ�
**	
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-12-22		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAICBCGenerateNewSeedAlg( BYTE *bSeed, int iSeedLen, char *cChallengeCode, int iChallengeCodeLen, char *cResponseCode, int iResponseCodeLen , BYTE *bNewSeed, int *iNewSeedLen);

/******************************************************************************
**
**��������:	DCSHSAICBCTimeTokeAlg
**
**��������:	���е�ʱ���Ͷ�̬�����㷨
**
**����˵��:
		BYTE *bSeed			[in /    ] - ���ӣ�20�ֽ�
		int iSeedLen		[in /    ] - ���ӳ��ȣ�20
		BYTE *bTokenTime	[in /    ] - ʱ�����ݣ�ע���ͷ����8�ֽ�
		int iTokenTimeLen	[in /    ] - ʱ�����ݳ��ȣ��̶�Ϊ8
		char *cTokenCode	[in / out] - ��̬���6�����ַ����ⲿ����ռ�
		int iTokenCodeLen	[in /    ] - ��̬����ȣ��̶�Ϊ6
**
**��������: int 
**		DCSHEMBADARG		���������˿�ָ��
**		DCSHEMSEEDLENERR	���ӳ��ȴ���
**      DCSHEMPRNLEN		��̬����ȴ���
**      DCSHEMBIGTIMELENERR	��ͷ�����ʱ�䳤�ȴ���
**		DCSHEMSUCCESS		�ɹ�
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-12-22		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAICBCTimeTokeAlg( BYTE *bSeed, int iSeedLen, BYTE *bTokenTime, int iTokenTimeLen, char *cTokenCode, int iTokenCodeLen );


/******************************************************************************
**
**��������:	DCSHSAICBCChallengeResponseAlg
**
**��������:	���е�ʱ����սӦ���Ͷ�̬�����㷨
**
**����˵��:
		BYTE *bSeed					[in /    ] - ���ӣ�20�ֽ�
		int iSeedLen				[in /    ] - ���ӳ��ȣ�20
		BYTE *bTokenTime			[in /    ] - ʱ�����ݣ���ͷ����8�ֽ�
		int iTokenTimeLen			[in /    ] - ʱ�䳤�ȣ��̶�Ϊ8
		char *cResponse				[in /    ] - ��ս���⣬�ַ���
		unsigned int iResponseLen	[in /    ] - ��󳤶�64
		char *cTokenCode			[in / out] - ��̬���6�����ַ����ⲿ����ռ�
		int iTokenCodeLen			[in /    ] - ��̬����ȣ��̶�Ϊ6
**
**��������: int 
**		DCSHEMBADARG		���������˿�ָ��
**		DCSHEMSEEDLENERR	���ӳ��ȴ���
**      DCSHEMPRNLEN		��̬����ȴ���
**      DCSHEMBIGTIMELENERR	��ͷ�����ʱ�䳤�ȴ���
**      DCSHEMCHANLENERR	��ս����ĵĳ��ȴ���64������
**		DCSHEMSUCCESS		�ɹ�		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-12-22		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAICBCChallengeResponseAlg( BYTE *bSeed, int iSeedLen, BYTE *bTokenTime, int iTokenTimeLen, char *cResponse, unsigned int iResponseLen, char *cTokenCode, int iTokenCodeLen );

/******************************************************************************
**
**��������:	DCSHSAICBCPinLockedAlg
**
**��������:	�������Ƶ�PIN������������������㷨
**
**����˵��:
		BYTE *bSeed				[in /    ] - �������ݣ�20�ֽ�
		int iSeedLen			[in /    ] - ���ӳ��ȣ�20
		BYTE *bTokenTime		[in /    ] - ʱ�����ݣ���ͷ����8�ֽ�
		int iTokenTimeLen		[in /    ] - ʱ�����ݳ��ȣ�8
		char *cPin				[in /    ] - ���Ƶ�PIN�룬6�����ַ�,��0x00,0x00
		unsigned int iPinLen	[in /    ] - PIN�볤�ȣ�8
		char *cPinLockedCode	[in / out] - ���Ƶ������룬8�����ַ�
		int iPinLockedCodeLen	[in /    ] - �����볤�ȣ�8
**
**��������: int 
**		DCSHEMBADARG		���������˿�ָ��
**		DCSHEMSEEDLENERR	���ӳ��ȴ���
**      DCSHEMPRNLEN		��̬����ȴ���
**      DCSHEMBIGTIMELENERR	��ͷ�����ʱ�䳤�ȴ���
**      DCSHEMCHANLENERR	��ս����ĵĳ��ȴ���64������
**      DCSHEMPINLEN		Pin�볤�ȴ���
**		DCSHENUNLOCKPINERR	���ƽ�������ַ������ȴ���8����
**		DCSHEMSUCCESS		�ɹ�	
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-12-22		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAICBCPinLockedAlg( BYTE *bSeed, int iSeedLen, BYTE *bTokenTime, int iTokenTimeLen, char *cPin, unsigned int iPinLen, char *cPinLockedCode, int iPinLockedCodeLen );


/******************************************************************************
**
**��������:	DCSHSAConvertICBCTokenSeed
**
**��������:	��һ��ʹ��ʱת��ԭʼ����������
**
**����˵��:
		PTTOKENRECORDDATA pdata	[in / out] - pdata����
		char *cActiveCode		[in /    ] - �����룬12������
**
**��������:  int 
**		DCSHEMBADARG		���������˿�ָ��
**		DCSHEMSEEDLENERR	���ӳ��ȴ���
**      DCSHEMPRNLEN		��̬����ȴ���
**      DCSHEMBIGTIMELENERR	��ͷ�����ʱ�䳤�ȴ���
**      DCSHEMCHANLENERR	��ս����ĵĳ��ȴ���64������
**      DCSHEMPINLEN		Pin�볤�ȴ���
**		DCSHENUNLOCKPINERR	���ƽ�������ַ������ȴ���8����
**		DCSHEMSUCCESS		�ɹ�**		
**		
**����˵��:	�����뱻������ucUserLastName��
			ԭʼ���ӱ�������ucUserFirstName��
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAConvertICBCTokenSeed(PTTOKENRECORDDATA pdata, char *cActiveCode );

/******************************************************************************
**
**��������:	DCSHSAGetICBCActiveCode
**
**��������:	ת��֮ǰ��ü����룬12������
**
**����˵��:
		PTTOKENRECORDDATA pdata	[in /    ] - pdata
		char *cActiveCode		[in / out] - ���Ƽ����룬12�����֣��ⲿӦ����12���ϵĿռ�
**
**��������:  int 
**		DCSHEMBADARG		���������˿�ָ��
**		DCSHEMSEEDLENERR	���ӳ��ȴ���
**      DCSHEMPRNLEN		��̬����ȴ���
**      DCSHEMBIGTIMELENERR	��ͷ�����ʱ�䳤�ȴ���
**      DCSHEMCHANLENERR	��ս����ĵĳ��ȴ���64������
**      DCSHEMPINLEN		Pin�볤�ȴ���
**		DCSHENUNLOCKPINERR	���ƽ�������ַ������ȴ���8����
**		DCSHEMSUCCESS		�ɹ�
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetICBCActiveCode(PTTOKENRECORDDATA pdata, char *cActiveCode );


/******************************************************************************
**
**��������:	SM3
**
**��������:	���ܸչ�����SM3�㷨
**
**����˵��:
		unsigned char *Block			[in /    ] - SM3�ĵ���������
		unsigned int ulBlock			[in /    ] - �����鳤�ȣ��̶�Ϊ64
		unsigned char *bSM3HashValue	[in / out] - SM3ժҪ
**
**��������:  int 
**		
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-10		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASM3(unsigned char *Block, unsigned int ulBlock, unsigned char *bSM3HashValue);

/******************************************************************************
**
**��������:	TestCreateTimeTokencodeRange -- ����ר��
**
**��������:	����һ��ʱ�䷶Χ�ڵĶ�̬����
**
**����˵��:
		PTTOKENRECORDDATA pdata	[in / out] - 
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateTimeTokencodeRange(PTTOKENRECORDDATA pdata);

/******************************************************************************
**
**��������:	TestCreateTimeTokencode  -- ����ר��
**
**��������:	
**
**����˵��:
		void	[in / out] - 
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateTimeTokencode(void);

/******************************************************************************
**
**��������:	TestCreateTimeTokencode_Host  -- ����ר��
**
**��������:	
**
**����˵��:
		void	[in / out] - 
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateTimeTokencode_Host(void);

/******************************************************************************
**
**��������:	TestCreateEventTokencode  -- ����ר��
**
**��������:	
**
**����˵��:
		void	[in / out] - 
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateEventTokencode(void);

/******************************************************************************
**
**��������:	TestCreateChallengeResponsecode -- ����ר��
**
**��������:	
**
**����˵��:
		void	[in / out] - 
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateChallengeResponsecode(void);

/******************************************************************************
**
**��������:	TestCreateTimeTokencodeSig  -- ����ר��
**
**��������:	
**
**����˵��:
		void	[in / out] - 
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateTimeTokencodeSig(void);

/******************************************************************************
**
**��������:	TestCreateTimeTokencodeATM  -- ����ר��
**
**��������:	
**
**����˵��:
		void	[in / out] - 
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateTimeTokencodeATM(void);

/******************************************************************************
**
**��������:	TestCreateEventTokencodeATM  -- ����ר��
**
**��������:	
**
**����˵��:
		void	[in / out] - 
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateEventTokencodeATM(void);

/******************************************************************************
**
**��������:	TestPrintCardInfo  -- ����ר��
**
**��������:	
**
**����˵��:
		char *CardFileName	[in / out] - 
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API void TestPrintCardInfo(char *CardFileName);

/******************************************************************************
**
**��������:	DCSHChangeTokenSeed  -- ����ר��
**
**��������:	
**
**����˵��:
		PTTOKENRECORDDATA pdata	[in / out] - 
**
**��������: int 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2011-1-11		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAChangeTokenSeed(PTTOKENRECORDDATA pdata);


#if OS_SELECT_WINDOWS

/******************************************************************************
**
**��������:	DCSHSACheckShecaCert
**
**��������:	�����Ϻ�CA�Ŀ��������˽ӿڳ�ʼ��eKey������ȡ֤����Ϣ�����������eKey������
**
**����˵��:
		char *cPrikeyPassword	[in /	 ] - ˽Կ����Կ
		char* pCertInfoFile		[in / out] - ֤���һЩ��Ϣ�����浽�ļ�
**
**��������:  int 
**		DCSHEMSHECADLLERR	�Ϻ�CA�Ľӿڲ����ڣ��밴װ�Ϻ�CA��֤�������
**		DCSHEMSHECAKEYERR	eKey����ʧ�ܣ�����eKey�Ƿ�����
**		DCSHEMSUCCESS		�ɹ������Դ�ӡ֤����Ϣ
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-9-2		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckShecaCert (char *cPrikeyPassword, char* pCertInfoFile);

/******************************************************************************
**
**��������:	DCSHSACheckSJK0817E
**
**��������:	��⻪�������ļ��ܿ��Ƿ����������ҽ����˼ӽ������㣬SM1
**
**����˵��:
			[in / out] - 
**
**��������:  int 
**		DCSHEMPCIOPENERR	���ܿ���ʼ��ʧ��
**		DCSHEMPCISESSIONERR	��������ʧ��
**		DCSHEMPCIOPERERR	���ܿ�����ʧ��
**		DCSHEMSUCCESS		�ɹ�
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-9-2		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckSJK0817E ();

/******************************************************************************
**
**��������:	DCSHSAMakePubkeyEncRecord
**
**��������:	�û��Ĺ�Կ���ܻỰ��Կ����
**
**����˵��:
		char *userCertPath			[in /    ] - �û�֤���·��
		unsigned char *pRecordData	[in /    ] - �Ự��Կ��16�ֽ�
		unsigned int RecordDataLen	[in /    ] - ���ݳ��ȣ�16
		unsigned char *pEvpSigData	[in / out] - ���ܵ����ݣ�128�ֽڣ��ⲿ����ռ�
		unsigned int EvpSigDataLen	[in / out] - ���ܵ����ݳ��ȣ�Ӧ����128
**
**��������:  int 
**		EMREADUSERCERTERR	��ȡ�û�֤��ʧ��
**		EMDECUSERCERTERR	�����û�֤��ʧ��
**		EMSUCCESS			�ɹ�
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2008-6-12		�		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAMakePubkeyEncRecord (char *userCertPath, unsigned char *pRecordData, unsigned int RecordDataLen, unsigned char *pEvpSigData, unsigned int *EvpSigDataLen);

/******************************************************************************
**
**��������:	TestRandom
**
**��������:	�����������
**
**����˵��:
		int iNumber	[in / ] - ���� 
**
**��������: DCSHCOREAPI_API int 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-9-7		�		Created
******************************************************************************/
DCSHCOREAPI_API int TestRandom(int iNumber);

/******************************************************************************
**
**��������:	TestSM1
**
**��������:	SM1�㷨����
**
**����˵��:
		int iNumber	[in / ] - ����
**
**��������: DCSHCOREAPI_API int 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-9-7		�		Created
******************************************************************************/
DCSHCOREAPI_API int TestSM1(int iNumber);

/******************************************************************************
**
**��������:	TestSM3
**
**��������:	SM3�㷨����
**
**����˵��:
		int iNumber	[in / ] - ����
**
**��������: DCSHCOREAPI_API int 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2010-9-7		�		Created
******************************************************************************/
DCSHCOREAPI_API int TestSM3(int iNumber);

#endif

/******************************************************************************
**
**��������:	TestTokenCodeNumberIndex
**
**��������:	ʹ��10����ͬ�����ӣ�ÿ�����Ӵ�tokentime=i*60����10000��tokencode��
            ��10000��tokencode��������ͳ�ƣ�����0-9���ֳ��ֵ�Ƶ��
**
**����˵��:
		int	iAlgID [in / ] - �㷨
**
**��������: int 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-1-16		�		Created
******************************************************************************/
DCSHCOREAPI_API int TestTokenCodeNumberIndex(int iAlgID);


/******************************************************************************
**
**��������:	TestTokenCodeDoubleIdx
**
**��������:	�趨5��ʱ��㣬��ÿ��������������ɸ����ӣ���������ʱ����ϼ���tokencode
            �����ɸ�tokencode��������ȶԣ������ظ�����
			�����ɸ���Ϊ���ݲ�������ָ��
**
**����˵��:
		int seedCount	[in / ] - ÿ��ʱ��㣬����seedCount�����ӣ�����seedCount����̬����
**		int	iAlgID		[in / ] - �㷨
**��������: int 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-1-16		�		Created
******************************************************************************/
DCSHCOREAPI_API int TestTokenCodeDoubleIdx(int seedCount, int iAlgID);


/******************************************************************************
**
**��������:	TestTokenCodePrefect
**
**��������:	������֤��̬��������ܣ�1000000����֤
            ֱ�����У�����1��
			ƫ��1������2��
			ƫ��3������6��
			ƫ��5������10��
**
**����˵��:
			[in / out] - 
**
**��������: void 
**		[��ӷ���ֵ˵��]
**		
**		
**����˵��:	
**
**��ʷ:		����		����		��Ϊ
		2009-1-16		�		Created
******************************************************************************/
DCSHCOREAPI_API void TestTokenCodePrefect(int MAXCYCLETEST, int iAlgID);

#ifdef  __cplusplus
}
#endif

#endif




