ECHO OFF

REM	============================================================================================================
@SET	VSINSTALLDIR=C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE
@SET	VCINSTALLDIR=C:\Program Files\Microsoft Visual Studio .NET 2003
@SET	FrameworkDir=C:\WINDOWS\Microsoft.NET\Framework
@SET	FrameworkVersion=v1.1.4322
@SET	FrameworkSDKDir=C:\Program Files\Microsoft Visual Studio .NET 2003\SDK\v1.1
@IF	"%VCINSTALLDIR%"==""	SET VCINSTALLDIR=%VSINSTALLDIR%
@SET	DevEnvDir=%VSINSTALLDIR%
@SET	MSVCDir=%VCINSTALLDIR%\VC7

@SET	PATH=%DevEnvDir%;%MSVCDir%\BIN;%VCINSTALLDIR%\Common7\Tools;%VCINSTALLDIR%\Common7\Tools\bin\prerelease;%VCINSTALLDIR%\Common7\Tools\bin;%FrameworkSDKDir%\bin;%FrameworkDir%\%FrameworkVersion%;%PATH%;
@SET	INCLUDE=%MSVCDir%\ATLMFC\INCLUDE;%MSVCDir%\INCLUDE;%MSVCDir%\PlatformSDK\include\prerelease;%MSVCDir%\PlatformSDK\include;%FrameworkSDKDir%\include;%INCLUDE%
@SET	LIB=%MSVCDir%\ATLMFC\LIB;%MSVCDir%\LIB;%MSVCDir%\PlatformSDK\lib\prerelease;%MSVCDir%\PlatformSDK\lib;%FrameworkSDKDir%\lib;%LIB%

REM ========================================================================================
ECHO	BUILD_LIB
	devenv /clean "Debug - (서버용)" /project "MOLA" ".\SunServer.MOLA.sln"
	devenv /build "Debug - (서버용)" /project "MOLA" ".\SunServer.MOLA.sln"
REM ------------------------------------------------
	SET	DEBUG_DIR=.\DEBUG
	SET	DEST_DIR=%BACKUP_DIR%\MOLA_sd
	ECHO	COMPLETE AND BACKUP_LIB %DEST_DIR%

REM ------------------------------------------------

	devenv /clean "Release" /project "MOLA" ".\SunServer.MOLA.sln"
	devenv /build "Release" /project "MOLA" ".\SunServer.MOLA.sln"
REM ------------------------------------------------
	SET	DEBUG_DIR=.\RELEASE
	SET	DEST_DIR=%BACKUP_DIR%\MOLA
	ECHO	COMPLETE AND BACKUP_LIB %DEST_DIR%

REM ------------------------------------------------

	devenv /clean "Release - (서버용)" /project "MOLA" ".\SunServer.MOLA.sln"
	devenv /build "Release - (서버용)" /project "MOLA" ".\SunServer.MOLA.sln"
REM ------------------------------------------------
	SET	DEBUG_DIR=.\RELEASE
	SET	DEST_DIR=%BACKUP_DIR%\MOLA_s
	ECHO	COMPLETE AND BACKUP_LIB %DEST_DIR%

REM ------------------------------------------------

REM ========================================================================================

	PAUSE

REM ========================================================================================
