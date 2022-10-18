#pragma once

#pragma warning(push)
#pragma warning(disable:4702)

#include <map>

#pragma warning(pop)

class TestCounter
{
	Singleton( TestCounter );

public:
	class Counter
	{
		friend class TestCounter;

	public:
				void		Start();
				void		Stop();

				void		Inc( DWORD val );
				void		Dec( DWORD val );

				DWORD		GetCount();
				DWORD		GetTimeGap();

	private:
				Counter();
				~Counter();

				DWORD		m_tickStart, m_tickStop;
				DWORD		m_value;
	};

public:
			Counter*		Get( DWORD subjectID );
			Counter*		Add( DWORD subjectID );
			void			Remove( DWORD subjectID );
			void			RemoveAll();

			void			Inc( DWORD subjectID, DWORD val );
			void			Dec( DWORD subjectID, DWORD val );

private:
	typedef	std::map	< DWORD, Counter* >		MapCounter;

			eXtreme::Mutex	m_mutex;
			MapCounter		m_mapCounter;
};