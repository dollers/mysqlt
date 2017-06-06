#include "mysqlc.h"



#define url(host,port) "tcp://"+host+":"+port

MysqlC::MysqlC() {
// this->host="127.0.0.1";
// this->port=3306;
// this->user="root";
// this->password="root";
	MysqlC("127.0.0.1", 3306, "root", "root");
}



MysqlC::MysqlC(std::string host, int port , std::string user, std::string password ) {
	this->host=host;
	this->port=port;
	this->user=user;
	this->password=password;

	this->connect();
// this->print_log(host+user+password);
}

MysqlC::~MysqlC() {

}


void MysqlC::print_log (std::string str) {
	std::cout << str << std::endl;
}

void MysqlC::set_host(std::string host) {
	this->host = host ;
}
void MysqlC::set_port(int port) {
	this->port = port ;
}
void MysqlC::set_user(std::string user) {
	this->user = user ;
}
void MysqlC::set_password(std::string password) {
	this->password = password ;
}
// char** MysqlC::get_connect_info() {
// 	return {this->host,this->user};
// }



bool MysqlC::connect() {
	try {

		std::stringstream tmp;
		tmp << this->port ; 
/* Create a connection */
		driver = get_driver_instance();
	// std::cout << this->host.append("dd") << std::endl;
		con = driver->connect(this->host.insert(0,"tcp://").append(":")+tmp.str(), this->user, this->password);



	} catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}



bool MysqlC::use_schema(std::string schema) {
	try {
		this->con->setSchema(schema);
		this->stmt = con->createStatement();
	}catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	
}


sql::ResultSet* MysqlC::query(std::string statement) {
	try {
		if (this->res != NULL)
		{
			return this->stmt->executeQuery(statement);
		}else {
			return NULL ;
		} 

	} catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}

}



bool MysqlC::execute(std::string statement) {
	try {
		if (this->stmt != NULL )
		{
			this->stmt->execute(statement) ;
			// this->stmt->execute(statement) ;
			return true ;
		}else {
			std::cout << "d" << std::endl;
			return false ;
		}
		
	} catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}