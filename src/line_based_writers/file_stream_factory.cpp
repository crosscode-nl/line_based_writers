#include "line_based_writers/file_stream_factory.h"
#include <charconv>

namespace crosscode::line_based_writers {

    macro_handler::macro_handler(std::size_t counter) : counter_{counter} {}

    void macro_handler::begin_render() {}

    void macro_handler::done_render() {
        counter_++;
    }

    std::string macro_handler::handle_counter(std::string_view macro_param) const {
            if (!macro_param.empty()) {
                int min_digits;
                auto[p, ec] = std::from_chars(std::data(macro_param), std::data(macro_param) + std::size(macro_param), min_digits);
                if (ec == std::errc()) {
                    auto result = std::to_string(counter_);
                    min_digits -= std::size(result);
                    if (min_digits > 0) {
                        result = std::string(min_digits, '0') + result;
                    }
                    return result;
                }
            }
            return std::to_string(counter_);
    }

    std::string macro_handler::handle(std::string_view macro_name, std::string_view macro_param) const {
        if (macro_name=="COUNTER") return handle_counter(macro_param);
        if (macro_name=="NUM") return handle_counter(macro_param);
        return {};
    }


    file_name_generator::file_name_generator(std::string_view filename_tpl,std::size_t counter)
    : macro_generator_{macro_tool::macro_lexer(filename_tpl),counter} {}

    std::string file_name_generator::generate() {
        std::string result = macro_generator_.render();

        return result;
    }

}