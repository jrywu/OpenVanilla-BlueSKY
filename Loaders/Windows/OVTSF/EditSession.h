//
//
// Derived from Microsoft Sample IME by Jeremy '13,7,17
//
//



#pragma once

class COVTSF;

class CEditSessionBase : public ITfEditSession
{
public:
    CEditSessionBase(_In_ COVTSF *pTextService, _In_ ITfContext *pContext);
    virtual ~CEditSessionBase();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, _Outptr_ void **ppvObj);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    // ITfEditSession
    virtual STDMETHODIMP DoEditSession(TfEditCookie ec) = 0;

protected:
    ITfContext *_pContext;
    COVTSF *_pTextService;

private:
    LONG _refCount;     // COM ref count
};
