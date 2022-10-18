// TplSingleton.h: interface for the CTplSingleton class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#define _instance(_class)			pharos::helper::Singleton<_class>::GetInstance()
#define _releaseInstance(_class)	pharos::helper::Singleton<_class>::ReleaseInstance()

namespace pharos
{
	namespace helper
	{
		template <typename _Tp> class Singleton : public _Tp
		{

		public:
			static _Tp* GetInstance()
			{
				if(m_instance == NULL){
					m_instance = (_Tp*)new Singleton();
				}
				return m_instance;
			};

			static void ReleaseInstance()
			{
				delete m_instance;
				m_instance = NULL;
			};

		protected:
			Singleton(){};
			~Singleton(){};

		private:
			Singleton(const Singleton& source){};
			static _Tp* m_instance; //singleton class instance
		};

		//static class member initialisation.
		template <typename _Tp> _Tp* Singleton<_Tp>::m_instance = NULL;
	} // end namespace system
}  // end namespace pharos
