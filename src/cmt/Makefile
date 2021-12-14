#
# Copyright 1992
# Georgia Tech Research Corporation
# All Rights Reserved.
# This material may be reproduced by or for the U.S. Government
# pursuant to the copyright license under the clause at DFARS
# 252.227-7013 (Oct. 1988)
#

SUBDIRS = cmpp mkmoddir mkudndir mksimdir


all: $(SUBDIRS)
	
$(SUBDIRS): dummy.make
	cd $@;\
		make

	
dummy.make: 
	true

