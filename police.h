// ============================================================
// police.h — Police System (Declaration)
// Underworld Empire Management System
// Manages law enforcement, raids, investigations
// ============================================================
#pragma once

#include "utils.h"

class Police {
private:
    int alertLevel;         // 0-100, based on player heat
    bool investigating;     // active investigation flag
    int raidCooldown;       // turns until next possible raid
    int crackdownIntensity; // how severe the response is
    int officersCorrupted;  // number of bribed officers

public:
    Police();

    // Core mechanics
    void updateAlert(int playerHeat);
    bool attemptRaid(int playerHeat);
    bool triggerInvestigation(int playerHeat);
    int conductCrackdown();  // returns money seized
    bool attemptArrest();    // attempt to arrest a member
    void corruptOfficer();

    // Display
    void displayStatus() const;
    void displayRaidWarning() const;

    // Getters
    int getAlertLevel() const { return alertLevel; }
    bool isInvestigating() const { return investigating; }
    int getRaidCooldown() const { return raidCooldown; }
    int getOfficersCorrupted() const { return officersCorrupted; }

    // Setters
    void setAlertLevel(int val) { alertLevel = std::max(0, std::min(100, val)); }
    void setInvestigating(bool val) { investigating = val; }
    void decrementCooldown() { if (raidCooldown > 0) raidCooldown--; }

    // Save/Load
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
};
