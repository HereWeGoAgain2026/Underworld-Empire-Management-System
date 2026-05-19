// ============================================================
// game.h — Main Game Controller (Declaration)
// Underworld Empire Management System
// Orchestrates all systems, menus, and game loop
// ============================================================
#pragma once

#include "player.h"
#include "member.h"
#include "territory.h"
#include "mission.h"
#include "police.h"
#include "rivalgang.h"
#include "blackmarket.h"
#include "news.h"

class Game {
private:
    // Core systems
    Player player;
    std::vector<GangMember*> members;
    std::vector<Territory> territories;
    std::vector<RivalGang> rivals;
    Police police;
    BlackMarket market;
    NewsSystem news;

    // Game state
    bool running;
    int turn;
    std::string saveDirectory;

    // ── Startup & UI ──
    void bootSequence();
    void displayTitle();
    void displayMainMenu();
    void displayGameOver();

    // ── Menu Handlers ──
    void viewEmpire();
    void recruitMembers();
    void manageTerritories();
    void startMission();
    void rivalActivity();
    void policeNetwork();
    void blackMarketMenu();

    // ── Save/Load ──
    void saveGame();
    void loadGame();

    // ── Turn Processing ──
    void endTurn();
    void processRandomEvent();
    void processLoyaltyDecay();
    void processRivalAI();
    void processPoliceActivity();
    void collectIncome();
    void paySalaries();

    // ── Territory Sub-menu ──
    void attackTerritory();
    void upgradeTerritory();
    void viewTerritoryDetails();

    // ── Member Management ──
    void viewAllMembers();
    void dismissMember();
    void payBonusToMember();

    // ── Helpers ──
    int getTeamStrength() const;
    int getSpecialBonus(const std::string& type) const;
    int getActiveMemberCount() const;
    int getPlayerTerritoryCount() const;
    bool checkGameOver() const;

public:
    Game();
    ~Game();

    // Main entry point
    void run();
};
