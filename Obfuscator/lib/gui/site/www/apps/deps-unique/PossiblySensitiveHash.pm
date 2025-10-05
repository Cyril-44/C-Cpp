package PossiblySensitiveHash;

use strict;

use Tie::Hash;  # contains Tie::StdHash
use base 'Tie::StdHash';

sub TIEHASH  { 
	my ($class,$is_insensitive) = @_;
    my $this = {'d',{}, 'is_insensitive', $is_insensitive};
    bless $this, $class;
     }
sub STORE    { $_[0]->{d}->{  $_[0]->xform($_[1]) } = $_[2] }
sub FETCH    { $_[0]->{d}->{ $_[0]->xform($_[1])  } }
sub FIRSTKEY { my $a = scalar keys %{$_[0]->{d}}; each %{$_[0]->{d}} }
sub NEXTKEY  { each %{$_[0]->{d}} }
sub EXISTS   { exists $_[0]->{d}->{   $_[0]->xform($_[1]) } }
sub DELETE   { delete $_[0]->{d}->{ $_[0]->xform($_[1]) }  }
sub CLEAR    { %{$_[0]->{d}} = () }

sub xform
{
    my($this,$k) = @_;
    !$this->{is_insensitive} ? $k : lc($k);
}

sub set_insensitive { 
    my($this,$v) = @_;
    $this->{is_insensitive} = $v;
}

1;

=head

=head1 DESCRIPTION

This package implements possibly case-insensitive hash.

=head1 SYNOPSIS

 tie my(%hs), 'PossiblySensitiveHash',0;  #hash will be case-sensitive
 tie my(%hi), 'PossiblySensitiveHash',1;  #hash will be case-insensitive

=head1 COPYRIGHT

Copyright (c) stunnix.com 2015

=cut
