
#uncomment the following line to get a trace of startup process inside
# 'c:/spwstrace.txt' or '/tmp/spwstrace.txt'
#$config{_debugtofile} = "1";

##lines below is a sample on how to start arbitrary program before or
#after browser. They will be automatically terminated when magic url
#is visited.
#$config{program_to_run_before_browser_1} = ($^O =~ /(win32)|(cygwin)/i) ?
#      './notepad.exe' : $^O eq 'darwin' ? './my-osx-program-name' :
#	'./my-linux-program-name';
##this line instructs to wait 1 second after starting that program.
#$config{program_to_run_before_browser_1_sleep_seconds} = 1;

#$config{_debugtofile} = 1;

#$config{win32_firewall_alert_message} = 'Turn firewalls off, dude!';


$config{win32_msie_prop__addressbar} = 0;
$config{win32_msie_prop__menubar} = 0;
$config{win32_msie_prop__statusbar} = 0;
$config{win32_msie_prop__toolbar} = 0;

#$config{program_to_run_before_server_1} = './my-linux-program-name';

$config{urlpath} = '/obf-ui/start.shtml';

#$config{urlhost} = 'localhost';

$config{win32_prefer_ff_embedded} = 1;
$config{win32_show_trayicon} = 1;
$config{osx_show_trayicon} = 1;

$config{win32_try_using_pipes} = 1;

if ($^O =~ /(win32)|(cygwin)/i) {
    $config{urlpath} = '/obf-ui/ui.xpl/none/';
    $ENV{STUNNIX_WIN32_DO_NOT_FETCH_RAM_INFO} = 1;
};