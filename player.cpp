// ============================================================
// player.cpp — Player Class Implementation
// Underworld Empire Management System
// ============================================================

#include "player.h"

Player::Player()
    : name("Unknown"), money(5000), respect(10), heatLevel(5),
      rank("Street Rat"), turnsPlayed(0),
      weapons(0), armor(0), vehicles(0), intel(0), fakePassports(0),
      totalEarnings(0), totalMissionsCompleted(0),
      totalMemberRecruited(0), totalTerritoriesCaptured(0) {}

Player::Player(const std::string& name)
    : name(name), money(5000), respect(10), heatLevel(5),
      rank("Street Rat"), turnsPlayed(0),
      weapons(0), armor(0), vehicles(0), intel(0), fakePassports(0),
      totalEarnings(0), totalMissionsCompleted(0),
      totalMemberRecruited(0), totalTerritoriesCaptured(0) {}

// ─────────────── Rank Progression ───────────────

void Player::calculateRank() {
    // Street Rat -> Hustler -> Enforcer -> Boss -> Kingpin
    int score = respect + (totalMissionsCompleted * 3) +
                (totalTerritoriesCaptured * 5) + (totalMemberRecruited * 2);
    if (score >= 200)     rank = "Kingpin";
    else if (score >= 120) rank = "Boss";
    else if (score >= 60)  rank = "Enforcer";
    else if (score >= 25)  rank = "Hustler";
    else                   rank = "Street Rat";
}

std::string Player::getRankTitle() const { return rank; }

int Player::getRankLevel() const {
    if (rank == "Kingpin")   return 5;
    if (rank == "Boss")      return 4;
    if (rank == "Enforcer")  return 3;
    if (rank == "Hustler")   return 2;
    return 1;
}

// ─────────────── Empire View ───────────────

void Player::viewEmpire(const std::vector<GangMember*>& members,
                        const std::vector<Territory>& territories) const {
    clearScreen();
    std::cout << std::endl;
    printDoubleBorder();
    printBoxLine(Color::BOLD + Color::BRIGHT_CYAN + "EMPIRE INTELLIGENCE REPORT" + Color::RESET);
    printDoubleBorderMid();
    printBoxLine(Color::BRIGHT_WHITE + "Operator: " + Color::BOLD + name + Color::RESET);
    printBoxLine(Color::BRIGHT_WHITE + "Rank: " + getRankColor(rank) + rank + Color::RESET);
    printDoubleBorderBottom();

    std::cout << std::endl;
    printSectionHeader("FINANCIAL STATUS");
    std::cout << "  " << Color::BRIGHT_GREEN << "  Cash:          " << Color::BOLD
              << formatMoney(money) << Color::RESET << std::endl;
    std::cout << "  " << Color::DIM << "  Total Earned:  " << Color::RESET
              << formatMoney(totalEarnings) << std::endl;
    printSectionFooter();

    printSectionHeader("EMPIRE METRICS");
    printProgressBar("Respect", respect, 100, Color::BRIGHT_GREEN);
    printProgressBar("Heat", heatLevel, 100, getHeatColor(heatLevel));
    std::cout << std::endl;
    std::cout << "  " << Color::DIM << "  Active Members:     " << Color::RESET;
    int active = 0;
    for (auto* m : members) if (m->isAlive() && !m->isArrested()) active++;
    std::cout << Color::CYAN << active << Color::RESET << "/" << members.size() << std::endl;

    int ownedT = 0;
    for (const auto& t : territories) if (t.getOwner() == "player") ownedT++;
    std::cout << "  " << Color::DIM << "  Territories:        " << Color::RESET
              << Color::CYAN << ownedT << Color::RESET << "/" << territories.size() << std::endl;
    std::cout << "  " << Color::DIM << "  Missions Completed: " << Color::RESET
              << Color::CYAN << totalMissionsCompleted << Color::RESET << std::endl;
    std::cout << "  " << Color::DIM << "  Turns Played:       " << Color::RESET
              << Color::CYAN << turnsPlayed << Color::RESET << std::endl;
    printSectionFooter();

    printSectionHeader("INVENTORY");
    std::cout << "  " << Color::RED << "  Weapons:    " << Color::BOLD << weapons << Color::RESET
              << Color::DIM << "  (+combat)" << Color::RESET << std::endl;
    std::cout << "  " << Color::BLUE << "  Armor:      " << Color::BOLD << armor << Color::RESET
              << Color::DIM << "  (+defense)" << Color::RESET << std::endl;
    std::cout << "  " << Color::GREEN << "  Vehicles:   " << Color::BOLD << vehicles << Color::RESET
              << Color::DIM << "  (+escape)" << Color::RESET << std::endl;
    std::cout << "  " << Color::YELLOW << "  Intel:      " << Color::BOLD << intel << Color::RESET
              << Color::DIM << "  (+planning)" << Color::RESET << std::endl;
    std::cout << "  " << Color::MAGENTA << "  Passports:  " << Color::BOLD << fakePassports << Color::RESET
              << Color::DIM << "  (+safety)" << Color::RESET << std::endl;
    printSectionFooter();
}

void Player::displayHUD(int turn, int memberCount, int territoryCount, int totalTerritories) const {
    printDoubleBorder();
    std::string turnStr = "Turn: " + std::to_string(turn);
    printBoxLine(Color::BOLD + Color::BRIGHT_CYAN + "UNDERWORLD EMPIRE CONTROL" + Color::RESET
                 + std::string(std::max(1, 28 - (int)turnStr.size()), ' ')
                 + Color::DIM + turnStr + Color::RESET);
    printDoubleBorderMid();
    printBoxLine(Color::BRIGHT_WHITE + "  " + name + Color::RESET
                 + "  " + getRankColor(rank) + "[" + rank + "]" + Color::RESET
                 + "  " + Color::GREEN + formatMoney(money) + Color::RESET
                 + "  Rep:" + Color::BRIGHT_GREEN + std::to_string(respect) + Color::RESET
                 + "  Heat:" + getHeatColor(heatLevel) + std::to_string(heatLevel) + Color::RESET);
    printBoxLine(Color::DIM + "  Members: " + Color::RESET + Color::CYAN + std::to_string(memberCount) + Color::RESET
                 + Color::DIM + "  Territories: " + Color::RESET + Color::CYAN
                 + std::to_string(territoryCount) + "/" + std::to_string(totalTerritories) + Color::RESET);
    printDoubleBorderBottom();
}

// ─────────────── Money ───────────────

void Player::addMoney(int amount) {
    money += amount;
    if (amount > 0) totalEarnings += amount;
}

bool Player::spendMoney(int amount) {
    if (money >= amount) { money -= amount; return true; }
    return false;
}

void Player::collectTerritoryIncome(std::vector<Territory>& territories) {
    int total = 0;
    for (auto& t : territories) {
        int inc = t.generateIncome();
        total += inc;
    }
    if (total > 0) {
        addMoney(total);
        printSuccess("Territory income collected: " + formatMoney(total));
    }
}

void Player::payMemberSalaries(const std::vector<GangMember*>& members) {
    int total = 0;
    for (auto* m : members) {
        if (m->isAlive() && !m->isArrested()) total += m->getSalary();
    }
    if (total > 0) {
        if (money >= total) {
            money -= total;
            printInfo("Salaries paid: " + formatMoney(total));
        } else {
            int deficit = total - money;
            money = 0;
            printWarning("Insufficient funds! Deficit: " + formatMoney(deficit));
            printWarning("Crew morale is dropping!");
        }
    }
}

// ─────────────── Heat / Respect ───────────────

void Player::addHeat(int amount) { heatLevel = std::min(100, heatLevel + amount); }
void Player::reduceHeat(int amount) { heatLevel = std::max(0, heatLevel - amount); }
void Player::addRespect(int amount) { respect = std::min(100, respect + amount); }
void Player::loseRespect(int amount) { respect = std::max(0, respect - amount); }

// ─────────────── Inventory ───────────────

void Player::addItem(const std::string& category, int amount) {
    if (category == "weapons") weapons += amount;
    else if (category == "armor") armor += amount;
    else if (category == "vehicles") vehicles += amount;
    else if (category == "intel") intel += amount;
    else if (category == "fakePassports") fakePassports += amount;
}

int Player::getItemCount(const std::string& category) const {
    if (category == "weapons") return weapons;
    if (category == "armor") return armor;
    if (category == "vehicles") return vehicles;
    if (category == "intel") return intel;
    if (category == "fakePassports") return fakePassports;
    return 0;
}

int Player::getCombatBonus() const {
    return weapons * 8 + armor * 5 + intel * 3;
}

int Player::getEscapeBonus() const {
    return vehicles * 10 + fakePassports * 5;
}

// ─────────────── Save / Load ───────────────

void Player::saveToFile(std::ofstream& out) const {
    out << name << "\n" << money << "\n" << respect << "\n" << heatLevel << "\n"
        << rank << "\n" << turnsPlayed << "\n"
        << weapons << "\n" << armor << "\n" << vehicles << "\n"
        << intel << "\n" << fakePassports << "\n"
        << totalEarnings << "\n" << totalMissionsCompleted << "\n"
        << totalMemberRecruited << "\n" << totalTerritoriesCaptured << "\n";
}

void Player::loadFromFile(std::ifstream& in) {
    std::getline(in, name);
    std::string line;
    auto readInt = [&]() -> int {
        std::getline(in, line);
        return line.empty() ? 0 : std::stoi(line);
    };
    money = readInt();
    respect = readInt();
    heatLevel = readInt();
    std::getline(in, rank);
    turnsPlayed = readInt();
    weapons = readInt();
    armor = readInt();
    vehicles = readInt();
    intel = readInt();
    fakePassports = readInt();
    totalEarnings = readInt();
    totalMissionsCompleted = readInt();
    totalMemberRecruited = readInt();
    totalTerritoriesCaptured = readInt();
}
