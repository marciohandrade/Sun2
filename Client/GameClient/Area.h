#pragma once

class CArea
{
public:
	CArea(void);
	virtual ~CArea(void);

	virtual BOOL IsInside(WzVector* pPos) = 0;
};


class CTempAttackArea : public CArea
{
	WzVector m_VecSrcPostion;
	WzVector m_VecSrcDir;
	WzVector m_VecDest;
	float m_fRadian;

public:
	CTempAttackArea(WzVector * pVecSrcPostion, 
		WzVector * pVecSrcDir, 
		float fRadian)
		: m_VecSrcPostion(*pVecSrcPostion)
		, m_VecSrcDir(*pVecSrcDir)
		, m_fRadian(fRadian)	{}

	virtual BOOL IsInside(WzVector* pPos);
};
