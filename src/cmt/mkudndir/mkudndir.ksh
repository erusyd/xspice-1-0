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
# MKUDNDIR - create a User-Defined Node directory and install default template files
#
#     NOTE: must use ksh since Apollo sh does not support shell functions.
#
#     usage: mkudndir [directory] [-n node-type-name]
#
#     If the names are not given on the command line, they are read
#     interactively.
#
# Bill Kuhn - November 11 1991  -  Copied and adapted from mkmoddir by Stever Tynor
#

# Include global XSPICE definitions
. /usr/local/xspice-1-0/include/shell.include

template_dir=$ROOT/lib/cmt/mkudndir

templ_mf=$template_dir/Makefile.tpl
templ_udn=$template_dir/udnfunc.tpl

prog=$0

usage ()
{
        echo
	echo Error: $*
	echo usage: mkudndir [directory] [-n node-type-name]
        echo
	exit 1
}

dirname=
nodename=

# Get command line arguments

echo
while [ -n "$1" ]
do
	case $1 in
		-n)
			if [ -n "$nodename" ]
			then
				usage SPICE Model name already set.
			fi
			shift
			nodename=$1 ;;
		?*)
			if [ -n "$dirname" ]
			then
				usage Directory name already set.
			fi
			dirname=$1 ;;
	esac
	shift
done

# Prompt for any required names not given on command line

if [ -z "$dirname" ]
then
	echo -n "Directory name: "
	read dirname
fi

if [ -z "$nodename" ]
then
	nodename=`basename $dirname`
	echo -n "Node type name ["$nodename"]: "
	read tmp
	if [ -n "$tmp" ]
	then
		nodename=$tmp
	fi
fi

if [ -z "$dirname" -o -z "$nodename" ]
then
        echo
	echo ERROR: invalid directory, or udn type name.
        echo
	exit 1
fi

# Create the new directory

echo
mkdir $dirname

if [ 0 != $?  ]
then
        echo
	echo ERROR: mkdir failed - unable to create model directory: $dirname.
        echo
	exit 1
fi

# Copy over file templates and run "sed" to use user-supplied
# name

cp $templ_mf  $dirname/Makefile
if [ 0 != $?  ]
then
        echo
	echo ERROR: Makefile template copy failed.
        echo
	exit 1
fi

sed -e "s/NODE_TYPE_NAME/$nodename/g" \
   $templ_udn > $dirname/udnfunc.c

if [ 0 != $?  ]
then
        echo
	echo ERROR: udnfunc.c template copy failed.
        echo
	exit 1
fi

# Report success

echo
echo User-Defined Node directory \"$dirname\" created.
echo
echo Edit file \"udnfunc.c\" to define your node type.
echo Then run \"make\" to compile it.
echo

exit 0
