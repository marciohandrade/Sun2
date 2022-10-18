//------------------------------------------------------------------------------
/**

������ �ؽ��ĸ� �����ϴ� �޴���

@author
		���翵
@since
		[6/14/2005]
@remarks
	- AddRef() / RemoveRef() �߰�
	  [6/22/2005]

	- IconImage���� �ؽ��� ��� �������ְ� ��.

@TODO
	- Render �κп��� �پ��ϰ� ȿ���� �����ֵ��� �Ѵ�.
*/


#ifndef ICONIMAGE_H 
#define ICONIMAGE_H

class IconMgr;
#ifdef _NA_000000_20130114_RENEWER_UI
#define ICON_IMAGE_SIZE (32)
#else
#define ICON_IMAGE_SIZE (38)
#endif


class IconImage 
{
public:
	friend class IconMgr;

	IconImage();
	~IconImage();

public:

    enum ACTIVE_EFFECT_STATE
    {
        ACTIVE_EFFECT_READY,
        ACTIVE_EFFECT_START,
        ACTIVE_EFFECT_END
    };

	void	Create(HANDLE	hTexture, int iSrcWidth, int iSrcHeight);
	void	Destroy();

	void	Render(RECT * pDest, RECT * pSrc = NULL);
	void	Render(RECT * pDest,WzColor* Color, RECT * pSrc = NULL);

	void	RenderCooldown(RECT * pDest, float fRate);
	void	RenderWarning(RECT *pDest, float fRate);

    void RenderWarningSpinCooltime(RECT* rect, float rate);
    void RenderSpinCooltime(RECT* rect, float rate);
    void RenderCoverImage(RECT* rect, WzColor color);
    void    RenderDeactivated(RECT *pDest);
	void	RenderWarning(RECT *pDest);
	void	RenderBlocked(RECT * pDest);
	void	RenderRewardList(RECT * pDest);
    void RenderFreezing(RECT* pDest);
	int		GetSrcWidth() const;
	int		GetSrcHeight() const;

private:
	DWORD	m_dwKey;	
	DWORD	m_dwResourceID;
	HANDLE	m_hTexture;
	int		m_iSrcWidth;
	int		m_iSrcHeight;
    ACTIVE_EFFECT_STATE    active_render;      //Ȱ��ȭ ��ũ �غ�/��/��
    int     active_start_tick;  //Ȱ��ȭ ��ũ ���� �ð�

};

#endif
// ICONIMAGE_H