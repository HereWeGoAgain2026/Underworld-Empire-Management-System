// ============================================================
// mission.cpp — Mission System Implementation
// Underworld Empire Management System
// ============================================================

#include "mission.h"

// ═══════════ Base Mission ═══════════

Mission::Mission(const std::string& name, const std::string& desc, const std::string& type,
                 int difficulty, int moneyReward, int respectReward, int heatCost, int reqStrength)
    : name(name), description(desc), type(type), difficulty(difficulty),
      moneyReward(moneyReward), respectReward(respectReward),
      heatCost(heatCost), requiredStrength(reqStrength) {}

// ═══════════ Robbery Mission ═══════════

RobberyMission::RobberyMission(const std::string& name, int difficulty, int reward)
    : Mission(name, "Armed robbery operation requiring brute force and precision.",
              "Robbery", difficulty, reward, difficulty/5, difficulty/4, difficulty/2) {}

MissionResult RobberyMission::execute(int teamStr, int bonus, int heat, int members) {
    MissionResult r = {false, 0, 0, 0, 0, 0, ""};
    int power = teamStr + bonus + getRandomInt(-15, 20);
    int threshold = difficulty + (heat / 5);
    r.heatChange = heatCost + getRandomInt(2, 8);

    if (power >= threshold) {
        r.success = true;
        int earned = moneyReward + getRandomInt(-moneyReward/5, moneyReward/4);
        r.moneyChange = earned;
        r.respectChange = respectReward + getRandomInt(1, 5);
        r.narrative = "The crew moved in with surgical precision. The vault was cracked, "
                      "and " + formatMoney(earned) + " was secured. Clean exit.";
        if (randomChance(15)) { r.membersLost = 1; r.narrative += " One operative sustained critical injuries."; }
    } else {
        r.success = false;
        int lost = getRandomInt(500, moneyReward/3);
        r.moneyChange = -lost;
        r.respectChange = -getRandomInt(2, 6);
        r.heatChange += 5;
        r.narrative = "The operation went sideways. Security was tighter than expected. Lost " + formatMoney(lost) + " in equipment.";
        if (randomChance(25)) { r.membersArrested = 1; r.narrative += " One member was apprehended."; }
        if (randomChance(10)) { r.membersLost = 1; r.narrative += " Casualties confirmed."; }
    }
    return r;
}

void RobberyMission::displayMission() const {
    std::cout << Color::RED << "  [ROBBERY] " << Color::BOLD << Color::BRIGHT_WHITE << name << Color::RESET << std::endl;
    std::cout << Color::DIM << "    " << description << Color::RESET << std::endl;
    std::cout << "    Diff: " << Color::YELLOW << difficulty << Color::RESET
              << "  Reward: " << Color::GREEN << formatMoney(moneyReward) << Color::RESET
              << "  Heat: " << Color::RED << "+" << heatCost << Color::RESET
              << "  MinStr: " << Color::CYAN << requiredStrength << Color::RESET << std::endl;
}

// ═══════════ Smuggling Mission ═══════════

SmugglingMission::SmugglingMission(const std::string& name, int difficulty, int reward)
    : Mission(name, "Transport contraband across city borders undetected.",
              "Smuggling", difficulty, reward, difficulty/6, difficulty/5, difficulty/3) {}

MissionResult SmugglingMission::execute(int teamStr, int bonus, int heat, int members) {
    MissionResult r = {false, 0, 0, 0, 0, 0, ""};
    int power = teamStr + bonus * 2 + getRandomInt(-10, 25);
    int threshold = difficulty + (heat / 4);
    r.heatChange = heatCost + getRandomInt(1, 5);

    if (power >= threshold) {
        r.success = true;
        int earned = moneyReward + (bonus * 20) + getRandomInt(0, moneyReward/3);
        r.moneyChange = earned;
        r.respectChange = respectReward + getRandomInt(2, 4);
        r.narrative = "The shipment arrived safely through the back channels. " + formatMoney(earned) + " secured from the deal.";
    } else {
        r.success = false;
        int lost = getRandomInt(300, moneyReward/4);
        r.moneyChange = -lost;
        r.respectChange = -getRandomInt(1, 4);
        r.heatChange += 8;
        r.narrative = "Border patrol intercepted the shipment. " + formatMoney(lost) + " in contraband was seized.";
        if (randomChance(30)) { r.membersArrested = 1; r.narrative += " Driver was detained."; }
    }
    return r;
}

void SmugglingMission::displayMission() const {
    std::cout << Color::MAGENTA << "  [SMUGGLE] " << Color::BOLD << Color::BRIGHT_WHITE << name << Color::RESET << std::endl;
    std::cout << Color::DIM << "    " << description << Color::RESET << std::endl;
    std::cout << "    Diff: " << Color::YELLOW << difficulty << Color::RESET
              << "  Reward: " << Color::GREEN << formatMoney(moneyReward) << Color::RESET
              << "  Heat: " << Color::RED << "+" << heatCost << Color::RESET
              << "  MinStr: " << Color::CYAN << requiredStrength << Color::RESET << std::endl;
}

// ═══════════ Hacking Mission ═══════════

HackingMission::HackingMission(const std::string& name, int difficulty, int reward)
    : Mission(name, "Digital infiltration of high-value financial systems.",
              "Hacking", difficulty, reward, difficulty/4, difficulty/8, difficulty/4) {}

MissionResult HackingMission::execute(int teamStr, int bonus, int heat, int members) {
    MissionResult r = {false, 0, 0, 0, 0, 0, ""};
    int power = (teamStr / 2) + bonus * 3 + getRandomInt(-5, 30);
    int threshold = difficulty;
    r.heatChange = heatCost;

    if (power >= threshold) {
        r.success = true;
        int earned = moneyReward + getRandomInt(0, moneyReward/2);
        r.moneyChange = earned;
        r.respectChange = respectReward + getRandomInt(3, 8);
        r.heatChange -= getRandomInt(2, 5);
        r.narrative = "Firewalls bypassed. " + formatMoney(earned) + " siphoned through encrypted channels. Tracks covered.";
    } else {
        r.success = false;
        r.moneyChange = -getRandomInt(100, 500);
        r.respectChange = -getRandomInt(1, 3);
        r.heatChange += 3;
        r.narrative = "Intrusion detected. The target's security traced our signal before extraction.";
    }
    return r;
}

void HackingMission::displayMission() const {
    std::cout << Color::CYAN << "  [HACK]    " << Color::BOLD << Color::BRIGHT_WHITE << name << Color::RESET << std::endl;
    std::cout << Color::DIM << "    " << description << Color::RESET << std::endl;
    std::cout << "    Diff: " << Color::YELLOW << difficulty << Color::RESET
              << "  Reward: " << Color::GREEN << formatMoney(moneyReward) << Color::RESET
              << "  Heat: " << Color::RED << "+" << heatCost << Color::RESET
              << "  MinStr: " << Color::CYAN << requiredStrength << Color::RESET << std::endl;
}

// ═══════════ Racing Mission ═══════════

RacingMission::RacingMission(const std::string& name, int difficulty, int reward)
    : Mission(name, "Illegal street race through the city's underbelly.",
              "Racing", difficulty, reward, difficulty/3, difficulty/6, difficulty/4) {}

MissionResult RacingMission::execute(int teamStr, int bonus, int heat, int members) {
    MissionResult r = {false, 0, 0, 0, 0, 0, ""};
    int power = (teamStr / 3) + bonus * 2 + getRandomInt(-10, 35);
    int threshold = difficulty - 5;
    r.heatChange = heatCost + getRandomInt(2, 6);

    if (power >= threshold) {
        r.success = true;
        int earned = moneyReward + getRandomInt(0, moneyReward/4);
        r.moneyChange = earned;
        r.respectChange = respectReward + getRandomInt(4, 10);
        r.narrative = "Crossed the finish line in first. The crowd went wild. " + formatMoney(earned) + " in prize money.";
    } else {
        r.success = false;
        int lost = getRandomInt(200, 800);
        r.moneyChange = -lost;
        r.respectChange = -getRandomInt(2, 5);
        r.narrative = "Lost control on the final stretch. Vehicle damaged. " + formatMoney(lost) + " in repairs.";
        if (randomChance(15)) { r.membersLost = 1; r.narrative += " Critical crash — one operative down."; }
    }
    return r;
}

void RacingMission::displayMission() const {
    std::cout << Color::GREEN << "  [RACE]    " << Color::BOLD << Color::BRIGHT_WHITE << name << Color::RESET << std::endl;
    std::cout << Color::DIM << "    " << description << Color::RESET << std::endl;
    std::cout << "    Diff: " << Color::YELLOW << difficulty << Color::RESET
              << "  Reward: " << Color::GREEN << formatMoney(moneyReward) << Color::RESET
              << "  Heat: " << Color::RED << "+" << heatCost << Color::RESET
              << "  MinStr: " << Color::CYAN << requiredStrength << Color::RESET << std::endl;
}

// ═══════════ Heist Mission ═══════════

HeistMission::HeistMission(const std::string& name, int difficulty, int reward)
    : Mission(name, "High-stakes bank infiltration. Maximum risk, maximum reward.",
              "Heist", difficulty, reward, difficulty/3, difficulty/3, difficulty*2/3) {}

MissionResult HeistMission::execute(int teamStr, int bonus, int heat, int members) {
    MissionResult r = {false, 0, 0, 0, 0, 0, ""};
    int power = teamStr + bonus + getRandomInt(-20, 15);
    int threshold = difficulty + (heat / 3) + 10;
    r.heatChange = heatCost + getRandomInt(5, 15);

    if (members < 3) {
        r.success = false;
        r.narrative = "Not enough crew for a heist of this magnitude. The operation was aborted.";
        return r;
    }

    if (power >= threshold) {
        r.success = true;
        int earned = moneyReward + getRandomInt(moneyReward/4, moneyReward/2);
        r.moneyChange = earned;
        r.respectChange = respectReward + getRandomInt(5, 15);
        r.narrative = "The heist was a masterpiece. " + formatMoney(earned) + " extracted from the vault. "
                      "The city will remember this for years.";
        if (randomChance(20)) { r.membersLost = 1; r.narrative += " One operative didn't make it out."; }
    } else {
        r.success = false;
        int lost = getRandomInt(1000, moneyReward/2);
        r.moneyChange = -lost;
        r.respectChange = -getRandomInt(5, 12);
        r.heatChange += 10;
        r.narrative = "The alarms triggered. SWAT response was immediate. " + formatMoney(lost) + " lost in the chaos.";
        r.membersArrested = getRandomInt(1, std::min(2, members));
        if (randomChance(20)) r.membersLost = 1;
        r.narrative += " " + std::to_string(r.membersArrested) + " members arrested.";
    }
    return r;
}

void HeistMission::displayMission() const {
    std::cout << Color::BRIGHT_YELLOW << "  [HEIST]   " << Color::BOLD << Color::BRIGHT_WHITE << name << Color::RESET << std::endl;
    std::cout << Color::DIM << "    " << description << Color::RESET << std::endl;
    std::cout << "    Diff: " << Color::RED << Color::BOLD << difficulty << Color::RESET
              << "  Reward: " << Color::GREEN << Color::BOLD << formatMoney(moneyReward) << Color::RESET
              << "  Heat: " << Color::RED << "+" << heatCost << Color::RESET
              << "  MinStr: " << Color::CYAN << requiredStrength << Color::RESET << std::endl;
}

// ═══════════ Mission Generator ═══════════

static const std::vector<std::string> robberyNames = {
    "Jewelry Store Hit", "Casino Vault Break", "Armored Truck Ambush",
    "Warehouse Raid", "Pawn Shop Takedown", "Art Gallery Heist"
};
static const std::vector<std::string> smuggleNames = {
    "Cross-Border Cargo Run", "Midnight Shipment", "Harbor Contraband Drop",
    "Tunnel Network Transfer", "Airport Cargo Swap", "Submarine Delivery"
};
static const std::vector<std::string> hackNames = {
    "Bank Server Breach", "Corporate Data Theft", "Government File Decrypt",
    "Crypto Exchange Drain", "Dark Web Auction", "Satellite Hijack"
};
static const std::vector<std::string> raceNames = {
    "Midnight Drag Race", "Highway Sprint", "Underground Circuit",
    "Police Chase Derby", "Neon District Rally", "Canyon Run"
};
static const std::vector<std::string> heistNames = {
    "Federal Reserve Break-in", "Diamond Exchange Heist", "Casino Royal Job",
    "Central Bank Infiltration", "Museum of Antiquities", "Tech Corp Vault"
};

std::vector<Mission*> generateAvailableMissions(int rankLevel) {
    std::vector<Mission*> missions;
    int baseDiff = 20 + rankLevel * 10;
    int baseReward = 2000 + rankLevel * 1500;

    // Generate 3-5 missions
    int count = getRandomInt(3, 5);
    for (int i = 0; i < count; i++) {
        int type = getRandomInt(1, 5);
        int diff = baseDiff + getRandomInt(-10, 20);
        int reward = baseReward + getRandomInt(-500, 1000);
        switch (type) {
            case 1: missions.push_back(new RobberyMission(
                robberyNames[getRandomInt(0, (int)robberyNames.size()-1)], diff, reward)); break;
            case 2: missions.push_back(new SmugglingMission(
                smuggleNames[getRandomInt(0, (int)smuggleNames.size()-1)], diff, reward)); break;
            case 3: missions.push_back(new HackingMission(
                hackNames[getRandomInt(0, (int)hackNames.size()-1)], diff, reward)); break;
            case 4: missions.push_back(new RacingMission(
                raceNames[getRandomInt(0, (int)raceNames.size()-1)], diff, reward)); break;
            case 5: missions.push_back(new HeistMission(
                heistNames[getRandomInt(0, (int)heistNames.size()-1)], diff * 3/2, reward * 2)); break;
        }
    }
    return missions;
}

void cleanupMissions(std::vector<Mission*>& missions) {
    for (auto* m : missions) delete m;
    missions.clear();
}
