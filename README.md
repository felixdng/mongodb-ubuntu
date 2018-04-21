操作系统：Ubuntu14.04
安装mongocxx驱动以及服务器配置，以下是安装步骤。

参考文档：
https://mongodb.github.io/mongo-cxx-driver/mongocxx-v3/installation/
[csdn博客: linux 下安装mongocxx] https://blog.csdn.net/qq_35001989/article/details/76703824
[csdn博客: CentOS7下mongoc安装，验证] https://blog.csdn.net/qq_35001989/article/details/76652949
[csdn博客：cmake 安装] https://blog.csdn.net/qq_35001989/article/details/76618795

本文使用的文件下载链接：
链接：https://pan.baidu.com/s/1VOfn0faWb9CbPEkzrcYuEg 密码：o89h

/* ============================================================ */

一．安装编译工具
sudo apt-get install -y gcc g++ git build-essential libtool automake autoconf wget curl

二．安装cmake3.9
1. 编译安装
wget https://cmake.org/files/v3.9/cmake-3.9.0.tar.gz
tar -xvf cmake-3.9.0.tar.gz
cd cmake-3.9.0
./configure
make -j4
sudo make install

2. 测试验证
mkdir -p cmake_test
cd cmake_test
vim main.c
/* ------------------- main.c : begin ------------------- */
#include <stdio.h>
int main()
{
    printf("cmake test OK!\n");
    return 0;
}
/* ------------------- main.c : end ------------------- */
  
vim CMakeLists.txt (文件名不能改成其他)
/* ------------------- CMakeLists.txt : begin ------------------- */
PROJECT(HELLO)
SET(SRC_LIST main.c)
MESSAGE(STATUS "this is BINARY dir" ${HELLO_BINARY_DIR})
MESSAGE(STATUS "this is SOURCE dir" ${HELLO_SOURCE_DIR})
ADD_EXECUTABLE(hello ${SRC_LIST})
/* ------------------- CMakeLists.txt : end ------------------- */

cmake .    //"."代表当前目录
make
./hello    //输出cmake test OK!，到此完成cmake3.9.0安装验证

三．安装mongodb服务器
1. 安装
curl -O https://fastdl.mongodb.org/linux/mongodb-linux-x86_64-3.0.6.tgz
tar -xvf mongodb-linux-x86_64-3.0.6.tgz
sudo mv mongodb-linux-x86_64-3.0.6 /usr/local/mongodb

2. 配置
cd $HOME         //进入home目录
mkdir -p mongo_data
cd mongo_data
mkdir -p logs
mkdir -p db
vim mongodb.conf
/* -------------------- mongodb.conf : begin -------------------- */
dbpath=/home/dzq/mongo_data/db
logpath=/home/dzq/mongo_data/logs/arbiter.log
logappend=true
port=28000
bind_ip=127.0.0.1,192.168.27.150
fork=true
journal=true
/* -------------------- mongodb.conf : end -------------------- */

/* 启动服务器 */
/usr/local/mongodb/bin/mongod -f /home/dzq/mongo_data/mongodb.conf

/* 启动shell客户端 */
/usr/local/mongodb/bin/mongo 127.0.0.1:28000

四．安装mongoc驱动
1. 编译安装
wget https://github.com/MongoDB/mongo-c-driver/releases/download/1.6.3/mongo-c-driver-1.6.3.tar.gz
tar -xvf mongo-c-driver-1.6.3.tar.gz
cd mongo-c-driver-1.6.3
./configure --disable-automatic-init-and-cleanup
make -j4
sudo make install

2. 添加环境变量
vim $HOME/.bashrc
/* ------------------- .bashrc : begin ------------------- */
PATH=$PATH:$HOME/bin:/usr/local/mongodb/bin
export PATH
LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH
CPATH=$CPATH:/usr/local/include/libmongoc-1.0:/usr/local/include/libbson-1.0
export CPATH
/* ------------------- .bashrc : end ------------------- */

/* 使.bashrc立即生效 */
source $HOME/.bashrc

3. 测试验证
mkdir -p mongoc_test
cd mongoc_test
vim connect.c
/* ------------------- connect.c : begin ------------------- */
#include "bson.h"
#include "bcon.h"
#include "mongoc.h"
int main (int argc, char *argv[])
{
   mongoc_client_t      *client;
   mongoc_database_t    *database;
   mongoc_collection_t  *collection;
   bson_t               *command, reply, *insert;
   bson_error_t          error;
   char                 *str;
   bool                  retval;
   mongoc_init ();
   client = mongoc_client_new ("mongodb://localhost:28000");
   mongoc_client_set_appname (client, "connect-example");
   database = mongoc_client_get_database (client, "db_name");
   collection = mongoc_client_get_collection (client, "db_name", "coll_name");
   command = BCON_NEW ("ping", BCON_INT32 (1));
   retval = mongoc_client_command_simple (client, "admin", command, NULL, &reply, &error);
   if (!retval) {
      fprintf (stderr, "%s\n", error.message);
      return EXIT_FAILURE;
   }
   str = bson_as_json (&reply, NULL);
   printf ("%s\n", str);
   insert = BCON_NEW ("hello", BCON_UTF8 ("world"));
   if (!mongoc_collection_insert (collection, MONGOC_INSERT_NONE, insert, NULL, &error)) {
      fprintf (stderr, "%s\n", error.message);
   }
   bson_destroy (insert);
   bson_destroy (&reply);
   bson_destroy (command);
   bson_free (str);
   mongoc_collection_destroy (collection);
   mongoc_database_destroy (database);
   mongoc_client_destroy (client);
   mongoc_cleanup ();
   return 0;
}
/* ------------------- connect.c : end ------------------- */

gcc -o connect connect.c -lmongoc-1.0 -lbson-1.0
./connect     //如果配置成功则在终端输出  { "ok" : 1.0 }

五．安装mongocxx驱动
1. 编译安装
curl -OL https://github.com/mongodb/mongo-cxx-driver/archive/r3.1.2.tar.gz
tar -xvf r3.1.2.tar.gz
cd mongo-cxx-driver-r3.1.2/build
cmake -DCMAKE_BUILD_TYPE=Release -DBSONCXX_POLY_USE_MNMLSTC=1 -DCMAKE_INSTALL_PREFIX=/usr/local/mongo-cxx-driver-r3.1.2 -DLIBBSON_DIR=/usr/local/lib -DLIBMONGOC_DIR=/usr/local/lib ..
sudo make EP_mnmlstc_core
make -j4
sudo make install

2. 添加环境变量
vim $HOME/.bashrc
/* ------------------- .bashrc : begin ------------------- */
CPATH=$CPATH:/usr/local/mongo-cxx-driver-r3.1.2/include/bsoncxx/v_noabi:/usr/local/mongo-cxx-driver-r3.1.2/include/mongocxx/v_noabi
export CPATH
/* ------------------- .bashrc : end ------------------- */

/* 使.bashrc立即生效 */
source $HOME/.bashrc

/* 添加动态链接库的路径 */
sudo ldconfig /usr/local/mongo-cxx-driver-r3.1.2/lib

3. 测试验证
mkdir -p mongocxx_test
cd mongocxx_test
vim test.cpp
/* --------------------- test.cpp : begin --------------------- */
#include <iostream>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
int main(int argc, char *argv[])
{
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri("mongodb://localhost:28000")};

    bsoncxx::builder::stream::document document{};

    auto collection = conn["testdb"]["testcollection"];
    document << "hello" << "world";

    collection.insert_one(document.view());
    auto cursor = collection.find({});

    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}
/* --------------------- test.cpp : end --------------------- */
	  
g++ --std=c++11 test.cpp -o test -L /usr/local/mongo-cxx-driver-r3.1.2/lib -lmongocxx -lbsoncxx
./test
正确安装验证会显示如下：
{ "_id" : { "$oid" : "5adb3a621d41c858be0a2382" }, "hello" : "world" }
