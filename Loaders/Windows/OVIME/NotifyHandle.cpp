//#define OV_DEBUG

#include "OVIME.h" 

static bool isPrivateEightExecuting = false;
static bool inprivate2=false;
static bool inprivate21=false;
static bool inprivate22=false;
static bool inprivate5=false;


BOOL CALLBACK ReloadIME(HWND hwnd, LPARAM lp)
{
	TCHAR tmp[9];
	GetClassName( hwnd, tmp, 8 );

	
	if( ! _tcscmp( tmp, UICLASSNAME ) ) {
		SendMessage( hwnd, WM_IME_RELOADCONFIG, 0, 0 );
	}
	return TRUE;
}

LRESULT NotifyHandle(HIMC hUICurIMC,
				  HWND hWnd,
				  UINT message,
				  WPARAM wParam,
				  LPARAM lParam)
{
	LRESULT lRet = 0L;
	if(!hUICurIMC) return lRet;

	dsvr->setHIMC(hUICurIMC);
	AVLoader* loader = AVLoader::open();
	loader->connectDisplayServer(dsvr);

	switch (wParam)
	{
	case IMN_OPENSTATUSWINDOW:
		//{
		murmur("IMN_OPENSTATUSWINDOW");
		/*murmur("\thwnd=%x", hWnd);		 

		loader->setGlobalConfig("StatusBar");
		UICreateStatusWindow(hWnd, 
			atoi(loader->getGlobalConfigKey("StatusPosX")),
			atoi(loader->getGlobalConfigKey("StatusPosY")),
			!!atoi(loader->getGlobalConfigKey("IsDocked"))
			);

		isChinese = !!atoi(loader->getGlobalConfigKey("isChinese"));
		murmur("\tSwitching to saved  CHI/ENG  mode, isChinese: %d.", isChinese);
		dsvr->showStatus(true);
		SendMessage(hWnd, WM_IME_NOTIFY, IMN_PRIVATE, isChinese?21:22);
		if(isWindows8()&&!isChinese) dsvr->showStatus(false);	//Hide status bar in engilsh mode in win8
		
		
		

		break;
		}*/
	case IMN_SETOPENSTATUS: // the same as IMN_OPENSTATUSWINDOW
		{
		murmur("IMN_SETOPENSTATUS");
		loader->setGlobalConfig("StatusBar");
		UICreateStatusWindow(hWnd, 
			atoi(loader->getGlobalConfigKey("StatusPosX")), 
			atoi(loader->getGlobalConfigKey("StatusPosY")),
			!!atoi(loader->getGlobalConfigKey("IsDocked"))
			);
		
		dsvr->showStatus(true);
		
		
		if(isWindows8()){
			//isChinese = !!atoi(loader->getGlobalConfigKey("isChinese"));
			murmur("\tSwitching to saved  CHI/ENG  mode, isChinese: %d, and isFullshape: %d", isChinese, isFullShape);
			SendMessage(hWnd, WM_IME_NOTIFY, IMN_PRIVATE, isChinese?21:22);
			if(!isChinese && &isFullShape){
				dsvr->showStatus(false);	//Hide status bar in engilsh mode in win8
				DWORD conv, sentence;
				ImmGetConversionStatus(hUICurIMC, &conv, &sentence);
				isFullShape = (conv & IME_CMODE_FULLSHAPE) == 0 ? false : true;
				if(isFullShape){  // force switch to half-shape when statusbar is not shown (for windows 8).
					murmur("\t Going to hide Statusbar! Switch back to half-shape");
					conv &= ~IME_CMODE_FULLSHAPE;									
					ImmSetConversionStatus( hUICurIMC, conv, sentence);
				}
			}
			
		}
		
		
		
		
		
		break;
		}
	case IMN_CLOSESTATUSWINDOW:
		{
		murmur("IMN_CLOSESTATUSWINDOW");
		dsvr->showStatus(false);
		loader->setGlobalConfig("StatusBar");
		//loader->setGlobalConfigKey("isChinese", isChinese?"1":"0"); //save chi/eng mode to global dictionary.no longer used.
		break;
		}
	case IMN_OPENCANDIDATE:
		murmur("IMN_OPENCANDIDATE");
		dsvr->SetCandiEnabled(true);
		dsvr->showCandi(true);		
		break;

	case IMN_CLOSECANDIDATE:
		murmur("IMN_CLOSECANDIDATE");
		dsvr->showCandi(false);		
		break;

	case IMN_CHANGECANDIDATE:
		dsvr->SetCandiEnabled(true);
		murmur("IMN_CHANGECANDIDATE");
		break;

	case IMN_SETCONVERSIONMODE:
		murmur("IMN_SETCONVERSIONMODE");
		DWORD conv, sentence;
		ImmGetConversionStatus(hUICurIMC, &conv, &sentence);
		isChinese = (conv & IME_CMODE_NATIVE) == 0 ? false : true;
		isFullShape = (conv & IME_CMODE_FULLSHAPE) == 0 ? false : true;

		if(isFullShape && !dsvr->getStatusEnabled()){  // force switch to half-shape when statusbar is not shown (for windows 8).
			murmur("\t Statusbar not shown!! Switch back to half-shape");
			conv &= ~IME_CMODE_FULLSHAPE;									
			ImmSetConversionStatus( hUICurIMC, conv, sentence);
		}

		if( isFullShape ) UISetFull();
		else	UISetHalf();
		//ImmSetConversionStatus( hUICurIMC, conv, sentence);
		//murmur("\t");
		murmur("\tIsChinese:%i; IsFullShape:%i", isChinese, isFullShape);
		break;  

	case IMN_SETSENTENCEMODE:
		murmur("IMN_SETSENTENCEMODE");
		break;

	case IMN_SETCANDIDATEPOS: // For the Search Text Field of Firefox
		{
		murmur("IMN_SETCANDIDATEPOS"); 
		//dsvr->SetCompEnabled(true);  
		dsvr->SetCandiEnabled(true);
		murmur("\tRefreshUI()"); 
		RefreshUI(hUICurIMC);  
		//dsvr->moveCandi(CandX,CandY);
		//not finished!!
		break;
		}
 
	case IMN_SETCOMPOSITIONFONT: 
		{
		murmur("IMN_SETCOMPOSITIONFONT");
		dsvr->SetCompEnabled(true); 
		//dsvr->SetCandiEnabled(true);
		murmur("\tRefreshUI()"); 
		RefreshUI(hUICurIMC);  
		//dsvr->showStatus(true);
		//<comment author='b6s'>
		// It is weird but... when the attached app got focus back,
		// this message occurred.
		//</comment>
	
		break;
		}
	case IMN_SETCOMPOSITIONWINDOW:
		{
		/*	Sent immediately before the IME generates 
			the composition string as a result of a keystroke.
			This message is a notification to an IME window to 
			open its composition window. An application should process
			this message if it displays composition characters itself. */
		murmur("IMN_SETCOMPOSITIONWINDOW");
		dsvr->SetCompEnabled(true);		
		murmur("\tRefreshUI()"); 
		RefreshUI(hUICurIMC);  
		break;
		}
	case IMN_GUIDELINE:
		murmur("IMN_GUIDELINE");
		break;

	case IMN_SETSTATUSWINDOWPOS:
		murmur("IMN_SETSTATUSWINDOWPOS");
		//		MoveStatusWindow(hWnd, );
		break;

	case IMN_PRIVATE:
		{
		murmur("IMN_PRIVATE");
		murmur("\thwnd=%x", hWnd);
		murmur("lParam=%i", lParam);
		switch(lParam) 
		{
		case 0:
			break;
		case 1: //Change UI Half/Full from key: shift+space
			{
			murmur("\tChange UI Half/Full.");
			//UIChangeHalfFull();   //move to IMN_SETCONVERSIONMODE
			 
			break;
			}

		case 2:  //Switch between CHI/ENG mode
		{
			if(!inprivate2){
				inprivate2=true;
				murmur("\tSwitch  CHI/ENG  mode, current isChinese: %d, and isFullshape: %d", isChinese, isFullShape);
				ImmModel* model = ImmModel::open(hUICurIMC); // force commit before changing 
				if(wcslen(GETLPCOMPSTR(model->getCompStr())) )
					loader->unloadCurrentModule();
				model->close();
			
				UISetChiEng();  

				if( hUICurIMC )
				{
					isChinese=!isChinese;

					DWORD conv, sentence;
					ImmGetConversionStatus( hUICurIMC, &conv, &sentence);
					if( isChinese ){
						conv |= IME_CMODE_NATIVE;
					}
					else{
						conv &= ~IME_CMODE_NATIVE;	
					}
					ImmSetConversionStatus( hUICurIMC, conv, sentence);
				}				
				murmur("\tnew isChinese: %d.", isChinese);
				//loader->setGlobalConfig("StatusBar");
				//loader->setGlobalConfigKey("isChinese", isChinese?"1":"0"); //save chi/eng mode to global dictionary
				if(isChinese&&!dsvr->getStatusEnabled()) dsvr->showStatus(true);

				inprivate2=false;
			}

			break;
		}

		case 21:  //Switch to Chinese mode
		{
			if(!inprivate21){
				inprivate21=true;
				murmur("\tSwitch to Chinese mode.");
			
				ImmModel* model = ImmModel::open(hUICurIMC); // force commit before changing 
				if(wcslen(GETLPCOMPSTR(model->getCompStr())) )
					loader->unloadCurrentModule();
				model->close();
			
				UISetChinese();  

				if( hUICurIMC )
				{
					isChinese=true;
					DWORD conv, sentence;
					ImmGetConversionStatus( hUICurIMC, &conv, &sentence);
					ImmSetConversionStatus( hUICurIMC, conv | IME_CMODE_NATIVE, sentence);
				}				
				//should not save the isChinese to config, cause looping here!!
				//loader->setGlobalConfig("StatusBar");   
				//loader->setGlobalConfigKey("isChinese", "1"); //save chi/eng mode to global dictionary
				if(isChinese&&!dsvr->getStatusEnabled()) dsvr->showStatus(true);
				inprivate21=false;
			}
			break;
		}

		case 22:  //Switch to English mode
		{
			if(!inprivate22){
				inprivate22=true;
				murmur("\tSwitch to English mode.");
			
				ImmModel* model = ImmModel::open(hUICurIMC); // force commit before changing 
				if(wcslen(GETLPCOMPSTR(model->getCompStr())) )
					loader->unloadCurrentModule();
				model->close();
			
				UISetEnglish();  

				if( hUICurIMC )
				{
					isChinese=false;
					DWORD conv, sentence;
					ImmGetConversionStatus( hUICurIMC, &conv, &sentence);
					ImmSetConversionStatus( hUICurIMC, conv & ~IME_CMODE_NATIVE, sentence);
				}				
				//should not save the isChinese to config, cause looping here!!
				//loader->setGlobalConfig("StatusBar");   
				//loader->setGlobalConfigKey("isChinese", "0"); //save chi/eng mode to global dictionary
				inprivate22=false;
				}
			break;
		}


		case 3: //Switch InputMethod From Menu
			{
			murmur("\tChange Modules by Mouse");
			/* close module and set new IC */

			loader->unloadCurrentModule();
			int moduleId = UIModuleChange();
			UISetStatusModStrCurrent(moduleId);
			loader->syncConfigToMenu(moduleId);
			//loader->initContext(moduleId);
			break;
			}
		case 4: // Change Traditional/Simplified Chinese ; Deprecated
			//murmur("\tChange UI Traditional/Simplified Chinese");			
			//UIChangeSimpifiedOrTraditional(hWnd); 
			break;
		case 5: { //Change BoPoMoFo keyboard layout by ctrl +'='
			/* close module and set new IC */	
			if(!inprivate5)
			{
				inprivate5 = true;
				int moduleId = loader->getSwitchedBoPoMoFoLayoutModIndex(UICurrentInputMethod());
				murmur("\tChange BoPoMoFo keyboard layout by ctrl+=. New moduleId:%d", moduleId);
				if(moduleId != UICurrentInputMethod()) {
					loader->unloadCurrentModule();
					UISetStatusModStrCurrent(moduleId);
					loader->syncConfigToMenu( UIModuleChange());
					//loader->initContext(moduleId);	
				}
				inprivate5 =false;
			}
			break;
			}
		case 6: // Test Notify. (ctrl+alt+k)	
			{
			murmur("\tToggle Small/Large Candidate window.");
			murmur("\tTest Notify window");
			char *str="Hide Deskband";			
			
			
			dsvr->showNotify(str); 

			break;
			}
		
		case 8: //Rotate Modules by ctrl +'\'
			{
			
				if(!isPrivateEightExecuting) {
					isPrivateEightExecuting = true;
					loader->unloadCurrentModule();
					int moduleId = UIModuleRotate();					
					murmur("Ctrl+ \\; Switch IM to module:%d", moduleId);
					UISetStatusModStrCurrent(moduleId);
					loader->syncConfigToMenu(moduleId);
					//loader->initContext(moduleId);
					isPrivateEightExecuting = false; 
				}

			break;
			}
		case 9: //Set all module names
			{			

			int modAmount = loader->getInputMethodCount() +loader->getKeypreprocessorCount() + loader->getOutputFilterCount();
			murmur("Loading module names to status bar:%d modules", modAmount);
			const char** modNameList = new const char* [modAmount];

			loader->getAllModuleNames(modNameList);
			
			for(int i=0;i<modAmount; i++)
				murmur("module %d: %s", i, modNameList[i]);

			UISetkpCount(loader->getKeypreprocessorCount());
			UISetofCount(loader->getOutputFilterCount());
			UISetimCount(loader->getInputMethodCount());

			UISetStatusModStrMenuAll(modAmount, modNameList);
			UISetStatusModStrCurrent(UICurrentInputMethod());
			loader->syncConfigToMenu(UICurrentInputMethod() );//UIModuleChange());  // update current IC.
			

			//dsvr->showStatus(true);
			
			
			break;
			}
		case 10: // Call OVpreference and send reload to all OV instances after process ended.
			{
			murmur("Shell execute OVpreference if exist");
								
			TCHAR dir[MAX_PATH], file[MAX_PATH];
			
			mbstowcs(dir,loader->getBaseDirectory(),MAX_PATH);

			_tcscpy(file,dir);
			_tcscat(file,_T("OVPreferences.exe"));
				
			char buff[MAX_PATH];
			wcstombs(buff,file, MAX_PATH);
			murmur("OVpreference:%s",buff);

				//if(PathFileExists(buf)){ 
			if(!_waccess(file,0)){
				SHELLEXECUTEINFO ShExecInfo = {0};
				ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
				ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
				ShExecInfo.hwnd = NULL;
				ShExecInfo.lpVerb = NULL;
				ShExecInfo.lpFile = file;		
				ShExecInfo.lpParameters = _T("");	
				ShExecInfo.lpDirectory = dir;
				ShExecInfo.nShow = SW_SHOWNORMAL;
				ShExecInfo.hInstApp = NULL;	
				ShellExecuteEx(&ShExecInfo);
				WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

				murmur("send WM_IME_RELOADCONFIG to all windows. My Hwnd:%x", hWnd);
				EnumChildWindows( GetDesktopWindow(), ReloadIME, NULL);

			}

			break;

			}
		case 11: // Status bar loaction updated.
			{
			POINT pt;

			loader->setGlobalConfig("StatusBar");
			dsvr->getStatusPos(&pt);
			if((pt.x!=-1)&&(pt.y!=-1)) {
				murmur("Status location changed notification from UI: Save new position : (%i, %i).", pt.x, pt.y);
				char tmp[32]; 
				sprintf(tmp, "%d", pt.x);
				loader->setGlobalConfigKey("StatusPosX",tmp);
				sprintf(tmp, "%d", pt.y);
				loader->setGlobalConfigKey("StatusPosY",tmp);
				//sprintf(tmp, "%d", dsvr->GetisStatusDocked());
				//loader->setGlobalConfigKey("IsDocked",tmp);
				
			}	  	
			
			break;
			}
		case 12: // Select Candi item with mouse.
			{
			AVKeyCode k;
			k.setCode(UIGetCandSelectedItemKey());
			
			loader->keyEvent(UICurrentInputMethod(), k);

			break;
			}
		case 13: // KP OF checked status updated.
			{
			murmur("Sync KP OF checked status to config.");
			loader->syncMenuToConfig(UICurrentInputMethod());

			murmur("send WM_IME_RELOADCONFIG to all windows. My Hwnd:%x", hWnd);
			EnumChildWindows( GetDesktopWindow(), ReloadIME, NULL);
			break;
			}
	
		
		default:
			murmur("\tUknown IMN_PRIVATE");
			break;
		}
		break;
		}
	default:
		murmur("WM_IME_NOTIFY: no this wParam");
		break;
	}

	return lRet;
}

