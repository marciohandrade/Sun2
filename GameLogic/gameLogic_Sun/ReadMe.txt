========================================================================
    ACTIVE TEMPLATE LIBRARY : gameLogic_Sun Project Overview
========================================================================

AppWizard has created this gameLogic_Sun project for you to use as the starting point for
writing your Dynamic Link Library (DLL).
This project is implemented with Visual C++ attributes.

This file contains a summary of what you will find in each of the files that
make up your project.

gameLogic_Sun.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

_gameLogic_Sun.idl
    This file will be generated by the compiler when the project is built. It will contain the IDL 
    definitions of the type library, the interfaces and co-classes defined in your project.
    This file will be processed by the MIDL compiler to generate:
        C++ interface definitions and GUID declarations (_gameLogic_Sun.h)
        GUID definitions                                (_gameLogic_Sun_i.c)
        A type library                                  (_gameLogic_Sun.tlb)
        Marshaling code                                 (_gameLogic_Sun_p.c and dlldata.c)
gameLogic_Sun.cpp
    This file contains the object map and the implementation of your DLL's exports.
gameLogic_Sun.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.

gameLogic_Sun.def
    This module-definition file provides the linker with information about the exports
    required by your DLL. It contains exports for:
        DllGetClassObject  
        DllCanUnloadNow    
        GetProxyDllInfo    
        DllRegisterServer	
        DllUnregisterServer

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named gameLogic_Sun.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file that defines resource IDs.

/////////////////////////////////////////////////////////////////////////////
Proxy/stub DLL project and module definition file:

gameLogic_Sunps.vcproj
    This file is the project file for building a proxy/stub DLL if necessary.
	The IDL file in the main project must contain at least one interface and you must 
	first compile the IDL file before building the proxy/stub DLL.	This process generates
	dlldata.c, gameLogic_Sun_i.c and gameLogic_Sun_p.c which are required
	to build the proxy/stub DLL.

gameLogic_Sunps.def
    This module definition file provides the linker with information about the exports
    required by the proxy/stub.
/////////////////////////////////////////////////////////////////////////////
