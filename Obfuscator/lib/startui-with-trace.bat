@echo off
echo Stunnix Advanced Web Server will start. It may take a while. Once 
echo it is started, a trace of startup will be shown in Notepad.
echo

set FN=%TEMP%\stunnix-startup-%RANDOM%.txt
gui\perl\win32\wperl.exe gui\site\stunnixwebsrv\startsite.pl _debug 1 2>%FN%
start %FN%
