/***************************************************************************
* 文件:     DCSH_SAE_1.h
*
* 功能说明: 动联动态安全认证引擎1.0头文件
*
* 版权所有：上海动联信息技术有限公司
*
* 作者：    杨波
* 时间：    2008年5月13日
* 地点：    上海 动联
*
***************************************************************************/
/*-------------------------------------------------------------------------
*修改记录*
*修改时间*             *修改内容*
*
-------------------------------------------------------------------------
*备注：
*     此头文件内只允许申明，不可定义
*     此头文件内所有改动均需详细说明
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

//动态认证引擎的系统默认值
#define SYSTEMPIN        "888888"		/* 系统默认的PIN*/	
#define TOTALSECONDS     0x47791200		/* 2008年1月1日00点00时00分，流失的秒*/
#define DCPVTLEN         232			/* 私有数据长度*/
#define SEEDLEN	         16				/* 种子的长度，建议使用128位，经常用于密钥*/
#define EX_SEEDLEN		 4				/* 种子的扩展，支持20字节*/
#define MAXSEEDLEN		 20				/* 系统支持的最大种子长度，建议64，目前使用20*/
#define MAXOFFSETSECONDS 43200			/* (12*60*60)秒， 偏移量超过12小时，无论是否认证通过，应该永久禁用*/
#define ONEDAYSECONDS	 86400			/* (24*60*60)秒， 一天的秒数，用于获得动态口令 */
#define TWOWEEKSECONDS   1209600		/* (14*24*60*60)秒，相距2周未登陆，则窗口自动扩展1，但最大不超过大窗口*/
#define ONEWEEKSECONDS   604800			/* (7*24*60*60)秒，相距1周未登陆，动联需要时间补偿*/
#define MAXRESYNCHTIMES	 1000			//事件型令牌重新同步最大寻找次数
										//事件型令牌在重新同不是分两种方式：
										//1、计数从1开始，到MAXRESYNCHTIMES 中寻找
										//2、从当前最后一次有效计数，到MAXRESYNCHTIMES中寻找
#define SM3HASHSTRINGLENGTH	64			//SM3算法的令牌的明文组装空间，格式参考《时间同步一次性口令认证系统标准0702》
#define DATETIMESTRINGLEN	13			//SM3算法的日期转换，12字节，最末尾第13字节为间隔的商
#define THANDELTATIMES		3			//前一次使用的时间至少是此次时间差的3倍，用于时间补偿
#define	GETTOKENCODESTRHEAD	"%010u:%03u:%03u:%010u:"	//"起始时间:令牌间隔:动态口令长度:动态口令个数:动态口令集合"


/*定义算法*/
#define ALGORITHM_SHA1		0x01 /* 产生20字节摘要 */
#define ALGORITHM_RC6		0x02 /* 块加密，速度快*/
#define ALGORITHM_SM3		0x03 /* SM3，类似SHA1，复旦微电子 */
#define ALGORITHM_AES		0x04 /* 块加密，标准算法*/
#define ALGORITHM_ATM		0x00 /* 支持ATM机方式的挑战应答，带时间*/
#define ALGORITHM_ATM_MD5	0x05 /* 支持ATM机方式的挑战应答，带时间，用户口令使用MD5摘要*/
#define ALGORITHM_ATM_SHA1	0x06 /* 支持ATM机方式的挑战应答，带时间，用户口令使用SHA1摘要*/
#define ALGORITHM_HMAC		0x08 /* OTP，产生20字节摘要 */
#define ALGORITHM_YQ		0x10 /* AES，使用新的截取方式 */
#define ALGORITHM_COS		0x20 /* HMAC，cos的sim卡方式 */
#define ALGORITHM_ASP		0x30 /* 卓望SIM卡令牌，使用HMAC */
#define ALGORITHM_UNI		0x07 /* 联通SIM卡令牌，使用HMAC，但是截取规则不一样 */
#define ALGORITHM_TOTP		0x09 /* 工商银行使用的时间型OTP，OTP的时候等同ALGORITHM_HMAC，挑战则追加时间 */
#define ALGORITHM_SSM3		0x11 /* SM3算法，国密办刚公开，软算法，依然采用原先的规则 */

#define STRLENLOGID      48		/*LoginId的长度*/
#define STRLENAME        24		/*UserName的长度*/
#define GMTSTRLEN        24		/*时间字符的长度 GMT 2008-05-14 04:09:22*/
#define URGENCYPWLEN	 20		/*紧急密码的最大长度*/

#define DCSH_MIN_PIN_LEN     4  /*Pin的最小长度*/
#define DCSH_MAX_PIN_LEN     8  /*Pin的最大长度*/
#define DCSH_PIN_LEN         12 /*Pin的长度空间，限制为4-8*/
#define DCSH_SN_LEN          12 /*序列号最大长度*/
#define DCSH_PRN_LEN         12 /*tokencode最大长度*/
#define DCSH_RESPONSE_LEN	 16 /*用户响应码，可能包含PIN*/
#define DCSH_CHALLENGE_LEN	 16 /*挑战码，建议8字节*/
#define UNLOCKCODELEN		 8  /*解锁码的长度为8个数字*/	
#define HMACQUESTIONLEN		 8	/*挑战应答，HMAC算法的question长度一定为8*/	
#define HOSTBYTELEN			 12	/*取12个字节，使用最后4/6位*/
#define UNICOMMODLEN		 1000 /*取摸，获得3个数字，不足则前端补零 */
#define UNICOMNUMBER		 3    /*每个Short取模后最大数字为3个*/

#define OCRASUITE		"OCRA-1:HOTP-SHA1-6:QN08"   /* 挑战应答的规则 */
#define OCRASUITE_ICBC	"OCRA-1:HOTP-SHA1-6:QA64-T" /* 工商银行挑战应答时间型令牌的规则 */
#define SM3OCRASUITE	"OCRA-1:HOTP-SM3-8:QN08"    /* SM3算法的挑战应答规则 */

//令牌类型
#define DCSH_TOKEN_TYPE_TIME	0 /*时间类型的令牌*/
#define DCSH_TOKEN_TYPE_EVENT	1 /*事件类型的令牌*/
#define DCSH_TOKEN_TYPE_MOBILE	2 /*手机类型的令牌, 动联专用*/

//令牌的应用类型
#define DCSH_HARDWARE_TOKEN_DC		1 /*动联生产的硬件令牌*/ //目前我们在做的，专门特指K5令牌
#define DCSH_HARDWARE_TOKEN_DC_EX	2 /*动联生产的令牌和 其他厂商的扩展*/ 
									  //主要使用了另一种传递参数方式
									  //支持杭州驭强的令牌 --AES
									  //支持cos的SIM卡令牌 --COS
									  //支持复旦微电子的SM3令牌，可能成为行业标准 --SM3
									  //动联自己的令牌
									  //卓望和联通的SIM卡令牌

#define DCSH_HARDWARE_TOKEN_CHR		3 /*异步类型的硬件令牌*/ //挑战应答，RSA有一款
#define DCSH_SOFTWARE_TOKEN_DC		4 /*动联生产的软件令牌*/ //标志位区分，等同于DCSH_HARDWARE_TOKEN_DC_EX
#define DCSH_SOFTWARE_TOKEN_PHONE	5 /*手机令牌          */ //标志位区分，等同于DCSH_HARDWARE_TOKEN_DC_EX
#define DCSH_SOFTWARE_TOKEN_CHR		6 /*异步类型的软件令牌*/ //用于手机异步令牌
#define DCSH_SOFTWARE_TOKEN_SMS		7 /*短信类型的软件令牌*/ //短信令牌
#define DCSH_HARDWARE_TOKEN_COVER	8 /*刮刮卡令牌*/
#define DCSH_HARDWARE_TOKEN_ATM		9 /*属于异步类型的令牌*/ //为银行ATM输入用户密码到令牌上
#define DCSH_SOFTWARE_TOKEN_ATM		10 /*属于异步软件令牌*/ //为银行ATM软件令牌，可用于手机和软件
#define DCSH_HARDWARE_TOKEN_SIM		11 /*卓望和联通的SIM卡令牌也单独列为应用类型，也可属于DC_EX*/
#define DCSH_HARDWARE_TOKEN_GETDC	12 /*K5 该令牌类型支持获取动态口令，建议该类型不用于认证*/
#define DCSH_HARDWARE_TOKEN_GETEX	13 /*K8 该令牌类型支持获取动态口令，建议该类型不用于认证*/


//认证类型
#define DCSH_PRN_TYPE						0 /*直接使用tokencode认证*/
#define DCSH_PASSCODE_TYPE					1 /*带PIN的tokencode认证 */
#define DCSH_CHALLENGE_RESPONSE_TYPE		2 /*挑战响应的认证方式 */
#define DCSH_PASSCODE_NEXT_TYPE				3 /*需要返回下一个tokencode的认证方式*/
#define DCSH_SMS_GETPRN_TYPE				4 /*短消息认证，获得tokencode*/
#define DCSH_SMS_PRN_TYPE					5 /*短消息认证，认证tokencode*/
#define DCSH_MSCHAP2_PASSCODE_TYPE			6 /*使用MS-Chap v2的认证方式，由引擎计算tokencode来验证*/
#define DCSH_MSCHAP2_CHANGE_PW_TYPE			7 /*修改令牌的PIN码*/
#define DCSH_SIGN_PRN_TYPE					8 /*数字签名的动态认证方式*/
#define DCSH_GET_SIGN_PRN_TYPE				9 /*获得签名口令，可用于短信数字签名方式*/
#define DCSH_COVER_GET_HOSTCODE_TYPE		10 /*获得刮刮卡的当前计数的主机码*/
#define DCSH_COVER_HOST_PRN_TYPE			11 /*刮刮卡码认证，包含主机字符 动态口令 PIN*/
#define DCSH_CHALLENGE_RESPONSE_ATM_TYPE	12 /*ATM用户密码的挑战应答认证模式，用户密码支持摘要算法 */

//认证状态，认证模式
#define DCSH_LOGON_STATE_FIRST_LOGIN         1
#define DCSH_LOGON_STATE_NORMAL              2
#define DCSH_LOGON_STATE_NEXT_TOKENCODE      3
#define DCSH_LOGON_STATE_NEXT_TOKENCODE_2    4

#define DCSH_MAX_PINTIMES	3    /*最大Pin输入错误次数*/
#define DCSH_MAX_THRESOLD	10   /*最大tokencode错误次数，超过进入next模式*/
#define DCSH_MAX_REPEATED	5    /*允许重放次数，超过则禁用*/

#define ICBC_ACTIVECHECKCODELEN	12	/* 工行令牌种子激活码的长度12，前6激活，后6校验 */
#define ICBC_ACTIVECODELEN		6	/* 工行令牌种子真正激活码6 */
#define ICBC_ACTIVECODECALCLEN	8	/* 工行令牌种子真正激活码6带入运算时的真实长度8 */
#define ICBC_CHECKCODELEN		6	/* 工行令牌种子真正校验码长度6 */
#define ICBC_PINLOCKCODELEN		8	/* 工行令牌PIN输入错误锁定后锁定码长度 */
#define ICBC_PINUNLOCKCODELEN	6	/* 工行令牌解锁码长度 */


#define		DCSHEMSUCCESS			0 //验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
#define     DCSHEMFIRST				1 //第一次使用令牌登陆，成功，等同于EMSUCCESS
#define		DCSHEMBADARG			2 //参数错误，参数可能为空指针
#define		DCSHEMBADREC			3 //严重错误！PDATA内部数据有问题，如签名等
#define		DCSHEMPRNLEN			4 //Tokencode的长度错误
#define		DCSHEMTIMRNG			5 //passcode时间错误，不在生命期限内
#define		DCSHEMNOTASSIGNED		6 //令牌还没有分配
#define     DCSHEMDISABLE			7 //令牌处于禁用状态
#define		DCSHEMFOREVERDISABLE	8 //令牌被永久禁用，可能是时间偏移超过12小时
#define		DCSHEMNOPIN				9 //没有输入pin码
#define		DCSHEMPINLEN			10 //输入的PIN长度错误
#define		DCSHEMBADPIN			11 //输入错误的PIN
#define		DCSHEMSTOLEN			12 //因为PIN错误次数到达极限，默认为丢失
#define		DCSHEMOUTWIND			13 //在任何窗口内找不到tokencode，可能需要再同步
#define		DCSHEMREPPRN			14 //这是重放，需要小心，应该立即记录，并提醒管理员
#define		DCSHEMREPDISABLE		15 //重放到一定的次数则禁用
#define		DCSHEMNEXTPRN			16 //进入Next的模式
#define		DCSHEMTHRESHOLDNXT		17 //输入tokencode的错误次数到达极限，以后进入NEXT模式
#define		DCSHEMBADNEX			18 //在Next模式下输入错了tokencode
#define		DCSHEMNEXTDISABLE		19 //在Next模式下输入错误次数到达极限，被禁用
#define		DCSHEMSYN1OK			20 //重新同步模式下，第一次匹配tokencode已经找到
#define     DCSHEMFIRSTDISABLE      21 //在First模式下，多次输入失败，则禁用
#define 	DCSHEMBADNEX2           22 //在Next2下输入错误的tokencode
#define 	DCSHEMBADINPUT          23 //文件无法打开
#define 	DCSHEMNOMEM             24 //读取BIN文件，没有足够的内存空间
#define 	DCSHEMBADINPFIL         25 //BIN的文件内容错误，严重错误
#define		DCSHEMBADSIG            26 //BIN文件签名错误，严重错误
#define 	DCSHEMEMPNAME           27 //文件名称错误
#define		DCSHEMKEYPAIRERR		28 //密钥对创建失败
#define		DCSHEMCERTERR			29 //根证书创建失败
#define		DCSHEMSEEDERR			30 //创建种子失败
#define		DCSHEMREADPUBKEYERR		31 //读取公钥失败
#define		DCSHEMPUBKEYENCERR		32 //公钥加密失败
#define		DCSHEMREADSEEDERR		33 //读根种子文件失败
#define		DCSHEMREADPRIKEYERR		34 //读根私钥失败
#define		DCSHEMPRIKEYDECERR		35 //根私钥解密失败
#define		DCSHEMREADPKCS10ERR		36 //读取PKCS#10文件错误
#define		DCSHEMTOKENTYPE			37 //错误的令牌应用类型
#define		DCSHEMREADROOTCERTERR	38 //读取根证书失败
#define		DCSHEMDECROOTCERTERR	39 //解析根证书失败
#define		DCSHEMDECPKCS10ERR		40 //解析PKCS#10失败
#define		DCSHEMUSERCERTERR		41 //创建用户证书失败
#define		DCSHEMVERSIGDATAERR		42 //验证签名错误，严重错误
#define		DCSHEMDECEVPDATAERR		43 //解密数字信封错误，严重错误
#define		DCSHEMREADUSERCERTERR	44 //读取用户证书失败
#define		DCSHEMDECUSERCERTERR	45 //解析用户证书失败
#define		DCSHEMEVPDATAERR		46 //制作数字信封失败
#define		DCSHEMSIGDATAERR		47 //制作数字签名失败
#define		DCSHEMREADSEEDFILEERR	48 //读取种子文件失败
#define		DCSHEMPKCS10ERR			49 //创建PKCS#10失败
#define		DCSHEMBADFILETYPE		50 //错误的种子文件类型
#define		DCSHEMRESPONSEERR		51 //挑战认证验证失败
#define		DCSHEMBADTYPE			52 //错误的令牌类型
#define		DCSHEMSNASSIGNED		53 //指定的序列号已经分配，且已经使用
#define		DCSHEMBADURGENCYPW		54 //紧急密码错误
#define		DCSHEMSMSBADPRN			55 //短消息认证，错误的动态口令
#define		DCSHEMSMSTIMEOUT		56 //口令正确，但已经超时
#define		DCSHEMBADGETURGENCYPW	57 //获取紧急密码失败
#define		DCSHEMPKI128			58 //CryptoAPI在公钥加密时偶尔会丢失1字节
#define		DCSHEMLICDEVSNERR		59 //授权，版权控制时设备序列号错误
#define		DCSHEMLICDATEEND		60 //授权，授权文件设定的有效期结束
#define		DCSHEMLICEKEYERR		61 //授权，eKey内部的版权信息错误，严重
#define		DCSHEMLICUSBERROR		62 //授权，usb读eKey错误，设备原因，严重
#define		DCSHEMMSCHAPV2ERROR		63 //ms chap v2 认证，客户端提交nt_response错误
#define		DCSHEMMSCHAPV2ERRORPW	64 //ms chap v2 修改口令，口令存在异常
#define		DCSHEMDEVSNERR			65 //授权，eKey序列号不匹配
#define		DCSHEMDEVFILEERROR		66 //ekey内部文件不存在
#define		DCSHEMFORCESYNTYPE		67 //强制同步的类型错误，应该是只能是事件型
#define		DCSHEMNOTCOVERTYPE		68 //不是刮刮卡令牌类型，不能生成卡片
#define		DCSHEMOVERCOVERNUM		69 //种子生成卡片口令的数量大于最大配置
#define		DCSHEMLASTCOVERNUM		70 //最后一个覆盖区不用于认证，而是用于换卡
#define		DCSHEMCOVERNOTUSED		71 //当前的覆盖区还没有用于认证
#define		DCSHEMMUSTGETCOVER		72 //刮刮卡认证必须先获得主机字符
#define		DCSHEMHOSTCODEERRO		73 //主机字符错误
#define		DCSHEMCOVERCODEERR		74 //覆盖区域的动态口令错误
#define		DCSHEMERRORTODISABLE	75 //多次认证错误，导致令牌被禁用
#define		DCSHEMUPDATECODEERR		76 //刮刮卡更换码错误，即最后一个刮刮卡码错误
#define		DCSHEMBASE64CODEERR		77 //base64编解码错误
#define		DCSHEMURGPWDERR			78 //更改服务码失败
#define		DCSHEMMODIFYSNERR		79 //修改序列号时，序列号输入错误
#define		DCSHEMPCIOPENERR		80 //打开PCI SJK0817E加密卡失败
#define		DCSHEMPCISESSIONERR		81 //创建会话失败，可能PCI中断，也可能达到最大会话数
#define		DCSHEMPCISM3ERR			82 //SM3算法内部错误
#define		DCSHEMNOSUPPORT			83 //该系统不支持该算法，目前SM3仅用于windows
#define		DCSHEMFORCECOUNTERLESS	84 //客户端强制同步服务端计数器时，计数器小于服务器已经使用的
#define		DCSHEMSHECADLLERR		85 //加载sheca的开发接口失败
#define		DCSHEMSHECAKEYERR		86 //驱动sheca的usb-eKey操作异常
#define		DCSHEMPCIOPERERR		87 //加密卡运算操作失败
#define		DCSHEMICBCVALUEERR		88 //工行进行种子异或的数据交验失败
#define		DCSHEMICBCHAVEXOR		89 //工行的种子数据已经异或得到了新的种子数据
#define		DCSHEMSEEDLENERR		90 //种子长度错误，可能是工行的种子有问题
#define		DCSHEMACTIVELENERR		91 //激活码长度错误，6数字，运算长度为8，补0x00,0x00
#define		DCSHEMCHECKLENERR		92 //校验码长度错误，6数字
#define		DCSHEMACTIVCHECKERR		93 //激活校验码错误，12数字
#define		DCSHEMBIGTIMELENERR		94 //运算的时间，大头自序，长度错误
#define		DCSHEMCHANLENERR		95 //挑战的字符串长度错误，工行必须小于64
#define		DCSHENUNLOCKPINERR		96 //令牌解锁码的字符串长度错误，8数字
#define		DCSHENOUTONEDAY			97 //获取动态口令，时间型，超过了24小时
#define		DCSHENOUTMAXCOUNTER		98 //获取动态口令，事件型，超过了1000个


typedef struct CARDREC
{
	char ucSerialNumber[DCSH_SN_LEN];		/* Token序列号字符表示      */
	char ucUrgencyPassWord[URGENCYPWLEN];	/* 紧急密码					*/
	char ucSeedNumber[32];					/* 种子转换的26个数字		*/
} TOKENCARDRECORD, *PTTOKENCARDRECORD;


typedef struct DCSHPDATA {
    unsigned int ulSerialNumber;     /* Token序列号              */
    char ucSerialNumber[DCSH_SN_LEN];/* Token序列号字符表示      */
    unsigned int ulPRNLen;			 /* Tokencode的长度          */
    unsigned int ulInterval;		 /* 时间间隔，产生tokencode  */
	unsigned int ulBirth;			 /* 出生日期，从 1/1/2008    */
    unsigned int ulDeath;			 /* 有效期，秒，从 1/1/2008  */
    char cTokenType;				 /* Token类型                */	
    char cIsHardwareToken;			 /* 是不是硬件               */
	char cAlgorithmSData;			 /* 算法，私有数据的加密算法 */
	char cAlgorithmToken;			 /* 算法，tokencode的产生算法*/	
	
	int  iState;					 /* 状态值，这个非常重要的字段*/
	char cEnabled;					 /* 是否可用，第一判断要素    */
    char cAssigned;					 /* 是否分配，第一判断要素    */
    char cNextPRNMode;				 /* NextToekn模式             */
    char cPINDisabled;				 /* PIN是否使用               */
    
    unsigned int ulLastlogin;		 /* 最近登陆时间,无论成功失败，秒，从 1/1/2008 */
    
	char ucUserLoginID[STRLENLOGID]; /* */
    char ucUserFirstName[STRLENAME]; /* */
    char ucUserLastName[STRLENAME];  /* */	
	
    int iMaxBadPins;                 /* Pin优于tokencode认证，每次登陆成功前最大错误次数，超过则disable */
    int iNextTokencodeModeThreshold; /* tokenode错误的极限值，超过该值，则进入Next Token模式 */
	int iMaxRepeateds;               /* 监测重放攻击极限值，反复提交先前的tokencode，或者同一时间提交tokencode则理解为重放，达到极限值则禁用 */
	int iMaxBadNextTokencodes;		 /* 在Next模式下，允许最大错误次数 */
	
    int iPrecision;                  /* 精度，离ulLastSuccessLogin每隔一个月累加精度秒 */
    
    int iLargeWindowSeconds;		 /* 秒 */
    int iMediumWindowSeconds;		 /* 秒 */
    int iSmallWindowSeconds;		 /* 秒 */

	char ucTokenTypeString[8];		 /* 令牌的类型描述 */
	int iSeedLen;					 /* 种子长度，如果是0，则默认为16 */
	char ucReserved[4];              /* 保留字段 */
    unsigned char ucPrivateData[DCPVTLEN]; /* 加密数据-种子 pin等 */ 
} TOKENRECORDDATA, *PTTOKENRECORDDATA; /* 424字节  Base64为568字节*/

typedef struct DCSHSAE_TOKENRECORD_INFO {
    unsigned int ulSerialNumber;     /* Token序列号              */
    char ucSerialNumber[DCSH_SN_LEN];/* Token序列号字符表示      */
    unsigned int ulPRNLen;			 /* Tokencode的长度          */
    unsigned int ulInterval;		 /* 时间间隔，产生tokencode  */
	unsigned int ulBirth;			 /* 出生日期，从 1/1/2008    */
    unsigned int ulDeath;			 /* 有效期，秒，从 1/1/2008  */
    char cTokenType;				 /* Token类型                */	
    char cIsHardwareToken;			 /* 是不是硬件               */
	char cAlgorithmSData;			 /* 算法，私有数据的加密算法 */
	char cAlgorithmToken;			 /* 算法，tokencode的产生算法*/	
	
	int  iState;					 /* 状态值，这个非常重要的字段*/
	char cEnabled;					 /* 是否可用，第一判断要素    */
    char cAssigned;					 /* 是否分配，第一判断要素    */
    char cNextPRNMode;				 /* NextToekn模式             */
    char cPINDisabled;				 /* PIN是否使用               */
    
    unsigned int ulLastlogin;		 /* 最近登陆时间,无论成功失败，秒，从 1/1/2008 */
    
	char ucUserLoginID[STRLENLOGID]; /* */
    char ucUserFirstName[STRLENAME]; /* */
    char ucUserLastName[STRLENAME];  /* */	
	
    int iMaxBadPins;                 /* Pin优于tokencode认证，每次登陆成功前最大错误次数，超过则disable */
    int iNextTokencodeModeThreshold; /* tokenode错误的极限值，超过该值，则进入Next Token模式 */
	int iMaxRepeateds;               /* 监测重放攻击极限值，反复提交先前的tokencode，或者同一时间提交tokencode则理解为重放，达到极限值则禁用 */
	int iMaxBadNextTokencodes;		 /* 在Next模式下，允许最大错误次数 */
	
    int iPrecision;                  /* 精度，离ulLastSuccessLogin每隔一个月累加精度秒 */
    
    int iLargeWindowSeconds;		 /* 秒 */
    int iMediumWindowSeconds;		 /* 秒 */
    int iSmallWindowSeconds;		 /* 秒 */

	char ucTokenTypeString[8];		 /* 令牌的类型描述 */
	int iSeedLen;					 /* 种子长度，如果是0，则默认为16 */
	char ucReserved[4];              /* 保留字段 */
} TOKENPUBLICINFO, *PTTOKENPUBLICINFO;     /* 192字节  */

typedef struct {
        LONG type;					/* PASSCODE_TYPE/PASSCODE_NEXT_TYPE */
		ULONG passc_time;			/* passcodetime */
        char prn[DCSH_PRN_LEN];		/* tokencode */
        char pin[DCSH_PIN_LEN];		/* pin码 */
        char nextPrn[DCSH_PRN_LEN];	/* PASSCODE_NEXT_TYPE输出，下一个tokencode */
} DCSHPASSCODE;

typedef struct {
        LONG type;					/* PRN_TYPE */
        ULONG passc_time;			/* passcodetime */
        char prn[DCSH_PRN_LEN];		/* tokencode */
} DCSHPRN;

typedef struct {
        LONG type;						/* CHALLENGE_RESPONSE_TYPE */
        ULONG passc_time;				/* passcodetime */
        char challenge[DCSH_CHALLENGE_LEN];  /* 挑战码 */
        char response[DCSH_RESPONSE_LEN];    /* 响应码 */
} DCSHCHALLENGE_RESPONSE;

typedef struct {
        LONG type;						/* CHALLENGE_RESPONSE_ATM_TYPE */
        ULONG passc_time;				/* passcodetime */
		int  iPasswdAlg;				/* 用户输入密码的摘要算法，0表示直接使用*/
        char challenge[DCSH_CHALLENGE_LEN];  /* 挑战码 */
        char response[DCSH_RESPONSE_LEN];    /* 响应码 */
} DCSHCHALLENGE_RESPONSE_ATM;

typedef struct {
        LONG type;					/* DCSH_SMS_GETPRN_TYPE/DCSH_SMS_PRN_TYPE */
        ULONG valid_time;			/* 有效时间，秒 */
        char prn[DCSH_PRN_LEN];		/* tokencode，返回/验证 */
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
	char key1[20];				/* 第一个关键信息，建议6个数字  */
	char key2[20];				/* 第二个关键信息，建议6个数字  */
	char key3[20];				/* 第三个关键信息，建议6个数字  */
	char prn[DCSH_PRN_LEN];		/* DCSH_SIGN_PRN_TYPE 为认证数字签名的动态口令*/
								/* DCSH_GET_SIGN_PRN_TYPE 为获得签名动态口令*/
	char pin[DCSH_PIN_LEN];		/* pin码	        */
}  DCSH_SIGN_PASSCODE; //数字签名的认证类型

typedef struct
{
	LONG type;						/*DCSH_COVER_GET_HOSTCODE_TYPE / DCSH_COVER_HOST_PRN_TYPE*/
	ULONG passc_time;				/*系统时间*/
	char cHostCode[DCSH_PRN_LEN];	/*主机字符*/ //认证时提交，获得时返回
	char cTokenCode[DCSH_PRN_LEN];	/*动态口令*/ //获得时实用
	char pin[DCSH_PRN_LEN];			/*pin码*/	 //认证时使用
	int  iCurrentCount;				/*当前计数器*///获得时使用返回
} DCSH_COVER_PASSCODE; //刮刮卡认证类型

/*联合类型，用于上述类型的认证*/
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
**函数名称:	DCSHSAGetPasscodeTime
**
**功能描述:	根据系统时间取得当前令牌需要的时间UTC,相对于2008/1/1的秒数
**
**参数说明:
		void	[] - 
**
**返回类型: LONG 
**		返回整型，秒
**		
**		
**其他说明:	在进行引擎认证时，必须调用此函数提供的时间
**
**历史:		日期		作者		行为
		2008-5-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API LONG DCSHSAGetPasscodeTime (void);

/******************************************************************************
**
**函数名称:	DCSHSAGetPasscodeTimeOTP
**
**功能描述:	获取系统的秒，相对于1970/1/1
**
**参数说明:
		ULONG PasscodeTime	[in /   ] - DCSHSAGetPasscodeTime 获得的时间
**
**返回类型:  LONG 
**		返回整型，秒
**		
**		
**其他说明:	在进行OTP认证时，调用该接口获得时间：秒
**
**历史:		日期		作者		行为
		2011-1-7		杨波		Created
******************************************************************************/
DCSHCOREAPI_API LONG DCSHSAGetPasscodeTimeOTP (ULONG PasscodeTime);

/******************************************************************************
**
**函数名称:	DCSHSAFmtTime
**
**功能描述:	把认证引擎使用的时间转换为字符串 "GMT 2008-05-14 04:09:22"
**
**参数说明:
		LONG tod		[in /    ] - 引擎时间，秒
		char *outstr	[in / out] - 输出字符串，外部分配空间，长度为 GMTSTRLEN
**
**返回类型: int 
**		EMSUCCESS 成功转换
**		EMBADARG  参数错误，可能outstr为空指针
**		
**其他说明:	时间为GMT时间，并不是UTC时间，注意时区
**
**历史:		日期		作者		行为
		2008-5-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAFmtTime (LONG tod, char *outstr);


/******************************************************************************
**
**函数名称:	DCSHSASetUserLogin
**
**功能描述:	分配令牌给用户,令牌的第一个操作应该是分配
**			分配用户时将重置令牌PIN码，但是令牌自身的状态位和计数器不改变
**			可以反复分配
**
**参数说明:
		PTTOKENDATA pdata	[in / out] - 引擎数据包，不得修改
		char *name			[in /    ] - 用户名称
**
**返回类型: int 
**		EMSUCCESS 成功
**		EMBADARG  参数错误，可能为空指针，也可能name的长度大于STRLENLOGID
**		EMBADREC  PDATA内部数据有问题，如签名等
**		EMFOREVERDISABLE 永久禁用
**
**其他说明:	
**
**历史:		日期		作者		行为
		2008-5-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetUserLogin(PTTOKENRECORDDATA pdata ,char *name);


/******************************************************************************
**
**函数名称:	DCSHSASetTokenCodeLength
**
**功能描述:	重新设置动态口令的长度，最大不超过DCSH_PRN_LEN，最小没有限制
**			因为RFC取Long型，最大值以4字节表示，所以实际长度不可能超过10个数字
**
**参数说明:
		PTTOKENRECORDDATA pdata	[in / out] - pdata
		int iLen				[in /    ] - 新的长度，应该不大于DCSH_PRN_LEN
**
**返回类型:  int 
**		EMSUCCESS 成功
**		EMBADARG  参数错误，可能为空指针，也可能iLen大于DCSH_PRN_LEN
**		EMBADREC  PDATA内部数据有问题，如签名等
**		EMFOREVERDISABLE 永久禁用	
**		
**		
**其他说明:	建议在 4 - 8之间
**
**历史:		日期		作者		行为
		2009-8-18		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetTokenCodeLength(PTTOKENRECORDDATA pdata , int iLen);


/******************************************************************************
**
**函数名称:	DCSHSAEnableToken
**
**功能描述:	让已经分配的令牌启用，同时清除计数器和Next模式，必须在令牌分配之后才能启用
**			启用操作将清除所有状态位和计数器，但是不清除重放和最后认证时间
**
**参数说明:
		PTTOKENDATA pdata	[in / out] - 引擎数据包
		char *pin			[in /    ] - pin，如果是NULL或者长度为0，则强制设置SYSTEMPIN
**
**返回类型: int 
**		EMSUCCESS 设置可用成功
**		EMBADARG  参数错误，pdata可能为空指针，也可能pin长度大于MAX_PIN_LEN小于MIN_PIN_LEN
**		EMBADREC  PDATA内部数据有问题，如签名等
**		EMNOTASSIGNED 还没有分配用户
**		EMPINLEN  pin的长度有问题，应该 4<=pinlen<=8
**		EMFOREVERDISABLE 永久禁用
**
**其他说明:	调用该函数前应该先调用DCSHSASetUserLogin
**
**历史:		日期		作者		行为
		2008-5-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAEnableToken (PTTOKENRECORDDATA pdata, char *pin);

/******************************************************************************
**
**函数名称:	DCSHSADisableToken
**
**功能描述:	禁用一个令牌
**
**参数说明:
		PTTOKENDATA pdata	[in / out] - 引擎数据包
**
**返回类型: int 
**		EMSUCCESS 设置禁用成功
**		EMBADARG  参数错误，pdata可能为空指针
**		EMBADREC  PDATA内部数据有问题，如签名等
**		EMDISABLE 已经设置为禁用了
**		EMFOREVERDISABLE 永久禁用
**		
**		
**其他说明:	任何时候禁用令牌应该调用该函数，而不得胡乱修改
**
**历史:		日期		作者		行为
		2008-5-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSADisableToken (PTTOKENRECORDDATA pdata);

/******************************************************************************
**
**函数名称:	DCSHSASetPinMaxErrors
**
**功能描述:	设置或更改PIN错误的最大次数，设置时不考虑状态位
**
**参数说明:
		PTTOKENDATA pdata	[in / out] - 引擎数据包
		int		iPinErrs	[in /    ] - 最大错误次数
**
**返回类型: int 
**		EMSUCCESS 设置pin成功
**		EMBADARG  参数错误，pdata可能为空指针
**		
**其他说明:	在禁用之前修改最大值有作用，如果禁用后，也可以修改，但在启用后起作用
**
**历史:		日期		作者		行为
		2008-5-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetPinMaxErrors (PTTOKENRECORDDATA pdata, int iPinErrs);


/******************************************************************************
**
**函数名称:	DCSHSASetPin
**
**功能描述:	给一个已分配可用的令牌设置PIN，同时清除所有计数器和Next模式，不清除重放和时间
**			注意，也不修改状态位，被禁用的将依然禁用
**
**参数说明:
		PTTOKENDATA pdata	[in / out] - 引擎数据包
		char *pin			[in /    ] - pin，如果是NULL或者长度为0，则强制设置SYSTEMPIN
**
**返回类型: int 
**		EMSUCCESS 设置pin成功
**		EMBADARG  参数错误，pdata可能为空指针
**		EMBADREC  PDATA内部数据有问题，如签名等
**		EMDISABLE 为禁用状态
**		EMPINLEN  pin的长度有问题，应该 4<=pinlen<=8
**		EMFOREVERDISABLE 永久禁用
**		
**其他说明:	在成功登陆后，应提供用户修改PIN的地方
**
**历史:		日期		作者		行为
		2008-5-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetPin (PTTOKENRECORDDATA pdata, char *pin);


/******************************************************************************
**
**函数名称:	DCSHSASetPinDisabledFlag
**
**功能描述:	设置是否使用PIN的标志位，1 禁用 0 启用
**			未启用的令牌不能设置
**
**参数说明:
		PTTOKENDATA pdata	[in / out] - 引擎数据包
		int b				[in /    ] - 1 禁用PIN认证，0 使用PIN认证 
**
**返回类型: int 
**		EMSUCCESS 设置Flag成功
**		EMBADARG  参数错误，pdata可能为空指针
**		EMBADREC  PDATA内部数据有问题，如签名等
**		EMDISABLE 为禁用状态，不可设置
**		EMFOREVERDISABLE 永久禁用
**		
**		
**其他说明:	建议使用PIN认证
**
**历史:		日期		作者		行为
		2008-5-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetPinDisabledFlag (PTTOKENRECORDDATA pdata,int b);


/******************************************************************************
**
**函数名称:	DCSHSASetNextTokencodeModeThreshold
**
**功能描述:	设置tokencode错误最大值，达到这个数后，则进入next模式
**			未启用的令牌不可设置
**
**参数说明:
		PTTOKENDATA pdata	[in / out] - 引擎数据包
		int index			[in /    ] - 最大tokencode错误极限值
**
**返回类型: int 
**		EMSUCCESS 设置Threshol成功
**		EMBADARG  参数错误，pdata可能为空指针，或者index错了(1-10之间)
**		EMDISABLE 为禁用状态，不可设置
**		
**		
**其他说明:	现在放开了最大10的限制，可设置任意大
**
**历史:		日期		作者		行为
		2008-5-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetNextTokencodeModeThreshold (PTTOKENRECORDDATA pdata, int index);


/******************************************************************************
**
**函数名称:	DCSHSAGetTokenInfo
**
**功能描述:	获取令牌的详细信息
**
**参数说明:
		PTTOKENDATA pdata						[in /    ] - 引擎数据包 
		PTTOKENSECRETDATA tokenInfo	            [in / out] - 详细信息，外面分配空间
**
**返回类型: int 
**		EMSUCCESS 设置Threshol成功
**		EMBADARG  参数错误，pdata可能为空指针
**		EMBADREC  PDATA内部数据有问题，如签名等
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2008-5-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetTokenInfo (PTTOKENRECORDDATA pdata, PTTOKENPUBLICINFO tokenInfo);


/******************************************************************************
**
**函数名称:	DCSHSAUpdateTokenSeed
**
**功能描述:	认证引擎内部创建种子，并更新pdata内的原有种子，注意，这将导致种子文件失效
			更新种子后的令牌将作为全新的令牌，走Firt模式
**
**参数说明:
		PTTOKENRECORDDATA pdata	[in / out] - pdata数据结构
		int iBitLen		        [in /    ] - 种子的位长，128或者160
		char *cSeed				[in / out] - 种子字符串，base64字符串，注意"==",真实长度为cSeedLen
		int *cSeedLen			[in / out] - base64种子的字符串长度
**
**返回类型:  int 
**		EMSUCCESS 设置种子成功
**		EMBADARG  参数错误，pdata可能为空指针		
**		
**		
**其他说明:	动联的种子生成有严格的规则，涉及到算法和加密卡；
			同时pdata结构和序列号/种子有密切关联，用于防伪；
			更新种子将是的严格的规则和防伪校验失效；
			另，使用者需要注意，一旦更新后，将导致种子文件BIN失效；
**
**历史:		日期		作者		行为
		2010-2-4		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAUpdateTokenSeed (PTTOKENRECORDDATA pdata, int iBitLen, char *cSeed, int *cSeedLen);


/******************************************************************************
**
**函数名称:	DCSHSACheckPasscode
**
**功能描述:	动态口令认证
**
**参数说明:
		LONG passc_time		[in /    ] - DCSHSAGetPasscodeTime取得时间
		PTTOKENDATA pdata	[in / out] - 引擎数据包
		char *prn			[in /    ] - tokencode
		char *pin			[in /    ] - pin
**
**返回类型: int 
**		EMSUCCESS			验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
**		EMBADARG			参数错误，参数可能为空指针
**		EMBADREC			严重错误！PDATA内部数据有问题，如签名等
**      EMFIRST				第一次使用令牌登陆，成功，等同于EMSUCCESS

**		EMPRNLEN			Tokencode的长度错误
**		EMTIMRNG			passcode时间错误，不在生命期限内
**		EMNOTASSIGNED		令牌还没有分配
**      EMDISABLE			令牌处于禁用状态
**		EMFOREVERDISABLE	令牌被永久禁用，可能是时间偏移超过12小时
**		EMNOPIN				没有输入pin码
**		EMPINLEN			输入的PIN长度错误
**		EMBADPIN			输入错误的PIN
**		EMSTOLEN			因为PIN错误次数到达极限，默认为丢失
**		EMOUTWIND			在任何窗口内找不到tokencode，可能需要再同步
**		EMREPPRN			这是重放，需要小心，应该立即记录，并提醒管理员
**		EMREPDISABLE		重放到一定的次数则禁用
**		EMNEXTPRN			进入Next的模式
**		EMTHRESHOLDNXT		输入tokencode的错误次数到达极限，以后进入NEXT模式
**		EMBADNEX			在Next模式下输入错了tokencode
**		EMNEXTDISABLE		在Next模式下输入错误次数到达极限，被禁用
**		EMBADTYPE			错误的令牌类型
**		
**		
**其他说明:	最重要的认证接口，特别需要注意返回值
**
**历史:		日期		作者		行为
		2008-5-15		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckPasscode (ULONG passc_time, PTTOKENRECORDDATA pdata, char *prn, char *pin);


/******************************************************************************
**
**函数名称:	DCSHSACheckATMTokenCode
**
**功能描述:	ATM令牌的认证接口，需要注意用户密码的转换
**
**参数说明:
		ULONG passc_time						[in /    ] - 时间
		PTTOKENRECORDDATA pdata					[in / out] - pdata
		int iPasswdAlg							[in /    ] - 对用户口令的算法
		char *atmpassword						[in /    ] - 用户口令，也可能是摘要数据
		char *prn								[in /    ] - 动态口令
**
**返回类型:  int 
**		EMSUCCESS			验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
**		EMBADARG			参数错误，参数可能为空指针
**		EMBADREC			严重错误！PDATA内部数据有问题，如签名等
**      EMFIRST				第一次使用令牌登陆，成功，等同于EMSUCCESS

**		EMPRNLEN			Tokencode的长度错误
**		EMTIMRNG			passcode时间错误，不在生命期限内
**		EMNOTASSIGNED		令牌还没有分配
**      EMDISABLE			令牌处于禁用状态
**		EMFOREVERDISABLE	令牌被永久禁用，可能是时间偏移超过12小时
**		EMNOPIN				没有输入pin码
**		EMPINLEN			输入的PIN长度错误
**		EMBADPIN			输入错误的PIN
**		EMSTOLEN			因为PIN错误次数到达极限，默认为丢失
**		EMOUTWIND			在任何窗口内找不到tokencode，可能需要再同步
**		EMREPPRN			这是重放，需要小心，应该立即记录，并提醒管理员
**		EMREPDISABLE		重放到一定的次数则禁用
**		EMNEXTPRN			进入Next的模式
**		EMTHRESHOLDNXT		输入tokencode的错误次数到达极限，以后进入NEXT模式
**		EMBADNEX			在Next模式下输入错了tokencode
**		EMNEXTDISABLE		在Next模式下输入错误次数到达极限，被禁用
**		EMBADTYPE			错误的令牌类型
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-12-7		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckATMTokenCode (ULONG passc_time, PTTOKENRECORDDATA pdata, int iPasswdAlg, char *atmpassword /*用户密码作为challenge*/, char *prn);


/******************************************************************************
**
**函数名称:	DCSHSACheckSIGPasscode
**
**功能描述:	签名动态口令认证，外带三个关键信息，必须为数字
**
**参数说明:
		ULONG passc_time		[in /    ] - 当前系统时间
		PTTOKENRECORDDATA pdata	[in / out] - pdata
		char *key1				[in /    ] - 关键信息1，建议6个数字
		char *key2				[in /    ] - 关键信息2，建议6个数字
		char *key3				[in /    ] - 关键信息3，建议6个数字
		char *prn				[in /    ] - 动态口令
		char *pin				[in /    ] - 令牌PIN码
**
**
**返回类型: int 
**		EMSUCCESS			验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
**		EMBADARG			参数错误，参数可能为空指针
**		EMBADREC			严重错误！PDATA内部数据有问题，如签名等
**      EMFIRST				第一次使用令牌登陆，成功，等同于EMSUCCESS

**		EMPRNLEN			Tokencode的长度错误
**		EMTIMRNG			passcode时间错误，不在生命期限内
**		EMNOTASSIGNED		令牌还没有分配
**      EMDISABLE			令牌处于禁用状态
**		EMFOREVERDISABLE	令牌被永久禁用，可能是时间偏移超过12小时
**		EMNOPIN				没有输入pin码
**		EMPINLEN			输入的PIN长度错误
**		EMBADPIN			输入错误的PIN
**		EMSTOLEN			因为PIN错误次数到达极限，默认为丢失
**		EMOUTWIND			在任何窗口内找不到tokencode，可能需要再同步
**		EMREPPRN			这是重放，需要小心，应该立即记录，并提醒管理员
**		EMREPDISABLE		重放到一定的次数则禁用
**		EMNEXTPRN			进入Next的模式
**		EMTHRESHOLDNXT		输入tokencode的错误次数到达极限，以后进入NEXT模式
**		EMBADNEX			在Next模式下输入错了tokencode
**		EMNEXTDISABLE		在Next模式下输入错误次数到达极限，被禁用
**		EMBADTYPE			错误的令牌类型
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-2-12		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckSIGPasscode (ULONG passc_time, PTTOKENRECORDDATA pdata, char *key1, char *key2, char *key3, char *prn, char *pin);

/******************************************************************************
**
**函数名称:	DCSHSAGetSIGPasscode
**
**功能描述:	获取签名动态口令，内部使用
**
**参数说明:
		ULONG passc_time		[in /    ] - 系统时间
		PTTOKENRECORDDATA pdata	[in /    ] - pdata
		char *key1				[in /    ] - 关键信息1
		char *key2				[in /    ] - 关键信息2
		char *key3				[in /    ] - 关键信息3
		char *prn				[in / out] - 动态口令，外部分配空间，并初始化
**
**返回类型:  int 
**		EMSUCCESS			验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
**		EMBADARG			参数错误，参数可能为空指针
**		EMBADREC			严重错误！PDATA内部数据有问题，如签名等
**		EMPRNLEN			Tokencode的长度错误
**		EMTIMRNG			passcode时间错误，不在生命期限内
**		EMNOTASSIGNED		令牌还没有分配
**      EMDISABLE			令牌处于禁用状态
**		EMFOREVERDISABLE	令牌被永久禁用，可能是时间偏移超过12小时
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-2-12		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetSIGPasscode (ULONG passc_time, PTTOKENRECORDDATA pdata, char *key1, char *key2, char *key3, char *prn);

/******************************************************************************
**
**函数名称:	DCSHSACheckUrgencyPassWord
**
**功能描述:	检测紧急口令，跳过所有的状态位检测，区分大小写
**			没有分配的令牌不允许使用紧急口令
**
**参数说明:
		PTTOKENDATA pdata		[in / ] - PDATA
		char *ucUrgencyPassWord	[in / ] - 紧急口令，16字符
**
**返回类型:  int 
**		EMBADARG		参数错误
**		EMBADURGENCYPW 紧急口令认证失败
**		EMSUCCESS	紧急口令认证成功
**
**其他说明:	
**
**历史:		日期		作者		行为
		2008-7-7		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckUrgencyPassWord (PTTOKENRECORDDATA pdata, char *ucUrgencyPassWord);


/******************************************************************************
**
**函数名称:	DCSHSAGetUrgencyPassWord
**
**功能描述:	获取紧急口令，跳过所有的状态位检测
**
**参数说明:
		PTTOKENDATA pdata		[in /    ] - PDATA
		char *ucUrgencyPassWord	[in / out] - 紧急口令，16字符，外部分配空间
**
**返回类型:  int 
**		EMBADARG		参数错误
**		EMBADGETURGENCYPW 紧急口令认证失败
**		EMSUCCESS		获取成功
**
**其他说明:	已经修改，在未分配的情况下也允许得到紧急口令
**
**历史:		日期		作者		行为
		2008-7-7		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetUrgencyPassWord (PTTOKENRECORDDATA pdata, char *ucUrgencyPassWord);


/******************************************************************************
**
**函数名称:	DCSHSASetUrgencyPassWord
**
**功能描述:	设置新的服务码，由引擎内部设置，设置成功后，返回新的服务码
**
**参数说明:
		PTTOKENRECORDDATA pdata		[in / out] - pdata
		char *ucNewUrgencyPassWord	[in / out] - 输出新的服务码，16个字节，建议外部分配20字节的空间
**
**返回类型:  int 
**		DCSHEMURGPWDERR	更改服务码失败
**		DCSHEMSUCCESS	更改服务码成功
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-10-27		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASetUrgencyPassWord (PTTOKENRECORDDATA pdata, char *ucNewUrgencyPassWord);


/******************************************************************************
**
**函数名称:	DCSHSACheckTokenvalue
**
**功能描述:	相当于FirstLogin,但是不修改任何数据，也不判断标志位
**
**参数说明:
		LONG passc_time		[in /    ] - 时间,DCSHSAGetPasscodeTime取得时间
		PTTOKENDATA pdata	[in /    ] - 引擎数据包
		char *prn			[in /    ] - tokencode
**
**返回类型: int 
**		EMSUCCESS			检查通过
**		EMBADARG			参数错误，参数可能为空指针
**		EMBADREC			严重错误！PDATA内部数据有问题，如签名等
**	    EMPRNLEN			Tokencode的长度错误
**		EMTIMRNG			passcode时间错误，不在生命期限内
**		EMOUTWIND			在任何窗口内找不到tokencode，可能需要再同步
**		EMBADTYPE			错误的令牌类型		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2008-5-16		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckTokenvalue (ULONG passc_time, PTTOKENRECORDDATA pdata, char *prn);


/******************************************************************************
**
**函数名称:	DCSHSAResynchTokenEx
**
**功能描述:	重新同步，非常大的计算量，12小时内寻找
**
**参数说明:
		LONG passc_time		[in /    ] - 时间,DCSHSAGetPasscodeTime取得时间
		PTTOKENDATA pdata	[in / out] - 引擎数据包
		char *prn			[in /    ] - tokencode
**
**返回类型: int 
**		EMSUCCESS			重新同步
**		EMBADARG			参数错误，参数可能为空指针
**		EMBADREC			严重错误！PDATA内部数据有问题，如签名等
**	    EMPRNLEN			Tokencode的长度错误
**		EMTIMRNG			passcode时间错误，不在生命期限内
**		EMOUTWIND			在任何窗口内找不到tokencode，可能需要再同步
**		EMSYN1OK			重新同步模式下，第一次匹配tokencode已经找到
**		EMBADTYPE			错误的令牌类型
**				
**其他说明:	重新同步对“永久失效”有作用
**
**历史:		日期		作者		行为
		2008-5-16		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAResynchTokenEx (ULONG passc_time, PTTOKENRECORDDATA pdata, char *prn);


/******************************************************************************
**
**函数名称:	DCSHSAInitBINRead
**
**功能描述:	打开BIN文件，进行格式分解，返回空间和总记录数
**
**参数说明:
		const char *InputFile	[in /    ] - 文件名称
		int **inpfil			[in / out] - 解密的空间，使用DCSHSAExitBINRead释放
		int *iRecoeds           [in / out] - 总共的记录数
		char *cType				[in / out] - 种子文件的类型
**
**返回类型: int 
**		EMSUCCESS       成功
**		EMEMPNAME       文件名称错误
**		EMBADINPUT      文件无法打开
**		EMNOMEM         没有足够的内存空间
**		EMBADINPFIL     BIN的文件内容错误，严重错误
**		EMBADSIG        文件签名错误，严重错误
**		EMBADFILETYPE	错误的种子文件类型
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2008-5-16		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAInitBINRead (const char *InputFile, void **inpfil, unsigned int *iRecoeds, char *cType);


/******************************************************************************
**
**函数名称:	DCSHSAReadBINRec
**
**功能描述:	从解密的数据中读取记录
**
**参数说明:
		char *userPriKeyPath[in /    ] - 用户私要文件
		void *inpfil		[in /    ] - 解密的数据块
		PTTOKENDATA pdata	[in / out] - 组装成PDATA
		int i               [in /    ] - 记录的索引号,从0开始
		char cType			[in / out] - 种子文件的类型
**
**返回类型: int 
**		EMSUCCESS       关闭成功
**      EMBADARG        错误的参数，可能是空指针
**		EMBADFILETYPE   种子文件的类型错误
**
**其他说明:	
**
**历史:		日期		作者		行为
		2008-5-16		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAReadBINRec (char *userPriKeyPath, void *inpfil, PTTOKENRECORDDATA pdata, unsigned int i, char cType);

/******************************************************************************
**
**函数名称:	DCSHSAExitBINRead
**
**功能描述:	清除BIN文件解密的内存空间
**
**参数说明:
		void *fc	[in /    ] - 空间地址 
**
**返回类型: int 
**		EMSUCCESS 关闭成功
**		EMBADARG  参数错误，可能是空指针
**		
**		
**其他说明:	用于释放接口内部的内存空间，其他地方也可以使用
**
**历史:		日期		作者		行为
		2008-5-16		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAExitBINRead (void *fc);


/******************************************************************************
**
**函数名称:	DCSHMakeTokenRecord -- 测试用
**
**功能描述:	手工制作一个PDATA,SDATA
**
**参数说明:
		PTTOKENDATA pdata	[in / out] - 初始化的数据
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2008-5-17		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void DCSHSAMakeTokenRecord(PTTOKENRECORDDATA pdata);


/******************************************************************************
**
**函数名称:	DCSHMakeTokenRecordEvent -- 测试用
**
**功能描述:	手工制作一个PDATA,SDATA
**
**参数说明:
		PTTOKENDATA pdata	[in / out] - 初始化的数据
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2008-5-17		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void DCSHSAMakeTokenRecordEvent(PTTOKENRECORDDATA pdata);


/******************************************************************************
**
**函数名称:	DCSHMakeTokenRecordMobile -- 测试用
**
**功能描述:	创建一个手机令牌，特指动联专用的
**
**参数说明:
		PTTOKENDATA pdata	[in / out] - 初始化的数据
**
**返回类型:  void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void DCSHSAMakeTokenRecordMobile(PTTOKENRECORDDATA pdata);


/******************************************************************************
**
**函数名称:	DCSHSASplitBINFile
**
**功能描述:	分割种子文件，支持加密格式，也支持数字信封格式
**
**参数说明:
		char *pSeedFile				[in /    ] - 种子文件路径
		unsigned int iNumber		[in /    ] - 分割多少个出来
**
**返回类型:  int 
**		EMREADSEEDFILEERR	读取种子文件失败
**		EMBADFILETYPE		种子文件类型错误
**		EMSUCCESS			成功
**		
**其他说明:	源文件保留，创建拆分的两个文件
**
**历史:		日期		作者		行为
		2008-6-12		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASplitBINFile (char *pSeedFile, unsigned int iNumber);



/******************************************************************************
**
**函数名称:	DCSHSAGenerateUrgencyPassWord (16字节的紧急口令)
**
**功能描述:	生成高质量的随机数，然后对该随机数进行Base64编码，截取16字节，作为紧急口令
**
**参数说明:
		char *ucUrgencyPassWord	[in / out] - 外部分配空间，返回紧急口令字符，建议20个字节空间
**
**返回类型:  int 
**			DCSHEMBADARG	参数错误
**			DCSHEMSEEDERR	BBS计算随机数时错误，可重新尝试
**			DCSHEMSUCCESS	创建紧急口令成功
**
**其他说明:	
**
**历史:		日期		作者		行为
		2008-7-7		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGenerateUrgencyPassWord(char *ucUrgencyPassWord);


/******************************************************************************
**
**函数名称:	DCSHSAGetChallenge
**
**功能描述:	产生一个指定长度的随机数作为挑战码
**
**参数说明:
		unsigned char *randomData	[in / out] - 随机数，挑战码，外部分配空间
		int iNumBytes				[in /    ] - 要求的随机数长度,最好是8
**
**返回类型: int 
**		EMBADARG		参数错误
**		EMSEEDERR		随机数产生错误
**		EMSUCCESS		成功
**		
**其他说明:	增加了返回全0的判断，如果发现是0，则可以重新尝试
**
**历史:		日期		作者		行为
		2008-6-23		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetChallenge(char *randomData, int iNumBytes);


/******************************************************************************
**
**函数名称:	DCSHSACheckCredential，兼容各种令牌的统一认证接口，比CheckPasscode更兼容
**
**功能描述:	兼容单一的tokencode认证
			兼容PIN码+tokencode认证
			兼容PIN码+tokencode认证，并且在成功后获取下一个正确的tokencode返回
			兼容挑战响应模式认证
			兼容SIM卡令牌
			兼容COS等其他卡商的令牌
			兼容ATM令牌认证
			兼容刮挂卡令牌认证
			兼容MS-Chap V2的认证方式
			兼容短信令牌
			兼容签名认证
**
**参数说明:
		PTTOKENDATA pdata			[in / out] - 公用数据结构
		PT_CODE2CHECK ptCode2Check	[in / out] - 根据各种类型组装的数据结构
**
**返回类型: int 
**		EMSUCCESS			验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
**		EMBADARG			参数错误，参数可能为空指针
**		EMBADREC			严重错误！PDATA内部数据有问题，如签名等
**      EMFIRST				第一次使用令牌登陆，成功，等同于EMSUCCESS

**		EMPRNLEN			Tokencode的长度错误
**		EMTIMRNG			passcode时间错误，不在生命期限内
**		EMNOTASSIGNED		令牌还没有分配
**      EMDISABLE			令牌处于禁用状态
**		EMFOREVERDISABLE	令牌被永久禁用，可能是时间偏移超过12小时
**		EMNOPIN				没有输入pin码
**		EMPINLEN			输入的PIN长度错误
**		EMBADPIN			输入错误的PIN
**		EMSTOLEN			因为PIN错误次数到达极限，默认为丢失
**		EMOUTWIND			在任何窗口内找不到tokencode，可能需要再同步
**		EMREPPRN			这是重放，需要小心，应该立即记录，并提醒管理员
**		EMREPDISABLE		重放到一定的次数则禁用
**		EMNEXTPRN			进入Next的模式
**		EMTHRESHOLDNXT		输入tokencode的错误次数到达极限，以后进入NEXT模式
**		EMBADNEX			在Next模式下输入错了tokencode
**		EMNEXTDISABLE		在Next模式下输入错误次数到达极限，被禁用
**		EMBADTYPE			错误的令牌类型
**		EMRESPONSEERR		挑战认证方式认证失败
**		
**其他说明:	请参考联合类型PT_DCSHCODE2CHECK详细说明
**
**历史:		日期		作者		行为
		2008-6-23		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckCredential (PTTOKENRECORDDATA pdata, PT_DCSHCODE2CHECK ptCode2Check);

/******************************************************************************
**
**函数名称:	ReadCardInfoFromFile
**
**功能描述:	读取令牌刮卡信息的文件，请参考PTTOKENCARDRECORD结构体
**
**参数说明:
		char *CardFileName					[in /    ] - 文件名称
		PTTOKENCARDRECORD *pTokenCardRecord	[in / out] - 数据块，内部分配空间，DCSHSAExitBINRead释放
		int *iCounter						[in / out] - 总记录数
**
**返回类型: int  
**		EMSUCCESS 成功
**		EMBADARG  失败，可能参数传递错误
**		
**其他说明:	pTokenCardRecord的内存空间请使用DCSHSAExitBINRead释放
**
**历史:		日期		作者		行为
		2008-8-5		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAReadCardInformation (char *CardFileName, PTTOKENCARDRECORD *pTokenCardRecord, int *iCounter);


/******************************************************************************
**
**函数名称:	DCSHSAGetSMSPasscode
**
**功能描述:	获取短消息认证的动态口令，同时设置该口令的有效时间
**
**参数说明:
		LONG valid_time		[in /    ] - 有效时间，单位秒
		PTTOKENDATA pdata	[in / out] - 引擎数据包
		char *prn			[in / out] - 返回的tokencode，外部分配数组
**
**返回类型: int 
**		EMSUCCESS			验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
**		EMBADARG			参数错误，参数可能为空指针
**		EMBADREC			严重错误！PDATA内部数据有问题，如签名等
**		EMTIMRNG			passcode时间错误，不在生命期限内
**		EMNOTASSIGNED		令牌还没有分配
**      EMDISABLE			令牌处于禁用状态
**		EMFOREVERDISABLE	令牌被永久禁用，可能是时间偏移超过12小时
**		EMBADTYPE			错误的令牌类型
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2008-8-15		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetSMSPasscode (ULONG valid_time, PTTOKENRECORDDATA pdata, char *prn);

/******************************************************************************
**
**函数名称:	DCSHSACheckSMSPasscode
**
**功能描述:	认证短信动态口令，在有效时间内
**
**参数说明:
		PTTOKENDATA pdata	[in / ] - 引擎数据包
		char *prn			[in / ] - 返回的tokencode，外部分配数组
**
**返回类型: int 
**		EMSUCCESS			验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
**		EMBADARG			参数错误，参数可能为空指针
**		EMBADREC			严重错误！PDATA内部数据有问题，如签名等
**		EMTIMRNG			passcode时间错误，不在生命期限内
**		EMNOTASSIGNED		令牌还没有分配
**      EMDISABLE			令牌处于禁用状态
**		EMFOREVERDISABLE	令牌被永久禁用，可能是时间偏移超过12小时
**		EMSMSTIMEOUT		超时，但口令正确
**		EMSMSBADPRN			口令错误
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2008-8-15		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckSMSPasscode (PTTOKENRECORDDATA pdata, char *prn);


/******************************************************************************
**
**函数名称:	DCSHSACheckPasscodeEx，建议使用DCSHSACheckCredential方法
**
**功能描述:	这是一个简便的认证接口，根据组合方式，也可以进行认证
			扩展的、统一的认证接口
			如果是 时间、事件、手机 认证，则 prn=tokencode,NULL,0
			如果是 异步认证（如手机），   则 prn=response,challenge=challenge,0
			如果是短信认证，获得动态口令，则 prn=外部数组,NULL,valid_time=有效时间（秒）
			                认证动态口令，则 prn=tokencode,NULL,0
			如果是ATM令牌，需要传递算法， 则 prn=动态口令,challenge=挑战码（用户口令，也可能使摘要）,valid_time=算法（ALGORITHM_ATM/MD5/SH1）
			如果是刮刮卡令牌，只进行认证，则 prn=动态口令,challenge=主机码,valid_time=0）
**
**参数说明:
		PTTOKENRECORDDATA pdata	[in / out] - 引擎数据包
		char *prn				[in / out] - 根据要求存放tokencode，或者响应码，或者返回值，参考上访说明
		char *challenge			[in /    ] - 异步模式，则为挑战码，参考上访说明
		ULONG valid_time		[in /    ] - 获得短信口令，存放有效时间，秒，参考上访说明
**
**返回类型:  int 
**
**		EMBADARG			参数错误，参数可能为空指针
**		EMTOKENTYPE			错误的令牌应用类型
**		
**		
**其他说明:	注意，本接口不支持带PIN的认证
**
**历史:		日期		作者		行为
		2008-9-3		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckPasscodeEx (PTTOKENRECORDDATA pdata, char *prn, char *challenge, ULONG valid_time);


/******************************************************************************
**
**函数名称:	DCSHSAMsChap2CheckAndMakeResponse
**
**功能描述:	MS CHAP V2，对客户端提交的nt_response进行认证
			
			请查看DCSHMSCHAP2_PASSCODE结构，请查看RFC2759

			引擎支持MS CHAP v2，但引擎内除了PIN和动态口令，没有用户名、服务端挑战码、客户端挑战码等
			所以，要求调用该接口的时候必须送入

			返回的AuthenticatorResponse是这样的格式“S=0123456789ABCDEF0123456789ABCDEF01234567”
**
**参数说明:
		DCSHMSCHAP2_PASSCODE *pDCSHMSCHAP2Block	[in / out] - 结构内包含了MS Chap V2的各个字段，AuthenticatorResponse由该函数生成
		char *cMsChap2Password					[in /    ] - 口令，注意：
																		如果启用PIN，则口令应该为“动态口令+PIN码“
																		如果不启用PIN，则口令就是”动态口令“

																		注意，用户应用也应该如此编排
**
**返回类型: int 
**		DCSHEMBADARG			参数错误
**		DCSHEMMSCHAPV2ERROR		mschapv2认证客户端提交的响应码错误
**		DCSHEMSUCCESS			成功
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-2-5		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAMsChap2CheckAndMakeResponse(DCSHMSCHAP2_PASSCODE *pDCSHMSCHAP2Block, char *cMsChap2Password);

/******************************************************************************
**
**函数名称:	DCSHSACheckPasscodeMsChap2
**
**功能描述:	
**
**参数说明:
		LONG passc_time		[in /    ] - DCSHSAGetPasscodeTime取得时间
		PTTOKENDATA pdata	[in / out] - 引擎数据包
		unsigned char *pMsChap2Block[in / out] - DCSHMSCHAP2_PASSCODE类型的block
**
**返回类型: int 
**		EMSUCCESS			验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
**		EMBADARG			参数错误，参数可能为空指针
**		EMBADREC			严重错误！PDATA内部数据有问题，如签名等
**      EMFIRST				第一次使用令牌登陆，成功，等同于EMSUCCESS

**		EMPRNLEN			Tokencode的长度错误
**		EMTIMRNG			passcode时间错误，不在生命期限内
**		EMNOTASSIGNED		令牌还没有分配
**      EMDISABLE			令牌处于禁用状态
**		EMFOREVERDISABLE	令牌被永久禁用，可能是时间偏移超过12小时
**		EMNOPIN				没有输入pin码
**		EMPINLEN			输入的PIN长度错误
**		EMBADPIN			输入错误的PIN
**		EMSTOLEN			因为PIN错误次数到达极限，默认为丢失
**		EMOUTWIND			在任何窗口内找不到tokencode，可能需要再同步
**		EMREPPRN			这是重放，需要小心，应该立即记录，并提醒管理员
**		EMREPDISABLE		重放到一定的次数则禁用
**		EMNEXTPRN			进入Next的模式
**		EMTHRESHOLDNXT		输入tokencode的错误次数到达极限，以后进入NEXT模式
**		EMBADNEX			在Next模式下输入错了tokencode
**		EMNEXTDISABLE		在Next模式下输入错误次数到达极限，被禁用
**		EMBADTYPE			错误的令牌类型
**		
**		
**其他说明:	
			ms-chap v2的认证方式是得不到用户的PIN码和动态口令的
			于是，我们必须从引擎中得到这些，然后逆过程计算响应码，进行比较
			此时，我们就会发现无法检测重放
			对于这个问题，我们只能恰当的判断：
			1、我们总是记住最后4次正确的tokencode
			2、如果这次验证和历史的tokencode相同，则认为重放
			3、如果在窗口内扫描，一旦发现与历史tokencode相同，则跳过

**
**历史:		日期		作者		行为
		2008-5-15		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckPasscodeMsChap2 (ULONG passc_time, PTTOKENRECORDDATA pdata, unsigned char *pMsChap2Block/*DCSHMSCHAP2_PASSCODE类型的block*/);


/******************************************************************************
**
**函数名称:	DCSHSAMsChap2ChangePassword
**
**功能描述:	MS CHAP V2，解密得到新口令，并验证新口令
			
			请查看DCSHMSCHAP2_CHANGE_PW结构，请查看RFC2759

			修改口令之前先要完成认证

			认证通过后再解密新口令，并且验证新口令是否正确

			返回的AuthenticatorResponse是这样的格式“S=0123456789ABCDEF0123456789ABCDEF01234567”
**
**参数说明:
		DCSHMSCHAP2_CHANGE_PW *pDCSHMSCHAP2Blockpw[in /    ] - 结构内包含了MS Chap V2的各个字段，AuthenticatorResponse由该函数生成
		char *oldPinAndTokencode				  [in /    ] - 口令，注意：
																		如果启用PIN，则口令应该为“动态口令+PIN码“
																		如果不启用PIN，则口令就是”动态口令“
	
																		注意，用户应用也应该如此编排
**		char *newpin							  [in / out] - 新口令

**返回类型: int 
**		DCSHEMBADARG			参数错误
**		DCSHEMMSCHAPV2ERRORPW	mschapv2客户端提交的新口令校验异常
**		DCSHEMSUCCESS			成功
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-2-5		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAMsChap2ChangePassword(DCSHMSCHAP2_CHANGE_PW *pMsChap2Blockpw, char *oldPinAndTokencode, char *newpin);


/******************************************************************************
**
**函数名称:	DCSHSAChangePINCodeMsChap2
**
**功能描述:	ms chap v2 更改口令
**
**参数说明:
		ULONG passc_time				[in /    ] - DCSHSAGetPasscodeTime取得时间
		PTTOKENRECORDDATA pdata			[in /    ] - 引擎数据包
		unsigned char *pMsChap2Blockpw	[in / out] - 参考DCSHMSCHAP2_CHANGE_PW结构，将返回authenticatorResponse
**
**返回类型:  int 
**		DCSHEMBADARG	参数错误
**		DCSHEMSUCCESS	成功
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-2-5		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAChangePINCodeMsChap2 (ULONG passc_time, PTTOKENRECORDDATA pdata, unsigned char *pMsChap2Blockpw/*DCSHMSCHAP2_CHANGE_PW类型的block*/);


/******************************************************************************
**
**函数名称:	DCSHSAForceSynchronizeEventCounter
**
**功能描述: 强制同步事件型令牌的计数器
**			在某种情况下，需要获得服务端当前的计数器，然后累加一个常量，作为强制的同步的参数，要求客户端强制修改计数器等于这个参数	
**
**参数说明:
		PTTOKENRECORDDATA pdata		[in / out] - 引擎数据包
		unsigned int CounterAdd		[in /    ] - 一个常量，建议大于100小于1000；
		unsigned int *SrvCounter	[in / out] - 设置当前令牌的计数器的值，并且返回该值
**
**返回类型:  int 
**		DCSHEMBADARG			参数错误
**		DCSHEMFORCESYNTYPE		强制同步令牌类型错误，不是事件型
**		DCSHEMSUCCESS			成功
**
**其他说明:	
**
**历史:		日期		作者		行为
		2009-6-22		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAForceSynchronizeEventCounter(PTTOKENRECORDDATA pdata, unsigned int CounterAdd, unsigned int *SrvCounter );

/******************************************************************************
**
**函数名称:	DCSHSAForceSynchronizeEventCounterReverse
**
**功能描述:	根据客户端的计数器强制同步服务器中计数器
			计数器更改的时候，不允许服务器的计数器往小修改，客户端提交的计数器必须大于服务器的计数器

**
**参数说明:
		PTTOKENRECORDDATA pdata		[in / out] - 引擎数据包
		unsigned int ClientCounter	[in /    ] - 客户端的计数器
**
**返回类型:  int 
**		DCSHEMBADARG			参数错误
**		DCSHEMFORCESYNTYPE		强制同步令牌类型错误，不是事件型
**		DCSHEMFORCECOUNTERLESS	不允许使用客户端的小值修改服务端的大值
**		DCSHEMSUCCESS			成功
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2010-1-27		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAForceSynchronizeEventCounterReverse(PTTOKENRECORDDATA pdata, unsigned int ClientCounter );


/******************************************************************************
**
**函数名称:	DCSHSAUnlockPinpadToken
**
**功能描述:	输出K8锁定的解锁码，带pad的令牌在多次输入pin失败后会锁定，此时提供屏幕的数字和序列号，可到服务端进行“获取解锁码”
**
**参数说明:
		PTTOKENRECORDDATA pdata	[in /    ] - 引擎数据包
		char *cChallenge		[in /    ] - 令牌锁定后，屏幕上的8位数字
		char *cUnLockCode		[in / out] - 解锁码，8位数字
**
**返回类型:  int 
**		DCSHEMBADARG			参数错误
**		DCSHEMSUCCESS			成功
**
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-7-29		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAUnlockPinpadToken(PTTOKENRECORDDATA pdata, char *cChallenge, char *cUnLockCode);

/*
基本的两大类型：[时间型Time]			[事件型Event（手机令牌是事件型）]

基本的两种介质：[硬件（SIM卡是硬件）]	[软件（手机上的是软件）]

动联的令牌种类：[普通令牌K5旧 K3新]		[短信令牌]	[手机令牌] 	[软件令牌]

令牌的应用模式：[普通认证模式]	[主机认证模式]	[挑战应答模式]	[数字签名模式]	[MSChapV2模式]

每个模式都有自己的认证接口

上面的关系是层层叠加的，动联的K8令牌就是时间型的硬件K3令牌，同时支持了挑战应答和数字签名模式

举例：短信令牌可以是时间类型的，也可以是事件类型的
      短信令牌传统意义上是软件介质，如果要认为硬件的，也可以
	  短信令牌的应用模式是获取再认证，这个就等同于普通认证模式
	  短信令牌也可以支持其他模式，只要调用接口就可以
	  就如同K5令牌完全可以支持数字签名模式，只不过K5没有键盘而已

  这个举例说明四层结构都是层层叠加方式的，最终应用以接口调用和实际令牌决定

*/

/******************************************************************************
**
**函数名称:	DCSHSAGetCoverHostCode
**
**功能描述:	获得当前的计数器对应的主机字符和动态口令，返回主机字符和当前使用的次数
**
**参数说明:
		ULONG passc_time		[in /    ] - 系统时间
		PTTOKENRECORDDATA pdata	[in /    ] - pdata
		char *cHostCode			[in / out] - 主机字符，外部分配空间
		int *iCurrentUseCount	[in / out] - 当前使用的次数
**
**返回类型:  int 
**		DCSHEMBADARG	参数错误
**		DCSHEMNOTCOVERTYPE	不是刮刮卡类型的令牌
**		DCSHEMOVERCOVERNUM	达到最大值，刮刮卡可能是用到最后一个了
**		DCSHEMSUCCESS	成功
**
**其他说明:	
**
**历史:		日期		作者		行为
		2009-8-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetCoverHostCode (ULONG passc_time, PTTOKENRECORDDATA pdata, char *cHostCode, int *iCurrentUseCount);


/******************************************************************************
**
**函数名称:	DCSHSACheckCoverTokenCode
**
**功能描述:	认证覆盖区的动态口令，需要传递主机字符和动态口令，如果启用了PIN，还需要传递PIN
**
**参数说明:
		ULONG passc_time		[in /    ] - 当前系统时间
		PTTOKENRECORDDATA pdata	[in / out] - pdata
		char *cHostCode			[in /    ] - 主机字符
		char *cTokenCode		[in /    ] - 覆盖区动态口令
		char *pin				[in /    ] - pin码
**
**返回类型:  int 
**		DCSHEMBADARG	参数错误
**		DCSHEMNOTCOVERTYPE	不是刮刮卡类型
**		DCSHEMMUSTGETCOVER	认证之前应该获得主机字符
**		DCSHEMOVERCOVERNUM	不认证最大值和以后的
**		DCSHEMHOSTCODEERRO	主机字符错误
**		DCSHEMERRORTODISABLE	动态口令错误太多次，被禁用
**		DCSHEMCOVERCODEERR	覆盖区动态口令错误
**		DCSHEMSUCCESS		认证通过
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-8-14		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckCoverTokenCode (ULONG passc_time, PTTOKENRECORDDATA pdata, char *cHostCode, char *cTokenCode, char *pin);


/******************************************************************************
**
**函数名称:	DCSHSAUpdateNewCoverCard
**
**功能描述:	
**
**参数说明:
		ULONG passc_time											[in /    ] - 系统时间
		PTTOKENRECORDDATA pdataO									[in / out] - 旧卡pdata
		char *cHostCodeO											[in /    ] - 主机字符
		char *cTokenCodeO											[in /    ] - 动态口令
		char *pinO													[in /    ] - pin		
        PTTOKENRECORDDATA pdataN									[in / out] - 新卡pdata 
		char *cHostCodeN											[in /    ] - 主机字符
		char *cTokenCodeN											[in /    ] - 动态口令
		char *pinN													[in /    ] - pin
**
**返回类型: int 
**		DCSHEMBADARG	参数错误
**		DCSHEMNOTCOVERTYPE	不是刮刮卡类型
**		DCSHEMMUSTGETCOVER	认证之前应该获得主机字符
**		DCSHEMOVERCOVERNUM	不认证最大值和以后的
**		DCSHEMHOSTCODEERRO	主机字符错误
**		DCSHEMERRORTODISABLE	动态口令错误太多次，被禁用
**		DCSHEMCOVERCODEERR	覆盖区动态口令错误
**		DCSHEMSUCCESS		认证通过，可以更新		
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-8-16		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAUpdateNewCoverCard (ULONG passc_time, PTTOKENRECORDDATA pdataO, char *cHostCodeO, char *cTokenCodeO, char *pinO,
											                    PTTOKENRECORDDATA pdataN, char *cHostCodeN, char *cTokenCodeN, char *pinN);

/******************************************************************************
**
**函数名称:	DCSHSAXORStringTo8Char
**
**功能描述:	把字符串通过异或转化为8字节，原则是8字节分组，对应的位置异或
**
**参数说明:
		const char *cBuffer			[in /    ] - 原始字符串，长度不限 
		unsigned char *cDDWBUffer	[in / out] - 输出的8字节，必须外部提供8字节空间，且初始化0
**
**返回类型:  int 
**		EMBADARG  参数错误，参数可能是空指针，或者算法值不对
**		EMSUCCESS 成功		
**		
**		
**其他说明:	
**			小于等于8字节的字符串则直接copy
**
**历史:		日期		作者		行为
		2009-12-8		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAXORStringTo8Char (const char *cBuffer, unsigned char *cDDWBUffer);



//---------------------------------客户订制的开发接口------------------------------------//


/******************************************************************************
**
**函数名称:	DCSHSACheckPasscode_for_hexin
**
**功能描述:	专门为核心定制的认证接口
			该接口中，返回值DCSHEMSUCCESS和DCSHEMFIRST是正确的，其他的统统的认为是错误
			返回值同时返回偏移量的变化和update的建议，其中：
			DCSHEMFIRST返回值必须update；
			DCSHEMSUCCESS时，我们认为offset变化了就应该update,但决定权由核心控制；但是注意，当iAdjustStep接近小窗口最大值时，必须update;
**
**参数说明:
		LONG passc_time		[in /    ] - DCSHSAGetPasscodeTime取得时间
		PTTOKENDATA pdata	[in / out] - 引擎数据包
		char *prn			[in /    ] - tokencode
		char *pin			[in /    ] - pin
		int *iAdjustStep	[in / out] - 此次认证调整的偏移量，可能是0，也可能是正数，也可能是负数
		int *iAdviseUpdate	[in / out] - 动联建议是否Update，0-表示不需要，1-表示需要
**
**返回类型: int 
**		EMSUCCESS			验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
**      EMFIRST				第一次使用令牌登陆，成功，等同于EMSUCCESS
**		其他值统统认为失败

**		DCSHSACheckPasscode的返回值相同。
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2010-1-4		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckPasscode_for_hexin (ULONG passc_time, PTTOKENRECORDDATA pdata, char *prn, char *pin, int *iAdjustStep, int *iAdviseUpdate);

/******************************************************************************
**
**函数名称:	DCSHSAModifyTokenDataWindow
**
**功能描述:	修改种子数据的认证窗口
**			建议：
					1、券商、游戏 设置成为 时间型（120，300，600），事件型（2，5，10）
					2、银行、企业 设置成为 时间型（60，180，300），事件型（ 1，3，5）
**			
**			该操作会清除所有状态位和认证模式
**参数说明:
		PTTOKENRECORDDATA pdata		[in / out] - pdata，种子数据
		int iSmallWindowSeconds		[in /    ] - 新的小窗口
		int iMediumWindowSeconds	[in /    ] - 新的中窗口
		int iLargeWindowSeconds		[in /    ] - 新的大窗口
**
**返回类型:  int 
**			EMSUCCESS			修改成功，必须返回必须保存PDATA到数据库
**			其他的返回值都是失败
**		
**其他说明:	该接口属于逼迫得以使用，客户在订购令牌时，应该预先向厂商提出令牌的详细策略需求。
**
**历史:		日期		作者		行为
		2010-8-4		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAModifyTokenDataWindow (PTTOKENRECORDDATA pdata, int iSmallWindowSeconds, int  iMediumWindowSeconds, int iLargeWindowSeconds);


/******************************************************************************
**
**函数名称:	DCSHSACheckPasscode_for_City9
**
**功能描述:	专门为九城定制的认证接口
			该接口中，返回值DCSHEMSUCCESS和DCSHEMFIRST是正确的，其他的统统的认为是错误
			返回值同时返回偏移量的变化和update的建议，其中：
			DCSHEMFIRST返回值必须update；
			DCSHEMSUCCESS时，我们认为offset变化了就应该update,但决定权由九城控制；但是注意，当iAdjustStep接近小窗口最大值时，必须update;
**
**参数说明:
		char *pdataBase64	[in / out] - 引擎数据包的base64字符串
		char *prn			[in /    ] - tokencode
		char *pin			[in /    ] - pin
		int *iAdjustStep	[in / out] - 此次认证调整的偏移量，可能是0，也可能是正数，也可能是负数
		int *iAdviseUpdate	[in / out] - 动联建议是否Update，0-表示不需要，1-表示需要
**
**返回类型: int 
**		EMSUCCESS			验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
**      EMFIRST				第一次使用令牌登陆，成功，等同于EMSUCCESS
**		其他值统统认为失败

**		DCSHSACheckPasscode的返回值相同。
**		
**		
**其他说明:	
**			内部调用了取系统UTC时间
**			内部对状态位进行了重置操作
**			为了降低客户应用的数据库操作，提供了Update数据库的建议
**			如果外部用户有错误锁定操作，所有非正确返回值都可以不保存数据库
**
**历史:		日期		作者		行为
		2010-1-24		杨波		Created
		2010-6-24		杨波		对pdata的数据进行了Base64转换
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckPasscode_for_City9 ( char *pdataBase64, char *prn, char *pin, int *iAdjustStep, int *iAdviseUpdate);

/******************************************************************************
**
**函数名称:	DCSHSAInitPdata_for_City9
**
**功能描述:	为9城定制的种子数据初始化的接口，其功能就是 分配/启用/禁用PIN
**
**参数说明:
		char *pdataBase64	[in / out] - 引擎数据包的base64字符串
**
**返回类型: 
**		EMBADARG  参数错误，参数可能是空指针，或者算法值不对
**		EMSUCCESS 成功
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2010-1-25		杨波		Created
		2010-6-24		杨波		对pdata的数据进行了Base64转换
******************************************************************************/
DCSHCOREAPI_API int DCSHSAInitPdata_for_City9 ( char *pdataBase64);


/******************************************************************************
**
**函数名称:	DCSHSAResynchTokenEx_for_City9
**
**功能描述:	为9城定制的重新同步接口
**
**参数说明:
		char *pdataBase64	[in / out] - 引擎数据包的base64字符串
		char *prn1			[in /    ] - 第一个动态口令
		char *prn2			[in /    ] - 连续的第二个动态口令
**
**返回类型: int 
**		DCSHEMBADARG	参数错误
**		DCSHEMSUCCESS	同步成功
**		其它值参考返回信息列表
**		
**		
**其他说明:	只有当返回DCSHEMSUCCESS时，才需要Update数据库
**
**历史:		日期		作者		行为
		2010-8-12		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAResynchTokenEx_for_City9(char *pdataBase64, char *prn1, char *prn2);

/******************************************************************************
**
**函数名称:	DCSHSAGetCustomTokenPasscode
**
**功能描述:	获取定制类型令牌的认证的动态口令，用于DCSH_HARDWARE_TOKEN_GET类型的令牌
			目前为恒生定制接口， 将来会增加策略控制，如时间不会回逆获取

			注意，一旦获取动态口令后，该动态口令不可用于动联系统认证
			First获取后，将设置为Normal，当作第一次已经被使用
			Normal获取后依然是Normal
			Next/Next2获取后，还原成Normal
**
**参数说明:
		LONG passc_time		[in /    ] - DCSHSAGetPasscodeTime取得时间
		char *pdatabase64	[in / out] - 引擎数据包的base64字符串
		char *prn			[in / out] - 返回的tokencode，外部分配数组
**
**返回类型: int 
**		EMSUCCESS			验证通过，除了EMBADREC，所有的返回必须保存PDATA到数据库
**		EMBADARG			参数错误，参数可能为空指针
**		EMBADREC			严重错误！PDATA内部数据有问题，如签名等
**		EMTIMRNG			passcode时间错误，不在生命期限内
**		EMNOTASSIGNED		令牌还没有分配
**      EMDISABLE			令牌处于禁用状态
**		EMFOREVERDISABLE	令牌被永久禁用，可能是时间偏移超过12小时
**		EMBADTYPE			错误的令牌类型
**		
**		
**其他说明:	建议每一次获得动态口令后都应该保存pdatabase64，如果对性能有要求，也可以不保存
**
**历史:		日期		作者		行为
		2010-3-28		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetCustomTokenPasscode (ULONG passc_time, char *pdataBase64, char *prn);


/******************************************************************************
**
**函数名称:	DCSHSABase64Encode
**
**功能描述:	对二进制流进行编码，形成可见字符
**
**参数说明:
		unsigned char * binaryDataPtr	[in /    ] - 二进制流
		int binaryDataLength			[in /    ] - 二进制流长度
		char * stringData				[in / out] - 可见字符串，外部分配空间
		int *stringDataLength			[in / out] - 可见字符串长度
**
**返回类型: int 
**		DCSHEMSUCCESS			成功
**		DCSHEMBASE64CODEERR	 失败
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-8-19		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSABase64Encode( unsigned char * binaryDataPtr, int binaryDataLength, char * stringData ,int *stringDataLength );

/******************************************************************************
**
**函数名称:	DCSHSABase64Decode
**
**功能描述:	对base64编码进行解码，获得二进制流
**
**参数说明:
		char * stringData				[in /    ] - 可见字符串
		int stringDataLen				[in /    ] - 可见字符串长度
		unsigned char * binaryDataPtr	[in / out] - 二进制流，外部分配空间
		int * binaryDataLength			[in / out] - 二进制流的长度
**
**返回类型:  int 
**		DCSHEMSUCCESS			成功
**		DCSHEMBASE64CODEERR	 失败
**			
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-8-19		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int  DCSHSABase64Decode( char * stringData, int stringDataLen, unsigned char * binaryDataPtr, int * binaryDataLength );


/******************************************************************************
**
**函数名称:	DCSHSAUpdateNewSeedForICBC
**
**功能描述:	更新种子数据，工行使用了身份证码的数字与种子重新异或后，得到新的种子
**
**参数说明:
		PTTOKENRECORDDATA pdata	[in / out] - 种子数据
		char *cID				[in /    ] - 异或的数字，应该是9byte
**
**返回类型:  int 
**		DCSHEMBADARG 参数错误
**		DCSHEMICBCHAVEXOR 已经异或过了
**		DCSHEMSUCCESS 成功
**其他说明:	
**
**历史:		日期		作者		行为
		2010-11-19		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAUpdateNewSeedForICBC(PTTOKENRECORDDATA pdata, char *cID );

/******************************************************************************
**
**函数名称:	DCSHSAICBCActiveUnlockAlg
**
**功能描述:	工行令牌的种子二次变形算法和令牌解锁算法
**
**参数说明:
		BYTE *bSeed				[in /    ] - 种子，变形之前为老种子，变形之后为新种子，20字节
		int iSeedLen			[in /    ] - 种子长度，工行固定为20字节，固定为20
		char *cChallengeCode	[in /    ] - 挑战码，变形时为激活码6数字，后补0x00,0x00，解锁时为解锁码8数字
		int iChallengeCodeLen	[in /    ] - 挑战码的长度，固定为8
		char *cResponseCode		[in / out] - 响应码，即校验码或者解锁码，外部分配空间
		int iResponseCodeLen	[in /    ] - 响应码长度，固定为6
**
**返回类型: int 
**		DCSHEMBADARG		参数送入了空指针
**		DCSHEMSEEDLENERR	种子长度错误
**      DCSHEMCHECKLENERR	指定的响应码长度错误
**      DCSHEMACTIVELENERR	挑战码的长度错误
**		DCSHEMSUCCESS		成功
**		
**其他说明:	建议对Msg的数据长度补齐8Byte
**
**历史:		日期		作者		行为
		2010-12-22		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAICBCActiveUnlockAlg( BYTE *bSeed, int iSeedLen, char *cChallengeCode, int iChallengeCodeLen, char *cResponseCode, int iResponseCodeLen );

/******************************************************************************
**
**函数名称:	DCSHSAICBCGenerateNewSeedAlg
**
**功能描述:	工行令牌变形得到新种子算法，需要输入激活码和校验码
**
**参数说明:
		BYTE *bSeed				[in /    ] - 种子数据，20字节
		int iSeedLen			[in /    ] - 种子长度，20
		char *cChallengeCode	[in /    ] - 激活码，激活码为6个数字，输入时后补0x00,0x00
		int iChallengeCodeLen	[in /    ] - 激活码的长度，8
		char *cResponseCode		[in /    ] - 校验码，6数字字符
		int iResponseCodeLen	[in /    ] - 校验码的长度6
		BYTE *bNewSeed			[in / out] - 新的种子，外部分配空间，至少20字节
		int *iNewSeedLen		[in / out] - 新种子长度，20
**
**返回类型: int 
**		DCSHEMBADARG		参数送入了空指针
**		DCSHEMSEEDLENERR	种子长度错误
**      DCSHEMCHECKLENERR	指定的响应码长度错误
**      DCSHEMACTIVELENERR	挑战码的长度错误
**		DCSHEMACTIVCHECKERR	激活码校验失败，不进行变形
**		DCSHEMSUCCESS		成功
**	
**其他说明:	
**
**历史:		日期		作者		行为
		2010-12-22		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAICBCGenerateNewSeedAlg( BYTE *bSeed, int iSeedLen, char *cChallengeCode, int iChallengeCodeLen, char *cResponseCode, int iResponseCodeLen , BYTE *bNewSeed, int *iNewSeedLen);

/******************************************************************************
**
**函数名称:	DCSHSAICBCTimeTokeAlg
**
**功能描述:	工行的时间型动态口令算法
**
**参数说明:
		BYTE *bSeed			[in /    ] - 种子，20字节
		int iSeedLen		[in /    ] - 种子长度，20
		BYTE *bTokenTime	[in /    ] - 时间数据，注意大头字序，8字节
		int iTokenTimeLen	[in /    ] - 时间数据长度，固定为8
		char *cTokenCode	[in / out] - 动态口令，6数字字符，外部分配空间
		int iTokenCodeLen	[in /    ] - 动态口令长度，固定为6
**
**返回类型: int 
**		DCSHEMBADARG		参数送入了空指针
**		DCSHEMSEEDLENERR	种子长度错误
**      DCSHEMPRNLEN		动态口令长度错误
**      DCSHEMBIGTIMELENERR	大头字序的时间长度错误
**		DCSHEMSUCCESS		成功
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2010-12-22		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAICBCTimeTokeAlg( BYTE *bSeed, int iSeedLen, BYTE *bTokenTime, int iTokenTimeLen, char *cTokenCode, int iTokenCodeLen );


/******************************************************************************
**
**函数名称:	DCSHSAICBCChallengeResponseAlg
**
**功能描述:	工行的时间挑战应答型动态口令算法
**
**参数说明:
		BYTE *bSeed					[in /    ] - 种子，20字节
		int iSeedLen				[in /    ] - 种子长度，20
		BYTE *bTokenTime			[in /    ] - 时间数据，大头字序，8字节
		int iTokenTimeLen			[in /    ] - 时间长度，固定为8
		char *cResponse				[in /    ] - 挑战问题，字符串
		unsigned int iResponseLen	[in /    ] - 最大长度64
		char *cTokenCode			[in / out] - 动态口令，6数字字符，外部分配空间
		int iTokenCodeLen			[in /    ] - 动态口令长度，固定为6
**
**返回类型: int 
**		DCSHEMBADARG		参数送入了空指针
**		DCSHEMSEEDLENERR	种子长度错误
**      DCSHEMPRNLEN		动态口令长度错误
**      DCSHEMBIGTIMELENERR	大头字序的时间长度错误
**      DCSHEMCHANLENERR	挑战问题的的长度大于64，错误
**		DCSHEMSUCCESS		成功		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2010-12-22		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAICBCChallengeResponseAlg( BYTE *bSeed, int iSeedLen, BYTE *bTokenTime, int iTokenTimeLen, char *cResponse, unsigned int iResponseLen, char *cTokenCode, int iTokenCodeLen );

/******************************************************************************
**
**函数名称:	DCSHSAICBCPinLockedAlg
**
**功能描述:	工行令牌的PIN输入错误的锁定码产生算法
**
**参数说明:
		BYTE *bSeed				[in /    ] - 种子数据，20字节
		int iSeedLen			[in /    ] - 种子长度，20
		BYTE *bTokenTime		[in /    ] - 时间数据，大头字序，8字节
		int iTokenTimeLen		[in /    ] - 时间数据长度，8
		char *cPin				[in /    ] - 令牌的PIN码，6数字字符,后补0x00,0x00
		unsigned int iPinLen	[in /    ] - PIN码长度，8
		char *cPinLockedCode	[in / out] - 令牌的锁定码，8数字字符
		int iPinLockedCodeLen	[in /    ] - 锁定码长度，8
**
**返回类型: int 
**		DCSHEMBADARG		参数送入了空指针
**		DCSHEMSEEDLENERR	种子长度错误
**      DCSHEMPRNLEN		动态口令长度错误
**      DCSHEMBIGTIMELENERR	大头字序的时间长度错误
**      DCSHEMCHANLENERR	挑战问题的的长度大于64，错误
**      DCSHEMPINLEN		Pin码长度错误
**		DCSHENUNLOCKPINERR	令牌解锁码的字符串长度错误，8数字
**		DCSHEMSUCCESS		成功	
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2010-12-22		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAICBCPinLockedAlg( BYTE *bSeed, int iSeedLen, BYTE *bTokenTime, int iTokenTimeLen, char *cPin, unsigned int iPinLen, char *cPinLockedCode, int iPinLockedCodeLen );


/******************************************************************************
**
**函数名称:	DCSHSAConvertICBCTokenSeed
**
**功能描述:	第一次使用时转换原始的种子数据
**
**参数说明:
		PTTOKENRECORDDATA pdata	[in / out] - pdata数据
		char *cActiveCode		[in /    ] - 激活码，12个数字
**
**返回类型:  int 
**		DCSHEMBADARG		参数送入了空指针
**		DCSHEMSEEDLENERR	种子长度错误
**      DCSHEMPRNLEN		动态口令长度错误
**      DCSHEMBIGTIMELENERR	大头字序的时间长度错误
**      DCSHEMCHANLENERR	挑战问题的的长度大于64，错误
**      DCSHEMPINLEN		Pin码长度错误
**		DCSHENUNLOCKPINERR	令牌解锁码的字符串长度错误，8数字
**		DCSHEMSUCCESS		成功**		
**		
**其他说明:	激活码被保存在ucUserLastName中
			原始种子被保存在ucUserFirstName种
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAConvertICBCTokenSeed(PTTOKENRECORDDATA pdata, char *cActiveCode );

/******************************************************************************
**
**函数名称:	DCSHSAGetICBCActiveCode
**
**功能描述:	转换之前获得激活码，12个数字
**
**参数说明:
		PTTOKENRECORDDATA pdata	[in /    ] - pdata
		char *cActiveCode		[in / out] - 令牌激活码，12个数字，外部应送入12以上的空间
**
**返回类型:  int 
**		DCSHEMBADARG		参数送入了空指针
**		DCSHEMSEEDLENERR	种子长度错误
**      DCSHEMPRNLEN		动态口令长度错误
**      DCSHEMBIGTIMELENERR	大头字序的时间长度错误
**      DCSHEMCHANLENERR	挑战问题的的长度大于64，错误
**      DCSHEMPINLEN		Pin码长度错误
**		DCSHENUNLOCKPINERR	令牌解锁码的字符串长度错误，8数字
**		DCSHEMSUCCESS		成功
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAGetICBCActiveCode(PTTOKENRECORDDATA pdata, char *cActiveCode );


/******************************************************************************
**
**函数名称:	SM3
**
**功能描述:	国密刚公开的SM3算法
**
**参数说明:
		unsigned char *Block			[in /    ] - SM3的单个块运算
		unsigned int ulBlock			[in /    ] - 单个块长度，固定为64
		unsigned char *bSM3HashValue	[in / out] - SM3摘要
**
**返回类型:  int 
**		
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-10		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSASM3(unsigned char *Block, unsigned int ulBlock, unsigned char *bSM3HashValue);

/******************************************************************************
**
**函数名称:	TestCreateTimeTokencodeRange -- 测试专用
**
**功能描述:	创建一段时间范围内的动态口令
**
**参数说明:
		PTTOKENRECORDDATA pdata	[in / out] - 
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateTimeTokencodeRange(PTTOKENRECORDDATA pdata);

/******************************************************************************
**
**函数名称:	TestCreateTimeTokencode  -- 测试专用
**
**功能描述:	
**
**参数说明:
		void	[in / out] - 
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateTimeTokencode(void);

/******************************************************************************
**
**函数名称:	TestCreateTimeTokencode_Host  -- 测试专用
**
**功能描述:	
**
**参数说明:
		void	[in / out] - 
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateTimeTokencode_Host(void);

/******************************************************************************
**
**函数名称:	TestCreateEventTokencode  -- 测试专用
**
**功能描述:	
**
**参数说明:
		void	[in / out] - 
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateEventTokencode(void);

/******************************************************************************
**
**函数名称:	TestCreateChallengeResponsecode -- 测试专用
**
**功能描述:	
**
**参数说明:
		void	[in / out] - 
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateChallengeResponsecode(void);

/******************************************************************************
**
**函数名称:	TestCreateTimeTokencodeSig  -- 测试专用
**
**功能描述:	
**
**参数说明:
		void	[in / out] - 
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateTimeTokencodeSig(void);

/******************************************************************************
**
**函数名称:	TestCreateTimeTokencodeATM  -- 测试专用
**
**功能描述:	
**
**参数说明:
		void	[in / out] - 
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateTimeTokencodeATM(void);

/******************************************************************************
**
**函数名称:	TestCreateEventTokencodeATM  -- 测试专用
**
**功能描述:	
**
**参数说明:
		void	[in / out] - 
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void TestCreateEventTokencodeATM(void);

/******************************************************************************
**
**函数名称:	TestPrintCardInfo  -- 测试专用
**
**功能描述:	
**
**参数说明:
		char *CardFileName	[in / out] - 
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void TestPrintCardInfo(char *CardFileName);

/******************************************************************************
**
**函数名称:	DCSHChangeTokenSeed  -- 测试专用
**
**功能描述:	
**
**参数说明:
		PTTOKENRECORDDATA pdata	[in / out] - 
**
**返回类型: int 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2011-1-11		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAChangeTokenSeed(PTTOKENRECORDDATA pdata);


#if OS_SELECT_WINDOWS

/******************************************************************************
**
**函数名称:	DCSHSACheckShecaCert
**
**功能描述:	接入上海CA的开发包，此接口初始化eKey，并读取证书信息，以用来检测eKey的正常
**
**参数说明:
		char *cPrikeyPassword	[in /	 ] - 私钥的密钥
		char* pCertInfoFile		[in / out] - 证书的一些信息，保存到文件
**
**返回类型:  int 
**		DCSHEMSHECADLLERR	上海CA的接口不存在，请按装上海CA的证书管理器
**		DCSHEMSHECAKEYERR	eKey操作失败，请检查eKey是否正常
**		DCSHEMSUCCESS		成功，可以打印证书信息
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2010-9-2		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckShecaCert (char *cPrikeyPassword, char* pCertInfoFile);

/******************************************************************************
**
**函数名称:	DCSHSACheckSJK0817E
**
**功能描述:	检测华正天网的加密卡是否正常，并且进行了加解密运算，SM1
**
**参数说明:
			[in / out] - 
**
**返回类型:  int 
**		DCSHEMPCIOPENERR	加密卡初始化失败
**		DCSHEMPCISESSIONERR	创建连接失败
**		DCSHEMPCIOPERERR	加密卡运算失败
**		DCSHEMSUCCESS		成功
**其他说明:	
**
**历史:		日期		作者		行为
		2010-9-2		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSACheckSJK0817E ();

/******************************************************************************
**
**函数名称:	DCSHSAMakePubkeyEncRecord
**
**功能描述:	用户的公钥加密会话密钥数据
**
**参数说明:
		char *userCertPath			[in /    ] - 用户证书的路径
		unsigned char *pRecordData	[in /    ] - 会话密钥，16字节
		unsigned int RecordDataLen	[in /    ] - 数据长度，16
		unsigned char *pEvpSigData	[in / out] - 加密的数据，128字节，外部分配空间
		unsigned int EvpSigDataLen	[in / out] - 加密的数据长度，应该是128
**
**返回类型:  int 
**		EMREADUSERCERTERR	读取用户证书失败
**		EMDECUSERCERTERR	解析用户证书失败
**		EMSUCCESS			成功
**其他说明:	
**
**历史:		日期		作者		行为
		2008-6-12		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int DCSHSAMakePubkeyEncRecord (char *userCertPath, unsigned char *pRecordData, unsigned int RecordDataLen, unsigned char *pEvpSigData, unsigned int *EvpSigDataLen);

/******************************************************************************
**
**函数名称:	TestRandom
**
**功能描述:	真随机数测试
**
**参数说明:
		int iNumber	[in / ] - 次数 
**
**返回类型: DCSHCOREAPI_API int 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2010-9-7		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int TestRandom(int iNumber);

/******************************************************************************
**
**函数名称:	TestSM1
**
**功能描述:	SM1算法测试
**
**参数说明:
		int iNumber	[in / ] - 次数
**
**返回类型: DCSHCOREAPI_API int 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2010-9-7		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int TestSM1(int iNumber);

/******************************************************************************
**
**函数名称:	TestSM3
**
**功能描述:	SM3算法测试
**
**参数说明:
		int iNumber	[in / ] - 次数
**
**返回类型: DCSHCOREAPI_API int 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2010-9-7		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int TestSM3(int iNumber);

#endif

/******************************************************************************
**
**函数名称:	TestTokenCodeNumberIndex
**
**功能描述:	使用10个不同的种子，每个种子从tokentime=i*60计算10000个tokencode，
            对10000个tokencode进行数字统计，计算0-9数字出现的频率
**
**参数说明:
		int	iAlgID [in / ] - 算法
**
**返回类型: int 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-1-16		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int TestTokenCodeNumberIndex(int iAlgID);


/******************************************************************************
**
**函数名称:	TestTokenCodeDoubleIdx
**
**功能描述:	设定5个时间点，在每个点随机创建若干个种子，并且在这时间点上计算tokencode
            对若干个tokencode进行逐个比对，进行重复计数
			“若干个”为传递参数，可指定
**
**参数说明:
		int seedCount	[in / ] - 每个时间点，创建seedCount个种子，计算seedCount个动态口令
**		int	iAlgID		[in / ] - 算法
**返回类型: int 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-1-16		杨波		Created
******************************************************************************/
DCSHCOREAPI_API int TestTokenCodeDoubleIdx(int seedCount, int iAlgID);


/******************************************************************************
**
**函数名称:	TestTokenCodePrefect
**
**功能描述:	测试验证动态口令的性能，1000000次验证
            直接命中，计算1次
			偏移1，计算2次
			偏移3，计算6次
			偏移5，计算10次
**
**参数说明:
			[in / out] - 
**
**返回类型: void 
**		[添加返回值说明]
**		
**		
**其他说明:	
**
**历史:		日期		作者		行为
		2009-1-16		杨波		Created
******************************************************************************/
DCSHCOREAPI_API void TestTokenCodePrefect(int MAXCYCLETEST, int iAlgID);

#ifdef  __cplusplus
}
#endif

#endif




