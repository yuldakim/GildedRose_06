#include "GildedRoseItem.h"
class SulfurasItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override { /* 전설 아이템은 품질 변화 없음 */ }
  void updateSellIn() override { /* 전설 아이템은 유통기한 변화 없음 */ }
};