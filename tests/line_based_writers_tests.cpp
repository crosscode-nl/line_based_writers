#include "doctest.h"
#include "line_based_writers.h"
#include <sstream>

namespace lbw = crosscode::line_based_writers;
using namespace std::literals;

using stringstream_stream_writer = lbw::stream_writer<std::stringstream>;

class string_stream_factory {
    std::stringstream ss_;
    public:
    void begin() {
        ss_.str("");
    }

    std::stringstream& current() {
        return ss_;
    }

    void commit() {

    }

};

using batch_stream_writer = lbw::line_buffer<lbw::batch_stream_writer<string_stream_factory>>;
using batch_stream_writer_ts = lbw::line_buffer_ts<lbw::batch_stream_writer<string_stream_factory>>;

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
    TEST_CASE("Can create line buffer of 1") {
        batch_stream_writer lb{1u};
        SUBCASE("Can write line to line buffer") {
           lb("This is a line");
           REQUIRE("This is a line\n"==lb.sink().factory().current().str());
        }
    }
    TEST_CASE("Can create line buffer of 2") {
        batch_stream_writer lb{2u};
        SUBCASE("Can write line to line buffer, but is not outputted to sink yet") {
            lb("line 1");
            REQUIRE(""==lb.sink().factory().current().str());
            SUBCASE("Can write another line to line buffer, but is outputted to sink") {
                lb("line 2");
                REQUIRE("line 1\nline 2\n"==lb.sink().factory().current().str());
                SUBCASE("Can write another line to line buffer, but is not outputted to sink yet") {
                    lb("line 3");
                    REQUIRE("line 1\nline 2\n"==lb.sink().factory().current().str());
                    SUBCASE("Can write another line to line buffer, but is outputted to sink") {
                        lb("line 4");
                        REQUIRE("line 3\nline 4\n" == lb.sink().factory().current().str());
                    }
                }
            }

        }
    }
   TEST_CASE("Can create thread safe line buffer of 1") {
        batch_stream_writer_ts lb{1u};
        SUBCASE("Can write line to line buffer") {
           lb("This is a line");
           REQUIRE("This is a line\n"==lb.sink().factory().current().str());
        }
    }
    TEST_CASE("Can create thread safe line buffer of 2") {
        batch_stream_writer_ts lb{2u};
        SUBCASE("Can write line to line buffer, but is not outputted to sink yet") {
            lb("line 1");
            REQUIRE(""==lb.sink().factory().current().str());
            SUBCASE("Can write another line to line buffer, but is outputted to sink") {
                lb("line 2");
                REQUIRE("line 1\nline 2\n"==lb.sink().factory().current().str());
                SUBCASE("Can write another line to line buffer, but is not outputted to sink yet") {
                    lb("line 3");
                    REQUIRE("line 1\nline 2\n"==lb.sink().factory().current().str());
                    SUBCASE("Can write another line to line buffer, but is outputted to sink") {
                        lb("line 4");
                        REQUIRE("line 3\nline 4\n" == lb.sink().factory().current().str());
                    }
                }
            }

        }
    }
}