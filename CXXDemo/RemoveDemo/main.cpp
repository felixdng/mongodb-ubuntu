#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/collection.hpp>

using namespace std;

int main()
{
    using bsoncxx::builder::stream::document;
    using mongocxx::collection;

    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{"mongodb://192.168.6.129:28000"}};

    mongocxx::database db = conn["mytestdb"];

    document filter_builder{};
    filter_builder << "ProgramUUID" << "f0111234";

     //remove one document
    db["TestTool_Program"].delete_one(filter_builder.view());

    //remove all documents that match a condition
    db["TestTool_Program"].delete_many(filter_builder.view());

    //remove all  documents in a collection
    db["TestTool_Program"].delete_many({});

    //drop a collection
    db["TestTool_Program"].drop();

    return 0;
}
