#include "hash_table.h"

template <typename Dimensions, typename Metrics>
void HashTable<Dimensions, Metrics>::AddEntry(const Dimensions &dimensions,
                                              const TimedMetrics &metrics) {
  TBBHashTable::accessor accessor;
  tbb_hash_table.insert(accessor,
                        std::pair<const Dimensions &, std::set<TimedMetrics>>{
                            dimensions, std::set<TimedMetrics>{}});
  accessor->second.insert(metrics);
}
