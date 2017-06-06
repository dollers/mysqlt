#include "thread.h"
#include "stdlib.h"

#define THREAD_NUMS 5


void* (*fun)(void *) ;

void* PrintHello(void *threadid)
{  
   // 对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取
	int tid = *((int*)threadid);
	std::cout << "Hello Runoob! 线程 ID, " << tid << std::endl;
	pthread_exit(NULL);
}


Thread::Thread() {

	Thread(THREAD_NUMS,PrintHello) ;
	// this->threads_num = 5;
	// // pthread_t threadss[5];
	// this->threads = (pthread_t*)malloc(sizeof(pthread_t)*this->threads_num);
	// std::cout << "====" << std::endl;
	// int rc;
	// int indexes[this->threads_num];
	// for(int i=0; i < this->threads_num; i++ ){
	// 	std::cout << "main() : 创建线程, " << i << std::endl;
 //      indexes[i] = i; //先保存i的值
 //      // 传入的时候必须强制转换为void* 类型，即无类型指针        
 //      rc = pthread_create(&(*(this->threads+i)),//&threadss[i] 
 //      	NULL,
 //      	 PrintHello,
 //      	  (void *)&(indexes[i]));
 //      if (rc){
 //         std::cout << "Error:无法创建线程," << rc << std::endl;
 //         // exit(-1);
 //      }
	// }
	// pthread_exit(NULL);
}


Thread::Thread(int num, void* (*funs)(void *)) {
	fun = funs;
	this->threads_num = num;
	// pthread_t threadss[5];
	this->threads = (pthread_t*)malloc(sizeof(pthread_t)*this->threads_num);
	// this->run_thread(fun,(void*)1);
	// std::cout << "====" << std::endl;
	// int rc;
	// int indexes[this->threads_num];
	// for(int i=0; i < this->threads_num; i++ ){
	// 	std::cout << "main() : 创建线程, " << i << std::endl;
      // indexes[i] = i; //先保存i的值
      // // 传入的时候必须强制转换为void* 类型，即无类型指针        
      // rc = pthread_create(&(*(this->threads+i)),//&threadss[i] 
      // 	NULL,
      // 	 fun,
      // 	  (void *)&(indexes[i]));
      // if (rc){
      //    std::cout << "Error:无法创建线程," << rc << std::endl;
      //    // exit(-1);
      // }
	// }


	// pthread_exit(NULL);
}





Thread::~Thread () {

}


void* Thread::say_hello(void* args)
{
	std::cout << "Hello Runoob！" << std::endl;
}



// bool Thread::set_run_fun(void* (*fun)(void *), void* arg) {

// }
bool Thread::run_thread(void* (*fun)(void *), void* arg) {
	fun(NULL) ;
	int rc ;
	static int idx = 0; 
	std::cout << "1" << std::endl;
	rc = pthread_create(&(*(this->threads+(++idx))), NULL, fun, arg) ;
	std::cout << "2" << std::endl;
	if (rc){
		std::cout << "Error:无法创建线程," << rc << std::endl;
		return false ;
	}else { 
		return true ;
	}
}


bool Thread::run_thread() {
	// int rc ;
	// static int idx = 0; 
	// rc = pthread_create(&(*(this->threads+(++idx))), NULL, fun, arg) ;
	// if (rc){
	// 	std::cout << "Error:无法创建线程," << rc << std::endl;
	// 	return 10;
	// }else {
	// 	return 0 ;
	// }
	return true ;
}