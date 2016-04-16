

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Feb 25 13:46:23 2016
 */
/* Compiler settings for NmdMul.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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

#ifndef __NmdMul_h__
#define __NmdMul_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWlNamedMult_FWD_DEFINED__
#define __IWlNamedMult_FWD_DEFINED__
typedef interface IWlNamedMult IWlNamedMult;

#endif 	/* __IWlNamedMult_FWD_DEFINED__ */


#ifndef __NmdMul_FWD_DEFINED__
#define __NmdMul_FWD_DEFINED__

#ifdef __cplusplus
typedef class NmdMul NmdMul;
#else
typedef struct NmdMul NmdMul;
#endif /* __cplusplus */

#endif 	/* __NmdMul_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IWlNamedMult_INTERFACE_DEFINED__
#define __IWlNamedMult_INTERFACE_DEFINED__

/* interface IWlNamedMult */
/* [object][unique][helpstring][uuid] */ 


EXTERN_C const IID IID_IWlNamedMult;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C7212134-7716-101A-AA54-00608C61D0B1")
    IWlNamedMult : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE put_FileName( 
            /* [in] */ const unsigned char *FileName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ const unsigned char *Profile) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_ColumnCount( 
            /* [out] */ short *Count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_MultCount( 
            /* [out] */ short *Count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_MultTitle( 
            /* [in] */ short Mult,
            /* [out] */ unsigned char **Title) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Column( 
            /* [in] */ short Mult,
            /* [out] */ short *Column) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_ColumnTitle( 
            /* [in] */ short Column,
            /* [out] */ unsigned char **Title) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IndexFromName( 
            /* [in] */ const unsigned char *Name,
            /* [out] */ short *Mult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NameFromIndex( 
            /* [in] */ short Mult,
            /* [out] */ unsigned char **Name) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IWlNamedMultVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWlNamedMult * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWlNamedMult * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWlNamedMult * This);
        
        HRESULT ( STDMETHODCALLTYPE *put_FileName )( 
            IWlNamedMult * This,
            /* [in] */ const unsigned char *FileName);
        
        HRESULT ( STDMETHODCALLTYPE *Init )( 
            IWlNamedMult * This,
            /* [in] */ const unsigned char *Profile);
        
        HRESULT ( STDMETHODCALLTYPE *get_ColumnCount )( 
            IWlNamedMult * This,
            /* [out] */ short *Count);
        
        HRESULT ( STDMETHODCALLTYPE *get_MultCount )( 
            IWlNamedMult * This,
            /* [out] */ short *Count);
        
        HRESULT ( STDMETHODCALLTYPE *get_MultTitle )( 
            IWlNamedMult * This,
            /* [in] */ short Mult,
            /* [out] */ unsigned char **Title);
        
        HRESULT ( STDMETHODCALLTYPE *get_Column )( 
            IWlNamedMult * This,
            /* [in] */ short Mult,
            /* [out] */ short *Column);
        
        HRESULT ( STDMETHODCALLTYPE *get_ColumnTitle )( 
            IWlNamedMult * This,
            /* [in] */ short Column,
            /* [out] */ unsigned char **Title);
        
        HRESULT ( STDMETHODCALLTYPE *IndexFromName )( 
            IWlNamedMult * This,
            /* [in] */ const unsigned char *Name,
            /* [out] */ short *Mult);
        
        HRESULT ( STDMETHODCALLTYPE *NameFromIndex )( 
            IWlNamedMult * This,
            /* [in] */ short Mult,
            /* [out] */ unsigned char **Name);
        
        END_INTERFACE
    } IWlNamedMultVtbl;

    interface IWlNamedMult
    {
        CONST_VTBL struct IWlNamedMultVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWlNamedMult_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWlNamedMult_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWlNamedMult_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWlNamedMult_put_FileName(This,FileName)	\
    ( (This)->lpVtbl -> put_FileName(This,FileName) ) 

#define IWlNamedMult_Init(This,Profile)	\
    ( (This)->lpVtbl -> Init(This,Profile) ) 

#define IWlNamedMult_get_ColumnCount(This,Count)	\
    ( (This)->lpVtbl -> get_ColumnCount(This,Count) ) 

#define IWlNamedMult_get_MultCount(This,Count)	\
    ( (This)->lpVtbl -> get_MultCount(This,Count) ) 

#define IWlNamedMult_get_MultTitle(This,Mult,Title)	\
    ( (This)->lpVtbl -> get_MultTitle(This,Mult,Title) ) 

#define IWlNamedMult_get_Column(This,Mult,Column)	\
    ( (This)->lpVtbl -> get_Column(This,Mult,Column) ) 

#define IWlNamedMult_get_ColumnTitle(This,Column,Title)	\
    ( (This)->lpVtbl -> get_ColumnTitle(This,Column,Title) ) 

#define IWlNamedMult_IndexFromName(This,Name,Mult)	\
    ( (This)->lpVtbl -> IndexFromName(This,Name,Mult) ) 

#define IWlNamedMult_NameFromIndex(This,Mult,Name)	\
    ( (This)->lpVtbl -> NameFromIndex(This,Mult,Name) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWlNamedMult_INTERFACE_DEFINED__ */



#ifndef __NMDMULLib_LIBRARY_DEFINED__
#define __NMDMULLib_LIBRARY_DEFINED__

/* library NMDMULLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_NMDMULLib;

EXTERN_C const CLSID CLSID_NmdMul;

#ifdef __cplusplus

class DECLSPEC_UUID("78CD1146-68F0-11D1-8419-00400534A64D")
NmdMul;
#endif
#endif /* __NMDMULLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


