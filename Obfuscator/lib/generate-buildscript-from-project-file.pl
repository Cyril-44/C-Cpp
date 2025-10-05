#!/usr/bin/perl

### If you are on Linux or Mac, run 
###			perldoc NAME-OF-THIS-FILE
### in terminal (while in the directory with this file) to see user guide for this file.
 
use strict;
use POSIX;
use FindBin qw($Bin);
use File::Spec;

my $cwd = getcwd(); #save cwd
my $lang = '';
our $_prodoptions = {}; #dir with opfiles will be stored as key 'dir_with_opfiles'
my $prodoptions;

#export PATH_INFO=/none/project.open
#export STUNNIXWS_STATEDIR=/home/hvv/
#export DOCUMENT_ROOT=/home/hvv/tmp/devel/cxx-obfus/build/lib/gui/site/www/root

sub catdir{File::Spec->catdir(@_);};
sub catfile{File::Spec->catfile(@_);};

sub setup_environment
{
    {
	my $fn = "$Bin/gui/site/www/apps/lib/obfui/prodoptions.pl";
	do $fn;
	if (!-r $fn) {
	    die("can't read prodoptions from $fn $?");
	}
	$prodoptions = $_prodoptions;
	$lang = $prodoptions->{lang};
    }
    {
	my $docroot = catdir($Bin,"gui/site/www/root");
	$ENV{DOCUMENT_ROOT} = $docroot;
    }
    {
	my $root = catdir($Bin,"gui/site/config");
	$ENV{SERVER_ROOT} = $root . '/';
    }
    {
	my $rootdir = $ENV{HOME};
	if ($^O =~ /win32/i) {
	    $rootdir = $ENV{LOCALAPPDATA};
	}
	$ENV{STUNNIXWS_STATEDIR} = $rootdir;
    }
    {#calculate dir_with_opfiles
	my $pfx = $^O =~ /Win32/ ?  "" : '.';
	my $subdir = $pfx . $prodoptions->{lang} . "-obfus-projects";
	$prodoptions->{dir_with_opfiles} = catdir($ENV{STUNNIXWS_STATEDIR},$subdir);
    }
}


sub make_request{my($url)
    = @_;
    $ENV{PATH_INFO} = '/' . $url;
    $ENV{STUNNIX_DO_NOT_EMIT_HTTP_HEADERS} = 1;
    my $fn = catfile($Bin,'gui/site/www/root/obf-ui/ui.xpl');
    if (!-r $fn) {
	print STDERR "can't read $fn: $_";
	exit 1;
    };
    system($^X,$fn);
}

#type can be 'sh' or 'bat'. If 'outfile' is empty, it's produced on stdout.
sub gen_buildscript{my($opfilename,$type,$outfile)
    = @_;
    my $tmpopname = 'XXXXX';
    my $tmpopfname = catfile($prodoptions->{dir_with_opfiles},$tmpopname) . '.op';
    {#copy $optfilename to dir
	if (!-r $opfilename) {
	    print STDERR "can't read $opfilename: $?";
	    return 1;
	}
	_copyfile($opfilename,$tmpopfname) && return 1;
    }
    {
	if (length($outfile))
	{
	    open(STDOUT,'>',$outfile);
	}
    }    
    {
	my $verb = $type eq 'bat' ? 'bat' : 'buildscript';
	my $ret = make_request( $tmpopname . '/project.export.export.' . $verb);
	unlink($tmpopfname);
	return $ret;
    }
    0;
}

sub _copyfile{my($innm,$outnm)
    = @_;
    open(IN,'<',$innm ) || return 1;
    open(OUT,   '>', $outnm) || return 1;
    binmode(IN);
    binmode(OUT);
    my ($len,$buf);
    while (($len = read(IN,$buf,16*1024)) != 0)
    {
        print OUT $buf;
        $buf = '';
    }
    close(IN);
    close(OUT);
    0;
}

setup_environment();
exit(doit());

sub doit
{
    my @args = @ARGV;
    my ($verb,@rest_args)  = @args;
    if ($verb eq 'tell-dir-with-opfiles') {
	print $prodoptions->{dir_with_opfiles};		
    } elsif ($verb eq 'make-request') {
	return make_request(@rest_args);
    } elsif ($verb eq 'generate-buildscript' || $verb eq 'generate-buildscript-bat') {
	my $type = $verb eq 'generate-buildscript-bat' ? 'bat' : 'sh';
	my ($opfilename,$outfile) = @rest_args;
	return gen_buildscript($opfilename,$type,$outfile);
    } else {
	use Pod::Usage;
	pod2usage( { -verbose => 2, 'message' => "Unknown command $verb! See below accepted command line syntax.\n", 
	    -exitval => 4 });
	return 1;
    };    
    0;
}

=pod
=head1 NAME

generate-buildscript-from-project-file.pl - generate buildscripts from .op files using command line


=head1 SYNOPSIS

B<generate-buildscript-from-project-file.pl>
    S<[ B<generate-buildscript  opfilename [outfile] > ]> 
    S<[ B<generate-buildscript-bat opfilename [outfile] > ]> 
    S<[ B<tell-dir-with-opfiles> ]>
    S<[ B<make-request rest-of-url> ]>

=head1 DESCRIPTION

This tool can be used for generation of buildscripts from I<.op> files using command line.

To use this tool, install the command line alias to it to some directory (preferably listed in the PATH)
using I<Tools> - I<Configure command line tools..> menu of Project Manager. Then you will be able to call
the alias from that directory or without mentioning directory name if the directory is listed in the PATH 
environment variable.

The first command line argument of the tool specifies mode of operation, the rest of arguments 
provide options of the mode.

=head2 Command line options

=over 4

=item  S<[ B<generate-buildscript  opfilename [outfile] > ]> 

=item  S<[ B<generate-buildscript-bat opfilename [outfile] > ]> 

Use these commands to generate buildscripts (in form of shell script or Windows .bat file) from
I<.op> file. If B<outfile> is specified, the script is written to B<outfile>, otherwise it's produced on stdout.

B<opfilename> specifies full path to I<.op> file. You can get I<.op> file by using I<Project> - I<Export for backup>
via GUI. Also you can copy it directly from a directory where Project Manager keeps its project files (use option
S< B<tell-dir-with-opfiles> > of this tool to find where the directory resides).

This option is handy if you keep your I<.op> file in version tracking system like Git.

=item  S<[ B<tell-dir-with-opfiles> ]>

Writes to stdout the path to the directory where Project Manager stores project files.

=item   S<[ B<make-request rest-of-url> ]>

Emulate HTTP request to the Project Manager. You can use this option to simulate clicking on any menu item
found in Project Manager GUI. The raw HTML output will be produced on stdout.

This feature is not documented yet. Let us know if you wish to use it.

The hostname, port and I</obf-ui/ui.xpl/> part of URL path need to be omitted, so 
to simulate I<Project>-I<Open>, pass I<none/project.open> as B<rest-of-url>.


=back


=cut

