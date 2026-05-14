#pragma once
#include "GildedRoseItem.h"
#include "Item.h"
#include <memory>
#include <vector>


class GildedRose {
private:
  static constexpr const char *AGED_BRIE = "Aged Brie";
  static constexpr const char *BACKSTAGE_PASS =
      "Backstage passes to a TAFKAL80ETC concert";
  static constexpr const char *SULFURAS = "Sulfuras, Hand of Ragnaros";

  std::unique_ptr<GildedRoseItem> createItem(Item &item);

public:
  std::vector<Item> &items;
  GildedRose(std::vector<Item> &items);
  void updateQuality();
};