call "C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\Bin\ResGen.exe" "..\..\Properties\Resource.en-US.resx"
call "C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\Bin\ResGen.exe" "..\..\Properties\Resource.zh-TW.resx"
rem mkdir "C:\OpenVanilla\Resources"
rem mkdir ".\Resources"
rem copy "..\..\Properties\Resource.en-US.resources" ".\Resources\"
rem copy "..\..\Properties\Resource.zh-TW.resources" ".\Resources\"
rem copy "..\..\Properties\Resource.en-US.resources" "C:\OpenVanilla\Resources\"
rem copy "..\..\Properties\Resource.zh-TW.resources" "C:\OpenVanilla\Resources\"
mkdir "c:\OpenVanilla\en-US"
mkdir "c:\OpenVanilla\zh-TW"
copy ".\en-US\OVPreferences.resources.dll" "c:\OpenVanilla\en-US\"
copy ".\zh-TW\OVPreferences.resources.dll" "c:\OpenVanilla\zh-TW\"

