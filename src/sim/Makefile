#
# Put DEV and FTE last since they are the most complex.  DEV takes a long
# time because of the number of subdirs.  FTE must be last since it builds
# the XSPICE cores.  The additional directory "simdir" builds default
# executables and installs them in the XSPICE bin directory.
#

SUBDIRS = CKT CP HLP INP NI SMP MFB   MIF CM EVT ENH IPC ICM IDN   DEV FTE  simdir


all: ${SUBDIRS}
	
${SUBDIRS}: dummy.make
	cd $@;\
		make

	
dummy.make: 
	true
