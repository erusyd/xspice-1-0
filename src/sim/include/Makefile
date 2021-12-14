# Make file for C version of spice 3

# for System-V make that uses the wrong shell otherwise
SHELL=/bin/sh

HFILES	= \
	ACdefs.h\
	ASRCdefs.h\
	BJTdefs.h\
	BSIMdefs.h\
	CAPdefs.h\
	CCCSdefs.h\
	CCVSdefs.h\
	CKTdefs.h\
	CONST.h\
	CPdefs.h\
	CPextern.h\
	CPstd.h\
	CSWdefs.h\
	DEVdefs.h\
	DIOdefs.h\
	FTEcmath.h\
	FTEconstant.h\
	FTEdata.h\
	FTEdbgraph.h\
	FTEdebug.h\
	FTEdefs.h\
	FTEdevice.h\
	FTEextern.h\
	FTEgraf.h\
	FTEgraph.h\
	FTEhelp.h\
	FTEinp.h\
	FTEinput.h\
	FTEparse.h\
	FTEredirect.h\
	GENdefs.h\
	HLPdefs.h\
	IFerrmsgs.h\
	IFsim.h\
	INDdefs.h\
	INPdefs.h\
	INPmacs.h\
	INPparseTree.h\
	ISRCdefs.h\
	JFETdefs.h\
	JOBdefs.h\
	MESdefs.h\
	MOS1defs.h\
	MOS2defs.h\
	MOS3defs.h\
	OPTdefs.h\
	OPdefs.h\
	PZdefs.h\
	RESdefs.h\
	SENdefs.h\
	SMPdefs.h\
	SPerror.h\
	SWdefs.h\
	TFdefs.h\
	TRANdefs.h\
	TRAdefs.h\
	TRCVdefs.h\
	TSKdefs.h\
	URCdefs.h\
	VCCSdefs.h\
	VCVSdefs.h\
	VSRCdefs.h\
	complex.h\
	prefix.h\
	prefixbsd.h\
	prefixcms.h\
	prefixhpux.h\
	prefixpc.h\
	prefixvms.h\
	suffix.h\
	suffixbsd.h\
	suffixcms.h\
	suffixhpux.h\
	suffixpc.h\
	suffixvms.h\
	util.h\
	version.h

VMSFILES= 

TARGET  = DUMMY

DEFS	= 
VERSION =
#----------------------------------------------------------------------------
SOURCE=	${HFILES}
ALLFILES= ${SOURCE} Makefile 

INCLUDE=
CFLAGS = -pg -O  -I${INCLUDE}
LINTFLAGS= -u -DDEBUG -I${INCLUDE}

LDFLAGS = -z

DESTDIR = 
MACHINE	= 
SRCDIR  = .

.c.o: $*.c
	${CC} ${CFLAGS} -c $*.c

source:	${SOURCE} tags

opt: ${SOURCE} tags

reopt: ${SOURCE} tags

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
#	ar r ${TARGET}.a $?
#	ranlib ${TARGET}.a

${TARGET}:	${COBJS} 
	${CC}  ${CFLAGS} -o ${TARGET} ${COBJS} ${LDFLAGS}

lint:	${SOURCE}
	lint ${LINTFLAGS} ${CFILES}

qgrind:	${SOURCE}
	qgrind -lc ${HFILES}

vgrind:	${SOURCE}
	vgrind -lc ${HFILES}

names:
	echo ${SOURCE} Makefile 
tags: ${SOURCE}
	ctags  ../*/*.c > /dev/null 2>&1

clean:
	-rm -f ${COBJS} 
	-rm -f ${CFILES}

install: ${TARGET}
	cp ${TARGET} ${DESTDIR}/${TARGET}
	strip ${DESTDIR}/${TARGET}

${HFILES}:
	co ${VERSION} $@

