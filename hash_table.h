#ifndef HASH_TABLE
#define HASH_TABLE

#include <ctime>

#include <tuple>
#include <vector>

#include <boost/functional/hash.hpp>
#include <tbb/concurrent_hash_map.h>

#include "tuple_generator.h"

template <typename T> struct TupleHasher {
  size_t operator()(const T &t) const { return hash(t); }
  size_t hash(const T &t) const { return boost::hash_value(t); }
  bool equal(const T &t1, const T &t2) const { return t1 == t2; }
};

template <typename Dimensions, typename Metrics> class HashTable {
public:
  using TimedMetrics = std::pair<Metrics, time_t>;
  using Records = std::vector<std::pair<Dimensions, TimedMetrics>>;

  enum class AggregationMethod : uint8_t { SUM, AVERAGE, MIN, MAX };

  void AddEntry(const Dimensions &dimensions, const TimedMetrics &metrics);

  template <
      typename AggregationMethods,
      typename std::enable_if_t<std::tuple_size<Metrics>::value ==
                                std::tuple_size<AggregationMethods>::value>>
  Records Clone(AggregationMethods aggregation_methods);

  Records Clone();

private:
  using TBBHashTable =
      tbb::concurrent_hash_map<Dimensions, std::vector<TimedMetrics>,
                               TupleHasher<Dimensions>>;

  TBBHashTable tbb_hash_table;
};

#include "hash_table_inl.h"

#endif
