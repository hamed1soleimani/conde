#ifndef HASH_TABLE_INL_H_
#define HASH_TABLE_INL_H_

template <typename Dimensions, typename Metrics>
void HashTable<Dimensions, Metrics>::AddEntry(const Dimensions &dimensions,
                                              const TimedMetrics &metrics) {
  typename TBBHashTable::accessor accessor;
  tbb_hash_table.insert(
      accessor, std::pair<const Dimensions &, std::vector<TimedMetrics>>{
                    dimensions, std::vector<TimedMetrics>{}});
  accessor->second.push_back(metrics);
}

template <typename Dimensions, typename Metrics>
template <typename AggregationMethods,
          typename std::enable_if_t<std::tuple_size<Metrics>::value ==
                                    std::tuple_size<AggregationMethods>::value>>
auto HashTable<Dimensions, Metrics>::Clone(
    AggregationMethods aggregation_methods) -> Records {
  throw "not implemented yet!";
}

template <typename Dimensions, typename Metrics>
auto HashTable<Dimensions, Metrics>::Clone() -> Records {
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

#endif
