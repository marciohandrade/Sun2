#ifndef     __UIBASE_DEFINE_H__
#define     __UIBASE_DEFINE_H__


//------------------------------------------------------------------------------ 
enum eFIS
{
    eFIS_IDLE,				////eFateItemSystem_IDLE
    eFIS_MOVIE_RUNNING,  
    eFIS_WAIT_RESPONSE,
    eFIS_RESULT_SUCCESS,
    eFIS_END,
};

//------------------------------------------------------------------------------
enum
{
    TIMER_STAT_NONE = 0,
    TIMER_STAT_IDLE,
    TIMER_STAT_FINISH,
};

//------------------------------------------------------------------------------
class CTimerSimple
{
private:
    bool            m_bActivated;
    DWORD           m_dwStartSecond;
    DWORD           m_dwEndSecond;

public:
    CTimerSimple() 
        : m_bActivated(false)
        , m_dwStartSecond(0)
        , m_dwEndSecond(0)
      {
      }

      bool On( DWORD dwSecond = 1 )
      {
          if (m_bActivated)
              return false;

          m_dwStartSecond = clock_function::GetTickCount();
          m_dwEndSecond = m_dwStartSecond + (dwSecond * 1000);
          m_bActivated = true;

          return true;
      }

      bool OnFloat( FLOAT fSecond = .1f )
      {
          if (m_bActivated)
              return false;

          m_dwStartSecond = clock_function::GetTickCount();
          m_dwEndSecond = m_dwStartSecond + (DWORD)(fSecond * 1000.0f);
          m_bActivated = true;

          return true;
      }

      void Off()
      {
          m_bActivated = false;
      }

      int Check()
      {
          if (!m_bActivated)
              return TIMER_STAT_NONE;

          if (m_dwEndSecond < clock_function::GetTickCount())
              return TIMER_STAT_FINISH;

          return TIMER_STAT_IDLE;
      }

      DWORD SpentTime()
      {
          DWORD spent_time = 0;
          if (m_bActivated)
          {
              spent_time = clock_function::GetTickCount() - m_dwStartSecond;
          }
          return spent_time;
      }
};

#endif  //  __UIBASE_DEFINE_H__
