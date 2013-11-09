#--------------------------------------------------------
# File: Makefile    Author(s): Simon DÉSAULNIERS
#                              Frédéric Hamelin
# Date: 2013-10-22
#--------------------------------------------------------
# Makefile pour le code source du projet inf1010-tp2
#--------------------------------------------------------
BINDIR=../bin
PROGRAM=inf1010-tp2
CLISERV_LINK=
DEBUG=-g

all: client server
client: client.o
	gcc -I. -o $(BINDIR)/$(PROGRAM)-client $(CLISERV_LINK) -lncurses client.o 
client.o: client.c client.h
	gcc -I. -c $(DEBUG) client.c
server: server.o
	gcc -I. -o $(BINDIR)/$(PROGRAM)-server $(CLISERV_LINK) -lpthread server.o 
server.o: server.c server.h
	gcc -I. -c $(DEBUG) server.c
