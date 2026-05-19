// ============================================================
// blackmarket.h — Black Market System (Declaration)
// Underworld Empire Management System
// Underground shop for weapons, gear, and intel
// ============================================================
#pragma once

#include "utils.h"

// ─────────────────────────────────────────────
//  Black Market Item
// ─────────────────────────────────────────────
struct BlackMarketItem {
    std::string name;
    std::string description;
    std::string category;       // weapon, armor, vehicle, intel, special
    int price;
    std::string statAffected;   // which player stat it boosts
    int statBonus;
    bool available;

    BlackMarketItem() : price(0), statBonus(0), available(true) {}
    BlackMarketItem(const std::string& n, const std::string& d, const std::string& cat,
                    int p, const std::string& stat, int bonus)
        : name(n), description(d), category(cat), price(p),
          statAffected(stat), statBonus(bonus), available(true) {}
};

// ─────────────────────────────────────────────
//  Black Market Shop
// ─────────────────────────────────────────────
class BlackMarket {
private:
    std::vector<BlackMarketItem> inventory;
    int refreshCooldown;

public:
    BlackMarket();

    // Initialize stock
    void initializeStock();
    void refreshStock();

    // Shop operations
    void displayShop() const;
    BlackMarketItem purchaseItem(int index, int& playerMoney);
    int getItemCount() const { return static_cast<int>(inventory.size()); }

    // Getters
    const std::vector<BlackMarketItem>& getInventory() const { return inventory; }
};
