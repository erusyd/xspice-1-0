# $Id: Makefile,v 1.18 92/12/09 18:34:26 bill Exp $

# FILE  Makefile
# 
# MEMBER OF process cmpp
# 
# Copyright 1991
# Georgia Tech Research Corporation
# Atlanta, Georgia 30332
# All Rights Reserved
# 
# PROJECT A-8503
# 
# AUTHORS
# 
#     9/12/91  Steve Tynor
# 
# MODIFICATIONS
# 
#     7/5/92  Bill Kuhn  Include global make.include file for one-point
#                        specification of compiler, etc.
# 
# SUMMARY
# 
#     Contains make instructions for building process 'cmpp'.
# 


# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include

CFLAGS = -g

LFLAGS = -v

H_SRCS = cmpp.h ifs_yacc.h mod_yacc.h
C_SRCS = main.c pp_ifs.c pp_lst.c pp_mod.c read_ifs.c util.c writ_ifs.c
L_SRCS = ifs_lex.l mod_lex.l
Y_SRCS = ifs_yacc.y mod_yacc.y
S_SRCS = mod_lex.sed mod_yacc.sed

SRCS = $(H_SRCS) $(C_SRCS) $(L_SRCS) $(Y_SRCS) $(S_SRCS) Makefile

OBJS = $(C_SRCS:.c=.o) $(Y_SRCS:.y=.o) $(L_SRCS:.l=.o) 

SPROTOS = $(OBJS:.o=.sproto)
PROTOS = $(OBJS:.o=.proto)

.c.o: $*.c
	${CC} ${CFLAGS} ${INCLUDE} -c $*.c

all : cmpp

.KEEP_STATE:

cmpp : $(OBJS)
	$(CC) $(CFLAGS) -o cmpp $(OBJS)
	cp cmpp $(BINDIR)/cmpp

test_mod : test_mod.o mod_lex.o mod_yacc.o
	$(CC) $(CFLAGS) -o test_mod test_mod.o mod_lex.o mod_yacc.o

test_ifs : $(OBJS) test_ifs.o
	$(CC) $(CFLAGS) -o test_ifs test_ifs.o $(OBJS)

mod_yacc.c mod_tok.h : mod_yacc.y mod_yacc.h mod_yacc.sed
	$(YACC) -d mod_yacc.y
	mv y.tab.h mod_tok.h
	sed -f mod_yacc.sed y.tab.c > mod_yacc.c
	/bin/rm y.tab.c

mod_yacc.o : mod_yacc.c mod_tok.h 

mod_lex.o : mod_lex.l mod_tok.h mod_yacc.h mod_lex.sed
	$(LEX) -t -v mod_lex.l | sed -f mod_lex.sed > mod_lex.c
	$(CC) $(CFLAGS) -c mod_lex.c

ifs_yacc.c ifs_tok.h : ifs_yacc.y
	$(YACC) -d ifs_yacc.y
	mv y.tab.h ifs_tok.h
	mv y.tab.c ifs_yacc.c

ifs_yacc.o : ifs_yacc.c ifs_tok.h ifs_yacc.h

ifs_lex.o : ifs_lex.l ifs_tok.h ifs_yacc.h
	$(LEX) -t -v ifs_lex.l > ifs_lex.c
	$(CC) $(CFLAGS) -c ifs_lex.c

.SUFFIXES: $(SUFFIXES) .proto .sproto

.c.proto:
	mkproto_ifchange $(MKPROTOFLAG) $< $@

.c.sproto:
	mkproto -s $< > $@

include DEPEND

clean_proto:
	-/bin/rm *.proto *.sproto

fake_proto:
	touch $(PROTOS) $(SPROTOS)

TAGS: FORCE
	etags -t $(SRCS)

DEPEND: FORCE
	$(CC) $(CFLAGS) -M *.c > DEPEND

checkpoint:
	(echo -n "one-line ci Message: "; \
	 read msg; \
	 ci -q -u -m"$$$$msg" $(SRCS) ;\
	 rcsfreeze)

install: cmpp
	cp cmpp $(BINDIR)

print:
	lprint -lc *.[chyl] | psc> /tmp/t.$$$$;\
	lprint -ff Makefile *.sed | psc >> /tmp/t.$$$$;\
	cat /tmp/t.$$$$ | psnup -n 2 | /com/prf -printer lw -transparent 

FORCE:
