#include <ctime>

#include <thread>

#include <gtest/gtest.h>

#include "hash_table.h"

using namespace std;

using SampleTable = HashTable<tuple<string>, tuple<int>>;

TEST(HashTableTest,
     unaggregated_clone_size_should_be_zero_when_hash_table_is_empty) {
  SampleTable table;
  EXPECT_EQ(table.Clone().size(), 0);
}

TEST(HashTableTest,
     unaggregated_clone_should_contains_valid_contents_with_one_entry) {
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

TEST(
    HashTableTest,
    unaggregated_clone_shold_contains_duplicated_records_when_duplicates_added) {
  SampleTable table;
  const std::string dimension = "fist_dimension_value";
  const int metric = 999;
  const time_t now = time(nullptr);
  table.AddEntry(dimension, SampleTable::TimedMetrics(metric, now));
  table.AddEntry(tuple<string>(dimension),
                 SampleTable::TimedMetrics(tuple<int>(metric), now));
  SampleTable::Records records = table.Clone();
  EXPECT_EQ(records.size(), 2);
  EXPECT_EQ(std::get<0>(records[0].first), dimension);
  EXPECT_EQ(std::get<0>(records[0].second.first), metric);
  EXPECT_EQ(records[0].second.second, now);
  EXPECT_EQ(std::get<0>(records[0].first), std::get<0>(records[1].first));
  EXPECT_EQ(std::get<0>(records[0].second.first),
            std::get<0>(records[1].second.first));
  EXPECT_EQ(records[0].second.second, records[1].second.second);
}

TEST(HashTableTest,
     unaggregated_clone_should_count_duplicated_records_at_different_times) {
  SampleTable table;
  const std::string dimension = "fist_dimension_value";
  const int metric = 999;
  const time_t now = time(nullptr);
  table.AddEntry(dimension, SampleTable::TimedMetrics(metric, now));
  table.AddEntry(tuple<string>(dimension),
                 SampleTable::TimedMetrics(tuple<int>(metric), now + 1));
  SampleTable::Records records = table.Clone();
  EXPECT_EQ(records.size(), 2);
}

TEST(
    HashTableTest,
    unaggregated_clone_should_contains_correct_elements_while_adding_1000_entries) {
  SampleTable table;
  const std::string dimension = "dimension_value";
  const time_t now = time(nullptr);
  for (int i = 0; i < 1000; i++) {
    table.AddEntry(dimension + to_string(i), SampleTable::TimedMetrics(i, now));
  }
  SampleTable::Records records = table.Clone();
  EXPECT_EQ(records.size(), 1000);
}

TEST(HashTableTest, table_should_be_empty_right_after_the_clone) {
  SampleTable table;
  const std::string dimension = "dimension_value";
  const time_t now = time(nullptr);
  for (int i = 0; i < 1000; i++) {
    table.AddEntry(dimension + to_string(i), SampleTable::TimedMetrics(i, now));
  }
  SampleTable::Records records = table.Clone();
  EXPECT_EQ(records.size(), 1000);
  SampleTable::Records records1 = table.Clone();
  EXPECT_EQ(records1.size(), 0);
}

TEST(HashTableTest,
     hash_table_should_handle_too_much_inserts_from_different_threads) {
  SampleTable table;
  const std::string dimension = "dimension_value";
  auto lambda_inserter = [&](int thread_id) {
    for (int i = 0; i < 1000000; i++) {
      table.AddEntry(to_string(thread_id) + ":" + dimension + to_string(i),
                     SampleTable::TimedMetrics(i * thread_id, time(nullptr)));
    }
  };

  std::vector<std::thread> threads;

  for (int i = 0; i < 10; i++) {
    threads.push_back(std::thread(lambda_inserter, i));
  }

  for (int i = 0; i < 10; i++) {
    threads[i].join();
  }

  SampleTable::Records records = table.Clone();
  EXPECT_EQ(records.size(), 10 * 1000000);
}

TEST(HashTableTest,
     hash_table_should_handle_duplications_from_multiple_threads) {
  SampleTable table;
  const std::string dimension = "dimension_value";
  const time_t now = time(nullptr);
  auto lambda_inserter = [&]() {
    for (int i = 0; i < 1000000; i++) {
      table.AddEntry(dimension + to_string(i),
                     SampleTable::TimedMetrics(i, now));
    }
  };

  std::vector<std::thread> threads;

  for (int i = 0; i < 10; i++) {
    threads.push_back(std::thread(lambda_inserter));
  }

  for (int i = 0; i < 10; i++) {
    threads[i].join();
  }

  SampleTable::Records records = table.Clone();
  EXPECT_EQ(records.size(), 10000000);
}
