#include "GildedRoseItem.h"
class NormalItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override {
    decreaseQuality();
    if (item.sellIn < 0)
      decreaseQuality();
  }
};