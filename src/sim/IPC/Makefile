
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


# Uncomment the appropriate set of three lines below according to
# your IPC type.  Use the first if you do not plan to use IPC.

#IPC_KIND = IPC_DEBUG_VIA_STDIO
#OBJS   =  IPC.o IPCtiein.o IPCstdio.o
#CFILES =  IPC.c IPCtiein.c IPCstdio.c

#IPC_KIND = IPC_AEGIS_MAILBOXES
#OBJS   =  IPC.o IPCtiein.o IPCaegis.o
#CFILES =  IPC.c IPCtiein.c IPCaegis.c

IPC_KIND = IPC_UNIX_SOCKETS
OBJS   =  IPC.o IPCtiein.o IPCsockets.o
CFILES =  IPC.c IPCtiein.c IPCsockets.c



CFLAGS = -D$(IPC_KIND)

TARGET = ../IPC

INCLUDE= -I../include

.c.o: $*.c
	${CC} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a

${TARGET}.a:  ${OBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


