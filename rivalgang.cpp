// ============================================================
// rivalgang.cpp — Rival Gang AI Implementation
// Underworld Empire Management System
// ============================================================

#include "rivalgang.h"

RivalGang::RivalGang()
    : name("Unknown"), leaderName("Unknown"), power(30), territoryCount(1),
      aggression(30), atWar(false), wealth(5000) {}

RivalGang::RivalGang(const std::string& name, const std::string& leader,
                     int power, int aggression)
    : name(name), leaderName(leader), power(power), territoryCount(1),
      aggression(aggression), atWar(false), wealth(power * 200) {}

int RivalGang::attackPlayer(int playerDefense) {
    int attackPower = power + getRandomInt(-15, 20);
    if (attackPower > playerDefense) {
        int damage = (attackPower - playerDefense) * getRandomInt(50, 150);
        return damage;  // money stolen
    }
    weakenGang(getRandomInt(3, 8));
    return 0;
}

int RivalGang::stealIncome(int playerIncome) {
    if (!randomChance(aggression / 2)) return 0;
    int stolen = playerIncome * getRandomInt(10, 30) / 100;
    wealth += stolen;
    return stolen;
}

bool RivalGang::expandTerritory(std::vector<Territory>& territories) {
    if (!randomChance(aggression / 3)) return false;

    for (auto& t : territories) {
        if (t.getOwner() == "neutral" && randomChance(30)) {
            t.setOwner(name);
            territoryCount++;
            power = std::min(100, power + 3);
            return true;
        }
    }
    return false;
}

void RivalGang::strengthenGang() {
    power = std::min(100, power + getRandomInt(1, 5));
    if (wealth > 3000) {
        wealth -= 1000;
        aggression = std::min(100, aggression + 2);
    }
}

void RivalGang::weakenGang(int amount) {
    power = std::max(5, power - amount);
    aggression = std::max(10, aggression - amount / 2);
}

void RivalGang::displayInfo() const {
    std::string threatColor;
    std::string threatLevel;
    if (power >= 75) { threatColor = Color::BRIGHT_RED + Color::BOLD; threatLevel = "EXTREME"; }
    else if (power >= 50) { threatColor = Color::RED; threatLevel = "HIGH"; }
    else if (power >= 30) { threatColor = Color::YELLOW; threatLevel = "MODERATE"; }
    else { threatColor = Color::GREEN; threatLevel = "LOW"; }

    std::cout << "  " << threatColor << "◆ " << Color::BOLD << Color::BRIGHT_WHITE
              << name << Color::RESET;
    if (atWar) std::cout << Color::BRIGHT_RED << Color::BOLD << " [AT WAR]" << Color::RESET;
    std::cout << std::endl;
    std::cout << "    Leader: " << Color::CYAN << leaderName << Color::RESET
              << "  Power: " << threatColor << power << Color::RESET
              << "  Zones: " << Color::YELLOW << territoryCount << Color::RESET
              << "  Threat: " << threatColor << threatLevel << Color::RESET << std::endl;
    printProgressBar("Aggression", aggression, 100, Color::RED, 15);
}

void RivalGang::displayThreatLevel() const {
    std::string bar;
    int filled = power / 5;
    for (int i = 0; i < 20; i++) {
        bar += (i < filled) ? "█" : "░";
    }
    std::cout << "  " << name << ": " << (power >= 50 ? Color::RED : Color::YELLOW)
              << bar << " " << power << "%" << Color::RESET << std::endl;
}

void RivalGang::saveToFile(std::ofstream& out) const {
    out << name << "|" << leaderName << "|" << power << "|" << territoryCount
        << "|" << aggression << "|" << atWar << "|" << wealth << "\n";
}

void RivalGang::loadFromFile(std::ifstream& in) {
    std::string line;
    if (!std::getline(in, line) || line.empty()) return;
    std::stringstream ss(line);
    std::string tok;
    std::vector<std::string> t;
    while (std::getline(ss, tok, '|')) t.push_back(tok);
    if (t.size() < 7) return;
    name = t[0];
    leaderName = t[1];
    power = std::stoi(t[2]);
    territoryCount = std::stoi(t[3]);
    aggression = std::stoi(t[4]);
    atWar = (t[5] == "1");
    wealth = std::stoi(t[6]);
}

// ═══════════ Initialize Rival Gangs ═══════════

std::vector<RivalGang> initializeRivalGangs() {
    std::vector<RivalGang> gangs;
    gangs.push_back(RivalGang("Iron Syndicate",   "Dmitri Volkov",    45, 55));
    gangs.push_back(RivalGang("Crimson Serpents",  "Mei Ling Chen",   35, 40));
    gangs.push_back(RivalGang("Shadow Collective", "Marcus Kane",     55, 65));
    gangs.push_back(RivalGang("Neon Cartel",       "Isabella Reyes",  40, 50));
    return gangs;
}
