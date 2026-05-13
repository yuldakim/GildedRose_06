#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

// 2단계 퀄리티 상승 함수 분리
void increaseQuality(Item &item) {
  if (item.quality < 50) {
    item.quality = item.quality + 1;
  }
}

// 2단계 퀄리티 하락 함수 분리
void decreaseQuality(Item &item) {
  if (item.quality > 0) {
    item.quality = item.quality - 1;
  }
}

void GildedRose::updateQuality() {
  for (size_t i = 0; i < items.size(); i++) {
    // 1단계 Sulfuras의 퀄리티와 유효기간은 변화가 없다.
    // 그래서 Loop 최상단에서 제외한다.
    if (items[i].name == "Sulfuras, Hand of Ragnaros")
      continue;

    if (items[i].name == "Aged Brie" ||
        items[i].name == "Backstage passes to a TAFKAL80ETC concert") {
      // 2단계 Aged Brie와 Backstage Passes가 10초과인경우
      increaseQuality(items[i]);
      if (items[i].name == "Backstage passes to a TAFKAL80ETC concert") {
        if (items[i].sellIn < 11)
          increaseQuality(items[i]); // 2단계 Backstage passes Quality +2 증가
        if (items[i].sellIn < 6)
          increaseQuality(items[i]); // 2단계 Backstage passes Quality +3 증가
      }
    } else {
      decreaseQuality(items[i]);
    }

    // 2단계 유통기한 감소
    items[i].sellIn = items[i].sellIn - 1;

    // 2단계 유통기간 만료 후 처리
    if (items[i].sellIn < 0) {
      if (items[i].name == "Aged Brie") {
        increaseQuality(items[i]);
      } else if (items[i].name == "Backstage passes to a TAFKAL80ETC concert") {
        items[i].quality = 0;
      } else {
        decreaseQuality(items[i]);
      }
    }
  }
}
