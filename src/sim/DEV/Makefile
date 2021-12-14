
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES = \
		DEVsupport.c

COBJS = \
		DEVsupport.o

SUBDIRS = \
		ASRC\
		BJT\
		BSIM\
		CAP\
		CCCS\
		CCVS\
		CSW\
		DIO\
		IND\
		ISRC\
		JFET\
		MES\
		MOS1\
		MOS2\
		MOS3\
		RES\
		SW\
		TRA\
		URC\
		VCCS\
		VCVS\
		VSRC

TARGET= ../DEV

INCLUDE= -I../include

CFLAGS = 

.c.o: $*.c
	${CC} ${CFLAGS} ${INCLUDE} -c $*.c



all: ${SUBDIRS} library

library: ${TARGET}.a

${TARGET}.a: ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a

${SUBDIRS}: dummy.make
	cd $@; \
		make
	
dummy.make:
	true


