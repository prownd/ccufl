#v1
CC = gcc
AR=ar rc
LD=ld
MV=mv
INSTALL=install
DIR=ccufl


CFLAGS = -ggdb3 -Wall -fPIC -std=gnu99 -D_GNU_SOURCE -DPROV #-DNATION -DFRONT
LDFLAGS= -shared
LIB_DIR = ./lib
OBJS_DIR=./objs
BUILD_DEB_TMP_DIR=./build_deb_tmp_dir


LIB_OBJ=libccufl.a
SHARE_OBJ=libccufl.so
INCS = -I src/
SRCS = src/log.c 			\
	   src/htab.c			\
	   src/hashtable.c		\
	   src/slist.c			\
	   src/pqueue.c			\
	   src/conf.c			\
	   src/conn.c			\
	   src/error.c			\
	   src/tlv.c			\
	   src/times.c			\
	   src/util.c		    \
	   src/arraylist.c		\
	   src/stack.c			\
	   src/tree.c           \
	   src/md5.c			\
	   src/sha1.c			\
	   src/sha256.c			\
	   src/base64.c			\
	   src/adlist.c			\
	   src/sds.c			\
	   src/ssort.c			\
	   src/uuid.c
OBJS = objs/log.o 			\
	   objs/htab.o			\
	   objs/hashtable.o		\
	   objs/slist.o			\
	   objs/pqueue.o		\
	   objs/conn.o			\
	   objs/tlv.o			\
	   objs/error.o       	\
	   objs/conf.o			\
	   objs/times.o			\
	   objs/util.o		   	\
	   objs/arraylist.o		\
	   objs/stack.o		   	\
	   objs/tree.o			\
	   objs/md5.o		   	\
	   objs/sha1.o			\
	   objs/sha256.o		\
	   objs/base64.o		\
	   objs/adlist.o		\
	   objs/sds.o			\
	   objs/ssort.o			\
	   objs/uuid.o			

$(shell  mkdir  -p  ${OBJS_DIR})

all: $(SHARE_OBJ) $(LIB_OBJ)

$(SHARE_OBJ):$(OBJS)
	$(LD) -o $(SHARE_OBJ) $(OBJS) $(LDFLAGS)
	$(shell mkdir -p ${LIB_DIR})
	$(MV) $(SHARE_OBJ) $(LIB_DIR)

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

objs/error.o	: src/error.c
	$(CC) -c src/error.c -o objs/error.o $(CFLAGS)

objs/times.o : src/times.c
	$(CC) -c src/times.c -o objs/times.o $(CFLAGS)

objs/util.o : src/util.c
	$(CC) -c src/util.c -o objs/util.o $(CFLAGS)
	
objs/arraylist.o : src/arraylist.c
	$(CC) -c src/arraylist.c -o objs/arraylist.o $(CFLAGS)

objs/stack.o : src/stack.c
	$(CC) -c src/stack.c -o objs/stack.o $(CFLAGS)

objs/tree.o : src/tree.c
	$(CC) -c src/tree.c -o objs/tree.o $(CFLAGS)

objs/md5.o : src/md5.c
	$(CC) -c src/md5.c -o objs/md5.o $(CFLAGS)
	
objs/sha1.o : src/sha1.c
	$(CC) -c src/sha1.c -o objs/sha1.o $(CFLAGS)

objs/sha256.o : src/sha256.c
	$(CC) -c src/sha256.c -o objs/sha256.o $(CFLAGS)

objs/base64.o : src/base64.c
	$(CC) -c src/base64.c -o objs/base64.o $(CFLAGS)

objs/adlist.o : src/adlist.c
	$(CC) -c src/adlist.c -o objs/adlist.o $(CFLAGS)

objs/sds.o : src/sds.c
	$(CC) -c src/sds.c -o objs/sds.o $(CFLAGS)

objs/ssort.o : src/ssort.c
	$(CC) -c src/ssort.c -o objs/ssort.o $(CFLAGS)

objs/uuid.o : src/uuid.c
	$(CC) -c src/uuid.c -o objs/uuid.o $(CFLAGS)

dist :
	cd .. && tar cvzf $(DIR).tar.gz $(DIR)	

rpm:
	make clean
	tar -zcvf ccufl-1.0.tar.gz  ../ccufl/Makefile  ../ccufl/src/ ../ccufl/test ../ccufl/README.md ../ccufl/rpm_pkg/ccufl.spec
	rpmbuild -ta ccufl-1.0.tar.gz 
	$(RM) -f ccufl-1.0.tar.gz 
deb:
	make
	$(INSTALL) -d $(BUILD_DEB_TMP_DIR)
	$(INSTALL) -d  $(BUILD_DEB_TMP_DIR)/'usr/local/lib/'
	$(INSTALL) $(LIB_DIR)/$(LIB_OBJ) $(BUILD_DEB_TMP_DIR)/'usr/local/lib/'
	$(INSTALL) $(LIB_DIR)/$(SHARE_OBJ) $(BUILD_DEB_TMP_DIR)/'usr/local/lib/'
	$(INSTALL) -d  $(BUILD_DEB_TMP_DIR)/'usr/local/include/ccufl/'
	$(INSTALL) src/*.h  $(BUILD_DEB_TMP_DIR)/'usr/local/include/ccufl/'
	$(INSTALL) -d $(BUILD_DEB_TMP_DIR)/'DEBIAN'
	$(INSTALL) deb_pkg/ccufl.control  $(BUILD_DEB_TMP_DIR)/'DEBIAN'/control
	$(INSTALL) -d $(BUILD_DEB_TMP_DIR)/usr/share/doc/libccufl
	$(INSTALL) deb_pkg/ccufl_changelog  $(BUILD_DEB_TMP_DIR)/usr/share/dock/libccufl/changelog
	gzip --best $(BUILD_DEB_TMP_DIR)/usr/share/doc/libccufl/changelog
	dpkg  -b $(BUILD_DEB_TMP_DIR) libccufl_1.0.0-1_amd64.deb
	$(RM) -rf $(BUILD_DEB_TMP_DIR)
	make clean

install : 
	$(INSTALL) $(LIB_DIR)/$(LIB_OBJ) '/usr/local/lib/'
	$(INSTALL) $(LIB_DIR)/$(SHARE_OBJ) '/usr/local/lib/'
	ldconfig
	$(INSTALL) -d  '/usr/local/include/ccufl/'
	$(INSTALL) src/*.h  '/usr/local/include/ccufl/'
uninstall:
	${RM} -fr '/usr/local/include/ccufl'
	${RM} -f '/usr/local/lib/'$(LIB_OBJ)
	${RM} -f '/usr/local/lib/'$(SHARE_OBJ)
	ldconfig
clean :
	$(RM) $(OBJS) 
	$(RM) $(LIB_OJB)
	$(RM) -fr $(LIB_DIR)
	$(RM) -fr $(OBJS_DIR)
