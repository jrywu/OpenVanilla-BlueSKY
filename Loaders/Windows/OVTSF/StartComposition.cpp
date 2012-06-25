//////////////////////////////////////////////////////////////////////
//
// Derived from Microsoft TSF sample by Jeremy '12,6,25
//
//  StartComposition.cpp
//
//          the rountins to start a new composition object.
//
//////////////////////////////////////////////////////////////////////
#define OV_DEBUG
#include "Globals.h"
#include "EditSession.h"
#include "TextService.h"
#include "OVUtility.h"
//+---------------------------------------------------------------------------
//
// CStartCompositinoEditSession
//
//----------------------------------------------------------------------------

class CStartCompositionEditSession : public CEditSessionBase
{
public:
    CStartCompositionEditSession(CTextService *pTextService, ITfContext *pContext) : CEditSessionBase(pTextService, pContext)
    {
    }

  // ITfEditSession
    STDMETHODIMP DoEditSession(TfEditCookie ec);
};

//+---------------------------------------------------------------------------
//
// DoEditSession
//
//----------------------------------------------------------------------------

STDAPI CStartCompositionEditSession::DoEditSession(TfEditCookie ec)
{
	murmur("StartComposition:CStartCompositionEditSession::DoEditSession()");
    ITfInsertAtSelection *pInsertAtSelection = NULL;
    ITfRange *pRangeInsert = NULL;
    ITfContextComposition *pContextComposition = NULL;
    ITfComposition *pComposition = NULL;
    HRESULT hr = E_FAIL;

  // A special interface is required to insert text at the selection
    if (_pContext->QueryInterface(IID_ITfInsertAtSelection, (void **)&pInsertAtSelection) != S_OK)
    {
        goto Exit;
    }

  // insert the text
    if (pInsertAtSelection->InsertTextAtSelection(ec, TF_IAS_QUERYONLY, NULL, 0, &pRangeInsert) != S_OK)
    {
        goto Exit;
    }

  // get an interface on the context to deal with compositions
    if (_pContext->QueryInterface(IID_ITfContextComposition, (void **)&pContextComposition) != S_OK)
    {
        goto Exit;
    }

  // start the new composition
    if ((pContextComposition->StartComposition(ec, pRangeInsert, _pTextService, &pComposition) == S_OK) && (pComposition != NULL))
    {
      // Store the pointer of this new composition object in the instance 
      // of the CTextService class. So this instance of the CTextService 
      // class can know now it is in the composition stage.
        _pTextService->_SetComposition(pComposition);

      // 
      //  set selection to the adjusted range
      // 
        TF_SELECTION tfSelection;
        tfSelection.range = pRangeInsert;
        tfSelection.style.ase = TF_AE_NONE;
        tfSelection.style.fInterimChar = FALSE;
        _pContext->SetSelection(ec, 1, &tfSelection);
    }

Exit:
    if (pContextComposition != NULL)
        pContextComposition->Release();

    if (pRangeInsert != NULL)
        pRangeInsert->Release();

    if (pInsertAtSelection != NULL)
        pInsertAtSelection->Release();

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _StartComposition
//
// this starts the new composition at the selection of the current 
// focus context.
//----------------------------------------------------------------------------

void CTextService::_StartComposition(ITfContext *pContext)
{
	murmur("StartComposition:CTextService::_StartComposition()");
    CStartCompositionEditSession *pStartCompositionEditSession;

    if (pStartCompositionEditSession = new CStartCompositionEditSession(this, pContext))
    {
        HRESULT hr;
      // A synchronus document write lock is requred.
      // the CStartCompositionEditSession will do all the work when the
      // CStartCompositionEditSession::DoEditSession method is called by the context
        pContext->RequestEditSession(_tfClientId, pStartCompositionEditSession, TF_ES_SYNC | TF_ES_READWRITE, &hr);

        pStartCompositionEditSession->Release();
    }
}
