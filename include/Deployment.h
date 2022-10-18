#pragma once

//국내를 제외한 해외 서버에서는 두개의 define이 필요하고 국내는 주석처리한다.
//#define __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//#define __WAVERIX_DATA_STRUCTURE_RELOCATION__
//

// 다음 NATION_BUILD_VERSION이 활성화되면 Version.h의 설정보다 우선한다.
// 최신개발 서버 등에서 일시적으로 trunk 소스의 Version.h를 변경하지 않으면서 
// 다른 버전을 빌드해야 할 필요가 있는 경우에는
// 이 매크로 정의를 적절히 설정해 활성화할 수 있다.
//
#define NATION_BUILD_VERSION 16001 // 빌드버전(서버, 클라 공통)