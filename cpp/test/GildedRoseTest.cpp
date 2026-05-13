#include "GildedRose.h"
#include <gtest/gtest.h>

TEST(GildedRoseTest, Foo) {
  std::vector<Item> items = {Item("foo", 0, 0)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ("fixme", app.items[0].name);
}

// 1. 일반 아이템: 품질이 0이면 더 이상 감소하지 않음
TEST(GildedRoseTest, NonameQualityZeroNoDecrease) {
  std::vector<Item> items = {Item("noname", 0, 0)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, items[0].sellIn);
  EXPECT_EQ(0, items[0].quality);
}
