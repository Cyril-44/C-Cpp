#!/usr/bin/perl

$ostype = 'win';

use Win32::GUI;
use Win32::Pipe;
use strict;
use FindBin;
use lib ("$FindBin::Bin");

our %config;

sub t { #print STDERR join("\n",@_,''); 
} ;

_init_srv_info($ARGV[0]);

my $id = 1;
my $icon = new Win32::GUI::Icon($config{is_cdbuilder} ?
     'site/stunnixwebsrv/stunnixsrv-cdbuilder.ico' : 'site/stunnixwebsrv/stunnixsrv.ICO');

#hidden window that will never be shown

my $classw32 = Win32::GUI::Class->new(-name => 'SAWS_trayicon_'. $config{'_httpd_port'});
my $main = Win32::GUI::Window->new(
    -name   => 'Main',
    -text   => 'Perl',
    -width  => 200,
    -height => 200,
    -class  => $classw32,
);
my $main2 = Win32::GUI::Window->new(
    -name   => 'Main',
    -text   => 'Perl',
    -width  => 200,
    -height => 200,

);
$main2->AddTimer("Timer1",500);


$main->Enable();
$main2->Enable();

my $ni = $main->AddNotifyIcon(
    -name => "systray",
    -id   => $id,
    -icon => $icon,
    -tip  => ( $config{is_cdbuilder} ?  "Stunnix CDBuilder - http://stunnix.com/": 
	length($config{win32_trayicon_tooltip_text}) ?  $config{win32_trayicon_tooltip_text} :
	"Stunnix Web Server - http://stunnix.com/"),
);
my $systray_menu = new Win32::GUI::Menu(
        "SystrayMenu Functions" => "SystrayMenu",
	do_populate_menu_entries(),
#        "> Exit"       => "SystrayExit",
    );
Win32::GUI::Dialog();
Win32::GUI::Show($main);
Win32::GUI::Show($main2);

exit(0);


sub systray_RightClick {
    my($x, $y) = Win32::GUI::GetCursorPos();
    $main->TrackPopupMenu($systray_menu->{SystrayMenu}, $x, $y-50);
}


sub Main_Terminate { 
    mylog("main_term");
    Win32::GUI::NotifyIcon::Delete( $ni, -id =>  $id );
    return -1;    
}

sub Main2_Terminate { 
    mylog("main_term");
    Win32::GUI::NotifyIcon::Delete( $ni, -id =>  $id );
    return -1;    
}

sub Main_Close {
    mylog("main_close");
    Win32::GUI::NotifyIcon::Delete( $ni, -id =>  $id );
    return -1;    
}


sub systray_Terminate {
    mylog("systr_term");
    Win32::GUI::NotifyIcon::Delete( $ni, -id =>  $id );
    return -1;
}


#hack - we can't detect the fact that window has been closed by other
#process except by checking for window text
sub Timer1_Timer
{
	my $txt = $main->Text;
	if (!$txt)
	{
		Main2_Terminate();
		exit(0);
	};
}

sub SystrayExit_Click
{
	Main_Terminate();
};

#################app-specific code follows




use startutils;

sub _init_srv_info
{
	my ($fn) = @_;
	my $VAR1 = do $fn;
	%config = %{$VAR1->{ALL}};

	$config{is_cdbuilder} = 1 if ($config{sitename} eq 'SAWS-Demo-1-0');
}

sub do_populate_menu_entries()
{
	my @items;
	if (!$config{nobrowser})
	{
		push @items, "> Open site again", "homereopen";
                push @items, "> Open inspector again", "inspreopen"
			if ($config{webserver_inspector})
	};
	push @items, "> Terminate site", "itemTerminate";
	@items;
}

sub _openurl
{
	my ($url) = @_;
	$config{_openurl}= $url;
	startutils::open_page( $config{'_httpd_port'});	
}
sub homereopen_Click
{
	_openurl($config{_homeurl});
};

sub inspreopen_Click
{
	_openurl($config{_insponlyurl});
};

sub itemTerminate_Click
{
	my $port = $config{'_httpd_port'};
	my $remote = opensk($port);
        t("connection failed") if !$remote;
        return 0 if !$remote;
	my $reqstr = "GET /stop-the-server/helper-stop-httpd.exe HTTP/1.0\n\n";
	if ($config{_win32_using_pipes}) {
	    $remote->Write($reqstr);
            my ($str,$allstr)=('','');
	    do {
    	        $str = $remote->Read();
        	$allstr .= $str;
    	    } while (length($str));
    	    $remote->Disconnect();
	} else {
    	    t("sending req to remote - port $port");
	    syswrite($remote,$reqstr);
	    my $buf;
	    sysread($remote,$buf,10000);	
	};
        t("calling main_terminate");
	Main_Terminate();
};


sub mylog
{
	return;
	my ($str) = @_;
	open(F,">>log");
	print F "logged $str\n";
	close F;
}