#ifndef     __UIDEF_MANAGER_TRADE_H__
#define     __UIDEF_MANAGER_TRADE_H__

struct IM_TRADE_MANAGER
{
	static const DWORD TRADE;
	static const DWORD TRADE_NUMBER_KEYBOARD;
};

// Trade userinterface resource info
const char  c_strDlgFname_Trade[]   = ("Data\\Interface\\72_Trade.iwz");
const DWORD c_dwCompareID_Trade     = StrToWzID("0720");

enum
{
    TRADE_STATUS_IDLE=0,    // 거래 중
    TRADE_STATUS_PROPOSAL,    // 거래 제시
    TRADE_STATUS_ACCEPT,  // 거래 수락
};

class Object;
class uicTradeInfo
{
public:
    uicTradeInfo() { Clear(); }
    ~uicTradeInfo() {}

    inline void Clear()
    {
        m_strName.clear();
        m_isSet = false;
        m_dwKey = 0;
        m_DealMoney = 0;
        m_pObject = NULL;
        m_status = -1;
    }

    util::_tstring          m_strName;
    bool                    m_isSet;
    DWORD                   m_dwKey;
    MONEY                   m_DealMoney;
    Object*                 m_pObject;
    int                     m_status;
};

enum
{
    TRADE_MOD_PENDING_UNKNOWN=0,
    TRADE_MOD_PENDING_PUT,
    TRADE_MOD_PENDING_GET,
    TRADE_MOD_PENDING_PUTMONEY,
};

typedef struct modPending_Put_s
{
    POSTYPE pos;
} modPending_Put_t;

typedef struct modPending_Get_s
{
    POSTYPE pos;
} modPending_Get_t;

typedef struct modPending_PutMoney_s
{
    MONEY   money;
} modPending_PutMoney_t;

#endif
