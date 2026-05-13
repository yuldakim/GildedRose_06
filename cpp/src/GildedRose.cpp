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
    // 3단계 Item[i] 보다는 참조문 사용하여 한눈에 들어오게 개선
    Item &item = items[i];

    // 1단계 Sulfuras의 퀄리티와 유효기간은 변화가 없다.
    // 그래서 Loop 최상단에서 제외한다.
    if (item.name == "Sulfuras, Hand of Ragnaros")
      continue;

    if (item.name == "Aged Brie" ||
        item.name == "Backstage passes to a TAFKAL80ETC concert") {
      // 2단계 Aged Brie와 Backstage Passes가 10초과인경우
      increaseQuality(item);
      if (item.name == "Backstage passes to a TAFKAL80ETC concert") {
        if (item.sellIn < 11)
          increaseQuality(item); // 2단계 Backstage passes Quality +2 증가
        if (item.sellIn < 6)
          increaseQuality(item); // 2단계 Backstage passes Quality +3 증가
      }
    } else {
      decreaseQuality(item);
    }

    // 2단계 유통기한 감소
    item.sellIn = item.sellIn - 1;

    // 2단계 유통기간 만료 후 처리
    if (item.sellIn < 0) {
      if (item.name == "Aged Brie") {
        increaseQuality(item);
      } else if (item.name == "Backstage passes to a TAFKAL80ETC concert") {
        item.quality = 0;
      } else {
        decreaseQuality(item);
      }
    }
  }
}
