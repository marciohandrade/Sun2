@echo off
set choice_build=2
set /p choice_build=�� Build �� 1 �� ����, Rebuild All �� 2 �� �����ϼ��� (1,2)
if not '%choice_build%'=='1' goto rebuild

rem Build
BuildConsole GameClient\GameClient.sln /cfg="Release" /log=GameClient /ShowTime
BuildConsole GameClient\GameClient.sln /cfg="Release - ���� ���� ����" /log=SunGame /ShowTime
BuildConsole GameClient\GameClient.sln /cfg="Release - ���� ���� ���� ( ���� ���� )" /log=SunGameDump /ShowTime


goto end

:rebuild

rem Rebuild All
BuildConsole GameClient\GameClient.sln /cfg="Release" /rebuild /log=GameClient /ShowTime
BuildConsole GameClient\GameClient.sln /cfg="Release - ���� ���� ����" /rebuild /log=SunGame /ShowTime
BuildConsole GameClient\GameClient.sln /cfg="Release - ���� ���� ���� ( ���� ���� )" /rebuild /log=SunGameDump /ShowTime

:end