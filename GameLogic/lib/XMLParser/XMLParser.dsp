# Microsoft Developer Studio Project File - Name="XMLParser" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=XMLParser - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XMLParser.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XMLParser.mak" CFG="XMLParser - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XMLParser - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XMLParser - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "XMLParser - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XMLParser - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "XMLParser"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XMLParser - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_XML_PARSER" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0x80c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x80c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\XMLParser.lib"

!ELSEIF  "$(CFG)" == "XMLParser - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /ML /W3 /Gm /GX /ZI /Od /I "../zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_XML_PARSER" /YX"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x80c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x80c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\XMLParserd.lib"

!ELSEIF  "$(CFG)" == "XMLParser - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XMLParser___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "XMLParser___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XMLParser___Win32_Unicode_Release"
# PROP Intermediate_Dir "XMLParser___Win32_Unicode_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_XML_PARSER" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_XML_PARSER" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x80c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x80c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\XMLParser.lib"
# ADD LIB32 /nologo /out:"..\lib\XMLParseru.lib"

!ELSEIF  "$(CFG)" == "XMLParser - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "XMLParser___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "XMLParser___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "XMLParser___Win32_Unicode_Debug"
# PROP Intermediate_Dir "XMLParser___Win32_Unicode_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_XML_PARSER" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_XML_PARSER" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x80c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x80c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\XMLParserd.lib"
# ADD LIB32 /nologo /out:"..\lib\XMLParserud.lib"

!ENDIF 

# Begin Target

# Name "XMLParser - Win32 Release"
# Name "XMLParser - Win32 Debug"
# Name "XMLParser - Win32 Unicode Release"
# Name "XMLParser - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MarkupArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkupSTL.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\STL2String.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MarkupArchive.h
# End Source File
# Begin Source File

SOURCE=.\MarkupSTL.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\STL2String.h
# End Source File
# Begin Source File

SOURCE=.\StLXMLParser.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
