#ifndef __PROGRAMCOMMON_TABLE_DATA_H
#define __PROGRAMCOMMON_TABLE_DATA_H
#pragma once

//==================================================================================================
//  (History)
//      ��ũ��Ʈ�� �о ��ü�� ���̺� ���� ���·� �����Ű�� Ŭ����
//      Create : 2006. 11. 23�� �ݿ���.
//      Make By �����
//      ��Ģ :
//      - �ش� ��ũ��Ʈ���� ���� ù����(�ּ�����) �ݵ�� �ʵ���̾�� �Ѵ�.
//      - ��� �����ʹ� ��Ʈ������ ����ȴ�. ==> ��ȯ�Լ��� �����Ѵ�.
//      - ù���� �ʵ� �����͸� �о ����Ʈ�� ������, ���� ����Ʈ�� ����Ʈ ������ŭ �ݺ��ؼ� �о� ���
//      ���� �����͸� �����Ѵ�.
//      - �ʵ���� LinearHash �� ����ǰ�, ������ ���� �����ʹ� �迭�� ����ȴ�.
//      - SCRIPT_COLUMN_DATA�� BUG_SCRIPT_COLUMN_DATA�� �������� ���� �ʰ�, �̷��� �����ϰ� �Ѱ��� �ӵ� ������
//      �迭�� ���� �ϱ� ���ؼ���. �������� ����� new�� �Ἥ �ϸ� ������ �ӵ��� ��������.
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
    // wchar_t�� ����� �� ���� �ʴ�. �� �ٰŷ� ���� �������� ȿ�뼺�� ������ ���� ũ��.
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

    // �� ���� �Ľ� ��Ŀ������ ��ü �����Ϳ� ���� �ε��� ���̺��� �����ؼ� �����ϴ� ����̴�.
    // �������� �м� ������ ���̺��� ũ�⸦ �ڿ������� ����ϴµ��� ���ʿ��� ������ �䱸�ȴ�.
    // ���� �ڿ������� �� ���� �䱸�Ǵ� �ִ�ġ�� ���� ��Ը��� ���̺��� ũ�⸦ �䱸�ϰ� �ȴ�.
    // CreateEnvironment | DestroyEnvironment�� �װ� ���� ���� ���۸� �����Ѵ�.
    static BOOLEAN CreateEnvironment(); // ScriptParserManager Load ���� ����
    static BOOLEAN DestroyEnvironment();// ScriptParserManager Load ���� ����
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
