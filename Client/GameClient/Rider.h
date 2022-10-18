
#pragma once

struct sRiderInfo;
class ObjectAniEventController;

enum RiderParts
{
	RP_BASE,	//탈것기본모형
	RP_BODY,	//갑옷
	RP_BOOTS,	//부츠	
	RP_HELM,	//투구
	RP_COUNT,
};

class Rider : public ObjectAniEventController
{
public:
	Rider( void );
	virtual ~Rider( void );

    //라이더 생성
	virtual BOOL Create(DWORD dwRiderIndex, Player* master_ptr, BOOL summon_effect);
    
    //리소스해제
	virtual void Destroy( void );
	
	//------------------------------------------------------------------------------
	//! Qualifier: 애니메이션설정
	//! Parameter: WzID wzID : 애니메이션 코드
	//! Parameter: BOOL bLoop : loop 여부
    //! Parameter: BOOL bRestart : bLoop == TRUE 이면 무조건 bRestart = FALSE로 셋팅됨
	//------------------------------------------------------------------------------
	void SetAnimation(WzID wzID, BOOL bLoop = FALSE, BOOL bRestart = TRUE);

    //애니메이션계산
	virtual void AnimateUnit(Player* master_ptr);
    
    //애니메이션 프레임계산
	virtual BOOL Process(DWORD dwTick, Player* master_ptr);

    //랜더:Display용
	virtual BOOL Render( St_RenderCommand *pCommand);

    //랜더
	virtual BOOL Render();
	
    //라이더복사(주인공탈것용)
	void CopyRider(Rider *pRider, bool is_with_parts = true);

    //라이더복사(슬롯에서탈것용)
    void CopyRiderBody(DWORD dwRiderIndex, Player* master_ptr);

    //변환(위치,회전)
	void Transform(CWzUnitDraw* pMaster);

    //베이스파츠(라이더몸통)
    CWzUnitDraw* GetRenderPart(){return m_ppRenderPart[RP_BASE];}
    
    //라이더파츠(라이더파츠)
    inline CWzUnitDraw* GetRenderPart(int iIndex);
    
    //위치.회전.크기
    void SetScale(WzVector vScale);

    void SetPosition(WzVector *pPos) { if(GetRenderPart()) GetRenderPart()->SetPosition(*pPos);}

	void SetRotation(const WzVector* normal_axis, float master_angle);

    void SetForceLod(int iLod);

    //파츠 장착.탈착.서브타입(왁싱)
    void AttachRiderPart(int nIdx, DWORD dwItemCode);

    //------------------------------------------------------------------------------
    //! Returns:   void
    //! Qualifier: z-base 값옷 부터 
    //! Parameter: int nIdx
    //!             0 = 갑옷
    //!             1 = 부츠	
    //!             2 = 투구
    //------------------------------------------------------------------------------
    void DetachRiderPart(int nIdx);

    void DetachPart(const RiderParts& rider_parts);

    void SetSubtype(int nPartIdx,int nSubType);
	    
    void SetColor(WzColor color){m_Color=color;}
    
    void EnableFunction(BOOL bFlag);
    
    char GetAniMapping(){return m_Animaping;}
    
    DWORD GetRiderIndex(){return m_dwRiderIndex;}

    WzVector* GetLinkPos(){return &m_vLinkPos;}
	
    bool RefreshApperRider(Player* master_ptr);
    
    //! 탈것이 날기 생태인지
    bool is_spread() { return is_spread_; }
    void set_spread(bool val) { is_spread_ = val; }

    bool is_possible_fly();

    WzID current_animation() const { return current_animation_; }
    void set_current_animation(WzID val) { current_animation_ = val; }

private:
    // 소환이펙트 유무
    DWORD effect_code() const { return effect_code_; }
    void set_effect_code(DWORD val) { effect_code_ = val; }
    // 보간상태
    bool is_interpoate() const { return is_interpoate_; }
    void set_is_interpoate(bool val) { is_interpoate_ = val; }
    // 보간축
    WzVector& interpolate_axis(){ return interpolate_axis_; }
    void set_interpolate_axis(const WzVector& val) { interpolate_axis_ = val; }
    // 이전 노말축
    WzVector& prev_axis(){ return prev_axis_; }
    void set_prev_axis(const WzVector& val) { prev_axis_ = val; }
    // 이전 각도
    float prev_angle() const { return prev_angle_; }
    void set_prev_angle(float val) { prev_angle_ = val; }
    // 회전행렬
    WzMatrix& rotate_matrix(){ return rotate_matrix_; }
    //라이더 맵핑정보
    char					m_Animaping;
    //라이더인덱스
    DWORD					m_dwRiderIndex;
    //링크본노드번호
    int						m_iLinkNode;
    //컬러
    WzColor                 m_Color;
    //라이더파츠정보
    CWzUnitDraw*			m_ppRenderPart[RP_COUNT];
    //링크위치
    WzVector				m_vLinkPos;
    //지형에 붙이기위한 정보
    
    //생성시체크함수
    BOOL                    m_bSetInterpolate;
    //현재 애니메이션 아이디
    WzID			        current_animation_;
    //메인파츠 리소스코드
    DWORD body_skin_;
    //생성시 이펙트코드
    DWORD effect_code_;
    //회전.축보간(회전)
    float prev_angle_;
    WzMatrix rotate_matrix_;
    bool is_interpoate_;
    WzVector prev_axis_;
    WzVector interpolate_axis_;

    //! 탈것이 날개를 폈는지.
    bool is_spread_;
};

//라이더파츠값만 
CWzUnitDraw* Rider::GetRenderPart(int iIndex)
{
    if(iIndex==RP_BASE)
    {
        return NULL;
    }

    if(iIndex>=RP_COUNT)
    {
        return NULL;
    }
    else  
    {
        return m_ppRenderPart[iIndex];
    }
}
