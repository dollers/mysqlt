

#ifndef _MYSQLC_
#define _MYSQLC_ 

#include <iostream>
#include "string.h"
#include <sstream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#endif

class MysqlC
{
public:
	MysqlC();
	~MysqlC();

	MysqlC(std::string host, int port , std::string user, std::string password );


public:
	void print_log(std::string str);
	

public:
void set_host(std::string host);
void set_port(int port);
void set_user(std::string user);
void set_password(std::string password);
	// char** get_connect_info() ;


public:
	// bool connect() ;
	bool use_schema(std::string schema) ;
	sql::ResultSet* query(std::string statement) ;
	bool execute(std::string statement) ;
	// bool commit() ;
	// int
private:
	bool connect() ;

public:
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;

private:
	std::string host ;
	int port ;
	std::string user ;
	std::string password ; 


// private:
	// std::string tcp = "tcp://";
};