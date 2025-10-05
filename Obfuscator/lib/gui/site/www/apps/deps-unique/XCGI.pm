package XCGI;

sub run
{
    my ($params) = @_;
    $params->{function} = 'handler' if !exists $params->{function};
    my $f = "$params->{module}::$params->{function}(0,$params->{args})";
    if ($ENV{RUNNING_VIA_XCGI})
    {
	#returns a sub that caller has to invoke in order to exec that app
	unshift(@INC, $params->{inc})  if $params->{inc};
	#TODO: implement support for time-to-live
	my $modpath = $params->{module};
	$modpath =~ s,::,/,g;
	my $endstr = $params->{nocache} ? "\$ENV{EXIT_NEEDED} = 1;" : '';
	my $str = "sub { require $params->{module}; $f; $endstr; }";
	my $s;
	eval("\$s = $str");
	print STDERR "failed setting up XCGI callback: $@\n" if $@;
	return $s;
    } elsif ($ENV{FCGI_ROLE}) {
	#mod_fastcgi case
	unshift(@INC, $params->{inc})  if $params->{inc};
	eval { require $params->{module}; };
	eval "use CGI::Fast qw(:standard); ";
	while (new CGI::Fast)
	{
	    eval("$f;");
	};
    } else {
	#plain cgi case
	unshift(@INC, $params->{inc})  if $params->{inc};
	eval("require $params->{module}; $f;");
    };
}

1;