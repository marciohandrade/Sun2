#include "StdAfx.h"
#include ".\pmsmanager.h"

#ifdef _JP_0_20101123_PMS

PMSManager::PMSManager(void)
{
    DWORD pms_result = pms_wrapper_.Init(__argc, __argv);
    if(PMSC_ERR_OK != pms_result)
    {
        SUNLOG(eCRITICAL_LOG, "PMS Init fail (%u)", pms_result);        
    }
}

PMSManager::~PMSManager(void)
{
    Stop();
}

void PMSManager::Run()
{
    if(pms_wrapper_.GetStatus() == PMSC_STATUS_INIT ||
        pms_wrapper_.GetStatus() == PMSC_STATUS_STOP)
    {   
        DWORD pms_result = pms_wrapper_.Run(&pms_observer_); 
        if(PMSC_ERR_OK != pms_result)
        {
            SUNLOG(eCRITICAL_LOG, "PMS Run fail (%u)", pms_result);
        }
    }    
}

void PMSManager::Stop()
{
    if(pms_wrapper_.GetStatus() == PMSC_STATUS_RUN ||
        pms_wrapper_.GetStatus() == PMSC_STATUS_NOUSE ||
        pms_wrapper_.GetStatus() == PMSC_STATUS_TRY_CONNECTING)
    {
        pms_wrapper_.Stop(); 
    }     
}

void PMSManager::DisplayPMSInfo()
{
    const INT print_max = 4096;

    INT current_writed_count = 0;
    // CHANGES: :) it's not intended if the original code like a '[16383 + sizeof(TCHAR)]'
    // because the original code same as char = 16383 + 1, wchar_t = (16383 + 2) * 2
    // furthermore, it may lead to 'insufficient stack by a page fault exception' danger
    // that the 32K size stack variable is big if real write process is occurred.
    TCHAR pms_info[print_max];

    current_writed_count += _sntprintf( pms_info + current_writed_count, print_max - current_writed_count, 
        "===================  PMS Info ====================\n" );
    current_writed_count += _sntprintf( pms_info + current_writed_count, print_max - current_writed_count, 
        "PMSCONN_STATUSCODE : %u \n",  GetStatus());
    current_writed_count += _sntprintf( pms_info + current_writed_count, print_max - current_writed_count, 
        "Category count : %u \n",  GetCategory());
    current_writed_count += _sntprintf( pms_info + current_writed_count, print_max - current_writed_count, 
        "Session count : %u \n",  GetSessioCount());
    current_writed_count += _sntprintf( pms_info + current_writed_count, print_max - current_writed_count, 
        "User count : %u \n",  GetUserCount());
    current_writed_count += _sntprintf( pms_info + current_writed_count, print_max - current_writed_count, 
        "Room count : %u \n",  GetRoomCount());
    current_writed_count += _sntprintf( pms_info + current_writed_count, print_max - current_writed_count, 
        "Channel count : %u \n",  GetChannelCount());
    current_writed_count += _sntprintf( pms_info + current_writed_count, print_max - current_writed_count, 
        "==================================================\n" );
    pms_info[_countof(pms_info) - 1] = _T('\0');

    _putts( pms_info );
}

#endif //_JP_0_20101123_PMS