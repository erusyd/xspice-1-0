#! /bin/ksh
#
#
# Copyright 1992
# Georgia Tech Research Corporation
# All Rights Reserved.
# This material may be reproduced by or for the U.S. Government
# pursuant to the copyright license under the clause at DFARS
# 252.227-7013 (Oct. 1988)
#
#
# MKSIMDIR - create a simulator directory and install default template files
#
#     NOTE: must use ksh since Apollo sh does not support shell functions.
#
#     usage: mksimdir [directory]
#
#     If the directory name is not given on the command line, it is read
#     interactively.
#
# Bill Kuhn - March 1991 (Adapted from Steve Tynor's mkmoddir script)
#

# Include global XSPICE definitions
. /usr/local/xspice-1-0/include/shell.include


template_dir=$ROOT/lib/cmt/mksimdir

templ_mf=$template_dir/Makefile.tpl
templ_mod_lst=$template_dir/modpath.tpl
templ_udn_lst=$template_dir/udnpath.tpl

prog=$0

usage ()
{
        echo
	echo Error: $*
	echo usage: mksimdir [directory]
        echo
	exit 1
}

dirname=

# Get directory name from command line if supplied

echo
while [ -n "$1" ]
do
	case $1 in
		?*)
			if [ -n "$dirname" ]
			then
				usage Directory name already set.
			fi
			dirname=$1 ;;
	esac
	shift
done

# If not supplied on command line, prompt for directory name.

if [ -z "$dirname" ]
then
	echo -n "Directory name: "
	read dirname
fi

if [ -z "$dirname" ]
then
        echo
	echo ERROR: invalid directory.
        echo
	exit 1
fi

# Create the new directory

echo
mkdir $dirname

if [ 0 != $?  ]
then
        echo
	echo ERROR: mkdir failed - unable to create simulator directory: $dirname.
        echo
	exit 1
fi

# Copy over the file templates.

cp $templ_mf  $dirname/Makefile
if [ 0 != $?  ]
then
        echo
	echo ERROR: Makefile copy failed.
        echo
	exit 1
fi

cp $templ_mod_lst  $dirname/modpath.lst
if [ 0 != $?  ]
then
        echo
	echo ERROR: modpath.lst template copy failed.
        echo
	exit 1
fi

cp $templ_udn_lst  $dirname/udnpath.lst
if [ 0 != $?  ]
then
        echo
	echo ERROR: udnpath.lst template copy failed.
        echo
	exit 1
fi

# Report success

echo
echo Simulator directory \"$dirname\" created.
echo
echo Edit files \"modpath.lst\" and \"udnpath.lst\" to
echo specify desired models and node types respectively.
echo Then run \"make\" to build the simulator executable.
echo

exit 0
