#include "GildedRose.h"

#include <algorithm>

namespace {
const char* const AgedBrie = "Aged Brie";
const char* const BackstagePass = "Backstage passes to a TAFKAL80ETC concert";
const char* const Sulfuras = "Sulfuras, Hand of Ragnaros";
const char* const ConjuredPrefix = "Conjured";

bool isConjured(const Item& item) {
    return item.name.rfind(ConjuredPrefix, 0) == 0;
}

void increaseQuality(Item& item, int amount) {
    item.quality = std::min(50, item.quality + amount);
}

void decreaseQuality(Item& item, int amount) {
    item.quality = std::max(0, item.quality - amount);
}
}  // namespace

GildedRose::GildedRose(std::vector<Item>& items) : items(items) {}

void GildedRose::updateQuality() {
    for (Item& item : items) {
        if (item.name == Sulfuras) {
            continue;
        }

        if (item.name == AgedBrie) {
            increaseQuality(item, item.sellIn <= 0 ? 2 : 1);
        } else if (item.name == BackstagePass) {
            if (item.sellIn <= 0) {
                item.quality = 0;
            } else if (item.sellIn <= 5) {
                increaseQuality(item, 3);
            } else if (item.sellIn <= 10) {
                increaseQuality(item, 2);
            } else {
                increaseQuality(item, 1);
            }
        } else if (isConjured(item)) {
            decreaseQuality(item, item.sellIn < 0 ? 4 : 2);
        } else {
            decreaseQuality(item, item.sellIn <= 0 ? 2 : 1);
        }

        item.sellIn -= 1;
    }
}
