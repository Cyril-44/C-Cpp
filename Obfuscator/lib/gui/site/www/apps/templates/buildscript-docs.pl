=pod 

=head1 NAME

buildscript.pl - script to reprocess all or only changed parts of the project
using one of Stunnix Obfuscators.

=head1 SYNOPSIS

perl B<buildscript.pl>
    S<[ B<--help> ]>
    S<[ B<--verbose ..>] [ B<--nostop> ] [ B<--force-one-by-one> ] > 
    S<[ B<--list-autoadded-files> ] [ B<--list-absent-files> ] [ B<--list-unhandled-files> ] > 
    S<[ B<--override-input-directory ..>] >
    S<[ B<--override-output-directory ..>] > 
    S<[ B<--override-state-directory ..>] >
    S<[ B<--override-symbols-subdirectory ..>] >
    S<[ B<--op> I<build>|I<build-and-test>|I<rebuildall-and-test>| >
    S<	        I<rebuildall>|I<clearout>|I<clearstate>|I<clearall>|I<no-op>] >
    S<[ I<filenames...> ] >

=head1 DESCRIPTION

This is a perl script file that can be used to perform various operations
on the project created in Stunnix Obfuscator Project Manager. This script
can be recreated by running Stunnix Obfuscator Project Manager, then visiting
Project menu and clicking on the menu item I<Generate buildscript> - you
will be prompted to enter name of file to save this script to. Also it's possible to
regenerate it from command line, see documentation for generate-buildscript-from-project-file.pl in the I<Help>
menu or online. On Windows, it's possible to generate buildscript in form of bat file too (there is a separate
menu item in Project menu for that).

In order to run this script, type I<perl buildscript.pl [arguments]> (in case you've generated it as Windows .bat
file - just run that file).


This script can be used to rebuild all files, rebuild only "dirty" files,
clear all output files and/or state files produced during build process - 
the operation performed is specified as the value of the I<op> commandline
option. It's possible to specify the set of files to apply 
operation 
to by passing space-separated list of file names (relative to the project 
"input" directory) after all commandline arguments. There are other switches 
to affect the behaviour.

This script is cross-platform, it will work correctly on any computer
provided that paths listed
in the script are correct for your system. It's possible to edit this file
by hand in order to tweak commandline options or set of files, provided
Perl language syntax is respected.

This script can be thought of as a replacement for Makefile for
I<make> utility, traditionally used for building large or complex projects.
It's recommended to generate such script rather than to write Makefile
instead since commandlines to be written are rather complex. So the 
recommended approach for people who prefer writing Makefiles for their 
projects is to create project in Stunnix Obfuscator Project Manager,
properly assign all options, export the buildscript and then use the
buildscript for building the project; if necessary, changes in the 
commandline options and adding set of files can be performed by editing
this script (though using Stunnix Obfuscator Project Manager would
still be more convenient for this task too).

If this script is invoked with empty commandline, all "dirty" output files 
(for which corresponding original files are more fresh than output files)
will be rebuilt.

Execution is aborted in case error occurs during processing of some 
file. It's possible to not abort processing in case of error by passing 
option I<--nostop>.

Effectively buildscript contains names of file names (relative to Project's Input Directory)
and settings used to process them, and meta settings like location of Stunnix Obfuscator and Perl interpreter
and Project's Input, Output and State directories (which can be overriden from command line). 
Exceptions are also not embedded into buildscript, they are loaded from file 
I<list-of-symbols-for-obfuscator/user-exceptions.txt> residing in Project's Input directory.

The set of files belonging to the project can be configured in I<Settings>, I<For files> menu.
It's possible to define rules for processing files that do not belong to the project, given their filename extension,
in I<Settings>, I<Edit handlng of unlisted files>, and these rules are also respected by buildscripts. 
This allows to create a "universal" buildscript, that will allow processing all files in any directory recursively - 
just create a project with no files (or remove all files from your project using I<Settings>, I<For files>, 
checking all files and directories and clicking I<Stop managing>), define rules using
 I<Settings>, I<Edit handlng of unlisted files> based on filename extension, and invoke generated buildscript overriding
Project's Input/Output/State Directories using I<--override-XXX-directory> command line options.

It's possible to generate buildscripts from .op files (it's an internal form of Project Manager's project files)
using command line using I<generate-buildscript-from-project-file.pl> tool.

=head1 OPTIONS

=over 4

=item B<--op>

Specifies operation to be performed. Can be either of the following:

=over 2

=item I<build>

Process only those files, which were changed after their protected version
was built (or for which no protected version exists). This is the default
operation, it's performed when no I<--op> option was specified at all.

=item I<build-and-test>

Same as I<build>, but also runs B<test command> (specified in project options)
if processing all files was successful. It's useful to run some testsuite or
just navigate the browser to some page by using the test command.

=item I<rebuildall>

Processes all files in the project, without comparing the time of the
modification of input and output files.

=item I<rebuild-and-test>

Same as I<rebuild>, but also runs but B<test command> (specified in 
project options) if processing of all files was successful. 

=item I<clearout>

Clear all output files.

=item I<clearstate>

Clear all state files that were produced during building of the project -
such files are typically lists of exceptions extracted automatically and
mapping of original to protected symbols.

=item I<clearall>

Does everything that operations I<clearout> and I<clearstate>
do.

=item I<no-op>

Does nothing. Useful with B<--list-autoadded-files> to query various info about the project.

=back


=item B<--list-autoadded-files>

Prints list of files in the project that got their processing mode autoassigned.
Use with B<--op no-op> to prevent any operation performed with files.

=item B<--list-absent-files>

Prints list of files inside input directory that are registered in the project but are missing in the
filesystem.
Use with B<--op no-op> to prevent any operation performed with files.

=item B<--list-unhandled-files> 

Prints list of files inside input directory that are neither registered in the project nor autoadded to the project.
Use with B<--op no-op> to prevent any operation performed with files.

=item B<--verbose>

Passing this option multiplie types increases level of verbosity of its
output - e.g. printing file name being processed, command line being executed and so on..

=item B<--nostop>

Do not abort if some error occured while processing some file.

=item B<--force-one-by-one>

Forces protecting files one by one. This dramatically increases time it takes to protect the project.
Use this option only for debugging obfuscator itself, to find out which of input files is causing obfuscator
to abort execution.

=item B<--override-input-directory ..>

=item B<--override-output-directory ..>

=item B<--override-state-directory ..>

=item B<--override-symbols-subdirectory ..>

Override location of project's input directory, project's output directory, project's state directory or
project's directory where names of symbols are stored.
These command line options allow you to have several sets of project trees to be handled by a single buildscript.
There is no need to override all these directories at once - you can e.g. override only project's input 
directory. If you've specified project's output or project's state directory as relative paths (either in project's
settings or via these command line options), they will be handled correctly.

=item B<--help>

Produces help on options the script supports.

=back

=cut
