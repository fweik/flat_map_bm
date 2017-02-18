#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>

#include <iostream>

#include <benchmark/benchmark.h>

#include <boost/container/flat_map.hpp>

using value_type = std::pair<int, int>;

std::vector<value_type> random_vector(std::size_t n) {
  std::vector<value_type> ret(n);

  std::mt19937 mersenne_engine(n);
  std::uniform_int_distribution<int> dist;

  std::transform(ret.begin(), ret.end(), ret.begin(),
                 [&dist, &mersenne_engine](value_type) -> value_type {
                   return {dist(mersenne_engine), dist(mersenne_engine)};
                 });

  return ret;
}

template <typename Container>
static void BM_fill_random(benchmark::State &state) {
  auto values = random_vector(state.range(0));

  while (state.KeepRunning()) {
    benchmark::DoNotOptimize(Container(values.begin(), values.end()));
  }
}

template <typename Container>
static void BM_fill_ordered(benchmark::State &state) {
  auto values = random_vector(state.range(0));
  std::sort(values.begin(), values.end(),
            [](value_type const &a, value_type const &b) {
              return a.first < b.first;
            });

  while (state.KeepRunning()) {
    benchmark::DoNotOptimize(Container(values.begin(), values.end()));
  }
}

template <typename Container>
static void BM_sort_and_fill(benchmark::State &state) {
  auto values = random_vector(state.range(0));
  std::sort(values.begin(), values.end(),
            [](value_type const &a, value_type const &b) {
              return a.first < b.first;
            });

  while (state.KeepRunning()) {
    benchmark::DoNotOptimize(Container(values.begin(), values.end()));
  }
}

template <typename Container>
static void BM_traversal(benchmark::State &state) {
  auto values = random_vector(state.range(0));
  Container map(values.begin(), values.end());

  while (state.KeepRunning()) {
    benchmark::DoNotOptimize(std::accumulate(
        map.begin(), map.end(), 0,
        [](int s, value_type const &e) { return s + e.second; }));
  }
}

template <typename Container>
static void BM_random_access(benchmark::State &state) {
  auto values = random_vector(state.range(0));

  int i = 0;
  for (auto &e : values) {
    e.first = i++;
  }

  Container map(values.begin(), values.end());

  std::mt19937 mersenne_engine(state.range(0));
  std::uniform_int_distribution<int> dist(0, state.range(0) - 1);

  int sum = 0;
  while (state.KeepRunning()) {
    auto const id = dist(mersenne_engine);

    benchmark::DoNotOptimize(sum += map.at(id));
  }
}

BENCHMARK_TEMPLATE(BM_random_access, std::unordered_map<int, int>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10);
BENCHMARK_TEMPLATE(BM_random_access, boost::container::flat_map<int, int>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10);
BENCHMARK_TEMPLATE(BM_fill_random, std::unordered_map<int, int>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10);
BENCHMARK_TEMPLATE(BM_fill_random, boost::container::flat_map<int, int>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10);
BENCHMARK_TEMPLATE(BM_fill_ordered, std::unordered_map<int, int>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10);
BENCHMARK_TEMPLATE(BM_fill_ordered, boost::container::flat_map<int, int>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10);
BENCHMARK_TEMPLATE(BM_traversal, std::unordered_map<int, int>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10);
BENCHMARK_TEMPLATE(BM_traversal, boost::container::flat_map<int, int>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10);

BENCHMARK_MAIN();
