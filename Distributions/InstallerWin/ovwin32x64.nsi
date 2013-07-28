; Script generated by the HM NIS Edit Script Wizard.

!include "Registry.nsh"
;!include "LogicLib.nsh"
!include x64.nsh

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "OpenVanilla"
!define PRODUCT_VERSION "0.9b"
!define PRODUCT_PUBLISHER "OpenVanilla.org"
!define PRODUCT_WEB_SITE "http://openvanilla.org/"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
; ## HKLM = HKEY_LOCAL_MACHINE
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define IME_ROOT_KEY "HKLM"
; ## HKCU = HKEY_CURRENT_USER
!define IME_CURRENT_USER "HKCU"
!define IME_KEY "SYSTEM\CurrentControlSet\Control\Keyboard Layouts"
!define IME_KEY_USER "Keyboard Layout\Preload\"

SetCompressor lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
!define MUI_HEADERIMAGE 
!define MUI_HEADERIMAGE_NOSTRETCH
!define MUI_HEADERIMAGE_BITMAP "ov-installer2.bmp"
;!define MUI_WELCOMEFINISHPAGE_BITMAP_NOSTRETCH
!define MUI_WELCOMEFINISHPAGE_BITMAP "ov-installer.bmp"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "LICENSE-zh-Hant.rtf"
; Directory page
;!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
;!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "TradChinese"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

; .NET Framework 4.0 testing/installing
; .NET start -----------------------------------


; .net framework version v4.0.30319 here
!define DOT_MAJOR "4"
!define DOT_MINOR "0"
!define DOT_MINOR_MINOR "30319"

!define BASE_URL http://download.microsoft.com/download
;.net Framework 4.0
!define URL_DOTNET_30319  "${BASE_URL}/9/5/A/95A9616B-7A37-4AF6-BC36-D6EA96C8DAAE/dotNetFx40_Full_x86_x64.exe"
!define URL_VC_REDISTX64_2012U3  "${BASE_URL}/1/6/B/16B06F60-3B20-4FF2-B699-5E9B7962F9AE/VSU3/vcredist_x64.exe"
!define URL_VC_REDISTX86_2012U3  "${BASE_URL}/1/6/B/16B06F60-3B20-4FF2-B699-5E9B7962F9AE/VSU3/vcredist_x86.exe"

; Variables
;Var "LANGUAGE_DLL_TITLE"
;Var "LANGUAGE_DLL_INFO"
Var "URL_DOTNET"
Var "OSLANGUAGE"
Var "DOTNET_RETURN_CODE"
Var "URL_VCX86"
Var "URL_VCX64"

; Language Strings
LangString DESC_REMAINING ${LANG_TradChinese} " ( 剩餘 %d %s%s )"
LangString DESC_PROGRESS ${LANG_TradChinese} "%d.%01dkB" ;"%dkB (%d%%) of %dkB @ %d.%01dkB/s"
LangString DESC_PLURAL ${LANG_TradChinese} " "
LangString DESC_HOUR ${LANG_TradChinese} "小時"
LangString DESC_MINUTE ${LANG_TradChinese} "分鐘"
LangString DESC_SECOND ${LANG_TradChinese} "秒"
LangString DESC_CONNECTING ${LANG_TradChinese} "連接中..."
LangString DESC_DOWNLOADING ${LANG_TradChinese} "下載 %s"
LangString DESC_SHORTDOTNET ${LANG_TradChinese} ".Net Framework 4.0"
LangString DESC_LONGDOTNET ${LANG_TradChinese} "Microsoft .Net Framework 4.0"
LangString DESC_DOTNET_DECISION ${LANG_TradChinese} "安裝此輸入法之前，必須先安裝 $(DESC_SHORTDOTNET) $\n強烈建議您先安裝完 \
  $(DESC_SHORTDOTNET)$\n再繼續進行安裝輸入法。$\n若你想繼續安裝 \
  ，您的電腦必須連接網路。$\n您要繼續這項安裝嗎？"
LangString SEC_DOTNET ${LANG_TradChinese} "$(DESC_SHORTDOTNET) "
LangString DESC_INSTALLING ${LANG_TradChinese} "安裝中"
LangString DESC_DOWNLOADING1 ${LANG_TradChinese} "下載中"
LangString DESC_DOWNLOADFAILED ${LANG_TradChinese} "下載失敗:"
LangString ERROR_DOTNET_DUPLICATE_INSTANCE ${LANG_TradChinese} "已有另一個 $(DESC_SHORTDOTNET) 啟動安裝程序"
LangString ERROR_NOT_ADMINISTRATOR ${LANG_TradChinese} "請將您的權限提升至電腦系統管理員，謝謝"
LangString ERROR_INVALID_PLATFORM ${LANG_TradChinese} "此產品並不適用於這個平台"
LangString DESC_DOTNET_TIMEOUT ${LANG_TradChinese} " $(DESC_SHORTDOTNET) 安裝已逾時"
LangString ERROR_DOTNET_INVALID_PATH ${LANG_TradChinese} "$(DESC_SHORTDOTNET) 安裝檔 \
  並未在以下路徑:$\n"
LangString ERROR_DOTNET_FATAL ${LANG_TradChinese} "嚴重錯誤訊息發生在安裝 \
  $(DESC_SHORTDOTNET) 過程當中"
LangString FAILED_DOTNET_INSTALL  ${LANG_TradChinese} "$(DESC_SHORTDOTNET) 安裝失敗"
; VC redistributble....
LangString DESC_VCX86 ${LANG_TradChinese} "Visual C++ 2012 Redistritable Update 3 x86"
LangString DESC_VCX64 ${LANG_TradChinese} "Visual C++ 2012 Redistritable Update 3 x64"
LangString DESC_VCX86_DECISION ${LANG_TradChinese} "安裝此輸入法之前，必須先安裝 $(DESC_VCX86)，若你想繼續安裝 \
  ，您的電腦必須連接網路。$\n您要繼續這項安裝嗎？"
LangString DESC_VCX64_DECISION ${LANG_TradChinese} "安裝此輸入法之前，必須先安裝 $(DESC_VCX64)，若你想繼續安裝 \
  ，您的電腦必須連接網路。$\n您要繼續這項安裝嗎？"

; .NET end --------------------------------------------
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "OpenVanilla-Windows-0.9b-x64.exe"
InstallDir "$PROGRAMFILES64\OpenVanilla"
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
 ${If} ${RunningX64}
        ${EnableX64FSRedirection}
        ${else}
        MessageBox MB_OK "此安裝檔為64bit版本, 請重新下載32bit版本"
        Abort
  ${EndIf}
  ReadRegStr $0 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion"
  StrCmp $0 "" StartInstall 0
;  	  MessageBox MB_OK  "偵測到舊版 $0，必須先移除才能安裝新版。"
    MessageBox MB_OKCANCEL|MB_ICONQUESTION "偵測到舊版 $0，必須先移除才能安裝新版。是否要現在進行？" IDOK +2
    	Abort
   ExecWait '"$INSTDIR\uninst.exe" /S _?=$INSTDIR'
    ${DisableX64FSRedirection}
   IfFileExists "$SYSDIR\OVIME.ime"  0 CheckX64     ;代表反安裝失敗 
   	Abort
  CheckX64:
  ${EnableX64FSRedirection}
   IfFileExists "$SYSDIR\OVIME.ime"  0 RemoveFinished     ;代表反安裝失敗 
        Abort
    RemoveFinished:     
    		MessageBox MB_ICONINFORMATION|MB_OK "舊版已移除。"
    StartInstall:     
;  !insertmacro MUI_LANGDLL_DISPLAY

  StrCpy $URL_VCX86 "${URL_VC_REDISTX86_2012U3}"
  StrCpy $URL_VCX64 "${URL_VC_REDISTX64_2012U3}"
;DOTNET start --------------------------------------------
  StrCpy $URL_DOTNET "${URL_DOTNET_30319}"
  StrCpy $OSLANGUAGE "30319"
  InitPluginsDir

;DOTNET end ----------------------------------------------    

FunctionEnd

; Call IsDotNetInstalledAdv
; This function will abort the installation if the required version 
; or higher version of the .NET Framework is not installed.  Place it in
; either your .onInit function or your first install section before 
; other code.
Function IsDotNetInstalledAdv
   Push $0
   Push $1
   Push $2
   Push $3
   Push $4
   Push $5
 
  StrCpy $0 "0"
  StrCpy $1 "SOFTWARE\Microsoft\.NETFramework" ;registry entry to look in.
  StrCpy $2 0
 
  StartEnum:
    ;Enumerate the versions installed.
    EnumRegKey $3 HKLM "$1\policy" $2
 
    ;If we don't find any versions installed, it's not here.
    StrCmp $3 "" noDotNet notEmpty
 
    ;We found something.
    notEmpty:
      ;Find out if the RegKey starts with 'v'.  
      ;If it doesn't, goto the next key.
      StrCpy $4 $3 1 0
      StrCmp $4 "v" +1 goNext
      StrCpy $4 $3 1 1
 
      ;It starts with 'v'.  Now check to see how the installed major version
      ;relates to our required major version.
      ;If it's equal check the minor version, if it's greater, 
      ;we found a good RegKey.
      IntCmp $4 ${DOT_MAJOR} +1 goNext yesDotNet
      ;Check the minor version.  If it's equal or greater to our requested 
      ;version then we're good.
      StrCpy $4 $3 1 3
      IntCmp $4 ${DOT_MINOR} +1 goNext yesDotNet
 
      ;detect sub-version - e.g. 2.0.50727
      ;takes a value of the registry subkey - it contains the small version number
      EnumRegValue $5 HKLM "$1\policy\$3" 0
 
      IntCmpU $5 ${DOT_MINOR_MINOR} yesDotNet goNext yesDotNet
 
    goNext:
      ;Go to the next RegKey.
      IntOp $2 $2 + 1
      goto StartEnum
 
 ; yesDotNetReg: 
    ;Now that we've found a good RegKey, let's make sure it's actually
    ;installed by getting the install path and checking to see if the 
    ;mscorlib.dll exists.
    ;EnumRegValue $2 HKLM "$1\policy\$3" 0
    ;$2 should equal whatever comes after the major and minor versions 
    ;(ie, v1.1.4322)
    ;StrCmp $2 "" noDotNet
    ;ReadRegStr $4 HKLM $1 "InstallRoot"
    ;Hopefully the install root isn't empty.
    ;StrCmp $4 "" noDotNet
    ;build the actuall directory path to mscorlib.dll.
    ;StrCpy $4 "$4$3.$2\mscorlib.dll"
    ;IfFileExists $4 yesDotNet noDotNet
    ;Goto yesDotNet
    
  noDotNet:
    ;Nope, something went wrong along the way.  Looks like the 
    ;proper .NET Framework isn't installed.  
 
    ;Uncomment the following line to make this function throw a message box right away 
    ;MessageBox MB_OK "You must have v${DOT_MAJOR}.${DOT_MINOR}.${DOT_MINOR_MINOR} or greater of the .NET Framework installed. "
   ; Abort
     StrCpy $0 0
     Goto done
 
     yesDotNet:
    ;Everything checks out.  Go on with the rest of the installation.
    StrCpy $0 1
 
   done:
     Pop $4
     Pop $3
     Pop $2
     Pop $1
     Exch $0
 FunctionEnd

 Section $(SEC_DOTNET) SECDOTNET
    SectionIn RO
    IfSilent lbl_IsSilent
      
    StrCpy $DOTNET_RETURN_CODE "0"

    call IsDotNetInstalledAdv
      Pop $0
    ${If} $0 == 0
    	Goto lbl_DownloadRequired
    ${ENDIF}
    
    Goto lbl_NoDownloadRequired
    
    lbl_DownloadRequired:
    DetailPrint "$(DESC_DOWNLOADING1) $(DESC_SHORTDOTNET)..."
    MessageBox MB_ICONEXCLAMATION|MB_YESNO|MB_DEFBUTTON2 "$(DESC_DOTNET_DECISION)" /SD IDNO \
      IDYES +2 IDNO 0
    Abort
    ; "Downloading Microsoft .Net Framework"
    AddSize 153600
    nsisdl::download /TRANSLATE "$(DESC_DOWNLOADING)" "$(DESC_CONNECTING)" \
       "$(DESC_SECOND)" "$(DESC_MINUTE)" "$(DESC_HOUR)" "$(DESC_PLURAL)" \
       "$(DESC_PROGRESS)" "$(DESC_REMAINING)" \
       /TIMEOUT=30000 "$URL_DOTNET" "$PLUGINSDIR\dotnetfx.exe"
    Pop $0
    StrCmp "$0" "success" lbl_continue
    DetailPrint "$(DESC_DOWNLOADFAILED) $0"
    Abort
 
    lbl_continue:
      DetailPrint "$(DESC_INSTALLING) $(DESC_SHORTDOTNET)..."
      ;Banner::show /NOUNLOAD "$(DESC_INSTALLING) $(DESC_SHORTDOTNET)..."
      ;nsExec::ExecToStack '"$PLUGINSDIR\dotnetfx.exe" /q /c:"install.exe /noaspupgrade /q"'
      nsExec::ExecToStack "$PLUGINSDIR\dotnetfx.exe /q"
      pop $DOTNET_RETURN_CODE
      ;MessageBox MB_ICONINFORMATION|MB_OK "show dotnet return code:$DOTNET_RETURN_CODE" IDOK +1
      ;Banner::destroy
      ;SetRebootFlag true
      ; silence the compiler
      Goto lbl_NoDownloadRequired
    
      lbl_NoDownloadRequired:
 
      ; obtain any error code and inform the user ($DOTNET_RETURN_CODE)
      ; If nsExec is unable to execute the process,
      ; it will return "error"
      ; If the process timed out it will return "timeout"
      ; else it will return the return code from the executed process.
      StrCmp "$DOTNET_RETURN_CODE" "" lbl_NoError
      StrCmp "$DOTNET_RETURN_CODE" "0" lbl_NoError
      StrCmp "$DOTNET_RETURN_CODE" "3010" lbl_reboot ;indicate success with a reboot required
      StrCmp "$DOTNET_RETURN_CODE" "8192" lbl_reboot ;reboot is required
      StrCmp "$DOTNET_RETURN_CODE" "error" lbl_Error
      StrCmp "$DOTNET_RETURN_CODE" "timeout" lbl_TimeOut
      ; It's a .Net Error
      StrCmp "$DOTNET_RETURN_CODE" "4097" lbl_Error_NotAdministrator
      StrCmp "$DOTNET_RETURN_CODE" "4098" lbl_Error_InstallMSIFailed ; need new step
      StrCmp "$DOTNET_RETURN_CODE" "4099" lbl_Error_WInstallerNotProper ; need new step
      StrCmp "$DOTNET_RETURN_CODE" "4100" lbl_Error
      StrCmp "$DOTNET_RETURN_CODE" "4101" lbl_Error_DuplicateInstance
      StrCmp "$DOTNET_RETURN_CODE" "4102" lbl_Error
      StrCmp "$DOTNET_RETURN_CODE" "4103" lbl_Error
      StrCmp "$DOTNET_RETURN_CODE" "4111" lbl_Error
      StrCmp "$DOTNET_RETURN_CODE" "4113" lbl_Error_BetaRemove ; need new step
      StrCmp "$DOTNET_RETURN_CODE" "4115" lbl_Error ; temp path is too long
      StrCmp "$DOTNET_RETURN_CODE" "4116" lbl_Error ; the source path is too long
      StrCmp "$DOTNET_RETURN_CODE" "4118" lbl_Error ; fail to create log file
      StrCmp "$DOTNET_RETURN_CODE" "4119" lbl_Error
      StrCmp "$DOTNET_RETURN_CODE" "4120" lbl_Error
      StrCmp "$DOTNET_RETURN_CODE" "4121" lbl_Error
      StrCmp "$DOTNET_RETURN_CODE" "4122" lbl_Error
      StrCmp "$DOTNET_RETURN_CODE" "4123" lbl_Error ; already in os component like vista
      StrCmp "$DOTNET_RETURN_CODE" "4124" lbl_Error
      StrCmp "$DOTNET_RETURN_CODE" "8191" lbl_Error
      StrCmp "$DOTNET_RETURN_CODE" "1602" lbl_Error_UserCancel ; need new step
      
      
      StrCmp "$DOTNET_RETURN_CODE" "1633" lbl_Error_InvalidPlatform lbl_FatalError
      ; all others are fatal
 
    lbl_Error_InstallMSIFailed:
    lbl_Error_WInstallerNotProper:
    DetailPrint "Windows Installer 安裝失敗"
    GoTo lbl_Done
    
    lbl_Error_BetaRemove:
    DetailPrint "必須先移除 .Net Framework Beta 版"
    GoTo lbl_Done
    
    lbl_Error_UserCancel:
    DetailPrint "使用者自行取消"
    GoTo lbl_Done
    
    lbl_Error_DuplicateInstance:
    DetailPrint "$(ERROR_DOTNET_DUPLICATE_INSTANCE)"
    GoTo lbl_Done
 
    lbl_Error_NotAdministrator:
    DetailPrint "$(ERROR_NOT_ADMINISTRATOR)"
    GoTo lbl_Done
 
    lbl_Error_InvalidPlatform:
    DetailPrint "$(ERROR_INVALID_PLATFORM)"
    GoTo lbl_Done
 
    lbl_TimeOut:
    DetailPrint "$(DESC_DOTNET_TIMEOUT)"
    GoTo lbl_Done
 
    lbl_Error:
    DetailPrint "$(ERROR_DOTNET_INVALID_PATH)"
    GoTo lbl_Done
 
    lbl_FatalError:
    DetailPrint "$(ERROR_DOTNET_FATAL)[$DOTNET_RETURN_CODE]"
    ;GoTo lbl_Done
        
    lbl_reboot:
    DetailPrint "  請重開機後繼續安裝"
    MessageBox MB_ICONQUESTION|MB_YESNO "需要開機後繼續安裝，您要繼續這項安裝嗎?" IDNO lbrebootAbort
    reboot
 lbrebootAbort:
    Abort

    lbl_Done:
    DetailPrint "$(FAILED_DOTNET_INSTALL)"
    Abort
    
    lbl_NoError:
    lbl_IsSilent: 
  SectionEnd

;!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
;!insertmacro MUI_DESCRIPTION_TEXT ${SECDOTNET} $(DESC_LONGDOTNET)
;!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section  "CheckVCRedist" VCR
  Push $R0
  ;{3D6AD258-61EA-35F5-812C-B7A02152996E} for x86 VC 2012 Upate3
  ;{2EDC2FA3-1F34-34E5-9085-588C9EFD1CC6} for x64 VC 2012 Upate3
  SetRegView 64
  ClearErrors
  ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{2EDC2FA3-1F34-34E5-9085-588C9EFD1CC6}" "Version"
  IfErrors 0 VCx64RedistInstalled
  AddSize 14000
  MessageBox MB_ICONEXCLAMATION|MB_YESNO|MB_DEFBUTTON2 "$(DESC_VCX64_DECISION)" /SD IDNO IDYES +1 IDNO VCRedistInstalledAbort
  nsisdl::download /TRANSLATE "$(DESC_DOWNLOADING)" "$(DESC_CONNECTING)" \
       "$(DESC_SECOND)" "$(DESC_MINUTE)" "$(DESC_HOUR)" "$(DESC_PLURAL)" \
       "$(DESC_PROGRESS)" "$(DESC_REMAINING)" \
       /TIMEOUT=30000 "$URL_VCX64" "$PLUGINSDIR\vcredist_x64.exe"
    Pop $0
    StrCmp "$0" "success" lbl_continue64
    DetailPrint "$(DESC_DOWNLOADFAILED) $0"
    Abort
 
    lbl_continue64:
      DetailPrint "$(DESC_INSTALLING) $(DESC_VCX64)..."
      nsExec::ExecToStack "$PLUGINSDIR\vcredist_x64.exe /q"
      ;pop $DOTNET_RETURN_CODE
  
  ;SetOutPath $INSTDIR
  ;File "VCRedist\vcredist_x64.exe"
  ;ExecWait '"$INSTDIR\vcredist_x64.exe" /q' # silent install
VCx64RedistInstalled:
 SetRegView 32
 ClearErrors
  ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{3D6AD258-61EA-35F5-812C-B7A02152996E}" "Version"
  IfErrors 0 VCRedistInstalled
  ;MessageBox MB_ICONQUESTION|MB_YESNO "需要 MS VC++ 2012 x86 Redistributable，您要繼續這項安裝嗎?" IDNO VCRedistInstalledAbort
  MessageBox MB_ICONEXCLAMATION|MB_YESNO|MB_DEFBUTTON2 "$(DESC_VCX86_DECISION)" /SD IDNO IDYES +1 IDNO VCRedistInstalledAbort
  nsisdl::download /TRANSLATE "$(DESC_DOWNLOADING)" "$(DESC_CONNECTING)" \
       "$(DESC_SECOND)" "$(DESC_MINUTE)" "$(DESC_HOUR)" "$(DESC_PLURAL)" \
       "$(DESC_PROGRESS)" "$(DESC_REMAINING)" \
       /TIMEOUT=30000 "$URL_VCX86" "$PLUGINSDIR\vcredist_x86.exe"
    Pop $0
    StrCmp "$0" "success" lbl_continue
    DetailPrint "$(DESC_DOWNLOADFAILED) $0"
    Abort
 
    lbl_continue:
      DetailPrint "$(DESC_INSTALLING) $(DESC_VCX86)..."
      nsExec::ExecToStack "$PLUGINSDIR\vcredist_x86.exe /q"
      ;pop $DOTNET_RETURN_CODE
  
  ;File "VCRedist\vcredist_x86.exe"
 ; ExecWait '"$INSTDIR\vcredist_x86.exe" /q' # silent install
  Goto VCRedistInstalled
VCRedistInstalledAbort:
  Quit
VCRedistInstalled:
  Exch $R0
SectionEnd

Section "MainSection" SEC01
  SetOutPath "$SYSDIR"
  SetOverwrite ifnewer
  ${DisableX64FSRedirection}
  File "system32.x64\OVIME.ime"
  File "system32.x64\*.dll"
  ${EnableX64FSRedirection}
  File "system32.x86\OVIME.ime"
  File "system32.x86\*.dll"
  
SectionEnd

Section "Modules" SEC02
SetOutPath $PROGRAMFILES64
  SetOVerwrite ifnewer
  File /r /x ".svn" "OpenVanilla"
  nsExec::ExecToStack '"$PROGRAMFILES64\Openvanilla\OVUtil.exe" uninstall "$PROGRAMFILES64\Openvanilla\OVUIServer.dll"'
  nsExec::ExecToStack '"$PROGRAMFILES64\Openvanilla\OVUtil.exe" install "$PROGRAMFILES64\Openvanilla\OVUIServer.dll"'
;SetOutPath "$APPDATA\OpenVanilla\"
;  File "config.xml"

SectionEnd

Section -AdditionalIcons
  SetOutPath $SMPROGRAMS\OpenVanilla
  CreateDirectory "$SMPROGRAMS\OpenVanilla"
  CreateShortCut "$SMPROGRAMS\OpenVanilla\Uninstall.lnk" "$PROGRAMFILES64\Openvanilla\uninst.exe"
  CreateShortCut "$SMPROGRAMS\OpenVanilla\OVPreferences.lnk" "$PROGRAMFILES64\Openvanilla\OVPreferences.exe"
SectionEnd

Section -Post
  WriteUninstaller "$PROGRAMFILES64\OpenVanilla\uninst.exe"
  ${If} ${RunningX64}
  	SetRegView 64
  ${EndIf}
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$PROGRAMFILES64\OpenVanilla\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "${PRODUCT_NAME}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$SYSDIR\OVIME.IME"
  WriteRegDWORD ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "EstimatedSize" 16931
  ;Call dotNet2AppachKeyIns
  System::Call 'imm32::ImmInstallIME(t "$SYSDIR\OVIME.ime", t "$(^Name)")'
  ${registry::Open} "${IME_ROOT_KEY}\${IME_KEY}" "/N='OVIME.ime' /G=1 /T=REG_SZ" $0
  ${registry::Find} $0 $1 $2 $3 $4
  StrCpy $6 $1 "" -8
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Key" "$6"
  ${registry::Close} "$0"
  System::Call "user32::LoadKeyboardLayout(l $6, i 1)"
SectionEnd

Function un.onUninstSuccess  
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name)已移除成功。" /SD IDOK
FunctionEnd

Function un.onInit
;!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "確定要完全移除$(^Name)？" /SD IDYES IDYES +2
  Abort
FunctionEnd

Section Uninstall
/*
  ReadRegStr $0 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Key"
  System::Call "user32::UnloadKeyboardLayout(l r0) i .r1 ?e"
  ${If} $1 == 0
   MessageBox MB_OK "偵測到有程式正在使用輸入法，請重新開機以繼續移除舊版。是否要立即重新開機？" IDOK
    Call :lbNeedReboot
  ${EndIf}
*/
  ClearErrors
  ${DisableX64FSRedirection}
   IfFileExists "$SYSDIR\OVIME.ime"  0 +3 
  Delete "$SYSDIR\OVIME.ime"
  IfErrors lbNeedReboot +1
  ${EnableX64FSRedirection}
   IfFileExists "$SYSDIR\OVIME.ime"  0  lbContinueUninstall  
  Delete "$SYSDIR\OVIME.ime"
  IfErrors lbNeedReboot lbContinueUninstall

  lbNeedReboot:
  MessageBox MB_ICONSTOP|MB_YESNO "偵測到有程式正在使用輸入法，請重新開機以繼續移除舊版。是否要立即重新開機？" IDNO lbNoReboot
  ReadRegStr $0 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Key"
  DeleteRegKey ${IME_ROOT_KEY} "${IME_KEY}\$0"
  
  ${registry::Open} "${IME_CURRENT_USER}\" " /V=1 /S=1 /N='$0' /G=1 /T=REG_SZ" $9
  ${registry::Find} $9 $1 $2 $3 $4  
  DeleteRegValue "${IME_CURRENT_USER}" "${IME_KEY_USER}" "$2"
  ${registry::Close} "$9"
  Reboot

  lbNoReboot:
  MessageBox MB_ICONSTOP|MB_OK "請將所有程式關閉，再嘗試執行本安裝程式。若仍看到此畫面，請重新開機。" IDOK +1
  Quit

  lbContinueUninstall:
  ReadRegStr $0 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Key"
  DeleteRegKey ${IME_ROOT_KEY} "${IME_KEY}\$0"
  
  ${registry::Open} "${IME_CURRENT_USER}\" " /V=1 /S=1 /N='$0' /G=1 /T=REG_SZ" $9
  ${registry::Find} $9 $1 $2 $3 $4  
  DeleteRegValue "${IME_CURRENT_USER}" "${IME_KEY_USER}" "$2"
  ${registry::Close} "$9"


  nsExec::ExecToStack '"$PROGRAMFILES64\OpenVanilla\OVUtil.exe" uninstall "$PROGRAMFILES64\Openvanilla\OVUIServer.dll"'
	
  ${DisableX64FSRedirection}
  Delete "$SYSDIR\OVIMEUI.dll"
  Delete "$SYSDIR\libltdl-3.dll"
  Delete "$SYSDIR\libhunspell.dll"
  ${EnableX64FSRedirection}
  Delete "$SYSDIR\OVIMEUI.dll"
  Delete "$SYSDIR\libltdl3.dll"
  Delete "$SYSDIR\sqlite3.dll"
  Delete "$SYSDIR\libhunspell.dll"

  Delete "$PROGRAMFILES64\Openvanilla\uninst.exe"
;  RMDir /r "$PROGRAMFILES\OpenVanilla"
  RMDir /r "$PROGRAMFILES64\Openvanilla"
  Delete "$SMPROGRAMS\OpenVanilla\Uninstall.lnk"
  Delete "$SMPROGRAMS\OpenVanilla\OVPreferences.lnk"
  RMDir "$SMPROGRAMS\OpenVanilla"
  ${If} ${RunningX64}
  	SetRegView 64
  ${EndIf}
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  SetAutoClose true
SectionEnd
