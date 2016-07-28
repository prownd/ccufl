/*************************************************************************
> File Name: uuid_test.c
> Author: hanjinpeng
> Mail: hanjinpeng127@163.com 
> Created Time: 2016年07月27日 星期三 12时41分11秒
************************************************************************/
#include "uuid.h"

int main(int argc, char *argv[]){
  uuid_t u;
  char  uuid_str[38];
#if 0
  uuid_create(&u);
  printf("uuid_create(): "); puid(u);
  snpuid(uuid_str,sizeof(uuid_str),u);
  printf("\nas a string %s\n",uuid_str);
#endif
  get_uuid_string(uuid_str,sizeof(uuid_str));
  printf("uuid_str is %s\n",uuid_str);
  get_uuid_string_toupper(uuid_str,sizeof(uuid_str));
  printf("uuid_str is %s\n",uuid_str);
  return 0;
}
