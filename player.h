// ============================================================
// player.h — Player Class (Declaration)
// Underworld Empire Management System
// Core player state, progression, and empire management
// ============================================================
#pragma once

#include "utils.h"
#include "member.h"
#include "territory.h"

class Player {
private:
    std::string name;
    int money;
    int respect;            // 0-100
    int heatLevel;          // 0-100
    std::string rank;
    int turnsPlayed;

    // Inventory counts (from black market purchases)
    int weapons;
    int armor;
    int vehicles;
    int intel;
    int fakePassports;

    // Stats tracking
    int totalEarnings;
    int totalMissionsCompleted;
    int totalMemberRecruited;
    int totalTerritoriesCaptured;

public:
    Player();
    Player(const std::string& name);

    // Rank progression: Street Rat → Hustler → Enforcer → Boss → Kingpin
    void calculateRank();
    std::string getRankTitle() const;
    int getRankLevel() const;

    // Empire display
    void viewEmpire(const std::vector<GangMember*>& members,
                    const std::vector<Territory>& territories) const;
    void displayHUD(int turn, int memberCount, int territoryCount, int totalTerritories) const;

    // Money management
    void addMoney(int amount);
    bool spendMoney(int amount);
    void collectTerritoryIncome(std::vector<Territory>& territories);
    void payMemberSalaries(const std::vector<GangMember*>& members);

    // Heat management
    void addHeat(int amount);
    void reduceHeat(int amount);

    // Respect management
    void addRespect(int amount);
    void loseRespect(int amount);

    // Inventory management
    void addItem(const std::string& category, int amount);
    int getItemCount(const std::string& category) const;
    int getCombatBonus() const;
    int getEscapeBonus() const;

    // Getters
    std::string getName() const { return name; }
    int getMoney() const { return money; }
    int getRespect() const { return respect; }
    int getHeatLevel() const { return heatLevel; }
    std::string getRank() const { return rank; }
    int getTurnsPlayed() const { return turnsPlayed; }
    int getWeapons() const { return weapons; }
    int getArmor() const { return armor; }
    int getVehicles() const { return vehicles; }
    int getIntel() const { return intel; }
    int getTotalEarnings() const { return totalEarnings; }
    int getTotalMissions() const { return totalMissionsCompleted; }

    // Setters
    void setName(const std::string& n) { name = n; }
    void setMoney(int val) { money = val; }
    void setHeatLevel(int val) { heatLevel = std::max(0, std::min(100, val)); }
    void setRespect(int val) { respect = std::max(0, std::min(100, val)); }
    void incrementTurns() { turnsPlayed++; }
    void addMissionCompleted() { totalMissionsCompleted++; }
    void addTerritoryCapture() { totalTerritoriesCaptured++; }
    void addMemberRecruited() { totalMemberRecruited++; }

    // Save/Load
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
};
