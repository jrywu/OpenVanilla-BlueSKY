//#define OV_DEBUG
//#define showCompWindow  //Uncomment this to show composition window in wince.
#include "ImmController.h"
#ifdef WINCE
#include "OVIMEUI.h"
#endif

ImmController* ImmController::m_self = NULL;

ImmController::ImmController()
{ 
	m_shiftPressedTime = 0;
	m_isCompStarted = false;
}

ImmController::~ImmController(void)
{
	m_self = NULL; 
} 

ImmController* ImmController::open()
{
	murmur("ImmController::open\n");
	if(m_self == NULL)
		m_self = new ImmController();

	return m_self;
}

void ImmController::close(void)
{
	murmur("ImmController::close\n");
	if(m_self) delete m_self;
}

int ImmController::onKeyShiftOnly(HIMC hIMC, LPARAM lKeyData)
{
	int shiftState;
	if(!getKeyInfo(lKeyData).isKeyUp)  // Shift pressed
	{
		m_shiftPressedTime = GetTickCount(); 
		murmur("shift-only: down; TickCount:%d", m_shiftPressedTime);
		shiftState = 1;
	}
	else 
	{
	murmur("tickcound- m_shiftPressedTime: %d", GetTickCount() - m_shiftPressedTime);
	if(GetTickCount() - m_shiftPressedTime < 200) // Shift Up
	{
		murmur("shift-only: up");
		
		//Toggle Chinese/English mode.
		//lParam == 2

#ifdef WINCE
/*  //not working on pda keyboard (shift is toggle key)
		DWORD conv, sentence;
		ImmGetConversionStatus(hIMC, &conv, &sentence);
		if(conv & IME_CMODE_NATIVE) conv &= !IME_CMODE_NATIVE;
		else conv |= IME_CMODE_NATIVE;
		ImmSetConversionStatus(hIMC, conv, sentence);
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_SETCONVERSIONMODE, 0);
*/
#else
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 2);
#endif
		shiftState = 2;
		m_shiftPressedTime = 0;
	}
	else   //Shift Up too late
	{ 
		murmur("shift-only: other");
		shiftState = 0;
		m_shiftPressedTime = 0;
	}
	}
	return shiftState;
}

int ImmController::onKeyShift(HIMC hIMC, UINT uVKey, LPARAM lKeyData)
{
	murmur("S: vkey =%u", LOWORD(uVKey));
	int processState;
	if(LOWORD(uVKey) == VK_SPACE)   //shift + space
	{
		
		murmur("S_Space: Full-Half char");
#ifdef WINCE
		DWORD conv, sentence;
		ImmGetConversionStatus(hIMC, &conv, &sentence);
		if(conv & IME_CMODE_FULLSHAPE) conv &= !IME_CMODE_FULLSHAPE;
		else conv |= IME_CMODE_FULLSHAPE;
		ImmSetConversionStatus(hIMC, conv, sentence);
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_SETCONVERSIONMODE, 0);
#else
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 1);
#endif
		m_shiftPressedTime = 0;
		
		processState = 1;
	}
	else if(LOWORD(uVKey) == VK_SHIFT)  // only shift
	{
		switch(onKeyShiftOnly(hIMC, lKeyData)) {
			case 1:
				murmur("S: EN-ZH: waiting for key-up");
				processState = 1;
				break;
			case 2:
				murmur("S: EN-ZH: proceeded");
				processState = 2;
				break;
			case 0:
			default:
				murmur("S: passed");
				m_shiftPressedTime = 0;
				processState = 0;
				break;
		}
	}
	else  //shift + ? 
	{
		murmur("S_%u: assume normal", LOWORD(uVKey));
		m_shiftPressedTime = 0;
		DWORD conv, sentence;
		ImmGetConversionStatus(hIMC, &conv, &sentence);

		if(!(conv & IME_CMODE_NATIVE) &&!(conv & IME_CMODE_FULLSHAPE))  //processState = 0;
			processState = 0;//Half-shape alphanumeric mode
		else
			processState = 2;
	}
	return processState;
}

int ImmController::onKeyCtrl(HIMC hIMC, UINT uVKey)
{
	int processState;
	switch(LOWORD(uVKey)) {
		case VK_CONTROL:
			murmur("C: passed");
			processState = 0;
			break;
		case VK_MENU:
			murmur("C_A: passed");
			processState = 0;
			break;
#ifdef WINCE
		case VK_BACKSLASH:
#else
		case VK_OEM_5:
#endif
			//Rotae the module by Ctrl+"\":
			//lParam == 8
			murmur("C_\\: Rotae module");
#ifdef WINCE
#else
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 8);
#endif
			processState = 1;
			break;
/*-------------Jan 1, 09. conflict with word subscript hotkey Ctrl+"+".
#ifdef WINCE
		case VK_EQUAL:
#else
		case VK_OEM_PLUS:					
#endif
			//Change the BoPoMoFo keyboard layout by Ctrl+"=":
			//lParam == 5
			murmur("C_=: change Hsu's layout");
#ifdef WINCE
#else
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 5);
#endif 
			processState = 1; 
			break; 
			*/  
		case VK_SPACE:{
			murmur("C_Space: switch IME");

			if(ImmGetOpenStatus(hIMC)){
				AVLoader* loader = AVLoader::open(); 
				loader->unloadCurrentModule();
#ifndef WINCE
			}
#else
				ImmSetOpenStatus(NULL, FALSE);
			}
			else
				ImmSetOpenStatus(NULL, TRUE);
#endif
			if(isWindows8())
				MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 2); //Switch chi/eng in windows 8.

			processState = 1;
			break;
					  }
		case VK_SHIFT:
			murmur("C_S: rotate IME");
			processState = 1;
			break;
		default:
			murmur("C_%u: assume normal", LOWORD(uVKey));
			processState = 2;
	}
	return processState;
}

int ImmController::onKeyCtrlAlt(HIMC hIMC, UINT uVKey)
{
	int processState;
	switch(LOWORD(uVKey)) {
		case VK_MENU:
		case VK_CONTROL:
			murmur("C_A: passed");
			processState = 0;
			break;
		case VK_G:
			//Toggle Traditional / Simplified Chinese.
			//lParam == 4
			murmur("C_A_g: TW-CN");
#ifdef WINCE
			processState = 2;
#else
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 4);
			processState = 1;
#endif
			
			break;
		case VK_K:
			//Toggle Large Candidate window.
			//lParam == 6
#ifdef WINCE
			processState = 2;
#else
			murmur("C_A_k: Expand Cand");
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 6);
			
#endif
			break;
		case VK_L:
			// Test Notify window.
			murmur("C_A_l: Notify");
#ifdef WINCE
			processState = 1;
#else
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 7);
			processState = 1;
#endif
			break;
		default:
			murmur("C_A_%u: assume normal", LOWORD(uVKey));
			processState = 2;
			break;
	}
	return processState;
}

int ImmController::onControlEvent
(HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState)
{
	int processState;

	if(getKeyInfo(lKeyData).isKeyUp) 
	{
		if(LOWORD(uVKey) == VK_SHIFT) {
			processState = onKeyShift(hIMC, uVKey, lKeyData);
		} else {
			murmur("other key up");
			m_shiftPressedTime = 0;
			processState = 0;
		}
	}
	else if(isCtrlPressed(lpbKeyState))
	{
		if(isAltPressed(lpbKeyState)) {
			murmur("ctrl-alt state");
			processState = onKeyCtrlAlt(hIMC, uVKey);
		}
		else {
			murmur("ctrl state");
			processState = onKeyCtrl(hIMC, uVKey);
		}
	}
#ifdef WINCE
	else if(isShiftPressed(lpbKeyState)|| m_shiftPressedTime) {
#else
	else if(isShiftPressed(lpbKeyState)) {
#endif
		murmur("shift state");
		processState = onKeyShift(hIMC, uVKey, lKeyData);

		//else processState = onKeyShift(hIMC, uVKey, lKeyData);
	}
	else if(isFnPressed(uVKey)) {
		murmur("Fn pressed");
		processState = 0;
	}
	else {
		murmur("other state: assume normal");
		m_shiftPressedTime = 0;

		DWORD conv, sentence;
		ImmGetConversionStatus(hIMC, &conv, &sentence);
		
		if(!(conv & IME_CMODE_NATIVE) && !(conv & IME_CMODE_FULLSHAPE))
			processState = 0; //half-shape alphanumeric mode
		else processState = 2;
	}
	return processState;
}

BOOL ImmController::onTypingEvent
(HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState)
{
	BOOL isProcessed = FALSE;
	
	dsvr->hideNotify();  // hide notify window for any typing event.
	

	ImmModel* model = ImmModel::open(hIMC);
	if(!m_isCompStarted)
	{
		murmur("IMM:STARTCOMPOSITION"); 
		MyGenerateMessage(hIMC, WM_IME_STARTCOMPOSITION, 0, 0);
#ifdef showCompWindow
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, WM_IME_STARTCOMPOSITION);
#endif
		m_isCompStarted = true;

	}


	ImmModel::close();

	int k = LOWORD(uVKey);
	if( k >= 65 && k <= 90)
		k = k + 32;

	switch(LOWORD(uVKey)) {
	case VK_PRIOR: // pageup
		k = 11;
		break;
	case VK_NEXT: // pagedown
		k = 12;
		break;
	case VK_END:
		k = 4;
		break;
	case VK_HOME:
		k = 1;
		break;
	case VK_LEFT:
		k = 28;
		break;
	case VK_UP:
		k = 30;
		break;
	case VK_RIGHT:
		k = 29;
		break;
	case VK_DOWN:
		k = 31;
		break;
	case VK_DELETE:
		k = 127;
		break;
	default:
		//murmur("uVKey: %x, %c\n", LOWORD(uVKey), LOWORD(uVKey));
		break;
	}

#ifdef WINCE
	murmur("uVKey: %c, hex:%x", k, k);
	UINT spec=MapVirtualKeyW(uVKey, 2);
	if(spec) k= (char)spec;
	
	
	if(isShiftPressed(lpbKeyState)) {
		switch(LOWORD(uVKey)) {
			case VK_1:
				k='!';
				break;
			case VK_2:
				k='@';
				break;
			case VK_3:
				k='#';
				break;
			case VK_4:
				k='$';
				break;
			case VK_5:
				k='%';
				break;
			case VK_6:
				k='^';
				break;
			case VK_7:
				k='&';
				break;
			case VK_8:
				k='*';
				break;
			case VK_9:
				k='(';
				break;
			case VK_0:
				k=')';
				break;
			case VK_SEMICOLON:
				k=';';
				break;
			case VK_COMMA:
				k='<';
			case VK_HYPHEN:
				k='_';
				break;
			case VK_EQUAL:
				k='+';
			case VK_PERIOD:
				k='>';
			case VK_SLASH:
				k='?';
				break;
				
			default:
				break;
		}	
	}
	murmur("Mapped Keycode: %c, hex:%x", k, k);

#else
	WORD out[2];
	int spec =
		ToAscii(uVKey, MapVirtualKey(uVKey, 0), lpbKeyState, (LPWORD)&out, 0);
	if(spec == 1) k = (char)out[0];
	murmur("KEY: %c", out[0]);
#endif

	DWORD conv, sentence;	
	AVKeyCode keycode(k);
	ImmGetConversionStatus(hIMC, &conv, &sentence);
	//Force Capslock on Fullshape mode
	if(LOWORD(lpbKeyState[VK_CAPITAL])  || (!(conv & IME_CMODE_NATIVE)&& (conv & IME_CMODE_FULLSHAPE)))
		keycode.setCapslock(1);
	if(!(conv & IME_CMODE_FULLSHAPE)) {
		if(isShiftPressed(lpbKeyState) || LOWORD(uVKey) == VK_SHIFT)
			keycode.setShift(1);
		}
	else if(isShiftPressed(lpbKeyState) !=  !!(LOWORD(lpbKeyState[VK_CAPITAL])))
			keycode.setShift(1);
		
	if(isCtrlPressed(lpbKeyState))
		keycode.setCtrl(1);
	if(isAltPressed(lpbKeyState))
		keycode.setAlt(1);
	if((lpbKeyState[VK_NUMLOCK])
		&& (uVKey >= VK_NUMPAD0)
		&& (uVKey <= VK_DIVIDE))
		keycode.setNum(1);
	
	AVLoader* loader = AVLoader::open();
	murmur("Send to loader->keyevent, currentIC,%d", UICurrentInputMethod());
	if(loader->keyEvent(UICurrentInputMethod(), keycode)) 
	{
		isProcessed = TRUE;
	
	} else {
		
		model = ImmModel::open(hIMC);
		if(wcslen(GETLPCOMPSTR(model->getCompStr())) &&( LOWORD(uVKey) == VK_LEFT ||  LOWORD(uVKey) ==VK_RIGHT) )
			isProcessed = true;
		

		if(m_isCompStarted &&
			wcslen(GETLPCOMPSTR(model->getCompStr())) == 0)
		{
			murmur("IMM:ENDCOMPOSITION");
			m_isCompStarted = false;
			MyGenerateMessage(hIMC,	WM_IME_ENDCOMPOSITION, 0, 0);
#ifdef showCompWindow
			MyGenerateMessage(hIMC,	WM_IME_NOTIFY, IMN_PRIVATE, WM_IME_ENDCOMPOSITION);
#endif
			
		}
		ImmModel::close(); 		
		

	}
	
	

	return isProcessed; 
}
bool ImmController::isWindows8(){
	murmur("Try to get windows version.");
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);

	
	if( ! bOsVersionInfoEx ) return false;
	murmur("windows version: %u.%u\n", osvi.dwMajorVersion, osvi.dwMinorVersion);
	if ( osvi.dwMajorVersion == 6  && osvi.dwMinorVersion == 2)
	  return true;
	else
	  return false;


}