#this code is Perl code - see http://perl.com for more details on the language. It's similar to C, except that 
#names of variables are prepended with $,@ or %
package main; #this line is required

#this subroutine is called for directories
#return 1 to tell that last component of path can be mangled. This subroutine will be called for each subdirectory 
#in the directory hierarchy
sub fnmangling_check_predicate_dirname{
    my($fullpath)  = @_;


    if (string_in_set($fullpath,'/foo/','/foo/bar/')) {
      return 0;
    };
    return 1; 
}



#############this subroutine is called for filenames. If Obfuscator needs answer about /foo/bar/myfile.c , it will pass:
# $fullpath =  /foo/bar/myfile.c     -  full path of the file relative to the project's input directory.
# $dirpart =  /foo/bar/     -  directory part of the file 
# $filename = myfile.c      - just name of file
# $basename = myfile              - name of file without extension
# $file_extension = c               - file's extension
####### Return value 
# Return value of 1 means 'yes, obfuscate name of file (myfile.c)'.
# Whether to mangle name of directory /foo/ and name of directory foo/bar/ will be told by 
# another subroutine - fnmangling_check_predicate_dirname
sub fnmangling_check_predicate_filename{
    my($fullpath,$dirpart,$filename,$basename,$file_extension)  = @_;

    #print STDERR "pred filename called for $filename\n"; #same logging aid - will trace this string in the
	# "the following output was produced" when you activate anything in Build->*build menu

    if (string_in_set($file_extension,'m','mm')) {
       return 0;
    }
    if ($filename =~ m"^constants_") {
       #do not alter names of files whoose name starts with constants_
       return 0;
    }

    return 1; 
} 


#this is utility function for checking whether first argument is in the list. Feel free to use it while implementing
#predicate's logic
sub string_in_set
{
  my($subj,@items) = @_;
  my %h;
  map {$h{$_}=1} @items;
  return 0+$h{$subj};
}



#ensure the code ends with 1; - otherwise a parsing failure will occur
1;

