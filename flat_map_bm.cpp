#include <algorithm>
#include <random>
#include <vector>
#include <unordered_map>

#include <benchmark/benchmark.h>

#include <boost/container/flat_map.hpp>

using value_type = std::pair<int, int>;

std::vector<value_type> random_vector(std::size_t n) {
  std::vector<value_type> ret(n);

  std::mt19937 mersenne_engine(n);
  std::uniform_int_distribution<int> dist;

  std::transform(ret.begin(), ret.end(), ret.begin(), [&dist, &mersenne_engine](value_type) -> value_type {
							return {dist(mersenne_engine), dist(mersenne_engine)};
						      });
  
  return ret;
}


template<typename Container>
static void BM_fill_random(benchmark::State &state) {
  auto values = random_vector(10000);
  
  while(state.KeepRunning()) {
    //    benchmark::DoNotOptimize(Container(values.begin(), values.end()));
  }  
}

BENCHMARK_TEMPLATE(BM_fill_random, std::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BM_fill_random, boost::container::flat_map<int, int>);


BENCHMARK_MAIN();

