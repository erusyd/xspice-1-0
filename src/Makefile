#
# Copyright 1992
# Georgia Tech Research Corporation
# All Rights Reserved.
# This material may be reproduced by or for the U.S. Government
# pursuant to the copyright license under the clause at DFARS
# 252.227-7013 (Oct. 1988)
#
#
# Top-level Makefile for the XSPICE Simulator and Code Model Subsystem.
#
# NOTE: 'sim' must go last because it will build an executable
#       that includes the previously compiled objects.
#       'cmt' must go first because it is required to make
#       the models and node types in cml and udnl

SUBDIRS = cmt cml udnl sim


all: $(SUBDIRS)
	
$(SUBDIRS): dummy.make
	cd $@;\
		make

	
dummy.make: 
	true

