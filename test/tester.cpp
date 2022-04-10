#include "ULL.hpp"
#include <random>
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
TEST(UllTest, InsertionCaseOneSingleBlock) {
   std::vector<int> expected = {0, 42, 1, 2};

   ULL<int> ull;
   for (int i = 0; i < 3; ++i) {
      ull.append(i);
   }
   ull.insert_at(1, 42);

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
TEST(UllTest, InsertionCaseTwoSingleBlock) {
   std::vector<int> expected{0, 42, 1, 2, 3};

   ULL<int> ull;

   ASSERT_TRUE(ull.is_empty());

   for (int i = 0; i < 4; ++i) {
      ull.append(i);
   }
   ull.insert_at(1, 42);

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
   std::vector<int> expected = {-15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, 42, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

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
}
// ------------------------------------------------------------------------
TEST(UllTest, Iterator) {
   std::vector<int> expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

   ULL<int> ull;

   for (int i = 0; i < 10; ++i) {
      ull.append(i);
   }

   int i = 0;
   for (auto it = ull.begin(); it != ull.end(); ++it) {
      EXPECT_EQ(*it, expected[i]);
      ++i;
   }

   i = 0;
   for (auto it : ull) {
      EXPECT_EQ(it, expected[i]);
      ++i;
   }
}
// ------------------------------------------------------------------------
TEST(UllTest, IteratorDecrement) {
   std::vector<int> expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

   ULL<int> ull;

   for (int i = 0; i < 11; ++i) {
      ull.append(i);
   }

   auto it = ull.begin();
   for (int i = 0; i < 10; ++i) {
      EXPECT_EQ(*it, expected[i]);
      ++it;
   }

   for (int i = 10; i >= 0; --i) {
      EXPECT_EQ(*it, expected[i]);
      --it;
   }

   EXPECT_EQ(it, ull.end());
}
// ------------------------------------------------------------------------
TEST(UllTest, IteratorSubscript) {
   std::vector<int> expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

   ULL<int> ull;

   for (int i = 0; i < 10; ++i) {
      ull.append(i);
   }

   int i = 0;
   for (auto it = ull.begin(); it != ull.end(); ++it) {
      EXPECT_EQ(it[i], expected[i]);
      ++i;
   }
}
// ------------------------------------------------------------------------
TEST(UllTest, Subscript) {
   ULL<int> ull;

   for (int i = 0; i < 10; ++i) {
      ull.append(i);
   }

   ull[5] = 42;

   EXPECT_EQ(ull[5], 42);
}
// ------------------------------------------------------------------------
TEST(UllTest, RemoveCaseOne) {
   std::vector<int> expected{0, 4, 8, 9, 10, 11};

   ULL<int> ull;

   for (int i = 0; i < 12; ++i) {
      ull.append(i);
   }

   // Prepare Case One
   ull.remove_at(2);
   ull.remove_at(2);
   ull.remove_at(3);
   ull.remove_at(3);
   ull.remove_at(3);

   ull.remove_at(1);

   for (int i = 0; i < expected.size(); ++i) {
      EXPECT_EQ(*ull.get(i), expected[i]);
   }
}
// ------------------------------------------------------------------------
TEST(UllTest, RemoveCaseTwo) {
   std::vector<int> expected{1, 5, 7};

   ULL<int> ull;

   for (int i = 0; i < 8; ++i) {
      ull.append(i);
   }

   // Prepare Case Two
   ull.remove_at(0);
   ull.remove_at(1);
   ull.remove_at(2);
   ull.remove_at(3);

   ull.remove_at(1);

   for (int i = 0; i < expected.size(); ++i) {
      EXPECT_EQ(*ull.get(i), expected[i]);
   }
}
// ------------------------------------------------------------------------
TEST(UllTest, RemoveCaseThree) {
   std::vector<int> expected{1, 5, 7, 9, 10, 12, 13, 14, 15};

   ULL<int> ull;

   for (int i = 0; i < 16; ++i) {
      ull.append(i);
   }

   // Prepare Case Three
   ull.remove_at(0);
   ull.remove_at(2);
   ull.remove_at(2);
   ull.remove_at(3);
   ull.remove_at(4);
   ull.remove_at(6);

   ull.remove_at(1);

   for (int i = 0; i < expected.size(); ++i) {
      EXPECT_EQ(*ull.get(i), expected[i]);
   }
}
// ------------------------------------------------------------------------
TEST(UllTest, InsertionAndRemove) {
   ULL<int> ull;

   for (int i = 0; i < 8; ++i) {
      ull.append(i);
   }

   ull.remove_at(4);
   ull.remove_at(6);
   ull.remove_at(2);
   ull.remove_at(1);

   for (int i = 8; i < 16; ++i) {
      ull.insert_at(2, i);
   }

   ull.remove_at(7);
   ull.remove_at(5);
   ull.remove_at(3);
   ull.remove_at(2);
   ull.remove_at(3);
   ull.remove_at(3);
   ull.remove_at(1);
   ull.remove_at(0);
   ull.remove_at(0);
   ull.remove_at(2);
   ull.remove_at(1);
   ull.remove_at(0);

   EXPECT_TRUE(ull.is_empty());
}
// ------------------------------------------------------------------------
TEST(UllTest, ManyInsertionsAndRemove) {
   ULL<int> ull;
   ull.append(0);

   std::random_device rd;
   std::mt19937 gen(rd());
   for (int i = 1; i < 10'000; ++i) {
      std::uniform_int_distribution<> dist(0, ull.length - 1);
      ull.insert_at(dist(gen), i);
   }

   for (int i = 1; i < 10'000; ++i) {
      std::uniform_int_distribution<> dist(0, ull.length - 1);
      ull.remove_at(dist(gen));
   }

   ull.remove_at(0);

   EXPECT_TRUE(ull.is_empty());
}
// ------------------------------------------------------------------------
