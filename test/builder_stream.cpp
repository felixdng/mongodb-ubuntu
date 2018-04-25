#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/types.hpp>

using namespace bsoncxx;

int main(int, char**) {
    using builder::stream::document;
    using builder::stream::array;

    // bsoncxx::builder::stream presents an iostream like interface for succintly
    // constructing complex BSON objects.

    // stream::document builds a BSON document 构造一个BSON文档
    auto doc = document{};
    // stream::array builds a BSON array 构造一个BSON数组
    auto arr = array{};

    // We append keys and values to documents using the '<<' operator;使用<<运算符
    // 连接key和value
    doc << "myKey"
        << "myValue";
    // We can chain any number of keys and values
    doc << "foo" << types::b_bool{false} << "baz" << types::b_int32{1234} << "quz"
        << types::b_double{1.234};

    // For arrays, everything just becomes a value， array里面的值都是value而不是key
    arr << 1 << 2 << types::b_bool{true};

    // The stream namespace includes some helpers that can be used similarly
    // to the stream manipulators in <iomanip>
    // To build a subdocument, use open_document and close_document 
    // 文档里面嵌套文档使用open_document和close_document
    using builder::stream::open_document;
    using builder::stream::close_document;
    doc << "mySubDoc" << open_document << "subdoc key"
        << "subdoc value" << close_document;
    // To build a subarray, use open_array and close_array 
    // 数组里面嵌套数组，使用open_array和close_array
    using builder::stream::open_array;
    using builder::stream::close_array;
    doc << "mySubArr" << open_array << 1 << types::b_bool{false} << "hello" << close_array;

    // There is a special finalize helper that converts a stream to its underlying bson value
    // this is useful for writing one-liners, for example
    using builder::stream::finalize;
    auto myQuery = document{} << "foo"
                              << "bar" << finalize;

    // There is a special concatenate helper to add all keys and corresponding values from one
    //将一个文档中的内容加到另一个文档中（保持其key-value的对应关系），可以使用concatenate
    // document into another.
    using bsoncxx::builder::concatenate;
    doc << concatenate(myQuery.view());
    // `doc` now looks like:
    // {
    //   "myKey": "myValue",
    //   ...
    //   "mySubArr": [...],
    //   "foo": "bar"
    // }

    // To nest an existing bsoncxx::document::value into a builder stream, you can create a
    // types::b_document and append it. Alternatively you can open a new document and concatenate
    // the value in. Or, simplest, is just to stream in the view.
    bsoncxx::document::value nestedValue = document{} << "nested" << true << finalize;
    document topLevelDoc{};
    topLevelDoc << "subDoc1" << types::b_document{nestedValue.view()} << "subDoc2" << open_document
                << concatenate(nestedValue.view()) << close_document << "subDoc3" << nestedValue
                << finalize;

    // `topLevelDoc` now looks like:
    // {
    //     "subDoc1": {
    //         "nested": true
    //     },
    //     "subDoc2": {
    //         "nested": true
    //     },
    //     "subDoc3": {
    //         "nested": true
    //     }
    // }
}
