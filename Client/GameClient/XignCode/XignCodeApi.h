#pragma once

#if USING_XIGNCODE

//! 2011.2.8 / i4u4me / 
//! _WIN32_WINNT 0x500 에서는 StringCbCopyW, StringCbCatW 함수를 지원하지 않느다.
//! 윌비아 담당자 요청으로 _STRSAFE_H_INCLUDED_ undef 한다.

#undef _STRSAFE_H_INCLUDED_

//#include "../../../include/XignCode/IncludeListForXignCode.h"
#define XIGNCODE_INCLUDE_PATH( v )	 <../../../include/XignCode/##v/zwave_sdk_client.h>
#include XIGNCODE_INCLUDE_PATH( XIGNCODE_VERSION )

//------------------------------------------------------------------------------ 
namespace XignCodeApi
{
    struct XignCode_UserInfo
    {
        const static int kStringLength = 80;

        xbool is_ready;
        xwch user_id[kStringLength];
        xwch character_name[kStringLength];
        xwch server_name[kStringLength];

        xint map_id;
        xint field_id;
        xwch field_name[kStringLength];

        XignCode_UserInfo()
        {
            memset(this, 0, sizeof(*this));
        }
    };

    // user context
    XignCode_UserInfo* xigncode_user_info();

    //! 멀티 바이트로 받아서 유니코드로 컨버팅
    bool SetInfo(xcstr _server_name, xcstr _user_id, xcstr _character_name, xint _map_id, xint _field_id, xcstr _field_name);

    //! Init XIGNCODE LIB warpper function
    bool Init();
    void Release();

    //! 서버 연결 시점 호출
    bool Connect();

    //! 서버 접속 해제 시점
    void Disconnect();

    //------------------------------------------------------------------------------
    //! Returns:   bool : true : 해당 함수내에서 메세지를 처리하고 이후 프로세스에서 모두 무시
    //!                     false : 메세지 넘김
    //! Qualifier: xigncode 패킷이 왔는지 처리 여부
    //! Parameter: MSG_BASE * pk : 패킷 베이스
    //------------------------------------------------------------------------------
    bool RecvPacket(MSG_BASE* pk);

    //! XIGNCODE 코드 패킷 실행 후 결과 전송 콜백 함수
    void CALLBACK OnProbeComplete(HANDLE CodeBox
								     ,CPACKETDATA Packet
								     ,PACKETDATA Buffer
								     ,SIZE_T BufferSize
								     ,PVOID Context);

    //! 유니코드 콜백 사용
    xstatus XCALL MyGameGetUserInfoCallback(xuint info_id, xwstr buffer, xsize buffer_size, xpvoid context);
};

#endif //USING_XIGNCODE