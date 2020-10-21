#ifndef LINE_BASED_WRITERS_LINE_BASED_WRITERS_H
#define LINE_BASED_WRITERS_LINE_BASED_WRITERS_H

#include "line_based_writers/version.h"
#include <vector>
#include <algorithm>

namespace crosscode::line_based_writers {

    template<typename Tstream_sink>
    class stream_writer {
    public:
        using sink_type = Tstream_sink;
    private:
        sink_type sink_;
    public:
        template <typename ...Args>
        explicit stream_writer(Args&&... args) : sink_{std::forward<Args>(args)...} {}
        stream_writer() = default;
        stream_writer(stream_writer<sink_type>&&) noexcept = default;
        stream_writer(const stream_writer<sink_type>&) noexcept = delete;
        stream_writer<sink_type>&operator=(const stream_writer<sink_type>&) = delete;

        template<typename Tline>
        void operator()(Tline &&line) {
            sink_ << std::forward<Tline>(line) << "\n";
        }

        sink_type& sink() { return sink_; }
    };


    template<typename Tline_based_protocol_sink>
    class line_buffer {
    public:
        using sink_type = Tline_based_protocol_sink;
    private:
        std::size_t buffer_size_;
        sink_type sink_;
        std::vector<std::string> buffer_;
    public:
        template <typename Tbuffer_size, typename ...Args>
        explicit line_buffer(Tbuffer_size buffer_size, Args&&... args) : buffer_size_{buffer_size}, sink_{std::forward<Args>(args)...} {}
        template <typename Tbuffer_size>
        explicit line_buffer(Tbuffer_size buffer_size) : buffer_size_{buffer_size} {}
        line_buffer(line_buffer<sink_type>&&) noexcept = default;
        line_buffer(const line_buffer<sink_type>&) noexcept = delete;
        line_buffer<sink_type>&operator=(const line_buffer<sink_type>&) = delete;

        template<typename Tline>
        void operator()(Tline &&line) {
            if (buffer_size_<=1) {
                sink_(std::forward<Tline>(line));
            } else {
                buffer_.emplace_back(std::forward<Tline>(line));
                if (buffer_.size()==buffer_size_) {
                    auto emit_to_sink = [this](const std::string& s){
                        sink_(s);
                    };
                    std::for_each(begin(buffer_),end(buffer_),emit_to_sink);
                    buffer_.clear();
                }
            }
        }

        sink_type& sink() { return sink_; }
    };

}

#endif //LINE_BASED_WRITERS_LINE_BASED_WRITERS_H
