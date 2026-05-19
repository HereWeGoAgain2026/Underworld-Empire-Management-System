// ============================================================
// territory.cpp — Territory System Implementation
// Underworld Empire Management System
// ============================================================

#include "territory.h"

Territory::Territory()
    : name("Unknown"), income(0), dangerLevel(0), owner("neutral"),
      defenseStrength(0), active(true) {}

Territory::Territory(const std::string& name, int income, int dangerLevel,
                     const std::string& owner, int defenseStrength)
    : name(name), income(income), dangerLevel(dangerLevel),
      owner(owner), defenseStrength(defenseStrength), active(true) {}

int Territory::generateIncome() const {
    if (!active || owner != "player") return 0;
    // Income varies +/- 20%
    int variance = getRandomInt(-income/5, income/5);
    return income + variance;
}

bool Territory::defendAttack(int attackPower) {
    int defense = defenseStrength + getRandomInt(-10, 15);
    if (defense >= attackPower) {
        return true;  // defense holds
    }
    return false;  // territory lost
}

void Territory::upgradeDefense(int amount) {
    defenseStrength = std::min(100, defenseStrength + amount);
}

void Territory::displayTerritory() const {
    std::string ownerColor;
    std::string ownerLabel;
    if (owner == "player") {
        ownerColor = Color::BRIGHT_GREEN;
        ownerLabel = "YOUR TERRITORY";
    } else if (owner == "neutral") {
        ownerColor = Color::DIM;
        ownerLabel = "Neutral Zone";
    } else {
        ownerColor = Color::BRIGHT_RED;
        ownerLabel = owner;
    }

    std::cout << "  " << ownerColor << "■ " << Color::BOLD << Color::BRIGHT_WHITE
              << std::setw(16) << std::left << name << Color::RESET;
    std::cout << Color::DIM << " │ " << Color::RESET;
    std::cout << Color::GREEN << "$" << std::setw(5) << income << Color::RESET;
    std::cout << Color::DIM << " │ " << Color::RESET;
    std::cout << "Risk:" << (dangerLevel > 70 ? Color::RED : (dangerLevel > 40 ? Color::YELLOW : Color::GREEN))
              << std::setw(3) << dangerLevel << Color::RESET;
    std::cout << Color::DIM << " │ " << Color::RESET;
    std::cout << "Def:" << Color::CYAN << std::setw(3) << defenseStrength << Color::RESET;
    std::cout << Color::DIM << " │ " << Color::RESET;
    std::cout << ownerColor << ownerLabel << Color::RESET;
    if (!active) std::cout << Color::RED << " [SHUTDOWN]" << Color::RESET;
    std::cout << std::endl;
}

void Territory::saveToFile(std::ofstream& out) const {
    out << name << "|" << income << "|" << dangerLevel << "|"
        << owner << "|" << defenseStrength << "|" << active << "\n";
}

void Territory::loadFromFile(std::ifstream& in) {
    std::string line;
    if (!std::getline(in, line) || line.empty()) return;
    std::stringstream ss(line);
    std::string tok;
    std::vector<std::string> t;
    while (std::getline(ss, tok, '|')) t.push_back(tok);
    if (t.size() < 6) return;
    name = t[0];
    income = std::stoi(t[1]);
    dangerLevel = std::stoi(t[2]);
    owner = t[3];
    defenseStrength = std::stoi(t[4]);
    active = (t[5] == "1");
}

// ═══════════ Default City Territories ═══════════

std::vector<Territory> initializeDefaultTerritories() {
    std::vector<Territory> territories;
    territories.push_back(Territory("The Docks",       800,  35, "neutral", 25));
    territories.push_back(Territory("Downtown",       1500,  55, "neutral", 45));
    territories.push_back(Territory("The Slums",       500,  20, "neutral", 15));
    territories.push_back(Territory("Port District",  1200,  50, "neutral", 35));
    territories.push_back(Territory("Industrial Zone", 1000,  45, "neutral", 30));
    territories.push_back(Territory("Neon Strip",     1800,  70, "neutral", 50));
    territories.push_back(Territory("Old Town",        600,  25, "neutral", 20));
    territories.push_back(Territory("The Highrise",   2000,  80, "neutral", 60));
    return territories;
}

// ═══════════ ASCII City Map ═══════════

void displayCityMap(const std::vector<Territory>& territories) {
    auto getColor = [&](int idx) -> std::string {
        if (idx < 0 || idx >= (int)territories.size()) return Color::DIM;
        if (territories[idx].getOwner() == "player") return Color::BRIGHT_GREEN;
        if (territories[idx].getOwner() == "neutral") return Color::DIM;
        return Color::BRIGHT_RED;
    };
    auto getTag = [&](int idx) -> std::string {
        if (idx < 0 || idx >= (int)territories.size()) return "???";
        std::string n = territories[idx].getName();
        if (n.size() > 12) n = n.substr(0, 12);
        return n;
    };

    std::cout << std::endl;
    std::cout << Color::CYAN << Color::BOLD << "                    ╔══ CITY MAP ══╗" << Color::RESET << std::endl;
    std::cout << std::endl;
    // Row 1: Highrise --- Neon Strip
    std::cout << "        " << getColor(7) << "┌──────────────┐" << Color::RESET
              << "────" << getColor(5) << "┌──────────────┐" << Color::RESET << std::endl;
    std::cout << "        " << getColor(7) << "│ " << std::setw(13) << std::left << getTag(7) << "│" << Color::RESET
              << "    " << getColor(5) << "│ " << std::setw(13) << std::left << getTag(5) << "│" << Color::RESET << std::endl;
    std::cout << "        " << getColor(7) << "└──────┬───────┘" << Color::RESET
              << "────" << getColor(5) << "└──────┬───────┘" << Color::RESET << std::endl;
    std::cout << "               │                    │" << std::endl;
    // Row 2: Downtown --- Industrial
    std::cout << "        " << getColor(1) << "┌──────┴───────┐" << Color::RESET
              << "────" << getColor(4) << "┌──────┴───────┐" << Color::RESET << std::endl;
    std::cout << "        " << getColor(1) << "│ " << std::setw(13) << std::left << getTag(1) << "│" << Color::RESET
              << "    " << getColor(4) << "│ " << std::setw(13) << std::left << getTag(4) << "│" << Color::RESET << std::endl;
    std::cout << "        " << getColor(1) << "└──────┬───────┘" << Color::RESET
              << "────" << getColor(4) << "└──────┬───────┘" << Color::RESET << std::endl;
    std::cout << "               │                    │" << std::endl;
    // Row 3: Slums --- Port
    std::cout << "        " << getColor(2) << "┌──────┴───────┐" << Color::RESET
              << "────" << getColor(3) << "┌──────┴───────┐" << Color::RESET << std::endl;
    std::cout << "        " << getColor(2) << "│ " << std::setw(13) << std::left << getTag(2) << "│" << Color::RESET
              << "    " << getColor(3) << "│ " << std::setw(13) << std::left << getTag(3) << "│" << Color::RESET << std::endl;
    std::cout << "        " << getColor(2) << "└──────┬───────┘" << Color::RESET
              << "────" << getColor(3) << "└──────┬───────┘" << Color::RESET << std::endl;
    std::cout << "               │                    │" << std::endl;
    // Row 4: Old Town --- Docks
    std::cout << "        " << getColor(6) << "┌──────┴───────┐" << Color::RESET
              << "────" << getColor(0) << "┌──────┴───────┐" << Color::RESET << std::endl;
    std::cout << "        " << getColor(6) << "│ " << std::setw(13) << std::left << getTag(6) << "│" << Color::RESET
              << "    " << getColor(0) << "│ " << std::setw(13) << std::left << getTag(0) << "│" << Color::RESET << std::endl;
    std::cout << "        " << getColor(6) << "└──────────────┘" << Color::RESET
              << "────" << getColor(0) << "└──────────────┘" << Color::RESET << std::endl;
    std::cout << std::endl;
    std::cout << "    " << Color::BRIGHT_GREEN << "■ Yours" << Color::RESET << "   "
              << Color::DIM << "■ Neutral" << Color::RESET << "   "
              << Color::BRIGHT_RED << "■ Rival" << Color::RESET << std::endl;
}
