#ifndef LINE_BASED_WRITERS_FILE_STREAM_FACTORY_H
#define LINE_BASED_WRITERS_FILE_STREAM_FACTORY_H

#include <fstream>
#include "macro_tool.h"
#include <chrono>
#include <charconv>

namespace crosscode::line_based_writers {

    /// macro_handler contains logic for handling macros encountered during filename rendering.
    /// \tparam now The function to use for retrieving the current time. Replaceable to enable unit tests.
    template <auto now=std::chrono::system_clock::now>
    struct macro_handler {
        std::size_t counter_;

        /// begin_render is called before a macro rendition of a string
        /// Can be used to fix values like date time or create resources.
        static void begin_render() {}

        /// done_render is called after a macro rendition of a string
        /// Could be used to increment counters or clean up resources.
        void done_render() {
            counter_++;
        }

        /// handle_counter handles the counter macro (COUNTER and NUM)
        /// It takes a parameter in the form of a number that stands for the amount of digits to render.
        /// If counter is 1 and param is 4 then 0001 will be rendered
        /// \param macro_param container the parameter for the counter macro.
        /// \return the replacement string for the counter macro
        [[nodiscard]]
        std::string handle_counter(std::string_view macro_param) const {
            if (!macro_param.empty()) {
                std::size_t min_digits;
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

        /// macro_handler constructor Constructs a macro handler. The counter parameter determines the initial value of the counter.
        /// \param counter
        explicit macro_handler(std::size_t counter) : counter_{counter} {}
        [[nodiscard]]

        /// Handle will handle a macro
        /// It will delegate the call to specific methods that implement handling a certain macro.
        /// When no match is found it will return an empty string.
        /// \param macro_name The name of the macro
        /// \param macro_param The parameter of the macro
        /// \return The replacement string for the macro
        std::string handle(std::string_view macro_name, std::string_view macro_param) const {
            if (macro_name=="COUNTER") return handle_counter(macro_param);
            if (macro_name=="NUM") return handle_counter(macro_param);
            return {};
        }

    };

    /// file_name_generator generates filenames based on templates.
    /// \tparam now The function to use for retrieving the current time. Replaceable to enable unit tests.
    template <auto now=std::chrono::system_clock::now>
    class file_name_generator {
    public:
        using macro_handler_type = macro_handler<now>;
    private:
        macro_tool::macro_render_engine<macro_handler_type> macro_generator_;
    public:
        ///  file_name_generator constructs a filename generator based on a template provided with filename_tpl
        /// \param filename_tpl The filename template to use. See macro_handler for supported macros.
        /// \param counter The initial counter value to use with the macro_handler.
        explicit file_name_generator(std::string_view filename_tpl,std::size_t counter=0) : macro_generator_{macro_tool::macro_lexer(filename_tpl),counter} {}

        /// generate generates a filename
        std::string generate() {
            return macro_generator_.render();
        }
    };

    /// file_stream_factory_template is a template for generating file streams.
    /// It is used with batch_stream_writer to generate a new stream for each batch to write.
    /// \tparam Tfile_name_generator The filename generator to use generating filenames.
    /// \tparam Tstream The stream type to be used. In production it is ofstream but it is replaced with a fake
    /// in the unit tests.
    template <typename Tfile_name_generator, typename Tstream>
    class file_stream_factory_template {
    public:
        using file_name_generator_type = Tfile_name_generator;
        using stream_type = Tstream;
    private:
        file_name_generator_type file_name_generator_;
        stream_type stream_;
    public:

        /// file_stream_factory_template constructor initializes the file_stream_factory_template using a forwarding
        /// reference. It will pass all arguments to the Tfile_name_generator constructor.
        /// \tparam Args Parameter pack of argument types
        /// \param args Expanded parameter pack arguments
        template <typename ...Args>
        explicit file_stream_factory_template(Args&&... args) : file_name_generator_{std::forward<Args>(args)...} {}
        file_stream_factory_template() = default;
        file_stream_factory_template(file_stream_factory_template<file_name_generator_type,stream_type>&&) noexcept = default;
        file_stream_factory_template(const file_stream_factory_template<file_name_generator_type,stream_type>&) noexcept = delete;
        file_stream_factory_template<file_name_generator_type,stream_type>&operator=(const file_stream_factory_template<file_name_generator_type,stream_type>&) = delete;

        /// begin is called when a new stream should be created.
        void begin() {
            stream_.open(file_name_generator_.generate(),std::ios::trunc|std::ios::binary|std::ios_base::out);
        }

        /// write write a line to the currently open stream
        /// \tparam Tline The type of the line to write
        /// \param line The line to write
        template<typename Tline>
        void write(Tline &&line) {
            stream_ << line << "\n";
        }

        /// commit is called when writing to the stream has been completed
        void commit() {
            stream_.flush();
        }

#ifdef CROSSCODE_ACCESS_TO_UNIT_TEST
        /// returns the underlying stream. It is used for unit tests only.
        const stream_type& underlying_stream() const {
            return stream_;
        }
#endif
    };

    /// This type is a file_stream_factory_template using our file_name_generator. This type is used for unit tests.
    template <typename Tstream>
    using file_stream_factory_no_stream = file_stream_factory_template<file_name_generator<>,Tstream>;
    /// This type contains is a file_stream_factory using our file_name_generator and ofstream as underlying_stream.
    using file_stream_factory = file_stream_factory_no_stream<std::ofstream>;
}

#endif //LINE_BASED_WRITERS_FILE_STREAM_FACTORY_H
