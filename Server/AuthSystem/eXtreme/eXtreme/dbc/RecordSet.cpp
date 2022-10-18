#include "eXtreme/eXtreme.h"
#include "RecordSet.h"

using namespace eXtreme;

RecordSet::RecordSet()
: m_callbackError(0)
{
}

RecordSet::~RecordSet()
{
    while(m_listRecordInfo.size() > 0)
    {
        RecordInfo* pRecordInfo = m_listRecordInfo.front();

        m_listRecordInfo.pop_front();
        delete pRecordInfo;
    }
}

void
RecordSet::Release()
{
    if(m_command)
        m_command.Release();
}

RecordInfo*
RecordSet::FindRecordInfo(_bstr_t& rowName)
{
    LIST_RECORD_INFO::const_iterator it = m_listRecordInfo.begin(),
                                     end = m_listRecordInfo.end();
    for( ; it != end; ++it)
    {
        RecordInfo* pRecordInfo = *it;

        if(pRecordInfo->rowName == rowName)
            return pRecordInfo;
    }
    return NULL;
}

void
RecordSet::RegisterVariable(void* ptr, _bstr_t rowName)
{
    RecordInfo* pRecordInfo = new RecordInfo();

    pRecordInfo->ptr = ptr;
    pRecordInfo->rowName = rowName;
    pRecordInfo->recordtype = RecordInfo::RECORD_TYPE_VARIABLE;

    try
    {
        m_listRecordInfo.push_back(pRecordInfo);						
    }
    catch(std::bad_alloc&)
    {
        OutputDebugString("[RecordSet::RegisterVariable] Varialble Already exist! \n");
    }
}

void
RecordSet::RegisterParameter(void* ptr, _bstr_t rowName,
                             DataTypeEnum datatype, ParameterDirectionEnum dir,
                             long size, _bstr_t var)
{
    RecordInfo* pRecordInfo = FindRecordInfo(rowName);
    if(pRecordInfo == 0)
    {
		pRecordInfo = new RecordInfo();
		m_listRecordInfo.push_back(pRecordInfo);		
    }

    pRecordInfo->ptr = ptr;
    pRecordInfo->rowName = rowName;
    pRecordInfo->datatype = datatype;
    pRecordInfo->dir = dir;
    pRecordInfo->size = size;
    pRecordInfo->var = var;
    pRecordInfo->recordtype = RecordInfo::RECORD_TYPE_PARAMETER;
}

bool
RecordSet::BindingVariable()
{
    //printf(__FUNCTION__ "\n");
    try
    {
        if(IsEndRecord() == true)
        {
            //printf("    end record" __FUNCTION__ "\n");
            EndFetch();
            return false;
        }
        //printf("  pass  " __FUNCTION__ "\n");
        LIST_RECORD_INFO::const_iterator it = m_listRecordInfo.begin(),
                                         end = m_listRecordInfo.end();

        for( ; it != end; ++it)
        {
            RecordInfo* pRecordInfo = *it;
            if(pRecordInfo->recordtype != RecordInfo::RECORD_TYPE_VARIABLE)
                continue;

            _variant_t vAttachValue = m_Recordset->Fields->GetItem(pRecordInfo->rowName)->Value;

            AttachVariable(vAttachValue, pRecordInfo->ptr);
            //printf("    " __FUNCTION__ "\n");
        }
    }
    catch(_com_error& e)
    {
        if(m_callbackError != 0)
            (*m_callbackError)(this, e.Error(), e.ErrorMessage(), e.Source(), e.Description());

        return false;
    }

    return true;
}

void
RecordSet::BindingParameter()
{
    //printf(__FUNCTION__ "\n");
    try
    {
        LIST_RECORD_INFO::const_iterator it = m_listRecordInfo.begin(),
                                         end = m_listRecordInfo.end();
        for( ; it != end; ++it)
        {
            RecordInfo* pRecordInfo = *it;
            if(pRecordInfo->recordtype != RecordInfo::RECORD_TYPE_PARAMETER) {	
                //printf("    pRecordInfo->recordtype    %d\n", pRecordInfo->recordtype);
                continue;
            }

            if(pRecordInfo->ptr == 0) {
                //printf("    pRecordInfo->ptr   %p \n", pRecordInfo->ptr);
                continue;
            }
            if(pRecordInfo->dir == adParamUnknown || pRecordInfo->dir == adParamInput) {
                //printf("    pRecordInfo->dir   %d \n", pRecordInfo->dir);
                continue;
            }

            _variant_t vAttachValue = m_command->Parameters->Item[pRecordInfo->rowName]->Value;
            AttachVariable(vAttachValue, pRecordInfo->ptr);
            //printf("    " __FUNCTION__ "\n");
        }
    }
    catch(_com_error& e)
    {
        if(m_callbackError != 0)
        {
            (*m_callbackError)(this, e.Error(), e.ErrorMessage(), e.Source(), e.Description());
        }
    }
}

bool
RecordSet::AppendParameter()
{
    try
    {
        _ParameterPtr param;
        LIST_RECORD_INFO::const_iterator it = m_listRecordInfo.begin(),
                                         end = m_listRecordInfo.end();
        for( ; it != end; ++it)
        {
            RecordInfo* pRecordInfo = *it;
            if(pRecordInfo->recordtype != RecordInfo::RECORD_TYPE_PARAMETER)
                continue;

            ParameterDirectionEnum param_dir = pRecordInfo->dir;
            if(param_dir == adParamInput || param_dir == adParamInputOutput) {
                param = m_command->CreateParameter(pRecordInfo->rowName, pRecordInfo->datatype,
                                                   param_dir,
                                                   pRecordInfo->size, pRecordInfo->var);
            }
            else if(param_dir == adParamOutput || param_dir == adParamReturnValue) {
                param = m_command->CreateParameter(pRecordInfo->rowName, pRecordInfo->datatype,
                                                   param_dir,
                                                   pRecordInfo->size);
            }

            m_command->Parameters->Append(param);
        }
    }
    catch(_com_error& e)
    {
        if(m_callbackError != 0)
            (*m_callbackError)(this, e.Error(), e.ErrorMessage(), e.Source(), e.Description());

        return false;
    }

    return true;
}


void
RecordSet::PrepareSP(_bstr_t& spName)
{
    m_command.CreateInstance(__uuidof(Command));
    m_command->CommandText  = spName;
    m_command->CommandType  = adCmdStoredProc;
}

bool
RecordSet::Execute(_bstr_t& queryStr, _ConnectionPtr conn)
{
    try
    {
        EndFetch();
        //printf("" __FUNCTION__ "\n");
        m_Recordset = conn->Execute(queryStr, 0, 0);
        if(IsEndRecord() == false)
        {
            //printf("    " __FUNCTION__ "\n");
            m_Recordset->MoveFirst();
        }
        else
        {
            //printf("    " __FUNCTION__ "\n");
            return true;
        }
    }
    catch(_com_error& e)
    {
        if(m_callbackError != 0)
        {
            (*m_callbackError)(this, e.Error(), e.ErrorMessage(), e.Source(), e.Description());
        }

        EndFetch();

        return false;
    }
    //printf("    " __FUNCTION__ "\n");
    return BindingVariable();
}

void
RecordSet::EndFetch()
{
    if(m_Recordset == 0 || m_Recordset->GetState() == adStateClosed)
    {
        return;
    }

    try
    {
        m_Recordset->Close();
    }   
    catch(_com_error& e)
    {
        if(m_callbackError != 0)
            (*m_callbackError)(this, e.Error(), e.ErrorMessage(), e.Source(), e.Description());
    }

    m_Recordset     = 0;
    m_opItemCount   = 0;
}

bool
RecordSet::IsEndRecord()
{
    try
    {
        if(m_Recordset == 0)
            return true;

        if(m_Recordset->GetState() == adStateClosed)
            return true;

        if(m_Recordset->adoEOF != FALSE)
            return true;

        return false;
    }
    catch(_com_error& e)
    {
        if(m_callbackError != 0)
            (*m_callbackError)(this, e.Error(), e.ErrorMessage(), e.Source(), e.Description());

        return true;
    }
}

long
RecordSet::GetRecordCount()
{
    if(m_Recordset == 0 || m_Recordset->GetState() == adStateClosed)
        return 0;

    long    count = 0;

    try
    {
        count = m_Recordset->RecordCount;
    }
    catch(_com_error& e)
    {
        if(m_callbackError != 0)
            (*m_callbackError)(this, e.Error(), e.ErrorMessage(), e.Source(), e.Description());

        count = 0;
    }

    return count;
}

bool
RecordSet::Next()
{
    m_opItemCount = 0;

    try
    {
        HRESULT hr = m_Recordset->MoveNext();
        if(FAILED(hr))
        {
            return false;
        }
    }
    catch(_com_error& e)
    {
        if(m_callbackError != 0)
            (*m_callbackError)(this, e.Error(), e.ErrorMessage(), e.Source(), e.Description());

        EndFetch();

        return false;
    }

    return BindingVariable();
}

bool
RecordSet::Prev()
{
    m_opItemCount = 0;

    try
    {
        HRESULT hr = m_Recordset->MovePrevious();
        if(FAILED(hr))
        {
            return false;
        }
    }
    catch(_com_error& e)
    {
        if(m_callbackError != 0)
            (*m_callbackError)(this, e.Error(), e.ErrorMessage(), e.Source(), e.Description());

        EndFetch();

        return false;
    }

    return BindingVariable();
}

void
RecordSet::AttachVariable(_variant_t& value, void* ptr)
{
    //printf("" __FUNCTION__ " %d\n", value.vt);
    switch(value.vt)
    {
    case VT_I2:
    //case VT_UI2:
        *(SHORT*)ptr    = (SHORT)value.iVal;
        //printf("VT_I2\n");
        break;
    case VT_INT:
    case VT_I4:
    //case VT_UI4:
        *(INT*)ptr      = (INT)value.intVal;
        //printf("VT_I4\n");
        break;
    case VT_I8:
    //case VT_UI8:
        *(LONGLONG*)ptr = (LONGLONG)value.llVal;
        //printf("VT_I8\n");
        break;
    case VT_UI2:
        *(USHORT*)ptr   = (USHORT)value.uiVal;
        //printf("VT_UI2\n");
        break;
    case VT_UINT:
    case VT_UI4:
        *(UINT*)ptr     = (UINT)value.uintVal;
        //printf("VT_UI4\n");
        break;
    case VT_UI8:
        *(ULONGLONG*)ptr = (ULONGLONG)value.ullVal;
        //printf("VT_UI8\n");
        break;
    case VT_DATE:
        *(DATE*)ptr     = (DATE)value.date;
        //printf("VT_DATE\n");
        break;
    case VT_BOOL:
        *(BOOL*)ptr     = (BOOL)value.boolVal;
        //printf("VT_BOOL\n");
        break;
    case VT_DECIMAL:
        *(LONGLONG*)ptr = (LONGLONG)value.llVal;
        //printf("VT_ARRAY | VT_UI1:\n");
        break;
    case VT_BSTR:
        wcscpy((wchar_t*)ptr, value.bstrVal);
        //printf("VT_BSTR\n");
        break;
    case VT_I1:
    case VT_UI1:
        *(CHAR*)ptr     = (CHAR)value.cVal;
        //printf("VT_I1\n");
        break;
    case VT_R8:
        *(float*)ptr    = (float)value.dblVal;
        //printf("VT_R8\n");
        break;
    case VT_NULL:
        //printf("VT_NULL\n");
        break;
        //  Binary
    case VT_ARRAY | VT_UI1:
        memcpy(ptr, value.parray->pvData, value.parray->rgsabound->cElements);
        //printf("VT_ARRAY | VT_UI1:\n");
        break;
    }
}
