#pragma once

#include "Singleton.h"
#include "SunEventInfoParser.h"
#include <list>

class Player;
#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
class IEventControlPeakTime;
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT
class PeakTimeEvent : public util::Singleton< PeakTimeEvent >
{
public:		PeakTimeEvent(VOID);
			~PeakTimeEvent(VOID);

public:		VOID				Init();
            // MasterServer로부터 서버키를 받은 후 대상 월드 지정된 이벤트 정리, _NA_002316_20110310_WORLD_SPECIFIC_PEAKTIME_EVENT
            void                SieveEventsByWorldId();
			FLOAT				GetEventExp( Player* pPlayer );
#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT 
			bool				AddGmEvent(eEVENT_MODE event_mode, int start_date, WORD start_time, int end_date, WORD end_time, int reward_ratio);
            bool                AddGmEvent(EVENT_INFO event_info);
                                //해당 이벤트 정보와 기간중복되는 이벤트 제거
            bool                DeleteExistOverlapEvent(EVENT_INFO& event_info);
            bool                DeleteProgressEvent();
            bool                SendGMEventList(Player* gm_player);

            void                SetEventControlClass(IEventControlPeakTime* controller) {event_control_ = controller;}
            IEventControlPeakTime* GetEventControlClass() {return event_control_;}

            VOID                SendProgressEventExpRateInfo(Player* player);
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

private:	VOID				AddEvent( EVENT_INFO& Eventnfo );
			EVENT_INFO*			FindEvent( DWORD dwEventID );
#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
			static const INT	GetGmEventTempIndex() { ++gmevent_eventid_counter_; return gmevent_eventid_counter_;}
            VOID                SendEventNotifyPacket(ePEAKTIME_EVENT_NOTIFY type, FLOAT exp_ratio, 
                                                      BYTE start_day = 0, DWORD start_time = 0,
                                                      BYTE end_day = 0, DWORD end_time = 0 );
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT



private:	EVENTINFO_MAP			m_mapPeakTimeEvent;
#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
			static INT gmevent_eventid_counter_;

            IEventControlPeakTime* event_control_;
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

			// 현 시점에서 상시 계산되도 동일한 결과가 나오는 수치 할당
private:	struct sFIXEXP_VALUEs
			{
				FLOAT		EXPVAL_USERTYPE;
				FLOAT		EXPVAL_NEWCHAR_UNDER30LEVEL;
				FLOAT		EXPVAL_ACCUMULATED_TIME_60x2;
				FLOAT		EXPVAL_ACCUMULATED_TIME_60x4;
				FLOAT		EXPVAL_ACCUMULATED_TIME_60x6;
			};
			enum eFIXEXP_USRTYPEs
			{
				EXPUSR_NORMAL				= 0,
				EXPUSR_ONLYPCROOM			,		// Normal + PC
				EXPUSR_UPPERBOUND			,
			};
			typedef sFIXEXP_VALUEs			FIXED_EXP_USRVALUES[EXPUSR_UPPERBOUND];

private:	static const int				MAX_LEVELED_ARRSIZE = 256;
			struct LEVELED_EXP_ARRAY
			{
				sFIXEXP_VALUEs				ARRAY[MAX_LEVELED_ARRSIZE];
				sFIXEXP_VALUEs&				operator[] ( DWORD idx ) { return ARRAY[idx]; }
				const sFIXEXP_VALUEs&		operator[] ( DWORD idx ) const { return ARRAY[idx]; }
			};
			typedef LEVELED_EXP_ARRAY		LEVELED_EXP_USRVALUES[EXPUSR_UPPERBOUND];

private:	struct CalcedExp
			{
				VOID						Init()
				{ ZeroMemory( &m_ExpValues, sizeof(m_ExpValues) ); }
				LEVELED_EXP_ARRAY&			operator[] ( DWORD idx ) { return m_ExpValues[idx]; }
				const LEVELED_EXP_ARRAY&	operator[] ( DWORD idx ) const { return m_ExpValues[idx]; }

				LEVELED_EXP_USRVALUES		m_ExpValues;
			};
private:	CalcedExp						m_ExpInfo;
			friend class IEventControlPeakTime;
};