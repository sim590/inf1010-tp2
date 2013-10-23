#--------------------------------------------------------
# File: Makefile    Author(s): Simon DÉSAULNIERS
#                              Frédéric Hamelin
# Date: 2013-10-22
#--------------------------------------------------------
# Makefile pour le code source du projet inf1010-tp2
#--------------------------------------------------------
BINDIR=../bin
PROGRAM=inf1010-tp2
CLIENT_HEADERS=-lncurses
SERVER_HEADERS=

all: client server
client:
	gcc -o $(BINDIR)/$(PROGRAM)-client client.c $(CLIENT_HEADERS)
server:
	gcc -o $(BINDIR)/$(PROGRAM)-server server.c $(SERVER_HEADERS)
