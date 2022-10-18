

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Jan 19 10:01:53 2006
 */
/* Compiler settings for _gameLogic_Sun.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef ___gameLogic_Sun_h__
#define ___gameLogic_Sun_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICSunOnline_FWD_DEFINED__
#define __ICSunOnline_FWD_DEFINED__
typedef interface ICSunOnline ICSunOnline;
#endif 	/* __ICSunOnline_FWD_DEFINED__ */


#ifndef __CCSunOnline_FWD_DEFINED__
#define __CCSunOnline_FWD_DEFINED__

#ifdef __cplusplus
typedef class CCSunOnline CCSunOnline;
#else
typedef struct CCSunOnline CCSunOnline;
#endif /* __cplusplus */

#endif 	/* __CCSunOnline_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ICSunOnline_INTERFACE_DEFINED__
#define __ICSunOnline_INTERFACE_DEFINED__

/* interface ICSunOnline */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICSunOnline;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("14E845B9-0298-4CA1-97AC-6E9536865051")
    ICSunOnline : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE decodeSkill( 
            /* [in] */ BYTE buff[ 200 ],
            /* [retval][out] */ BSTR *bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE decodeMission( 
            /* [in] */ BYTE buff[ 128 ],
            /* [retval][out] */ BSTR *bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE decodeInventoryItem( 
            /* [in] */ BYTE buff[ 1425 ],
            /* [retval][out] */ BSTR *bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE decodeTempInventoryItem( 
            /* [in] */ BYTE buff[ 380 ],
            /* [retval][out] */ BSTR *bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE decodeEquipItem( 
            /* [in] */ BYTE buff[ 323 ],
            /* [retval][out] */ BSTR *bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE decodeWarehouseItem( 
            /* [in] */ BYTE buff[ 1900 ],
            /* [retval][out] */ BSTR *bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE encodeSkill( 
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 80000 ],
            /* [retval][out] */ int *nResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE encodeMission( 
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 128 ],
            /* [retval][out] */ int *nResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE encodeInventoryItem( 
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 1425 ],
            /* [retval][out] */ int *nResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE encodeTempInventoryItem( 
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 380 ],
            /* [retval][out] */ int *nResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE encodeEquipItem( 
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 323 ],
            /* [retval][out] */ int *nResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE encodeWarehouseItem( 
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 1900 ],
            /* [retval][out] */ int *nResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE init( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE release( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE createItem( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICSunOnlineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICSunOnline * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICSunOnline * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICSunOnline * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICSunOnline * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICSunOnline * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICSunOnline * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICSunOnline * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *decodeSkill )( 
            ICSunOnline * This,
            /* [in] */ BYTE buff[ 200 ],
            /* [retval][out] */ BSTR *bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *decodeMission )( 
            ICSunOnline * This,
            /* [in] */ BYTE buff[ 128 ],
            /* [retval][out] */ BSTR *bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *decodeInventoryItem )( 
            ICSunOnline * This,
            /* [in] */ BYTE buff[ 1275 ],
            /* [retval][out] */ BSTR *bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *decodeTempInventoryItem )( 
            ICSunOnline * This,
            /* [in] */ BYTE buff[ 340 ],
            /* [retval][out] */ BSTR *bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *decodeEquipItem )( 
            ICSunOnline * This,
            /* [in] */ BYTE buff[ 289 ],
            /* [retval][out] */ BSTR *bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *decodeWarehouseItem )( 
            ICSunOnline * This,
            /* [in] */ BYTE buff[ 1700 ],
            /* [retval][out] */ BSTR *bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *encodeSkill )( 
            ICSunOnline * This,
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 80000 ],
            /* [retval][out] */ int *nResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *encodeMission )( 
            ICSunOnline * This,
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 128 ],
            /* [retval][out] */ int *nResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *encodeInventoryItem )( 
            ICSunOnline * This,
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 1275 ],
            /* [retval][out] */ int *nResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *encodeTempInventoryItem )( 
            ICSunOnline * This,
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 340 ],
            /* [retval][out] */ int *nResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *encodeEquipItem )( 
            ICSunOnline * This,
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 289 ],
            /* [retval][out] */ int *nResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *encodeWarehouseItem )( 
            ICSunOnline * This,
            /* [in] */ BSTR bstrXml,
            /* [out] */ BYTE buff[ 1700 ],
            /* [retval][out] */ int *nResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *init )( 
            ICSunOnline * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *release )( 
            ICSunOnline * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *createItem )( 
            ICSunOnline * This);
        
        END_INTERFACE
    } ICSunOnlineVtbl;

    interface ICSunOnline
    {
        CONST_VTBL struct ICSunOnlineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICSunOnline_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICSunOnline_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICSunOnline_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICSunOnline_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICSunOnline_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICSunOnline_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICSunOnline_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICSunOnline_decodeSkill(This,buff,bstrXml)	\
    (This)->lpVtbl -> decodeSkill(This,buff,bstrXml)

#define ICSunOnline_decodeMission(This,buff,bstrXml)	\
    (This)->lpVtbl -> decodeMission(This,buff,bstrXml)

#define ICSunOnline_decodeInventoryItem(This,buff,bstrXml)	\
    (This)->lpVtbl -> decodeInventoryItem(This,buff,bstrXml)

#define ICSunOnline_decodeTempInventoryItem(This,buff,bstrXml)	\
    (This)->lpVtbl -> decodeTempInventoryItem(This,buff,bstrXml)

#define ICSunOnline_decodeEquipItem(This,buff,bstrXml)	\
    (This)->lpVtbl -> decodeEquipItem(This,buff,bstrXml)

#define ICSunOnline_decodeWarehouseItem(This,buff,bstrXml)	\
    (This)->lpVtbl -> decodeWarehouseItem(This,buff,bstrXml)

#define ICSunOnline_encodeSkill(This,bstrXml,buff,nResult)	\
    (This)->lpVtbl -> encodeSkill(This,bstrXml,buff,nResult)

#define ICSunOnline_encodeMission(This,bstrXml,buff,nResult)	\
    (This)->lpVtbl -> encodeMission(This,bstrXml,buff,nResult)

#define ICSunOnline_encodeInventoryItem(This,bstrXml,buff,nResult)	\
    (This)->lpVtbl -> encodeInventoryItem(This,bstrXml,buff,nResult)

#define ICSunOnline_encodeTempInventoryItem(This,bstrXml,buff,nResult)	\
    (This)->lpVtbl -> encodeTempInventoryItem(This,bstrXml,buff,nResult)

#define ICSunOnline_encodeEquipItem(This,bstrXml,buff,nResult)	\
    (This)->lpVtbl -> encodeEquipItem(This,bstrXml,buff,nResult)

#define ICSunOnline_encodeWarehouseItem(This,bstrXml,buff,nResult)	\
    (This)->lpVtbl -> encodeWarehouseItem(This,bstrXml,buff,nResult)

#define ICSunOnline_init(This)	\
    (This)->lpVtbl -> init(This)

#define ICSunOnline_release(This)	\
    (This)->lpVtbl -> release(This)

#define ICSunOnline_createItem(This)	\
    (This)->lpVtbl -> createItem(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_decodeSkill_Proxy( 
    ICSunOnline * This,
    /* [in] */ BYTE buff[ 200 ],
    /* [retval][out] */ BSTR *bstrXml);


void __RPC_STUB ICSunOnline_decodeSkill_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_decodeMission_Proxy( 
    ICSunOnline * This,
    /* [in] */ BYTE buff[ 128 ],
    /* [retval][out] */ BSTR *bstrXml);


void __RPC_STUB ICSunOnline_decodeMission_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_decodeInventoryItem_Proxy( 
    ICSunOnline * This,
    /* [in] */ BYTE buff[ 1275 ],
    /* [retval][out] */ BSTR *bstrXml);


void __RPC_STUB ICSunOnline_decodeInventoryItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_decodeTempInventoryItem_Proxy( 
    ICSunOnline * This,
    /* [in] */ BYTE buff[ 340 ],
    /* [retval][out] */ BSTR *bstrXml);


void __RPC_STUB ICSunOnline_decodeTempInventoryItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_decodeEquipItem_Proxy( 
    ICSunOnline * This,
    /* [in] */ BYTE buff[ 289 ],
    /* [retval][out] */ BSTR *bstrXml);


void __RPC_STUB ICSunOnline_decodeEquipItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_decodeWarehouseItem_Proxy( 
    ICSunOnline * This,
    /* [in] */ BYTE buff[ 1700 ],
    /* [retval][out] */ BSTR *bstrXml);


void __RPC_STUB ICSunOnline_decodeWarehouseItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_encodeSkill_Proxy( 
    ICSunOnline * This,
    /* [in] */ BSTR bstrXml,
    /* [out] */ BYTE buff[ 80000 ],
    /* [retval][out] */ int *nResult);


void __RPC_STUB ICSunOnline_encodeSkill_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_encodeMission_Proxy( 
    ICSunOnline * This,
    /* [in] */ BSTR bstrXml,
    /* [out] */ BYTE buff[ 128 ],
    /* [retval][out] */ int *nResult);


void __RPC_STUB ICSunOnline_encodeMission_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_encodeInventoryItem_Proxy( 
    ICSunOnline * This,
    /* [in] */ BSTR bstrXml,
    /* [out] */ BYTE buff[ 1275 ],
    /* [retval][out] */ int *nResult);


void __RPC_STUB ICSunOnline_encodeInventoryItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_encodeTempInventoryItem_Proxy( 
    ICSunOnline * This,
    /* [in] */ BSTR bstrXml,
    /* [out] */ BYTE buff[ 340 ],
    /* [retval][out] */ int *nResult);


void __RPC_STUB ICSunOnline_encodeTempInventoryItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_encodeEquipItem_Proxy( 
    ICSunOnline * This,
    /* [in] */ BSTR bstrXml,
    /* [out] */ BYTE buff[ 289 ],
    /* [retval][out] */ int *nResult);


void __RPC_STUB ICSunOnline_encodeEquipItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_encodeWarehouseItem_Proxy( 
    ICSunOnline * This,
    /* [in] */ BSTR bstrXml,
    /* [out] */ BYTE buff[ 1700 ],
    /* [retval][out] */ int *nResult);


void __RPC_STUB ICSunOnline_encodeWarehouseItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_init_Proxy( 
    ICSunOnline * This);


void __RPC_STUB ICSunOnline_init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_release_Proxy( 
    ICSunOnline * This);


void __RPC_STUB ICSunOnline_release_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICSunOnline_createItem_Proxy( 
    ICSunOnline * This);


void __RPC_STUB ICSunOnline_createItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICSunOnline_INTERFACE_DEFINED__ */



#ifndef __gameLogic_Sun_LIBRARY_DEFINED__
#define __gameLogic_Sun_LIBRARY_DEFINED__

/* library gameLogic_Sun */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_gameLogic_Sun;

EXTERN_C const CLSID CLSID_CCSunOnline;

#ifdef __cplusplus

class DECLSPEC_UUID("28094A5A-19C9-4ED9-AC75-AC8369AB1E11")
CCSunOnline;
#endif
#endif /* __gameLogic_Sun_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


