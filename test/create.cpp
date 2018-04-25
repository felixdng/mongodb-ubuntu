#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;

int main(int argc, char**argv) {
    mongocxx::instance inst{};
    const auto uri = mongocxx::uri{(argc >= 2) ? argv[1] : "mongodb://localhost:28000"};
    mongocxx::client conn{uri};

    auto db = conn["mytestdb"];

    // TODO: fix dates

    // @begin: cpp-insert-a-document
    bsoncxx::document::value restaurant_doc =
        document{} << "address" << open_document << "street"
                   << "2 Avenue"
                   << "zipcode"
                   << "10075"
                   << "building"
                   << "1480"
                   << "coord" << open_array << -73.9557413 << 40.7720266 << close_array
                   << close_document << "borough"
                   << "Manhattan"
                   << "cuisine"
                   << "Italian"
                   << "grades" << open_array << open_document << "date"
                   << bsoncxx::types::b_date{std::chrono::milliseconds{12323}} << "grade"
                   << "A"
                   << "score" << 11 << close_document << open_document << "date"
                   << bsoncxx::types::b_date{std::chrono::milliseconds{121212}} << "grade"
                   << "B"
                   << "score" << 17 << close_document << close_array << "name"
                   << "Vella"
                   << "restaurant_id"
                   << "41704620" << finalize;

    // We choose to move in our document here, which transfers ownership to insert_one()
    auto res = db["restaurants"].insert_one(std::move(restaurant_doc));
    // @end: cpp-insert-a-document
}
