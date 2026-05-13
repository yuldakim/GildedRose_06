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

// 5. Aged Brie: 유통기한 지나면 품질 2배 증가 (+2)
TEST(GildedRoseTest, AgedBrieIncreaseDoubleAfterSellIn) {
  std::vector<Item> items = {Item("Aged Brie", 0, 0)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, items[0].sellIn);
  EXPECT_EQ(2, items[0].quality);
}

// 6-1. Aged Brie: 품질 최대치 50 제한
TEST(GildedRoseTest, AgedBrieQualityMax50) {
  std::vector<Item> items = {Item("Aged Brie", 0, 50)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, items[0].sellIn);
  EXPECT_EQ(50, items[0].quality);
}

// 7. Backstage Pass: 유통기한 10일 초과 시 품질 +1
TEST(GildedRoseTest, BackstagePassIncreaseNormalBefore10Days) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 15, 0)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(14, items[0].sellIn);
  EXPECT_EQ(1, items[0].quality);
}

// 8. Backstage Pass: 유효기간 지난 날 → 품질 = 0
TEST(GildedRoseTest, BackstagePassNormalExpiration) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 0, 10)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, items[0].sellIn);
  EXPECT_EQ(0, items[0].quality);
}

// 9. Backstage Pass: 품질이 초과였지만 유효기간 지나서 0 처리
TEST(GildedRoseTest, BackstagePassAbnormalQualityExpiration) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 0, 51)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, items[0].sellIn);
  EXPECT_EQ(0, items[0].quality);
}