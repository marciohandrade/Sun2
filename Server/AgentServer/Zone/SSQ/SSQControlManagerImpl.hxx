#pragma once

//==================================================================================================
//  <SSQControlManagerImpl>
struct SSQControlManagerImpl
{
    friend class WarControlManager;
    friend class SSQUpdateCtrl;
    friend struct OpSSQInfoController;

    ITimerBase   ScheduleUpdateTimer;
    ITimerBase   MessageUpdateTimer;
    SSQ_UPDATERS UpdateList;
    ITimerBase   SSQTicketTimer;
public:
    nsSSQ::SSQTicketTimeInfo SSQTicketTimeInfoCtrl;
    SSQSingletonNotice SSQSingletonNoticeCtrl;

    SSQControlManagerImpl();
    ~SSQControlManagerImpl();

private:
    SSQ_UPDATERS_IT _FindCodeOfSSQ(const SSQCODE CodeOfSSQ);
    SSQ_UPDATERS_IT _FindZoneKeyofSSQ(const KEYTYPE KeyOfSSQ);
    VOID _RegisterUpdateCtrl(const SSQCODE CodeOfSSQ, SSQUpdateCtrl* const pCtrl);
    VOID _ReleaseUpdateCtrl();
    VOID _InitializeUpdateCtrl();

    //  REG_SSQ_FIELD_CTRL_LIST() 목록에 존재하는 내용 선언...
    //─────────────────────────────────────────
    //  <PACKET HANDLER ARGUMENTs>
    //struct sJOIN_INFO_COND
    //{
    //  FlowControl&        IN  m_rFlow;
    //  User* const         IN  m_pUser;
    //  ServerSessionEx*    OUT m_pFieldServer;
    //  SSQInfo*            OUT m_pSSQInfo;
    //  sSSQ_CTRL_NODE*     OUT m_pCtrlNode;
    //  DungeonSSQ*         OUT m_pDungeonSSQ;

    //  sJOIN_INFO_COND(FlowControl& rFlow, User* const pUser)
    //      : m_rFlow(rFlow), m_pUser(pUser)
    //      , m_pFieldServer(0), m_pSSQInfo(0), m_pCtrlNode(0), m_pDungeonSSQ(0)
    //  {}
    //};

    //typedef AutoTransactionObject<sJOIN_INFO_COND>        AutoTransactionJoinSynData;

    //─────────────────────────────────────────
    //  <PACKET HANDLERs>
#define SSQ_PACKET_HANDLER_CG_DECL(c, p)    \
    VOID    SSQControlManagerImpl::OnRecv_##p (const MSG_##p* pRecvMsg, const WORD wSize, User* const pUser)

#define SSQ_PACKET_HANDLER_AG_DECL(c, p)    \
    VOID    SSQControlManagerImpl::OnRecv_##p (const MSG_##p* pRecvMsg, const WORD wSize, GameServerSession* const pSession)

#define SSQ_PACKET_HANDLER_AW_DECL(c, p)    \
    VOID    SSQControlManagerImpl::OnRecv_##p (const MSG_##p* pRecvMsg, const WORD wSize, WorldServerSession* const pSession)

    SSQ_PACKET_HANDLER_CG_DECL(CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_INFO_SYN);
    SSQ_PACKET_HANDLER_CG_DECL(CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_SYN);
    SSQ_PACKET_HANDLER_CG_DECL(CG_ZONE_EX, CG_ZONE_EX_SSQ_ROOM_INFO_SYN);
    SSQ_PACKET_HANDLER_CG_DECL(CG_ZONE_EX, CG_ZONE_EX_SSQ_OBSERVER_SYN);
    SSQ_PACKET_HANDLER_AG_DECL(AG_ZONE, AG_ZONE_SSQ_JOIN_INFO_ACK);
    SSQ_PACKET_HANDLER_AG_DECL(AG_ZONE, AG_ZONE_SSQ_JOIN_ACK);
    SSQ_PACKET_HANDLER_AG_DECL(AG_ZONE, AG_ZONE_SSQ_ROOM_CTRL_CMD);
    SSQ_PACKET_HANDLER_AG_DECL(AG_ZONE, AG_ZONE_SSQ_ROOM_INFO_BRD);
    SSQ_PACKET_HANDLER_AW_DECL(AW_OPERATION, AW_OPERATION_ROOM_CTRL_CMD);

    RC::eROOM_RESULT EnterenceDefaultConstraintCheck(User* const pUser,
                                                     const BOOLEAN bJoinActionIsT_JoinInfoIsF);
    struct EnterenceControlArg
    {
        const BOOLEAN IsJoinAction;
        const SSQCODE Inputed_CodeOfSSQ;
        const POSTYPE Inputed_PosOfItem;
        EnterenceControlArg(const BOOLEAN bJoinActionIsT_JoinInfoIsF,
                            const SSQCODE inputed_CodeOfSSQ, // JOIN_SYN일 경우라면 입력될 것이다.
                            const POSTYPE inputed_PosOfItem)
            : IsJoinAction(bJoinActionIsT_JoinInfoIsF)
            , Inputed_CodeOfSSQ(inputed_CodeOfSSQ)
            , Inputed_PosOfItem(inputed_PosOfItem)
        {
        }
        __DISABLE_COPY(EnterenceControlArg);
    };
    RC::eROOM_RESULT EnterenceControl(FlowControl& INOUT flow,
                                      User* const pUser, const EnterenceControlArg& arg);
    VOID OnRecv_AG_ZONE_SSQ_JOIN_ACK_ByField(const MSG_AG_ZONE_SSQ_JOIN_ACK* pRecvMsg, const WORD wSize,
                                             GameServerSession* const pSession,
                                             FlowControl& flow, BYTE& errorGroup);
    VOID OnRecv_AG_ZONE_SSQ_JOIN_ACK_ByBattle(const MSG_AG_ZONE_SSQ_JOIN_ACK* pRecvMsg, const WORD wSize,
                                              GameServerSession* const pSession,
                                              FlowControl& flow, BYTE& errorGroup);

    //  for DB synchronization
    VOID OnRecv_AG_RoomJoinAck(SSQUpdateCtrl* const pCtrl, User* const pUser,
                               DungeonSSQ* const pDungeonSSQ);
    DWORD _Check_C2G_RequestRule(const WAR_MSG_HDR* IN pStream, User* const pUser);
    static const DWORD RC_ROOM_FLOW_CONTROL_DEFAULT_RETURN_VALUE = RC::RC_ROOM_FAILED + 100000;
    static const DWORD RC_ROOM_ACK_FAIL_MESSAGE = RC::RC_ROOM_FAILED + 100001;
    WAVERIX_TEST_BLOCK_IN_CLASS(SSQControlManagerImpl);
};

