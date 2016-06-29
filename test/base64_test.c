#include <stdio.h>
#include <memory.h>
#include "base64.h"

int main(){
	char text[2][1024] = {{"fo"},{"foobar"}};
	char code[2][1024] = {{"Zm8="},{"Zm9vYmFy"}};
	char buf[1024];
	size_t buf_len;
	int i=0;
	for (i = 0; i < 2; i++) {
		buf_len = base64_encode(text[i], buf, strlen(text[i]), 1);
		printf("text:%s    --   encode:%s\n",text[i],buf);
		memset(buf, 0, sizeof(buf));
		buf_len = base64_decode(code[i], buf, strlen(code[i]));
		printf("code:%s    --   text:%s\n",code[i],buf);
	}
	return 0;
}
