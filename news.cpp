// ============================================================
// news.cpp — News & Events System Implementation
// Underworld Empire Management System
// ============================================================

#include "news.h"

NewsSystem::NewsSystem() : maxHeadlines(10) {
    initializeEventPool();
}

void NewsSystem::initializeEventPool() {
    eventPool = {
        {"Informant Betrayal", "A trusted insider sold information to the police. Heat is rising.",
         "negative", 0, -5, 15, -10},
        {"Hidden Stash Found", "Your crew discovered a hidden cache of cash in an old safehouse.",
         "positive", 5000, 3, 0, 5},
        {"Corrupt Officer Deal", "A corrupt detective offers to look the other way — for a price.",
         "neutral", -3000, 0, -20, 0},
        {"Rival Ambush", "A rival gang launched a surprise attack on your convoy.",
         "negative", -2000, -5, 5, -3},
        {"Money Laundering Success", "A complex laundering scheme paid off handsomely.",
         "positive", 8000, 5, 3, 0},
        {"Warehouse Fire", "A fire destroyed one of your supply warehouses.",
         "negative", -4000, -3, 0, -5},
        {"Secret Intel Leak", "Classified police documents were leaked to your network.",
         "positive", 0, 5, -10, 5},
        {"Street Reputation Boost", "Word on the street is spreading — your crew is feared and respected.",
         "positive", 0, 10, 5, 8},
        {"Federal Investigation", "The feds are sniffing around. Stay low.",
         "negative", 0, 0, 20, -5},
        {"Lucky Break", "An opportunity fell into your lap. A wealthy target with no security.",
         "positive", 6000, 4, 2, 0},
        {"Crew Morale Boost", "After a string of successes, your crew's spirits are high.",
         "positive", 0, 2, 0, 15},
        {"Market Crash", "The underground economy took a hit. Profits are down.",
         "negative", -3000, -2, 0, -3},
        {"Police Informant Exposed", "You discovered a mole in your organization.",
         "neutral", 0, 3, -5, -15},
        {"Arms Deal Opportunity", "A weapons dealer offers surplus military hardware at a discount.",
         "positive", -1000, 2, 3, 0},
        {"Witness Intimidation Success", "Key witnesses in a case against you have gone silent.",
         "positive", 0, 4, -15, 0},
        {"Territory Dispute", "Two rival gangs are fighting — an opportunity to expand.",
         "neutral", 0, 0, 0, 0},
    };
}

void NewsSystem::generateHeadline(const std::string& context) {
    recentHeadlines.insert(recentHeadlines.begin(), context);
    if ((int)recentHeadlines.size() > maxHeadlines) {
        recentHeadlines.pop_back();
    }
}

void NewsSystem::displayLatestNews() const {
    std::cout << std::endl;
    printDoubleBorder();
    printBoxLine(Color::BOLD + Color::BRIGHT_YELLOW + "UNDERGROUND NEWS NETWORK" + Color::RESET);
    printDoubleBorderMid();

    if (recentHeadlines.empty()) {
        printBoxLine(Color::DIM + "No recent news." + Color::RESET);
    } else {
        int count = std::min(5, (int)recentHeadlines.size());
        for (int i = 0; i < count; i++) {
            std::string bullet = (i == 0) ? Color::BRIGHT_YELLOW + " >> " : Color::DIM + "    ";
            printBoxLine(bullet + recentHeadlines[i] + Color::RESET);
        }
    }
    printDoubleBorderBottom();
}

void NewsSystem::displayNewsTicker() const {
    if (recentHeadlines.empty()) return;
    std::cout << Color::DIM << "  ─── " << Color::BRIGHT_YELLOW << "NEWS: "
              << Color::RESET << Color::YELLOW << recentHeadlines[0]
              << Color::DIM << " ───" << Color::RESET << std::endl;
}

RandomEvent NewsSystem::triggerRandomEvent() {
    if (eventPool.empty()) {
        return {"Nothing", "The city is quiet tonight.", "neutral", 0, 0, 0, 0};
    }
    int idx = getRandomInt(0, (int)eventPool.size() - 1);
    RandomEvent event = eventPool[idx];
    generateHeadline(event.title + " — " + event.description);
    return event;
}

void NewsSystem::displayEvent(const RandomEvent& event) const {
    std::string color;
    std::string icon;
    if (event.type == "positive") { color = Color::BRIGHT_GREEN; icon = "✓"; }
    else if (event.type == "negative") { color = Color::BRIGHT_RED; icon = "✗"; }
    else { color = Color::BRIGHT_YELLOW; icon = "►"; }

    std::cout << std::endl;
    std::cout << color << Color::BOLD;
    std::cout << "  ╔══════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "  ║  " << icon << " EVENT: " << std::setw(44) << std::left
              << event.title.substr(0, 44) << "║" << std::endl;
    std::cout << "  ╠══════════════════════════════════════════════════════╣" << Color::RESET << color << std::endl;

    // Word-wrap description
    std::string desc = event.description;
    int lineW = 52;
    for (size_t i = 0; i < desc.size(); i += lineW) {
        std::string seg = desc.substr(i, lineW);
        int pad = lineW - (int)seg.size();
        std::cout << "  ║  " << seg << std::string(std::max(0, pad), ' ') << "║" << std::endl;
    }

    std::cout << Color::BOLD;
    std::cout << "  ╠══════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "  ║ ";
    if (event.moneyEffect != 0)
        std::cout << " Money:" << (event.moneyEffect > 0 ? "+" : "") << event.moneyEffect;
    if (event.respectEffect != 0)
        std::cout << " Rep:" << (event.respectEffect > 0 ? "+" : "") << event.respectEffect;
    if (event.heatEffect != 0)
        std::cout << " Heat:" << (event.heatEffect > 0 ? "+" : "") << event.heatEffect;
    if (event.loyaltyEffect != 0)
        std::cout << " Loyalty:" << (event.loyaltyEffect > 0 ? "+" : "") << event.loyaltyEffect;
    // Pad the effects line
    std::cout << std::string(10, ' ') << "║" << std::endl;
    std::cout << "  ╚══════════════════════════════════════════════════════╝" << Color::RESET << std::endl;
}

void NewsSystem::reportMissionResult(const std::string& missionName, bool success) {
    if (success)
        generateHeadline("Successful operation: " + missionName + " — crew escapes undetected.");
    else
        generateHeadline("Failed operation: " + missionName + " — authorities investigating.");
}

void NewsSystem::reportTerritoryChange(const std::string& territory, const std::string& newOwner) {
    if (newOwner == "player")
        generateHeadline("BREAKING: " + territory + " seized by your organization.");
    else
        generateHeadline("ALERT: " + territory + " has fallen to " + newOwner + ".");
}

void NewsSystem::reportPoliceAction(const std::string& action) {
    generateHeadline("POLICE: " + action);
}

void NewsSystem::reportRivalAction(const std::string& rivalName, const std::string& action) {
    generateHeadline(rivalName + ": " + action);
}
