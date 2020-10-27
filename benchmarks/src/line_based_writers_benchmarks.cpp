#include <benchmark/benchmark.h>
#include <sstream>

static void BM_BenchMark(benchmark::State& state) {
    // Perform setup here
    std::stringstream ss;
    for (auto _ : state) {
        // Tested code here
        ss << "Test" << "\n";
    }
}
// Register the function as a benchmark
BENCHMARK(BM_BenchMark);
// Run the benchmark
BENCHMARK_MAIN();