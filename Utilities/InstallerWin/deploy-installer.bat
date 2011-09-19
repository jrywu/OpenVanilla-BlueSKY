mkdir "..\InstallerWin\system32.x86"
mkdir "..\InstallerWin\system32.x64
mkdir "..\InstallerWin\OpenVanilla"
mkdir "..\InstallerWin\OpenVanilla\Modules"
mkdir "..\InstallerWin\OpenVanilla\Modules\x86"
mkdir "..\InstallerWin\OpenVanilla\Modules\x64"
mkdir "..\InstallerWin\OpenVanilla\Modules\OVIMArray"
mkdir "..\InstallerWin\OpenVanilla\Modules\OVIMGeneric"
rem mkdir "..\InstallerWin\OpenVanilla\Modules\OVIMRomanNew"
mkdir "..\InstallerWin\OpenVanilla\Modules\OVOFReverseLookup
rem mkdir "..\InstallerWin\OpenVanilla\en-US"
rem mkdir "..\InstallerWin\OpenVanilla\zh-TW"
copy "..\..\Dep\x86\libltdl3.dll" ..\InstallerWin\system32.x86\
copy "..\..\Dep\x86\libhunspell.dll" ..\InstallerWin\system32.x86\
copy "..\..\Dep\x86\sqlite3.dll" ..\InstallerWin\system32.x86\
copy "..\..\Dep\x64\libltdl-3.dll" ..\InstallerWin\system32.x64\
copy "..\..\Dep\x64\libhunspell.dll" ..\InstallerWin\system32.x64\
copy "..\..\Dep\x64\sqlite3.dll" ..\InstallerWin\system32.x64\
copy "..\..\Loaders\Win32-IME\OVManagedUI\bin\Release\OVManagedUI.dll" ..\InstallerWin\OpenVanilla\
copy "..\..\Loaders\Win32-IME\OVManagedUI\bin\Release\OVManagedUI.dll" ..\InstallerWin\OpenVanilla\
copy "..\..\HelperApps\GacUtil\bin\Release\GacUtil.exe" ..\InstallerWin\OpenVanilla\
copy ..\..\Loaders\Win32-IME\OVIMEUI\Release\x86\OVIMEUI.dll ..\InstallerWin\system32.x86\
copy ..\..\Loaders\Win32-IME\OVIMEUI\Release\x64\OVIMEUI.dll ..\InstallerWin\system32.x64\
copy ..\..\Loaders\Win32-IME\OVIME\Release\x86\OVIME.ime ..\InstallerWin\system32.x86\
copy ..\..\Loaders\Win32-IME\OVIME\Release\x64\OVIME.ime ..\InstallerWin\system32.x64\
copy ..\..\Loaders\Win32-IME\OVPreferences\bin\Release\OVPreferences.exe ..\InstallerWin\OpenVanilla\
copy ..\..\Loaders\Win32-IME\OVPreferences\bin\Release\zh-TW\* ..\InstallerWin\OpenVanilla\zh-TW
copy ..\..\Loaders\Win32-IME\OVPreferences\bin\Release\en-US\* ..\InstallerWin\OpenVanilla\en-US
copy "..\..\Modules\OVIMArray\Release\x86\OVIMArray.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMArray\Release\x64\OVIMArray.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\DataTables\array30.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMArray\"
copy "..\..\DataTables\array-special.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMArray\"
copy "..\..\DataTables\array-shortcode.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMArray\"
copy "..\..\Modules\OVIMGenericSQ\Release\x86\OVIMGenericSQ.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMGenericSQ\Release\x64\OVIMGenericSQ.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
rem copy "..\..\DataTables\dayi3.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMGeneric\"
rem copy "..\..\DataTables\ehq-symbols.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMGeneric\"
rem copy "..\..\Modules\OVIMRomanNew\Release\x86\OVIMRomanNew.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
rem copy "..\..\Modules\OVIMRomanNew\Release\x64\OVIMRomanNew.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
rem copy "..\..\Modules\OVIMRomanNew\en_US.aff" "..\InstallerWin\OpenVanilla\Modules\OVIMRomanNew\"
rem copy "..\..\Modules\OVIMRomanNew\en_US.dic" "..\InstallerWin\OpenVanilla\Modules\OVIMRomanNew\"
copy "..\..\Modules\OVIMPhonetic\Release\x86\OVIMPhoneticStatic.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMPhonetic\Release\x64\OVIMPhoneticStatic.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVKPPhraseTools\Release\x86\OVKPPhraseTools.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVKPPhraseTools\Release\x64\OVKPPhraseTools.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVOFFullWidthCharacter\Release\x86\OVOFFullWidthCharacter.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVOFFullWidthCharacter\Release\x64\OVOFFullWidthCharacter.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVOFHanConvert\Release\x86\OVOFHanConvert.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVOFHanConvert\Release\x64\OVOFHanConvert.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVOFReverseLookup\Release\x86\OVOFReverseLookup.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVOFReverseLookup\Release\x64\OVOFReverseLookup.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\DataTables\lookup-bpmf.cin" "..\InstallerWin\OpenVanilla\Modules\OVOFReverseLookup\"

pause
