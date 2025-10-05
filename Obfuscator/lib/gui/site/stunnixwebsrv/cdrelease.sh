#!/bin/bash
#this script takes 3 params:
#	PID of SPWS (to kill it)
#	time in seconds to kill it after
#	path to cdrom mountpoint
#It's meant to work on any Unix and MacOSX
pid=$1
timeout=$2
cdpath=$3
donteject=$4
transfile=$5

(
trap "" INT;
trap ""  QUIT;
trap ""  CHLD;
trap ""  HUP;
trap ""  PIPE;
trap ""  TERM;

sleep ${timeout}
kill ${pid} 
sleep 2 #to let the slaves die

rm -f "${transfile}"


if test "${donteject}" -ne 1 ; then 
    #the commands below will work only on Unixes, but probably not on macosx
    umount "${cdpath}"  

    #the commands below will work only on Linuxes (or if 'eject' program is
    #installed)
    eject "${cdpath}";

    #this command is for macosx only.
    hdiutil detach "${cdpath}"

    #this command is for macosx only: in hopes there is only 1 cdrom in the 
    #computer
    #drutil tray eject
fi





) >/dev/null 2>&1

#more insights:
#       You could use AppleScript to tell the Finder to eject the CD. Call
#       this  AppleScript  from  Perl  using  the "osascript" command line
#       shell (or using Mac::Glue).
#       Or the 'disktool' command: disktool -e disk2. You need to know the
#       name  of  the disk, however (disktool can also list the disks cur-
#       rently mounted).
