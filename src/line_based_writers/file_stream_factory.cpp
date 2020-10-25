#include "line_based_writers/file_stream_factory.h"

namespace crosscode::line_based_writers {

    file_name_generator::file_name_generator(std::string_view filename_tpl,std::size_t counter) : macro_generator_{filename_tpl}, counter_{counter} {

        auto num_macro = [this](std::string_view param) {
            auto digits = std::size(param);
            std::string number = std::to_string(counter_);
            if (std::size(number)>=digits) return number;
            auto digits_to_add = digits - std::size(number);
            return std::string(digits_to_add,'0')+number;
        };

        macro_generator_.register_macro_handler("NUM",num_macro);
    }

    std::string file_name_generator::generate() {
        std::string result = macro_generator_.render();
        counter_++;
        return result;
    }

}