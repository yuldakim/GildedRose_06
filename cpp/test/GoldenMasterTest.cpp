#include <gtest/gtest.h>

#include "GildedRose.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {
constexpr int GoldenMasterDays = 30;

std::vector<Item> createTexttestFixtureItems() {
    return {
        Item("+5 Dexterity Vest", 10, 20),
        Item("Aged Brie", 2, 0),
        Item("Elixir of the Mongoose", 5, 7),
        Item("Sulfuras, Hand of Ragnaros", 0, 80),
        Item("Sulfuras, Hand of Ragnaros", -1, 80),
        Item("Backstage passes to a TAFKAL80ETC concert", 15, 20),
        Item("Backstage passes to a TAFKAL80ETC concert", 10, 49),
        Item("Backstage passes to a TAFKAL80ETC concert", 5, 49),
        Item("Conjured Mana Cake", 3, 6),
    };
}

std::string renderTexttestFixtureOutput() {
    std::vector<Item> items = createTexttestFixtureItems();
    GildedRose app(items);
    std::ostringstream output;

    output << "OMGHAI!\n";
    for (int day = 0; day <= GoldenMasterDays; ++day) {
        output << "-------- day " << day << " --------\n";
        output << "name, sellIn, quality\n";
        for (const Item& item : items) {
            output << item.toString() << '\n';
        }
        output << '\n';

        app.updateQuality();
    }

    return output.str();
}

void writeReceivedOutput(const std::string& output) {
    std::ofstream receivedFile(GOLDEN_MASTER_RECEIVED_FILE, std::ios::binary);
    ASSERT_TRUE(receivedFile.is_open())
        << "Cannot write received golden master file: " << GOLDEN_MASTER_RECEIVED_FILE;
    receivedFile << output;
}
}  // namespace

TEST(GoldenMasterTest, TexttestFixtureOutputMatchesApprovedFile) {
    // Given
    std::ifstream approvedFile(GOLDEN_MASTER_APPROVED_FILE, std::ios::binary);
    ASSERT_TRUE(approvedFile.is_open())
        << "Cannot open approved golden master file: " << GOLDEN_MASTER_APPROVED_FILE;
    std::ostringstream expectedContents;
    expectedContents << approvedFile.rdbuf();
    const std::string expected = expectedContents.str();

    // When
    const std::string actual = renderTexttestFixtureOutput();

    // Then
    if (actual != expected) {
        writeReceivedOutput(actual);
    }

    EXPECT_EQ(expected, actual)
        << "Golden master changed. Review "
        << GOLDEN_MASTER_RECEIVED_FILE
        << " and approve it by replacing "
        << GOLDEN_MASTER_APPROVED_FILE
        << " when the behavior change is intentional.";
}
