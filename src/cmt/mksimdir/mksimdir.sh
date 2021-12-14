#! /bin/sh
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
#     usage: mksimdir [directory]
#
#     If the directory name is not given on the command line, it is read
#     interactively.
#
# Bill Kuhn - March 1991 (Adapted from Steve Tynor's mkmoddir script)
#
# Note: not using shell functions for error messages since older
# versions of sh (such as the one in Apollo's Domain/OS 10.3) do not
# support them.

# Include global XSPICE definitions
. /usr/local/xspice-1-0/include/shell.include


template_dir=$ROOT/lib/cmt/mksimdir

templ_mf=$template_dir/Makefile.tpl
templ_mod_lst=$template_dir/modpath.tpl
templ_udn_lst=$template_dir/udnpath.tpl

prog=$0

usage="mksimdir [directory]"

dirname=

# Get directory name from command line if supplied

echo
while [ -n "$1" ]
do
	case $1 in
		?*)
			if [ -n "$dirname" ]
			then
			        echo
				echo ERROR: Directory name already set.
				echo $usage
			        echo
				exit 1
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
