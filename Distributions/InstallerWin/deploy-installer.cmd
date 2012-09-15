@echo off
mkdir "..\InstallerWin\system32.x86"
mkdir "..\InstallerWin\system32.x64
mkdir "..\InstallerWin\OpenVanilla"
mkdir "..\InstallerWin\OpenVanilla\Modules"
mkdir "..\InstallerWin\OpenVanilla\Modules\x86"
mkdir "..\InstallerWin\OpenVanilla\Modules\x64"
mkdir "..\InstallerWin\OpenVanilla\Modules\OVIMArray"
mkdir "..\InstallerWin\OpenVanilla\Modules\OVIMGeneric"
rem mkdir "..\InstallerWin\OpenVanilla\Modules\OVIMRomanNew"
rem mkdir "..\InstallerWin\OpenVanilla\Modules\OVOFReverseLookup
mkdir "..\InstallerWin\OpenVanilla\en-US"
mkdir "..\InstallerWin\OpenVanilla\zh-TW"
copy "..\..\Dep\x86\libltdl3.dll" ..\InstallerWin\system32.x86\
copy "..\..\Dep\x86\libhunspell.dll" ..\InstallerWin\system32.x86\
copy "..\..\Dep\x86\sqlite3.dll" ..\InstallerWin\system32.x86\
copy "..\..\Dep\x86\msvcp110.dll" ..\InstallerWin\system32.x86\
copy "..\..\Dep\x86\msvcp110d.dll" ..\InstallerWin\system32.x86\
copy "..\..\Dep\x86\msvcr110.dll" ..\InstallerWin\system32.x86\
copy "..\..\Dep\x86\msvcr110d.dll" ..\InstallerWin\system32.x86\
copy "..\..\Dep\x64\libltdl-3.dll" ..\InstallerWin\system32.x64\
copy "..\..\Dep\x64\libhunspell.dll" ..\InstallerWin\system32.x64\
copy "..\..\Dep\x64\sqlite3.dll" ..\InstallerWin\system32.x64\
copy "..\..\Dep\x64\msvcp110.dll" ..\InstallerWin\system32.x64\
copy "..\..\Dep\x64\msvcp110d.dll" ..\InstallerWin\system32.x64\
copy "..\..\Dep\x64\msvcr110.dll" ..\InstallerWin\system32.x64\
copy "..\..\Dep\x64\msvcr110d.dll" ..\InstallerWin\system32.x64\
REM copy "..\..\Loaders\Windows\OVUIServer\bin\Release\OVUIServer.dll" ..\InstallerWin\OpenVanilla\
REM copy "..\..\Loaders\Windows\OVUIServer\bin\Release\OVUIServer.dll" ..\InstallerWin\OpenVanilla\
copy ..\..\Loaders\Windows\OVIMEUI\Release\x86\OVIMEUI.dll ..\InstallerWin\system32.x86\
copy ..\..\Loaders\Windows\OVIMEUI\Release\x64\OVIMEUI.dll ..\InstallerWin\system32.x64\
copy ..\..\Loaders\Windows\OVIME\Release\x86\OVIME.ime ..\InstallerWin\system32.x86\
copy ..\..\Loaders\Windows\OVIME\Release\x64\OVIME.ime ..\InstallerWin\system32.x64\
copy ..\InstallerWin\OVUtil\bin\Release\OVUtil.exe ..\InstallerWin\OpenVanilla\
copy ..\..\HelperApps\PreferencesWin\bin\Release\OVPreferences.exe ..\InstallerWin\OpenVanilla\
copy ..\..\HelperApps\PreferencesWin\bin\Release\zh-TW\* ..\InstallerWin\OpenVanilla\zh-TW
copy ..\..\HelperApps\PreferencesWin\bin\Release\en-US\* ..\InstallerWin\OpenVanilla\en-US
copy "..\..\Modules\OVIMArray\Release\x86\OVIMArray.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMArray\Release\x64\OVIMArray.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVIMEsperanto\Release\x86\OVIMEsperanto.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMEsperanto\Release\x64\OVIMEsperanto.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\DataTables\ovimarray.db" "..\InstallerWin\OpenVanilla\Modules\OVIMArray\"
copy "..\..\DataTables\assoc.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMArray\"
copy "..\..\DataTables\array30.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMArray\"
copy "..\..\DataTables\array-special.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMArray\"
copy "..\..\DataTables\array-shortcode.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMArray\"
copy "..\..\Modules\OVIMGenericSQ\Release\x86\OVIMGenericSQ.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMGenericSQ\Release\x64\OVIMGenericSQ.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\DataTables\ovimgeneric.db" "..\InstallerWin\OpenVanilla\Modules\OVIMGeneric\"
copy "..\..\DataTables\assoc.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMGeneric\"
copy "..\..\DataTables\dayi3-patched.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMGeneric\"
copy "..\..\DataTables\cj-ext.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMGeneric\"
copy "..\..\DataTables\simplex-ext.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMGeneric\"
copy "..\..\DataTables\wubizixing.cin" "..\InstallerWin\OpenVanilla\Modules\OVIMGeneric\"
rem copy "..\..\Modules\OVIMRomanNew\Release\x86\OVIMRomanNew.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
rem copy "..\..\Modules\OVIMRomanNew\Release\x64\OVIMRomanNew.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
rem copy "..\..\Modules\OVIMRomanNew\en_US.aff" "..\InstallerWin\OpenVanilla\Modules\OVIMRomanNew\"
rem copy "..\..\Modules\OVIMRomanNew\en_US.dic" "..\InstallerWin\OpenVanilla\Modules\OVIMRomanNew\"
copy "..\..\Modules\OVIMPhonetic\Release\x86\OVIMPhonetic.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMPhonetic\Release\x64\OVIMPhonetic.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVIMTibetan\Release\x86\OVIMTibetan.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMTibetan\Release\x64\OVIMTibetan.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVIMUnicode\Release\x86\OVIMUnicode.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMUnicode\Release\x64\OVIMUnicode.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
rem copy "..\..\Modules\OVKPPhraseTools\Release\x86\OVKPPhraseTools.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
rem copy "..\..\Modules\OVKPPhraseTools\Release\x64\OVKPPhraseTools.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVOFFullWidthCharacter\Release\x86\OVOFFullWidthCharacter.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVOFFullWidthCharacter\Release\x64\OVOFFullWidthCharacter.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVOFHanConvert\Release\x86\OVOFHanConvert.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVOFHanConvert\Release\x64\OVOFHanConvert.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
rem copy "..\..\Modules\OVOFReverseLookup\Release\x86\OVOFReverseLookup.dll" "..\InstallerWin\OpenVanilla\Modules\x86\"
rem copy "..\..\Modules\OVOFReverseLookup\Release\x64\OVOFReverseLookup.dll" "..\InstallerWin\OpenVanilla\Modules\x64\"
echo ===============================================================================
echo Please download and install Nullosft Sciptable Install System (NSIS) comiler . 
echo                                  ---http://nsis.sourceforge.net/Download-----
echo Then compile ovwin32.nsi and ovwin32x64.nsi to build installer packages for 
echo windows 32bit and 64 bit
echo ===============================================================================
pause
