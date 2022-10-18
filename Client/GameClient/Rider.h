
#pragma once

struct sRiderInfo;
class ObjectAniEventController;

enum RiderParts
{
	RP_BASE,	//Ż�ͱ⺻����
	RP_BODY,	//����
	RP_BOOTS,	//����	
	RP_HELM,	//����
	RP_COUNT,
};

class Rider : public ObjectAniEventController
{
public:
	Rider( void );
	virtual ~Rider( void );

    //���̴� ����
	virtual BOOL Create(DWORD dwRiderIndex, Player* master_ptr, BOOL summon_effect);
    
    //���ҽ�����
	virtual void Destroy( void );
	
	//------------------------------------------------------------------------------
	//! Qualifier: �ִϸ��̼Ǽ���
	//! Parameter: WzID wzID : �ִϸ��̼� �ڵ�
	//! Parameter: BOOL bLoop : loop ����
    //! Parameter: BOOL bRestart : bLoop == TRUE �̸� ������ bRestart = FALSE�� ���õ�
	//------------------------------------------------------------------------------
	void SetAnimation(WzID wzID, BOOL bLoop = FALSE, BOOL bRestart = TRUE);

    //�ִϸ��̼ǰ��
	virtual void AnimateUnit(Player* master_ptr);
    
    //�ִϸ��̼� �����Ӱ��
	virtual BOOL Process(DWORD dwTick, Player* master_ptr);

    //����:Display��
	virtual BOOL Render( St_RenderCommand *pCommand);

    //����
	virtual BOOL Render();
	
    //���̴�����(���ΰ�Ż�Ϳ�)
	void CopyRider(Rider *pRider, bool is_with_parts = true);

    //���̴�����(���Կ���Ż�Ϳ�)
    void CopyRiderBody(DWORD dwRiderIndex, Player* master_ptr);

    //��ȯ(��ġ,ȸ��)
	void Transform(CWzUnitDraw* pMaster);

    //���̽�����(���̴�����)
    CWzUnitDraw* GetRenderPart(){return m_ppRenderPart[RP_BASE];}
    
    //���̴�����(���̴�����)
    inline CWzUnitDraw* GetRenderPart(int iIndex);
    
    //��ġ.ȸ��.ũ��
    void SetScale(WzVector vScale);

    void SetPosition(WzVector *pPos) { if(GetRenderPart()) GetRenderPart()->SetPosition(*pPos);}

	void SetRotation(const WzVector* normal_axis, float master_angle);

    void SetForceLod(int iLod);

    //���� ����.Ż��.����Ÿ��(�ν�)
    void AttachRiderPart(int nIdx, DWORD dwItemCode);

    //------------------------------------------------------------------------------
    //! Returns:   void
    //! Qualifier: z-base ���� ���� 
    //! Parameter: int nIdx
    //!             0 = ����
    //!             1 = ����	
    //!             2 = ����
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
    
    //! Ż���� ���� ��������
    bool is_spread() { return is_spread_; }
    void set_spread(bool val) { is_spread_ = val; }

    bool is_possible_fly();

    WzID current_animation() const { return current_animation_; }
    void set_current_animation(WzID val) { current_animation_ = val; }

private:
    // ��ȯ����Ʈ ����
    DWORD effect_code() const { return effect_code_; }
    void set_effect_code(DWORD val) { effect_code_ = val; }
    // ��������
    bool is_interpoate() const { return is_interpoate_; }
    void set_is_interpoate(bool val) { is_interpoate_ = val; }
    // ������
    WzVector& interpolate_axis(){ return interpolate_axis_; }
    void set_interpolate_axis(const WzVector& val) { interpolate_axis_ = val; }
    // ���� �븻��
    WzVector& prev_axis(){ return prev_axis_; }
    void set_prev_axis(const WzVector& val) { prev_axis_ = val; }
    // ���� ����
    float prev_angle() const { return prev_angle_; }
    void set_prev_angle(float val) { prev_angle_ = val; }
    // ȸ�����
    WzMatrix& rotate_matrix(){ return rotate_matrix_; }
    //���̴� ��������
    char					m_Animaping;
    //���̴��ε���
    DWORD					m_dwRiderIndex;
    //��ũ������ȣ
    int						m_iLinkNode;
    //�÷�
    WzColor                 m_Color;
    //���̴���������
    CWzUnitDraw*			m_ppRenderPart[RP_COUNT];
    //��ũ��ġ
    WzVector				m_vLinkPos;
    //������ ���̱����� ����
    
    //������üũ�Լ�
    BOOL                    m_bSetInterpolate;
    //���� �ִϸ��̼� ���̵�
    WzID			        current_animation_;
    //�������� ���ҽ��ڵ�
    DWORD body_skin_;
    //������ ����Ʈ�ڵ�
    DWORD effect_code_;
    //ȸ��.�ຸ��(ȸ��)
    float prev_angle_;
    WzMatrix rotate_matrix_;
    bool is_interpoate_;
    WzVector prev_axis_;
    WzVector interpolate_axis_;

    //! Ż���� ������ �����.
    bool is_spread_;
};

//���̴��������� 
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
