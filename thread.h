#include <pthread.h>
#include <iostream>





class Thread
{
public:
	Thread();
	~Thread();
	Thread(int ,void* (*)(void *));
	
public:
	void* say_hello(void* args) ;
	// void* PrintHello(void *threadid) ;

public:
	// bool set_run_fun(void* (*)(void *)) ;
	bool run_thread(void* (*fun)(void *), void* arg) ;
	bool run_thread() ;
private:
	int threads_num ;
	pthread_t *threads ;

};