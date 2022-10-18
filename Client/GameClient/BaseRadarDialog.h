#pragma once
#include "uiBase.h"
#include "RadarInfoParser.h"

// 전방 선언
class SCObject;
class NPC;
class Monster;
class Player;

enum eFULL_RADAR_STATE
{
	eFULL_RADAR_STATE_ALL = 0,
	eFULL_RADAR_STATE_ALPHA = 1,
	eFULL_RADAR_STATE_MAX = 2,

	eMINI_MAP = 99,
};


struct PORTAL_OFFSET_PARAM
{
	//인풋인지여부
	BOOL bInPut;

	union 
	{
		//인풋정보(그릴때 옵셋값을 붙여서 그려준다)
		struct
		{
			int  nCol,nRow;
		}INPUT_INFO;

		//아웃풋(포탈이름이 그려지는 x,y,w.h값)		
		struct
		{
			RECT nRect;		//포탈텍스트 영역
		}OUTPUT_INFO;
	};

};

class BaseRadarDialog : public uiBase
{
public:
	BaseRadarDialog(InterfaceManager *pUIMan);
	virtual ~BaseRadarDialog();
	virtual void Init(CDrawBase * pDrawBase );
	virtual void Release();
	virtual void NetworkProc(MSG_BASE * pMsg );
	virtual VOID Process(DWORD dwTick);

    void SetMapName(CCtrlStaticWZ* pControl, TCHAR* pszMapName);
    BOOL RenderPortalName(char* PortalID, CCtrlPictureWZ * pCtrlPicture,void *pParm=NULL);
    void ResetRadarState(){m_btRadarState = eFULL_RADAR_STATE_MAX;}
    void IncreaseRadarState();
    BYTE GetRadarState(){return m_btRadarState;}
    WORD GetRadarImageWidth() const { return m_wRadarImageWidth; }
    WORD GetRadarImageHeight() const { return m_wRadarImageHeight; }
    virtual bool GetPictureSize(RECT& rect) { return false; };

    void SetRadarState(eFULL_RADAR_STATE value) { m_btRadarState = (BYTE)value; }
    WzVector CalculatePointPosition(float x,float y, float z);
    bool GetShowResize() { return m_bResizefullmap; }

protected:
    virtual bool is_full_map() { return false; }

	// 랩핑 함수. 인자로 이미지 컨트롤을 받는다. 
	void Wrap_loadMapImage(CControlWZ * pControl,  DWORD fieldID, BYTE btImgType  = 0, TCHAR* file_name = NULL); // btImgType (0 == jpg , 1 == dds)
	void Wrap_render(CCtrlPictureWZ * pCtrlPicture);
	BOOL Wrap_CheckClickedPosition(CCtrlPictureWZ * pCtrlPicture);

    void ShowToolTipMapName(CCtrlStaticWZ* pControl, TCHAR* pszMapName);
    bool IsShowToolTipForMapName()	{	return m_bShowTooltipForMapName; }

    static void DrawControlImage(CControlWZ* control_ptr, void* parameter);

	virtual BOOL InterSectRect(POINT point) {if(eFULL_RADAR_STATE_ALPHA <= m_btRadarState)return FALSE; else return TRUE;}
	
    // 포탈 출력 관련
    void CalcPortalOffset(CCtrlPictureWZ * pCtrlPicture);

protected:
    // 맵 이미지 컨트롤 사이즈
	WORD m_wRadarImageWidth;
	WORD m_wRadarImageHeight;

    BYTE m_btRadarState;

    // 포탈 출력 관련
	int m_PortalOffset[3][10];
	std::vector<RECT> ListNpcRect_;

    bool m_bResizefullmap;

private:
	bool m_bShowTooltipForMapName;

protected:

#ifdef _NA_0_20111013_RADAR_PATHFINDING
    bool PathFindMove( CControlWZ* pPictureControl );
    bool PathFindMoveToObject( Object* pObject );
#endif //_NA_0_20111013_RADAR_PATHFINDING
};

inline	void BaseRadarDialog::IncreaseRadarState()
{
	if (m_btRadarState >= eFULL_RADAR_STATE_MAX) 
    {
		m_btRadarState = 0; 
    }
	else 
    {
		m_btRadarState++;
    }

	RECT rt;
	memset(&rt, 0, sizeof(rt));
	if (eFULL_RADAR_STATE_ALPHA <= m_btRadarState)
	{
		GetDialogWZ()->GetClientRect(&rt);
	}
	DisableInterSectRect(rt);
}

