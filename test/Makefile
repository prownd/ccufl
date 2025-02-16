#ver 1.0
CC = gcc
MAKE=make


CFLAGS = -ggdb3 -Wall -fPIC -std=gnu99 -D_GNU_SOURCE -DPROV #-DNATION -DFRONT

INCS = -I ../src/

all: log_test  cjson_test adlist_test dict_test hashtable_test mxml_test sha1_test  ssort_test tree_test arraylist_test error_test htab_test pqueue_test stack_test uuid_test base64_test conf_test md5_test sds_test slist_test  generic_list_test

log_test : log_test.c ../src/log.c 
	$(CC) log_test.c ../src/log.c  -o log_test  $(CFLAGS) $(INCS)

adlist_test : adlist_test.c ../src/adlist.c 
	$(CC) adlist_test.c ../src/adlist.c  -o adlist_test  $(CFLAGS) $(INCS)

cjson_test : cjson_test.c ../src/cJSON.c 
	$(CC) cjson_test.c ../src/cJSON.c  -o cjson_test  $(CFLAGS) $(INCS) -lm

dict_test : dict_test.c ../src/dict.c 
	$(CC) dict_test.c ../src/dict.c  -o dict_test  $(CFLAGS) $(INCS)

hashtable_test : hashtable_test.c ../src/hashtable.c 
	$(CC) hashtable_test.c ../src/hashtable.c  -o hashtable_test  $(CFLAGS) $(INCS) -lm

mxml_test : mxml_test.c ../src/mxml.c 
	$(CC) mxml_test.c ../src/mxml.c  -o mxml_test  $(CFLAGS) $(INCS) 
	
sha1_test : sha1_test.c ../src/sha1.c 
	$(CC) sha1_test.c ../src/sha1.c  -o sha1_test  $(CFLAGS) $(INCS)

ssort_test : ssort_test.c ../src/ssort.c 
	$(CC) ssort_test.c ../src/ssort.c  -o ssort_test  $(CFLAGS) $(INCS)

tree_test : tree_test.c ../src/tree.c 
	$(CC) tree_test.c ../src/tree.c  -o tree_test  $(CFLAGS) $(INCS)

arraylist_test : arraylist_test.c ../src/arraylist.c 
	$(CC) arraylist_test.c ../src/arraylist.c  -o arraylist_test  $(CFLAGS) $(INCS)
	
error_test : error_test.c ../src/error.c 
	$(CC) error_test.c ../src/error.c  -o error_test  $(CFLAGS) $(INCS)

htab_test : htab_test.c ../src/htab.c 
	$(CC) htab_test.c ../src/htab.c  -o htab_test  $(CFLAGS) $(INCS)

pqueue_test : pqueue_test.c ../src/pqueue.c 
	$(CC) pqueue_test.c ../src/pqueue.c  -o pqueue_test  $(CFLAGS) $(INCS) -lpthread

stack_test : stack_test.c ../src/stack.c 
	$(CC) stack_test.c ../src/stack.c  -o stack_test  $(CFLAGS) $(INCS) -lpthread

uuid_test : uuid_test.c ../src/uuid.c 
	$(CC) uuid_test.c ../src/uuid.c  -o uuid_test  $(CFLAGS) $(INCS)

base64_test : base64_test.c ../src/base64.c 
	$(CC) base64_test.c ../src/base64.c  -o base64_test  $(CFLAGS) $(INCS)

conf_test : conf_test.c ../src/conf.c  ../src/htab.c ../src/util.c
	$(CC) conf_test.c ../src/conf.c  ../src/htab.c ../src/util.c -o conf_test  $(CFLAGS) $(INCS) -lm

md5_test : md5_test.c ../src/md5.c 
	$(CC) md5_test.c ../src/md5.c  -o md5_test  $(CFLAGS) $(INCS)
	
sds_test : sds_test.c ../src/sds.c 
	$(CC) sds_test.c ../src/sds.c  -o sds_test  $(CFLAGS) $(INCS)

slist_test : slist_test.c ../src/slist.c 
	$(CC) slist_test.c ../src/slist.c  -o slist_test  $(CFLAGS) $(INCS) -lpthread

generic_list_test :  generic_list_test.c 
	$(CC)  generic_list_test.c  -I ../src  -o  generic_list_test  $(CFLAGS) $(INCS) -lpthread

clean :
	$(RM) log_test  cjson_test adlist_test dict_test hashtable_test mxml_test sha1_test  ssort_test tree_test arraylist_test error_test htab_test pqueue_test stack_test uuid_test base64_test conf_test md5_test sds_test slist_test  generic_list_test
