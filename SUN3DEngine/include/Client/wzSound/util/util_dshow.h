#ifndef __UTIL_DSHOW_H__
#define __UTIL_DSHOW_H__
// util_dshow.h

#include <windows.h>
#include <assert.h>
#include <atlbase.h>
#include <atlconv.h>
#include <dshow.h>


//////////////////////////////////////////////////////////////////////////

#define WM_GRAPHNOTIFY	(WM_APP + 1)
#define WM_GRAPH_EOF	(WM_APP +2)

#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

//#define JIF(x) { if (FAILED(hr=(x))) \
//	{ TRACE(TEXT("FAILED(hr=0x%x) in ") TEXT(#x) TEXT("\n"), hr); throw hr; }}

#define LIF(x) { if (FAILED(hr=(x))) \
	{ /*_LOG(TEXT("FAILED(hr=0x%x) in ") TEXT(#x), hr); */throw hr; }}


#define CONNECTPINS(x, y, z)			\
{										\
	if ((y != NULL) && (z != NULL))		\
	{									\
		IPin *pPin;						\
		z->ConnectedTo(&pPin);			\
										\
		if (pPin == NULL)				\
			x->Connect(y, z);			\
		else							\
			pPin->Release();			\
	}									\
}


//
// Useful Defines
//

#define ONE_SECOND	10000000

HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
void RemoveGraphFromRot(DWORD pdwRegister);
HRESULT RenderFilter(IGraphBuilder* pFilterGraph, IBaseFilter* pFilter);

HWND MakeHelperWindow(char* pClassname, WNDPROC lpfnWndProc);

HRESULT GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);

#endif //__UTIL_DSHOW_H__