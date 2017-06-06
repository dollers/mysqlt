

#include "mysqlc.h"
// #include "thread.h"

#include <pthread.h>
using namespace std;
#define NUM_THREADS 5


void* say_hello(void* args)
{
	std::cout << "Hello Runoob！" << std::endl;
}


void* insert(void * arg) {
	MysqlC* mysqlc = new MysqlC("192.168.56.104", 3306, "hotdb_datasource", "hotdb_datasource");
	mysqlc->use_schema("test");
	std::cout << "IN" << std::endl;
	while(1) {
		if (!mysqlc->execute("insert into t1 value(1)"))
		{
			std::cout << "ERROR" << std::endl;
		}
	}
}


int main(int argc, char const *argv[])
{

	// insert(NULL);
	/* code */
	// MysqlC* mysqlc = new MysqlC("192.168.56.104", 3306, "hotdb_datasource", "hotdb_datasource");


// 	mysqlc->use_schema("test");
// 	sql::ResultSet *res;
// 	mysqlc->print_log("asd");
// 	while(1) {
// 		res = mysqlc->query("show tables");

// 		while (res->next()) {
// 			std::cout << "\t... MySQL test tables: ";
// /* Access column data by alias or column name */
// 			std::cout << res->getString(1) << std::endl;
// 			std::cout << "\t... MySQL says it again: ";
// /* Access column data by numeric offset, 1 is the first column */
// 			std::cout << res->getString(1) << std::endl;
// 		}
// 	}


// 	// for (int i = 0; i > -1 ; ++i)
// 	// {
// 	// 	mysqlc->execute("insert into t1 value(1)") ;
// 	// }

// 	while(1) {
// 		if (!mysqlc->execute("insert into t1 value(1)"))
// 		{
// 			return 10;
// 		}
// 	}

	// pthread_t tids[NUM_THREADS];
	// for(int i = 0; i < NUM_THREADS; ++i)
	// {
 //        //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
	// 	int ret = pthread_create(&tids[i], NULL, insert, NULL);
	// 	if (ret != 0)
	// 	{
	// 		cout << "pthread_create error: error_code=" << ret << endl;
	// 	}
	// }
 //    //等各个线程退出后，进程才结束，否则进程强制结束了，线程可能还没反应过来；
	// pthread_exit(NULL);

	Thread* t = new Thread(5,insert);
	t->run_thread(insert,(void*)NULL);
	t->run_thread(insert,(void*)NULL);
	t->run_thread(insert,(void*)NULL);
	return 0;
}