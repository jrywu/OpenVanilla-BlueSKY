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

ref class OVUIServerCandiWrapper{
private:
	static IMECandidateForm^ m_instance = nullptr;

public:
	static IMECandidateForm^ Instance()
	{
		if(m_instance == nullptr)
			m_instance = gcnew IMECandidateForm();
		return m_instance;
	}
};

HWND _CreateCandiPage() //create without handle (x)
{	
	return (HWND)(safe_cast<IntPtr^>(
		OVUIServerCandiWrapper::Instance()->Handle)->ToPointer());
}

HWND _CreateCandiPageWithHandle(HWND hwnd) //create with handle (x)
{	
	OVUIServerCandiWrapper::Instance()->SetHWND(safe_cast<IntPtr>(hwnd));
	return (HWND)(safe_cast<IntPtr^>(
		OVUIServerCandiWrapper::Instance()->Handle)->ToPointer());
}

void _SetCandiString(const wchar_t* candiStr)
{		
	OVUIServerCandiWrapper::Instance()
		->SetCandidates(gcnew String(candiStr)); 
}

void _ShowCandiPage()
{		
	OVUIServerCandiWrapper::Instance()->ShowNoActive();
}

void _MoveCandiPage(int x, int y, int compHeight)
{
	OVUIServerCandiWrapper::Instance()->SetLocation(x, y, compHeight);
}

void _HideCandiPage()
{
	OVUIServerCandiWrapper::Instance()->HideNoActive();
}

void _ClearCandiPage()
{
	OVUIServerCandiWrapper::Instance()->ClearCandidates();
}

void _EndCandiPage()
{
	OVUIServerCandiWrapper::Instance()->DisposeForm();
}

void _ExpandCandi() //®i¶} Candi window
{
	OVUIServerCandiWrapper::Instance()->SwitchCandiMode();	
}

void _SelectNextItem(int i)
{
	OVUIServerCandiWrapper::Instance()->selectNextItem(i);	
}

int _GetSelectedItemKey()
{
	return OVUIServerCandiWrapper::Instance()->getSelectedItemKey();	

}

int _GetCandWidth()
{
	return OVUIServerCandiWrapper::Instance()->GetWidth();	

}

int _GetCandHeight()
{
	return OVUIServerCandiWrapper::Instance()->GetHeight();

}

void _SetCandiFont(const wchar_t * ff, int fontsize)
{
	OVUIServerCandiWrapper::Instance()->SetFont(gcnew String(ff), fontsize);	
}

void _SetCandiColor(int forecolor, int backcolor, int titlecolor, int titlebackcolor)
{
	OVUIServerCandiWrapper::Instance()->SetColor(forecolor, backcolor, titlecolor, titlebackcolor);	
}

void _SetCandiOpacity(int o)
{
	OVUIServerCandiWrapper::Instance()->SetOpacity(o);	
}