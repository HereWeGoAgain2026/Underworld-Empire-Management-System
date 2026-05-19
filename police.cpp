// ============================================================
// police.cpp — Police System Implementation
// Underworld Empire Management System
// ============================================================

#include "police.h"

Police::Police()
    : alertLevel(0), investigating(false), raidCooldown(0),
      crackdownIntensity(1), officersCorrupted(0) {}

void Police::updateAlert(int playerHeat) {
    alertLevel = playerHeat;
    if (playerHeat > 80) crackdownIntensity = 3;
    else if (playerHeat > 50) crackdownIntensity = 2;
    else crackdownIntensity = 1;
}

bool Police::attemptRaid(int playerHeat) {
    if (raidCooldown > 0) return false;

    int raidChance = playerHeat / 2 - (officersCorrupted * 8);
    if (raidChance < 5) raidChance = 5;

    if (randomChance(raidChance)) {
        raidCooldown = getRandomInt(2, 5);
        return true;
    }
    return false;
}

bool Police::triggerInvestigation(int playerHeat) {
    if (investigating) return false;
    int chance = playerHeat / 3;
    if (randomChance(chance)) {
        investigating = true;
        return true;
    }
    return false;
}

int Police::conductCrackdown() {
    int seized = getRandomInt(500, 3000) * crackdownIntensity;
    if (officersCorrupted > 0) {
        seized = seized * (100 - officersCorrupted * 15) / 100;
        if (seized < 0) seized = 0;
    }
    investigating = false;
    raidCooldown = getRandomInt(3, 6);
    return seized;
}

bool Police::attemptArrest() {
    int chance = 20 + (crackdownIntensity * 10) - (officersCorrupted * 5);
    if (chance < 5) chance = 5;
    return randomChance(chance);
}

void Police::corruptOfficer() {
    if (officersCorrupted < 5) {
        officersCorrupted++;
    }
}

void Police::displayStatus() const {
    printSectionHeader("POLICE NETWORK STATUS");

    std::string threatLevel;
    std::string threatColor;
    if (alertLevel >= 80) { threatLevel = "CRITICAL"; threatColor = Color::BRIGHT_RED + Color::BOLD; }
    else if (alertLevel >= 60) { threatLevel = "HIGH"; threatColor = Color::RED; }
    else if (alertLevel >= 40) { threatLevel = "ELEVATED"; threatColor = Color::YELLOW; }
    else if (alertLevel >= 20) { threatLevel = "GUARDED"; threatColor = Color::GREEN; }
    else { threatLevel = "LOW"; threatColor = Color::BRIGHT_GREEN; }

    std::cout << std::endl;
    printProgressBar("Alert Level", alertLevel, 100, getHeatColor(alertLevel));
    std::cout << "  " << Color::DIM << "Threat Status: " << Color::RESET
              << threatColor << threatLevel << Color::RESET << std::endl;
    std::cout << "  " << Color::DIM << "Investigation: " << Color::RESET
              << (investigating ? Color::RED + "ACTIVE" : Color::GREEN + "None") << Color::RESET << std::endl;
    std::cout << "  " << Color::DIM << "Raid Cooldown: " << Color::RESET
              << Color::CYAN << raidCooldown << " turns" << Color::RESET << std::endl;
    std::cout << "  " << Color::DIM << "Corrupted Officers: " << Color::RESET
              << Color::YELLOW << officersCorrupted << "/5" << Color::RESET << std::endl;
    std::cout << "  " << Color::DIM << "Response Level: " << Color::RESET
              << threatColor << "Tier " << crackdownIntensity << Color::RESET << std::endl;

    printSectionFooter();
}

void Police::displayRaidWarning() const {
    printAlert("! POLICE RAID INCOMING !",
               "Law enforcement is mobilizing for a raid on your operations. "
               "Brace for potential losses.",
               Color::RED);
}

void Police::saveToFile(std::ofstream& out) const {
    out << alertLevel << "|" << investigating << "|" << raidCooldown
        << "|" << crackdownIntensity << "|" << officersCorrupted << "\n";
}

void Police::loadFromFile(std::ifstream& in) {
    std::string line;
    if (!std::getline(in, line) || line.empty()) return;
    std::stringstream ss(line);
    std::string tok;
    std::vector<std::string> t;
    while (std::getline(ss, tok, '|')) t.push_back(tok);
    if (t.size() < 5) return;
    alertLevel = std::stoi(t[0]);
    investigating = (t[1] == "1");
    raidCooldown = std::stoi(t[2]);
    crackdownIntensity = std::stoi(t[3]);
    officersCorrupted = std::stoi(t[4]);
}
