#pragma once
#ifndef __QUERY_RESULT_H__
#define __QUERY_RESULT_H__

//==================================================================================================
/// Custom 쿼리들을 모듈에서 유연하게 사용하기 위해 만든 인터페이스 추상 클래스
/**
    @author
        Kim Min Wook < taiyo@webzen.co.kr >
    @since
        2004. 10. 20
    @remarks
        - 인스턴스화 할 수 없다.
        - QueryResult 상속받은 클래스는 쿼리의 종류에 따라
          (select or insert,update,delete) 스크립트를 작성해야 한다.
    @par
        - m_dwIndex 는 쿼리들을 구분하기 위해 사용될 수 있다.
        - m_pVoidObject 맴버는 사용자가 커스텀하게 사용할 수 있다.
    @history
        - ~10/08/06|custom
        - 10/08/06|waverix|code arrangement related to change types and making const member
*/
//==================================================================================================

class IODBC;

enum eQUERYTYPE
{
    eErrorQuery,
    eGeneralQuery,
    eLargeQuery,
};

//==================================================================================================

class SOLARDATABASE_API QueryResult
{   
public:
    enum {
        MAX_QUERY_LENGTH = 1024,
        MAX_SQL_MESSAGE_LENGTH = 512,
    };

    QueryResult() {}
    virtual ~QueryResult() {}

    virtual inline ULONG GetParameterNum() { return 0; }
    // (CHANGES) (f100806.2L) change to returns eLargeQuery fixed default type based on a case study
    virtual inline eQUERYTYPE GetQueryType() { return eGeneralQuery; }
    virtual inline void GetParameterInfo(ULONG ulParamIndex, SHORT& ioType, BYTE*& pParamPtr,
                                         LONG& lSize, SHORT& sqlType, SHORT& cType)
    {
        __UNUSED(ulParamIndex);
        __UNUSED(ioType);
        __UNUSED(pParamPtr);
        __UNUSED(lSize);
        __UNUSED(sqlType);
        __UNUSED(cType);
    }

    virtual inline ULONG GetMaxRowNum() const = 0;
    virtual inline ULONG GetMaxColNum() const = 0;
    virtual inline BYTE* GetBuffer(ULONG ulRow) = 0;

    virtual inline void GetColumnInfo(ULONG row, ULONG col, ULONG*& pColPtr,
                                      LONG& ulSize, SHORT& sColType)
    {
        __UNUSED(row);
        __UNUSED(col);
        __UNUSED(pColPtr);
        __UNUSED(ulSize);
        __UNUSED(sColType);
    };

    virtual inline ULONG* GetColumnLengthPtr(ULONG col) {
        __UNUSED(col);
        return NULL;
    }
    //
    DWORD  GetIndex() const;

    ULONG  GetResultColNum() const;
    ULONG  GetResultRowNum() const;

    LONG   GetAffectNum() const;

    void   SetAffectNum(LONG ulVal);
    void   SetResultColNum(ULONG ulVal);
    void   SetResultRowNum(ULONG ulVal);
    void   SetIndex(DWORD dwIdx);

    void*  GetVoidObject() const;
    void   SetVoidObject(void* pVoidObject);

    void   SetQuery(const char* pszQuery);
    const char* const GetQuery() const;

    const char* GetSqlState() const;
    const char* GetErrorMsg() const;
    USHORT GetNativeError() const;
    void   SetSqlState(const char* msg);
    void   SetErrorMsg(const char* msg);
    void   SetNativeError(USHORT val);

    const char*  GetSqlStatePtr() const;
    const char*  GetErrorMsgPtr() const;
    const USHORT* GetNativeErrorPtr() const;

    bool  IsError() const;
    void  SetError(bool v);
    DWORD GetQueryTime() const;
    void  SetQueryTime(DWORD v);
    DWORD GetStartQueryTime() const;
    void  SetStartQueryTime(DWORD tick);

private:
    ULONG* GetResultColNumPtr();
    ULONG* GetResultRowNumPtr();
    LONG*  GetAffectNumPtr();
    //
    DWORD  m_dwIndex;
    void*  m_pVoidObject;
    ULONG  m_ulResultColNum;
    ULONG  m_ulResultRowNum;
    LONG   m_lAffectNum;
    char   m_pszQuery[MAX_QUERY_LENGTH+1];
    char   m_szSqlState[MAX_SQL_MESSAGE_LENGTH+1];
    char   m_szErrorMsg[MAX_SQL_MESSAGE_LENGTH+1];
    USHORT m_usNativeError;
    bool   m_bError;
    DWORD  m_dwStartQueryTime;
    DWORD  m_dwQueryTime;
    //
    friend class IODBC;
};

//==================================================================================================
// implements

inline DWORD  QueryResult::GetIndex() const {
    return m_dwIndex;
}

inline ULONG* QueryResult::GetResultColNumPtr() {
    return &m_ulResultColNum;
}

inline ULONG* QueryResult::GetResultRowNumPtr() {
    return &m_ulResultRowNum;
}

inline LONG*  QueryResult::GetAffectNumPtr() {
    return &m_lAffectNum;
}

inline ULONG  QueryResult::GetResultColNum() const {
    return m_ulResultColNum;
}

inline ULONG  QueryResult::GetResultRowNum() const {
    return m_ulResultRowNum;
}

inline LONG   QueryResult::GetAffectNum() const {
    return m_lAffectNum;
}

inline void   QueryResult::SetAffectNum(LONG ulVal) {
    m_lAffectNum = ulVal;
}

inline void   QueryResult::SetResultColNum(ULONG ulVal) {
    m_ulResultColNum = ulVal;
}

inline void   QueryResult::SetResultRowNum(ULONG ulVal) {
    m_ulResultRowNum = ulVal;
}

inline void   QueryResult::SetIndex(DWORD dwIdx) {
    m_dwIndex = dwIdx;
}

inline void*  QueryResult::GetVoidObject() const {
    return m_pVoidObject;
}

inline void   QueryResult::SetVoidObject(void* pVoidObject) {
    m_pVoidObject = pVoidObject;
}

inline void   QueryResult::SetQuery(const char* pszQuery) {
    strncpy(m_pszQuery, pszQuery, MAX_QUERY_LENGTH);
    m_pszQuery[MAX_QUERY_LENGTH]='\0';
}

inline const char* const QueryResult::GetQuery() const {
    return m_pszQuery;
}

inline const char* QueryResult::GetSqlState() const {
    return m_szSqlState;
}

inline const char* QueryResult::GetErrorMsg() const {
    return m_szErrorMsg;
}

inline USHORT QueryResult::GetNativeError() const {
    return m_usNativeError;
}

inline void   QueryResult::SetSqlState(const char* msg) {
    strncpy(m_szSqlState,msg,MAX_SQL_MESSAGE_LENGTH);
    m_szSqlState[MAX_SQL_MESSAGE_LENGTH]='\0';
}

inline void   QueryResult::SetErrorMsg(const char* msg) {
    strncpy(m_szErrorMsg,msg,MAX_SQL_MESSAGE_LENGTH);
    m_szErrorMsg[MAX_SQL_MESSAGE_LENGTH]='\0';
}

inline void   QueryResult::SetNativeError(USHORT val) {
    m_usNativeError = val;
}

inline const char*  QueryResult::GetSqlStatePtr() const {
    return m_szSqlState;
}

inline const char*  QueryResult::GetErrorMsgPtr() const {
    return m_szErrorMsg;
}

inline const USHORT* QueryResult::GetNativeErrorPtr() const {
    return &m_usNativeError;
}

inline bool  QueryResult::IsError() const {
    return m_bError;
}

inline void  QueryResult::SetError(bool v) {
    m_bError = v;
}

inline DWORD QueryResult::GetQueryTime() const {
    return m_dwQueryTime;
}

inline void  QueryResult::SetQueryTime(DWORD v) {
    m_dwQueryTime = v;
}

inline DWORD QueryResult::GetStartQueryTime() const {
    return m_dwStartQueryTime;
}

inline void  QueryResult::SetStartQueryTime(DWORD tick) {
    m_dwStartQueryTime=tick;
}

#endif // __QUERY_RESULT_H__