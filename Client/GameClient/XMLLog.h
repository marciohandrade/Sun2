//-------------------------------------------------------------------------------------------
/** FTextParam	
    @Author : 유재영 ( y2inc@webzen.co.kr )
    @Desc  : XMLparser를 상속 받아서 XML 형식으로 로그를 남기는 클래스
    @Since : 2006. 7. 31.

    @Function
    @TODO
*/
#ifdef _USE_XMLLOG
#pragma once
#include "xmlparser.h"

class XMLLog
{
public:
    XMLLog(void);
    virtual ~XMLLog(void);
};

#endif