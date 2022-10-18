#pragma once

class CDialogManager;

class CDlgBaseWZ 
{
public:
	CDlgBaseWZ();
	virtual ~CDlgBaseWZ();

	virtual bool				OnCreate(DWORD dwID, DWORD dwStyle, HWND hParentWnd, HINSTANCE hInstance, void* pDraw, CDialogManager* pPtr) PURE;
	virtual void				OnDraw(bool bl) PURE;
	virtual void				ShowWindowWZ(ENUM_STATEWZ eState) { m_eState = eState; }
    virtual void                SetScale(float width, float height) PURE;

public:

    DWORD                       GetDialogType() { return m_dwDialogType; }
	void						SetType(DWORD dwType) { m_dwDialogType = dwType; }
	bool						IsType(DWORD dwType)
	{
		if(m_dwDialogType&dwType)
			return true;
		return false;	
	}
	void						ADDType(DWORD dwType){m_dwDialogType |= dwType;}	
	
	bool						IsState(ENUM_STATEWZ eState)
	{
		if(m_eState == eState)
			return true;
		return false;
    }

protected:
    void SetParentWnd( HWND hParentWnd ) { m_hParentWnd = hParentWnd; }
    void SetInstanceHandle( HINSTANCE hInstance ) { m_hInstance = hInstance; }

    HWND GetParentWnd() { return m_hParentWnd; }
    HINSTANCE GetInstanceHandle() { return m_hInstance; }

private:
	ENUM_STATEWZ				m_eState;
	HWND						m_hParentWnd;
	HINSTANCE					m_hInstance;
	DWORD						m_dwDialogType;
};

