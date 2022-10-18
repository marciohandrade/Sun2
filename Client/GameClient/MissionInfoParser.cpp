#include "SunClientPrecompiledHeader.h"

#include "MissionInfoParser.h"


MissionInfoParser::MissionInfoParser(void)
{
}

MissionInfoParser::~MissionInfoParser(void)
{
}

VOID MissionInfoParser::Init()
{
}

VOID MissionInfoParser::Init(char* pack_file_name)
{
	SetPackFileName(pack_file_name);
}

VOID MissionInfoParser::Release()
{
	MISSION_OBJECT_LIST_ITR it = mission_object_data_table_.begin();
	MISSION_OBJECT_LIST_ITR end = mission_object_data_table_.end();
	for ( ; it != end; ++it)
	{
		delete (*it);
	}
	mission_object_data_table_.clear();
}

BOOL MissionInfoParser::_Load(BOOL reload)
{
	int nRowSize = GetRowSize();
	TCHAR temp_string[64] = {0,};
	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		// 스크립트에서 읽어온 정보
		_MissionObject* mission_Info_ptr = new _MissionObject;
        mission_Info_ptr->index = GetDataDWORD("Index", nRow);
		mission_Info_ptr->map_code = GetDataDWORD("MapCode", nRow);
		mission_Info_ptr->name_code = GetDataDWORD("NCode", nRow);
		mission_Info_ptr->field_code = GetDataDWORD("OpenFieldCode", nRow);
		mission_Info_ptr->mission_text_code = GetDataDWORD("MissionText", nRow);
		for (int i = 0; i < _MissionObject::MAX_OPEN_TRIGGER_ID; ++i)
		{
			sprintf(temp_string, "OpenTgrID%d", i+1);
			mission_Info_ptr->open_trigger_id[i] = GetDataDWORD(temp_string, nRow);
		}
        mission_Info_ptr->complete_field_code = GetDataDWORD("CloseFieldCode", nRow);
        mission_Info_ptr->complete_trigger_id = GetDataDWORD("CompleteTgrID", nRow);
        mission_Info_ptr->is_complete = false; // 초기값은 미션 미완료
        mission_Info_ptr->is_show = false; // 초기값은 안보이게
		mission_object_data_table_.push_back(mission_Info_ptr);
	}
    return TRUE;
}

BOOL MissionInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL reload)
{
	switch (script_code)
	{
	case SCRIPT_DISPLAY_FOR_MISSION_OBJECT:
		return _Load(reload);
	}

    return FALSE;	
}

int MissionInfoParser::GetMissionInfo(MISSION_OBJECT_LIST *list_ptr, DWORD map_code)
{
    int world_name_code = 0;
    _MissionObject* data = NULL;
    MISSION_OBJECT_LIST_ITR itr;
    MISSION_OBJECT_LIST_ITR end = mission_object_data_table_.end();

    for(itr = mission_object_data_table_.begin(); itr != end; ++itr)
    {
        data = (*itr);
        if(data == NULL)
        {
            continue;
        }
        if(data->map_code == map_code)
        {
            _MissionObject *temp_data = new _MissionObject;
            memcpy((void*)temp_data, (void*)data, sizeof(_MissionObject));
            world_name_code = temp_data->name_code;
            list_ptr->push_back(temp_data);
        }
    }
    return world_name_code;
}
