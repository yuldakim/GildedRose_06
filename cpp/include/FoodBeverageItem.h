#include "GildedRoseItem.h"
class FoodBeverageItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;

  void updateQuality() override {
    // 일반의2배감소
    if (item.quality > 0) {
      decreaseQuality();
      if (item.quality > 0)
        decreaseQuality(); // 2배
    }
    // sellIn 지나면추가2배(총4배)
    if (item.sellIn < 1) {
      if (item.quality > 0) {
        decreaseQuality();
        if (item.quality > 0)
          decreaseQuality();
      }
    }
  }
};