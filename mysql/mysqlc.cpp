#include "mysqlc.h"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/statement.h>


#define EXECUTE_SUCCESS 0
#define EXECUTE_FAILURE 1
#define url(ip,port) "tcp://"+ip+":"+port
#define CATCH_BODY { \
	cout << "# ERR: SQLException in " << __FILE__; \
	cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl; \
	cout << "# ERR: " << e.what(); \
	cout << " (MySQL error code: " << e.getErrorCode(); \
	cout << ", SQLState: " << e.getSQLState() << " )" << endl; \
}



MysqlC::MysqlC() {
	this->initConfig();
}
MysqlC::MysqlC(string ip,string user,string passwd,int port) {
	this->setConfig(ip,user,passwd,port);
}

MysqlC::~MysqlC() {
	delete this->stmt;
	delete this->con;
	delete this->driver;
}

void MysqlC::setConfig(string ip,string user,string passwd,int port) {
	this->config.ip=ip;
	this->config.port=port;
	this->config.user=user;
	this->config.passwd=passwd;
}

bool MysqlC::connect(){
	this->connect(this->config);
}

bool MysqlC::connect(struct config){
	try {
		this->driver = get_driver_instance();
		this->con = driver->connect(url(config.ip,config.port),config.user,config.passwd);
		this->stmt = con->createStatement();
		return EXECUTE_SUCCESS;
	} catch(SQLException &e) CATCH_BODY
}

ResultSet* MysqlC::query(string sql){
	try {
		return this->stmt->executeQuery(sql);
	} catch(SQLException &e) CATCH_BODY
}

bool MysqlC::execute(string sql){
	try {
		return this->stmt->execute(sql);
	} catch (SQLException &e) CATCH_BODY
	
}

int MysqlC::update(string sql) {
	try {
		return this->stmt->executeUpate(sql);
	}catch (SQLException %e) CATCH_BODY
}

void MysqlC::initConfig() {
	this->setConfig("127.0.0.1","root","root",3306);
}