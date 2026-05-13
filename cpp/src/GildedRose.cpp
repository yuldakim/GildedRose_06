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

    // 2단계 유통기한 감소
    // 4단계 유통기한 감소 위치 변경 퀄리티 계산 후 -> 계산 전
    item.sellIn--;

    // 5단계 아이템별 로직 완전 분리
    if (item.name == "Aged Brie") {
      increaseQuality(item);
      if (item.sellIn < 0) {
        increaseQuality(item);
      }
    }

    else if (item.name == "Backstage passes to a TAFKAL80ETC concert") {
      if (item.sellIn < 0) {
        item.quality = 0;
      } else {
        increaseQuality(item); // 기본 +1
        if (item.sellIn < 10)
          increaseQuality(item); // 추가 +1 (총 2)
        if (item.sellIn < 5)
          increaseQuality(item); // 추가 +1 (총 3)
      }
    } else {
      // 일반 아이템 (Normal Item)
      decreaseQuality(item);
      if (item.sellIn < 0) {
        decreaseQuality(item);
      }
    }
  }
}
