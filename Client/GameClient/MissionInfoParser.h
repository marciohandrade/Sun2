#ifndef SUN_DISPLAYMISSIONOBJECT_H_
#define SUN_DISPLAYMISSIONOBJECT_H_

#include <SolarHashTable.h>
#include <Singleton.h>

struct _MissionObject
{
    enum
    { 
        MAX_OPEN_TRIGGER_ID = 5,
    };
    DWORD       index;
    DWORD       map_code;               
    DWORD       name_code;              
    DWORD       field_code;             
    DWORD       mission_text_code;
    DWORD       open_trigger_id[MAX_OPEN_TRIGGER_ID];
    DWORD       complete_field_code;
    DWORD       complete_trigger_id;
    bool        is_complete;
    bool        is_show;
    BYTE        show_count;
};
typedef std::list<_MissionObject*>				MISSION_OBJECT_LIST;
typedef std::list<_MissionObject*>::iterator	MISSION_OBJECT_LIST_ITR;

class MissionInfoParser : public Singleton<MissionInfoParser>, public IParser
{
public:
    MissionInfoParser(void);
	virtual	~MissionInfoParser(void);

    VOID Init();
	VOID Init(char* pack_file_name);
	VOID Release();

	virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reload);
    int GetMissionInfo(MISSION_OBJECT_LIST *list_ptr, DWORD map_code);

	MISSION_OBJECT_LIST mission_object_data_table_;
private:
    BOOL _Load(BOOL reload);
};

#endif //SUN_DISPLAYMISSIONOBJECT_H_
