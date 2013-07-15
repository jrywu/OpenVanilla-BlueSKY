//#define OV_DEBUG
#include <vector>
#include "OVIMEUI.h"

#pragma managed


using namespace System;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Collections;
using namespace std;
using namespace OVUIServer;

ref class OVUIServerStatusWrapper{
private:
	static IMEStatusForm^ m_instance = nullptr;

public:
	static IMEStatusForm^ Instance()
	{
		if(m_instance == nullptr)
			m_instance =  gcnew IMEStatusForm(); 
		return m_instance;
	}
};

HWND _CreateStatusPage() //create
{		
	return (HWND)(safe_cast<IntPtr^>(
		OVUIServerStatusWrapper::Instance()->Handle)->ToPointer());
}

void _SetStatusAppHWnd(HWND hwnd)
{	
	OVUIServerStatusWrapper::Instance()->SetAppHWnd(safe_cast<IntPtr>(hwnd));
}

void _ShowStatusPage()
{	
	OVUIServerStatusWrapper::Instance()->ShowNoActive();	
}

void _MoveStatusPage(int x,int y)
{	
	OVUIServerStatusWrapper::Instance()->SetLocation(x,y);	
}

void _DockStatusPage(int x,int y)
{	
	OVUIServerStatusWrapper::Instance()->DockedLocation(x,y);	
}

void _SetStatusChiEng()
{	
	OVUIServerStatusWrapper::Instance()->SetChiEng();	
}

void _SetStatusChinese()
{	
	OVUIServerStatusWrapper::Instance()->SetChinese();	
}

void _SetStatusEnglish()
{	
	OVUIServerStatusWrapper::Instance()->SetEnglish();	
}



void _SetStatusHalfFull()
{	
	OVUIServerStatusWrapper::Instance()->SetHalfFull();	
}

void _SetStatusHalf()
{	
	OVUIServerStatusWrapper::Instance()->SetHalf();	
}
void _SetStatusFull()
{	
	OVUIServerStatusWrapper::Instance()->SetFull();	
}


bool _GetItemChecked(int index)
{	
	return OVUIServerStatusWrapper::Instance()->getItemChecked(index);	
}

bool _GetIsStatusDocked() 
{
	return OVUIServerStatusWrapper::Instance()->getIsDocked();
}

void _SetIsStatusDocked(bool t) 
{
	OVUIServerStatusWrapper::Instance()->setIsDocked(t);
}


void _SetItemChecked(int index, bool c)
{	
	OVUIServerStatusWrapper::Instance()->setItemChecked(index, c);	
}
void _SetItemEnabled(int index, bool c)
{	
	OVUIServerStatusWrapper::Instance()->setItemEnabled(index, c);	
}

void _SetStatusSimpifiedOrTraditional(bool isTraditional)
{		
	OVUIServerStatusWrapper::Instance()->SetSimpifiedOrTraditional(isTraditional);	
}

void _SetStatusModString(int index)
{
	OVUIServerStatusWrapper::Instance()->SetModString(index);		
}

void _ClearStatusMenuModString()
{
	OVUIServerStatusWrapper::Instance()->ClearMenuModString();	
}

void _SetStatusMenuModString(const wchar_t* menuModStr)
{
	OVUIServerStatusWrapper::Instance()->SetMenuModString(
		gcnew String(menuModStr));	
}

void _HideStatusPage()
{
	OVUIServerStatusWrapper::Instance()->HideNoActive();	
}

int _GetStatusSelectedModuleIndex()
{
	return OVUIServerStatusWrapper::Instance()->GetSelectedModuleIndex();	
}

void _RotateStatusSelectedModule()
{
	OVUIServerStatusWrapper::Instance()->RotateModule();	
}


void _setStatuskpCount(int count)
{
	OVUIServerStatusWrapper::Instance()->setkpCount(count);	
}

void _setStatusimCount(int count)
{
	OVUIServerStatusWrapper::Instance()->setimCount(count);	
}

void _setStatusofCount(int count)
{
	OVUIServerStatusWrapper::Instance()->setofCount(count);	
}