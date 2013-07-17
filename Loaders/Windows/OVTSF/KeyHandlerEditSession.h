//
//
// Derived from Microsoft Sample IME by Jeremy '13,7,17
//
//


#pragma once

#include "EditSession.h"
#include "Globals.h"

class CKeyHandlerEditSession : public CEditSessionBase
{
public:
    CKeyHandlerEditSession(COVTSF *pTextService, ITfContext *pContext, UINT uCode, WCHAR wch, _KEYSTROKE_STATE keyState) : CEditSessionBase(pTextService, pContext)
    {
        _uCode = uCode;
        _wch = wch;
        _KeyState = keyState;
    }

    // ITfEditSession
    STDMETHODIMP DoEditSession(TfEditCookie ec);

private:
    UINT _uCode;    // virtual key code
    WCHAR _wch;      // character code
    _KEYSTROKE_STATE _KeyState;     // key function regarding virtual key
};
