#pragma once

#include "wzLogDefine.h"


namespace WzLog
{
    class WzLogHandler
    {
    protected:
        WzLogHandler() {};   // ��ӿ� ���� ������ ��� �� �������̽�

    public:
        virtual ~WzLogHandler() {}

        BOOL Initialize() { return OnInitialize(); }
        void Release() { OnRelease(); }

        virtual void WriteLog( WZLOG_TYPE level, const char* message ) {}
        virtual void SetFilter( WZLOG_TYPE LogLevel, BOOL bEnable ) {}
        virtual BOOL GetFilter( WZLOG_TYPE LogLevel ) { return FALSE; }

    private:

        virtual BOOL OnInitialize() { return FALSE; }
        virtual void OnRelease() {}

    };
}