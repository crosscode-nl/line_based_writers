#ifndef LINE_BASED_WRITERS_LINE_BASED_WRITERS_H
#define LINE_BASED_WRITERS_LINE_BASED_WRITERS_H

#include "line_based_writers/version.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <mutex>

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

    template<typename Tstream_factory>
    class batch_stream_writer {
    public:
        using stream_factory_type = Tstream_factory;
    private:
        stream_factory_type stream_factory_;
    public:
        template <typename ...Args>
        explicit batch_stream_writer(Args&&... args) : stream_factory_{std::forward<Args>(args)...} {}
        batch_stream_writer() = default;
        batch_stream_writer(batch_stream_writer<stream_factory_type>&&) noexcept = default;
        batch_stream_writer(const batch_stream_writer<stream_factory_type>&) noexcept = delete;
        batch_stream_writer<stream_factory_type>&operator=(const batch_stream_writer<stream_factory_type>&) = delete;

        template<typename Iter>
        void operator()(Iter b,Iter e) {
            stream_factory_.begin();
            auto write_line_to_stream = [this](const auto& item) {
                stream_factory_.current() << item << "\n";
            };
            std::for_each(b,e,write_line_to_stream);
            stream_factory_.commit();
        }

        stream_factory_type& factory() { return stream_factory_; }
    };

    template<typename Tline_based_iterator_sink>
    class line_buffer {
    public:
        using sink_type = Tline_based_iterator_sink;
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
            buffer_.emplace_back(std::forward<Tline>(line));
            if (buffer_.size()==buffer_size_) {
                sink_(begin(buffer_),end(buffer_));
                buffer_.clear();
            }
        }

        void emit() {
            sink_(begin(buffer_),end(buffer_));
            buffer_.clear();
        }

        sink_type& sink() { return sink_; }
    };

    template<typename Tline_based_iterator_sink>
    class line_buffer_ts {
    public:
        using sink_type = Tline_based_iterator_sink;
    private:
        line_buffer<Tline_based_iterator_sink> lb_;
        std::unique_ptr<std::mutex> mutex_;
    public:
        template <typename Tbuffer_size, typename ...Args>
        explicit line_buffer_ts(Tbuffer_size buffer_size, Args&&... args) : lb_{buffer_size, std::forward<Args>(args)...}, mutex_{std::make_unique<std::mutex>()} {}
        template <typename Tbuffer_size>
        explicit line_buffer_ts(Tbuffer_size buffer_size) : lb_{buffer_size}, mutex_{std::make_unique<std::mutex>()} {}
        line_buffer_ts(line_buffer_ts<sink_type>&&) noexcept = default;
        line_buffer_ts(const line_buffer_ts<sink_type>&) noexcept = delete;
        line_buffer_ts<sink_type>&operator=(const line_buffer<sink_type>&) = delete;

        template<typename Tline>
        void operator()(Tline &&line) {
            std::scoped_lock lock{*mutex_};
            lb_(line);
        }

        void emit() {
            std::scoped_lock lock{*mutex_};
            lb_.emit();
        }

        sink_type& sink() { return lb_.sink(); }
    };

}

#endif //LINE_BASED_WRITERS_LINE_BASED_WRITERS_H
