CC = gcc
AR=ar rc
MV=mv

CFLAGS = -ggdb3 -Wall -std=gnu99 -D_GNU_SOURCE -DPROV #-DNATION -DFRONT
LIB_DIR = ./lib
OBJS_DIR=./objs

LIB_OBJ=libccufl.a
INCS = -I src/
SRCS = src/log.c 		\
	   src/htab.c		\
	   src/hashtable.c	\
	   src/slist.c		\
	   src/pqueue.c		\
	   src/conf.c		\
	   src/conn.c		\
	   src/log.c		\
	   src/tlv.c		\
	   src/times.c		\
	   src/util.c		
OBJS = objs/log.o 		\
	   objs/htab.o		\
	   objs/hashtable.o	\
	   objs/slist.o		\
	   objs/pqueue.o	\
	   objs/conn.o		\
	   objs/tlv.o		\
	   objs/log.o       \
	   objs/conf.o		\
	   objs/times.o		\
	   objs/util.o		
#LIBS = -L ./lib/  -lpthread -lm -lz

$(shell  mkdir  -p  ${OBJS_DIR})


all: $(LIB_OBJ)

$(LIB_OBJ):$(OBJS)
	$(AR) $(LIB_OBJ) $(OBJS)	
	$(shell mkdir -p ${LIB_DIR})
	$(MV) $(LIB_OBJ) $(LIB_DIR)
objs/log.o : src/log.c
		$(CC) -c src/log.c -o objs/log.o $(CFLAGS)

objs/htab.o : src/htab.c
		$(CC) -c src/htab.c -o objs/htab.o $(CFLAGS)

objs/hashtable.o : src/hashtable.c
		$(CC) -c src/hashtable.c -o objs/hashtable.o $(CFLAGS)

objs/slist.o : src/slist.c
		$(CC) -c src/slist.c -o objs/slist.o $(CFLAGS)

objs/pqueue.o : src/pqueue.c
		$(CC) -c src/pqueue.c -o objs/pqueue.o $(CFLAGS)

objs/conn.o	: src/conn.c
		$(CC) -c src/conn.c -o objs/conn.o $(CFLAGS)

objs/tlv.o : src/tlv.c
		$(CC) -c src/tlv.c -o objs/tlv.o $(CFLAGS)

objs/conf.o	: src/conf.c
		$(CC) -c src/conf.c -o objs/conf.o $(CFLAGS)

objs/times.o : src/times.c
		$(CC) -c src/times.c -o objs/times.o $(CFLAGS)

objs/util.o : src/util.c
		$(CC) -c src/util.c -o objs/util.o $(CFLAGS)
	
dist :
		cd .. && tar cvzf $(DIR).tar.gz $(DIR)	

install : 
		test -d '/usr/local/$(POJ)' || mkdir -p '/usr/local/$(POJ)'
#		test -d '/usr/local/$(POJ)/bin' || mkdir -p '/usr/local/$(POJ)/bin'

clean :
		$(RM) $(OBJS) 
		$(RM) $(LIB_OJB)
		$(RM) -fr $(LIB_DIR)
		$(RM) -fr $(OBJS_DIR)

