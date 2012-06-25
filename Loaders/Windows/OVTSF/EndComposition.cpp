//////////////////////////////////////////////////////////////////////
//
// Derived from Microsoft TSF sample by Jeremy '12,6,25
//
//  EndComposition.cpp
//
//          terminate the compositon object
//
//////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "EditSession.h"
#include "TextService.h"

//+---------------------------------------------------------------------------
//
// CEndCompositionEditSession
//
//----------------------------------------------------------------------------

class CEndCompositionEditSession : public CEditSessionBase
{
public:
    CEndCompositionEditSession(CTextService *pTextService, ITfContext *pContext) : CEditSessionBase(pTextService, pContext)
    {
    }

  // ITfEditSession
    STDMETHODIMP DoEditSession(TfEditCookie ec)
    {
        _pTextService->_TerminateComposition(ec, _pContext);
        return S_OK;
    }

};

//+---------------------------------------------------------------------------
//
// _TerminateComposition
//
//----------------------------------------------------------------------------

void CTextService::_TerminateComposition(TfEditCookie ec, ITfContext *pContext)
{
    if (_pComposition != NULL)
    {
      //
      // remove the display attribute from the composition range.
      //
        _ClearCompositionDisplayAttributes(ec, pContext);

        _pComposition->EndComposition(ec);
        _pComposition->Release();
        _pComposition = NULL;
    }
}

//+---------------------------------------------------------------------------
//
// _EndComposition
//
//----------------------------------------------------------------------------

void CTextService::_EndComposition(ITfContext *pContext)
{
    CEndCompositionEditSession *pEditSession;
    HRESULT hr;

    if (pEditSession = new CEndCompositionEditSession(this, pContext))
    {
        pContext->RequestEditSession(_tfClientId, pEditSession, TF_ES_ASYNCDONTCARE | TF_ES_READWRITE, &hr);
        pEditSession->Release();
    }
}

