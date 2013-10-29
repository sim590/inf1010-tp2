#--------------------------------------------------------
# File: Makefile    Author(s): Simon DÉSAULNIERS
#                              Frédéric Hamelin
# Date: 2013-10-22
#--------------------------------------------------------
# Makefile pour le code source du projet inf1010-tp2
#--------------------------------------------------------
BINDIR=../bin
PROGRAM=inf1010-tp2
CLISERV_HEADERS=client-server.h
CLISERV_LINK=

all: client server
client:
	gcc -o $(BINDIR)/$(PROGRAM)-client $(CLISERV_HEADERS) $(CLISERV_LINK) -lncurses client.c
server:                                                                                     
	gcc -o $(BINDIR)/$(PROGRAM)-server $(CLISERV_HEADERS) $(CLISERV_LINK) server.c
