#pragma once

#include <ReliableRatioRandomizer.h>
#include <ItemOptionHeader.h>
#include "Randomizer.h"

#ifndef _NA_000251_20100727_SOCKET_SYSTEM
struct SOCKET_RATIO
{
	BYTE Ratio[SOCKET_MAX];
};

class RatioManager
{
public:
	RatioManager(void);
	~RatioManager(void);

	VOID Init();
	VOID Release();

	// 인첸트 확률 선택기
	enum { MAX_ENCHANT_SELECTER_NUM = 6 };
	ReliableRatioRandomizer<2> m_EnchantRatioSelecter[MAX_ENCHANT_SELECTER_NUM];

	// 소켓 생성 성공 확률
	SOCKET_RATIO m_SocketRatio[RANK_MA-RANK_D];

};

extern RatioManager g_Ratio;
#endif // _NA_000251_20100727_SOCKET_SYSTEM

//!~ _NA_000000_20130612_RANDOMIZER_CHANGE
class RandomManager
{
public:
    RandomManager();
    ~RandomManager();

    void Init();
    void Release();

    Randomizer randomizer_enchant_;
    Randomizer randomizer_compose_;
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    Randomizer randomizer_awakening_;
    Randomizer randomizer_evolution_;
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
};
//~! _NA_000000_20130612_RANDOMIZER_CHANGE