#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/config/prelude.hpp>
#include <bsoncxx/types.hpp>

using namespace bsoncxx;

int main(int, char**) {
    using namespace builder::stream;

    builder::stream::document build_doc;
    // {
    //     "_id" : 1,
    //     "name" : { "first" : "John", "last" : "Backus" },
    //     "contribs" : [ "Fortran", "ALGOL", "Backus-Naur Form", "FP" ],
    //     "awards" : [
    //                {
    //                  "award" : "W.W. McDowell Award",
    //                  "year" : 1967,
    //                  "by" : "IEEE Computer Society"
    //                },
    //                { "award" : "Draper Prize",
    //                  "year" : 1993,
    //                  "by" : "National Academy of Engineering"
    //                }
    //     ]
    // }
    //name是key,它对应的value值是一个文件，里面有两个键值对，分别是"first" : "John"和 "last" : "Backus"。
    //contribs是key,它对应的value是一个数组，数组里面有四个value:"Fortran", "ALGOL", "Backus-Naur Form", "FP"
    //awards是key,它对应的value是一个数组，数组里面有两个文档，第一个文档里面有三个键值对，第二个文档里面也有三个键值对
    //至此，使用builder::stream::document可以实现构造一个比较复杂的文件了。
    build_doc << "_id" << 1 << "name" << open_document << "first"
              << "John"
              << "last"
              << "Backus" << close_document << "contribs" << open_array << "Fortran"
              << "ALGOL"
              << "Backus-Naur Form"
              << "FP" << close_array << "awards" << open_array << open_document << "award"
              << "W.W. McDowell Award"
              << "year" << 1967 << "by"
              << "IEEE Computer Society" << close_document << open_document << "award"
              << "Draper Prize"
              << "year" << 1993 << "by"
              << "National Academy of Engineering" << close_document << close_array;

    auto doc = build_doc.view();

    // Once we have the document view, we can use ["key"] or [index] notation to reach into nested
    // documents or arrays.
    //用流式方式构造的文件，可以通过类似下标的方式实现随机访问，这里使用auto关键字可以简化很多工作，也不容易出错。

    auto awards = doc["awards"];//名为awards的key所对应的value---是一个包含两个document的array
    auto first_award_year = awards[0]["year"];//awards对应的array里面的第一个array元素，即
    //{
    //                  "award" : "W.W. McDowell Award",
    //                  "year" : 1967,
    //                  "by" : "IEEE Computer Society"
    //                },
    //这个文档中名为year的key对应的value
    auto second_award_year = doc["awards"][1]["year"];
    auto last_name = doc["name"]["last"];

    // If the key doesn't exist, or index is out of bounds, we get invalid elements.
    //如果key不存在或者a要访问的rray的index越界，会得到无效的元素
    auto invalid1 = doc["name"]["middle"];
    auto invalid2 = doc["contribs"][1000];
    if (invalid1 || invalid2) {
        BSONCXX_UNREACHABLE;  // Not reached.
    }

    // Similarly, indexed access (either by string or numeric index) into a type that is not
    // a document or an array yields invalid eleemnts.

    auto invalid3 = doc["_id"]["invalid"];
    auto invalid4 = doc["name"][3];
    if (invalid3 || invalid4) {
        BSONCXX_UNREACHABLE;  // Not reached.
    }

    // Make all variables used.
    return (awards && first_award_year && second_award_year && last_name) ? EXIT_SUCCESS
                                                                          : EXIT_FAILURE;
}
