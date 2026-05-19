#pragma once
#include "Item.h"

class GildedRoseItem {
protected:
  Item &item;

public:
  GildedRoseItem(Item &item) : item(item) {}
  virtual ~GildedRoseItem() = default;

  // 아이템별로 다르게 동작할 핵심 로직
  virtual void updateQuality() = 0;

  // 유통기한 감소 (Sulfuras 제외 모든 아이템 공통)
  virtual void updateSellIn() { item.sellIn--; }

protected:
  void increaseQuality() {
    if (item.quality < 50)
      item.quality++;
  }
  void decreaseQuality() {
    if (item.quality > 0)
      item.quality--;
  }
};