#include "hash_table.h"

#include <cassert>

template <typename Dimensions, typename Metrics>
void HashTable<Dimensions, Metrics>::AddEntry(const Dimensions &dimensions,
                                              const TimedMetrics &metrics) {
  typename TBBHashTable::accessor accessor;
  tbb_hash_table.insert(accessor,
                        std::pair<const Dimensions &, std::set<TimedMetrics>>{
                            dimensions, std::set<TimedMetrics>{}});
  accessor->second.insert(metrics);
}

template <typename Dimensions, typename Metrics>
template <typename AggregationMethods,
          typename std::enable_if_t<std::tuple_size<Metrics>::value ==
                                    std::tuple_size<AggregationMethods>::value>>
auto HashTable<Dimensions, Metrics>::Clone(bool aggregate) -> Records {
  assert(!aggregate);
  TBBHashTable hash_table_clone;
  tbb_hash_table.swap(hash_table_clone);
  Records records;
  for (auto it = hash_table_clone.begin(); it != hash_table_clone.end(); ++it)
    for (auto innerit = it->second.begin(); innerit != it->second.end();
         ++innerit)
      records.push_back(
          std::pair<Dimensions, TimedMetrics>(it->first, *innerit));
  return records;
}
