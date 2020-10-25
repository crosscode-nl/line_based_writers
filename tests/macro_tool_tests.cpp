#include "doctest.h"
#include "line_based_writers.h"

namespace lbw = crosscode::line_based_writers;
using namespace std::literals;

TEST_SUITE("Macro tool tests") {
    TEST_CASE("macro_tool can render macro") {
        lbw::macro_tool macro{"Test %TEST%%% macro"};
                REQUIRE("Test  macro"==macro.render());
                SUBCASE("%TEST% macro replaced with THIS") {
            macro.register_macro_handler("",[](std::string_view) {
                return "%";
            });
            macro.register_macro_handler("TEST",[](std::string_view) {
                return "THIS";
            });
                    REQUIRE("Test THIS% macro"==macro.render());
        }
    }
    TEST_CASE("macro_tool can render macro with param") {
        lbw::macro_tool macro{"Test %TEST:MY%%% macro"};
                REQUIRE("Test  macro"==macro.render());
                SUBCASE("%TEST% macro replaced with THIS") {
            macro.register_macro_handler("TEST",[](std::string_view p) {
                return std::string{"THIS("} + std::string{p} + ")";
            });
                    REQUIRE("Test THIS(MY) macro"==macro.render());
        }
    }
}

