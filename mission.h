// ============================================================
// mission.h — Mission System (Declaration)
// Underworld Empire Management System
// Demonstrates: Polymorphism with mission types
// ============================================================
#pragma once

#include "utils.h"

// ─────────────────────────────────────────────
//  Mission Result Structure
// ─────────────────────────────────────────────
struct MissionResult {
    bool success;
    int moneyChange;
    int respectChange;
    int heatChange;
    int membersLost;
    int membersArrested;
    std::string narrative;  // story text for the result
};

// ─────────────────────────────────────────────
//  Base Class: Mission (Abstract)
// ─────────────────────────────────────────────
class Mission {
protected:
    std::string name;
    std::string description;
    std::string type;
    int difficulty;         // 1-100
    int moneyReward;
    int respectReward;
    int heatCost;           // heat increase on execution
    int requiredStrength;   // minimum team strength needed

public:
    Mission(const std::string& name, const std::string& desc, const std::string& type,
            int difficulty, int moneyReward, int respectReward, int heatCost, int reqStrength);
    virtual ~Mission() = default;

    // Pure virtual — each mission type resolves differently
    virtual MissionResult execute(int teamStrength, int specialBonus,
                                  int heatLevel, int memberCount) = 0;
    virtual void displayMission() const = 0;

    // Getters
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    int getDifficulty() const { return difficulty; }
    int getMoneyReward() const { return moneyReward; }
    int getRespectReward() const { return respectReward; }
    int getHeatCost() const { return heatCost; }
    int getRequiredStrength() const { return requiredStrength; }
};

// ─────────────────────────────────────────────
//  Derived: Robbery Mission
// ─────────────────────────────────────────────
class RobberyMission : public Mission {
public:
    RobberyMission(const std::string& name, int difficulty, int reward);
    MissionResult execute(int teamStrength, int specialBonus,
                          int heatLevel, int memberCount) override;
    void displayMission() const override;
};

// ─────────────────────────────────────────────
//  Derived: Smuggling Mission
// ─────────────────────────────────────────────
class SmugglingMission : public Mission {
public:
    SmugglingMission(const std::string& name, int difficulty, int reward);
    MissionResult execute(int teamStrength, int specialBonus,
                          int heatLevel, int memberCount) override;
    void displayMission() const override;
};

// ─────────────────────────────────────────────
//  Derived: Hacking Mission
// ─────────────────────────────────────────────
class HackingMission : public Mission {
public:
    HackingMission(const std::string& name, int difficulty, int reward);
    MissionResult execute(int teamStrength, int specialBonus,
                          int heatLevel, int memberCount) override;
    void displayMission() const override;
};

// ─────────────────────────────────────────────
//  Derived: Street Racing Mission
// ─────────────────────────────────────────────
class RacingMission : public Mission {
public:
    RacingMission(const std::string& name, int difficulty, int reward);
    MissionResult execute(int teamStrength, int specialBonus,
                          int heatLevel, int memberCount) override;
    void displayMission() const override;
};

// ─────────────────────────────────────────────
//  Derived: Bank Heist Mission (high risk/reward)
// ─────────────────────────────────────────────
class HeistMission : public Mission {
public:
    HeistMission(const std::string& name, int difficulty, int reward);
    MissionResult execute(int teamStrength, int specialBonus,
                          int heatLevel, int memberCount) override;
    void displayMission() const override;
};

// ─────────────────────────────────────────────
//  Mission Generator
// ─────────────────────────────────────────────
std::vector<Mission*> generateAvailableMissions(int playerRank);
void cleanupMissions(std::vector<Mission*>& missions);
