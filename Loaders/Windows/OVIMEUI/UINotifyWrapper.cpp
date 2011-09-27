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

ref class OVUIServerNotifyWrapper{
private:
	static IMENotifyForm^ m_instance = nullptr;

public:
	static IMENotifyForm^ Instance()
	{
		if(m_instance == nullptr)
			m_instance = gcnew IMENotifyForm();
		return m_instance;
	}
};

HWND _CreateNotifyPage()//create
{
	return (HWND)(safe_cast<IntPtr^>(
		OVUIServerNotifyWrapper::Instance()->Handle)->ToPointer());
}

HWND _CreateNotifyPageWithHandle(HWND hwnd)//create
{
	OVUIServerNotifyWrapper::Instance()->SetHWND(safe_cast<IntPtr>(hwnd));
	return (HWND)(safe_cast<IntPtr^>(
		OVUIServerNotifyWrapper::Instance()->Handle)->ToPointer());
}

void _SetNotifyString(const wchar_t* notifyStr)
{
	OVUIServerNotifyWrapper::Instance()->SetNotifyStr(gcnew String(notifyStr));
}

void _ShowNotifyPage()
{
	OVUIServerNotifyWrapper::Instance()->ShowNoActive();
}

void _MoveNotifyPage(int x,int y)
{
	OVUIServerNotifyWrapper::Instance()->SetLocation(x, y);
}

void _HideNotifyPage()
{
	OVUIServerNotifyWrapper::Instance()->HideNoActive();
}

void _ClearNotifyPage()
{
	OVUIServerNotifyWrapper::Instance()->ClearNotify();
}


void _EndNotifyPage()
{
	OVUIServerNotifyWrapper::Instance()->DisposeForm();
}
int _GetNotifyWidth()
{
	return OVUIServerNotifyWrapper::Instance()->GetWidth();
}
int _GetNotifyHeight()
{
	return OVUIServerNotifyWrapper::Instance()->GetHeight();
}

void _SetNotifyFont(const wchar_t * ff, int fontsize)
{
	OVUIServerNotifyWrapper::Instance()->SetFont(gcnew String(ff), fontsize);	
}

void _SetNotifyColor(int forecolor, int backcolor)
{
	OVUIServerNotifyWrapper::Instance()->SetColor(forecolor, backcolor);	
}

void _SetNotifyEnabled(bool b)
{
	OVUIServerNotifyWrapper::Instance()->SetNotifyEnabled(b);	
}
void _SetNotifyOpacity(int o)
{
	OVUIServerNotifyWrapper::Instance()->SetOpacity(o);	
}