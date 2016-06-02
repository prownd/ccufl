#include <stdio.h>
#include "mongo.h"
int main(){

    bson b, sub;
	bson_iterator itt;
	/* Create a rich document like this one:
	*
	* { _id: ObjectId("4d95ea712b752328eb2fc2cc"),
	*   user_id: ObjectId("4d95ea712b752328eb2fc2cd")
	*
	*   items: [
	*     { sku: "col-123",
	*       name: "John Coltrane: Impressions",
	*       price: 1099,
	*     },
	*
	*     { sku: "young-456",
	*       name: "Larry Young: Unity",
	*       price: 1199
	*     }
	*   ],
	*																						          *   address: {
	*     street: "59 18th St.",
    *     zip: 10010																
	*   },
	*
	*   total: 2298
	* }
	*/
	bson_init( &b );
	bson_append_new_oid( &b, "user_id" );
	bson_append_start_array( &b, "items" );
	bson_append_start_object( &b, "0" );
	bson_append_string( &b, "name", "John Coltrane: Impressions" );
	bson_append_int( &b, "price", 1099 );
	bson_append_finish_object( &b );
	bson_append_start_object( &b, "1" );
	bson_append_string( &b, "name", "Larry Young: Unity" );
	bson_append_int( &b, "price", 1199 );
	bson_append_finish_object( &b );
	bson_append_finish_object( &b );
	bson_append_start_object( &b, "address" );
	bson_append_string( &b, "street", "59 18th St." );
	bson_append_int( &b, "zip", 10010 );
	bson_append_finish_object( &b );
	bson_append_int( &b, "total", 2298 );
	bson_finish( &b );
	bson_print(&b);
	/* Advance to the 'items' array */
	bson_find( &itt, &b, "items" );
	/* Get the subobject representing items */
	bson_iterator_subobject( &itt, &sub );
	/* Now iterate that object */
	bson_print( &sub );

	/*
	*mongo的数据库
	*admin
	*han
	*test   --包含hehe表  test表
	*
	*/ 
	char * db="test";//普通数据库名字
	char * db1="admin";//admin数据库名字
	char * col="hehe";
	mongo conn[1];
	int ret;
	bson cmd[6],out[1];
	bson_iterator it[1];
	const char * result;
	int status=mongo_connect(conn,"127.0.0.1",27017);
	if(status==MONGO_OK)
			printf("连接成功\n");
	else	
		printf("连接成功\n");
	
	
	/*auth 认证 当mongod --auth启动时候就操作数据库就会有权限限制 
	  这样就要求认证 认证通过就可以记性相应的操作了
	每个数据库下面都有一个system.user表  
	存放用户 admin下的是系统用户就是dba
	普通数据库下的是普通用户 
	可以进行添加修改用户 db.addUser("user","passwd")  
	删除时db.system.users.remove({"name":"username1"})
	dba系统管理员必须在admin下认证 然后  就可以切换到其他数据库进行操作 
	普通的只能在当前数据库下操作
	*/
	if(mongo_cmd_authenticate(conn,db,"test","test")==MONGO_OK)
		printf("auth user  test success\n");
	else
		printf("auth user fail\n");
	if(mongo_cmd_authenticate(conn,db1,"root","root")==MONGO_OK)
		printf("auth admin user success\n");
	else
		printf("auth user fail\n");
	//添加一个用户 db.addUser("user","passwd")
	if(mongo_cmd_add_user(conn,"test","han1","test")==MONGO_OK)
			printf("添加用户成功\n");
	//drop table ==db.test.drop()==db.runCommand({"drop":"test"})
	if(mongo_cmd_drop_collection(conn,db1,col,NULL)==MONGO_OK)
			printf("drop collection success\n");
	else
			printf("drop collections fail\n");
	bson_init( cmd );
	bson_append_int( cmd, "buildinfo", 1 );
	bson_finish( cmd );		
	bson_init( cmd+1 );
	bson_append_string( cmd+1, "drop", "hehe" );
	bson_finish( cmd+1 );		
	bson_init( cmd+2 );
	bson_append_int( cmd+2, "listCommands", 1 );
	bson_finish( cmd+2 );		
	bson_init( cmd+3 );
	bson_append_int( cmd+3, "listDatabases", 1 );
	bson_finish( cmd+3 );		
	bson_init( cmd+4 );
	bson_append_int( cmd+4, "ping", 1 );
	bson_finish( cmd+4 );
	
	bson_init(cmd+5);
	bson_append_start_object(cmd+5,"group");
	bson_append_string(cmd+5,"ns","testgroup");
	bson_append_start_object(cmd+5,"key");
	bson_append_string(cmd+5,"age","true");
	bson_append_finish_object(cmd+5);
	bson_append_start_object(cmd+5,"initial");
	bson_append_int(cmd+5,"num",0);
	bson_append_finish_object(cmd+5);
	bson_append_string(cmd+5,"$reduce","function(doc,prev){prev.num++;}");
	bson_append_finish_object(cmd+5);
	bson_finish(cmd+5);
	/*group分组  用rumCommand分组
	db.runCommand({"group": {"ns":"testgroup","key":{"age":"true"},"initial":{"num":0},"$reduce":"function(doc,prev){prev.num++;}"}})
	*/
	if( mongo_run_command( conn, "admin", cmd, out ) == MONGO_ERROR ) {	
			ret = -1;
	}else {
		bson_iterator_init( it, out );
		result = bson_iterator_string( it );
		//memcpy( version, result, strlen( result ) );
		//bson_destroy( out );
		printf("result is :%s\n",result);
	}

	// 在test数据库中删除表test   db.runCommand({"drop":"test"})
	//==test.runCommand({"drop":"test"})
	//==test.$cmd.findone({"drop":"test"})
	//
	if( mongo_run_command( conn, "test", cmd+1, out ) == MONGO_OK )
			printf("删除hehe成功\n");


	//listCommands
	if( mongo_run_command( conn, "admin", cmd+2, out ) == MONGO_OK )
	{
			printf("list commands :\n");
			//bson_print(out);
	}

	//listDatabases
	//必须是admin才能list database命令这是个系统管理命令
	if( mongo_run_command( conn, "test", cmd+3, out ) == MONGO_OK )
	{
			bson_iterator_init( it, out );
			result=bson_iterator_string(it);
			printf("list databases :\n");
			bson_print(out);
	}
	//ping 命令
	if( mongo_run_command( conn, "admin", cmd+4, out ) == MONGO_OK )
	{
			printf("ping :");
			bson_print(out);
	}

	//db.group  group分组
	if( mongo_run_command( conn, "test", cmd+5, out ) == MONGO_OK )
	{
			printf("group by is  :");
			bson_print(out);
	}
	//bson没有数据添加的时候为5字节大小  添加数据的是{"a":1}就是12字节
	bson bsmall[1];
	bson_init( bsmall );
	bson_append_int( bsmall, "a", 1 );
	bson_finish( bsmall );	
	printf("bson_size size :%d\n",bson_size( bsmall ));
 	bson_destroy( bsmall );
	

	//插入数据 insert data
	bson data[1];
	bson_init(data);
	bson_append_string(data,"name","li");
	bson_append_start_object(data,"address");
	bson_append_string(data,"henan","zhengzhou");
	bson_append_string(data,"anhui","hefei");
	bson_append_finish_object(data);
	bson_append_int(data,"phone",145);
	bson_finish(data);
	if(mongo_insert(conn,"test.test",data,NULL)==MONGO_OK)
			printf("insert into success!");
	//批量插入数据
	bson *mdata[2];
	bson data1[1];
	bson data2[1];
	bson_init(data1);
	bson_append_string(data1,"name","zhangsan");
	bson_finish(data1);
	bson_init(data2);
	bson_append_string(data2,"name","lisi");
	bson_finish(data2);
	mdata[0]=data1;
	mdata[1]=data2;
	if(mongo_insert_batch(conn,"test.test",(const bson **)mdata,2,NULL,0)==MONGO_OK)
			printf("insert into batch success!!!!\n");

	/*update 更新修改数据
		db.test.update({"name":"wang"},{"$inc":{"phone":2}}) phone 的值 +2
		db.test.update({"name":"wang",{"$set":{"phone":36}}}) phone 的值设置为36
	*/
	bson condition[2];
	bson option[2];

	bson_init(condition);
	bson_append_string(condition,"name","wang");
	bson_finish(condition);
	
	bson_init(condition+1);
	bson_append_string(condition+1,"name","zhangsan");
	bson_finish(condition+1);
	
	bson_init(option);
	bson_append_start_object(option,"$inc");
	bson_append_int(option,"phone",3);
	bson_append_finish_object(option);
	bson_finish(option);
	
	bson_init(option+1);
	bson_append_start_object(option+1,"$set");
	bson_append_int(option+1,"phone",25);
	bson_append_finish_object(option+1);
	bson_finish(option+1);
	
	if(mongo_update(conn,"test.test",condition,option,0,NULL)==MONGO_OK)
			printf("update data success!!\n");
	if(mongo_update(conn,"test.test",condition+1,option+1,0,NULL)==MONGO_OK)
			printf("update data success!!\n");
	/* find  findOne  查找
	   db.test.find({"name":"wang"})
	   db.test.findOne({"name":"wang"})
	*/
	bson condition1[1];
	bson output[1];
	bson_init(condition1);
	bson_append_string(condition1,"name","wang");
	bson_finish(condition1);
	if(mongo_find_one(conn,"test.test",condition1,0,output)==MONGO_OK){
			printf("find_one is successed :\n");
			bson_print(output);
	}
	//cursor 游标 iterator游标
	mongo_cursor cur[1];
	mongo_cursor_init(cur,conn,"test.test");
	while(mongo_cursor_next(cur)==MONGO_OK){
		bson_iterator it;
		bson_iterator_init(&it,mongo_cursor_bson(cur));
		while(bson_iterator_next(&it)){
			fprintf(stderr," %s : ",bson_iterator_key(&it));
			switch(bson_iterator_type(&it)){
				case BSON_INT:
						fprintf(stderr,"(int) %d \n",bson_iterator_int(&it));
						break;
				case BSON_STRING:
						fprintf(stderr,"(string) %s \n",bson_iterator_string(&it));
						break;
				case BSON_OBJECT:
						fprintf(stderr,"(object) {.....}\n");
						break;
				default:
						fprintf(stderr,"(type)%d\n",bson_iterator_type(&it));
						break;
			}
		}
		fprintf(stderr,"\n");
	}
	//remove删除表中的数据 db.test.remove({"name":"wang"})
	bson condition2[1];
	bson_init(condition2);
	bson_append_string(condition2,"name","wang");
	bson_finish(condition2);
	if(mongo_remove(conn,"test.test",condition,NULL)==MONGO_OK)
			printf("remove删除成功\n");
	
	//count  db.test.count();
	bson querycondition[1];
	bson_init(querycondition);
	bson_append_start_object(querycondition,"phone");
	bson_append_int(querycondition,"$gt",20);
	bson_append_finish_object(querycondition);
	bson_finish(querycondition);
	printf("count is :%f\n",mongo_count(conn,"test","test",NULL));//查找test表中的数量
	//db.test.count({"phone":{"$gt":20}})
	printf("count is :%f\n",mongo_count(conn,"test","test",querycondition));//查找test中表phone大于3的数量

	bson_destroy(cmd);
	mongo_destroy(conn);
	return 0;
}
