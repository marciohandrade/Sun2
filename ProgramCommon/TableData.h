#ifndef __PROGRAMCOMMON_TABLE_DATA_H
#define __PROGRAMCOMMON_TABLE_DATA_H
#pragma once

//==================================================================================================
//  (History)
//      스크립트를 읽어서 전체를 테이블 구조 형태로 저장시키는 클래스
//      Create : 2006. 11. 23일 금요일.
//      Make By 조재식
//      규칙 :
//      - 해당 스크립트에서 제일 첫줄은(주석제외) 반드시 필드명이어야 한다.
//      - 모든 데이터는 스트링으로 저장된다. ==> 변환함수를 제공한다.
//      - 첫줄의 필드 데이터를 읽어서 리스트에 저장후, 실제 데이트를 리스트 개수만큼 반복해서 읽어 드려
//      실제 데이터를 저장한다.
//      - 필드명은 LinearHash 로 저장되고, 나머지 실제 데이터는 배열로 저장된다.
//      - SCRIPT_COLUMN_DATA와 BUG_SCRIPT_COLUMN_DATA를 동적으로 하지 않고, 이렇게 무식하게 한것은 속도 때문에
//      배열로 유지 하기 위해서다. 동적으로 사이즈를 new를 써서 하면 디버깅시 속도가 느려진다.
//
//  - 09.07.12, waverix, code rearrangement
//                     , delete working macro '__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM'
//  - 09.07.18, waverix, update
//

//#include <map>
//#include <hash_map>
//#include <string>
//#include <list>

//==================================================================================================
//==================================================================================================
#include <StreamHashKey.h>
#include <SolarHashTable.h>
#include <LinkedListCS.h>

class TableData
{
#define MAX_FIELD_NAME_SIZE     40
    // wchar_t를 사용할 것 같지 않다. 그 근거로 지금 시점에는 효용성의 문제가 가장 크다.
    static const DWORD MaxScriptDataSize = 12 * 1024 * 1024 * sizeof(char);
    static const DWORD MaxTokenStreamSize = (DWORD)(MaxScriptDataSize * 4);
    typedef BYTE DelimiterBuffer[32];

    struct ColInfo { DWORD  count;      char*       pRef[1024]; };
    struct RowInfo  { DWORD count;      ColInfo**   pRef; };

    struct LinePair { char* pLinePtr;   ColInfo*    pColumns;   };
    struct LineListNode : public util::__LList<LineListNode> { LinePair linePair;   };

    typedef util::SolarHashTable<INT>   TABLE_NAME_HASH;

public:
    TableData();
    ~TableData() { _UnLoad(); }

    //  <TableData>
    void    Init(BOOL bBigData = FALSE);
    void    Release() { _UnLoad(); }
    int     GetColSize() const { return (int)m_ColumnCount; }
    int     GetRowSize() const { return (int)m_LineRef.count; }
    inline const char* FindData(const char* pszColName, const UINT nRow) const;

    // corresponding TableData Interfaces
    bool    InsertData(UINT nColNum, UINT nRowNum, const char* pszData);
    INT     FindColNumByFieldName(const char* pszFieldName);
    bool    CreateColumn(const char* pszColName, INT nCol);
    BOOL    IsBigData(){ return 0; }

    // 현 시점 파싱 매커니즘은 전체 데이터에 대한 인덱스 테이블을 설정해서 접근하는 방식이다.
    // 데이터의 분석 시점에 테이블을 크기를 자연스럽게 계산하는데는 불필요한 연산이 요구된다.
    // 따라서 자연스럽게 현 시점 요구되는 최대치에 맞춘 대규모의 테이블의 크기를 요구하게 된다.
    // CreateEnvironment | DestroyEnvironment는 그걸 위한 고정 버퍼를 설정한다.
    static BOOLEAN CreateEnvironment(); // ScriptParserManager Load 시작 시점
    static BOOLEAN DestroyEnvironment();// ScriptParserManager Load 종료 시점
    BOOLEAN Load(eSCRIPT_CODE script_code, const TCHAR* pszFileName,
                 BOOLEAN use_packfile, const char* packFileName);
    BOOLEAN LoadBuffer(eSCRIPT_CODE script_code, const BYTE* pBuffer, const DWORD buffer_size);
    VOID    Close() { _UnLoad(); };

    void  RegisterColumnDelimiters(const char* delimiters); // { space, tab }
private:
    BOOLEAN  _OpenFileAndReadBuffer(const char* fileName,
                                    BOOLEAN use_packfile, const char* packFileName);
    BOOLEAN _UnLoad();
    BOOLEAN _BuildDictionary(eSCRIPT_CODE scriptCode);
    //static BOOLEAN _MakeHashInit();
    __forceinline char* _RawTokenizer(char* pToken, DelimiterBuffer* delimeters);
    // _BuildDictionary SequenceStep

    // LIST CONTROL
    // changes, f100813.3L, replace an interface to acquire an unique id
    //DWORD   MakeHash(const char* input_str) const;

    char*   m_pBuffer;
    DWORD   m_ColumnCount;
    DWORD   m_LineCount;
    util::__LList<LineListNode> m_Rows;
    RowInfo      m_LineRef;
    TABLE_NAME_HASH* m_pColumnHash;

    static BYTE* token_stream_static_;
    static DWORD* crc_table_static_;
    static char* file_data_buffer_static_;
    static DelimiterBuffer* delimiters_static_[2];
    static TABLE_NAME_HASH* column_hash_static_;
};

inline const char*
TableData::FindData(const char* pszColName, const UINT nRow) const
{
    if((nRow < (UINT)m_LineRef.count) == FALSE)
        return NULL;
    // changes, f100813.3L, replace an interface to acquire an unique id
    DWORD hashVal = util::StreamHashKey::GetStringHashKey(pszColName);
    int col = m_pColumnHash->GetData(hashVal);
    if(col)
        return m_LineRef.pRef[nRow]->pRef[col-1];

    return NULL;
}

//==================================================================================================
//==================================================================================================

#endif //__PROGRAMCOMMON_TABLE_DATA_H
