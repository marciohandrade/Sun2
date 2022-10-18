#pragma once

#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION

#include <SolarHashTable.h>
#include <Singleton.h>

// style_type 1 �� �� 
// special_animation[4]   0:�ɴ���, 1:�ɾ��ֱ�, 2:�ɱ���̵�, 3:�Ͼ��
// style_type 2 �� �� 
// special_animation[4]   0:�ɱ� �ܰ躰 ������� �ش� �ִϸ��̼� �ݺ�
struct SpaZoneInfo
{
	BYTE        index;
    DWORD       map_code;
    BYTE        style_type;             // Ư�� �ִϸ��̼� ��Ÿ��
    tagWZID     special_animation[4];   // Ư�� �ִϸ��̼� �ڵ�  0:�ɴ���, 1:�ɾ��ֱ�, 2:�ɱ���̵�, 3:�Ͼ��
    DWORD       base_costume_code;      // �⺻ ����� ������ (�ʱⰪ ����)
};


class SpaZoneInfoParser	: public Singleton<SpaZoneInfoParser> ,public IParser
{
public:
    enum SpaAnimationStyle            // Ư�� �ִϸ��̼� ��Ÿ��
    {
        AnimationStyle_None = 0,

        AnimationStyle_SitIdle = 1,   // �ɱ�IDLE �� ���� - ��õ��Ÿ��
        AnimationStyle_Sit,           // �ɱ� ��ü�� ����

        AnimationStyle_End,
    };

	SpaZoneInfoParser();
	~SpaZoneInfoParser();


	VOID                    Init(DWORD dwPoolSize , char *szPackfileName);
	VOID                    Init(DWORD dwPoolSize);

	VOID                    Release();
	virtual	BOOL            LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);


	SpaZoneInfo*    GetSpaZoneInfo(DWORD map_code);
    SpaAnimationStyle GetSpaAnimationStyle(DWORD map_code);
private:
	VOID		            Unload();
	util::SolarHashTable<SpaZoneInfo*>*     m_pDataTable;
};

#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION