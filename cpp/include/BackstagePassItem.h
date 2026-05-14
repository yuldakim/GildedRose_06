#include "GildedRoseItem.h"
class BackstagePassItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override {
    if (item.sellIn < 0) {
      item.quality = 0;
      return;
    }
    increaseQuality();
    if (item.sellIn < 10)
      increaseQuality();
    if (item.sellIn < 5)
      increaseQuality();
  }
};