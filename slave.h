#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


using namespace std;
using namespace sql;

class Slave {
private:

	ResultSet *reset;
	string errorcodes[5];
	string channels[5];
	string *gtid_mode;
	int len;

	string errorcode;

private:
	string channel_name[5]; 
	int channel_num;

private:
	Driver *driver;
	Connection * con;
	Statement *stmt;
public:
	Slave(string ip,string port,string user,string password);

	Slave();

	~Slave();


public:
	/* 提取出所有复制通道的错误 */
	void errorCode();


	/* 跳过错误 */
	bool jumpError(string error);

	bool check(string channel);

private:

	/* 判断通道数量 */
	int channelNum();
	/* 判断是否使用GTID */
	bool gtidMode(string channel);


	

	/* 跳过单个通道的错误 */
	void jumpErrorForChannel(string channel);

	/* 跳过基于文件位置的复制 */
	void jumpFilePosError(string channel);

	void jumpGTIDError(string channel);


	/* 初始化 */
	void init(string url,string user,string password);


};