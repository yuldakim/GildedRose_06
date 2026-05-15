#define APPROVALS_GOOGLETEST

#include "ApprovalTests.hpp"
#include "GildedRose.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(GildedRoseTest, UpdateQualityApproval) {
  std::vector<Item> items = {
      {"noname", 0, 0},
      {"noname", 0, 5},
      {"Sulfuras, Hand of Ragnaros", 0, 5},
      {"Sulfuras, Hand of Ragnaros", -1, 5},
      {"Aged Brie", 0, 0},
      {"Aged Brie", 0, 50},
      {"Backstage passes to a TAFKAL80ETC concert", 15, 0},
      {"Backstage passes to a TAFKAL80ETC concert", 0, 0},
      {"Backstage passes to a TAFKAL80ETC concert", 0, 51},
      {"[F&B] Bread", 5, 20},
      {"[F&B] Milk", 0, 20},
      {"[F&B] Water", 0, 1}};

  GildedRose app(items);
  app.updateQuality();

  std::vector<std::string> results;
  for (const auto &item : app.items) {
    results.push_back(item.name + ", " + std::to_string(item.sellIn) + ", " +
                      std::to_string(item.quality));
  }

  // 빈배열 케이스는 따로 분리
  std::vector<Item> emptyItems;
  GildedRose emptyApp(emptyItems);
  emptyApp.updateQuality();
  results.push_back("Empty case item count: " +
                    std::to_string(emptyApp.items.size()));

  ApprovalTests::Approvals::verifyAll("ItemsAfterUpdate", results);
}