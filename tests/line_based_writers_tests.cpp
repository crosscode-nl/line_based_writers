#include "doctest.h"
#include "line_based_writers.h"
#include <sstream>

namespace lbw = crosscode::line_based_writers;
using namespace std::literals;

using stringstream_stream_writer = lbw::stream_writer<std::stringstream>;

TEST_SUITE("Line based writers") {
    TEST_CASE("Can create stream writer with header") {
        stringstream_stream_writer sw("some header\n",std::ios_base::app | std::ios_base::out);
        SUBCASE("Can write line to stream_writer") {
            sw("This is a line");
            REQUIRE("some header\nThis is a line\n"==sw.sink().str());
        }
    }
    TEST_CASE("Can create stream writer") {
        stringstream_stream_writer sw;
        SUBCASE("Can write line to stream_writer") {
            sw("This is a line");
            REQUIRE("This is a line\n"==sw.sink().str());
        }
    }
    TEST_CASE("Can create line buffer of 1 with initial content") {
        lbw::line_buffer<stringstream_stream_writer> lbssw{1u, "some header\n",std::ios_base::app | std::ios_base::out};
        SUBCASE("Can write line to stream_writer") {
            lbssw("This is a line");
            REQUIRE("some header\nThis is a line\n"==lbssw.sink().sink().str());
        }
    }
    TEST_CASE("Can create line buffer of 1") {
        lbw::line_buffer<stringstream_stream_writer> lb{1u};
        SUBCASE("Can write line to line buffer") {
           lb("This is a line");
           REQUIRE("This is a line\n"==lb.sink().sink().str());
        }
    }
}