// ============================================================
// member.h — Gang Member Class Hierarchy (Declaration)
// Underworld Empire Management System
// Demonstrates: Inheritance, Polymorphism, Abstraction
// ============================================================
#pragma once

#include "utils.h"

// ─────────────────────────────────────────────
//  Base Class: GangMember (Abstract)
// ─────────────────────────────────────────────
class GangMember {
protected:
    std::string name;
    std::string type;       // Fighter, Hacker, Driver, Smuggler
    int loyalty;            // 0-100
    int strength;           // 0-100
    int salary;             // cost per turn
    int level;              // 1-10
    bool alive;
    bool arrested;

public:
    GangMember(const std::string& name, const std::string& type,
               int loyalty, int strength, int salary, int level);
    virtual ~GangMember() = default;

    // Pure virtual functions — must be overridden by derived classes
    virtual void displayInfo() const = 0;
    virtual std::string specialAbility() const = 0;
    virtual int getSpecialBonus() const = 0;

    // Common member functions
    void levelUp();
    void decreaseLoyalty(int amount);
    void increaseLoyalty(int amount);
    bool checkBetrayal() const;
    int getCombatPower() const;

    // Getters
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    int getLoyalty() const { return loyalty; }
    int getStrength() const { return strength; }
    int getSalary() const { return salary; }
    int getLevel() const { return level; }
    bool isAlive() const { return alive; }
    bool isArrested() const { return arrested; }

    // Setters
    void setAlive(bool val) { alive = val; }
    void setArrested(bool val) { arrested = val; }
    void setLoyalty(int val) { loyalty = std::max(0, std::min(100, val)); }

    // Save/Load
    virtual void saveToFile(std::ofstream& out) const;
    static GangMember* loadFromFile(std::ifstream& in);
};

// ─────────────────────────────────────────────
//  Derived Class: Fighter
//  Boosts combat power in missions and defense
// ─────────────────────────────────────────────
class Fighter : public GangMember {
public:
    Fighter(const std::string& name, int loyalty, int strength, int salary, int level);
    void displayInfo() const override;
    std::string specialAbility() const override;
    int getSpecialBonus() const override;
};

// ─────────────────────────────────────────────
//  Derived Class: Hacker
//  Lowers police heat and boosts hacking missions
// ─────────────────────────────────────────────
class Hacker : public GangMember {
public:
    Hacker(const std::string& name, int loyalty, int strength, int salary, int level);
    void displayInfo() const override;
    std::string specialAbility() const override;
    int getSpecialBonus() const override;
};

// ─────────────────────────────────────────────
//  Derived Class: Driver
//  Improves escape rates and racing missions
// ─────────────────────────────────────────────
class Driver : public GangMember {
public:
    Driver(const std::string& name, int loyalty, int strength, int salary, int level);
    void displayInfo() const override;
    std::string specialAbility() const override;
    int getSpecialBonus() const override;
};

// ─────────────────────────────────────────────
//  Derived Class: Smuggler
//  Increases profits and smuggling missions
// ─────────────────────────────────────────────
class Smuggler : public GangMember {
public:
    Smuggler(const std::string& name, int loyalty, int strength, int salary, int level);
    void displayInfo() const override;
    std::string specialAbility() const override;
    int getSpecialBonus() const override;
};

// ─────────────────────────────────────────────
//  Factory: Generate a random gang member
// ─────────────────────────────────────────────
GangMember* generateRandomMember(int qualityTier = 1);
