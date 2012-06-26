//////////////////////////////////////////////////////////////////////
//
// Derived from Microsoft TSF sample by Jeremy '12,6,25
//
//  KeyEventSink.cpp
//
//          ITfKeyEventSink implementation.
//
//////////////////////////////////////////////////////////////////////
#define OV_DEBUG
#include "Globals.h"
#include "TextService.h"
#include "CandidateList.h"
#include "OVUtility.h"
//
// GUID for the preserved keys.
//

// {30FA9233-976E-464E-9223-1815F4D4497B}
static const GUID GUID_PRESERVEDKEY_CTRL_SPACE = 
{ 0x30fa9233, 0x976e, 0x464e, { 0x92, 0x23, 0x18, 0x15, 0xf4, 0xd4, 0x49, 0x7b } };

// {F680818B-9574-4509-B40F-E7A3CB866CB1}
static const GUID GUID_PRESERVEDKEY_SHIFT_SPACE = 
{ 0xf680818b, 0x9574, 0x4509, { 0xb4, 0xf, 0xe7, 0xa3, 0xcb, 0x86, 0x6c, 0xb1 } };


// {4C88A00B-BE4A-4C43-92CB-CD225898E9E0}
static const GUID GUID_PRESERVEDKEY_CTRL_BACKSLASH = 
{ 0x4c88a00b, 0xbe4a, 0x4c43, { 0x92, 0xcb, 0xcd, 0x22, 0x58, 0x98, 0xe9, 0xe0 } };


// {BB456BD8-78E8-4DE4-9DCF-A19EF8CE4F2F}
static const GUID GUID_PRESERVEDKEY_SHIFT = 
{ 0xbb456bd8, 0x78e8, 0x4de4, { 0x9d, 0xcf, 0xa1, 0x9e, 0xf8, 0xce, 0x4f, 0x2f } };



//
// the preserved keys declaration
//

static const TF_PRESERVEDKEY c_pkeyCtrlSpace = { VK_SPACE, TF_MOD_CONTROL };
static const TF_PRESERVEDKEY c_pkeyShiftSpace = { VK_SPACE, TF_MOD_SHIFT };
static const TF_PRESERVEDKEY c_pkeyCtrlBackSlash = { VK_OEM_5, TF_MOD_CONTROL };
static const TF_PRESERVEDKEY c_pkeySHIFT =   { VK_SHIFT, TF_MOD_SHIFT };

//
// the description for the preserved keys
//
static const WCHAR c_szPKeyCtrlSpace[] = L"Ctrl+Space";
static const WCHAR c_szPKeyShiftSpace[] = L"Shift+Space";
static const WCHAR c_szPKeyCtrlBackSlash[] = L"Ctrl+\\";
static const WCHAR c_szPKeyShift[]    = L"Shift";

//+---------------------------------------------------------------------------
//
// _IsKeyEaten
//
//----------------------------------------------------------------------------

BOOL CTextService::_IsKeyEaten(ITfContext *pContext, WPARAM wParam)
{
	murmur("KeyEventSink:CTextService::_IsKeyEaten() for key = %d", wParam);
  // if the keyboard is disabled, keys are not consumed.
    if (_IsKeyboardDisabled())
        return FALSE;

  // if the keyboard is closed, keys are not consumed.
    if (!_IsKeyboardOpen())
        return FALSE;

  //
  // The text service key handler does not do anything while the candidate
  // window is shown.
  // The candidate list handles the keys through ITfContextKeyEventSink.
  //
    if (_pCandidateList &&
        _pCandidateList->_IsContextCandidateWindow(pContext))
    {
        return FALSE;
    }

  // eat only keys that CKeyHandlerEditSession can hadles.
    switch (wParam)
    {
        case VK_LEFT:
        case VK_RIGHT:
        case VK_RETURN:
        case VK_SPACE:
            if (_IsComposing())
                return TRUE;
            return FALSE;
    }

    if (wParam >= 'A' && wParam <= 'Z')
        return TRUE;

    return FALSE;
}


//+---------------------------------------------------------------------------
//
// OnSetFocus
//
// Called by the system whenever this service gets the keystroke device focus.
//----------------------------------------------------------------------------

STDAPI CTextService::OnSetFocus(BOOL fForeground)
{
	murmur("KeyEventSink:CTextService::OnSetFocus()");
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnTestKeyDown
//
// Called by the system to query this service wants a potential keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
	murmur("KeyEventSink:CTextService::OnTestKeyDown() for key = %d", wParam);
    *pfEaten = _IsKeyEaten(pContext, wParam);
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnKeyDown
//
// Called by the system to offer this service a keystroke.  If *pfEaten == TRUE
// on exit, the application will not handle the keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
	murmur("KeyEventSink:CTextService::OnKeyDown() for key = %d", wParam);
    *pfEaten = _IsKeyEaten(pContext, wParam);

    if (*pfEaten)
    {
        _InvokeKeyHandler(pContext, wParam, lParam);
    }
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnTestKeyUp
//
// Called by the system to query this service wants a potential keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
	murmur("KeyEventSink:CTextService::OnTestKeyUp() for key = %d", wParam);
    *pfEaten = _IsKeyEaten(pContext, wParam);
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnKeyUp
//
// Called by the system to offer this service a keystroke.  If *pfEaten == TRUE
// on exit, the application will not handle the keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
	murmur("KeyEventSink:CTextService::OnKeyUp() for key = %d", wParam);
    *pfEaten = _IsKeyEaten(pContext, wParam);
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnPreservedKey
//
// Called when a hotkey (registered by us, or by the system) is typed.
//----------------------------------------------------------------------------

STDAPI CTextService::OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pfEaten)
{
	murmur("KeyEventSink:CTextService::OnPreservedKey()");
    if (IsEqualGUID(rguid, GUID_PRESERVEDKEY_CTRL_SPACE))
    {
		murmur("OnPreservedKey(), GUID_PRESERVEDKEY_ONOFF");
        BOOL fOpen = _IsKeyboardOpen();
        _SetKeyboardOpen(fOpen ? FALSE : TRUE);
        *pfEaten = TRUE;
    }
	else if (IsEqualGUID(rguid, GUID_PRESERVEDKEY_SHIFT_SPACE))
	{
		murmur("OnPreservedKey(), GUID_PRESERVEDKEY_SHIFT_SPACE");
	}
	else if (IsEqualGUID(rguid, GUID_PRESERVEDKEY_CTRL_BACKSLASH))
	{
		murmur("OnPreservedKey(), GUID_PRESERVEDKEY_CTRL_BACKSLASH");
	}
    else if (IsEqualGUID(rguid, GUID_PRESERVEDKEY_SHIFT))
	{
		murmur("OnPreservedKey(), GUID_PRESERVEDKEY_SHIFT");
		BOOL fOpen = _IsKeyboardOpen();
        _SetKeyboardOpen(fOpen ? FALSE : TRUE);
        *pfEaten = TRUE;
	}
	else
    {
        *pfEaten = FALSE;
    }

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _InitKeyEventSink
//
// Advise a keystroke sink.
//----------------------------------------------------------------------------

BOOL CTextService::_InitKeyEventSink()
{
	murmur("KeyEventSink:CTextService::_InitKeyEventSink()");
    ITfKeystrokeMgr *pKeystrokeMgr;
    HRESULT hr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return FALSE;

    hr = pKeystrokeMgr->AdviseKeyEventSink(_tfClientId, (ITfKeyEventSink *)this, TRUE);

    pKeystrokeMgr->Release();

    return (hr == S_OK);
}

//+---------------------------------------------------------------------------
//
// _UninitKeyEventSink
//
// Unadvise a keystroke sink.  Assumes a sink has been advised already.
//----------------------------------------------------------------------------

void CTextService::_UninitKeyEventSink()
{
	murmur("KeyEventSink:CTextService::_UninitKeyEventSink()");
    ITfKeystrokeMgr *pKeystrokeMgr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return;

    pKeystrokeMgr->UnadviseKeyEventSink(_tfClientId);

    pKeystrokeMgr->Release();
}

//+---------------------------------------------------------------------------
//
// _InitPreservedKey
//
// Register a hot key.
//----------------------------------------------------------------------------

BOOL CTextService::_InitPreservedKey()
{
	murmur("KeyEventSink:CTextService::_InitPreservedKey()");
    ITfKeystrokeMgr *pKeystrokeMgr;
    HRESULT hr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return FALSE;

  // register Ctrl+space key
    hr = pKeystrokeMgr->PreserveKey(_tfClientId, 
                                    GUID_PRESERVEDKEY_CTRL_SPACE,
                                    &c_pkeyCtrlSpace,
                                    c_szPKeyCtrlSpace,
                                    wcslen(c_szPKeyCtrlSpace));
 // register Shift+space key
    hr = pKeystrokeMgr->PreserveKey(_tfClientId, 
                                    GUID_PRESERVEDKEY_SHIFT_SPACE,
                                    &c_pkeyShiftSpace,
                                    c_szPKeyShiftSpace,
                                    wcslen(c_szPKeyShiftSpace));

  // register ctrl+\ key
    hr = pKeystrokeMgr->PreserveKey(_tfClientId, 
                                    GUID_PRESERVEDKEY_CTRL_BACKSLASH,
                                    &c_pkeyCtrlBackSlash,
                                    c_szPKeyCtrlBackSlash,
                                    wcslen(c_szPKeyCtrlBackSlash));

  // register Shift key
    hr = pKeystrokeMgr->PreserveKey(_tfClientId, 
                                    GUID_PRESERVEDKEY_SHIFT,
                                    &c_pkeySHIFT,
                                    c_szPKeyShift,
                                    wcslen(c_szPKeyShift));

    pKeystrokeMgr->Release();

    return (hr == S_OK);
}

//+---------------------------------------------------------------------------
//
// _UninitPreservedKey
//
// Uninit a hot key.
//----------------------------------------------------------------------------

void CTextService::_UninitPreservedKey()
{
	murmur("KeyEventSink:CTextService::_UninitPreservedKey()");
    ITfKeystrokeMgr *pKeystrokeMgr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return;

    pKeystrokeMgr->UnpreserveKey(GUID_PRESERVEDKEY_CTRL_SPACE, &c_pkeyCtrlSpace);
	pKeystrokeMgr->UnpreserveKey(GUID_PRESERVEDKEY_SHIFT_SPACE, &c_pkeyShiftSpace);
    pKeystrokeMgr->UnpreserveKey(GUID_PRESERVEDKEY_CTRL_BACKSLASH, &c_pkeyCtrlBackSlash);
    pKeystrokeMgr->UnpreserveKey(GUID_PRESERVEDKEY_SHIFT, &c_pkeySHIFT);

    pKeystrokeMgr->Release();
}

