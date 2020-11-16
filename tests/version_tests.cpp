#include "doctest.h"
#include "line_based_writers/version.h"
#include <regex>
using namespace std::literals;

using namespace crosscode::line_based_writers;

TEST_SUITE("Version tests") {
    TEST_CASE ("Test if version is valid semver") {
        // Regex came from: semver.org
        const std::regex version_regex(R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)");
        REQUIRE(std::regex_match(std::string{version()},version_regex));
    }
}

