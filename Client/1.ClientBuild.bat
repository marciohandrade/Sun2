@echo off
set choice_build=2
set /p choice_build=☞ Build 는 1 을 선택, Rebuild All 은 2 를 선택하세요 (1,2)
if not '%choice_build%'=='1' goto rebuild

rem Build
BuildConsole GameClient\GameClient.sln /cfg="Release" /log=GameClient /ShowTime
BuildConsole GameClient\GameClient.sln /cfg="Release - 유저 배포 버전" /log=SunGame /ShowTime
BuildConsole GameClient\GameClient.sln /cfg="Release - 유저 배포 버전 ( 덤프 남김 )" /log=SunGameDump /ShowTime


goto end

:rebuild

rem Rebuild All
BuildConsole GameClient\GameClient.sln /cfg="Release" /rebuild /log=GameClient /ShowTime
BuildConsole GameClient\GameClient.sln /cfg="Release - 유저 배포 버전" /rebuild /log=SunGame /ShowTime
BuildConsole GameClient\GameClient.sln /cfg="Release - 유저 배포 버전 ( 덤프 남김 )" /rebuild /log=SunGameDump /ShowTime

:end