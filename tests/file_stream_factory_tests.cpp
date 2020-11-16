#include "doctest.h"

#define CROSSCODE_ACCESS_TO_UNIT_TEST

#include "line_based_writers.h"
#include <sstream>

namespace lbw = crosscode::line_based_writers;
using namespace std::literals;

struct fake_stream : public std::stringstream  {
    using std::stringstream::stringstream;
    std::string last_file_name;
    bool is_open=false;
    bool is_clear=true;
    ios_base::openmode last_open_mode{};
    void open(const std::string& file_name, ios_base::openmode mode = ios_base::out) {
        str("");
        last_file_name = file_name;
        last_open_mode = mode;
        is_open = true;
        is_clear = false;
    }
    void close() {
        is_open = false;
    }
    void clear() {
        is_clear = true;
    }

};


std::chrono::system_clock::time_point fake_now() {
    return std::chrono::system_clock::time_point{134055123456789ns};
}

using testable_file_stream_factory = lbw::file_stream_factory_no_stream<fake_stream>;

TEST_SUITE("File stream factory tests") {
    TEST_CASE("Can write to file_stream in file_stream_factory") {
        testable_file_stream_factory tfsf("/tmp/test-%NUM:4%.txt");
        tfsf.begin();
        REQUIRE("/tmp/test-0000.txt"==tfsf.underlying_stream().last_file_name);
        REQUIRE((std::ios::trunc|std::ios::binary|std::ios_base::out)==tfsf.underlying_stream().last_open_mode);
        REQUIRE(true==tfsf.underlying_stream().is_open);
        REQUIRE(false==tfsf.underlying_stream().is_clear);
        tfsf.write("test1");
        tfsf.write("test2");
        tfsf.commit();
        REQUIRE(false==tfsf.underlying_stream().is_open);
        REQUIRE(true==tfsf.underlying_stream().is_clear);
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
        lbw::file_name_generator fng("/tmp/test-%NUM:4%.txt");
        REQUIRE("/tmp/test-0000.txt"==fng.generate());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp/test-0001.txt" == fng.generate());
        }
    }
    TEST_CASE("Can create file_name_generator with template and 4 leading zeros and initial count is 9"){
        lbw::file_name_generator fng("/tmp/test-%NUM:4%.txt",9u);
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
        lbw::file_name_generator fng("/tmp-%NUM:4%/test-%NUM:4%.txt");
        REQUIRE("/tmp-0000/test-0000.txt"==fng.generate());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp-0001/test-0001.txt" == fng.generate());
        }
    }
    TEST_CASE("Can create file_name_generator with template and retrieve current year."){
        lbw::file_name_generator<fake_now> fng("/tmp/test-%YEAR%.txt");
        REQUIRE("/tmp/test-1970.txt"==fng.generate());
    }
    TEST_CASE("Can create file_name_generator with template and retrieve current month."){
        lbw::file_name_generator<fake_now> fng("/tmp/test-%MONTH%.txt");
        REQUIRE("/tmp/test-01.txt"==fng.generate());
    }
    TEST_CASE("Can create file_name_generator with template and retrieve current day."){
        lbw::file_name_generator<fake_now> fng("/tmp/test-%DAY%.txt");
        REQUIRE("/tmp/test-02.txt"==fng.generate());
    }
    TEST_CASE("Can create file_name_generator with template and retrieve current hour."){
        lbw::file_name_generator<fake_now> fng("/tmp/test-%HOUR%.txt");
        REQUIRE("/tmp/test-13.txt"==fng.generate());
    }
    TEST_CASE("Can create file_name_generator with template and retrieve current minute."){
        lbw::file_name_generator<fake_now> fng("/tmp/test-%MINUTE%.txt");
        REQUIRE("/tmp/test-14.txt"==fng.generate());
    }
    TEST_CASE("Can create file_name_generator with template and retrieve current second."){
        lbw::file_name_generator<fake_now> fng("/tmp/test-%SECOND%.txt");
        REQUIRE("/tmp/test-15.txt"==fng.generate());
    }
}

