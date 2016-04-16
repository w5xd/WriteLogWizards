#include <windows.h>
#include <windowsx.h>
#include <ole2.h>
#ifndef WIN32
#include <variant.h>
#else
#include "oleauto.h"
#endif

#include "port.h"
#include "mmdole.h"


//IPersistStorage Implementation
MmdPersistStorage::MmdPersistStorage(IUnknown FAR *p, CLSID c)
{
    UnkOuter    = p;
    ClassId = c;
    RefCount = 0;

}

MmdPersistStorage::~MmdPersistStorage()
{
}

STDMETHODIMP
MmdPersistStorage::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
    if (IsEqualIID(riid, IID_IPersistStorage))
    {
        *ppv = this;
    }
    else return UnkOuter->QueryInterface(riid, ppv);
    AddRef();
    return NOERROR;
}

STDMETHODIMP_(ULONG) MmdPersistStorage::AddRef(void)
{
    RefCount += 1;
    return UnkOuter->AddRef();
}

STDMETHODIMP_(ULONG) MmdPersistStorage::Release(void)
{
    RefCount -= 1;
    return UnkOuter->Release();
}

    // *** IPersist methods ***
STDMETHODIMP MmdPersistStorage::GetClassID(LPCLSID lpClassID)
{
    *lpClassID = ClassId;
    return NOERROR; 
}

    // *** IPersistStorage methods ***
STDMETHODIMP MmdPersistStorage::IsDirty (void)
{
    return ResultFromScode(S_OK);
}

STDMETHODIMP MmdPersistStorage::InitNew ( LPSTORAGE pStg)
{
    return NOERROR;
}

STDMETHODIMP MmdPersistStorage::Load ( LPSTORAGE pStg)
{

    return (NOERROR);
}

STDMETHODIMP MmdPersistStorage::Save ( LPSTORAGE pStgSave, BOOL fSameAsLoad)
{
    pStgSave->SetClass(ClassId);
    return (NOERROR);
}

STDMETHODIMP MmdPersistStorage::SaveCompleted ( LPSTORAGE pStgNew)
{
    return NOERROR;
}

STDMETHODIMP MmdPersistStorage::HandsOffStorage (void)
{
    return NOERROR;
}

HRESULT MmdPersistStorage::SaveCommon(
    LPSTORAGE p,
    const OLECHAR *StreamName,
    short *Config,
    int ConfigLength)
{
    IStream FAR *Stream;
    HRESULT hr;
    SCODE sc;
    char    ShortBuf[2];
    int i;

    hr = p->CreateStream(StreamName,
                STGM_DIRECT | STGM_READWRITE |
             STGM_CREATE | STGM_SHARE_EXCLUSIVE, 0, 0, &Stream);
    sc = GetScode(hr);
    if (Stream)
    {
        for (i = 0; i < ConfigLength; i += 1)
        {
            ShortBuf[0] = (char)Config[i];
            ShortBuf[1] = (char)(Config[i] >> 8);
            hr = Stream->Write(ShortBuf, 2, NULL);
        }
        hr = UserWrite(Stream);
        Stream->Release();
    }
    p->SetClass(ClassId);
    p->Commit(STGC_DEFAULT);
    return (hr);
}

HRESULT MmdPersistStorage::UserWrite(IStream FAR *s)
{
    return NOERROR;
}

HRESULT MmdPersistStorage::UserRead(IStream FAR *s)
{
    return NOERROR;
}


HRESULT  MmdPersistStorage::LoadCommon(LPSTORAGE p,
    const OLECHAR *StreamName,
    short *Config,
    int ConfigLength)
{
    IStream FAR *Stream;
    HRESULT hr;
    SCODE sc;
    int i;
    unsigned char ShortBuf[2];
    ULONG ByteCount;

        hr = p->OpenStream(StreamName, 0,
                STGM_DIRECT | STGM_READ | STGM_SHARE_EXCLUSIVE,
                0, &Stream);
        sc = GetScode(hr);
        if (Stream)
        {
            for (i = 0; i < ConfigLength; i += 1)
            {
                hr = Stream->Read(ShortBuf, 2, &ByteCount);
                if ((ByteCount != 2) || FAILED(hr))
                    break;
                Config[i] = (ShortBuf[0] & 0xFF) | (0xFF00 & (ShortBuf[1] << 8));
            }
            hr = UserRead(Stream);
            Stream->Release();
        }

    return hr;
}
