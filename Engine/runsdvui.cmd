cd /d "D:\Projects\talgaen-mythos\Engine" &msbuild "Engine.vcxproj" /t:sdvViewer /p:configuration="Debug" /p:platform=Win32
exit %errorlevel% 