// ============================================================
// rivalgang.h — Rival Gang AI System (Declaration)
// Underworld Empire Management System
// Simple AI-driven rival gangs that compete with the player
// ============================================================
#pragma once

#include "utils.h"
#include "territory.h"

// Forward declaration to avoid circular includes
class RivalGang {
private:
    std::string name;
    std::string leaderName;
    int power;              // overall gang strength 1-100
    int territoryCount;     // number of territories controlled
    int aggression;         // how likely to attack 1-100
    bool atWar;             // currently at war with player
    int wealth;             // rival's money pool

public:
    RivalGang();
    RivalGang(const std::string& name, const std::string& leader,
              int power, int aggression);

    // AI Actions
    int attackPlayer(int playerDefense);   // returns damage dealt
    int stealIncome(int playerIncome);     // returns money stolen
    bool expandTerritory(std::vector<Territory>& territories);
    void strengthenGang();
    void weakenGang(int amount);

    // Display
    void displayInfo() const;
    void displayThreatLevel() const;

    // Getters
    std::string getName() const { return name; }
    std::string getLeaderName() const { return leaderName; }
    int getPower() const { return power; }
    int getTerritoryCount() const { return territoryCount; }
    int getAggression() const { return aggression; }
    bool isAtWar() const { return atWar; }
    int getWealth() const { return wealth; }

    // Setters
    void setAtWar(bool val) { atWar = val; }
    void setPower(int val) { power = std::max(0, std::min(100, val)); }
    void setTerritoryCount(int val) { territoryCount = std::max(0, val); }
    void setAggression(int val) { aggression = std::max(0, std::min(100, val)); }

    // Save/Load
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
};

// ─────────────────────────────────────────────
//  Initialize default rival gangs
// ─────────────────────────────────────────────
std::vector<RivalGang> initializeRivalGangs();
