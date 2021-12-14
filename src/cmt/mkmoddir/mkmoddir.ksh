#! /bin/ksh
#
# $Id: mkmoddir.ksh,v 1.9 92/12/09 18:41:37 bill Exp $
#
# MKMODDIR - create a Code Model directory and install default template files
#
#     NOTE: must use ksh since Apollo sh does not support shell functions.
#
#     usage: mkmoddir [directory][-m spice-model-name][-c c-function-name]
#
#     If the names are not given on the command line, they are read
#     interactively.
#
# Steve Tynor - March 1991
#
#
# Copyright 1992
# Georgia Tech Research Corporation
# All Rights Reserved.
# This material may be reproduced by or for the U.S. Government
# pursuant to the copyright license under the clause at DFARS
# 252.227-7013 (Oct. 1988)
#

# Include global XSPICE definitions
. /usr/local/xspice-1-0/include/shell.include


template_dir=$ROOT/lib/cmt/mkmoddir

templ_mf=$template_dir/Makefile.tpl
templ_ifs=$template_dir/ifspec.tpl
templ_mod=$template_dir/cfunc.tpl

prog=$0

usage ()
{
        echo
	echo Error: $*
	echo usage: mkmoddir [directory][-m spice-model-name][-c c-function-name]
        echo
	exit 1
}

dirname=
modname=
funcname=

# Get arguments from command line, if any

echo
while [ -n "$1" ]
do
	case $1 in
		-m)
			if [ -n "$modname" ]
			then
				usage SPICE Model name already set.
			fi
			shift
			modname=$1 ;;
		-c)
			if [ -n "$funcname" ]
			then
				usage C Function name already set.
			fi
			shift
			funcname=$1 ;;
		?*)
			if [ -n "$dirname" ]
			then
				usage Directory name already set.
			fi
			dirname=$1 ;;
	esac
	shift
done

# Prompt for any needed names not given on command line

if [ -z "$dirname" ]
then
	echo -n "Directory name: "
	read dirname
fi

if [ -z "$modname" ]
then
	modname=`basename $dirname`
	echo -n "SPICE model name ["$modname"]: "
	read tmp
	if [ -n "$tmp" ]
	then
		modname=$tmp
	fi
fi

if [ -z "$funcname" ]
then
	funcname=ucm_$modname
	echo -n "C function name ["$funcname"]: "
	read tmp
	if [ -n "$tmp" ]
	then
		funcname=$tmp
	fi
fi

# Check for valid dirname, modname, and funcname

if [ -z "$dirname" -o -z "$modname" -o -z "$funcname" ]
then
        echo
	echo ERROR: invalid directory, SPICE model, or C function name.
        echo
	exit 1
fi

# Create the directory

echo
mkdir $dirname

if [ 0 != $?  ]
then
        echo
	echo ERROR: mkdir failed - unable to create model directory: $dirname.
        echo
	exit 1
fi

# Copy in the file templates and run "sed" to set the modname
# and funcname in the files.

cp $templ_mf  $dirname/Makefile
if [ 0 != $?  ]
then
        echo
	echo ERROR: Makefile template copy failed.
        echo
	exit 1
fi

sed -e "s/C_FUNC_NAME/$funcname/g" -e "s/MODEL_NAME/$modname/g" \
   $templ_ifs > $dirname/ifspec.ifs

if [ 0 != $?  ]
then
        echo
	echo ERROR: ifspec.ifs template copy failed.
        echo
	exit 1
fi

sed -e "s/C_FUNC_NAME/$funcname/g" -e "s/MODEL_NAME/$modname/g" \
   $templ_mod > $dirname/cfunc.mod

if [ 0 != $?  ]
then
        echo
	echo ERROR: cfunc.mod template copy failed.
        echo
	exit 1
fi

# Report success.

echo
echo Model Directory \"$dirname\" created.
echo
echo Edit files \"ifspec.ifs\" and \"cfunc.mod\"
echo to define your model.  Then run \"make\" to
echo preprocess and compile it.
echo

exit 0
