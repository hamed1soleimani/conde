#include <gtest/gtest.h>

#include "hash_table.h"

using namespace std;
 
TEST(HashTableTest, FirstTest) { 
	HashTable<tuple<string>, tuple<int>> table;
	EXPECT_EQ(table.Clone().size(), 0);
}
