//////////////////////////////////////////////////////////////////////
//
// Derived from Microsoft TSF sample by Jeremy '12,6,25
//
//  EnumDisplayAttribureInfo.h
//
//          ITfEnumDisplayAttributeInfo implementation.
//
//////////////////////////////////////////////////////////////////////

#ifndef ENUMDISPLAYATTRIBUTEINFO_H
#define ENUMDISPLAYATTRIBUTEINFO_H

//+---------------------------------------------------------------------------
//
// CEnumDisplayAttributeInfo
//
//----------------------------------------------------------------------------

class CEnumDisplayAttributeInfo : public IEnumTfDisplayAttributeInfo
{
public:
    CEnumDisplayAttributeInfo();
    ~CEnumDisplayAttributeInfo();

  // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void **ppvObj);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

  // IEnumTfDisplayAttributeInfo
    STDMETHODIMP Clone(IEnumTfDisplayAttributeInfo **ppEnum);
    STDMETHODIMP Next(ULONG ulCount, ITfDisplayAttributeInfo **rgInfo, ULONG *pcFetched);
    STDMETHODIMP Reset();
    STDMETHODIMP Skip(ULONG ulCount);

private:
    LONG _iIndex; // next display attribute to enum
    LONG _cRef; // COM ref count
};

#endif ENUMDISPLAYATTRIBUTEINFO_H
