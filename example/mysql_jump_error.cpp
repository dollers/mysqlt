/* Copyright 2008, 2010, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * There are special exceptions to the terms and conditions of the GPL
 * as it is applied to this software. View the full text of the
 * exception in file EXCEPTIONS-CONNECTOR-C++ in the directory of this
 * software distribution.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * */

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <stdio.h>
/*
 *   Include directly the different
 *     headers from cppconn/ and mysql_driver.h + mysql_util.h
 *       (and mysql_connection.h). This will reduce your build time!
 *       */
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;
using namespace sql;


int pause(float time)  
{  
	long zt1=clock();  
	long zt2;  
	while(1)  
	{
		zt2=clock();  
		if(((float)(zt2-zt1)/CLOCKS_PER_SEC)>time)
			return 0;  
		else {
			//gotoxy(23,1);
			//printf("%-9.2f",((float)(zt2-zt1)/CLOCKS_PER_SEC));
		}  
	}
}






class Slave {
private:
	Driver *driver;
	Connection * con;
	Statement *stmt;
	ResultSet *reset;
	string errorcodes[5];
	string channels[5];
	string *gtid_mode;
	int len;

public:
	Slave(string ip,string port,string user,string password) {
		string url = "tcp://" + ip + ":" + port;
		init(url,user,password);
	}

	Slave() {
		init("tcp://127.0.0.1:3306","root","root");
	}


public:
	/* 提取出所有复制通道的错误 */
	void errorCode(){
		try {
			ResultSet *re = stmt->executeQuery("show slave status");
			int i=0;
			while(re->next()) {
				errorcodes[i]=re->getString("Last_SQL_Errno");
				channels[i]=re->getString("Channel_Name");
				i++;
				len=i;
			}
		} catch (SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}


	/* 跳过错误 */
	bool jumpError(string error) {
		errorCode();
		for(int i = 0; i < len ; i++) {
			if (error == errorcodes[i])
			{
				jumpErrorForChannel(channels[i]);
			}
		}

	}

	bool check(string channel) {
		try {
			ResultSet *re = stmt->executeQuery("show slave status for channel '" + channel +"'");
			re->next();
			string check_r = re->getString("Slave_SQL_Running");
			if (check_r == "NO") {
				cout<< "Last_SQL_Errno : " << re->getString("Last_SQL_Errno") << endl;
				cout<< "Last_SQL_Error : " << re->getString("Last_SQL_Error") << endl;
				cout<<endl;
				return false;
			}else {
				return true;
			}
		} catch (SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			return true;
		}
	}


private:

	/* 判断是否使用GTID */
	bool gtidMode(string channel){
		try {
			ResultSet *re = stmt->executeQuery("show slave status for channel '" + channel + "'");
			re->next();
			string auto_positoin = re->getString("Auto_Position");
			re = stmt->executeQuery("show variables like 'gtid_mode'");
			re->next();
			string gtid_mode = re->getString("Value");
			if (auto_positoin == "1" || gtid_mode == "ON") {
				return true;
			}else {
				return false;
			}
		} catch (SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}


	

	/* 跳过单个通道的错误 */
	void jumpErrorForChannel(string channel){

		while(!slave->check()) {
			while(!slave->check()) {
				if (gtidMode(channel))
				{
					jumpGTIDError(channel);

				}else {
					jumpFilePosError(channel);
				}
			}
			pause(1);
		}
		
	}

	/* 跳过基于文件位置的复制 */
	void jumpFilePosError(string channel) {
		try {
			stmt->execute("stop slave for channel '" + channel + "'");
			stmt->execute("set @@global.sql_slave_skip_counter=1");
			stmt->execute("start slave for channel '" + channel + "'");	
		}catch (SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}


	void jumpGTIDError(string channel) {
		cout<< "gtid_mode:请手动跳过" << endl;
	}


	/* 初始化 */
	void init(string url,string user,string password){
		cout << url << " " << user << " " << password << endl;
		try {
			driver = get_driver_instance();
			con = driver->connect(url,user,password);
			stmt = con->createStatement();
		} catch (SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}


};


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


void jumpErrorFilePos(Statement *stmt) {
	stmt->execute("stop slave");
	stmt->execute("set @@global.sql_slave_skip_counter=1");
	stmt->execute("start slave");
}

void slaveStatus(Statement *stmt,ResultSet *reset) {


}


int main(int argc,char *argv[]) {
	string ip="127.0.0.1";
	string port="3306";
	string user="root";
	string password="root";
	string jumpcode="1062";
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
	}
	Slave *slave = new Slave(ip,port,user,password);
	// slave->jumpError(jumpcode);
	// pause(1);

	// cout << slave->check() << endl;
	while(!slave->check()) {
		while(!slave->check()) {
		// Slave *slave = new Slave(ip,port,user,password);
			slave->jumpError(jumpcode);
			pause(0.3);
		// cout << "=========" << endl;
		// cout << slave->check() << endl;
		}
		pause(1);
	}
	return 0;
}
