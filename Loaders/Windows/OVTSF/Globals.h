//////////////////////////////////////////////////////////////////////
//
// Derived from Microsoft TSF sample by Jeremy '12,6,25
//
//  Globals.h
//
//          Global variable declarations.
//
//////////////////////////////////////////////////////////////////////

#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>
#include <ole2.h>
#include <olectl.h>
#include <assert.h>
#include "msctf.h"

void DllAddRef();
void DllRelease();

#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))

#define TEXTSERVICE_LANGID	MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)

#define TEXTSERVICE_DESC    L"Open Vanilla 0.9b"
#define TEXTSERVICE_DESC_A   "Open Vanilla 0.9b"
#define TEXTSERVICE_MODEL   TEXT("Apartment")

#define TEXTSERVICE_ICON_INDEX  0
#define LANGBAR_ITEM_DESC   L"Current Input Method"

//+---------------------------------------------------------------------------
//
// SafeStringCopy
//
// Copies a string from one buffer to another.  wcsncpy does not always
// null-terminate the destination buffer; this function does.
//----------------------------------------------------------------------------

inline void SafeStringCopy(WCHAR *pchDst, ULONG cchMax, const WCHAR *pchSrc)
{
    if (cchMax > 0)
    {
        wcsncpy(pchDst, pchSrc, cchMax);
        pchDst[cchMax-1] = '\0';
    }
}

extern HINSTANCE g_hInst;

extern LONG g_cRefDll;

extern CRITICAL_SECTION g_cs;

extern const CLSID c_clsidTextService;

extern const GUID c_guidProfile;

extern const GUID c_guidLangBarItemButton;

extern const GUID c_guidDisplayAttributeInput;
extern const GUID c_guidDisplayAttributeConverted;

#endif // GLOBALS_H
