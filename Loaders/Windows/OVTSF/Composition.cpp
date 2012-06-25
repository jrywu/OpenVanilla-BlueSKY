//////////////////////////////////////////////////////////////////////
//
// Derived from Microsoft TSF sample by Jeremy '12,6,25
//
//  Composition.cpp
//
//          ITfCompositionSink interface and compositon utilities
//
//////////////////////////////////////////////////////////////////////
#define OV_DEBUG
#include "Globals.h"
#include "TextService.h"
#include "OVUtility.h"
//+---------------------------------------------------------------------------
//
// OnCompositionTerminated
//
// Callback for ITfCompositionSink.  The system calls this method whenever
// someone other than this service ends a composition.
//----------------------------------------------------------------------------

STDAPI CTextService::OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition *pComposition)
{
	murmur("CTextService::OnCompositionTerminated()");

  // releae our cached composition
    if (_pComposition != NULL)
    {
        _pComposition->Release();
        _pComposition = NULL;
    }

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _IsComposing
//
//----------------------------------------------------------------------------

BOOL CTextService::_IsComposing()
{
	murmur("CTextService::_IsComposing(), %d", _pComposition  != NULL);
    return _pComposition != NULL;
}

//+---------------------------------------------------------------------------
//
// _SetComposition
//
//----------------------------------------------------------------------------

void CTextService::_SetComposition(ITfComposition *pComposition)
{
	murmur("CTextService::_SetComposition()");
    _pComposition = pComposition;
}

