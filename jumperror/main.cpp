#include "slave.h"
#include <string.h>
#include <stdlib.h>



void cutstring(char *str,string param[2]) {
	const char *d = "=";
	char *p = strtok(str,d);
	int i=0;
	while(p) {
		param[i]=p;
		p=strtok(NULL,d);
		i++;
	}
}

bool isParam(string str,string param) {
	if(str == param)
		return true;
	else
		return false;
}



int main(int argc, char *argv[])
{
	string ip="127.0.0.1";
	string port="3306";
	string user="root";
	string password="root";
	string jumpcode="1062";
	int all_error;
	string param[2];
	for(int i = 1; i < argc; i++) {
		cutstring(argv[i],param);
		if(isParam("--ip",param[0])){
			ip=param[1];
		}
		if(isParam("--port",param[0])){
			port=param[1];
		}
		if(isParam("--user",param[0])){
			user=param[1];
		}
		if(isParam("--password",param[0])){
			password=param[1];
		}
		if(isParam("--jumpcode",param[0])){
			jumpcode=param[1];
		}
		if(isParam("--all",param[0])){
			all_error=atoi(param[1]);
		}
		if(isParam("--help",param[0])){
			cout << "./mysql_jump_error [--port=3306] [--ip=127.0.0.1] [--user=root] [--password=root] [--jumpcode=1062]" << endl;
			return 0;
		}
	}

	if ( all_error != 0 || all_error != 1 )
	{
		all_error = 0;
	}
	Slave *slave = new Slave(ip,port,user,password);
	slave->jumpError(jumpcode);
	return 0;
}