#pragma once
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "SUNRankingControl.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "FTextRender.h"

class SUNRankingPointControl : public SUNRankingControl
{
public:
	SUNRankingPointControl(void);
	virtual ~SUNRankingPointControl(void);

public:
	virtual void Init(SUNRankingSlot* parent_slot, CControlWZ* total_control_ptr);
	virtual void Update();
	virtual void Render();
	virtual void Release();

private:
	FTextParser				m_FTextParser;
	FTextSentence			m_FTextSentence;
	FTextParam				m_FTextParam;
	FTextRender				m_FTextRender;
};
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
