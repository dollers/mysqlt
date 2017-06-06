
#ifndef _IOSTREAM_H
#define _IOSTREAM_H 

#include <iostream>

#endif


#include "mysqlc.h"


#ifndef _RESULTSET_H_
#define _RESULTSET_H_ 

#include <cppconn/resultset.h>

#endif


int main(int argc, char const *argv[])
{
	MysqlC *mysqlc = new MysqlC();
	mysqlc->connect();
	ResultSet *re = mysqlc->query("show databases");
	while(re->next()) {
		cout << re->getString(1) << endl;
	}
	return 0;
}