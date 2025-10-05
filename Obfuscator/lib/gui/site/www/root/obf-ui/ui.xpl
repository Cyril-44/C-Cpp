#!/usr/bin/env perl

{
    use FindBin;
    chdir "$FindBin::Bin/";
}
{

push @INC, "../../apps/lib/","../../apps/deps-fallbacks";
foreach (qw(perl-obfus cxx-obfus js-obfus vbs-obfus))
{
    push @INC, "../../../$_/deps/";
}
unshift @INC, "../../apps/deps-unique";

eval("use XCGI;");
XCGI::run( {
	'inc'		=> "$ENV{'SERVER_ROOT'}../www/apps/lib/",
	'module' 	=> 'obfui::pub',
	'function'	=> 'doit'
    } );
}

