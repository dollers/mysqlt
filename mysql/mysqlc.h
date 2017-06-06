

// #ifndef _IOSTREAM_H
// #define _IOSTREAM_H 

// #include <iostream>

// #endif

#include <string.h>

#ifndef _RESULTSET_H_
#define _RESULTSET_H_ 

#include <cppconn/resultset.h>

#endif


class Cconfig
{
private:
	string ip;
	string user;
	string passwd;
	int port;
public:
	Cconfig();
	~Cconfig();
private:
	void initConfig();
public:
	void setConfig(string ip,string user,string passwd,int port);
};

class MysqlC
{
public:
	MysqlC(string ip,string user,string passwd,int port);
	MysqlC();
	~MysqlC();

public:
	bool connect();
	bool connect(struct config);
	ResultSet* query(string sql);
	bool execute(string sql);
	int update(string sql);


private:
	Config *config;

private:
	Driver *driver;
	Connection * con;
	Statement *stmt;

};