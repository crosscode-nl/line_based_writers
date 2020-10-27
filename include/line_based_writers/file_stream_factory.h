#ifndef LINE_BASED_WRITERS_FILE_STREAM_FACTORY_H
#define LINE_BASED_WRITERS_FILE_STREAM_FACTORY_H

#include <fstream>
#include <variant>
#include <algorithm>
#include <atomic>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>
#include <map>
#include "macro_tool.h"

namespace crosscode::line_based_writers {

    struct macro_handler {
        std::size_t counter_;
        static void begin_render();
        void done_render();
        [[nodiscard]]
        std::string handle_counter(std::string_view macro_param) const;
        explicit macro_handler(std::size_t counter);
        [[nodiscard]]
        std::string handle(std::string_view macro_name, std::string_view macro_param) const ;
    };

    class file_name_generator {
    private:
        macro_tool::macro_render_engine<macro_handler> macro_generator_;
    public:
        explicit file_name_generator(std::string_view filename_tpl,std::size_t counter=0);

        std::string generate();
    };

    template <typename Tfile_name_generator, typename Tstream>
    class file_stream_factory_template {
    public:
        using file_name_generator_type = Tfile_name_generator;
        using stream_type = Tstream;
    private:
        file_name_generator_type file_name_generator_;
        stream_type stream_;
    public:

        template <typename ...Args>
        explicit file_stream_factory_template(Args&&... args) : file_name_generator_{std::forward<Args>(args)...} {}
        file_stream_factory_template() = default;
        file_stream_factory_template(file_stream_factory_template<file_name_generator_type,stream_type>&&) noexcept = default;
        file_stream_factory_template(const file_stream_factory_template<file_name_generator_type,stream_type>&) noexcept = delete;
        file_stream_factory_template<file_name_generator_type,stream_type>&operator=(const file_stream_factory_template<file_name_generator_type,stream_type>&) = delete;

        void begin() {
            stream_.open(file_name_generator_.generate(),std::ios::trunc|std::ios::binary|std::ios_base::out);
        }

        template<typename Tline>
        void write(Tline &&line) {
            stream_ << line << "\n";
        }

        void commit() {
            stream_.flush();
        }

#ifdef CROSSCODE_ACCESS_TO_UNIT_TEST
        const stream_type& underlying_stream() const {
            return stream_;
        }
#endif
    };

    template <typename Tstream>
    using file_stream_factory_no_stream = file_stream_factory_template<file_name_generator,Tstream>;
    using file_stream_factory = file_stream_factory_no_stream<std::ofstream>;
}

#endif //LINE_BASED_WRITERS_FILE_STREAM_FACTORY_H
