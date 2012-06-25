//////////////////////////////////////////////////////////////////////
//
// Derived from Microsoft TSF sample by Jeremy '12,6,25
//
//  DisplayAttribureProvider.cpp
//
//          ITfDisplayAttributeProvider implementation.
//
//////////////////////////////////////////////////////////////////////

#include "globals.h"
#include "TextService.h"
#include "DisplayAttributeInfo.h"
#include "EnumDisplayAttributeInfo.h"

//+---------------------------------------------------------------------------
//
// EnumDisplayAttributeInfo
//
//----------------------------------------------------------------------------

STDAPI CTextService::EnumDisplayAttributeInfo(IEnumTfDisplayAttributeInfo **ppEnum)
{
    CEnumDisplayAttributeInfo *pAttributeEnum;

    if (ppEnum == NULL)
        return E_INVALIDARG;

    *ppEnum = NULL;

    if ((pAttributeEnum = new CEnumDisplayAttributeInfo) == NULL)
        return E_OUTOFMEMORY;

    *ppEnum = pAttributeEnum;

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// GetDisplayAttributeInfo
//
//----------------------------------------------------------------------------

STDAPI CTextService::GetDisplayAttributeInfo(REFGUID guidInfo, ITfDisplayAttributeInfo **ppInfo)
{
    if (ppInfo == NULL)
        return E_INVALIDARG;

    *ppInfo = NULL;

  // Which display attribute GUID?
    if (IsEqualGUID(guidInfo, c_guidDisplayAttributeInput))
    {
        if ((*ppInfo = new CDisplayAttributeInfoInput()) == NULL)
            return E_OUTOFMEMORY;
    }
    else if (IsEqualGUID(guidInfo, c_guidDisplayAttributeConverted))
    {
        if ((*ppInfo = new CDisplayAttributeInfoConverted()) == NULL)
            return E_OUTOFMEMORY;
    }
    else
    {
        return E_INVALIDARG;
    }


    return S_OK;
}
