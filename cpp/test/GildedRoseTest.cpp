#include "GildedRose.h"
#include <gtest/gtest.h>

// TEST(GildedRoseTest, Foo) {
//   std::vector<Item> items = {Item("foo", 0, 0)};
//   GildedRose app(items);
//   app.updateQuality();
//   EXPECT_EQ("fixme", app.items[0].name);
// }

// 1. 일반 아이템: 품질이 0이면 더 이상 감소하지 않음
TEST(GildedRoseTest, NonameQualityZeroNoDecrease) {
  std::vector<Item> items = {Item("noname", 0, 0)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, items[0].sellIn);
  EXPECT_EQ(0, items[0].quality);
}

// 2. 일반 아이템: 유통기한 지나면 품질 2배 감소 (-2)
TEST(GildedRoseTest, NonameQualityDecreaseDoubleAfterSellIn) {
  std::vector<Item> items = {Item("noname", 0, 5)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, items[0].sellIn);
  EXPECT_EQ(3, items[0].quality);
}

// 3. 전설템(Sulfuras): 유통기한 0일 때 변화 없음
TEST(GildedRoseTest, SulfurasNoChangeAtSellInZero) {
  std::vector<Item> items = {Item("Sulfuras, Hand of Ragnaros", 0, 5)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(0, items[0].sellIn);
  EXPECT_EQ(5, items[0].quality);
}

// 4. 전설템(Sulfuras): 유통기한 지났어도 변화 없음
TEST(GildedRoseTest, SulfurasNoChangeEvenAfterSellIn) {
  std::vector<Item> items = {Item("Sulfuras, Hand of Ragnaros", -1, 5)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, items[0].sellIn);
  EXPECT_EQ(5, items[0].quality);
}
