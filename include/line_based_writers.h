#ifndef LINE_BASED_WRITERS_LINE_BASED_WRITERS_H
#define LINE_BASED_WRITERS_LINE_BASED_WRITERS_H

#include "line_based_writers/version.h"
#include "line_based_writers/file_stream_factory.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <mutex>

/// crosscode::line_based_writers namespace provides line based writers for use with exporters for
/// https://github.com/crosscode-nl/simple_instruments like https://github.com/crosscode-nl/influxdblpexporter
namespace crosscode::line_based_writers {

    /// A stream_writer that can be used by https://github.com/crosscode-nl/influxdblpexporter
    /// This is used to write to a single stream.
    /// \tparam Tstream_sink The stream to write to.
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
        stream_writer(stream_writer<sink_type>&& rhs) noexcept : sink_{std::move(rhs.sink_)} {}
        stream_writer(const stream_writer<sink_type>&) = delete;
        stream_writer<sink_type>&operator=(const stream_writer<sink_type>&) = delete;

        template<typename Tline>
        void write(Tline &&line) {
            sink_ << std::forward<Tline>(line) << "\n";
        }

        sink_type& sink() { return sink_; }
    };

    /// batch_stream_writer writes batches of data to a stream.
    /// \tparam Tline_writer_factory An implementation of a line_writer_factory. file_stream_factory is the
    /// implementation we provide.
    template<typename Tline_writer_factory>
    class batch_stream_writer {
    public:
        using line_writer_factory = Tline_writer_factory;
    private:
        line_writer_factory line_writer_factory_;
    public:
        template <typename ...Args>
        explicit batch_stream_writer(Args&&... args) : line_writer_factory_{std::forward<Args>(args)...} {}
        batch_stream_writer() = default;
        batch_stream_writer(batch_stream_writer<line_writer_factory>&& rhs) noexcept : line_writer_factory_{std::move(rhs.line_writer_factory_)} {}
        batch_stream_writer(const batch_stream_writer<line_writer_factory>&) = delete;
        batch_stream_writer<line_writer_factory>&operator=(const batch_stream_writer<line_writer_factory>&) = delete;

        template<typename Iter>
        void write(Iter b,Iter e) {
            line_writer_factory_.begin();
            auto write_line_to_stream = [this](const auto& item) {
                line_writer_factory_.write(std::forward<decltype(item)>(item));
            };
            std::for_each(b,e,write_line_to_stream);

            line_writer_factory_.commit();
        }

        line_writer_factory& factory() { return line_writer_factory_; }
    };

    /// A line_buffer that can be used by https://github.com/crosscode-nl/influxdblpexporter
    /// This is used to buffer writes to a stream.
    /// \tparam Tline_based_iterator_sink The sink to write to when the buffer is emitted.
    template<typename Tline_based_iterator_sink>
    class line_buffer {
    public:
        using sink_type = Tline_based_iterator_sink;
    private:
        std::size_t buffer_size_;
        sink_type sink_;
        std::vector<std::string> buffer_;
    public:
        template <typename ...Args>
        explicit line_buffer(std::size_t buffer_size, Args&&... args) : buffer_size_{buffer_size}, sink_{std::forward<Args>(args)...} {}
        explicit line_buffer(std::size_t buffer_size) : buffer_size_{buffer_size} {}
        line_buffer(line_buffer<sink_type>&& rhs) noexcept : buffer_size_{rhs.buffer_size_}, sink_{std::move(rhs.sink_)}, buffer_{std::move(rhs.buffer_)} {}
        line_buffer(const line_buffer<sink_type>&) = delete;
        line_buffer<sink_type>&operator=(const line_buffer<sink_type>&) = delete;

        template<typename Tline>
        void write(Tline &&line) {
            buffer_.emplace_back(std::forward<Tline>(line));
            if (buffer_.size()==buffer_size_) {
                sink_.write(begin(buffer_),end(buffer_));
                buffer_.clear();
            }
        }

        void emit() {
            sink_.write(begin(buffer_),end(buffer_));
            buffer_.clear();
        }

        sink_type& sink() { return sink_; }

        ~line_buffer() {
            emit();
        }
    };

    /// line_buffer_ts is a thread safe wrapper around line_buffer
    /// \tparam Tline_based_iterator_sink The sink to write to when the buffer is emitted.
    template<typename Tline_based_iterator_sink>
    class line_buffer_ts {
    public:
        using sink_type = Tline_based_iterator_sink;
    private:
        line_buffer<Tline_based_iterator_sink> lb_;
        std::unique_ptr<std::mutex> mutex_;
    public:
        template <typename ...Args>
        explicit line_buffer_ts(std::size_t buffer_size, Args&&... args) : lb_{buffer_size, std::forward<Args>(args)...}, mutex_{std::make_unique<std::mutex>()} {}
        explicit line_buffer_ts(std::size_t buffer_size) : lb_{buffer_size}, mutex_{std::make_unique<std::mutex>()} {}
        line_buffer_ts(line_buffer_ts<sink_type>&& rhs) noexcept : lb_{std::move(rhs.lb_)}, mutex_{std::move(rhs.mutex_)} {}
        line_buffer_ts(const line_buffer_ts<sink_type>&) = delete;
        line_buffer_ts<sink_type>&operator=(const line_buffer<sink_type>&) = delete;

        template<typename Tline>
        void write(Tline &&line) {
            std::scoped_lock lock{*mutex_};
            lb_.write(line);
        }

        void emit() {
            std::scoped_lock lock{*mutex_};
            lb_.emit();
        }

        sink_type& sink() { return lb_.sink(); }

        ~line_buffer_ts() {
            emit();
        }
    };

    /// segmented_line_based_file_writer is class of type line_buffer<batch_stream_writer<file_stream_factory>>
    /// The first constructor parameter contains the buffer size of type std::size_t
    /// The second constructor parameter filename_tpl of type std::string_view, containing the path and filename template to write the output to.
    using segmented_line_based_file_writer = line_buffer<batch_stream_writer<file_stream_factory>>;

    /// segmented_line_based_file_writer_ts is class of type line_buffer_ts<batch_stream_writer<file_stream_factory>>, a thread safe version of segmented_line_based_file_writer
    /// The first constructor parameter contains the buffer size of type std::size_t
    /// The second constructor parameter filename_tpl of type std::string_view, containing the path and filename template to write the output to.
    using segmented_line_based_file_writer_ts = line_buffer_ts<batch_stream_writer<file_stream_factory>>;

}

#endif //LINE_BASED_WRITERS_LINE_BASED_WRITERS_H
