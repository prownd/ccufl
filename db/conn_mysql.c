#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
//gcc connmysql.c  -I/usr/include -lmysqlclient -lpthread -ldl -lm -L/usr/lib/mysql
/*
如果使用math.h库,编译时 要用参数 -lm
Q. 编译程序时得到undefined reference to 'xxxx'这样的错误提示 
A: 缺少某个库，用 -l参数将库加入。Linux的库命名是一致的，一般为libxxx.so,或libxxx.a,libxxx.la，那么你要链接某个库就用-lxxx，去掉头lib及"."后面的so，la,a等即可。 
同时，常见的库链接方法为： 
数学库 -lm ,链接libm.a ; posix线程 -lpthread 
这个过程是链接过程,程序中的 #include <math>语句只是保证编译通过. 
*/
/*
获取客户机版本信息：
char *mysql_get_client_info(void)
获取主机信息：
char *mysql_get_host_info(MYSQL *mysql)
获取协议版本信息：
unsigned int mysql_get_proto_info(MYSQL *mysql)
获取服务器版本信息：
char *mysql_get_server_info(MYSQL *mysql) 
获取可用数据库列表：
MYSQL_RES *mysql_list_dbs(MYSQL *mysql, const char *wild)
获取数据库的可有表列表
MYSQL_RES *mysql_list_tables(MYSQL *mysql, const char *wild)

获得结果集中的域的属性：
MYSQL_FIELD *mysql_fetch_field(MYSQL_RES *result)
获得域属性数组：
MYSQL_FIELD *mysql_fetch_fields(MYSQL_RES *result)
查询被update、delete、insert等受影响的行：
my_ulonglong mysql_affected_rows(MYSQL *mysql

1. 利用函数的返回值来判断函数执行是否正确。
2.使用mysql提供的错误号和错误信息：
错误号：
unsigned int mysql_errno(MYSQL *mysql)
错误信息：
char *mysql_error(MYSQL *mysql)



mysql -u root < crtdb
mysql -u root < indb
mysql -u root < usedb
mysql -u root < drpdb
*/


int main(){
	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int rownums; 
	if(mysql_init(&mysql))//初始化
			printf("init success..\n");
	else
			printf("init fail.. \n"),exit(1);
	//初始化数据结构 成功返回初始化句柄 失败为null
	if(mysql_real_connect(&mysql,"localhost","root","root","test",0,NULL,0))//进行连接mysql
			printf("connect success.. \n");
	else
			printf("connect failed ,try again..\n");
	//初始化数据结构 成功返回初始化句柄 失败为null
	if(mysql_query(&mysql,"select * from test")==0) //进行查询
			printf("query success..\n");
	else
			printf("query failed..\n");

	//get result
	if((result=mysql_store_result(&mysql))==NULL){
		fprintf(stderr,"Error in store_result: %s [%d]\n",mysql_error(&mysql),mysql_errno(&mysql));
		mysql_free_result(result);
		mysql_close(&mysql); 
		exit(-4);
	}
	//result=mysql_store_result(&mysql);//将查询的结果放在mysql_result结构中
	printf("rownums is :%ld\n",mysql_num_rows(result));//多少行
	printf("filednums is :%d\n",mysql_field_count(&mysql));//一行的字段数
	printf("filednums is :%d\n",mysql_num_fields(result));//一行的字段数
	while((row=mysql_fetch_row(result))){
		fprintf(stdout,"%s | %s | %s\n",row[0],row[1],row[2]);//row[]对应一行的字段数 0,1,2..
	}
	mysql_free_result(result);//关闭result结果集
	//mysql_query(&mysql,"insert into test values(9,'qianjiu',10119)");
	mysql_close(&mysql);//关闭mysql连接
	return 0;
}
