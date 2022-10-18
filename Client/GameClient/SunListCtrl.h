//////////////////////////////////////////////////////////////////////////
// Name : SunListCtrl
// Desc : CCtrlListWZ �� ����� ������ �ִ»��¿��� �ΰ����� �ʿ���������
//        �־�Ѽ� �ֵ��� �صд�.
//////////////////////////////////////////////////////////////////////////

#ifndef SUNLISTCTRL_H
#define SUNLISTCTRL_H

#pragma once

typedef std::map <DWORD, void *> LIST_INFO_MAP;

template <class DataType>
class SunListCtrl
{
public:
	SunListCtrl(void);
	virtual ~SunListCtrl(void);

public:
	BOOL Create(int iUseReportSlot);
	void Destory();
	
public:
	DWORD		AddData( DataType * pData );
	BOOL		RemoveData( DWORD dwID );

	void		SetReportUseCnt( int iCount );
	int			GetReportUseCnt(void) const;

	DWORD		GetCurSelID();
	DataType	GetData( DWORD dwID );
    
protected:
	DWORD AllocData(void);
	BOOL DeallocData( DWORD dwID );

private:
	CCtrlListWZ *	m_pWzListCtrl;
	LIST_INFO_MAP	m_SunListInfoMap;
};	

template <class DataType>
SunListCtrl<DataType>::SunListCtrl(void)
{
}

template <class DataType>
SunListCtrl<DataType>::~SunListCtrl(void)
{
}


#endif
//SUNLISTCTRL_H