# Microsoft Developer Studio Project File - Name="ClientDialog" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ClientDialog - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ClientDialog_VS60.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ClientDialog_VS60.mak" CFG="ClientDialog - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ClientDialog - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ClientDialog - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ClientDialog - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "include" /I "../ClientStub" /I "../public_src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "VERSION_VS60" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 SHELL32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"msvcrt.lib" /out:"../_bin/ClientDialog_VC60.exe" /libpath:"../_lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "ClientDialog - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /I "../ClientStub" /I "../public_src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "VERSION_VS60" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../_bin/ClientDialog_VC60d.exe" /pdbtype:sept /libpath:"../_lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ClientDialog - Win32 Release"
# Name "ClientDialog - Win32 Debug"
# Begin Group "include"

# PROP Default_Filter ""
# Begin Group "BillingClient"

# PROP Default_Filter ""
# Begin Group "interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\BillingClient\interface\LibClientSession.cpp

!IF  "$(CFG)" == "ClientDialog - Win32 Release"

!ELSEIF  "$(CFG)" == "ClientDialog - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\include\BillingClient\interface\LibClientSession.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\include\BillingClient\BillingClientDlg.cpp

!IF  "$(CFG)" == "ClientDialog - Win32 Release"

!ELSEIF  "$(CFG)" == "ClientDialog - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\include\BillingClient\BillingClientDlg.h
# End Source File
# End Group
# Begin Group "ShopClient"

# PROP Default_Filter ""
# Begin Group "interface No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\ShopClient\interface\ShopEH.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ShopClient\interface\ShopEH.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\include\ShopClient\ShopClientDlg.cpp

!IF  "$(CFG)" == "ClientDialog - Win32 Release"

!ELSEIF  "$(CFG)" == "ClientDialog - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\include\ShopClient\ShopClientDlg.h
# End Source File
# Begin Source File

SOURCE=.\include\ShopClient\ShopClientDlgUIHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ShopClient\ShopClientDlgUIHelper.h
# End Source File
# End Group
# Begin Group "ShopClientAR"

# PROP Default_Filter ""
# Begin Group "interface No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\ShopClientAR\interface\ShopEH_AR.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ShopClientAR\interface\ShopEH_AR.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\include\ShopClientAR\ShopClientDlg_AR.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ShopClientAR\ShopClientDlg_AR.h
# End Source File
# End Group
# Begin Group "ShopUpdate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\ShopUpdate\ShopNotifyImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ShopUpdate\ShopNotifyImpl.h
# End Source File
# Begin Source File

SOURCE=.\include\ShopUpdate\ShopUpdateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ShopUpdate\ShopUpdateDlg.h
# End Source File
# End Group
# Begin Group "tool"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\tool\ClientDialog.rc
# End Source File
# Begin Source File

SOURCE=.\include\tool\resource.h
# End Source File
# Begin Source File

SOURCE=.\include\tool\ScrollDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\include\tool\ScrollDialog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\include\ClientDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ClientDialog.h
# End Source File
# Begin Source File

SOURCE=.\include\ClientDialogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ClientDialogDlg.h
# End Source File
# Begin Source File

SOURCE=.\include\ImportLibrary.h
# End Source File
# Begin Source File

SOURCE=.\include\stdafx.cpp

!IF  "$(CFG)" == "ClientDialog - Win32 Release"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "ClientDialog - Win32 Debug"

# ADD CPP /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\include\stdafx.h
# End Source File
# End Group
# End Target
# End Project
