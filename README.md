# Underworld Empire Management System
> **Strategic Crime Network Simulation using Object-Oriented Programming in C++**
> *Version 2.0 (Cyberpunk Terminal Edition)*

---

## 🌌 Overview

The **Underworld Empire Management System** is a text-based, cinematic strategy simulation game built entirely in Modern C++ using advanced Object-Oriented Programming (OOP) principles. 

In this game, the player steps into the shoes of an underground operator starting with a single territory and zero crew. The goal is to grow the organization into a dominant criminal empire by balancing financial growth, hiring specialized crew members, conquering rival territories, dealing with persistent police heat, fighting off rival cartels, and managing black market goods.

---

## 🛠️ Tech Stack & OOP Architecture

This project is built from scratch utilizing the following software engineering paradigms:

- **Core Language**: C++14 / C++17
- **Encapsulation**: Private and protected data structures for all domain systems (`Player`, `GangMember`, `Territory`, `Mission`, `Police`, `RivalGang`, `BlackMarket`, `NewsSystem`) managed via getter/setter interfaces.
- **Inheritance**: 
  - Abstract `GangMember` base class inherited by specialized classes: `Fighter`, `Hacker`, `Driver`, and `Smuggler`.
  - Abstract `Mission` base class inherited by custom mission handlers: `RobberyMission`, `SmugglingMission`, `HackingMission`, `RacingMission`, and `HeistMission`.
- **Polymorphism**:
  - Virtual functions override member attributes and behavior dynamics (e.g., calling `specialAbility()` or running custom mission equations in `execute()`).
- **File Handling**: Game state serialization using pipe-delimited data structures processed via `std::ifstream` and `std::ofstream`.
- **Visual Design System**: Custom ANSI code-based render engine providing animated typing, text delays, color hierarchies, custom progress bars, and an ASCII city-map interface.

---

## 🎮 Core Game Systems

### 1. Player Progression (`Player`)
Tracks your resources: cash, respect (reputation), and police heat level. Rank dynamically upgrades as your score grows:
$$\text{Street Rat} \rightarrow \text{Hustler} \rightarrow \text{Enforcer} \rightarrow \text{Boss} \rightarrow \text{Kingpin}$$

### 2. Crew Management (`GangMember`)
Hire and manage crew members. Each class brings unique capabilities:
- **Fighter**: Boosts operation combat power.
- **Hacker**: Constantly reduces police heat and excels at digital hacks.
- **Driver**: Increases mission escape and racing success.
- **Smuggler**: Maximizes profits from territory income.

*Operatives have a dynamic loyalty meter. Low loyalty can trigger defection to rival gangs, money theft, or police tip-offs.*

### 3. Territory Domination (`Territory`)
Command a grid of 8 unique city zones (e.g., *The Docks*, *Downtown*, *Neon Strip*). Attack neutral or rival territories to claim them. Claims increase your turn-by-turn cashflow, but require upgrading defenses to repel hostile counter-attacks.

### 4. Interactive Operations (`Mission`)
Embark on high-stakes missions. Success probability scales dynamically based on crew layout, current heat, and item equipment. Rewards range from huge currency gains to permanent respect, while failure risks crew arrest or casualties.

### 5. Police Network (`Police`)
As your heat grows, law enforcement intensity scales. High heat levels trigger investigations, asset/cash seizures, territory shutdowns, and SWAT raids. Mitigate threats by buying off corrupted officers.

### 6. Rival Cartels (`RivalGang`)
Four active computer-controlled factions compete for territories, wage wars, intercept your income channels, and launch attacks against your properties.

### 7. Black Market Exchange (`BlackMarket`)
Purchase 16 specialized items categorized into Weapons, Armor, Vehicles, and Intel to permanently boost combat stats, defense strength, escape multipliers, and security features.

### 8. Live Events & News Network (`NewsSystem`)
Features a news ticker tracking events and changes in the city. Contains 16 random world events (e.g., informant leaks, money laundering windfalls, warehouse fires, market crashes) that alter variables each turn.

---

## 📁 File Structure

```
/
├── main.cpp             # Game entry point
├── game.h / .cpp        # Main game engine loop, startup logic
├── game_menus.cpp       # GUI command handlers and menus
├── utils.h              # Visual rendering utilities (ansi/delays/formats)
├── player.h / .cpp      # Player metrics and rank management
├── member.h / .cpp      # Polymorphic crew hierarchies and factory
├── territory.h / .cpp   # Area claims, defense, and ASCII map renderer
├── mission.h / .cpp     # Mission variants and resolution equations
├── police.h / .cpp      # Police alerts, raids, and bribes
├── rivalgang.h / .cpp   # AI cartel behaviors and declarations
├── blackmarket.h / .cpp # Market inventories and transaction logic
├── news.h / .cpp        # Headlines and random event pool
├── build.bat            # Windows MS-DOS compilation script
└── save/                # Output save file folder
```

---

## 🚀 Building and Running the Game

### Prerequisites
- **Compiler**: Any GCC compiler supporting C++11 or higher (e.g. MinGW, MSVC, Clang).

### 🖥️ Windows Build
1. Open Command Prompt (`cmd`) and navigate to the project directory.
2. Run the build batch file:
   ```cmd
   build.bat
   ```
3. Run the compiled game:
   ```cmd
   UnderworldEmpire.exe
   ```

### 🐧 Linux / macOS Build
1. Open your terminal and navigate to the folder.
2. Compile using the terminal:
   ```bash
   g++ -std=c++11 -o UnderworldEmpire main.cpp member.cpp territory.cpp mission.cpp police.cpp rivalgang.cpp blackmarket.cpp news.cpp player.cpp game.cpp game_menus.cpp -O2
   ```
3. Run the executable:
   ```bash
   ./UnderworldEmpire
   ```

---

## 💾 Save System Specifications
All game parameters, dynamic vectors, and attributes are recorded in a pipe-delimited data structures block within `save/savegame.txt`. 

To load your progress, select **Option 2 (Load Saved Operation)** during the terminal system boot sequence or run **Option 9** in the main command center.
