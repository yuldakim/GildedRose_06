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

// 6단계 특정 아이템 관련 동작 함수분리
void updateAgedBrie(Item &item) {
  increaseQuality(item);
  if (item.sellIn < 0) {
    increaseQuality(item);
  }
}

void updateNormalItem(Item &item) {
  decreaseQuality(item);
  if (item.sellIn < 0) {
    decreaseQuality(item);
  }
}

void updateBackstagePass(Item &item) {
  if (item.sellIn < 0) {
    item.quality = 0;
    return;
  }

  increaseQuality(item); // 기본 +1

  if (item.sellIn < 10) {
    increaseQuality(item); // 10일 이하 추가 +1
  }

  if (item.sellIn < 5) {
    increaseQuality(item); // 5일 이하 추가 +1
  }
}

void GildedRose::updateQuality() {
  // 6단계 반복문 내 조건 단순화
  for (auto &item : items) {

    // 1단계 Sulfuras의 퀄리티와 유효기간은 변화가 없다.
    // 그래서 Loop 최상단에서 제외한다.
    if (item.name == "Sulfuras, Hand of Ragnaros")
      continue;

    // 2단계 유통기한 감소
    // 4단계 유통기한 감소 위치 변경 퀄리티 계산 후 -> 계산 전
    item.sellIn--;

    // 5단계 아이템별 로직 완전 분리
    if (item.name == "Aged Brie") {
      updateAgedBrie(item);
    } else if (item.name == "Backstage passes to a TAFKAL80ETC concert") {
      updateBackstagePass(item);
    } else {
      updateNormalItem(item);
    }
  }
}
