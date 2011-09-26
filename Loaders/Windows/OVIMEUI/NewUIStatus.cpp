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

ref class CSharpStatusWrapper{
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
		CSharpStatusWrapper::Instance()->Handle)->ToPointer());
}

void _SetStatusAppHWnd(HWND hwnd)
{	
	CSharpStatusWrapper::Instance()->SetAppHWnd(safe_cast<IntPtr>(hwnd));
}

void _ShowStatusPage()
{	
	CSharpStatusWrapper::Instance()->ShowNoActive();	
}

void _MoveStatusPage(int x,int y)
{	
	CSharpStatusWrapper::Instance()->SetLocation(x,y);	
}

void _DockStatusPage(int x,int y)
{	
	CSharpStatusWrapper::Instance()->DockedLocation(x,y);	
}

void _SetStatusChiEng()
{	
	CSharpStatusWrapper::Instance()->SetChiEng();	
}

void _SetStatusHalfFull()
{	
	CSharpStatusWrapper::Instance()->SetHalfFull();	
}

bool _GetItemChecked(int index)
{	
	return CSharpStatusWrapper::Instance()->getItemChecked(index);	
}

bool _GetIsStatusDocked() 
{
	return CSharpStatusWrapper::Instance()->getIsDocked();
}

void _SetIsStatusDocked(bool t) 
{
	CSharpStatusWrapper::Instance()->setIsDocked(t);
}


void _SetItemChecked(int index, bool c)
{	
	CSharpStatusWrapper::Instance()->setItemChecked(index, c);	
}
void _SetItemEnabled(int index, bool c)
{	
	CSharpStatusWrapper::Instance()->setItemEnabled(index, c);	
}

void _SetStatusSimpifiedOrTraditional(bool isTraditional)
{		
	CSharpStatusWrapper::Instance()->SetSimpifiedOrTraditional(isTraditional);	
}

void _SetStatusModString(int index)
{
	CSharpStatusWrapper::Instance()->SetModString(index);		
}

void _ClearStatusMenuModString()
{
	CSharpStatusWrapper::Instance()->ClearMenuModString();	
}

void _SetStatusMenuModString(const wchar_t* menuModStr)
{
	CSharpStatusWrapper::Instance()->SetMenuModString(
		gcnew String(menuModStr));	
}

void _HideStatusPage()
{
	CSharpStatusWrapper::Instance()->HideNoActive();	
}

int _GetStatusSelectedModuleIndex()
{
	return CSharpStatusWrapper::Instance()->GetSelectedModuleIndex();	
}

void _RotateStatusSelectedModule()
{
	CSharpStatusWrapper::Instance()->RotateModule();	
}


void _setStatuskpCount(int count)
{
	CSharpStatusWrapper::Instance()->setkpCount(count);	
}

void _setStatusimCount(int count)
{
	CSharpStatusWrapper::Instance()->setimCount(count);	
}

void _setStatusofCount(int count)
{
	CSharpStatusWrapper::Instance()->setofCount(count);	
}