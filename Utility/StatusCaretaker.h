#pragma once

namespace util
{
	// _Ty에 해당하는 타입은 복사 생성자와 대입연산자를 지원해야 한다.
	//////////////////////////////////////////////////////////////////////////
	template< typename _Ty >
	class StatusCaretaker
	{
	public:
		StatusCaretaker(void) : m_Status( _Ty()) {}
		~StatusCaretaker(void) {}

		void SetStatus(const _Ty& resStatus)
		{
		}

		void GetStatus(_Ty& desStatus) const
		{
		}

		_Ty& GetStatusRef() const
		{
			return m_Status;
		}

	private:
		_Ty m_Status;
	};
}
