#pragma  once

#pragma warning(push)
#pragma warning(disable:4146)

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename("EOF", "adoEOF") 
using namespace ADODB;

#pragma warning(pop)


#include <oledb.h>
#include <icrsint.h>
#include <list>


namespace eXtreme {

struct RecordInfo
{
    enum RECORD_TYPE { RECORD_TYPE_VARIABLE, RECORD_TYPE_PARAMETER };

    void*                   ptr;
    _bstr_t                 rowName;
    DataTypeEnum            datatype;
    ParameterDirectionEnum  dir;
    long size;
    _bstr_t                 var;
    byte                    recordtype;
};

class RecordSet;

typedef void (*CallbackRecordsetErr)(RecordSet*, HRESULT, _bstr_t, _bstr_t, _bstr_t);

class RecordSet
{
    friend class DBC;

public:
    RecordSet();
    virtual ~RecordSet();

    void Release();

    // Bind할 Variable과 Parameter를 우선 m_listRecordInfo에 등록한다.
    void RegisterVariable(void* ptr, _bstr_t rowName);
    void RegisterParameter(void* ptr, _bstr_t rowName, DataTypeEnum type, ParameterDirectionEnum dir, long size, _bstr_t var);

    bool IsEndRecord();
    bool Next();
    bool Prev();

    long GetRecordCount();


    CallbackRecordsetErr m_callbackError;
    _bstr_t m_lastQuery;

private:
    bool Execute(_bstr_t& queryStr, _ConnectionPtr conn);
    void PrepareSP(_bstr_t& spName);
    void EndFetch();
    RecordInfo* FindRecordInfo(_bstr_t& rowName);

    void AttachVariable(_variant_t& value, void* ptr);
    bool BindingVariable();
    bool AppendParameter();
    void BindingParameter();

    _RecordsetPtr m_Recordset;
    _CommandPtr   m_command;

    typedef std::list< RecordInfo* > LIST_RECORD_INFO;
    LIST_RECORD_INFO m_listRecordInfo;
    int m_opItemCount;
};

}; //end of namespace 'eXtreme'




