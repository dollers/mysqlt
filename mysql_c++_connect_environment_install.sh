#!/bin/bash

# g++ -L/usr/local/lib -L/usr/local/mysql/lib -L/usr/lib64/mysql -L/usr/lib -lmysqlcppconn  main.cpp  -o test2


# g++ -o mysqlc main.cpp mysqlc.h mysqlc.cpp -L/usr/local/lib -L/usr/local/mysql/lib -L/usr/lib64/mysql -L/usr/lib -lmysqlcppconn
yum install -y gcc-c++
yum localinstall -y mysql-connector-*
yum install -y postgresql-devel



# boost_1_59_0
./bootstrap.sh

./b2 && ./b2 install