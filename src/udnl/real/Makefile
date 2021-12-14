#
# Makefile for User Defined Node directories
#

# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


INCLUDE = -I. -I$(ROOT)/include/sim

CFLAGS = 

.c.o: $*.c
	${CC} ${CFLAGS} ${INCLUDE} -c $*.c

#-----------------------------------------------------------------------------
# Edit the following definition to specify the object files that comprise 
# your node type. If your node type is completely specified in the 
# udnfunc.mod file, there is no need to edit this definition. 

CODE_MODEL_OBJECTS = udnfunc.o 

#-----------------------------------------------------------------------------
# DO NOT MODIFY THE FOLLOWING DEFINITIONS:

all : $(CODE_MODEL_OBJECTS)

udnfunc.o : udnfunc.c
