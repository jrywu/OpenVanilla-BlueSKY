//#define OV_DEBUG 

#pragma unmanaged

#include <stdio.h> 
#include "OVIMEUI.h"
#include "OVUIServer.h"
#include <shobjidl.h>
#include <shlobj.h>
#include <atlbase.h>

// Status window 
void UISetkpCount(int count)
{
	_setStatuskpCount(count);
}

void UISetofCount(int count)
{
	_setStatusofCount(count);
}
void UISetimCount(int count)
{

	_setStatusimCount(count);
}


void UICreateStatusWindow(HWND hUIWnd, int xPos, int yPos, bool isDocked)
{	
	
	
	if (!IsWindow(uiStatus.hWnd)) {
		murmur("\tUICreateStatusWindow, hUIWnd=%x @(%i, %i), isDocked:%d",hUIWnd, xPos, yPos, isDocked);
		RECT rec;

		uiStatus.pt.x = (xPos>0)?xPos : 0; 
		uiStatus.pt.y = (yPos>0)?yPos : 0;

		uiStatus.hWnd = _CreateStatusPage();

		murmur("\tStatusWindow created. hWnd=%x",uiStatus.hWnd);
	
		SendMessage(hUIWnd, WM_IME_NOTIFY, IMN_PRIVATE, 9);  // set all module names.
		hIMEWnd = hUIWnd; 
		
		if((xPos==0)||(yPos==0)) 
		{
			murmur("\tMove the status window to default location.");
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rec, 0);
			UIMoveStatusWindow(hUIWnd, rec.right - 200, rec.bottom - 50);
		}
		else
		{
			murmur("\tMove the status window to saved location, isDocked:%d.", UIGetIsStatusDocked());
			UIMoveStatusWindow(hUIWnd, xPos, yPos);
		}
		
		
	}
	else
		murmur("Status window is already created.");
	
		_SetStatusAppHWnd(hUIWnd);
}

const wchar_t* convertUtf8NameToUcs2(const char* name)
{
	wchar_t *modCurrentName;
	wchar_t modNameUCS2[1024];
	MultiByteToWideChar(
		CP_UTF8, 0, name, (int)strlen(name)+1, modNameUCS2, 1024);
	modCurrentName = modNameUCS2;
	return wcsdup(modCurrentName);
}

void UISetStatusModStrMenuEach(const char* newName)
{
	_SetStatusMenuModString(convertUtf8NameToUcs2(newName));
}

//set Status window module list
void UISetStatusModStrMenuAll(int modAmount, const char* modNames[])
{
	UIClearStatusMenuModString();	
	for(int i = 0; i < modAmount; i++){
		murmur("\t set module %i name:%s",i , modNames[i]);
		UISetStatusModStrMenuEach(modNames[i]);  
	}
}
bool UIGetModuleChecked(int index)
{
	return _GetItemChecked(index);
}

void UISetModuleChecked(int index, bool b)
{
	_SetItemChecked(index,b);
}

void UISetModuleEnabled(int index, bool b)
{
	_SetItemEnabled(index,b);
}

void UISetStatusModStrCurrent(int index)
{
	_SetStatusModString(index);
}

void UIClearStatusMenuModString()
{
	_ClearStatusMenuModString();
}

void UIDockStatusWindow(int X, int Y)
{   
	if (IsWindow(uiStatus.hWnd))
	{
		_DockStatusPage(X, Y);
	}
	
}




void UIMoveStatusWindow(HWND hUIWnd, int X, int Y)
{   
	
	/*if (!IsWindow(uiStatus.hWnd))
		UICreateStatusWindow(hUIWnd);*/

	if (IsWindow(uiStatus.hWnd))
	{
		POINT pt;
		RECT screenrc;
		SIZE sz;

		sz.cx = 0;
		sz.cy = 0;
		uiStatus.pt.x = (X>0)?X : 0; 
		uiStatus.pt.y = (Y>0)?Y : 0;
		sz.cx = uiStatus.sz.cx;
		sz.cy = uiStatus.sz.cy;
		pt.x = uiStatus.pt.x;
		pt.y = uiStatus.pt.y;
		
		MONITORINFO mi;
		mi.cbSize=sizeof(MONITORINFO); 
		
		GetMonitorInfo(MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST), &mi);
		if( (pt.x + sz.cx) > mi.rcMonitor.right )
			pt.x = mi.rcMonitor.right - 200;
			if( (pt.y + sz.cy) > mi.rcMonitor.bottom )
			pt.y = mi.rcMonitor.bottom - 50;		

/*		SystemParametersInfo(SPI_GETWORKAREA, 0, &screenrc, 0);

		if( (pt.x + sz.cx) > screenrc.right )
			pt.x = screenrc.right - 200;
		if( (pt.y + sz.cy) > screenrc.bottom )
			pt.y = screenrc.bottom - 50;		
*/
		_MoveStatusPage(pt.x, pt.y);
	}
	
}
//static bool m_docked=true;
void UIShowStatusWindow()
{
	
	if (IsWindow(uiStatus.hWnd))
	{	
		murmur("\tShow status window. ;");
		
		_ShowStatusPage(); //show		 
	}
} 


void UIHideStatusWindow()
{
	if (IsWindow(uiStatus.hWnd))
	{
		murmur("\tHide status window...");

		_HideStatusPage();
	}
	
}

bool UIGetIsStatusDocked()
{
	return _GetIsStatusDocked();
}


void UISetIsStatusDocked(bool t)
{
	_SetIsStatusDocked(t);
	if(!t)
		UIMoveStatusWindow(NULL, uiStatus.pt.x, uiStatus.pt.y);
}



int UIGetStatusPos(LPPOINT lp_pt)
{
	if (IsWindow(uiStatus.hWnd)) {
	LPRECT lp_rect;
	lp_rect = new RECT;
	GetWindowRect(uiStatus.hWnd,lp_rect);
	lp_pt->x = lp_rect->left;
	lp_pt->y = lp_rect->top;
	murmur("status window at (%i, %i).", lp_pt->x, lp_pt->y);
	return 1;
	} else return -1;
	
	
}

int CompIndexToXPos(int index);


void UISetCursorPos(int i)
{
	CompCursorPos = i;
}

#if 0
void UISetMarkFrom(int i)
{
	CompSelStart = i;
	murmur( "---> UISetMarkFrom Start %d", CompSelStart);


void UISetMarkTo(int i)
{
	CompSelEnd = i;
	murmur( "---> UISetMarkFrom End %d", CompSelEnd);
}
#endif

int UIModuleChange()
{
	CompCursorPos=0; 
	CurrentIC = _GetStatusSelectedModuleIndex();
	//UISetStatusModStrCurrent(CurrentIC);
	_ShowStatusPage();

	return CurrentIC;
}


int UIModuleRotate()
{
	_RotateStatusSelectedModule();
	return UIModuleChange();
}

void UIChangeHalfFull()
{
	_SetStatusHalfFull(); 
}

void UISetHalf()
{
	_SetStatusHalf(); 
}

void UISetFull()
{
	_SetStatusFull(); 
}


void UISetChiEng()
{
	_SetStatusChiEng();  
}

void UISetChinese()
{
	_SetStatusChinese();  
}

void UISetEnglish()
{
	_SetStatusEnglish();  
}


void UIChangeBoPoMoFoLayout(int index)
{
	if(index == -1) return;

	CompCursorPos=0;  //James test

	CurrentIC = index;
	UISetStatusModStrCurrent(CurrentIC);
	_ShowStatusPage();
}

int UICurrentInputMethod()
{
	return CurrentIC;
}



// Candi window

void UISetCandiColor(int forecolor, int backcolor, int titlecolor, int titlebackcolor)
{
	_SetCandiColor( forecolor, backcolor, titlecolor,titlebackcolor);
}

void UISetCandiFont(const wchar_t* ff, int fontsize)
{
	_SetCandiFont(ff, fontsize);
}

void UISetCandiOpacity(int o)
{
	_SetCandiOpacity(o);
}


int UIGetCandHeight()
{
	return _GetCandHeight();
}

int UIGetCandWidth()
{
	return _GetCandWidth();
}


void UICreateCandWindow(HWND hUIWnd)
{
	if (!IsWindow(uiCand.hWnd))
		uiCand.hWnd = _CreateCandiPageWithHandle(hUIWnd);		
	
	return;
}

int CompIndexToXPos( int i );

void UISetCandStr(wchar_t* lpStr)
{

	wchar_t* lpCandStr;
	
	if(wcslen(lpStr))
	{	
		lpCandStr = wcsdup(lpStr);
		_SetCandiString(lpCandStr);
	}
	else
	{
		_ClearCandiPage();		
	}

}


void UIMoveCandWindow(int X, int Y, int compHeight)
{
	_MoveCandiPage(X, Y, compHeight);	
	
}


static bool stillrunning = false;
void UISelectNextCandItem(int i)
{
	if(!stillrunning){
		stillrunning=true;
		_SelectNextItem(i);
		stillrunning=false;
	}
}
int UIGetCandSelectedItemKey()
{
	return _GetSelectedItemKey();
}

void UIShowCandWindow()
{
	murmur("UIShowCandWindow");
	if (IsWindow(uiCand.hWnd)) {
		_ShowCandiPage();
	}
}

void UIHideCandWindow()
{
	murmur("UIHideCandWindow");
	if (IsWindow(uiCand.hWnd)) {
		_HideCandiPage();
	}
}

void UIExpandCandi()
{
	_ExpandCandi();
}

// Composition window
void UICreateCompWindow(HWND hUIWnd)
{
	if (!IsWindow(uiComp.hWnd))
	{
		uiComp.hWnd = _CreateCompPage();
		_SetCompAppHWnd(hUIWnd);
	}
}

void UIMoveCompWindow(
	int x, int y,
	int fontHeight, const wchar_t* fontName)
{
	_MoveCompPage(x, y, fontHeight, fontName);
}

void UISetCompStr(wchar_t* lpStr)
{
	if(wcslen(lpStr))
	{
		lpCompStr = wcsdup(lpStr);
		_SetCompString(lpCompStr);
	}
}
void UIClearCompStr()
{
	_ClearCompPage();
}

void UISetCompCaretPosX(int xIndex)
{
	_SetCompCaretPosX(xIndex);
}


void UIShowCompWindow()
{

	if (IsWindow(uiComp.hWnd))
	{
		_ShowCompPage();
	}

}

void UIHideCompWindow()
{
	if (IsWindow(uiComp.hWnd))
	{
		_HideCompPage();		
	}
}

void UISetMarkFrom(int i)
{
	CompSelStart = i;
	_SetCompMarkFrom(CompSelStart);
}

void UISetMarkTo(int i)
{
	CompSelEnd = i;
	_SetCompMarkTo(CompSelEnd);
}

int UIGetHeight()
{
	return _GetHeight();
}

int UIGetCaretPosX()
{
	return _GetCaretPosX();
}


//Notify window

void UISetNotifyColor(int forecolor, int backcolor)
{
	_SetNotifyColor( forecolor, backcolor);
}

void UISetNotifyFont(const wchar_t* ff, int fontsize)
{
	_SetNotifyFont(ff, fontsize);
}

void UISetNotifyEnabled(bool b)
{
	_SetNotifyEnabled(b);
}

void UISetNotifyOpacity(int o)
{
	_SetNotifyOpacity(o);
}

int UIGetNotifyHeight()
{
	return _GetNotifyHeight();

}


int UIGetNotifyWidth()
{
	return _GetNotifyWidth();

}

void UICreateNotifyWindow(HWND hUIWnd)
{
	
	if (!IsWindow(uiNotify.hWnd))
	{
		uiNotify.hWnd=_CreateNotifyPage();

	}
	return;
}

void UIShowNotifyWindow()
{
	_ShowNotifyPage();

}

void UIHideNotifyWindow()
{
	_ClearNotifyPage(); 
	_HideNotifyPage();
} 

void UIMoveNotifyWindow(int X, int Y)
{ 
	_MoveNotifyPage(X, Y);
	
}

void UISetNotifyStr(wchar_t* lpStr)
{
	free(lpNotifyStr);
	lpNotifyStr = wcsdup(lpStr);
	_SetNotifyString(lpNotifyStr);
}
