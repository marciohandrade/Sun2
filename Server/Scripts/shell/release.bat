: SUN Online Server Release Script Runner Sample

@echo off
setlocal

rem release destination root path
set RELEASE_ROOT=D:\Release

rem path to ruby.exe
set RUBY_PATH=C:\ruby\bin
rem path to svn.exe
set SVN_PATH=C:\Program Files\CollabNet Subversion Client
rem path to devenv.exe
set DEVENV_PATH=C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE
rem Perforce clientspec root path (not used at this point)
set P4_ROOT_PATH=D:\Perforce
rem path to 7z.exe
set ZIP_PATH=C:\Program Files\7-Zip

set PATH=%PATH%;%SVN_PATH%;%DEVENV_PATH%;%ZIP_PATH%

rem subversion username
set USERNAME=serverbuild
rem subversion password
set PASSWORD=qlfemtjqj

rem quiet option specification : "--quiet" for no console interaction, or ""
set QUIET=
rem source update option specification : "--no-update" for no HEAD update, or ""
set NO_UPDATE=
rem game server group build option specification : "--buildgame" or ""
set BUILD_GAME=--buildgame
rem auth server group build option specification : "--buildauth" or ""
set BUILD_AUTH=--buildauth
rem rebuild option specification : "--rebuild" for full rebuild, or ""
set REBUILD=
rem gamelib option specification : "--gamelib" for game logic lib release, or ""
set GAMELIB=--gamelib
rem loglib option specification : "--loglib" for game log lib release, or ""
set LOGLIB=--loglib

rem source repository url
set REPO_URL=svn://10.101.102.1/SUN/trunk

rem update release scripts at the current working directory
echo Updating scripts...
svn.exe up . --username %USERNAME% --password %PASSWORD% --no-auth-cache --non-interactive

%RUBY_PATH%\ruby.exe release.rb -u %USERNAME% -p %PASSWORD% %QUIET% %NO_UPDATE% %BUILD_GAME% %BUILD_AUTH% %REBUILD% %GAMELIB% %LOGLIB% -c "%P4_ROOT_PATH%" -t "%RELEASE_ROOT%" "%REPO_URL%"

if not "%QUIET%" == "--quiet" pause

endlocal