#include <ctime>

#include <gtest/gtest.h>

#include "hash_table.h"

using namespace std;
 
using SampleTable = HashTable<tuple<string>, tuple<int>>;

TEST(HashTableTest, unaggregated_clone_size_should_be_zero_when_hash_table_is_empty) { 
	SampleTable table;
	EXPECT_EQ(table.Clone().size(), 0);
}

TEST(HashTableTest, unaggregated_clone_should_contains_valid_contents_with_one_entry) { 
	SampleTable table;
	const std::string dimension = "fist_dimension_value";
	const int metric = 999;
	const time_t now = time(nullptr);
	table.AddEntry(dimension, SampleTable::TimedMetrics(metric, now));
	SampleTable::Records records = table.Clone();
	EXPECT_EQ(records.size(), 1);
	EXPECT_EQ(std::get<0>(records[0].first), dimension); 
	EXPECT_EQ(std::get<0>(records[0].second.first), metric);
	EXPECT_EQ(records[0].second.second, now);
}
