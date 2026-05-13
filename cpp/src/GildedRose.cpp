#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

void GildedRose::updateQuality() {
  for (size_t i = 0; i < items.size(); i++) {
    // 1단계 Sulfuras의 퀄리티와 유효기간은 변화가 없다.
    // 그래서 Loop 최상단에서 제외한다.
    if (items[i].name == "Sulfuras, Hand of Ragnaros")
      continue;

    if (items[i].name != "Aged Brie" &&
        items[i].name != "Backstage passes to a TAFKAL80ETC concert") {
      if (items[i].quality > 0) {
        // if (items[i].name != "Sulfuras, Hand of Ragnaros") {
        items[i].quality = items[i].quality - 1;
        // }
      }
    } else {
      if (items[i].quality < 50) {
        items[i].quality = items[i].quality + 1;

        if (items[i].name == "Backstage passes to a TAFKAL80ETC concert") {
          if (items[i].sellIn < 11) {
            if (items[i].quality < 50) {
              items[i].quality = items[i].quality + 1;
            }
          }

          if (items[i].sellIn < 6) {
            if (items[i].quality < 50) {
              items[i].quality = items[i].quality + 1;
            }
          }
        }
      }
    }

    // if (items[i].name != "Sulfuras, Hand of Ragnaros") {
    items[i].sellIn = items[i].sellIn - 1;
    // }

    if (items[i].sellIn < 0) {
      if (items[i].name != "Aged Brie") {
        if (items[i].name != "Backstage passes to a TAFKAL80ETC concert") {
          if (items[i].quality > 0) {
            // if (items[i].name != "Sulfuras, Hand of Ragnaros") {
            items[i].quality = items[i].quality - 1;
            // }
          }
        } else {
          items[i].quality = items[i].quality - items[i].quality;
        }
      } else {
        if (items[i].quality < 50) {
          items[i].quality = items[i].quality + 1;
        }
      }
    }
  }
}
