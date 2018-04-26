
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/collection.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using mongocxx::collection;

int main()
{
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{"mongodb://192.168.6.129:28000"}};
    mongocxx::database db = conn["mytestdb"];

    //update top-level fields in a single document
    {
        /****************************************************
         找到有"ProgramUUID":"f0111234"键值对的文档，使用$set指定一个
         键"ExhibitionName"（如果文档中没有将新建），将这个键值对改
         成"ExhibitionName":"newName",然后使用$currentDate指定
         叫做"UpdateTime"的键，bool值true表示这个键对应的值使用Date的形式
         使用Unix的时间格式记录下当前的时间
        ****************************************************/
        document filter_builder{}, updata_builder{};
        filter_builder << "ProgramUUID" << "f0111234";
        updata_builder << "$set" << open_document
                       << "ActionCode" << "newActionCode" << close_document
                       << "$currentDate" << open_document
                       << "UpdateTime" << true << close_document;
        //db["TestTool_Program"].update_one(filter_builder.view(), updata_builder.view());
        db["TestTool_Program"].update_many(filter_builder.view(), updata_builder.view());
    }

    //replace the contents of a single document
    {
        //找到含有filter_builder信息的文档，将其中的对应信息替换成
        //replace_builder的信息
        document filter_builder, replace_builder;
        filter_builder << "PageUUID" << "f0019003";
        replace_builder << "PageName" << "replaceName";
        db["TestTool_Pages"].replace_one(filter_builder.view(), replace_builder.view());
    }

    return 0;
}
