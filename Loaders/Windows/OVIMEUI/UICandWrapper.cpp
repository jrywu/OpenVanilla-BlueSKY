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

ref class CSharpCandiWrapper{
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
		CSharpCandiWrapper::Instance()->Handle)->ToPointer());
}

HWND _CreateCandiPageWithHandle(HWND hwnd) //create with handle (x)
{	
	CSharpCandiWrapper::Instance()->SetHWND(safe_cast<IntPtr>(hwnd));
	return (HWND)(safe_cast<IntPtr^>(
		CSharpCandiWrapper::Instance()->Handle)->ToPointer());
}

void _SetCandiString(const wchar_t* candiStr)
{		
	CSharpCandiWrapper::Instance()
		->SetCandidates(gcnew String(candiStr)); 
}

void _ShowCandiPage()
{		
	CSharpCandiWrapper::Instance()->ShowNoActive();
}

void _MoveCandiPage(int x, int y, int compHeight)
{
	CSharpCandiWrapper::Instance()->SetLocation(x, y, compHeight);
}

void _HideCandiPage()
{
	CSharpCandiWrapper::Instance()->HideNoActive();
}

void _ClearCandiPage()
{
	CSharpCandiWrapper::Instance()->ClearCandidates();
}

void _EndCandiPage()
{
	CSharpCandiWrapper::Instance()->DisposeForm();
}

void _ExpandCandi() //�i�} Candi window
{
	CSharpCandiWrapper::Instance()->SwitchCandiMode();	
}

void _SelectNextItem(int i)
{
	CSharpCandiWrapper::Instance()->selectNextItem(i);	
}

int _GetSelectedItemKey()
{
	return CSharpCandiWrapper::Instance()->getSelectedItemKey();	

}

int _GetCandWidth()
{
	return CSharpCandiWrapper::Instance()->GetWidth();	

}

int _GetCandHeight()
{
	return CSharpCandiWrapper::Instance()->GetHeight();

}

void _SetCandiFont(const wchar_t * ff, int fontsize)
{
	CSharpCandiWrapper::Instance()->SetFont(gcnew String(ff), fontsize);	
}

void _SetCandiColor(int forecolor, int backcolor, int titlecolor, int titlebackcolor)
{
	CSharpCandiWrapper::Instance()->SetColor(forecolor, backcolor, titlecolor, titlebackcolor);	
}

void _SetCandiOpacity(int o)
{
	CSharpCandiWrapper::Instance()->SetOpacity(o);	
}