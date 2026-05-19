// ============================================================
// member.cpp — Gang Member Implementations
// Underworld Empire Management System
// ============================================================

#include "member.h"

// ─────────────── Name pools ───────────────
static const std::vector<std::string> firstNames = {
    "Viktor", "Dante", "Raven", "Blaze", "Shadow", "Viper", "Axel",
    "Scar", "Jinx", "Ghost", "Reaper", "Nyx", "Cobra", "Slate",
    "Razor", "Cypher", "Knox", "Onyx", "Flint", "Dagger",
    "Luna", "Ember", "Storm", "Vex", "Riot", "Ash", "Cruz",
    "Jett", "Kira", "Zane", "Mako", "Rogue", "Hex", "Talon"
};
static const std::vector<std::string> lastNames = {
    "Volkov", "Moretti", "Chen", "Santos", "Drakov", "Reyes", "Ivanov",
    "Kruger", "Tanaka", "Okafor", "Petrova", "Mendez", "Kozlov", "Park",
    "Blackwood", "Ashford", "Kessler", "Varga", "DeLuca", "Ortiz"
};

// ═══════════ GangMember Base ═══════════

GangMember::GangMember(const std::string& name, const std::string& type,
                       int loyalty, int strength, int salary, int level)
    : name(name), type(type), loyalty(loyalty), strength(strength),
      salary(salary), level(level), alive(true), arrested(false) {}

void GangMember::levelUp() {
    if (level < 10) {
        level++;
        strength = std::min(100, strength + getRandomInt(3, 8));
        salary += getRandomInt(100, 300);
        printSuccess(name + " leveled up to Level " + std::to_string(level) + "!");
    }
}

void GangMember::decreaseLoyalty(int amount) { loyalty = std::max(0, loyalty - amount); }
void GangMember::increaseLoyalty(int amount) { loyalty = std::min(100, loyalty + amount); }

bool GangMember::checkBetrayal() const {
    if (loyalty < 20) return randomChance(40);
    if (loyalty < 35) return randomChance(15);
    if (loyalty < 50) return randomChance(5);
    return false;
}

int GangMember::getCombatPower() const {
    return strength + (level * 5) + (loyalty / 10);
}

void GangMember::saveToFile(std::ofstream& out) const {
    out << type << "|" << name << "|" << loyalty << "|" << strength
        << "|" << salary << "|" << level << "|" << alive << "|" << arrested << "\n";
}

GangMember* GangMember::loadFromFile(std::ifstream& in) {
    std::string line;
    if (!std::getline(in, line) || line.empty()) return nullptr;
    std::vector<std::string> t;
    std::stringstream ss(line);
    std::string tok;
    while (std::getline(ss, tok, '|')) t.push_back(tok);
    if (t.size() < 8) return nullptr;

    GangMember* m = nullptr;
    int lo = std::stoi(t[2]), st = std::stoi(t[3]), sa = std::stoi(t[4]), lv = std::stoi(t[5]);
    if (t[0] == "Fighter")       m = new Fighter(t[1], lo, st, sa, lv);
    else if (t[0] == "Hacker")   m = new Hacker(t[1], lo, st, sa, lv);
    else if (t[0] == "Driver")   m = new Driver(t[1], lo, st, sa, lv);
    else if (t[0] == "Smuggler") m = new Smuggler(t[1], lo, st, sa, lv);
    if (m) { m->setAlive(t[6] == "1"); m->setArrested(t[7] == "1"); }
    return m;
}

// ═══════════ Fighter ═══════════

Fighter::Fighter(const std::string& n, int lo, int st, int sa, int lv)
    : GangMember(n, "Fighter", lo, st, sa, lv) {}

void Fighter::displayInfo() const {
    std::string status = alive ? (arrested ? Color::YELLOW+"JAILED" : Color::BRIGHT_GREEN+"ACTIVE") : Color::RED+"K.I.A.";
    std::cout << Color::RED << "  [F] " << Color::BOLD << Color::BRIGHT_WHITE << name
              << Color::RESET << Color::DIM << " Lv." << level << Color::RESET
              << "  STR:" << Color::RED << strength << Color::RESET
              << "  LOY:" << (loyalty>60?Color::GREEN:(loyalty>30?Color::YELLOW:Color::RED)) << loyalty << "%" << Color::RESET
              << "  $" << Color::YELLOW << salary << Color::RESET
              << "  [" << status << Color::RESET << "]" << std::endl;
}
std::string Fighter::specialAbility() const { return "Combat Specialist +"+std::to_string(getSpecialBonus())+"% combat"; }
int Fighter::getSpecialBonus() const { return 15 + (level * 3); }

// ═══════════ Hacker ═══════════

Hacker::Hacker(const std::string& n, int lo, int st, int sa, int lv)
    : GangMember(n, "Hacker", lo, st, sa, lv) {}

void Hacker::displayInfo() const {
    std::string status = alive ? (arrested ? Color::YELLOW+"JAILED" : Color::BRIGHT_GREEN+"ACTIVE") : Color::RED+"K.I.A.";
    std::cout << Color::CYAN << "  [H] " << Color::BOLD << Color::BRIGHT_WHITE << name
              << Color::RESET << Color::DIM << " Lv." << level << Color::RESET
              << "  STR:" << Color::CYAN << strength << Color::RESET
              << "  LOY:" << (loyalty>60?Color::GREEN:(loyalty>30?Color::YELLOW:Color::RED)) << loyalty << "%" << Color::RESET
              << "  $" << Color::YELLOW << salary << Color::RESET
              << "  [" << status << Color::RESET << "]" << std::endl;
}
std::string Hacker::specialAbility() const { return "Cyber Intrusion -"+std::to_string(getSpecialBonus())+" heat/turn"; }
int Hacker::getSpecialBonus() const { return 3 + level; }

// ═══════════ Driver ═══════════

Driver::Driver(const std::string& n, int lo, int st, int sa, int lv)
    : GangMember(n, "Driver", lo, st, sa, lv) {}

void Driver::displayInfo() const {
    std::string status = alive ? (arrested ? Color::YELLOW+"JAILED" : Color::BRIGHT_GREEN+"ACTIVE") : Color::RED+"K.I.A.";
    std::cout << Color::GREEN << "  [D] " << Color::BOLD << Color::BRIGHT_WHITE << name
              << Color::RESET << Color::DIM << " Lv." << level << Color::RESET
              << "  STR:" << Color::GREEN << strength << Color::RESET
              << "  LOY:" << (loyalty>60?Color::GREEN:(loyalty>30?Color::YELLOW:Color::RED)) << loyalty << "%" << Color::RESET
              << "  $" << Color::YELLOW << salary << Color::RESET
              << "  [" << status << Color::RESET << "]" << std::endl;
}
std::string Driver::specialAbility() const { return "Wheelman +"+std::to_string(getSpecialBonus())+"% escape"; }
int Driver::getSpecialBonus() const { return 10 + (level * 4); }

// ═══════════ Smuggler ═══════════

Smuggler::Smuggler(const std::string& n, int lo, int st, int sa, int lv)
    : GangMember(n, "Smuggler", lo, st, sa, lv) {}

void Smuggler::displayInfo() const {
    std::string status = alive ? (arrested ? Color::YELLOW+"JAILED" : Color::BRIGHT_GREEN+"ACTIVE") : Color::RED+"K.I.A.";
    std::cout << Color::MAGENTA << "  [S] " << Color::BOLD << Color::BRIGHT_WHITE << name
              << Color::RESET << Color::DIM << " Lv." << level << Color::RESET
              << "  STR:" << Color::MAGENTA << strength << Color::RESET
              << "  LOY:" << (loyalty>60?Color::GREEN:(loyalty>30?Color::YELLOW:Color::RED)) << loyalty << "%" << Color::RESET
              << "  $" << Color::YELLOW << salary << Color::RESET
              << "  [" << status << Color::RESET << "]" << std::endl;
}
std::string Smuggler::specialAbility() const { return "Black Market +"+std::to_string(getSpecialBonus())+"% profits"; }
int Smuggler::getSpecialBonus() const { return 10 + (level * 3); }

// ═══════════ Factory ═══════════

GangMember* generateRandomMember(int qualityTier) {
    std::string fn = firstNames[getRandomInt(0, (int)firstNames.size()-1)];
    std::string ln = lastNames[getRandomInt(0, (int)lastNames.size()-1)];
    std::string name = fn + " " + ln;
    int lo = getRandomInt(40,60) + qualityTier*5;
    int st = getRandomInt(20,40) + qualityTier*8;
    int sa = getRandomInt(300,600) + qualityTier*150;
    int lv = std::min(10, 1 + qualityTier/2);
    switch (getRandomInt(1,4)) {
        case 1: return new Fighter(name, lo, st+10, sa, lv);
        case 2: return new Hacker(name, lo, st-5, sa+100, lv);
        case 3: return new Driver(name, lo, st, sa, lv);
        default: return new Smuggler(name, lo, st-3, sa+50, lv);
    }
}
