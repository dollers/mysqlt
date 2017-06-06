#include "slave.h"
#include "pause.h"


// #define SLAVE_TCP_URL "tcp://127.0.0.1:3306"
#define SLAVE_IP "127.0.0.1"
#define SLAVE_PORT "3306"
#define SLAVE_USER "root"
#define SLAVE_PW "root"

#define SHOW_SLAVE_STATUS "show slave status"


#define CATCH_BODY { \
	cout << "# ERR: SQLException in " << __FILE__; \
	cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl; \
	cout << "# ERR: " << e.what(); \
	cout << " (MySQL error code: " << e.getErrorCode(); \
	cout << ", SQLState: " << e.getSQLState() << " )" << endl; \
}



Slave::Slave(string ip,string port,string user,string password) {
	string url = "tcp://" + ip + ":" + port;
	init(url,user,password);
}

Slave::Slave() {
	Slave(SLAVE_IP,SLAVE_PORT,SLAVE_USER,SLAVE_PW);
	// init(SLAVE_TCP_URL,SLAVE_USER,SLAVE_PW);
}

Slave::~Slave() {

}



/* 提取出所有复制通道的错误 */
void Slave::errorCode(){
	
}


	/* 跳过错误 */
bool Slave::jumpError(string error) {
	this->errorcode = error;
	for (int i = 0; i < this->channel_num; ++i)
	{
		// if(!this->check(this->channel_name[i])) {
		jumpErrorForChannel(this->channel_name[i]);
		// }
	}
}

/* 检查通道的 */
bool Slave::check(string channel) {
	try {
		ResultSet *re = this->stmt->executeQuery("show slave status for channel '" + channel +"'");
		re->next();
		string check_r = re->getString("Slave_SQL_Running");
		string check_n = re->getString("Last_SQL_Errno");
		if (check_r == "No" && this->errorcode == check_n) {
			cout<<endl;
			cout<< "Last_SQL_Errno : " << check_n << endl;
			cout<< "Last_SQL_Error : " << re->getString("Last_SQL_Error") << endl;
			return false;
		}else {
			return true;
		}
	} catch(SQLException &e) CATCH_BODY
}


	/* 判断是否使用GTID */
bool Slave::gtidMode(string channel){
	try {
		ResultSet *re = this->stmt->executeQuery("show slave status for channel '" + channel + "'");
		re->next();
		string auto_positoin = re->getString("Auto_Position");
		re = this->stmt->executeQuery("show variables like 'gtid_mode'");
		re->next();
		string gtid_mode = re->getString("Value");
		if (auto_positoin == "1" || gtid_mode == "ON") {
			return true;
		}else {
			return false;
		}
	} catch(SQLException &e) CATCH_BODY
}



/* 跳过单个通道的错误 */
void Slave::jumpErrorForChannel(string channel){
	Pause *pause = new Pause(0.3);
	// while(!slave->check()) {
	do {
		while(!this->check(channel)) {
			if (gtidMode(channel))
			{
				this->jumpGTIDError(channel);
			}else {
				this->jumpFilePosError(channel);
			}
			pause->pause(0.3);
		}
		// pause(3);
	// }
		pause->pause(3);
	}while(!this->check(channel));
}

	/* 跳过基于文件位置的复制 */
void Slave::jumpFilePosError(string channel) {
	try {
		this->stmt->execute("stop slave for channel '" + channel + "'");
		this->stmt->execute("set @@global.sql_slave_skip_counter=1");
		this->stmt->execute("start slave for channel '" + channel + "'");	
	} catch(SQLException &e) CATCH_BODY
}


void Slave::jumpGTIDError(string channel) {
	cout<< "gtid_mode:请手动跳过" << endl;
}


	/* 初始化 */
void Slave::init(string url,string user,string password){
	cout << url << " " << user << " " << password << endl;
	try {
		this->driver = get_driver_instance();
		this->con = driver->connect(url,user,password);
		this->stmt = con->createStatement();
		this->channelNum();
	} catch(SQLException &e) CATCH_BODY
}


int Slave::channelNum() {
	try {
		ResultSet *re = this->stmt->executeQuery(SHOW_SLAVE_STATUS);
		this->channel_num = re->rowsCount();
		for (int i = 0; i < this->channel_num; ++i)
		{
			re->next();
			this->channel_name[i] = re->getString("Channel_Name");
		}
		return channel_num;
	} catch(SQLException &e) CATCH_BODY
}