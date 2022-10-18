#pragma once

#include "scenebase.h"

#include "GraphicResourceList.h"
#include "../XMLParser/StdString.h"
#define CString CStdString

#include "../XMLParser/MarkupArchive.h"
using namespace markup;
using namespace std;

struct TestProgressInfoData
{
    DWORD dwTemp[10];
};


class ResourceTest;

typedef BOOL (ResourceTest::*TestingFunc)(TestProgressInfoData *);

enum eFILETYPE
{
    eFILETYPE_NONE = -1,
    eFILETYPE_WZU,
    eFILETYPE_TGA,
    eFILETYPE_CUR,
};

#define DELAY_END_TICK      5000


class ResourceTest : public SceneBase
{
public:
    enum eTEST_STATUS
    {
        eTEST_STATUS_NONE,
        eTEST_STATUS_START,
        eTEST_STATUS_GRAPHIC_RESOURCE,
        eTEST_STATUS_EFFECT,
        eTEST_STATUS_MAP,
        eTEST_STATUS_INTERFACE,
        eTEST_STATUS_END,
    };

    enum eLOAD_STAUTS
    {
        eLOAD_STATUS_OK                 = 0x01,
        eLOAD_STATUS_FAILED             = 0x02,
        eLOAD_STATUS_FAILED_NOT_EXIST   = 0x04,
    };

    ResourceTest(void);
    virtual ~ResourceTest(void);

public:
    // virtual methods
    BOOL    Init( void );						// 초기화, 로딩
    BOOL    Process( DWORD dwTick );			// 메인 계산
    BOOL    Render( DWORD dwTick );			    // 메인 랜더링
    void    Release( void );					// 종료

    void    Testing();

    BOOL    TestingGraphicResource(TestProgressInfoData * pData);
    BOOL    TestingMap(TestProgressInfoData * pData);
    BOOL    TestingEffect(TestProgressInfoData * pData);    
    BOOL    TestingInterface(TestProgressInfoData * pData);    
    BOOL    TestingEnd(TestProgressInfoData * pData);  

protected:
    void    forwardTest();
    void    changeTest(eTEST_STATUS eTestType);

protected:
    BOOL    writeLogGraphicResource( eLOAD_STAUTS eLoadStatus, RESOURCE_INFO * pInfo ,TCHAR* filename = NULL);
    BOOL    writeLogMap( eLOAD_STAUTS eLoadStatus, MAPID MapID, FIELDID FieldID);


private:
    eTEST_STATUS            m_eCurStatus;
    eTEST_STATUS            m_eNextStatus;
    TestingFunc             m_pfTestFunc;
    TestProgressInfoData    m_TokenData;

    CMarkupArchive *        m_pXMLLog;
    
};

extern ResourceTest	g_ResourceTestScene;

