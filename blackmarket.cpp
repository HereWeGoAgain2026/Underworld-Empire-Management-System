// ============================================================
// blackmarket.cpp — Black Market Implementation
// Underworld Empire Management System
// ============================================================

#include "blackmarket.h"

BlackMarket::BlackMarket() : refreshCooldown(0) {
    initializeStock();
}

void BlackMarket::initializeStock() {
    inventory.clear();
    // Weapons
    inventory.push_back(BlackMarketItem("Combat Pistol",      "Reliable sidearm for your crew.",                "weapon",  1500, "weapons", 1));
    inventory.push_back(BlackMarketItem("Assault Rifle",      "Military-grade automatic weapon.",               "weapon",  4500, "weapons", 2));
    inventory.push_back(BlackMarketItem("Tactical Shotgun",   "Close-range devastation guaranteed.",            "weapon",  3000, "weapons", 1));
    inventory.push_back(BlackMarketItem("Sniper System",      "Long-range precision elimination.",              "weapon",  8000, "weapons", 3));
    // Armor
    inventory.push_back(BlackMarketItem("Kevlar Vest",        "Standard ballistic protection.",                 "armor",   2000, "armor", 1));
    inventory.push_back(BlackMarketItem("Tactical Body Armor","Military-spec full-body protection.",            "armor",   5500, "armor", 2));
    inventory.push_back(BlackMarketItem("Stealth Suit",       "Reduces detection during operations.",           "armor",   7000, "armor", 3));
    // Vehicles
    inventory.push_back(BlackMarketItem("Getaway Sedan",      "Fast, inconspicuous escape vehicle.",            "vehicle", 6000, "vehicles", 1));
    inventory.push_back(BlackMarketItem("Armored SUV",        "Heavy-duty transport for high-risk ops.",        "vehicle",12000, "vehicles", 2));
    inventory.push_back(BlackMarketItem("Speed Bike",         "Fastest way through city traffic.",              "vehicle", 4000, "vehicles", 1));
    // Intel
    inventory.push_back(BlackMarketItem("Police Scanner",     "Monitor law enforcement frequencies.",           "intel",   3500, "intel", 1));
    inventory.push_back(BlackMarketItem("Insider Dossier",    "Classified intel on rival operations.",          "intel",   5000, "intel", 2));
    inventory.push_back(BlackMarketItem("City Blueprint Map", "Detailed layout of all key infrastructure.",     "intel",   4000, "intel", 1));
    // Special
    inventory.push_back(BlackMarketItem("Fake Passport Kit",  "New identity for compromised operatives.",       "special", 8000, "fakePassports", 1));
    inventory.push_back(BlackMarketItem("Bribe Package",      "Pre-assembled police corruption kit.",           "special",10000, "intel", 3));
    inventory.push_back(BlackMarketItem("Encrypted Phone",    "Secure communications for 5 turns.",             "special", 3000, "intel", 1));
}

void BlackMarket::refreshStock() {
    // Randomly make some items unavailable/available
    for (auto& item : inventory) {
        item.available = randomChance(75);
    }
}

void BlackMarket::displayShop() const {
    std::cout << std::endl;
    printDoubleBorder();
    printBoxLine(Color::BOLD + Color::BRIGHT_MAGENTA + "BLACK MARKET — Underground Exchange" + Color::RESET);
    printDoubleBorderMid();

    std::string lastCat = "";
    for (int i = 0; i < (int)inventory.size(); i++) {
        const auto& item = inventory[i];
        if (item.category != lastCat) {
            lastCat = item.category;
            std::string catLabel = lastCat;
            for (auto& c : catLabel) c = toupper(c);
            std::cout << Color::CYAN << "  ┌─ " << Color::BOLD << catLabel
                      << Color::RESET << Color::CYAN << " ─────────────────────────────────────────┐"
                      << Color::RESET << std::endl;
        }
        std::string available = item.available ? Color::BRIGHT_GREEN + "IN STOCK" : Color::RED + "SOLD OUT";
        std::cout << "  " << Color::BRIGHT_CYAN << std::setw(2) << (i + 1) << ". " << Color::RESET
                  << Color::BOLD << std::setw(22) << std::left << item.name << Color::RESET
                  << Color::GREEN << std::setw(8) << std::right << formatMoney(item.price) << Color::RESET
                  << "  +" << Color::YELLOW << item.statBonus << Color::RESET
                  << "  [" << available << Color::RESET << "]" << std::endl;
        std::cout << "      " << Color::DIM << item.description << Color::RESET << std::endl;
    }
    printDoubleBorderBottom();
}

BlackMarketItem BlackMarket::purchaseItem(int index, int& playerMoney) {
    BlackMarketItem empty;
    empty.name = "";
    if (index < 0 || index >= (int)inventory.size()) return empty;
    auto& item = inventory[index];
    if (!item.available) return empty;
    if (playerMoney < item.price) return empty;

    playerMoney -= item.price;
    item.available = false;
    return item;
}
