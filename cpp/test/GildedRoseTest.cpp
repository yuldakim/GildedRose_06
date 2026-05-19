#include <gtest/gtest.h>
#include "GildedRose.h"

class GildedRoseTest : public ::testing::Test {
protected:
    std::vector<Item> items;

    void updateQuality() {
        GildedRose app(items);
        app.updateQuality();
    }
};

TEST_F(GildedRoseTest, NormalItemDecreasesQualityAndSellInBeforeSellDate) {
    // Given
    items = { Item("Elixir of the Mongoose", 10, 20) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(9, items[0].sellIn);
    EXPECT_EQ(19, items[0].quality);
}

TEST_F(GildedRoseTest, NormalItemDegradesTwiceAsFastOnSellDate) {
    // Given
    items = { Item("+5 Dexterity Vest", 0, 10) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(-1, items[0].sellIn);
    EXPECT_EQ(8, items[0].quality);
}

TEST_F(GildedRoseTest, NormalItemDegradesTwiceAsFastAfterSellDate) {
    // Given
    items = { Item("+5 Dexterity Vest", -1, 10) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(-2, items[0].sellIn);
    EXPECT_EQ(8, items[0].quality);
}

TEST_F(GildedRoseTest, NormalItemQualityNeverDropsBelowZero) {
    // Given
    items = { Item("+5 Dexterity Vest", 5, 0) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(4, items[0].sellIn);
    EXPECT_EQ(0, items[0].quality);
}

TEST_F(GildedRoseTest, NormalItemQualityIsClampedAtZeroOnSellDate) {
    // Given
    items = { Item("+5 Dexterity Vest", 0, 1) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(-1, items[0].sellIn);
    EXPECT_EQ(0, items[0].quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesQualityBeforeSellDate) {
    // Given
    items = { Item("Aged Brie", 10, 20) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(9, items[0].sellIn);
    EXPECT_EQ(21, items[0].quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesQualityTwiceOnSellDate) {
    // Given
    items = { Item("Aged Brie", 0, 20) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(-1, items[0].sellIn);
    EXPECT_EQ(22, items[0].quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesQualityTwiceAfterSellDate) {
    // Given
    items = { Item("Aged Brie", -1, 20) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(-2, items[0].sellIn);
    EXPECT_EQ(22, items[0].quality);
}

TEST_F(GildedRoseTest, AgedBrieQualityNeverExceedsFifty) {
    // Given
    items = { Item("Aged Brie", 10, 50) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(9, items[0].sellIn);
    EXPECT_EQ(50, items[0].quality);
}

TEST_F(GildedRoseTest, AgedBrieQualityIsClampedAtFiftyOnSellDate) {
    // Given
    items = { Item("Aged Brie", 0, 49) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(-1, items[0].sellIn);
    EXPECT_EQ(50, items[0].quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesQualityByOneMoreThanTenDaysOut) {
    // Given
    items = { Item("Backstage passes to a TAFKAL80ETC concert", 11, 20) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(10, items[0].sellIn);
    EXPECT_EQ(21, items[0].quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesQualityByTwoAtTenDaysOut) {
    // Given
    items = { Item("Backstage passes to a TAFKAL80ETC concert", 10, 20) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(9, items[0].sellIn);
    EXPECT_EQ(22, items[0].quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesQualityByThreeAtFiveDaysOut) {
    // Given
    items = { Item("Backstage passes to a TAFKAL80ETC concert", 5, 20) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(4, items[0].sellIn);
    EXPECT_EQ(23, items[0].quality);
}

TEST_F(GildedRoseTest, BackstagePassQualityDropsToZeroOnSellDate) {
    // Given
    items = { Item("Backstage passes to a TAFKAL80ETC concert", 0, 20) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(-1, items[0].sellIn);
    EXPECT_EQ(0, items[0].quality);
}

TEST_F(GildedRoseTest, BackstagePassQualityNeverExceedsFifty) {
    // Given
    items = { Item("Backstage passes to a TAFKAL80ETC concert", 5, 49) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(4, items[0].sellIn);
    EXPECT_EQ(50, items[0].quality);
}

TEST_F(GildedRoseTest, SulfurasDoesNotChangeBeforeSellDate) {
    // Given
    items = { Item("Sulfuras, Hand of Ragnaros", 10, 80) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(10, items[0].sellIn);
    EXPECT_EQ(80, items[0].quality);
}

TEST_F(GildedRoseTest, SulfurasDoesNotChangeOnSellDate) {
    // Given
    items = { Item("Sulfuras, Hand of Ragnaros", 0, 80) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(0, items[0].sellIn);
    EXPECT_EQ(80, items[0].quality);
}

TEST_F(GildedRoseTest, SulfurasDoesNotChangeAfterSellDate) {
    // Given
    items = { Item("Sulfuras, Hand of Ragnaros", -1, 80) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(-1, items[0].sellIn);
    EXPECT_EQ(80, items[0].quality);
}

TEST_F(GildedRoseTest, SulfurasQualityIsAllowedAboveFifty) {
    // Given
    items = { Item("Sulfuras, Hand of Ragnaros", 5, 80) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(5, items[0].sellIn);
    EXPECT_EQ(80, items[0].quality);
}

TEST_F(GildedRoseTest, SulfurasQualityIsNotClampedWhenItStartsAtFifty) {
    // Given
    items = { Item("Sulfuras, Hand of Ragnaros", 5, 50) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(5, items[0].sellIn);
    EXPECT_EQ(50, items[0].quality);
}

TEST_F(GildedRoseTest, ConjuredItemDecreasesQualityTwiceAsFastBeforeSellDate) {
    // Given
    items = { Item("Conjured", 10, 20) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(9, items[0].sellIn);
    EXPECT_EQ(18, items[0].quality);
}

TEST_F(GildedRoseTest, ConjuredItemDecreasesQualityTwiceOnSellDate) {
    // Given
    items = { Item("Conjured", 0, 20) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(-1, items[0].sellIn);
    EXPECT_EQ(18, items[0].quality);
}

TEST_F(GildedRoseTest, ConjuredItemDecreasesQualityFourTimesAfterSellDate) {
    // Given
    items = { Item("Conjured", -1, 20) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(-2, items[0].sellIn);
    EXPECT_EQ(16, items[0].quality);
}

TEST_F(GildedRoseTest, ConjuredItemQualityNeverDropsBelowZero) {
    // Given
    items = { Item("Conjured", 10, 0) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(9, items[0].sellIn);
    EXPECT_EQ(0, items[0].quality);
}

TEST_F(GildedRoseTest, ConjuredPrefixItemsUseConjuredRules) {
    // Given
    items = { Item("Conjured Mana Cake", -1, 3) };

    // When
    updateQuality();

    // Then
    ASSERT_EQ(1, items.size());
    EXPECT_EQ(-2, items[0].sellIn);
    EXPECT_EQ(0, items[0].quality);
}
