//------------------------------------------------------------------------
// Name		: Sound_Define.h
// Author	: j0 
// Desc		: ���� �ѹ��� ��������.
//------------------------------------------------------------------------


enum SOUND_DEFINE
{
	BGM_FIELD_TEST		= 1,			// �� �̵��� ���
	BGM_BATTLE 			= 2,			// ���� �� ���
	MAINBGM				= 3,			// Main BGM

	// �������̽� 1001~ 2000				
	CLICK				=	1001,		// �α��� �� ��ư Ŭ��
	WIN_CLOSE			=	1002,		// ���� â ���� ��
	WIN_OPEN			=	1003,		// ���� â ���� ��
	EQUIP_OFF_DEFAULT	=	1004,		// ���â���� �������� ���� ��
	EQUIP_ON_DEFAULT	=	1005,		// ���â�� �������� ��� ��ų ��
	HOLD_BOTTLE			=	1006,		// �κ�â(�Ǵ� ������)�� ������ ���� ��
	HOLD_DEFAULT		=	1007,		// �κ�â(�Ǵ� ������)�� ��� ���� ��
	HOLD_MONEY			=	1008,		// �κ�â�� ���� ���� ��
	PUT_BOTTLE			=	1009,		// �κ�â(�Ǵ� ������)�� ������ ���� ��
	PUT_DEFAULT			=	1010,		// �κ�â(�Ǵ� ������)�� ��� ���� ��
	PUT_MONEY			=	1011,		// �κ�â�� ���� ���� ��
	USE_DEFAULT			=	1011,		// ����Ʈ ��

	// ĳ���� ǲ���� 2001~3000				
	F_DIRT1				=	2001,		// ���� �߰��� �Ҹ�
	F_DIRT2				=	2002,		// ��ٴ�
	F_DIRT3				=	2003,		
	F_DIRT4				=	2004,		
	F_DIRT_LAND			=	2005,		//������ �ٴ� 
	M_DIRT1				=	2006,		// ���� �߰��� �Ҹ�
	M_DIRT2				=	2007,		// ��ٴ�
	M_DIRT3				=	2008,		
	M_DIRT4				=	2009,		
	M_DIRT_LAND			=	2010,		//������ �ٴ�

	//ĳ���Ͱ�  ������ �޾��� �� 3001~4000				
	DAMAGE_DEFAULT1		=	3001,		//����Ʈ ��
	DAMAGE_DEFAULT2		=	3002,		
	DAMAGE_DEFAULT3		=	3003,		
	DAMAGE_HEAVY1		=	3004,		//���Ƹӿ� ������ ���� ���
	DAMAGE_HEAVY2		=	3005,		
	DAMAGE_HEAVY3		=	3006,		
	DAMAGE_MID1			=	3007,		//�̵�� �Ƹӿ� ������ ���� ���
	DAMAGE_MID2			=	3008,		
	DAMAGE_MID3			=	3009,		

	//ĳ���� ���ݽ� ����Ʈ ����4001~5000				
	IMPACT_2HSWORD1		=	4001,		//��հ�
	IMPACT_2HSWORD2		=	4002,		
	IMPACT_2HSWORD3		=	4003,		
	IMPACT_2HSWORD4		=	4004,		
	IMPACT_ARROW1		=	4005,		//�Ѽ� ���� ��� ����
	IMPACT_ARROW2		=	4006,		
	IMPACT_ARROW3		=	4007,		
	IMPACT_ARROW4		=	4008,		
	IMPACT_AXE1			=	4009,		//��յ���
	IMPACT_AXE2			=	4010,		
	IMPACT_AXE3			=	4011,		
	IMPACT_CRUSH1		=	4012,		//�ӽ� ����
	IMPACT_CRUSH2		=	4013,		
	IMPACT_CRUSH3		=	4014,		
	IMPACT_DEFAULT1		=	4015,		//�ӽ� ����
	IMPACT_DEFAULT2		=	4016,		
	IMPACT_DEFAULT3		=	4017,		
	IMPACT_HAMMER1		=	4018,		//�б��
	IMPACT_HAMMER2		=	4019,		
	IMPACT_HAMMER3		=	4020,		
	IMPACT_LASH1		=	4021,		//ä��
	IMPACT_LASH2		=	4022,		
	IMPACT_LASH3		=	4023,		
	IMPACT_SMASH1		=	4024,		//�ӽ� ����
	IMPACT_SMASH2		=	4025,		
	IMPACT_SMASH3		=	4026,		
	IMPACT_SWORD1		=	4027,		//�Ѽհ�, �
	IMPACT_SWORD2		=	4028,		
	IMPACT_SWORD3		=	4029,		

	//���� �ֵθ��� �Ҹ� 5001~6000					
	SWING_2HSWORD1		=	5001,		//��հ�	
	SWING_2HSWORD2		=	5002,			
	SWING_AXE1			=	5003,		//��յ���	
	SWING_AXE2			=	5004,			
	SWING_ETHER1		=	5005,		//���׸�����	
	SWING_ETHER2		=	5006,			
	SWING_LASH1			=	5007,		//ä��	
	SWING_LASH2			=	5008,			
	SWING_SPEAR1		=	5009,		//â	
	SWING_SPEAR2		=	5010,			
	SWING_SWORD1		=	5011,		//�Ѽհ�	
	SWING_SWORD2		=	5012,			
	//��Ÿ�� 3Ÿ° �ֵθ��� �Ҹ�,					
	UNI_CRITICAL		=	5801,		//ũ��Ƽ��	�Ѽհ�
	UNI_DAMAGE			=	5802,		//������	
	UNI_DEFENSE			=	5803,		//���潺	
	UNI_DOWN			=	5804,		//�ٿ�	�Ѽյ���,�Ѽհ�
	UNI_PIERCING		=	5805,		//�Ǿ��	
	UNI_SPEED			=	5806,		//���ǵ�	�Ѽհ�
	UNI_STUN			=	5807,		//����	��յ���

	//����Ŀ 6001~7000					
	BER_ATTACK1			=	6001,		//���ݽ� ��ħ	
	BER_ATTACK2			=	6002,			
	BER_ATTACK3			=	6003,			
	BER_ATTACK4			=	6004,			
	BER_ATTACK5			=	6005,			
	BER_BODYFALL		=	6006,		//������ ����	
	BER_DEATH			=	6007,		//������	
	BER_JUMP			=	6008,		//������	
	BER_PAIN1			=	6009,		//�����Ҹ�	
	BER_PAIN2			=	6010,		
	BER_PAIN3			=	6011,		
	BER_PAIN_HEAVY1		=	6012,	//ũ��Ƽ�ý� ���� �Ҹ�
								
	//�巡�� 7001~8000			,	
	DRA_ATTACK1			=	7001,		//���ݽ� ��ħ
	DRA_ATTACK2			=	7002,		
	DRA_ATTACK3			=	7003,		
	DRA_ATTACK4			=	7004,		
	DRA_ATTACK5			=	7005,		
	DRA_BODYFALL		=	7006,		//������ ����
	DRA_DEATH			=	7007,		//������
	DRA_JUMP1			=	7008,		//������
	DRA_JUMP2			=	7009,		
	DRA_PAIN1			=	7010,		//�����Ҹ�
	DRA_PAIN2			=	7011,		
	DRA_PAIN3			=	7012,		
	DRA_PAIN4			=	7013,		
	DRA_PAIN_HEAVY1		=	7014,		//ũ��Ƽ�ý� ���� �Ҹ�
								
	//������8001~9000				
								
	//��Ű��9001~10000				
								
	//������10001~11000				
								
	//����					
	//�巡������ 11001~12000				
	DZ_ATTACK1			=	11001,		//���ݽ� ��ħ
	DZ_ATTACK2			=	11002,		
	DZ_BODYFALL			=	11003,		// �ӽ�
	DZ_DEATH			=	11004,		//������
	DZ_HIT1				=	11005,		//���ݽ� Ÿ����
	DZ_HIT2				=	11006,		
	DZ_IDLE1			=	11007,		//idle ���·� ���� ��
	DZ_IDLE2			=	11008,		
	DZ_PAIN1			=	11009,		//�����Ҹ�
	DZ_PAIN2			=	11010,		
	DZ_PAIN3			=	11011,		
	DZ_PAIN_HEAVY		=	11012,		//ũ��Ƽ�ý� �����Ҹ�
	DZ_SIGHT1			=	11013,		//�� �߰� �� 
	DZ_SIGHT2			=	11014,		
	DZ_STEP1			=	11015,		//ǲ����
	DZ_STEP2			=	11016,		
	DZ_SWING1			=	11017,		//���� �ֵθ��� �Ҹ�
	DZ_SWING2			=	11018,		
								 
	//��Ʈ 12001~13000				
	BK_ATTACK1			=	12001,		//���ݽ� ��ħ
	BK_ATTACK2			=	12002,		
	BK_BODYFALL			=	12003,		// �ӽ�
	BK_DEATH			=	12004,		//������
	BK_HIT1				=	12005,		//���ݽ� Ÿ����
	BK_HIT2				=	12006,		
	BK_IDLE1			=	12007,		//idle ���·� ���� ��
	BK_IDLE2			=	12008,		
	BK_PAIN1			=	12009,		//�����Ҹ�
	BK_PAIN2			=	12010,		
	BK_PAIN3			=	12011,		
	BK_PAIN_HEAVY		=	12012,		//ũ��Ƽ�ý� �����Ҹ�
	BK_SIGHT1			=	12013,		//�� �߰� �� 
	BK_SIGHT2			=	12014,		
	BK_STEP1			=	12015,		//ǲ����
	BK_STEP2			=	12016,		
	BK_SWING1			=	12017,		//���� �ֵθ��� �Ҹ�
	BK_SWING2			=	12018,		
								 
	//�Ƿ����� (13001~14000)				
	AI_ATTACK1			=	13001,		//���ݽ� ��ħ
	AI_ATTACK2			=	13002,		
	AI_BODYFALL			=	13003,		// �ӽ�
	AI_DEATH			=	13004,		//������
	AI_HIT1				=	13005,		//���ݽ� Ÿ����
	AI_HIT2				=	13006,		
	AI_IDLE1			=	13007,		//idle ���·� ���� ��
	AI_IDLE2			=	13008,		
	AI_PAIN1			=	13009,		//�����Ҹ�
	AI_PAIN2			=	13010,		
	AI_PAIN3			=	13011,		
	AI_PAIN_HEAVY		=	13012,		//ũ��Ƽ�ý� �����Ҹ�
	AI_SIGHT1			=	13013,		//�� �߰� �� 
	AI_SIGHT2			=	13014,		
	AI_STEP1			=	13015,		//ǲ����
	AI_STEP2			=	13016,		
	AI_SWING1			=	13017,		//���� �ֵθ��� �Ҹ�
	AI_SWING2			=	13018,		
									 
	//�Ƿ� �ּ���(14001~15000)				
	NEC_ATTACK1			=	14001,		//���ݽ� ��ħ
	NEC_ATTACK2			=	14002,		
	NEC_BODYFALL		=	14003,		// �ӽ�
	NEC_DEATH			=	14004,		//������
	NEC_HIT1			=	14005,		//���ݽ� Ÿ����
	NEC_HIT2			=	14006,		
	NEC_IDLE1			=	14007,		//idle ���·� ���� ��
	NEC_IDLE2			=	14008,		
	NEC_PAIN1			=	14009,		//�����Ҹ�
	NEC_PAIN2			=	14010,		
	NEC_PAIN3			=	14011,		
	NEC_PAIN_HEAVY		=	14012,		//ũ��Ƽ�ý� �����Ҹ�
	NEC_SIGHT1			=	14013,		//�� �߰� �� 
	NEC_SIGHT2			=	14014,		
	NEC_STEP1			=	14015,		//ǲ����
	NEC_STEP2			=	14016,		
	NEC_SWING1			=	14017,		//���� �ֵθ��� �Ҹ�
	NEC_SWING2			=	14018,		
};									 


// ----------------------------

# define CHARACTER_ATTACK_SOUND_TERM (DRA_ATTACK1 - BER_ATTACK1)
// ----------------------------