
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CORE  = $(ROOT)/lib/sim/object/core.o
BCORE = $(ROOT)/lib/sim/object/bcore.o

CFILES	=\
	agraf.c\
	arg.c\
	aspice.c\
	breakpoint.c\
	circuits.c\
	clip.c\
	cmath1.c\
	cmath2.c\
	cmath3.c\
	cmath4.c\
	cmdtab.c\
	compose.c\
	cpinterface.c\
	debugcoms.c\
	define.c\
	device.c\
	diff.c\
	display.c\
	doplot.c\
	dotcards.c\
	error.c\
	evaluate.c\
	fourier.c\
	graf.c\
	graphdb.c\
	grid.c\
	inp.c\
	interpolate.c\
	mfb.c\
	misccoms.c\
	numparse.c\
	options.c\
	parse.c\
	plotcurve.c\
	plot5.c\
	postcoms.c\
	postscript.c\
	rawfile.c\
	resource.c\
	runcoms.c\
	shyu.c\
	signal.c\
	subckt.c\
	types.c\
	vectors.c\
	x10.c\
	x11.c\
	OUTinterface.c

#	writedata.c

COBJS	=\
	agraf.o\
	arg.o\
	aspice.o\
	breakpoint.o\
	circuits.o\
	clip.o\
	cmath1.o\
	cmath2.o\
	cmath3.o\
	cmath4.o\
	cmdtab.o\
	compose.o\
	cpinterface.o\
	debugcoms.o\
	define.o\
	device.o\
	diff.o\
	display.o\
	doplot.o\
	dotcards.o\
	error.o\
	evaluate.o\
	fourier.o\
	graf.o\
	graphdb.o\
	grid.o\
	inp.o\
	interpolate.o\
	mfb.o\
	misccoms.o\
	numparse.o\
	options.o\
	parse.o\
	plotcurve.o\
	plot5.o\
	postcoms.o\
	postscript.o\
	rawfile.o\
	resource.o\
	runcoms.o\
	shyu.o\
	signal.o\
	subckt.o\
	types.o\
	vectors.o\
	x10.o\
	x11.o\
	OUTinterface.o

#	writedata.o

CURSORS = \
	choice.cur\
	choicemask.cur\
	circle.cur\
	circlemask.cur\
	frown.cur\
	frownmask.cur\
	open.cur\
	openmask.cur\
	smpile.cur\
	smpilmask.cur\
	std.cur\
	stdmask.cur

BSFILES = bspice.o spiceif.o OUTinterface.o subckt.o \
	inp.o resource.o runcoms.o error.o types.o numparse.o shyu.o \
	../CP/quote.o ../CP/std.o ../CP/wlist.o ../CP/input.o ../CP/lexical.o \
	../CP/variable.o



INCLUDE = -I../include -I../MFB -I/usr/include/X11/Xaw -I/usr/include/X11

CFLAGS = 

LIBX = -L/usr/X11/lib -lXaw -lXt -lXmu -lX11

LDFLAGS = -z ${LIBX} -lm -ltermlib


.c.o: $*.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $*.c



all:  ../FTE.a core.o bcore.o
	@echo "All done.  core.o and bcore.o created and installed."

library:  ../FTE.a

../FTE.a:  $(COBJS)
	ar r ../FTE.a $?
	ranlib ../FTE.a



# Creation of XSPICE cores:
#
# Don't include SPIinit.o in the list.  It will get bound later when
# cmpp compiles the list of models included in the executable.  Also,
# note that spiceif.c has been modified to force inclusion of things
# in the core that were previously included due to external references
# found solely in SPIinit.c.
#
# Put ../xxx/*.o in the explicit object list to make sure all necessary fcns
# are available in the core.
#
core.o: nutmeg.o spiceif.o FORCE
	ld -r -o ${CORE} nutmeg.o spiceif.o \
		../CM/*.o ../MIF/*.o ../EVT/*.o ../ENH/*.o \
		../IDN/*.o ../ICM/POLY/*.o \
		../FTE.a ../*.a ../*.a ../*.a $(LDFLAGS)

bcore.o: $(BSFILES) FORCE
	ld -r -o ${BCORE} $(BSFILES) \
		../CM/*.o ../MIF/*.o ../EVT/*.o ../ENH/*.o ../IDN/*.o \
		../ICM/*/*.o \
		../INP.a ../CKT.a ../DEV.a ../NI.a ../SMP.a\
		../INP.a ../CKT.a ../DEV.a ../NI.a ../SMP.a\
		../INP.a ../CKT.a ../DEV.a ../NI.a ../SMP.a\
		../IPC.a


FORCE:
	true

