#include "stdafx.h"
#include "./IDBCInterface.h"

//==================================================================================================

IDBCInterface::IDBCInterface(void)
    : DBCDataRef(dbc_data_)
{
    ZeroMemory(&dbc_data_, sizeof(dbc_data_));
}

IDBCInterface::~IDBCInterface(void)
{
}

//==================================================================================================

bool IDBCInterface::Init(int index,
                         const char* ip_address, const char* database_name,
                         const char* user_name, const char* user_password,
                         fnCallBackMessage fnptr_error_message,
                         fnCallBackMessageEx fnptr_error_message_ex) 
{
    ZeroMemory(&dbc_data_, sizeof(dbc_data_));
    strncpy(dbc_data_.ip_address_, ip_address, MAX_IP_ADDRESS_LENGTH);
    strncpy(dbc_data_.database_name_, database_name, MAX_NAME_LENGTH);
    strncpy(dbc_data_.user_name_, user_name, MAX_NAME_LENGTH);
    strncpy(dbc_data_.user_password_, user_password, MAX_NAME_LENGTH);
    dbc_data_.index_ = index;
    dbc_data_.fnptr_error_message_ = fnptr_error_message;
    dbc_data_.fnptr_error_message_ex_ = fnptr_error_message_ex;
    //
    return true;
}