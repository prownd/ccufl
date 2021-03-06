#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "sha1.h"

void print_hash(unsigned char hash[]){
	int idx;
	for (idx=0; idx < 20; idx++)
		printf("%02x",hash[idx]);
	printf("\n");
}

void sha1_string(char * string){
	SHA1_CTX ctx;
	char  buf[20];
	sha1_init(&ctx);
	sha1_update(&ctx, string, strlen(string));
	sha1_final(&ctx, buf);
	print_hash(buf);
}

int main(){
	/*
	char hash1[20] = {0xa9,0x99,0x3e,0x36,0x47,0x06,0x81,0x6a,0xba,0x3e,0x25,0x71,0x78,0x50,0xc2,0x6c,0x9c,0xd0,0xd8,0x9d};
	char hash2[20] = {0x84,0x98,0x3e,0x44,0x1c,0x3b,0xd2,0x6e,0xba,0xae,0x4a,0xa1,0xf9,0x51,0x29,0xe5,0xe5,0x46,0x70,0xf1};
	char hash3[20] = {0x34,0xaa,0x97,0x3c,0xd4,0xc4,0xda,0xa4,0xf6,0x1e,0xeb,0x2b,0xdb,0xad,0x27,0x31,0x65,0x34,0x01,0x6f};
	*/
	char text1[] = {"abc"};
	char text2[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
	char text3[] = {"aaaaaaaaaa"};
	sha1_string(text1);
	sha1_string(text2);
	sha1_string(text3);
	return(0);
}
