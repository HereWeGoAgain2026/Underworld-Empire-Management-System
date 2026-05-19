// ============================================================
// game.cpp — Main Game Controller Implementation (Part 1)
// Underworld Empire Management System
// Core loop, boot sequence, menus, and turn processing
// ============================================================

#include "game.h"

// ═══════════ Constructor / Destructor ═══════════

Game::Game() : running(true), turn(1), saveDirectory("save") {}

Game::~Game() {
    for (auto* m : members) delete m;
    members.clear();
}

// ═══════════ Main Entry ═══════════

void Game::run() {
    enableANSI();
    clearScreen();
    bootSequence();
    displayTitle();

    // Ask to load or start new
    std::cout << std::endl;
    std::cout << Color::CYAN << "  ┌─────────────────────────────────────┐" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "  1. New Operation                    " << Color::CYAN << "│" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "  2. Load Saved Operation             " << Color::CYAN << "│" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  └─────────────────────────────────────┘" << Color::RESET << std::endl;
    int choice = getMenuChoice(1, 2);

    if (choice == 2) {
        loadGame();
    } else {
        std::string pName = getStringInput("Enter your operator codename: ");
        if (pName.empty()) pName = "Shadow";
        player = Player(pName);
        territories = initializeDefaultTerritories();
        rivals = initializeRivalGangs();
        // Give player starting territory
        territories[2].setOwner("player"); // The Slums
        // Give rivals some territories
        territories[5].setOwner(rivals[0].getName());
        territories[7].setOwner(rivals[2].getName());
        rivals[0].setTerritoryCount(2);
        rivals[2].setTerritoryCount(2);
        market = BlackMarket();
        printSuccess("Welcome to the underground, " + pName + ". Build your empire.");
        delay(1500);
    }

    // ── Main game loop ──
    while (running) {
        clearScreen();
        player.displayHUD(turn, getActiveMemberCount(), getPlayerTerritoryCount(), (int)territories.size());
        news.displayNewsTicker();
        displayMainMenu();
        int ch = getMenuChoice(0, 9);
        switch (ch) {
            case 1: viewEmpire(); break;
            case 2: recruitMembers(); break;
            case 3: manageTerritories(); break;
            case 4: startMission(); break;
            case 5: rivalActivity(); break;
            case 6: policeNetwork(); break;
            case 7: blackMarketMenu(); break;
            case 8: saveGame(); break;
            case 9: loadGame(); break;
            case 0:
                std::cout << std::endl;
                printWarning("Exiting the network. Stay in the shadows...");
                delay(1000);
                running = false;
                break;
            default:
                printError("Invalid command. Try again.");
                delay(800);
                break;
        }
        if (running && ch >= 1 && ch <= 7) {
            endTurn();
            if (checkGameOver()) { displayGameOver(); running = false; }
        }
    }
}

// ═══════════ Boot Sequence ═══════════

void Game::bootSequence() {
    std::cout << std::endl;
    std::cout << Color::DIM << "  ══════════════════════════════════════════════════" << Color::RESET << std::endl;
    std::cout << Color::CYAN << Color::BOLD << "          SYSTEM INITIALIZATION SEQUENCE" << Color::RESET << std::endl;
    std::cout << Color::DIM << "  ══════════════════════════════════════════════════" << Color::RESET << std::endl;
    std::cout << std::endl;
    delay(400);
    bootItem("Initializing underground network",    80, 400);
    bootItem("Decrypting police database",          90, 500);
    bootItem("Connecting black-market servers",     70, 350);
    bootItem("Loading city intelligence",           85, 450);
    bootItem("Establishing secure channels",        60, 300);
    bootItem("Synchronizing rival gang data",       75, 400);
    bootItem("Activating encryption protocols",     65, 350);
    std::cout << std::endl;
    delay(300);
    flashText("    >>> ACCESS GRANTED <<<", Color::BRIGHT_GREEN, 3);
    delay(800);
}

// ═══════════ Title Screen ═══════════

void Game::displayTitle() {
    clearScreen();
    std::cout << std::endl;
    std::cout << Color::BRIGHT_CYAN << Color::BOLD;
    std::cout << R"(
     ██╗   ██╗███╗   ██╗██████╗ ███████╗██████╗ 
     ██║   ██║████╗  ██║██╔══██╗██╔════╝██╔══██╗
     ██║   ██║██╔██╗ ██║██║  ██║█████╗  ██████╔╝
     ██║   ██║██║╚██╗██║██║  ██║██╔══╝  ██╔══██╗
     ╚██████╔╝██║ ╚████║██████╔╝███████╗██║  ██║
      ╚═════╝ ╚═╝  ╚═══╝╚═════╝ ╚══════╝╚═╝  ╚═╝
    )" << Color::RESET << std::endl;
    std::cout << Color::BRIGHT_CYAN;
    std::cout << R"(
     ██╗    ██╗ ██████╗ ██████╗ ██╗     ██████╗ 
     ██║    ██║██╔═══██╗██╔══██╗██║     ██╔══██╗
     ██║ █╗ ██║██║   ██║██████╔╝██║     ██║  ██║
     ██║███╗██║██║   ██║██╔══██╗██║     ██║  ██║
     ╚███╔███╔╝╚██████╔╝██║  ██║███████╗██████╔╝
      ╚══╝╚══╝  ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═════╝ 
    )" << Color::RESET << std::endl;
    std::cout << Color::DIM << "    ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << Color::RESET << std::endl;
    std::cout << Color::BRIGHT_YELLOW << Color::BOLD
              << "       EMPIRE MANAGEMENT CONTROL SYSTEM v2.0" << Color::RESET << std::endl;
    std::cout << Color::DIM << "       Strategic Crime Network Simulation (OOP C++)" << Color::RESET << std::endl;
    std::cout << Color::DIM << "    ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << Color::RESET << std::endl;
    delay(1500);
}

// ═══════════ Main Menu ═══════════

void Game::displayMainMenu() {
    std::cout << std::endl;
    std::cout << Color::CYAN << "  ┌──────────────────────────────────────────────┐" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BOLD << Color::BRIGHT_CYAN << "         COMMAND CENTER                       " << Color::CYAN << "│" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  ├──────────────────────────────────────────────┤" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "   1. " << Color::BRIGHT_GREEN  << "View Empire          " << Color::DIM << "  Intelligence   " << Color::CYAN << "│" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "   2. " << Color::BRIGHT_GREEN  << "Recruit Members      " << Color::DIM << "  Personnel     " << Color::CYAN << " │" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "   3. " << Color::BRIGHT_YELLOW << "Manage Territories   " << Color::DIM << "  Expansion     " << Color::CYAN << " │" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "   4. " << Color::BRIGHT_RED    << "Start Mission        " << Color::DIM << "  Operations    " << Color::CYAN << " │" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "   5. " << Color::BRIGHT_RED    << "Rival Gang Activity  " << Color::DIM << "  Threats       " << Color::CYAN << " │" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "   6. " << Color::BRIGHT_BLUE   << "Police Network       " << Color::DIM << "  Law Enforce   " << Color::CYAN << " │" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "   7. " << Color::BRIGHT_MAGENTA<< "Black Market         " << Color::DIM << "  Commerce      " << Color::CYAN << " │" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  ├──────────────────────────────────────────────┤" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "   8. " << Color::DIM           << "Save Game            " << Color::DIM << "  Backup        " << Color::CYAN << " │" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "   9. " << Color::DIM           << "Load Game            " << Color::DIM << "  Restore       " << Color::CYAN << " │" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  │" << Color::BRIGHT_WHITE << "   0. " << Color::RED            << "Exit Network         " << Color::DIM << "  Disconnect    " << Color::CYAN << " │" << Color::RESET << std::endl;
    std::cout << Color::CYAN << "  └──────────────────────────────────────────────┘" << Color::RESET << std::endl;
}

// ═══════════ Game Over ═══════════

void Game::displayGameOver() {
    clearScreen();
    std::cout << std::endl;
    if (player.getMoney() <= -5000) {
        printAlert("EMPIRE COLLAPSED", "Your debts have consumed you. The empire crumbles.", Color::RED);
    } else if (player.getHeatLevel() >= 100) {
        printAlert("ARRESTED", "The law finally caught up. Your empire is dismantled.", Color::RED);
    } else {
        printAlert("GAME OVER", "Your reign has ended.", Color::YELLOW);
    }
    std::cout << std::endl;
    std::cout << Color::DIM << "  Final Stats:" << Color::RESET << std::endl;
    std::cout << "  Turns: " << Color::CYAN << player.getTurnsPlayed() << Color::RESET
              << "  Money: " << Color::GREEN << formatMoney(player.getMoney()) << Color::RESET
              << "  Rank: " << getRankColor(player.getRank()) << player.getRank() << Color::RESET
              << "  Missions: " << Color::CYAN << player.getTotalMissions() << Color::RESET << std::endl;
    waitForEnter();
}

// ═══════════ Helpers ═══════════

int Game::getTeamStrength() const {
    int total = 0;
    for (auto* m : members)
        if (m->isAlive() && !m->isArrested()) total += m->getCombatPower();
    total += player.getCombatBonus();
    return total;
}

int Game::getSpecialBonus(const std::string& type) const {
    int total = 0;
    for (auto* m : members)
        if (m->isAlive() && !m->isArrested() && m->getType() == type)
            total += m->getSpecialBonus();
    return total;
}

int Game::getActiveMemberCount() const {
    int c = 0;
    for (auto* m : members) if (m->isAlive() && !m->isArrested()) c++;
    return c;
}

int Game::getPlayerTerritoryCount() const {
    int c = 0;
    for (const auto& t : territories) if (t.getOwner() == "player") c++;
    return c;
}

bool Game::checkGameOver() const {
    return (player.getMoney() <= -5000 || player.getHeatLevel() >= 100);
}
