//#include <vector>  
#include "OVIMEUI.h"

//wchar_t* lpCandStr;
//int numCand;
wchar_t* lpCompStr;
wchar_t* lpNotifyStr;
int CompCursorPos;
int CompSelStart;
int CompSelEnd;
UICHILD uiStatus;
UICHILD uiComp;
UICHILD uiCand;
UICHILD uiNotify;

HWND hIMEWnd;

bool isChinese = true;
bool isFullShape = false;

// InputContext
int CurrentIC=0;
