//////////////////////////////////////////////////////////////////////
//
// Derived from Microsoft TSF sample by Jeremy '12,6,25
//
//  DllMain.cpp
//
//          DllMain module entry point.
//
//////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "CandidateWindow.h"

//+---------------------------------------------------------------------------
//
// DllMain
//
//----------------------------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID pvReserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:

            g_hInst = hInstance;

            if (!InitializeCriticalSectionAndSpinCount(&g_cs, 0))
                return FALSE;

          // register candidate window class.
            CCandidateWindow::_InitWindowClass();

            break;

        case DLL_PROCESS_DETACH:

          // unregister candidate window class.
            CCandidateWindow::_UninitWindowClass();

            DeleteCriticalSection(&g_cs);

            break;
    }

    return TRUE;
}
