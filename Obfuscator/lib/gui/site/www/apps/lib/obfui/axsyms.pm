package obfui::axsyms;

use Win32::OLE::Const;
use Win32::OLE::TypeInfo;
use strict;

my @libs;
sub libCLSID    () {0}
sub libNAME     () {1}
sub libMAJOR    () {2}
sub libMINOR    () {3}
sub libLANGUAGE () {4}
sub libFILENAME () {5}

sub doit
{
    my ($havevals,$needevents,@libids) = @_;
    if ($havevals)
    {
	return ["#this functionality is present only in commercial version"];
    } else {
	my @items = map { [$_->[libNAME],
	    $_->[libMAJOR].'.'.$_->[libMINOR] ] } @libs;
	return \@items;
    }
}


sub _loadliblist
{
    Win32::OLE::Const->EnumTypeLibs(sub {
	my ($clsid,$title,$version,$langid,$filename) = @_;
	return unless $version =~ /^([0-9a-fA-F]+)\.([0-9a-fA-F]+)$/;
	my ($maj,$min) = (hex($1), hex($2));
	push @libs, [$clsid,$title,$maj,$min,$langid,$filename];
    });
    @libs = sort {lc $a->[libNAME] cmp lc $b->[libNAME]} @libs;
}

_loadliblist();


1;