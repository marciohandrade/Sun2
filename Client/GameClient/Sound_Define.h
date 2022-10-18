//------------------------------------------------------------------------
// Name		: Sound_Define.h
// Author	: j0 
// Desc		: 사운드 넘버를 디파인함.
//------------------------------------------------------------------------


enum SOUND_DEFINE
{
	BGM_FIELD_TEST		= 1,			// 맵 이동시 사용
	BGM_BATTLE 			= 2,			// 전투 시 사용
	MAINBGM				= 3,			// Main BGM

	// 인터페이스 1001~ 2000				
	CLICK				=	1001,		// 로그인 시 버튼 클릭
	WIN_CLOSE			=	1002,		// 각종 창 닫힐 때
	WIN_OPEN			=	1003,		// 각종 창 열릴 때
	EQUIP_OFF_DEFAULT	=	1004,		// 장비창에서 아이템을 집을 때
	EQUIP_ON_DEFAULT	=	1005,		// 장비창에 아이템을 장비 시킬 때
	HOLD_BOTTLE			=	1006,		// 인벤창(또는 퀵슬롯)에 물약을 집을 때
	HOLD_DEFAULT		=	1007,		// 인벤창(또는 퀵슬롯)에 장비를 집을 때
	HOLD_MONEY			=	1008,		// 인벤창에 돈을 집을 때
	PUT_BOTTLE			=	1009,		// 인벤창(또는 퀵슬롯)에 물약을 집을 때
	PUT_DEFAULT			=	1010,		// 인벤창(또는 퀵슬롯)에 장비 놓을 때
	PUT_MONEY			=	1011,		// 인벤창에 돈을 놓을 때
	USE_DEFAULT			=	1011,		// 디폴트 값

	// 캐릭터 풋스텝 2001~3000				
	F_DIRT1				=	2001,		// 여성 발걸음 소리
	F_DIRT2				=	2002,		// 흙바닥
	F_DIRT3				=	2003,		
	F_DIRT4				=	2004,		
	F_DIRT_LAND			=	2005,		//딱딱한 바닥 
	M_DIRT1				=	2006,		// 남성 발걸음 소리
	M_DIRT2				=	2007,		// 흙바닥
	M_DIRT3				=	2008,		
	M_DIRT4				=	2009,		
	M_DIRT_LAND			=	2010,		//딱딱한 바닥

	//캐릭터가  데미지 받았을 때 3001~4000				
	DAMAGE_DEFAULT1		=	3001,		//디폴트 값
	DAMAGE_DEFAULT2		=	3002,		
	DAMAGE_DEFAULT3		=	3003,		
	DAMAGE_HEAVY1		=	3004,		//헤비아머에 데미지 받은 경우
	DAMAGE_HEAVY2		=	3005,		
	DAMAGE_HEAVY3		=	3006,		
	DAMAGE_MID1			=	3007,		//미디엄 아머에 데미지 받은 경우
	DAMAGE_MID2			=	3008,		
	DAMAGE_MID3			=	3009,		

	//캐릭터 공격시 임펙트 사운드4001~5000				
	IMPACT_2HSWORD1		=	4001,		//양손검
	IMPACT_2HSWORD2		=	4002,		
	IMPACT_2HSWORD3		=	4003,		
	IMPACT_2HSWORD4		=	4004,		
	IMPACT_ARROW1		=	4005,		//한손 석궁 양손 석궁
	IMPACT_ARROW2		=	4006,		
	IMPACT_ARROW3		=	4007,		
	IMPACT_ARROW4		=	4008,		
	IMPACT_AXE1			=	4009,		//양손도끼
	IMPACT_AXE2			=	4010,		
	IMPACT_AXE3			=	4011,		
	IMPACT_CRUSH1		=	4012,		//임시 사운드
	IMPACT_CRUSH2		=	4013,		
	IMPACT_CRUSH3		=	4014,		
	IMPACT_DEFAULT1		=	4015,		//임시 사운드
	IMPACT_DEFAULT2		=	4016,		
	IMPACT_DEFAULT3		=	4017,		
	IMPACT_HAMMER1		=	4018,		//둔기류
	IMPACT_HAMMER2		=	4019,		
	IMPACT_HAMMER3		=	4020,		
	IMPACT_LASH1		=	4021,		//채찍
	IMPACT_LASH2		=	4022,		
	IMPACT_LASH3		=	4023,		
	IMPACT_SMASH1		=	4024,		//임시 사운드
	IMPACT_SMASH2		=	4025,		
	IMPACT_SMASH3		=	4026,		
	IMPACT_SWORD1		=	4027,		//한손검, 곡도
	IMPACT_SWORD2		=	4028,		
	IMPACT_SWORD3		=	4029,		

	//무기 휘두르는 소리 5001~6000					
	SWING_2HSWORD1		=	5001,		//양손검	
	SWING_2HSWORD2		=	5002,			
	SWING_AXE1			=	5003,		//양손도끼	
	SWING_AXE2			=	5004,			
	SWING_ETHER1		=	5005,		//에테르웨폰	
	SWING_ETHER2		=	5006,			
	SWING_LASH1			=	5007,		//채찍	
	SWING_LASH2			=	5008,			
	SWING_SPEAR1		=	5009,		//창	
	SWING_SPEAR2		=	5010,			
	SWING_SWORD1		=	5011,		//한손검	
	SWING_SWORD2		=	5012,			
	//스타일 3타째 휘두르는 소리,					
	UNI_CRITICAL		=	5801,		//크리티컬	한손검
	UNI_DAMAGE			=	5802,		//데미지	
	UNI_DEFENSE			=	5803,		//디펜스	
	UNI_DOWN			=	5804,		//다운	한손도끼,한손검
	UNI_PIERCING		=	5805,		//피어싱	
	UNI_SPEED			=	5806,		//스피드	한손검
	UNI_STUN			=	5807,		//스턴	양손도끼

	//버서커 6001~7000					
	BER_ATTACK1			=	6001,		//공격시 외침	
	BER_ATTACK2			=	6002,			
	BER_ATTACK3			=	6003,			
	BER_ATTACK4			=	6004,			
	BER_ATTACK5			=	6005,			
	BER_BODYFALL		=	6006,		//점프후 착지	
	BER_DEATH			=	6007,		//죽을때	
	BER_JUMP			=	6008,		//점프시	
	BER_PAIN1			=	6009,		//신음소리	
	BER_PAIN2			=	6010,		
	BER_PAIN3			=	6011,		
	BER_PAIN_HEAVY1		=	6012,	//크리티컬시 신음 소리
								
	//드래곤 7001~8000			,	
	DRA_ATTACK1			=	7001,		//공격시 외침
	DRA_ATTACK2			=	7002,		
	DRA_ATTACK3			=	7003,		
	DRA_ATTACK4			=	7004,		
	DRA_ATTACK5			=	7005,		
	DRA_BODYFALL		=	7006,		//점프후 착지
	DRA_DEATH			=	7007,		//죽을때
	DRA_JUMP1			=	7008,		//점프시
	DRA_JUMP2			=	7009,		
	DRA_PAIN1			=	7010,		//신음소리
	DRA_PAIN2			=	7011,		
	DRA_PAIN3			=	7012,		
	DRA_PAIN4			=	7013,		
	DRA_PAIN_HEAVY1		=	7014,		//크리티컬시 신음 소리
								
	//새도우8001~9000				
								
	//발키리9001~10000				
								
	//매지션10001~11000				
								
	//몬스터					
	//드래곤좀비 11001~12000				
	DZ_ATTACK1			=	11001,		//공격시 외침
	DZ_ATTACK2			=	11002,		
	DZ_BODYFALL			=	11003,		// 임시
	DZ_DEATH			=	11004,		//죽을때
	DZ_HIT1				=	11005,		//공격시 타격음
	DZ_HIT2				=	11006,		
	DZ_IDLE1			=	11007,		//idle 상태로 있을 때
	DZ_IDLE2			=	11008,		
	DZ_PAIN1			=	11009,		//신음소리
	DZ_PAIN2			=	11010,		
	DZ_PAIN3			=	11011,		
	DZ_PAIN_HEAVY		=	11012,		//크리티컬시 신음소리
	DZ_SIGHT1			=	11013,		//적 발견 시 
	DZ_SIGHT2			=	11014,		
	DZ_STEP1			=	11015,		//풋스텝
	DZ_STEP2			=	11016,		
	DZ_SWING1			=	11017,		//무기 휘두르는 소리
	DZ_SWING2			=	11018,		
								 
	//비스트 12001~13000				
	BK_ATTACK1			=	12001,		//공격시 외침
	BK_ATTACK2			=	12002,		
	BK_BODYFALL			=	12003,		// 임시
	BK_DEATH			=	12004,		//죽을때
	BK_HIT1				=	12005,		//공격시 타격음
	BK_HIT2				=	12006,		
	BK_IDLE1			=	12007,		//idle 상태로 있을 때
	BK_IDLE2			=	12008,		
	BK_PAIN1			=	12009,		//신음소리
	BK_PAIN2			=	12010,		
	BK_PAIN3			=	12011,		
	BK_PAIN_HEAVY		=	12012,		//크리티컬시 신음소리
	BK_SIGHT1			=	12013,		//적 발견 시 
	BK_SIGHT2			=	12014,		
	BK_STEP1			=	12015,		//풋스텝
	BK_STEP2			=	12016,		
	BK_SWING1			=	12017,		//무기 휘두르는 소리
	BK_SWING2			=	12018,		
								 
	//악령전사 (13001~14000)				
	AI_ATTACK1			=	13001,		//공격시 외침
	AI_ATTACK2			=	13002,		
	AI_BODYFALL			=	13003,		// 임시
	AI_DEATH			=	13004,		//죽을때
	AI_HIT1				=	13005,		//공격시 타격음
	AI_HIT2				=	13006,		
	AI_IDLE1			=	13007,		//idle 상태로 있을 때
	AI_IDLE2			=	13008,		
	AI_PAIN1			=	13009,		//신음소리
	AI_PAIN2			=	13010,		
	AI_PAIN3			=	13011,		
	AI_PAIN_HEAVY		=	13012,		//크리티컬시 신음소리
	AI_SIGHT1			=	13013,		//적 발견 시 
	AI_SIGHT2			=	13014,		
	AI_STEP1			=	13015,		//풋스텝
	AI_STEP2			=	13016,		
	AI_SWING1			=	13017,		//무기 휘두르는 소리
	AI_SWING2			=	13018,		
									 
	//악령 주술사(14001~15000)				
	NEC_ATTACK1			=	14001,		//공격시 외침
	NEC_ATTACK2			=	14002,		
	NEC_BODYFALL		=	14003,		// 임시
	NEC_DEATH			=	14004,		//죽을때
	NEC_HIT1			=	14005,		//공격시 타격음
	NEC_HIT2			=	14006,		
	NEC_IDLE1			=	14007,		//idle 상태로 있을 때
	NEC_IDLE2			=	14008,		
	NEC_PAIN1			=	14009,		//신음소리
	NEC_PAIN2			=	14010,		
	NEC_PAIN3			=	14011,		
	NEC_PAIN_HEAVY		=	14012,		//크리티컬시 신음소리
	NEC_SIGHT1			=	14013,		//적 발견 시 
	NEC_SIGHT2			=	14014,		
	NEC_STEP1			=	14015,		//풋스텝
	NEC_STEP2			=	14016,		
	NEC_SWING1			=	14017,		//무기 휘두르는 소리
	NEC_SWING2			=	14018,		
};									 


// ----------------------------

# define CHARACTER_ATTACK_SOUND_TERM (DRA_ATTACK1 - BER_ATTACK1)
// ----------------------------