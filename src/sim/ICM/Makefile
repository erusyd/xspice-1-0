
SUBDIRS = POLY


all: ${SUBDIRS}

${SUBDIRS}: dummy.make
	cd $@; \
		make

dummy.make:
	true


