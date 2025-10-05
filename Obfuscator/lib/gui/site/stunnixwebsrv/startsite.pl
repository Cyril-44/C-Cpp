#!/usr/bin/perl

package main;

use strict;
use FindBin;
use IO::Socket::INET;
use POSIX;

$ENV{PERl5LIB}=''; #it may have been set by Oracle to its own ancient Perl

use vars qw($orig_bin_dir $binary_httpd $interp_name $ostype %config);

my $platform = {}; #detailed version

$ostype = 'unix';
{
    $ostype = 'win' if ($^O =~ /(win32)|(cygwin)/i);
}
if ($ostype eq 'win')
{
    $SIG{ALRM} = 'IGNORE';
}


$orig_bin_dir = $FindBin::Bin;
$binary_httpd = 1;
$interp_name = "$^X";

my $no_perl = 0;

if ($ENV{PAR_INITIALIZED} && $ENV{PAR_TEMP})
{
    my $d = __FILE__;
    $d = substr($d,0,rindex($d,'/'));
    $FindBin::Bin = $d;	#"$ENV{PAR_TEMP}/inc/script";
    $no_perl = 1;
}
chdir $FindBin::Bin;
use lib ("$FindBin::Bin/../../perl/common-inc","$FindBin::Bin/../../perl/fallback-inc",$FindBin::Bin);


if ($no_perl)
{    
    my $exepath = $0;
    if ($^O =~ /(cyg)?win\d+/i)
    {
	$exepath =~ s,\\,/,g;
    }
    $exepath = "$orig_bin_dir/$0" if (index($exepath,$orig_bin_dir)!=0);
    $interp_name = "$exepath --runother";
};
$interp_name = "\"$interp_name\"";
$ENV{PERL_INTERP_NAME} = $interp_name;

if ($no_perl)
{
    if ($ARGV[0] eq '--runother')
    {
	shift @ARGV;
	my $file = shift @ARGV;
	my $pos = rindex($file,'/');
	if ($pos >= 0 )
	{
	    my $dir = substr($file,0,$pos);
	    chdir($dir);
	};
	do $file;
	print STDERR "there were problems running file $file: $@\n" if $@;
	exit;
    };
}

sub _init_platform
{
    my $n = $^O;
    if ($n =~ /Win/)
    {
	$platform->{cpuarch} = 'x86';
	$platform->{osname} = 'windows';
	$platform->{exe_ext} = '.exe';
	$platform->{dll_ext} = '.dll';
	$platform->{dyld_env} = 'PATH';
	$platform->{pathlist_sep} = ';';
	$platform->{platspec} = 'x86_windows';
    } elsif ($n eq 'darwin') {
	$platform->{cpuarch} = 'ppc';
	$platform->{osname} = 'macosx';
	$platform->{dll_ext} = '.so';
	$platform->{dyld_env} = 'DYLD_FALLBACK_LIBRARY_PATH';
	$platform->{pathlist_sep} = ':';
	$platform->{platspec} = 'ppc_osx';

	my $v = (split('\.',`uname -r`))[0];
	$platform->{osvermajor} = '10';
	$platform->{osverminor} = $v >= 10 ? '6' : $v >= 9 ? '5' : $v >=8 ? '4' : $v >=7 ? '3' : $v>=6 ? '2' : 1;

	my $uname_m_output = `uname -m`;
	if (index($uname_m_output,'86')>0 
		|| index($uname_m_output,'arm')>0 #hack - till we build everything for Apple ARM CPUs
	    )
	{
	    $platform->{darwin_x86} =1;
	    $platform->{platspec} = 'x86_osx';
	};
    } elsif ($n eq 'linux') {
	$platform->{osname} = 'linux';
	$platform->{dll_ext} = '.so';
	$platform->{dyld_env} = 'LD_LIBRARY_PATH';
	$platform->{pathlist_sep} = ':';
	$platform->{platspec} = 'x86_linux';
	#compute cpu arch
	my $requested_cpu = $ENV{STUNNIXWS_CPU_PLATFORM}; #it's initialized by 'autorun' script!
	if (!length($requested_cpu)) {
	    my $cpu = `uname -m`;
	    $platform->{cpuarch} = (index($cpu,'86')>=0) ? 'x86' : $cpu;    
	    if (index($cpu,'x86_64') >=0) {
		$platform->{is_linux_64bit}  = 1;
	    };
	} else {
	    $platform->{cpuarch} = $requested_cpu;
	    if (index($requested_cpu,'x86_64') >=0) {
		$platform->{is_linux_64bit}  = 1;
	    };
	}
	$platform->{platspec} = $platform->{cpuarch} . '_linux';
    };
};

_init_platform();

#These are configuration settings. They can be modified by the 
# '../config/siteinfo.pm' file that is evaluated (if it's present). 
#They also can be overriden by passing space-separated list of values on 
#commandline. 
#Ones with names starting from '_' are private and shouldn't be overriden from
#commandline in most cases
%config = ( 
#it's GREATLY recommended  to alter these options. 
#Override them in ../config/siteinfo.pm, NOT HERE!!
	'sitename', 	'default',
#there is can be a point in changing these:
#Override them in ../config/siteinfo.pm, NOT HERE!!
	'urlpath',	'', #document path to open on the site by default.
			    #this will go after http://127.0.0.1:9000:/
	'urlhost',	'127.0.0.1',
	'show_win32_firewall_alert',1,
	'win32_firewall_alert_message','',#if empty, default message is used

	'nobrowser',	0,  #if 1, browser won't be started. 
	'macosx_prefer_ie', 0,
	'win32_prefer_ie', 1,
	'win32_prefer_ie_kiosk_mode', 0,
	'port_to_start_scanning_from', 9000,
	'do_not_eject_on_shutdown',0,
	'restart_saws_if_running',0,
	'win32_show_trayicon',0,
	'win32_trayicon_force_exit_on_shutdown',0,
	'osx_show_trayicon',1,
	'preheat_urls','', #pipe-separated list of URLs to load before showing browser window, like "/foo.html|/blah.php"

	'pass_env_vars_to_scripts',0, #whether env vars set in script that starts SAWS are passed to CGi scripts.
	'php_dont_force_stunnix_mysql',0, #whether to force the use of mysqld started by SAWS for all php scripts
	'win32_try_using_pipes',0, #whether use of pipes is preferred
	'win32_use_httpd2',0,
#description of these properties can be found here:
#http://msdn.microsoft.com/workshop/browser/webbrowser/reference/objects/internetexplorer.asp
        'win32_msie_prop__addressbar','',
        'win32_msie_prop__fullscreen','',
        'win32_msie_prop__height','',
        'win32_msie_prop__left','',
        'win32_msie_prop__menubar','',
        'win32_msie_prop__resizable','',
        'win32_msie_prop__silent','',
        'win32_msie_prop__statusbar','',
        'win32_msie_prop__theatermode','',
        'win32_msie_prop__toolbar','',
        'win32_msie_prop__top','',
        'win32_msie_prop__width','',

#this block of settings specifies which programs to run before and 
#after browser; after program is run, this wrapper waits for specified
#count of seconds and then tries to run next program
#and so on. If program name is empty, nothing is run.
#  Specify path to program relative to the top of cdrom; use '/' as directory
#separators, e.g. "somedir/myprog.exe".
	'program_to_run_before_server_1', '',
	'program_to_run_before_server_1_sleep_seconds', '',
	'program_to_run_before_server_2', '',
	'program_to_run_before_server_2_sleep_seconds', '',
	'program_to_run_before_server_3', '',
	'program_to_run_before_server_3_sleep_seconds', '',

	'program_to_run_before_browser_1', '',
	'program_to_run_before_browser_1_sleep_seconds', '',
	'program_to_run_before_browser_2', '',
	'program_to_run_before_browser_2_sleep_seconds', '',
	'program_to_run_before_browser_3', '',
	'program_to_run_before_browser_3_sleep_seconds', '',

	'program_to_run_after_browser_1', '',
	'program_to_run_after_browser_1_sleep_seconds', '',
	'program_to_run_after_browser_2', '',
	'program_to_run_after_browser_2_sleep_seconds', '',
	'program_to_run_after_browser_3', '',
	'program_to_run_after_browser_3_sleep_seconds', '',




#when debugging the product, one can find tweaking these options helpful
	'_debug',	0,  #if 1, generated config file won't be deleted,
			    #and a trace of startup will be shown on console
	'_debugtofile',	0,


#these are private settings that nobody may wish to alter
	'_extroot',	'', #pipe-separated list directory names to look 
			    #for extensions
	'_httpd_mods_root', '',  #pipe-separated list directory names to look 
			    #for httpd modules; default is /httpd if empty
	'_home_subdir',	'.stunnixws',
	'_docroot_default', '../www/root',#where document root is located
			    #physically. No slash at the end!!
	'statedir', 	'',#autocomputed if empty
	'statedir_hash',	'', #some hash for state_dir, used to distinguish instances from different users.
	'_serverroot',	'',#don't change
	'_docroot',	'',#actual docroot, can 
	'_httpd2',	0, #it's 1 if based on httpd2.x
	'_other_progs_pidfile','other-progs-pids.txt',
	'_splashflagfilename','',
	'_splash_showhidefilename','', #if file exists, splash screen is hidden.
	'_showlogo_pid','0',
	'_osxtrayicon_pid',0,
	'_win32_using_pipes',0,	#whether it's decided to use pipes on the current OS
	'_win32_pipe_pfx',"\\\\.\\pipe\\STUNNIXPIPE", #pfx for pipe name
	'_win32_pipe_name_pfx',"STUNNIXPIPE", #pfx for pipe name
	'_stop_url','/stop-the-server/helper-stop-httpd.exe',
	'_contentroot','',	#where all content like databases and web root is stored. Used with compressed runtime
	'have_contentroot',0,
	'reset_all_state',0,	#1 means delete everything in STUNNIXWS_PERMSTATEDIR and STUNNIXWS_TEMPSTATEDIR
	'command','default', #start|reopen|stop
	'pids_to_kill_on_shutdown','', #space-separated list of PIDs to kill when web server is stopped
	'osx_nodewebkit_bundle_name','nwjs', #name of the nodewebkit directory for OSX  - by default it's nwjs.app
	'preheat_urls_no_initial',0, #if 1, "initial url to open" (see setting named 'urlpath' )will not be fetched in background during startup.
				    #preheating is performed to make web site open faster when browser starts. This setting has no GUI control in CDBuilder.

	'require_diskspace_mb_macosx',0, #size in MB to require on disk with temp files. 0 means "do not check it"
	'require_diskspace_mb_linux',0,
	'require_diskspace_mb_windows',0,

	'_required_httpd_port',0, #port number to use for HTTP. SAWS will try hard to use this port, waiting for 30 secs 
	    #until it becomes free. 0 if no preference
	'_required_mysql_port',0, #port number to use for MySQL. SAWS will try hard to use this port, waiting for 30 secs 
	    #until it becomes free. 0 if no preference.
#these entries predeclare private structures. Don't try to put any values
#here - they will be ignored most probably.
	'_extroots',	[],	
	'_httpd_mods_roots', [],

	'_binary_httpd', $binary_httpd,
	'_httpd_defines',{},
	'_onserver_started_callbacks',[],#list of subs to call
	'_other_spws_config_parts',[],#each item is a line for spws-layer config file
);

if (-f 'siteinfo.pm')
{
    do "siteinfo.pm";    
}
if (-f '../config/siteinfo.pm')
{
    do "../config/siteinfo.pm";
}

##########################
my $transfn = "transkind.txt";#has format like "stopping 74343". Words can be "starting" or "stopping"

sub TRANS_STARTING { "starting";}
sub TRANS_STOPPING { "stopping";}
#block of code related to transition state handling
#this gets current transition kind and checks whether PID in the and returns the string from the file if PID is alive
#otherwise returns empty string.
sub get_trans_kind
{
    my $fn = "$config{statedir}/${transfn}";
    if (open(FI,"<$fn"))
    {
        my $l = <FI>;
        chomp $l;
        close FI;
        my($state,$pid) = split(/\s+/,$l);
        if ($pid) {
            if (kill(0,$pid)) {
                return $state;
            } else {
                return 'err-piddead';
            };
        } else {
            return 'err-badformat';
        };
    } else {
        return 'err-nofile';#file is absent
    };
}

#sets content of $transfn. If 1st arg is empty, delete that file.
sub set_trans_kind
{
    my ($kind) = @_;
    my $fn = "$config{statedir}/${transfn}";
    if (!length($kind)) {
        unlink $fn;
        return;
    };
    if (open(FI,">$fn"))
    {
        print FI $kind . ' ' . $$;
        close FI;
    };
}

sub rmtree_except_portableff
{
    my ($p,$sep) = @_;
    {
	opendir(D,$p);
	foreach (readdir(D))
	{
	    next if $_ eq '.' || $_ eq '..' || $_ eq 'portableff' || $_ eq 'osx-trayicon-pid.txt';
	    my $fp = "${p}${sep}$_";
	    if (-d $fp)
	    {
		rmtree([$fp]);
	    } else {
		unlink($fp);
	    }
	}
	closedir D;
    }
}

#used for hashin path to statedir
sub checksum 
{
    my ($str) = @_;
    my $i=1;
    my $sum = 0;
    foreach (split(//,$str)) {my $v = $i++*ord($_) ; $sum+=$v;}
    $sum;
}

sub check_webserver_expiration
{
    {#show warning about expiration
	#my $fnm = "$config{_stunnixws_serverroot}/ws/mod/hash";
	my $fnm = "$config{_serverroot}/ws/mod/hash";
	if (open(F,"<$fnm"))
	{
	    my @lines = <F>;
	    close F;
	    my $v = eval(join('',@lines) );
	    if ($v && (time() > $v))
	    {
		my $msg = ".lairt  wen a daolnwod esaelP .detanimret saw dna deripxe sah noitacilppA ehT";
		$msg = join('',reverse(split('',$msg)));
		show_message_after_hiding_splash_screen($msg);
		exit(1);
	    };
	};
    }
}

sub _get_binary_path
{
    my $tool_name = $_[0];  # simple example
    my $tool_path = '';

    for my $path ( split /:/, $ENV{PATH} ) {
	if ( -f "$path/$tool_name" && -x _ ) {
    	    $tool_path = "$path/$tool_name";
        last;
	}
    }
    $tool_path;
}

sub show_message
{
    my($text) = @_;
    print STDERR $text . "\n";	
    if ($^O =~ /Win/) {
	eval("use startutils;");
	startutils::show_win32_message($text);
	#system("showmessage.exe $text");
    } elsif ($^O eq 'darwin') { #osx
	system('/usr/bin/osascript','-e',"tell application \"Finder\" to display dialog \"$text\" buttons {\"OK\"} with icon stop");	
    } else { #linux
	my $path_to_zenity = _get_binary_path('zenity');
	my $path_to_xmessage = _get_binary_path('xmessage');
	if (length($path_to_zenity)) {
	    system($path_to_zenity,'--error','--text=' . $text);
	} elsif (length($path_to_xmessage)) {
	    system($path_to_xmessage,$text);
	}	
    }
}

sub show_message_after_hiding_splash_screen
{
    my($text) = @_;
    hide_splash_screen();
    set_trans_kind();
    show_message($text);
}


sub hide_splash_screen
{
    unlink $config{_splashflagfilename} if length($config{_splashflagfilename});
    kill 9, $config{_showlogo_pid} if $config{_showlogo_pid};
}

#it will exit if checks fail
sub do_firstrun_checks
{
    firstrun_check_free_disk_space();
}

sub firstrun_check_free_disk_space()
{
    my $dir = $config{statedir};
    
    $dir =~ s,[/\\]+[^/\\]+[/\\]?$,,g; #remove last part of the path, since the directory does not exist yet
    

    my $required_mb = $config{'require_diskspace_mb_' . $platform->{osname} };    
    return if (!$required_mb); #we are not instructed to check disk space

    my $actual_free_mb = -1;
    my $diskname = '';
    if ($ostype eq 'win') {
	eval("use Win32::DriveInfo");
	if (!$@) {
	    $diskname = $dir =~ /^[a-z]/i ? 
		substr($dir,0,1)  #get disk name
		: $dir; #hope it's an UNC path
	    $actual_free_mb =  ((Win32::DriveInfo::DriveSpace($diskname))[6])/(1024*1024);
	}
    } else {
	my $toolname = $config{_serverroot} . 'get-disk-mb-free-' . $platform->{platspec};
	#to prevent shell metacharacters quoting, chdir to $dir and estimate free space in '.'
	my $curdir = getcwd();
	chdir($dir);
	t("running $toolname . in $dir");
	$actual_free_mb = `"$toolname" .`;
	chdir $curdir;
    };
    t("required free space: $required_mb MB, actual free space $actual_free_mb");
    return if $actual_free_mb < 0; #don't do checks if failed to compute free space
    
    if ($actual_free_mb < $required_mb) {
	my $need_extra_mb = $required_mb - $actual_free_mb;
	$need_extra_mb *= 1.1;

	if ($ostype eq 'win') {
	    $diskname = " $diskname:";
	    $diskname = uc $diskname;
	};
	show_message_after_hiding_splash_screen(
	    sprintf("There is not enough free space on your hard disk%s. Please free up %d megabytes on your hard disk and run the app again!",
		$diskname, $need_extra_mb)
	    );
	exit(1);
    };
}
##########################


my $tracefilename;

sub t { 
    print STDERR join('',@_) . "\n" if $config{_debug}; 
    if ($config{_debugtofile}) {
	#we do this so that windows to flush file buffers
	open(LOGDBG,">>${tracefilename}");
	print LOGDBG join('',@_) . "\n";
	close LOGDBG;
    };
}

{
    $config{_debugtofile} = 1 if $ENV{STUNNIX_DEBUG_TO_FILE};
	
    if ($config{_debugtofile}) {
	$tracefilename = $ostype eq 'win' ? 'c:/tmp/spwstrace.txt' : 
	    '/tmp/spwstrace.txt';
	unlink $tracefilename;
	my $stderrto = $ostype eq 'win' ? 'c:/tmp/spws-stderr.txt' : 
	    '/tmp/spws-stderr.txt';
	open(STDERR,">$stderrto");
	t("#NOTE: PLEASE ALSO SEE $stderrto for error messages produced by other parts of Stunnix Web Server!");
    };
}

use POSIX;
t("started on " . asctime(localtime(time()))) ;

{#override config from commandline
    my %newconfig = @ARGV;
    map { $config{$_} = $newconfig{$_} if exists $newconfig{$_} ;} 
	 keys %config;
}
{
    foreach (qw(_osxtrayicon_pid _showlogo_pid _splash_showhidefilename _splashflagfilename command))
    {
	my $v = $ENV{"SAWS$_"};
	$config{$_} = $v if length $v;
	delete $ENV{"SAWS$_"};
    }
    if (length( $ENV{SAWS_contentroot}))
    {
	$config{_contentroot} = $ENV{SAWS_contentroot};
	$config{have_contentroot} = 1;
    }
}

if (length($ENV{STUNNIXWS_OVERRIDEN_SETTINGS}))
{
    my %h = split(/\s+/,$ENV{STUNNIXWS_OVERRIDEN_SETTINGS});
    foreach my $k(keys %h)
    {
	$config{$k} = $h{$k}; #override settings from env variable
    }
}

{#clean various variables
    foreach (qw(STUNNIXWS_DO_NOT_SHOW_LOGO STUNNIXWS_OVERRIDEN_SETTINGS STUNNIXWS_TRACE
        STUNNIXWS_CWR_RESET_FORCED STUNNIXWS_NO_SPLASHSCREEN))
    {
	delete $ENV{$_};
    }
}

{#compute dirs
    if (!$config{statedir})
    {
	if ($ENV{HOME} && -d $ENV{HOME}) {
	    $config{statedir} = "$ENV{HOME}/$config{_home_subdir}/";
	} elsif ($ENV{TEMP}) {
	    $config{statedir} = "$ENV{TEMP}/$config{_home_subdir}/";
	} elsif ($ENV{TMP}) {
	    $config{statedir} = "$ENV{TMP}/$config{_home_subdir}/";
	} else {
	    $config{statedir} = "$FindBin::Bin/state";
	};
    }
    if (!$config{_serverroot})
    {
	$config{_stunnixws_serverroot} = $config{_serverroot} = 
	    "$FindBin::Bin/";
    }
    if ($config{have_contentroot})
    {
	$config{cdroot} = $config{_contentroot};
    } else {
	my $d = getcwd();
	chdir $config{_serverroot}; 
	chdir ".."; chdir "..";
	$config{cdroot} = getcwd();
	chdir $d;
    }
    if (!$config{_extroot})
    {
	$config{_extroot} = "$config{_serverroot}/../../extensions/";
    };
    if (!$config{_httpd_mods_root})
    {
	$config{_httpd_mods_root} = "$config{_serverroot}/../../httpd/";
    };
    {
	$config{_extroots} = [ split('\|',$config{_extroot}) ];
	$config{_httpd_mods_roots} = [ split('\|',$config{_httpd_mods_root}) ];
    }
}

 use File::Path;

{
    my $v = $ENV{SAWS_SITENAME_SUFFIX};
    $config{sitename} .= $v if length($v);
}

{#create statedir
    my $sep = $^O =~ /Win32/ ? '\\' : '/';
    my $statedir = $config{statedir};
    mkdir $statedir;
    
    $config{statedir} = $statedir .= "/$config{sitename}/";
    $config{statedir_hash} =  checksum($statedir);
    rmtree_except_portableff($statedir,$sep) if $config{reset_all_state};
    $config{_is_first_run} = ! -d $statedir;
    do_firstrun_checks() if ($config{_is_first_run}); 
    $ENV{STUNNIXWS_TMPSTATEDIR} = $statedir;
    mkdir $statedir;
    mkdir "$statedir/logs";

    _die("failed to create statedir $statedir:  $!") if ! -d $statedir;    

    #compute computable values
    $config{_portfnm} = "$statedir/port.file";
}
if ($^O =~ /Win32/)
{
    eval("use Win32::Registry;");
    if (!$@)
    {
	my $prefix = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders\\";
	my $item = "Local AppData";
	my $k;
	$::HKEY_CURRENT_USER->Open($prefix,$k);
	my ($type,$value);
        $k->QueryValueEx($item,$type,$value);
	if (!length($value)) { #Windows7
	    eval('use Win32 qw(CSIDL_LOCAL_APPDATA);');
	    $value = Win32::GetFolderPath(CSIDL_LOCAL_APPDATA());
	};
	$ENV{STUNNIXWS_STATEDIR} = $value;
    } else {
	#try to find some solution
	$ENV{STUNNIXWS_STATEDIR} = "c:/";
    }
} else {
    $ENV{STUNNIXWS_STATEDIR} = $ENV{HOME}; #support only unix for now
}

{
    my $sep = $^O =~ /Win32/ ? '\\' : '/';
    my $p = "$ENV{STUNNIXWS_STATEDIR}$sep$config{_home_subdir}";    
    mkdir $p;
    $p .= "$sep$config{sitename}$sep";
    rmtree_except_portableff($p,$sep) if ($config{reset_all_state});
    mkdir $p;
    $config{STUNNIXWS_PERMSTATEDIR} = $ENV{STUNNIXWS_PERMSTATEDIR} = $p;
    mkdir "$ENV{STUNNIXWS_TMPSTATEDIR}/logs"; #on unix it will be erased by  'rmtree([$p]) if $config{reset_all_state};'
}

if ($ostype ne 'win' && -r "$config{_serverroot}/dummyscript.sh" &&
     system("\"$config{_serverroot}/dummyscript.sh\" >/dev/null 2>&1")/256 )
{
    my $msg = "Your CDROM is mounted with 'noexec' flag or similar. Some functionality\n"
."may be lost, so either remount CDROM with 'noexec' flag or copy entire\n"
."CDROM to harddisk and run it from there";

    show_message($msg);
}


if ($config{command} ne 'stop')
{
    my $k;
    for (1..300) {
	$k = get_trans_kind();
	t("get_trans_kind returned $k");
	last if (!($k eq TRANS_STOPPING() || $k eq TRANS_STARTING()));
	sleep 1;
    }
    set_trans_kind(TRANS_STARTING());
}

check_webserver_expiration();
if ($binary_httpd && -r "httpd_modules/httpd.pm")
{
    eval('use httpd_modules::httpd;');
    my $o = new httpd_modules::httpd('httpd','booting'=>1);
    $o->configure({'config',\%config});
    $binary_httpd = $config{_binary_httpd};
} else {
    $binary_httpd = $config{_binary_httpd} = 0;
};



#inject method close() into Win32::Pipe
package Win32::Pipe;sub close { $_[0]->Disconnect; }package main; 

eval("use startutils;");

if ($config{ff_url_whitelist_enabled} && $config{ff_url_whitelist_enabled_with_fake_proxy})
{
    $config{noservers} = 0; #force enabling of webserver
};

if (!$config{noservers})
{
    startutils::register_server($^X,'Perl interpreter - part of Stunnix Web Server'); #'
    
    my $port;
    t("starting network operations");
    other_setup_funcs();
    show_firewall_warnings();
    $ENV{STARTSITE_PID} = $$; 
    $ENV{STUNNIX_WEBSERVER_WAS_ALREADY_RUNNING} = 1;
    if ($config{restart_saws_if_running} && $config{command} ne 'stop')
    {
	my $port = is_server_working();
	if ($port) {
	    do_stop_server($port);
	    sleep 45; #let everything stop
	};
    }
    $port = is_server_working();
    if (!$port && $config{command} ne 'stop')
    {

	$ENV{STUNNIX_WEBSERVER_WAS_ALREADY_RUNNING} = 0;
	$port = findfreeport(0,$config{_required_httpd_port});
	$config{'_httpd_port'} = $ENV{SERVER_PORT} = $port;
	foreach (1..3)	{ run_exta_program('before_server',$_) };
	prepare_trayicon();
	start_server($port);
	foreach (@{$config{_onserver_started_callbacks}})
	{
	    &$_();
	};
	unlink "$config{statedir}/$config{_other_progs_pidfile}";
    } else {
	if ($config{command} eq 'stop')
	{
	    #we were told to stop web server
	    do_stop_server($port);
	    exit 0;
	};
	$ENV{SERVER_PORT} = $port;
	foreach (1..3){ run_exta_program('before_server',$_) };

	#now try to read server props - start page is probably different from one 
	#defined in $config - e.g. due to the fact that .NET is absent and user has 
	#asked to change start page
	my $propfile = "$config{statedir}/server-info.pl";
	if (open(PROPFILE,"<$propfile"))
	{
	    our $VAR1;
	    do $propfile;
	    if ($VAR1->{ALL}) {
		t("reading new urlpath from props file");
		my @prop_names = qw(urlpath _docroot _httpd_port _win32_using_pipes _homeurl _openurl);
		foreach (@prop_names) { 
		    $config{$_} = $VAR1->{ALL}->{$_};
		}
	    };
	    close PROPFILE;
	}    
    };
    $config{'_httpd_port'} = $ENV{SERVER_PORT} = $port;
    foreach (1..3){ run_exta_program('before_browser',$_) };
    preheat_urls($port);
    open_page($port);
    hide_splash_screen();
    {
	my $pid = $config{_osxtrayicon_pid};
	kill 9, $pid if ($pid && !$config{osx_show_trayicon}); #kill process for osx tray icon 
    }
    foreach  (1..3) { run_exta_program('after_browser',$_) }
    set_trans_kind();

    {#log when this instance started
	my $fn = "$config{statedir}/started-timestamp.txt";
	if (open(PROPFILE,">$fn"))
	{
	    print PROPFILE $config{started_at_timestamp};
	    close PROPFILE;
	}    
    }

    t("done everything");
} else { #noservers mode
    open_page(1);
}


sub show_firewall_warnings
{
    if ($config{_is_first_run} && $config{show_win32_firewall_alert} && 
	$ostype eq 'win' && !$config{_win32_using_pipes})
    {
	my $msg = "The server will now load. " . 
	    "If prompted, please allow access through any " .
	    "firewalls (such as Zone Alarm or Windows firewall)";
	$msg = $config{win32_firewall_alert_message} 
		    if length($config{win32_firewall_alert_message});
	if ($config{_splash_showhidefilename}) {
	    open(FFF,">$config{_splash_showhidefilename}");
	    close FFF;
	};
	#system("showmessage.exe $msg");
	eval("use startutils;");
	startutils::show_win32_message($msg);
	unlink($config{_splash_showhidefilename}) if $config{_splash_showhidefilename};
    };

}
sub is_server_working
{
    #check existance of port.file
    my $port;
    return 0 if ! -f $config{_portfnm}; 
    {
	t("file '$config{_portfnm}' probably contains port number");
	if (open(PORTNR,"<$config{_portfnm}"))
	{
	    $port = <PORTNR>;
	    close PORTNR;
	    chomp $port;
	    t("[$port] is stored in file");
	    $port = 0 + $port;
	    return 0 if !$port;	    
	} else {
	    return 0;
	};
    }
    return is_server_working_on_port($port);
}



#returns free port
my @ports_already_found;
sub findfreeport
{
    my ($port_except,$port_to_use) = @_;    
    my @ports_except_list;

    if ($port_to_use) { #try for 30 seconds, on every second to use that port
	my $port = $port_to_use;
	for(my $try=0;$try < 30; ++$try) 
	{
	    t("trying port $port");
    	    my $remote = opensk($port);
	    if ($remote) {
		t("port $port is busy");
		$remote->close;
		sleep 1;
	    } else {
		t("port $port is free, will use it");
		push @ports_already_found, $port;
		return $port;
	    };	    
	};
    };

    push @ports_except_list, $port_except if $port_except;
    push @ports_except_list, @ports_already_found;
    my ($portfrom,$portto) = ($config{port_to_start_scanning_from},65536);
    my %ports_except;
    @ports_except{@ports_except_list} = @ports_except_list;
    for(my $port=$portfrom; $port < $portto; ++$port)
    {
	next if $ports_except{$port};
        my $remote = opensk($port);
	t("checking port $port");
	if ($remote) {
	    t("port $port is busy");
	    $remote->close;
	} else {
	    t("port $port is free, will use it");
	    push @ports_already_found, $port;
	    return $port;
	};
    };
    _die("no free ports found!");
}



sub prepare_trayicon
{
    return if $^O !~ /Win32/;
    if ($config{win32_show_trayicon}) {
	my $cmdline = "\"$^X\" site\\stunnixwebsrv\\win32-trayicon.pl $config{statedir}/server-info.pl";
	#hack - alter configs to reuse existing code    
	$config{program_to_run_trayicon_0} = $cmdline;
	$config{program_to_run_trayicon_0_arg0} = $^X;
	push @{$config{_onserver_started_callbacks}}, sub {	run_exta_program('trayicon',0); };
    };
    if ($config{win32_show_trayicon} || $config{win32_trayicon_force_exit_on_shutdown}) {
    	push @{$config{_other_spws_config_parts}},
		"OnExit WindowKill SAWS_trayicon_$config{_httpd_port}|10|0";
    }
};
