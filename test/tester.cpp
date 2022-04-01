#include "ULL.hpp"
#include <vector>
#include <gtest/gtest.h>
// ------------------------------------------------------------------------
TEST(UllTest, Get) {
   ULL<int> ull;
   ull.append(42);

   EXPECT_EQ(ull.get(-1), nullptr);
   EXPECT_EQ(ull.get(1), nullptr);
   EXPECT_EQ(*ull.get(0), 42);
}
// ------------------------------------------------------------------------
TEST(UllTest, InsertionCaseOne) {
   std::vector<int> expected{0, 42, 1, 2, 3, 4, 5, 6, 7, 8, 9};

   ULL<int> ull;

   ASSERT_TRUE(ull.is_empty());

   for (int i = 0; i < 10; ++i) {
      ull.append(i);
   }
   ull.insert_at(1, 42);

   EXPECT_EQ(ull.length, expected.size());
   EXPECT_EQ(ull.node_count, ull.length / ull.get_block_size());
   for (int i = 0; i < expected.size(); ++i) {
      EXPECT_EQ(*ull.get(i), expected[i]);
   }
}
// ------------------------------------------------------------------------
TEST(UllTest, InsertionCaseTwo) {
   std::vector<int> expected = {0, 42, 1, 2, 3, 4, 5, 6, 7};

   ULL<int> ull;

   ASSERT_TRUE(ull.is_empty());

   for (int i = 0; i < 8; ++i) {
      ull.append(i);
   }
   ull.insert_at(1, 42);

   EXPECT_EQ(ull.length, expected.size());
   EXPECT_EQ(ull.node_count, ull.length / ull.get_block_size());
   for (int i = 0; i < expected.size(); ++i) {
      EXPECT_EQ(*ull.get(i), expected[i]);
   }
}
// ------------------------------------------------------------------------
TEST(UllTest, InsertionCaseThreeBlockSizeThree) {
   std::vector<int> expected = {0, 42, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

   ULL<int> ull;

   ASSERT_TRUE(ull.is_empty());

   for (int i = 0; i < 16; ++i) {
      ull.append(i);
   }
   ull.insert_at(1, 42);

   EXPECT_EQ(ull.length, expected.size());
   EXPECT_EQ(ull.node_count, ull.length / ull.get_block_size());
   for (int i = 0; i < expected.size(); ++i) {
      EXPECT_EQ(*ull.get(i), expected[i]);
   }
}
// ------------------------------------------------------------------------
TEST(UllTest, InsertionCaseThreeBlockSizeFour) {
   std::vector<int> expected = {
      0, 42, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};

   ULL<int, 4> ull;
   for (int i = 0; i < 25; ++i) {
      ull.append(i);
   }
   ull.insert_at(1, 42);

   EXPECT_EQ(ull.length, expected.size());
   EXPECT_EQ(ull.node_count, ull.length / ull.get_block_size());
   for (int i = 0; i < expected.size(); ++i) {
      EXPECT_EQ(*ull.get(i), expected[i]);
   }
}
// ------------------------------------------------------------------------
TEST(UllTest, InsetionAppendPrepend) {
   std::vector<int> expected = {-15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

   ULL<int> ull;

   ASSERT_TRUE(ull.is_empty());

   for (int i = 1; i < 10; ++i) {
      ull.append(i);
   }

   for (int i = -1; i > -10; --i) {
      ull.prepend(i);
   }

   for (int i = 10; i < 16; ++i) {
      ull.append(i);
   }

   for (int i = -10; i > -16; --i) {
      ull.prepend(i);
   }

   ull.insert_at(11, 42);

   for (int i = 0; i < expected.size(); ++i) {
      EXPECT_EQ(*ull.get(i), expected[i]);
   }
   ull.print_list();
}
// ------------------------------------------------------------------------
