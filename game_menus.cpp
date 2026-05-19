// ============================================================
// game_menus.cpp — Game Menu Handlers & Turn Processing
// Underworld Empire Management System
// ============================================================

#include "game.h"

// ═══════════ View Empire ═══════════

void Game::viewEmpire() {
    player.viewEmpire(members, territories);
    waitForEnter();
}

// ═══════════ Recruit Members ═══════════

void Game::recruitMembers() {
    clearScreen();
    printHeader("RECRUITMENT CENTER");
    std::cout << std::endl;

    // Show current members
    if (!members.empty()) {
        printSectionHeader("CURRENT CREW");
        for (auto* m : members) m->displayInfo();
        printSectionFooter();
    }

    std::cout << std::endl;
    std::cout << Color::CYAN << "  ┌─────────────────────────────────────┐" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "  1. Recruit New Member ($2,000)      " << Color::CYAN << "│" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "  2. View All Members                 " << Color::CYAN << "│" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "  3. Pay Loyalty Bonus                " << Color::CYAN << "│" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "  4. Dismiss Member                   " << Color::CYAN << "│" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "  0. Back                             " << Color::CYAN << "│" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  └─────────────────────────────────────┘" << Color::RESET << std::endl;

    int ch = getMenuChoice(0, 4);
    if (ch == 1) {
        int cost = 2000 + (player.getRankLevel() * 500);
        if (!player.spendMoney(cost)) { printError("Insufficient funds! Need " + formatMoney(cost)); waitForEnter(); return; }
        GangMember* recruit = generateRandomMember(player.getRankLevel());
        members.push_back(recruit);
        player.addMemberRecruited();
        std::cout << std::endl;
        printSuccess("New recruit joined your organization!");
        recruit->displayInfo();
        std::cout << Color::DIM << "  Ability: " << Color::RESET << Color::YELLOW << recruit->specialAbility() << Color::RESET << std::endl;
        waitForEnter();
    } else if (ch == 2) { viewAllMembers(); }
    else if (ch == 3) { payBonusToMember(); }
    else if (ch == 4) { dismissMember(); }
}

void Game::viewAllMembers() {
    clearScreen();
    printHeader("CREW ROSTER");
    if (members.empty()) { printInfo("No members recruited yet."); waitForEnter(); return; }
    for (int i = 0; i < (int)members.size(); i++) {
        std::cout << Color::DIM << "  #" << (i+1) << Color::RESET << std::endl;
        members[i]->displayInfo();
        std::cout << Color::DIM << "     Ability: " << Color::RESET << members[i]->specialAbility() << std::endl;
    }
    waitForEnter();
}

void Game::payBonusToMember() {
    if (members.empty()) { printInfo("No members to pay."); waitForEnter(); return; }
    for (int i = 0; i < (int)members.size(); i++) {
        std::cout << "  " << Color::CYAN << (i+1) << ". " << Color::RESET << members[i]->getName()
                  << " (Loyalty: " << members[i]->getLoyalty() << "%)" << std::endl;
    }
    int ch = getMenuChoice(1, (int)members.size());
    if (ch < 1) return;
    int bonus = 1000;
    if (!player.spendMoney(bonus)) { printError("Need " + formatMoney(bonus)); waitForEnter(); return; }
    members[ch-1]->increaseLoyalty(15);
    printSuccess(members[ch-1]->getName() + " loyalty increased to " + std::to_string(members[ch-1]->getLoyalty()) + "%");
    waitForEnter();
}

void Game::dismissMember() {
    if (members.empty()) { printInfo("No members to dismiss."); waitForEnter(); return; }
    for (int i = 0; i < (int)members.size(); i++) {
        std::cout << "  " << Color::CYAN << (i+1) << ". " << Color::RESET << members[i]->getName() << std::endl;
    }
    int ch = getMenuChoice(1, (int)members.size());
    if (ch < 1) return;
    printWarning("Dismissed: " + members[ch-1]->getName());
    delete members[ch-1];
    members.erase(members.begin() + (ch-1));
    waitForEnter();
}

// ═══════════ Territory Management ═══════════

void Game::manageTerritories() {
    clearScreen();
    printHeader("TERRITORY COMMAND");
    displayCityMap(territories);
    std::cout << std::endl;

    printSectionHeader("ALL TERRITORIES");
    for (int i = 0; i < (int)territories.size(); i++) {
        std::cout << Color::DIM << "  " << (i+1) << "." << Color::RESET;
        territories[i].displayTerritory();
    }
    printSectionFooter();

    std::cout << std::endl;
    std::cout << Color::CYAN << "  1. " << Color::BRIGHT_WHITE << "Attack Territory" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  2. " << Color::BRIGHT_WHITE << "Upgrade Defense" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  0. " << Color::BRIGHT_WHITE << "Back" << Color::RESET << std::endl;

    int ch = getMenuChoice(0, 2);
    if (ch == 1) attackTerritory();
    else if (ch == 2) upgradeTerritory();
}

void Game::attackTerritory() {
    if (getActiveMemberCount() < 1) { printError("You need crew members to attack!"); waitForEnter(); return; }
    std::cout << std::endl << Color::BRIGHT_RED << "  Select territory to attack (1-" << territories.size() << "): " << Color::RESET;
    int ch = getMenuChoice(1, (int)territories.size());
    if (ch < 1) return;
    Territory& target = territories[ch-1];
    if (target.getOwner() == "player") { printInfo("You already control this territory."); waitForEnter(); return; }

    int attackPower = getTeamStrength() + getRandomInt(-10, 20);
    std::cout << std::endl;
    typeEffectLn(Color::BRIGHT_RED + "  Launching assault on " + target.getName() + "..." + Color::RESET, 30);
    delay(800);

    printProgressBar("Your Force", attackPower, attackPower + target.getDefenseStrength(), Color::GREEN);
    printProgressBar("Defense", target.getDefenseStrength(), attackPower + target.getDefenseStrength(), Color::RED);
    delay(500);

    if (!target.defendAttack(attackPower)) {
        std::string prevOwner = target.getOwner();
        target.setOwner("player");
        player.addRespect(8);
        player.addHeat(10);
        player.addTerritoryCapture();
        printSuccess("Territory captured: " + target.getName() + "!");
        news.reportTerritoryChange(target.getName(), "player");
        // Weaken rival if applicable
        for (auto& r : rivals) {
            if (r.getName() == prevOwner) { r.weakenGang(8); r.setTerritoryCount(r.getTerritoryCount()-1); r.setAtWar(true); }
        }
    } else {
        player.addHeat(5);
        player.loseRespect(3);
        printError("Attack repelled! " + target.getName() + " holds strong.");
        if (randomChance(20) && !members.empty()) {
            int idx = getRandomInt(0, (int)members.size()-1);
            members[idx]->setAlive(false);
            printError(members[idx]->getName() + " was killed in the assault.");
        }
    }
    waitForEnter();
}

void Game::upgradeTerritory() {
    std::cout << std::endl << "  Select your territory to upgrade: " << std::endl;
    std::vector<int> owned;
    for (int i = 0; i < (int)territories.size(); i++) {
        if (territories[i].getOwner() == "player") {
            owned.push_back(i);
            std::cout << "  " << Color::CYAN << owned.size() << ". " << Color::RESET << territories[i].getName()
                      << " (Def: " << territories[i].getDefenseStrength() << ")" << std::endl;
        }
    }
    if (owned.empty()) { printInfo("You don't own any territories."); waitForEnter(); return; }
    int ch = getMenuChoice(1, (int)owned.size());
    if (ch < 1) return;
    int cost = 3000;
    if (!player.spendMoney(cost)) { printError("Need " + formatMoney(cost)); waitForEnter(); return; }
    territories[owned[ch-1]].upgradeDefense(15);
    printSuccess("Defense upgraded for " + territories[owned[ch-1]].getName() + "!");
    waitForEnter();
}

// ═══════════ Missions ═══════════

void Game::startMission() {
    clearScreen();
    printHeader("MISSION OPERATIONS CENTER");
    if (getActiveMemberCount() < 1) { printError("Recruit crew members before attempting missions."); waitForEnter(); return; }

    std::vector<Mission*> available = generateAvailableMissions(player.getRankLevel());
    std::cout << std::endl;
    std::cout << Color::DIM << "  Team Strength: " << Color::RESET << Color::BRIGHT_GREEN << getTeamStrength() << Color::RESET << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < (int)available.size(); i++) {
        std::cout << Color::DIM << "  ─── Mission " << (i+1) << " ───" << Color::RESET << std::endl;
        available[i]->displayMission();
    }
    std::cout << Color::DIM << "  ─── 0. Abort ───" << Color::RESET << std::endl;

    int ch = getMenuChoice(0, (int)available.size());
    if (ch < 1) { cleanupMissions(available); return; }

    Mission* mission = available[ch-1];
    std::cout << std::endl;
    typeEffectLn(Color::BRIGHT_YELLOW + "  Deploying team for: " + mission->getName() + "..." + Color::RESET, 25);
    delay(1000);

    int specialB = 0;
    if (mission->getType() == "Robbery") specialB = getSpecialBonus("Fighter");
    else if (mission->getType() == "Smuggling") specialB = getSpecialBonus("Smuggler");
    else if (mission->getType() == "Hacking") specialB = getSpecialBonus("Hacker");
    else if (mission->getType() == "Racing") specialB = getSpecialBonus("Driver");
    else specialB = getSpecialBonus("Fighter") + getSpecialBonus("Hacker");

    MissionResult result = mission->execute(getTeamStrength(), specialB, player.getHeatLevel(), getActiveMemberCount());

    // Display result
    std::cout << std::endl;
    if (result.success) {
        printAlert("MISSION SUCCESS", result.narrative, Color::GREEN);
    } else {
        printAlert("MISSION FAILED", result.narrative, Color::RED);
    }

    player.addMoney(result.moneyChange);
    player.addRespect(result.respectChange);
    player.addHeat(result.heatChange);
    if (result.success) player.addMissionCompleted();
    news.reportMissionResult(mission->getName(), result.success);

    // Handle casualties
    for (int i = 0; i < result.membersLost && !members.empty(); i++) {
        int idx = getRandomInt(0, (int)members.size()-1);
        if (members[idx]->isAlive()) members[idx]->setAlive(false);
    }
    for (int i = 0; i < result.membersArrested && !members.empty(); i++) {
        int idx = getRandomInt(0, (int)members.size()-1);
        if (members[idx]->isAlive() && !members[idx]->isArrested()) members[idx]->setArrested(true);
    }

    // Level up chance for surviving members
    if (result.success) {
        for (auto* m : members) {
            if (m->isAlive() && !m->isArrested() && randomChance(30)) m->levelUp();
        }
    }
    player.calculateRank();
    cleanupMissions(available);
    waitForEnter();
}

// ═══════════ Rival Activity ═══════════

void Game::rivalActivity() {
    clearScreen();
    printHeader("RIVAL GANG INTELLIGENCE");
    std::cout << std::endl;
    for (auto& r : rivals) {
        r.displayInfo();
        std::cout << std::endl;
    }
    std::cout << Color::CYAN << "  1. " << Color::BRIGHT_WHITE << "Declare War on a Rival" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  2. " << Color::BRIGHT_WHITE << "Negotiate Peace" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  0. " << Color::BRIGHT_WHITE << "Back" << Color::RESET << std::endl;
    int ch = getMenuChoice(0, 2);
    if (ch == 1) {
        for (int i = 0; i < (int)rivals.size(); i++)
            std::cout << "  " << (i+1) << ". " << rivals[i].getName() << std::endl;
        int r = getMenuChoice(1, (int)rivals.size());
        if (r >= 1) {
            rivals[r-1].setAtWar(true);
            rivals[r-1].setAggression(std::min(100, rivals[r-1].getAggression()+20));
            printWarning("War declared on " + rivals[r-1].getName() + "!");
            player.addHeat(8);
            player.addRespect(5);
            news.reportRivalAction(rivals[r-1].getName(), "War declared — tensions escalating.");
        }
    } else if (ch == 2) {
        for (int i = 0; i < (int)rivals.size(); i++)
            if (rivals[i].isAtWar())
                std::cout << "  " << (i+1) << ". " << rivals[i].getName() << " [AT WAR]" << std::endl;
        int r = getMenuChoice(1, (int)rivals.size());
        if (r >= 1 && rivals[r-1].isAtWar()) {
            int cost = rivals[r-1].getPower() * 100;
            if (player.spendMoney(cost)) {
                rivals[r-1].setAtWar(false);
                rivals[r-1].setAggression(std::max(10, rivals[r-1].getAggression()-15));
                printSuccess("Peace negotiated with " + rivals[r-1].getName() + " for " + formatMoney(cost));
            } else { printError("Need " + formatMoney(cost) + " for negotiations."); }
        }
    }
    waitForEnter();
}

// ═══════════ Police Network ═══════════

void Game::policeNetwork() {
    clearScreen();
    printHeader("POLICE NETWORK MONITOR");
    police.displayStatus();
    std::cout << std::endl;
    std::cout << Color::CYAN << "  1. " << Color::BRIGHT_WHITE << "Bribe Officer ($5,000)     -15 Heat" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  2. " << Color::BRIGHT_WHITE << "Corrupt Officer ($10,000)  Permanent" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  3. " << Color::BRIGHT_WHITE << "Lay Low                    -5 Heat" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  0. " << Color::BRIGHT_WHITE << "Back" << Color::RESET << std::endl;
    int ch = getMenuChoice(0, 3);
    if (ch == 1) {
        if (player.spendMoney(5000)) {
            player.reduceHeat(15);
            printSuccess("Officer bribed. Heat reduced.");
            news.reportPoliceAction("Corruption suspected in precinct — investigation stalled.");
        } else printError("Insufficient funds.");
    } else if (ch == 2) {
        if (player.spendMoney(10000)) {
            police.corruptOfficer();
            player.reduceHeat(10);
            printSuccess("Officer permanently corrupted. Raids will be less effective.");
        } else printError("Insufficient funds.");
    } else if (ch == 3) {
        player.reduceHeat(5);
        printInfo("Laying low. Heat reduced slightly.");
    }
    waitForEnter();
}

// ═══════════ Black Market ═══════════

void Game::blackMarketMenu() {
    clearScreen();
    market.displayShop();
    std::cout << std::endl;
    std::cout << Color::GREEN << "  Your funds: " << Color::BOLD << formatMoney(player.getMoney()) << Color::RESET << std::endl;
    std::cout << Color::DIM << "  Enter item number to purchase (0 to exit): " << Color::RESET << std::endl;
    int ch = getMenuChoice(0, market.getItemCount());
    if (ch < 1) return;
    int money = player.getMoney();
    BlackMarketItem item = market.purchaseItem(ch-1, money);
    if (item.name.empty()) {
        printError("Purchase failed. Item unavailable or insufficient funds.");
    } else {
        player.setMoney(money);
        player.addItem(item.statAffected, item.statBonus);
        printSuccess("Acquired: " + item.name + " (+" + std::to_string(item.statBonus) + " " + item.statAffected + ")");
        player.addHeat(2);
    }
    waitForEnter();
}

// ═══════════ Save / Load ═══════════

void Game::saveGame() {
    // Create save directory
    #ifdef _WIN32
    system(("mkdir " + saveDirectory + " 2>nul").c_str());
    #else
    system(("mkdir -p " + saveDirectory).c_str());
    #endif

    std::ofstream file(saveDirectory + "/savegame.txt");
    if (!file.is_open()) { printError("Failed to open save file!"); waitForEnter(); return; }

    file << "=== UNDERWORLD SAVE ===\n";
    file << turn << "\n";
    file << "=== PLAYER ===\n";
    player.saveToFile(file);
    file << "=== MEMBERS ===\n";
    file << members.size() << "\n";
    for (auto* m : members) m->saveToFile(file);
    file << "=== TERRITORIES ===\n";
    file << territories.size() << "\n";
    for (auto& t : territories) t.saveToFile(file);
    file << "=== POLICE ===\n";
    police.saveToFile(file);
    file << "=== RIVALS ===\n";
    file << rivals.size() << "\n";
    for (auto& r : rivals) r.saveToFile(file);
    file << "=== END ===\n";
    file.close();

    printSuccess("Game saved successfully to " + saveDirectory + "/savegame.txt");
    waitForEnter();
}

void Game::loadGame() {
    std::ifstream file(saveDirectory + "/savegame.txt");
    if (!file.is_open()) { printError("No save file found!"); waitForEnter(); return; }

    std::string line;
    std::getline(file, line); // header
    std::getline(file, line); turn = std::stoi(line);
    std::getline(file, line); // === PLAYER ===
    player.loadFromFile(file);

    std::getline(file, line); // === MEMBERS ===
    std::getline(file, line);
    int memberCount = std::stoi(line);
    for (auto* m : members) delete m;
    members.clear();
    for (int i = 0; i < memberCount; i++) {
        GangMember* m = GangMember::loadFromFile(file);
        if (m) members.push_back(m);
    }

    std::getline(file, line); // === TERRITORIES ===
    std::getline(file, line);
    int terrCount = std::stoi(line);
    territories.clear();
    for (int i = 0; i < terrCount; i++) {
        Territory t;
        t.loadFromFile(file);
        territories.push_back(t);
    }

    std::getline(file, line); // === POLICE ===
    police.loadFromFile(file);

    std::getline(file, line); // === RIVALS ===
    std::getline(file, line);
    int rivalCount = std::stoi(line);
    rivals.clear();
    for (int i = 0; i < rivalCount; i++) {
        RivalGang r;
        r.loadFromFile(file);
        rivals.push_back(r);
    }
    file.close();

    printSuccess("Game loaded successfully! Welcome back, " + player.getName() + ".");
    waitForEnter();
}

// ═══════════ Turn Processing ═══════════

void Game::endTurn() {
    player.incrementTurns();
    turn++;
    player.calculateRank();

    collectIncome();
    paySalaries();
    processLoyaltyDecay();
    processPoliceActivity();
    processRivalAI();

    // 35% chance of random event
    if (randomChance(35)) processRandomEvent();

    // Natural heat decay
    player.reduceHeat(getRandomInt(1, 3));

    // Hacker heat reduction
    int hackerBonus = getSpecialBonus("Hacker");
    if (hackerBonus > 0) {
        player.reduceHeat(hackerBonus / 2);
    }

    police.updateAlert(player.getHeatLevel());
    police.decrementCooldown();
    market.refreshStock();
}

void Game::collectIncome() {
    player.collectTerritoryIncome(territories);
    // Smuggler profit bonus
    int smugBonus = getSpecialBonus("Smuggler");
    if (smugBonus > 0) {
        int bonus = getPlayerTerritoryCount() * smugBonus * 5;
        if (bonus > 0) { player.addMoney(bonus); }
    }
}

void Game::paySalaries() {
    player.payMemberSalaries(members);
    // If can't pay, loyalty drops
    int totalSal = 0;
    for (auto* m : members) if (m->isAlive() && !m->isArrested()) totalSal += m->getSalary();
    if (totalSal > 0 && player.getMoney() < 0) {
        for (auto* m : members) m->decreaseLoyalty(10);
        printWarning("Members are unhappy — loyalty dropping!");
    }
}

void Game::processLoyaltyDecay() {
    for (auto* m : members) {
        if (!m->isAlive() || m->isArrested()) continue;
        m->decreaseLoyalty(getRandomInt(1, 3));
        if (m->checkBetrayal()) {
            int betrayalType = getRandomInt(1, 3);
            if (betrayalType == 1) {
                int stolen = getRandomInt(500, 3000);
                player.addMoney(-stolen);
                printAlert("BETRAYAL", m->getName() + " stole " + formatMoney(stolen) + " and fled!", Color::RED);
                news.generateHeadline("Insider betrayal — " + m->getName() + " defected with stolen funds.");
                m->setAlive(false);
            } else if (betrayalType == 2) {
                player.addHeat(15);
                printAlert("INFORMANT", m->getName() + " leaked intel to the police!", Color::YELLOW);
                news.generateHeadline("Anonymous tip leads to increased police activity.");
                m->setLoyalty(0);
            } else {
                // Joins a rival gang
                if (!rivals.empty()) {
                    int ri = getRandomInt(0, (int)rivals.size()-1);
                    printAlert("DEFECTION", m->getName() + " joined " + rivals[ri].getName() + "!", Color::RED);
                    rivals[ri].setPower(std::min(100, rivals[ri].getPower()+5));
                    m->setAlive(false);
                }
            }
        }
    }
    // Remove dead members
    members.erase(std::remove_if(members.begin(), members.end(),
        [](GangMember* m) { if (!m->isAlive()) { delete m; return true; } return false; }),
        members.end());
}

void Game::processPoliceActivity() {
    if (police.attemptRaid(player.getHeatLevel())) {
        police.displayRaidWarning();
        delay(1500);
        int seized = police.conductCrackdown();
        player.addMoney(-seized);
        printError("Police seized " + formatMoney(seized) + " in the raid!");
        news.reportPoliceAction("Major raid conducted — assets seized.");

        if (police.attemptArrest() && !members.empty()) {
            int idx = getRandomInt(0, (int)members.size()-1);
            if (members[idx]->isAlive()) {
                members[idx]->setArrested(true);
                printError(members[idx]->getName() + " was arrested during the raid!");
            }
        }
        // Chance to lose a territory
        if (randomChance(25)) {
            for (auto& t : territories) {
                if (t.getOwner() == "player" && t.isActive()) {
                    t.setActive(false);
                    printError(t.getName() + " operations shut down by police!");
                    break;
                }
            }
        }
    }

    if (police.triggerInvestigation(player.getHeatLevel())) {
        printWarning("Police have opened an investigation into your operations.");
        news.reportPoliceAction("New investigation launched targeting organized crime.");
    }
}

void Game::processRivalAI() {
    for (auto& rival : rivals) {
        if (!rival.isAtWar()) { rival.strengthenGang(); continue; }

        // At war — rival attacks
        if (randomChance(rival.getAggression())) {
            int damage = rival.attackPlayer(getTeamStrength());
            if (damage > 0) {
                player.addMoney(-damage);
                player.loseRespect(3);
                printAlert("GANG WAR", rival.getName() + " attacked! Lost " + formatMoney(damage), Color::RED);
                news.reportRivalAction(rival.getName(), "launched assault — casualties reported.");
            } else {
                rival.weakenGang(5);
                printSuccess("Repelled attack from " + rival.getName() + "!");
                player.addRespect(3);
            }
        }

        // Rival steals income
        int stolen = rival.stealIncome(getPlayerTerritoryCount() * 500);
        if (stolen > 0) {
            player.addMoney(-stolen);
            printWarning(rival.getName() + " intercepted " + formatMoney(stolen) + " of your income.");
        }

        // Rival expands
        if (rival.expandTerritory(territories)) {
            news.reportRivalAction(rival.getName(), "expanded into new territory.");
        }
    }
}

void Game::processRandomEvent() {
    RandomEvent event = news.triggerRandomEvent();
    news.displayEvent(event);
    delay(1500);
    player.addMoney(event.moneyEffect);
    player.addRespect(event.respectEffect);
    player.addHeat(event.heatEffect);
    for (auto* m : members) {
        if (m->isAlive()) m->increaseLoyalty(event.loyaltyEffect);
    }
}
