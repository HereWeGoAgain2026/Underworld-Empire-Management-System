// ============================================================
// territory.h — Territory System (Declaration)
// Underworld Empire Management System
// Manages city zones, income, and territorial control
// ============================================================
#pragma once

#include "utils.h"

class Territory {
private:
    std::string name;
    int income;             // money generated per turn
    int dangerLevel;        // 1-100, affects risk
    std::string owner;      // "player", rival gang name, or "neutral"
    int defenseStrength;    // defense capability 1-100
    bool active;            // territory operational status

public:
    Territory();
    Territory(const std::string& name, int income, int dangerLevel,
              const std::string& owner, int defenseStrength);

    // Core functions
    int generateIncome() const;
    bool defendAttack(int attackPower);
    void displayTerritory() const;
    void upgradeDefense(int amount);

    // Getters
    std::string getName() const { return name; }
    int getIncome() const { return income; }
    int getDangerLevel() const { return dangerLevel; }
    std::string getOwner() const { return owner; }
    int getDefenseStrength() const { return defenseStrength; }
    bool isActive() const { return active; }

    // Setters
    void setOwner(const std::string& newOwner) { owner = newOwner; }
    void setActive(bool val) { active = val; }
    void setDefenseStrength(int val) { defenseStrength = std::max(0, std::min(100, val)); }
    void setIncome(int val) { income = std::max(0, val); }

    // Save/Load
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
};

// ─────────────────────────────────────────────
//  Initialize default city territories
// ─────────────────────────────────────────────
std::vector<Territory> initializeDefaultTerritories();

// ─────────────────────────────────────────────
//  Display ASCII City Map
// ─────────────────────────────────────────────
void displayCityMap(const std::vector<Territory>& territories);
