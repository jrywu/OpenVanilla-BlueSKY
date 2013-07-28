#ifndef OVIME_h
#define OVIME_h

#include <windows.h>
#include <tchar.h>

#ifdef WINCE
#include <imm.h>
#else
#include <immdev.h>
#endif

#include "ExtraStructs.h"
#include "Vk.h"

#include "OVIMEUI.h"

#include "AVDisplayServer.h"
#include "DisplayServer.h"
#include "AVLoader.h"
#include "ImmModel.h"
#include "ImmController.h"

// Extern
extern HINSTANCE hInst;
extern int CompX;
extern int CompY;
extern int CandX;
extern int CandY;
extern bool hasCompStarted;
extern bool isCompEnabled;

extern bool isChinese;
extern bool isFullShape;

extern BOOL isWindows8();

extern DisplayServer *dsvr;

// Constant
#define CS_OVIME (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS)
#define UICLASSNAME _T("OVIMEUI")
#ifdef WINCE
//#define UICOMPCLASSNAME _T("OVIMECOMP")
#define UICANDCLASSNAME _T("OVIMECAND")
//#define UISTATUSCLASSNAME _T("OVIMESTATUS")
#endif
	
#define UICHILDEXTRASIZE 12
#define FIGWL_MOUSE 4
#define FIGWL_SVRWND 8
#define FIM_CAPUTURED 0x01
#define FIM_MOVED 0x02

#define WM_UI_COMPMOVE (WM_APP+600)
#define WM_IME_RELOADCONFIG (WM_APP+601)

// Macro
#define GETLPCOMPSTR(lpcs) (wchar_t*)((LPBYTE)(lpcs) + (lpcs)->dwCompStrOffset)
#define GETLPCOMPATTR(lpcs) (BYTE*)((LPBYTE)(lpcs) + (lpcs)->dwCompAttrOffset)
#define GETLPCOMPCLS(lpcs) (DWORD*)((LPBYTE)(lpcs) + (lpcs)->dwCompClsOffset)

#define GETLPRESULTSTR(lpcs) (wchar_t*)((LPBYTE)(lpcs) + (lpcs)->dwResultStrOffset)
#define GETLPRESULTCLS(lpcs) (DWORD*)((LPBYTE)(lpcs) + (lpcs)->dwResultClsOffset)

#define GETLPREADSTR(lpcs) (wchar_t*)((LPBYTE)(lpcs) + (lpcs)->dwCompReadStrOffset)
#define GETLPREADATTR(lpcs) (BYTE*)((LPBYTE)(lpcs) + (lpcs)->dwCompReadAttrOffset)
#define GETLPREADCLS(lpcs) (DWORD*)((LPBYTE)(lpcs) + (lpcs)->dwCompReadClsOffset)

#define GETLPRESULTREADSTR(lpcs) (wchar_t*)((LPBYTE)(lpcs) + (lpcs)->dwResultReadStrOffset)
#define GETLPRESULTREADCLS(lpcs) (DWORD*)((LPBYTE)(lpcs) + (lpcs)->dwResultClsOffset)

#define GETLPCANDSTR(lpcs,i) (wchar_t*)((LPBYTE)(lpcs) + (lpcs)->dwOffset[i])

// Functions
extern "C" {
	// OVIME.cpp
	BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID);
	// UIWnd.cpp
	LRESULT APIENTRY UIWndProc(HWND, UINT, WPARAM, LPARAM);
	void RefreshUI(HIMC hUICurIMC);
	// ControlHandle.cpp
	LRESULT ControlHandle(HIMC, HWND, UINT, WPARAM, LPARAM);
	// NotifyHandel.cpp
	LRESULT NotifyHandle(HIMC, HWND, UINT, WPARAM, LPARAM);
	// IME.cpp
	void MyGenerateMessage(HIMC, UINT, WPARAM, LPARAM);
	BOOL MyGenerateMessageToTransKey(LPDWORD, UINT*, UINT, WPARAM, LPARAM);
	// Utils.cpp
	void MakeCompStr(LPMYPRIVATE, LPCOMPOSITIONSTRING, int Carex=-1 , int markFrom=-1, int markTo=-1);
	void InitCompStr(LPCOMPOSITIONSTRING);
	void InitCandInfo(LPCANDIDATEINFO);
	void UpdateCandidate(LPINPUTCONTEXT, const wchar_t*, DWORD dwPageSize=5);
	void ClearCandidate(LPCANDIDATEINFO);

	// Reloadcallback.cpp
	BOOL CALLBACK ReloadIME(HWND hwnd, LPARAM lp);

}
#endif // OVIME_h
