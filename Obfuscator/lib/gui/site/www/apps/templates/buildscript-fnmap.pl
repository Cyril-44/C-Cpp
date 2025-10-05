################################################################### buildscript-fnmap.pl begin
package fnmap;
use strict;
#use PossiblySensitiveHash;tie our(%fnmap), 'PossiblySensitiveHash';	tie our(%dirnmap), 'PossiblySensitiveHash';
our (%fnmap,%dirnmap); #maps relative path into new path

sub init_mapping { (%fnmap,%dirnmap) = ();
    return; #the code is disabled

    my $is_case_insensitive =  ($^O =~ /(win32)|(cygwin)/i || $^O eq 'darwin') ? 1 : 0;
    #$is_case_insensitive = 1; #testing it on linux
    (tied %fnmap)->set_insensitive($is_case_insensitive);
    (tied %dirnmap)->set_insensitive($is_case_insensitive);
}

sub _read_mapping_from_file
{
    my ($fn,$rhash) = @_;
    open(F,"<${fn}");
    foreach my $l (<F>)
    {
	$l =~ s,[\r\n]*,,sg;
	next if !length($l);
	my ($from,$to) = split(/[|]/,$l);
	$rhash->{$from} = $to;
    }
    close F;
}

sub _save_mapping_to_file
{
    my ($fn,$rhash) = @_;
    open(F,">${fn}");
    foreach my $k (sort { $a cmp $b } keys %$rhash)
    {
	print F $k . "|" . $rhash->{$k} . "\n";
    }
    close F;
}

sub read_fn_mapping_from_file{ my($fn) 
    = @_;
    _read_mapping_from_file($fn,\%fnmap);    
}


sub read_dirname_mapping_from_file{ my($fn) 
    = @_;
    _read_mapping_from_file($fn,\%dirnmap);    
}


sub save_fn_mapping_to_file{ my($fn) 
    = @_;
    _save_mapping_to_file($fn,\%fnmap);    
}


sub save_dirname_mapping_to_file{ my($fn) 
    = @_;
    _save_mapping_to_file($fn,\%dirnmap);    
}


sub list_modes_that_need_fn_mangling{my($modes,$modes_to_files)
    = @_;
    my (@list_of_modes);
    foreach my $m (keys %$modes)
    {
	if ($modes->{$m}->{fnmangling})
	{
	    my $l = $modes_to_files->{$m}; 
	    push @list_of_modes, $m if ($l && @$l);	    
	}
    }
    return @list_of_modes;
}

#returns list of fnames  it has to generate because they are missing
sub check_fnreplacement_available_for_list{my($allfnames)
    = @_;

    my (@missing_fns);
    foreach my $fn (@$allfnames) 
    {
	push @missing_fns, $fn if !length($fnmap{$fn});
    }

    return @missing_fns;
    
    #we skip directory names here because if filename for /a/b/fn.c is generated, then dirname for /a/b/ is already 
    #generated too.
}


#this can be called only after %fnmap is fully generated
sub build_missing_dirname_mapping
{
    my($alldirs) = @_;
    my (@togen);
    foreach my $d (@$alldirs)
    {
	push @togen,$d if ! exists $dirnmap{$d};
    }

    return if !@togen;

    #fill dirnmap with intermediate path. E.g. if we have mapping /a/b/c/d.c => /x/y/z/u.c then we
    # will store /a => /x, /a/b/ => /x/y/
    foreach  (keys %fnmap)
    {
	my $if = _normdirpath($_);
	my $of = _normdirpath($fnmap{$_});

	my @iparts = splitpath($if);
	my @oparts = splitpath($of);

	my ($i,@iparts_cur,@oparts_cur);
	for($i=0;$i<(@iparts-1);++$i)
	{
	    push @iparts_cur,$iparts[$i];
	    push @oparts_cur,$oparts[$i];

	    my ($id,$od) = (
		_normdirpath(join('/',@iparts_cur,'')),
		_normdirpath(join('/',@oparts_cur,''))
	    );
	    $dirnmap{$id} = $od;
	}
    }


    #if we have mapping /a/ => /x/ , then we can generate /a/q/w => /x/q/w
    foreach my $d (@$alldirs)
    {
	next if  exists $dirnmap{$d};
	my (@iparts) = splitpath(_normdirpath($d));
	my (@oparts,$ipath_prev);
	for(my $i=0;$i<@iparts;++$i)
	{
	    my @cur_iparts = @iparts[(0..$i)];
	    my $curipath = _normdirpath(join('/',@cur_iparts,''));
	    if (!exists $dirnmap{$curipath})
	    {
		for(my $j=$i;$j<@iparts;++$j)
		{
		    my $od =  _normdirpath(join('/',$dirnmap{$ipath_prev},@iparts[($i..$j)]));
		    my $id = _normdirpath(join('/',$ipath_prev, @iparts[($i..$j)]));
		    $dirnmap{$id} = $od;
		}
		last;
	    } else {
		$ipath_prev = $curipath;
	    }
	}
    }
}


# normalize directory path. i.e. ensure / at the begining and at the end
sub _normdirpath { 
    my ($fn) = @_;
    $fn .= '/';
    $fn =~ s,//+,/,g; 
    $fn =~ s,\./,/,g; #remove leading dot   
    $fn; 
}

#normalize path to file. Can't be called with path to dir!
sub _normfilepath { 
    my ($fn) = @_;
    $fn =~ s,//+,/,g; 
    $fn =~ s,^\./,/,g; #remove leading dot
    $fn; #TODO
}

sub splitpath
{	
    my($fn) = @_;
    my (@parts )  = split('/',$fn);
    return @parts;
}

sub filepath_to_dirpath_and_filename
{
    my($inpath) = @_;
    $inpath = _normfilepath($inpath);    
    my $slashpos = rindex($inpath,'/');
    die("no slash in path $inpath!") if $slashpos == -1;
    return (_normdirpath(substr($inpath,0,$slashpos)), substr($inpath,$slashpos+1));
}

sub do_map
{
    my($inpath) = @_;
    $inpath = _normfilepath($inpath);
    my $r = $fnmap{$inpath};
#print STDERR "DO_Map called for |$inpath| return is $r\n" if (length($r));
    return $r if (length($r));

    #if can be name of directory, without trailing / - check whether we have such in in the dirnmap
    if (substr($inpath,-1) ne '/') {
	$r =  $dirnmap{$inpath . '/'};
	return $r if length($r);
    }

    #we have no mapping. So we should have mapping for directory part of this path.
    {
	my ($dirpart,$fname) = filepath_to_dirpath_and_filename($inpath);
	my $od = $dirnmap{$dirpart};
	$r = $od  . $fname;
#print STDERR "about to return $r for $inpath\n ";
	return $r;
    }
}

1;

################################################################### buildscript-fnmap.pl end
