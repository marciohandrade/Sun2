#pragma once

namespace util
{
	// _Ty�� �ش��ϴ� Ÿ���� ���� �����ڿ� ���Կ����ڸ� �����ؾ� �Ѵ�.
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
