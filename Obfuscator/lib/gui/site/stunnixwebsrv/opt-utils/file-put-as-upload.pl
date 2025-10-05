#!/usr/bin/perl

=head1

This script puts data uploaded using file upload named 'data' to file whoose name is specified in 'filename' parameter.

=cut

use strict;
use CGI;
use utf8;

my $q = new CGI;
my ($fn,$data) = map { $q->param($_) } qw(filename);
my $userdata = $q->upload('data');
binmode($userdata);
utf8::decode($fn); #decode

sub webdie
{
    print STDERR @_,"\n";
    exit(0);
}


webdie("no value for parameter 'filename' specified") if (!length($fn));
print <<EOT
Content-Type: application/contentstream
Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0
Expires: Thu, 19 Nov 1981 08:52:00 GMT
Pragma: no-cache

EOT
;

if ($^O =~ /win32/i) { 
    my $scriptdir = $ENV{SCRIPT_FILENAME};
    $scriptdir =~ s,([^/\\]+)$,,g; #remove filename
    do $scriptdir  .'/file-utils-winstuff.pl';

    fileutils_file_put_from_file($fn,$userdata);
} else {
    open(FHHH,">$fn") || webdie("can't open file $fn for writing: $!");
    binmode(FHHH);    
    my ($cnt,$buf);
    while ($cnt=read($userdata,$buf,65536))
    {
        print FHHH $buf;
        $buf = '';
    }
    close FHHH;
    close $userdata;
}


