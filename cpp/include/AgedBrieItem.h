#include "GildedRoseItem.h"
class AgedBrieItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override {
    increaseQuality();
    if (item.sellIn < 0)
      increaseQuality();
  }
};