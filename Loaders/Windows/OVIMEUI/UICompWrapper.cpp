#include <vector>
#include "OVIMEUI.h"

#pragma managed

using namespace System;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Collections;
using namespace std;
using namespace OVUIServer;

ref class OVUIServerCompWrapper{
private:
	static IMECompRichForm^ m_instance = nullptr;

public:
	static IMECompRichForm^ Instance()
	{
		if(m_instance == nullptr)
			m_instance = gcnew IMECompRichForm();
		return m_instance;
	}
};

HWND _CreateCompPage()//create
{
	return (HWND)(safe_cast<IntPtr^>(
		OVUIServerCompWrapper::Instance()->Handle)->ToPointer());
}

void _SetCompString(const wchar_t* compStr)
{
	OVUIServerCompWrapper::Instance()->SetComp(gcnew String(compStr));
}

void _SetCompCaretPosX(int x)
{
	OVUIServerCompWrapper::Instance()->SetCaretX(x);
}

void _SetCompMarkFrom(int x)
{
	OVUIServerCompWrapper::Instance()->SetCompMarkFrom(x);
}

void _SetCompMarkTo(int x)
{
	OVUIServerCompWrapper::Instance()->SetCompMarkTo(x);
}

void _ShowCompPage()
{
	OVUIServerCompWrapper::Instance()->ShowNoActive();
}

void _MoveCompPage(
	int x, int y, int fontHeight, const wchar_t* fontName)
{
	OVUIServerCompWrapper::Instance()->SetLocation(x, y);
	OVUIServerCompWrapper::Instance()->SetFont(
		fontHeight, gcnew System::String(fontName));
}

void _HideCompPage()
{
	OVUIServerCompWrapper::Instance()->HideNoActive();
}

void _ClearCompPage()
{
	OVUIServerCompWrapper::Instance()->ClearComp();
}

void _EndCompPage()
{
	OVUIServerCompWrapper::Instance()->DisposeForm();
}

int _GetCompValue()
{
	return 0;
}

int _GetHeight()
{
	return OVUIServerCompWrapper::Instance()->Height;
}

int _GetCaretPosX()
{
	return OVUIServerCompWrapper::Instance()->GetCaretPosX();
}

void _SetCompAppHWnd(HWND hwnd)
{	
	OVUIServerCompWrapper::Instance()->SetAppHWnd(safe_cast<IntPtr>(hwnd));
}