// 1. 반드시 include보다 위에 선언해야 합니다.
#define APPROVALS_GOOGLETEST

#include "ApprovalTests.hpp"
#include "GildedRose.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>


TEST(GildedRoseTest, UpdateQualityApproval) {
  // ... 기존 테스트 코드 ...
  std::vector<Item> items = {
      {"Normal Item", 10, 20},
      {"Aged Brie", 2, 0},
      {"Sulfuras, Hand of Ragnaros", 0, 80},
      {"Backstage passes to a TAFKAL80ETC concert", 15, 20}};
  GildedRose app(items);
  app.updateQuality();

  std::vector<std::string> results;
  for (const auto &item : app.items) {
    results.push_back(item.name + ", " + std::to_string(item.sellIn) + ", " +
                      std::to_string(item.quality));
  }

  ApprovalTests::Approvals::verifyAll("ItemsAfterUpdate", results);
}