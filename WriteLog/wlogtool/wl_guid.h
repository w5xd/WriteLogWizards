#pragma once
#ifdef INITGUID
#define DEFINE_WL_GUID(iface, guid) \
    interface __declspec( uuid(guid) ) iface; \
    EXTERN_C const GUID IID_##iface = (__uuidof(iface))
#else
#define DEFINE_WL_GUID(iface, guid) \
    interface __declspec( uuid(guid) ) iface; \
    EXTERN_C const GUID  IID_##iface
#endif // INITGUID
