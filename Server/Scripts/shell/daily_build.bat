: SUN Online Server Daily Build Script Runner Sample

@echo off
setlocal

rem release destination root path
set RELEASE_ROOT=D:\Release.DailyBuilds

rem path to ruby.exe
set RUBY_PATH=C:\ruby\bin
rem path to svn.exe
set SVN_PATH=C:\Program Files\CollabNet Subversion Client
rem path to devenv.exe
set DEVENV_PATH=C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE
rem Perforce clientspec root path (not used at this point)
set P4_ROOT_PATH=D:\Perforce

set PATH=%PATH%;%SVN_PATH%;%DEVENV_PATH%

rem subversion username
set USERNAME=serverbuild
rem subversion password
set PASSWORD=qlfemtjqj

rem game server group build option specification : "--buildgame" or ""
set BUILD_GAME=--buildgame
rem auth server group build option specification : "--buildauth" or ""
set BUILD_AUTH=--buildauth
rem rebuild option specification : "--rebuild" for full rebuild, or ""
set REBUILD=--rebuild
rem gamelib option specification : "--gamelib" for game logic lib release, or ""
set GAMELIB=--gamelib

rem source repository url
set REPO_URL=svn://10.101.102.1/SUN/trunk

rem update release scripts at the current working directory
echo Updating scripts...
svn.exe up . --username %USERNAME% --password %PASSWORD% --no-auth-cache --non-interactive

%RUBY_PATH%\ruby.exe daily_build.rb -u %USERNAME% -p %PASSWORD% %BUILD_GAME% %BUILD_AUTH% %REBUILD% %GAMELIB% -c "%P4_ROOT_PATH%" -t "%RELEASE_ROOT%" "%REPO_URL%"

endlocal