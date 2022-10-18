#include "SunClientPrecompiledHeader.h"
#include "XignCodeApi.h"
#include "GlobalFunc.h"

#if USING_XIGNCODE

//------------------------------------------------------------------------------ 
namespace XignCodeApi
{
    const TCHAR kXignCodeLicense[] = _T("X77cjikcIV8G");

    void vAddMessage(char* inLogFormat, ...)
    {
#ifdef _DEV_VER
        va_list stList;

        TCHAR	szLog[MAX_PATH + 1] = {0,};

        if( 0 != inLogFormat )
        {
            va_start( stList, inLogFormat );

            //vsprintf_s(szLog, MAX_PATH, inLogFormat,stList);
            vsprintf(szLog, inLogFormat, stList);

            va_end( stList );

            printf("%s", szLog );
        }
#endif //_DEV_VER
    }

    //------------------------------------------------------------------------------ 
    XignCode_UserInfo* xigncode_user_info()
    {
        static XignCode_UserInfo context;
        return &context;
    }

    //------------------------------------------------------------------------------ 
    bool SetInfo(xcstr _server_name, xcstr _user_id, xcstr _character_name, xint _map_id, xint _field_id, xcstr _field_name)
    {
        if (_user_id == NULL || _character_name == NULL || _field_name == NULL)
        {
            return false;
        }

        xigncode_user_info()->map_id = _map_id;
        xigncode_user_info()->field_id = _field_id;

        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _server_name, -1, xigncode_user_info()->server_name, XignCode_UserInfo::kStringLength);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _user_id, -1, xigncode_user_info()->user_id, XignCode_UserInfo::kStringLength);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _character_name, -1, xigncode_user_info()->character_name, XignCode_UserInfo::kStringLength);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _field_name, -1, xigncode_user_info()->field_name, XignCode_UserInfo::kStringLength);

        xigncode_user_info()->is_ready = true;

        return true;
    }
    //------------------------------------------------------------------------------ 
    bool	Init()
    {
        xbool res = TRUE;

        if (res)
        {
            res = ZCWAVE_SysInit();
            if (res == FALSE)
            {
                vAddMessage("[ERR] ZCWAVE_SysInit %d", ::GetLastError());
            }
        }
        if (res)
        {
            xulong xign_flag = ZSOPT_USESYSGUARD;

#ifdef _DEV_VER
            xign_flag = ZSOPT_SHOWSPLASH | ZSOPT_USESYSGUARD | ZSOPT_SHOWTRAY;
#endif //_DEV_VER

            res = ZCWAVE_SysEnter(kXignCodeLicense, _T("XIGNCODE"), xign_flag);
            if (res == TRUE)
            {
                res = ZCWAVE_SendCommand(ZCMD_SET_USER_INFO_CALLBACKW, XignCodeApi::MyGameGetUserInfoCallback, xigncode_user_info());
                if (res == FALSE)
                {
                    vAddMessage("[ERR] ZCWAVE_SendCommand %d", ::GetLastError());
                }
            }
            else
            {
                vAddMessage("[ERR] ZCWAVE_SysEnter %d", ::GetLastError());
            }
        }

        return (res == TRUE);
    }
    //------------------------------------------------------------------------------ 
    bool    Connect()
    {
        xbool res = ZCWAVE_Init();

        if (res == FALSE)
        {
            vAddMessage("[ERR] ZCWAVE_Init %d", ::GetLastError());
        }

        return (res == TRUE);
    }
    //------------------------------------------------------------------------------ 
    void Release()
    {
        ZCWAVE_SysExit();
        ZCWAVE_SysCleanup();
    }
    //------------------------------------------------------------------------------ 
    void Disconnect()
    {
	    ZCWAVE_Cleanup();
    }
    //------------------------------------------------------------------------------ 
    void CALLBACK OnProbeComplete(HANDLE CodeBox 
                                     ,CPACKETDATA Packet 
                                     ,PACKETDATA Buffer 
                                     ,SIZE_T BufferSize 
                                     ,PVOID Context)
    {
        MSG_CG_XIGNCODE_SERVER_QUERY_ACK ack;

        if (BufferSize > XIGNCODE_GAME_PACKET_SIZE)
        {
            return;
        }

        //memcpy_s(ack.buffer, XIGNCODE_GAME_PACKET_SIZE, Buffer, BufferSize)
        memcpy(ack.buffer, Buffer, BufferSize);

        // Context 게임서버로 셋팅
        GlobalFunc::SendPacketToGameServer(&ack, sizeof(ack));
    }
    //------------------------------------------------------------------------------ 
    bool RecvPacket(MSG_BASE* pk)
    {
        if ((pk->m_byCategory == CG_ARMORKIT) && (pk->m_byProtocol == CG_XIGNCODE_SERVER_QUERY_CMD))
        {
            MSG_CG_XIGNCODE_SERVER_QUERY_CMD* cmd = static_cast<MSG_CG_XIGNCODE_SERVER_QUERY_CMD*>(pk);

            if (ZCWAVE_ProbeEx((const char*)(cmd->buffer), 
                                XIGNCODE_GAME_PACKET_SIZE, 
                                XignCodeApi::OnProbeComplete,
                                NULL) == FALSE)
            {
                vAddMessage("[ERR] ZCWAVE_ProbeEx");
                return true;
            }
        }

        return false;
    }
    //------------------------------------------------------------------------------ 
    xstatus XCALL MyGameGetUserInfoCallback(xuint info_id, xwstr buffer, xsize buffer_size, xpvoid context)
    {
        buffer_size /= 2; //!< 버프의 크기가 넘어옴
        XignCode_UserInfo* info = static_cast<XignCode_UserInfo*>(context);
        if (info != NULL && info->is_ready == TRUE)
        {
            switch (info_id)
            {
            case XUID_USER_ACCOUNT:
                {
                    _snwprintf(buffer, buffer_size, L"%s.%s", info->server_name, info->user_id);    

#if (XIGNCODE_VERSION >= 111215)
                    return X_SUCCESS;
#else
                    return X_OK;
#endif
                    
                }



            case XUID_USER_NAME:
                {
                    wcsncpy(buffer, info->character_name, buffer_size/*80*/);

#if (XIGNCODE_VERSION >= 111215)
                    return X_SUCCESS;
#else
                    return X_OK;
#endif
                }



            case XUID_USER_MAP_POINT:
                {
                    _snwprintf(buffer, buffer_size, L"%d, %d, %s", info->field_id, info->field_id, info->field_name);   

#if (XIGNCODE_VERSION >= 111215)
                    return X_SUCCESS;
#else
                    return X_OK;
#endif
                }
                

            case XUID_USER_EXTRA:
                return ZCWAVE_E_USER_INFO_ACCESS_DENIED;
            }
        }

        return ZCWAVE_E_USER_INFO_IS_NOT_READY;
    }

};

#endif //USING_XIGNCODE