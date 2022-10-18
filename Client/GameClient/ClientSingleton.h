#pragma once

namespace Client
{
    template <class T>
    class Singleton
    {
    public:
        // ������
        Singleton()
        {
            // ���߻���� ��� �� �����Ͱ��� �ٸ� �� �ִ�. �׷��� �� ���� �Ʒ����� �����ش�.
            int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;

            s_instance = (T*)( (int)this + offset );
        }

        // �Ҹ���
        virtual ~Singleton( void )
        {
            s_instance = NULL;
        }

        // �ν��Ͻ� ���
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