#ifdef _DLL_EXPORT_
#define VMAPI  extern "C" __declspec(dllexport) 
#else
#define VMAPI  extern "C" __declspec(dllimport) 
#endif



