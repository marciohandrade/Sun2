// ObserverControl.h: interface for the ObserverControl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __OBSERVERCONTROL_H__
#define __OBSERVERCONTROL_H__

#pragma once


namespace util
{
    template <typename T> 
    class ObserverControl
    {
        typedef std::list<T*> OBSERVER_LIST;

    private:
        template <class T_result, class T_arg>
        class Firer
        {
        public:
            explicit Firer1(T_result (T::*pMember)(T_arg), T_arg arg)
                : m_pMemFunc(pMember), m_arg(arg)
            {}

            T_result operator()(T *pClass) const {
                return ((pClass->*m_pMemFunc)(m_arg));
            }

        private:
            T_result (T::*m_pMemFunc)(T_arg);
            T_arg    m_arg;
        };

    public:
        ObserverControl() {}
        ~ObserverControl() {}

        void RegisterObserver(T* pObserver)     { listObserver_.push_back(pObserver); }
        void UnRegisterObserver(T* pObserver)   { listObserver_.remove(pObserver); }
        OBSERVER_LIST& GetObservers()           { return listObserver_; }

        template <class T_result, class T_arg>
        void _Fire(T_result (T::*pMember)(T_arg), T_arg arg)
        {
            std::for_each(listObserver_.begin(), listObserver_.end(), Firer<T_result, T_arg>(pMember, arg));
        }

    protected:
        OBSERVER_LIST listObserver_;
    };
}

// sample
//////////////////////////////////////////////////////////////////////////
//
// template <typename T> 
// class IObserver
// {
//     enum eEvent{ start };
// 
//     void Register() {
//         WarEventControl::Instance()->RegisterObserver(this);
//     }
// 
//     void OnEvent(eEvent ObEvent) { 
//         T* pObserver = static_cast<T*>(this);
//         return pObserver->OnEvent(ObEvent); 
//     }
// };
// 
// 
// class WarEventControl : public Singleton<WarEventControl>, public ObserverControl<IObserver>
// {
//     void Fire() {
//         _Fire<void, IObserver::eEvent>(&IObserver::OnEvent, IObserver::eEvent::start);
//     }
// };

//////////////////////////////////////////////////////////////////////////
//
// class GameDomination : public IObserver<GameDomination>
// {
//     void OnEvent(eEvent ObEvent) {
//         // process
//     }
// };

#endif // __OBSERVERCONTROL_H__

