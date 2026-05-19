#include "GildedRose.h"
#include "AgedBrieItem.h"
#include "BackstagePassItem.h"
#include "FoodBeverageItem.h"
#include "NormalItem.h"
#include "SulfurasItem.h"

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

// 팩토리 함수: 아이템 이름에 맞는 객체를 생성하여 반환
std::unique_ptr<GildedRoseItem> GildedRose::createItem(Item &item) {
  if (item.name == AGED_BRIE)
    return std::make_unique<AgedBrieItem>(item);
  if (item.name == BACKSTAGE_PASS)
    return std::make_unique<BackstagePassItem>(item);
  if (item.name == SULFURAS)
    return std::make_unique<SulfurasItem>(item);
  if (item.name.find("[F&B]") != std::string::npos)
    return std::make_unique<FoodBeverageItem>(item);
  return std::make_unique<NormalItem>(item);
}

void GildedRose::updateQuality() {
  for (auto &item : items) {
    auto gildedItem = createItem(item);

    // 다형성을 활용하여 각 객체에 정의된 로직을 실행
    gildedItem->updateSellIn();  // 1단계: 유통기한 처리
    gildedItem->updateQuality(); // 2단계: 품질 처리
  }
}