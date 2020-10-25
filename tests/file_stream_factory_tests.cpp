#include "doctest.h"
#include "line_based_writers.h"

namespace lbw = crosscode::line_based_writers;
using namespace std::literals;


/*
TEST_SUITE("File stream factory tests") {
    TEST_CASE("Can create file_name_generator with template and 4 leading zeros"){
        lbw::file_name_generator fng("/tmp/test-%NUM0000%.txt");
        REQUIRE("/tmp/test-0000.txt"==fng.get());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp/test-0001.txt" == fng.get());
        }
    }
    TEST_CASE("Can create file_name_generator with template and 4 leading zeros and initial count is 9"){
        lbw::file_name_generator fng("/tmp/test-%NUM0000%.txt",9u);
        REQUIRE("/tmp/test-0009.txt"==fng.get());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp/test-0010.txt" == fng.get());
        }
    }
    TEST_CASE("Can create file_name_generator with template and 0 leading zeros"){
        lbw::file_name_generator fng("/tmp/test-%NUM%.txt");
        REQUIRE("/tmp/test-0.txt"==fng.get());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp/test-1.txt" == fng.get());
        }
    }
    TEST_CASE("Can create file_name_generator with template and 0 leading zeros and initial count is 9"){
        lbw::file_name_generator fng("/tmp/test-%NUM%.txt",9u);
        REQUIRE("/tmp/test-9.txt"==fng.get());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp/test-10.txt" == fng.get());
        }
    }
    TEST_CASE("Can create file_name_generator with template and 4 leading zeros on two places"){
        lbw::file_name_generator fng("/tmp-%NUM0000%/test-%NUM0000%.txt");
        REQUIRE("/tmp-0000/test-0000.txt"==fng.get());
        SUBCASE("Will increment number on get:") {
            REQUIRE("/tmp-0000/test-0001.txt" == fng.get());
        }
    }
}

*/