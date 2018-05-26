#ifndef HASH_TABLE
#define HASH_TABLE

#include <ctime>

#include <set>
#include <vector>

#include <boost/functional/hash.hpp>
#include <tbb/concurrent_hash_map.h>

template <typename Dimensions, typename Metrics, typename AggregationMethods,
          typename = std::enable_if_t<std::tuple_size_v<Metrics> ==
                                      std::tuple_size_v<AggregationMethods>>>
class HashTable {
public:
  using TimedMetrics = std::pair<Metrics, time_t>;
  using Records = std::vector<std::pair<Dimensions, TimedMetrics>>;

  enum class AggregationMethod : uint8_t { SUM, AVERAGE, MIN, MAX };

  void AddEntry(Dimensions dimensions, TimedMetrics metrics);
  Records Clone(bool aggregate);

private:
  using Hasher = boost::functional::hash<Dimensions>;
  using TBBHashTable =
      tbb::concurrent_hash_map<Dimensions, std::set<TimedMetrics>, Hasher>;

  TBBHashTable tbb_hash_table;
};

#endif
