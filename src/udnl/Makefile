
SUBDIRS = real int


all: $(SUBDIRS)
	
$(SUBDIRS): dummy.make
	cd $@;\
		make
	cp -r $@ ../../lib/udnl/$@

	
dummy.make: 
	true
