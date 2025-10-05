@echo off
echo If you need to simulate first run of the product, kill all Apache.exe and wperl.exe processes and 
echo delete contents of %TEMP%\.stunnixws and %LOCALAPPDATA%\.stunnixws folders
echo
echo A new window will appear with a trace of web core of Stunnix Advanced Web Server.

gui\perl\win32\wperl.exe gui\site\stunnixwebsrv\startsite.pl _debug 1 _debugtofile 1

