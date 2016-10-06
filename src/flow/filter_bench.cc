#include "flow/filter.h"

#include <numeric>
#include <tuple>
#include <vector>

#include "benchmark/benchmark.h"

#include "flow/vector.h"

namespace latticeflow {

void TrueFilter(benchmark::State& state) {
  while (state.KeepRunning()) {
    state.PauseTiming();
    std::vector<std::tuple<int, int>> xs(state.range_x());
    std::iota(std::begin(xs), std::end(xs), 0);
    Vector<int, int> v(xs);
    Filter<int, int> f(&v, [](const std::tuple<int, int>&) { return true; });
    state.ResumeTiming();
    for (auto t = f.next(); t; t = f.next()) {
      // Do nothing.
    }
  }
}
BENCHMARK(TrueFilter)->Range(10, 10 << 10);

}  // namespace latticeflow

int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}
