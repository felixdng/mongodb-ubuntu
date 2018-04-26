#include <iostream>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/collection.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using mongocxx::collection;

using namespace std;

int main()
{
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{"mongodb://192.168.6.129:28000"}};
    mongocxx::database db = conn["mytestdb"];

    //查询数据库的集合中的所有文档
//    {
//        auto cursor = db["TestTool_Pages"].find({});
//        for (auto&& doc : cursor)
//            std::cout << bsoncxx::to_json(doc) << std::endl;
//    }

    {
        document filter_builder;
        filter_builder << "PageUUID" << "f0019003";

        auto cursor = db["TestTool_Pages"].find(filter_builder.view());
        for (auto&& doc : cursor)
            std::cout << bsoncxx::to_json(doc) << std::endl;
    }

    return 0;
}
