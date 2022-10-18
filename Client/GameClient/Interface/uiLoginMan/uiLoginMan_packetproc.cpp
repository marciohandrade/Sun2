//------------------------------------------------------------------------------
//  uiLoginMan_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginMan.h"
#include "PacketStruct_CL.h"
#include "SceneBase.h"
#include "WrapArmorKit.h"

#include "AutoTestManager.h"
#if USING_GAMEGUARD
#include "GameGuard/GameGuardApi.h"
#include "GlobalFunc.h"
#endif // USING_GAMEGUARD

void uiLoginMan::NetworkProc( MSG_BASE * pMsg )
{
	if(GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_LOGIN)
		return;

	switch( pMsg->m_byCategory )
	{
	case CATEGORY_AUTH:
		{
			switch( pMsg->m_byProtocol)
			{
			// 연결
			case FC_AUTH_READY_CMD:
				{
					// READY 수신
					this->RECV_AUTH_READY_CMD(pMsg);

					// 버전 확인 전송
					this->SEND_AUTH_VERIFY_SYN();
				}
				break;

			case FC_AUTH_VERIFY_ACK:
				{
					// 버전확인수신
					this->RECV_AUTH_VERIFY_ACK(pMsg);


#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
                    if( AutoTestManagerEx::Instance()->IsAutoLogin() == true )
                    {
                        AutoTestManagerEx::Instance()->RunLogin();
                    }
#endif
				}
				break;

#ifdef __CN_269_MATRIX_CARD_AUTH
                // 중국 보안카드
			case FC_AUTH_MATRIX_CARD_SYN:
				{
					// Matrix Card 인증 수신
					RECV_AUTH_MATRIX_CARD_SYN(pMsg);
				}
				break;
#endif

#if defined(__CN_20100909_SECURITY_TOKEN__)
            case AU_AUTH_SECURITY_TOKEN_ACK:
                {
                    // 중국 토큰
                    RECV_AUTH_SECURITY_TOKEN_ACK(pMsg);
                }
                break;

#elif defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_)
                // 일본 OneTimePassword
            case CF_AUTH_OTP_PASS_CMD:
                {
                    // 일본 OTP CMD 패킷 수신
                    RECV_AUTH_OTP_PASS_CMD(pMsg);
                }
                break;
            case AU_AUTH_OTP_PASS_ACK:
                {
                    // 일본 OTP CMD 패킷 수신
                    RECV_AUTH_OTP_PASS_ACK(pMsg);
                }
                break;
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

			case FC_AUTH_ID_PASSWORD_CHECK_ACK:
			//case FC_AUTH_TOKEN_PASS_ACK:
			//case FC_AUTH_REAUTH_PASS_ACK:			//<- 위 3개의 프로토콜은 같은 값
				{
					// 계정 인증 결과 수신
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
					if (m_bServerSelect)
					{
						RECV_AUTH_REAUTH_PASS_ACK(pMsg);
					}
					else
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__
					{
#if defined(_APPLY_KOREA_CHANNELING)
						RECV_AUTH_TOKENPASS_ACK(pMsg);
#elif defined(_NHN_USA_CHANNELING)
						RECV_AUTH_TOKEN_NORTHA_PASS_ACK(pMsg);
#elif defined(_APPLY_JAPAN_GAMECHU_CHANNELING)
                        RECV_AUTH_TOKEN_JP_GAMECHU_ACK(pMsg);
#elif defined(_APPLY_RUSSIA_INGAMBA_CHANNELING)
                        RECV_AUTH_PASS_RU_INGAMBA_ACK(pMsg);
#else
						RECV_AUTH_ID_PASSWORD_CHECK_ACK(pMsg);
#endif
					}					
#if USING_GAMEGUARD
                    // 게임 가드에 ID 전송
                    // 0 - 실패, 1 - 성공
                    if (GameGuardApi::Instance()->SendUserId(m_strTryID) == FALSE)
                    {
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
                        TCHAR result_log[INTERFACE_STRING_LENGTH] = {0, };
                        Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("GameGuardApi::SendUserId() 전송 실패 : %s\n"), m_strTryID);
                        GlobalFunc::Write_GameGuard_Log(result_log);
#endif // _NA_000000_20140519_GAMEGUARD_LOG

                    }
#endif // USING_GAMEGUARD
				}
				break;
			case FC_AUTH_SERVER_LIST_WORLD_ACK:
				{
					// 서버(월드 그룹) 이름 목록 수신
					this->RECV_AUTH_SERVER_LIST_WORLD_ACK(pMsg);
				}
				break;

			case FC_AUTH_SERVER_LIST_CHANNEL_ACK:
				{
					// 채널(서버) 목록 및 상태 수신
					this->RECV_AUTH_SERVER_LIST_CHANNEL_ACK(pMsg);
#if defined(_DEBUG) || defined(_GAMECLIENT_VER)
                    if( AutoTestManagerEx::Instance()->IsAutoLogin() == true )
                    {
                        AutoTestManagerEx::Instance()->RunSelectServer();
                    }
#endif
				}
				break;

			case FC_AUTH_SERVER_SELECT_ACK:
				{
					// 서버 선택 결과 수신
					this->RECV_AUTH_SERVER_SELECT_ACK(pMsg);
				}
				break;
			}
		}
		break;
	}
}
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
