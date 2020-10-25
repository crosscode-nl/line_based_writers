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

namespace crosscode::line_based_writers {

    class file_name_generator {
    private:
        /*
        template <auto s>
        class text : token {
            std::string render() override {
                return s;
            };
        };
        template <auto digits,auto current_value>
        class number : token {
            std::string render() override {
                 std::string value = to_string(current_value());
                 if (value.size()<digits) {
                     value = std::string(digits-value.size(),'0') + value;
                 }
                 return value;
            };
        };*/
        struct macro : public std::string {
            using std::string::string;
        };

        std::size_t counter_;
    public:
        file_name_generator(std::string_view filename_tpl,std::size_t counter=0) : counter_{counter} {
            std::vector<std::variant<std::string,macro>> parts;
         //   bool in_macro = false;
            for(const char &c : filename_tpl) {
                if (c=='%') {

                }
            }
        }

        std::string get() {
            return "test";
        }
    };

    template <typename Tfile_name_generator>
    class file_stream_factory {
    public:
        using file_name_generator_type = Tfile_name_generator;
    private:
        file_name_generator_type file_name_generator_;
        std::ofstream stream_;
    public:

        template <typename ...Args>
        explicit file_stream_factory(Args&&... args) : file_name_generator_{std::forward<Args>(args)...} {}
        file_stream_factory() = default;
        file_stream_factory(file_stream_factory<file_name_generator_type>&&) noexcept = default;
        file_stream_factory(const file_stream_factory<file_name_generator_type>&) noexcept = delete;
        file_stream_factory<file_name_generator_type>&operator=(const file_stream_factory<file_name_generator_type>&) = delete;

        void begin() {
            stream_.open(file_name_generator_.get(),std::ios::trunc|std::ios::binary);
        }

        template<typename Tline>
        void write(Tline &&line) {
            stream_ << line << "\n";
        }

        void commit() {
            stream_.flush();
        }
    };
}

#endif //LINE_BASED_WRITERS_FILE_STREAM_FACTORY_H
