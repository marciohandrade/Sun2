#include "StdAfx.h"
#include ".\ratiomanager.h"

#ifndef _NA_000251_20100727_SOCKET_SYSTEM
RatioManager g_Ratio;

RatioManager::RatioManager(void)
{
}

RatioManager::~RatioManager(void)
{
	Release();
}

VOID RatioManager::Release()
{
}

VOID RatioManager::Init()
{

	// ÀÎÃ¾Æ® ¼º°ø È®·ü ÃÊ±âÈ­
	for( int i = 0 ; i < MAX_ENCHANT_SELECTER_NUM ; ++i )
		m_EnchantRatioSelecter[i].Clear();

	BOOL rt = m_EnchantRatioSelecter[0].AddRatio( 1, 30, 100 ); ASSERT(rt);
	rt = m_EnchantRatioSelecter[1].AddRatio( 1, 45, 100 ); ASSERT(rt);
	rt = m_EnchantRatioSelecter[2].AddRatio( 1, 60, 100 ); ASSERT(rt);
	rt = m_EnchantRatioSelecter[3].AddRatio( 1, 70, 100 ); ASSERT(rt);
	rt = m_EnchantRatioSelecter[4].AddRatio( 1, 80, 100 ); ASSERT(rt);
	rt = m_EnchantRatioSelecter[5].AddRatio( 1, 90, 100 ); ASSERT(rt);


	// ¼ÒÄÏ »ý¼º ¼º°ø È®·ü ÃÊ±âÈ­
	// D·©Å©
	m_SocketRatio[RANK_D-RANK_D].Ratio[SOCKET_1] = 70;
	m_SocketRatio[RANK_D-RANK_D].Ratio[SOCKET_2] = 0;
	m_SocketRatio[RANK_D-RANK_D].Ratio[SOCKET_3] = 0;
	// C·©Å©
	m_SocketRatio[RANK_C-RANK_D].Ratio[SOCKET_1] = 70;
	m_SocketRatio[RANK_C-RANK_D].Ratio[SOCKET_2] = 10;
	m_SocketRatio[RANK_C-RANK_D].Ratio[SOCKET_3] = 0;
	// B·©Å©
	m_SocketRatio[RANK_B-RANK_D].Ratio[SOCKET_1] = 70;
	m_SocketRatio[RANK_B-RANK_D].Ratio[SOCKET_2] = 10;
	m_SocketRatio[RANK_B-RANK_D].Ratio[SOCKET_3] = 5;

}
#endif // _NA_000251_20100727_SOCKET_SYSTEM

//!~ _NA_000000_20130612_RANDOMIZER_CHANGE
RandomManager::RandomManager()
{

}

RandomManager::~RandomManager()
{

}

void RandomManager::Init()
{
    randomizer_enchant_.Create(100);
    randomizer_compose_.Create(1000);
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    randomizer_awakening_.Create(100);
    randomizer_evolution_.Create(100);
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
}

void RandomManager::Release()
{
    randomizer_enchant_.Destroy();
    randomizer_compose_.Destroy();
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    randomizer_awakening_.Destroy();
    randomizer_evolution_.Destroy();
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
}

//~! _NA_000000_20130612_RANDOMIZER_CHANGE
