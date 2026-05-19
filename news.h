// ============================================================
// news.h — News & Events System (Declaration)
// Underworld Empire Management System
// Generates immersive news headlines and random city events
// ============================================================
#pragma once

#include "utils.h"

// ─────────────────────────────────────────────
//  Random Event Structure
// ─────────────────────────────────────────────
struct RandomEvent {
    std::string title;
    std::string description;
    std::string type;       // positive, negative, neutral
    int moneyEffect;
    int respectEffect;
    int heatEffect;
    int loyaltyEffect;      // affects all members
};

// ─────────────────────────────────────────────
//  News & Events System
// ─────────────────────────────────────────────
class NewsSystem {
private:
    std::vector<std::string> recentHeadlines;
    std::vector<RandomEvent> eventPool;
    int maxHeadlines;

    void initializeEventPool();

public:
    NewsSystem();

    // Generate and display
    void generateHeadline(const std::string& context);
    void displayLatestNews() const;
    void displayNewsTicker() const;

    // Random events
    RandomEvent triggerRandomEvent();
    void displayEvent(const RandomEvent& event) const;

    // Contextual headlines based on game actions
    void reportMissionResult(const std::string& missionName, bool success);
    void reportTerritoryChange(const std::string& territory, const std::string& newOwner);
    void reportPoliceAction(const std::string& action);
    void reportRivalAction(const std::string& rivalName, const std::string& action);
};
