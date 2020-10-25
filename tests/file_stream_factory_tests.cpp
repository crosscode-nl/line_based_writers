#include "doctest.h"

#define CROSSCODE_ACCESS_TO_UNIT_TEST

#include "line_based_writers.h"
#include <sstream>

namespace lbw = crosscode::line_based_writers;
using namespace std::literals;

struct fake_stream : public std::stringstream  {
    using std::stringstream::stringstream;
    std::string last_file_name;
    ios_base::openmode last_open_mode;
    void open(const std::string& file_name, ios_base::openmode mode = ios_base::out) {
        str("");
        last_file_name = file_name;
        last_open_mode = mode;
    }

};

using testable_file_stream_factory = lbw::file_stream_factory_no_stream<fake_stream>;



TEST_SUITE("File stream factory tests") {
    TEST_CASE("Can write to file_stream in file_stream_factory") {
        testable_file_stream_factory tfsf("/tmp/test-%NUM:0000%.txt");
        tfsf.begin();
        REQUIRE("/tmp/test-0000.txt"==tfsf.underlying_stream().last_file_name);
        REQUIRE((std::ios::trunc|std::ios::binary|std::ios_base::out)==tfsf.underlying_stream().last_open_mode);
        tfsf.write("test1");
        tfsf.write("test2");
        tfsf.commit();
        REQUIRE("test1\ntest2\n"==tfsf.underlying_stream().str());
        REQUIRE("/tmp/test-0000.txt"==tfsf.underlying_stream().last_file_name);
        REQUIRE((std::ios::trunc|std::ios::binary|std::ios_base::out)==tfsf.underlying_stream().last_open_mode);
        SUBCASE("New begin should create new file") {
            tfsf.begin();
            REQUIRE("/tmp/test-0001.txt"==tfsf.underlying_stream().last_file_name);
            REQUIRE((std::ios::trunc|std::ios::binary|std::ios_base::out)==tfsf.underlying_stream().last_open_mode);
            tfsf.write("test3");
            tfsf.write("test4");
            tfsf.commit();
            REQUIRE("test3\ntest4\n"==tfsf.underlying_stream().str());
            REQUIRE("/tmp/test-0001.txt"==tfsf.underlying_stream().last_file_name);
            REQUIRE((std::ios::trunc|std::ios::binary|std::ios_base::out)==tfsf.underlying_stream().last_open_mode);
        }
    }
    TEST_CASE("Can create file_name_generator with template and 4 leading zeros"){
        lbw::file_name_generator fng("/tmp/test-%NUM:0000%.txt");
        REQUIRE("/tmp/test-0000.txt"==fng.generate());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp/test-0001.txt" == fng.generate());
        }
    }
    TEST_CASE("Can create file_name_generator with template and 4 leading zeros and initial count is 9"){
        lbw::file_name_generator fng("/tmp/test-%NUM:0000%.txt",9u);
        REQUIRE("/tmp/test-0009.txt"==fng.generate());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp/test-0010.txt" == fng.generate());
        }
    }
    TEST_CASE("Can create file_name_generator with template and 0 leading zeros"){
        lbw::file_name_generator fng("/tmp/test-%NUM%.txt");
        REQUIRE("/tmp/test-0.txt"==fng.generate());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp/test-1.txt" == fng.generate());
        }
    }
    TEST_CASE("Can create file_name_generator with template and 0 leading zeros and initial count is 9"){
        lbw::file_name_generator fng("/tmp/test-%NUM%.txt",9u);
        REQUIRE("/tmp/test-9.txt"==fng.generate());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp/test-10.txt" == fng.generate());
        }
    }
    TEST_CASE("Can create file_name_generator with template and 4 leading zeros on two places"){
        lbw::file_name_generator fng("/tmp-%NUM:0000%/test-%NUM:0000%.txt");
        REQUIRE("/tmp-0000/test-0000.txt"==fng.generate());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp-0001/test-0001.txt" == fng.generate());
        }
    }
}

