#ifndef LINE_BASED_WRITERS_MACRO_TOOL_H
#define LINE_BASED_WRITERS_MACRO_TOOL_H

#include <variant>
#include <algorithm>
#include <vector>
#include <numeric>
#include <functional>
#include <map>

namespace crosscode::line_based_writers {

    class macro_tool {
    private:
        struct string_token : public std::string {
            using std::string::string;
        };

        struct macro_token : public std::string {
            explicit macro_token(std::string_view macro);

            std::string macro_name;
            std::string macro_param;
        };

        friend class token_visitor;
    public:
        using macro_handler_type = std::function<std::string(std::string_view param)>;
        using macro_handlers_type = std::map<std::string, macro_handler_type, std::less<>>;
        using token_type = std::variant<string_token, macro_token>;
    private:
        std::vector <token_type> items_;
        macro_handlers_type macro_handlers_;
    public:
        explicit macro_tool(std::string_view text);
        void register_macro_handler(std::string macro, macro_handler_type f);
        std::string render();
    };
}


#endif //LINE_BASED_WRITERS_MACRO_TOOL_H
