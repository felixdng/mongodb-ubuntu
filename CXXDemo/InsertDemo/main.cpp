#include <iostream>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

using namespace std;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;
using mongocxx::collection;

int main(int argc, char *argv[])
{
    const string dbHost = "mongodb://192.168.6.129:28000";

    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{dbHost}};

    auto db = conn["mytestdb"]; //get database

    //get collections
    mongocxx::collection coll_program = db["TestTool_Program"];
    mongocxx::collection coll_page = db["TestTool_Pages"];

    document doc1{};
    //insert one document to TestTool_Program
    doc1 << "ProgramUUID" << "f0111234"
        << "ExhibitionName" << "theProgramName"
        << "LastUploadTime" << bsoncxx::types::b_date{std::chrono::milliseconds{12323}}
        << "LastDownLoadTime" << bsoncxx::types::b_date{std::chrono::milliseconds{12323}}
        << "ActionCode" << "eeff1234"
        << "PageArray" << open_array << "12340001" << "12340002" << "12340003" << close_array
        << "UpdateTime" << 9091;
    coll_program.insert_one(doc1.view());

    document doc2 = document{};
    doc2 << "PageUUID" << "f0019001"
        << "PageName" << "mainpage-1"
        << "ImageFullName" << "/Picture/image01.jpg"
        << "ButtonArray" << open_array << "f0104001" << "f0104002" << "f0104003" << close_array
        << "UpdateTime" << 8080;
    coll_page.insert_one(doc2.view());

    document doc3 = document{};
    doc3 << "PageUUID" << "f0019002"
        << "PageName" << "mainpage-2"
        << "ImageFullName" << "/Picture/image02.jpg"
        << "ButtonArray" << open_array << "f0104001" << "f0104002" << "f0104003" << close_array
        << "UpdateTime" << 8080;
    coll_page.insert_one(doc3.view());

    document doc4 = document{};
    doc4 << "PageUUID" << "f0019003"
        << "PageName" << "mainpage-3"
        << "ImageFullName" << "/Picture/image03.jpg"
        << "ButtonArray" << open_array << "f0104001" << "f0104002" << "f0104003" << close_array
        << "UpdateTime" << 8080;
    coll_page.insert_one(doc4.view());

    return 0;
}
