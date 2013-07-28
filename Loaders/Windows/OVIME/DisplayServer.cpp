//#define OV_DEBUG 
#include "DisplayServer.h"
#include "OVIME.h"
#include "ImmController.h"
#include <windows.h>

DisplayServer::DisplayServer()
{
	murmur("dsvr initialized\n");
	hasCompStarted=false;
	isStatusEnabled=false;
	isCompEnabled=false;
	isCandiEnabled=false;	
}

DisplayServer::~DisplayServer()
{
	m_hIMC = NULL;
}

void DisplayServer::setHIMC(HIMC hIMC)
{
	m_hIMC = hIMC;
}

AVDisplayServer *DisplayServer::getStatusPos(LPPOINT lp_pt)
{
	
	if(UIGetStatusPos(lp_pt)==-1) {
		lp_pt->x=-1;
		lp_pt->y=-1;
	}

	return this;
	 
}

AVDisplayServer *DisplayServer::setBufString(const char *str, int caretX, int markFrom, int markTo)
{
	hideNotify();

	ImmController* controller = ImmController::open();
	
	if(!controller->getCompStartedFlag()) {
		murmur("\tDSVR: STARTCOMPOSITION");		
		MyGenerateMessage( m_hIMC, WM_IME_STARTCOMPOSITION,0,0);  
	}

	
	ImmModel* model = ImmModel::open(m_hIMC);

	
	wchar_t wstr[1024];
	
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
 	wcscpy(model->getMyPrivate()->PreEditStr, wstr);
	


	MakeCompStr(model->getMyPrivate(), model->getCompStr(), caretX, markFrom, markTo);
	wchar_t* word = GETLPCOMPSTR(model->getCompStr());


	
	murmur("\tDISPLAYSERVER::setbufString str:%s, len,%i, caretX:%i",str, wcslen(wstr), caretX);

	
	UISetCompStr(model->getMyPrivate()->PreEditStr);
	if(caretX > -1 ) UISetCompCaretPosX(caretX); 
	else UISetCompCaretPosX((int)wcslen(wstr));
	
	
	ImmModel::close();
	
	
	murmur("\tsend WM_IME_COMPOSITION"); 
	MyGenerateMessage( m_hIMC, WM_IME_COMPOSITION, 
				*(WORD*)word,
				(GCS_COMPSTR|GCS_COMPATTR
				|GCS_COMPCLAUSE
				|GCS_CURSORPOS
				|GCS_DELTASTART				 
				));
	if(wcslen(wstr)==0){ //Composition buffer are all deleted send ENDCOMPOSITION
 		murmur("\tWM_IME_ENDCOMPOSITION"); 
		MyGenerateMessage(m_hIMC, WM_IME_ENDCOMPOSITION, 0, 0);

		
	}

	
	return this;
}

AVDisplayServer *DisplayServer::sendBuf(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);

	murmur("\tDISPLAYSERVER::sendbuf str:%s, len,%i",str, wcslen(wstr));
	ImmModel* model = ImmModel::open(m_hIMC); 
	wcscpy(model->getMyPrivate()->PreEditStr, L"");	
	MakeCompStr(model->getMyPrivate(), model->getCompStr());

	model->getCompStr()->dwResultStrLen = (int)wcslen(wstr);
	wchar_t* word = GETLPRESULTSTR(model->getCompStr());
	wcscpy(word, wstr);
	
	ImmModel::close();

	showBuf(false);
	UIClearCompStr();

	murmur("\tCOMPOSITION GCS_RESULTSTR");

	MyGenerateMessage(
		m_hIMC,
		WM_IME_COMPOSITION,
		*(WORD*)word,
		GCS_COMPSTR|GCS_COMPATTR|
		GCS_RESULTSTR|GCS_RESULTCLAUSE
	);

	ImmController* controller = ImmController::open();
	controller->setCompStartedFlag(false);


	murmur("\tDSRV:ENDCOMPOSITION");
	MyGenerateMessage(m_hIMC, WM_IME_ENDCOMPOSITION, 0, 0);

	return this;
}

AVDisplayServer *DisplayServer::setCandiString(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	ImmModel* model = ImmModel::open(m_hIMC);
	wcscpy(model->getMyPrivate()->CandStr, wstr);
	UpdateCandidate(model->getIMC(), wstr);

	UISetCandStr(wstr);


	ImmModel::close();
	

	return this;
}

AVDisplayServer *DisplayServer::showNotify(const char *str)
{

	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	murmur("showNotify:%s",wstr);
	UISetNotifyStr(wstr); 
	UIShowNotifyWindow();
	

	return this;
}

AVDisplayServer *DisplayServer::hideNotify()
{

	UIHideNotifyWindow();	
	return this;
}

AVDisplayServer *DisplayServer::moveNotify(int x, int y)
{
	UIMoveNotifyWindow(x, y);
	return this;
}
AVDisplayServer *DisplayServer::moveBuf(
	int x, int y, int fontHeight, const wchar_t* fontName)
{
	murmur("\t move comp to (%d,%d)",x,y);
	UIMoveCompWindow(x, y, fontHeight, fontName);
	return this;
}

AVDisplayServer *DisplayServer::moveCandi(int x, int y, int compHeight)
{
	murmur("\t move candi to (%d,%d)",x,y);
	UIMoveCandWindow(x, y, compHeight);
	return this;
}

AVDisplayServer *DisplayServer::SetCompStarted(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCompStarted=true");
	else
		murmur("\t*DisplayServer::SetCompStarted=false");
	
	hasCompStarted=t;
	return this;
}

AVDisplayServer *DisplayServer::SetStatusEnabled(bool t)
{
	if(t){
		murmur("\t*DisplayServer::SetStatusEnabled=true");
		isStatusEnabled=true;
	}else{
		murmur("\t*DisplayServer::SetStatusEnabled=false");
		if(isWindows8()){  //force hide notify and candi in windows 8
			isStatusEnabled=false;
			showCandi(false);	
			hideNotify();
		}

	}
	
	//isStatusEnabled=t;
	return this;
}

AVDisplayServer *DisplayServer::SetCompEnabled(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCompEnabled=true");
	else
		murmur("\t*DisplayServer::SetCompEnabled=false");
	
	isCompEnabled=t;
	return this;
}

AVDisplayServer *DisplayServer::SetCandiEnabled(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCandiEnabled=true");
	else
		murmur("\t*DisplayServer::SetCandiEnabled=false");
	
	isCandiEnabled=t;
	return this; 
}

AVDisplayServer *DisplayServer::showStatus(bool t)
{	
	murmur("dsvr: show status window: %d", t);
	//if(dsvr->isStatusEnabled && !t) 
	if(!t) 
	{
		UIHideStatusWindow();
		dsvr->SetStatusEnabled(false);
	}
	else//if(!dsvr->isStatusEnabled && t)
	{
		UIShowStatusWindow();
		dsvr->SetStatusEnabled(true);
	}
	
	return this;
}

bool DisplayServer::GetisStatusDocked()
{
	return UIGetIsStatusDocked();
}
void DisplayServer::SetisStatusDocked(bool t)
{
	UISetIsStatusDocked(t);
	
}


AVDisplayServer *DisplayServer::showBuf(bool t)
{
	if(dsvr->isCompEnabled)
	{
		ImmModel* model = ImmModel::open(m_hIMC);

		if(t &&
			model->getMyPrivate()->PreEditStr &&
			wcslen(model->getMyPrivate()->PreEditStr))
		{
			murmur("\t show comp");
			UIShowCompWindow();
		}
		else
		{
			murmur("\t hide comp");
			UIHideCompWindow();
		}

		ImmModel::close();
	}
	return this;
}

AVDisplayServer *DisplayServer::showCandi(bool t)
{
	if(!dsvr->isCandiEnabled) 
		dsvr->SetCandiEnabled(true);
		
//	{
		ImmModel* model = ImmModel::open(m_hIMC);

		if(t &&
			model->getMyPrivate()->CandStr &&
			wcslen(model->getMyPrivate()->CandStr))
		{
			murmur("\t show candi"); 
			RefreshUI(m_hIMC);
			UIShowCandWindow();
		}
		else
		{
			murmur("\t hide candi");
			UIHideCandWindow();
		}

		ImmModel::close();
//	}
	return this;
}

AVDisplayServer *DisplayServer::setMarkFrom(int i)
{
	UISetMarkFrom(i);
	return this;
}

AVDisplayServer *DisplayServer::setMarkTo(int i)
{
	UISetMarkTo(i);
	return this;
}

AVDisplayServer *DisplayServer::selectNextCandItem(int i)
{
	UISelectNextCandItem(i);
	return this;
}


int DisplayServer::getCandSelectedItemKey()
{
	return UIGetCandSelectedItemKey();
	
}

bool DisplayServer::getStatusModuleChecked(int index)
{
	return UIGetModuleChecked(index);
}


AVDisplayServer *DisplayServer::setStatusModuleChecked(int index, bool b)
{
	UISetModuleChecked(index, b);
	return this;
}

AVDisplayServer *DisplayServer::setStatusModuleEnabled(int index, bool b)
{
	UISetModuleEnabled(index, b);
	return this;
}

AVDisplayServer *DisplayServer::setNotifyGlobalEnabled(bool b)
{
	UISetNotifyEnabled(b);
	return this;
}

AVDisplayServer *DisplayServer::setNotifyColor(int forecolor, int backcolor)
{
	murmur("dsvr::setNotifyColor, %d, %d", forecolor, backcolor);
	return this;
}

AVDisplayServer *DisplayServer::setNotifyFont(const wchar_t* ff, int fontsize)
{
	UISetNotifyFont(ff, fontsize);
	return this;
}

AVDisplayServer *DisplayServer::setCandiColor(int forecolor, int backcolor, int titlecolor, int titlebackcolor)
{
	UISetCandiColor(forecolor, backcolor, titlecolor, titlebackcolor);
	return this;
}

AVDisplayServer *DisplayServer::setCandiFont(const wchar_t* ff, int fontsize)
{
	UISetCandiFont(ff, fontsize);
	return this;
}


AVDisplayServer *DisplayServer::setCandiOpacity(int o)
{
	UISetCandiOpacity(o);
	return this;
}

AVDisplayServer *DisplayServer::setNotifyOpacity(int o)
{
	UISetNotifyOpacity(o);
	return this;
}
