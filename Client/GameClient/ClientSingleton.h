#pragma once

namespace Client
{
    template <class T>
    class Singleton
    {
    public:
        // 생성자
        Singleton()
        {
            // 다중상속의 경우 이 포인터값이 다를 수 있다. 그래서 이 값을 아래에서 더해준다.
            int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;

            s_instance = (T*)( (int)this + offset );
        }

        // 소멸자
        virtual ~Singleton( void )
        {
            s_instance = NULL;
        }

        // 인스턴스 얻기
        static T* Instance( void )
        {
            return s_instance;
        }

        static void ReleaseInstance( void )
        {
            if( s_instance != NULL )
            {
                delete s_instance;
                s_instance = NULL;
            }
        }

    private:
        static T* s_instance;
    };


    template <class T> 
        T* Singleton<T>::s_instance = NULL;

};