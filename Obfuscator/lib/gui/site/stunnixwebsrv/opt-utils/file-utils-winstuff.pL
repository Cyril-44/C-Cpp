
=head1

This file is loaded only on Windows.

We have to use Win32 API functions since Perl functions do not allow unicode in file names (they accept only ANSI encoding).

This code uses webdie() function from outer scope.

=cut

use utf8;
use strict;
use Encode;
use FileHandle;
use Symbol         qw( gensym );
use Win32API::File qw( CreateFileW OsFHandleOpen OPEN_EXISTING GENERIC_WRITE CREATE_ALWAYS  GENERIC_WRITE OPEN_ALWAYS GENERIC_READ);# qw(GENERIC_WRITE OPEN_ALWAYS GENERIC_READ);


sub fileutils_file_get
{
	my ($fn) = @_;
	my $utf16  = Encode::encode("UTF-16LE", $fn);  
	utf8::downgrade($utf16);
       	$utf16  .= chr(0).chr(0);           # 0 terminate string
	my $F  = Win32API::File::CreateFileW($utf16, GENERIC_READ(), 0, [], OPEN_EXISTING(),0, 0); # Create file via Win32API
	print STDERR $^E if $^E;

	my $fh = gensym();
	Win32API::File::OsFHandleOpen($fh, $F, "r") or webdie("Cannot open file");


	binmode $fh; 
    	my ($cnt,$buf);
    	while ($cnt=read($fh,$buf,65536))
    	{
        	print $buf;
        	$buf = '';
    	}
    	close $fh;
}

sub fileutils_file_put
{
	my ($fn,$data) = @_;
	my $utf16  = Encode::encode("UTF-16LE", $fn);  
	utf8::downgrade($utf16);
       	$utf16  .= chr(0).chr(0);           # 0 terminate string
	my $F  = Win32API::File::CreateFileW($utf16, GENERIC_WRITE(), 0, [], CREATE_ALWAYS() ,0, 0); # Create file via Win32API
	print STDERR $^E if $^E;

	my $fh = gensym();
	Win32API::File::OsFHandleOpen($fh, $F, "w") or webdie("Cannot open file");
	binmode $fh; 
	print $fh $data;
 	close $fh;
}


sub     fileutils_file_put_from_file
{	
	my($fn,$userdata) = @_;
	my $utf16  = Encode::encode("UTF-16LE", $fn);  
	utf8::downgrade($utf16);
       	$utf16  .= chr(0).chr(0);           # 0 terminate string
	my $F  = Win32API::File::CreateFileW($utf16, GENERIC_WRITE, 0, [], CREATE_ALWAYS ,0, 0); # Create file via Win32API
	print STDERR $^E if $^E;

	my $fh = gensym();
	Win32API::File::OsFHandleOpen($fh, $F, "w") or webdie("Cannot open file");
	binmode $fh; 

    	my ($cnt,$buf);
    	while ($cnt=read($userdata,$buf,65536))
    	{
        	print $fh $buf;
        	$buf = '';
	}
 	close $fh;
 	close $userdata;
}




1;