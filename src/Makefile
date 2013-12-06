#--------------------------------------------------------
# File: Makefile    Author(s): Simon DÉSAULNIERS
#                              Frédéric Hamelin
# Date: 2013-10-22
#--------------------------------------------------------
# Makefile pour le code source du projet inf1010-tp2
#--------------------------------------------------------
BINDIR=../bin
PROGRAM=inf1010tp2
CLISERV_LINK=
CC=gcc -g -I. -c

all: client server
client: client.o BINDIR
	gcc -o $(BINDIR)/$(PROGRAM)-client $(CLISERV_LINK) client.o -lncurses
client.o: client.c client.h
	$(CC) client.c
server: server.o channels.o connections.o BINDIR
	gcc -o $(BINDIR)/$(PROGRAM)-server $(CLISERV_LINK) server.o channels.o connections.o -lpthread
server.o: server.c server.h
	$(CC) server.c
channels.o: channels.c channels.h
	$(CC) channels.c
connections.o: connections.c connections.h
	$(CC) connections.c
clean:
	rm -rf *.o
BINDIR:
	mkdir -p $(BINDIR)
