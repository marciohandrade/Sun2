#pragma once


class CWzUnitDraw;


class AttachmentInfo
{
public:
	AttachmentInfo()
	{
		m_eAttachType = ATTACHTYPE_GENERAL;
		m_SubType = 0;

		m_ResourceID = 0;
		m_BoneIndex = -1;

		m_bSpecularVariation = FALSE;

        MatrixIdentity(&m_RotateMatrix);

        m_Scale = 1.0f;
        m_ItemCode = 0;

        m_vOffset.x = 0.0f;
        m_vOffset.y = 0.0f;
        m_vOffset.z = 0.0f;
	}

private:
	eATTACHTYPE		m_eAttachType;
	int				m_SubType;

	RESOURCECODE	m_ResourceID; // ���ҽ� ���̵�
	int				m_BoneIndex;;   // ���� ����ȣ

	BOOL			m_bSpecularVariation;

    WzMatrix		m_RotateMatrix;
    WzVector        m_vOffset;

    float           m_Scale;

    //! �������ϰ�� ������ �ڵ嵵 ���� �����Ѵ�.
    //! �����ý��������ϸ鼭 �߰�
    DWORD           m_ItemCode;

public:
	eATTACHTYPE GetAttachType() const { return m_eAttachType; }
	void SetAttachType(eATTACHTYPE value) { m_eAttachType = value; }

	int GetSubType() const { return m_SubType; }
	void SetSubType(int value) { m_SubType = value; }

	RESOURCECODE GetResourceID() const { return m_ResourceID; }
	void SetResourceID( RESOURCECODE value) { m_ResourceID = value; }

	int GetBoneIndex() const { return m_BoneIndex; }
	void SetBoneIndex(int value) { m_BoneIndex = value; }

	BOOL IsSpecularVariation() const { return m_bSpecularVariation; }
	void SetSpecularVariation(BOOL value) { m_bSpecularVariation = value; }

    WzMatrix GetRotateMatrix() const { return m_RotateMatrix; }
    void SetRotateMatrix( const WzMatrix& value) { m_RotateMatrix = value; }

    WzVector GetOffset() const { return m_vOffset; }
    void SetOffset(const WzVector& val) { m_vOffset = val; }

    float GetScale() const { return m_Scale; }
    void SetScale(const float& scale) { m_Scale = scale; }
    
    DWORD GetItemCode() const { return m_ItemCode; }
    void SetItemCode(DWORD val) { m_ItemCode = val; }
};


class ObjectAttachment
{
public:
	ObjectAttachment();

	BOOL InitializeAttachment( BOOL bPlayer, const AttachmentInfo& pInfo, float fRequestRateResizeTexture );
	BOOL ReleaseAttachment();

	//------------------------------------------------------------------------------
	//! Qualifier: ĳ���Ϳ� �����Ǵ� ������ �ִϸ��̼�
	//! Parameter: WzID animID
	//! Parameter: BOOL bLoop
    //! Parameter: BOOL bRestart : bLoop == TRUE �̸� ������ bRestart = FALSE�� ���õ�
	//------------------------------------------------------------------------------
	void SetAnimation(WzID animID, BOOL bLoop, BOOL bRestart = TRUE);
	void Animate( CWzUnitDraw* pBasePart, const WzVector& VisiblePosition, float fHeightOffset );
	void ProcessAnimation( DWORD dwTick );
	void ProcessInterpolation( CWzUnitDraw* pBasePart, DWORD dwTick);
	void ApplyInterpolateAccesory( CWzUnitDraw* pBasePart );

	void Render();

private:
	CWzUnitDraw*	m_pUnitDraw;

	AttachmentInfo m_AttachmentInfo;

	WzID			m_animID;   
	BOOL			m_bAnimate;

	WzVector		m_wvInterpolateOffsetAttachment;
	int				m_iInterpolateTick;
	WzVector        m_vOldPos;
	float			m_fAccelerate;



public:
	AttachmentInfo& GetAttachmentInfo() { return m_AttachmentInfo; }
	const AttachmentInfo& GetAttachmentInfo() const { return m_AttachmentInfo; }
	//void SetAttachmentInfo(const AttachmentInfo& value) { m_AttachmentInfo = value; }


	CWzUnitDraw* GetUnitDraw() const { return m_pUnitDraw; }
};