// ============================================================
// utils.h — Terminal Utilities & Visual Engine
// Underworld Empire Management System
// Compatible with GCC 6.3+ (C++14)
// ============================================================
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <functional>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

// ─────────────────────────────────────────────
//  ANSI Color Constants (static to avoid ODR)
// ─────────────────────────────────────────────
namespace Color {
    static const std::string RESET   = "\033[0m";
    static const std::string BOLD    = "\033[1m";
    static const std::string DIM     = "\033[2m";
    static const std::string ITALIC  = "\033[3m";
    static const std::string UNDERLINE = "\033[4m";
    static const std::string BLINK   = "\033[5m";
    static const std::string BLACK   = "\033[30m";
    static const std::string RED     = "\033[31m";
    static const std::string GREEN   = "\033[32m";
    static const std::string YELLOW  = "\033[33m";
    static const std::string BLUE    = "\033[34m";
    static const std::string MAGENTA = "\033[35m";
    static const std::string CYAN    = "\033[36m";
    static const std::string WHITE   = "\033[37m";
    static const std::string BRIGHT_RED     = "\033[91m";
    static const std::string BRIGHT_GREEN   = "\033[92m";
    static const std::string BRIGHT_YELLOW  = "\033[93m";
    static const std::string BRIGHT_BLUE    = "\033[94m";
    static const std::string BRIGHT_MAGENTA = "\033[95m";
    static const std::string BRIGHT_CYAN    = "\033[96m";
    static const std::string BRIGHT_WHITE   = "\033[97m";
    static const std::string BG_BLACK   = "\033[40m";
    static const std::string BG_RED     = "\033[41m";
    static const std::string BG_GREEN   = "\033[42m";
    static const std::string BG_YELLOW  = "\033[43m";
    static const std::string BG_BLUE    = "\033[44m";
    static const std::string BG_MAGENTA = "\033[45m";
    static const std::string BG_CYAN    = "\033[46m";
    static const std::string BG_WHITE   = "\033[47m";
}

// ─────────────────────────────────────────────
//  Global Random Engine
// ─────────────────────────────────────────────
inline std::mt19937& getRandomEngine() {
    static std::mt19937 engine(static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()));
    return engine;
}

inline int getRandomInt(int minVal, int maxVal) {
    std::uniform_int_distribution<int> dist(minVal, maxVal);
    return dist(getRandomEngine());
}

inline bool randomChance(int percentChance) {
    return getRandomInt(1, 100) <= percentChance;
}

// ─────────────────────────────────────────────
//  Terminal Control (Windows-native delay)
// ─────────────────────────────────────────────
inline void enableANSI() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

inline void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

inline void delay(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
#endif
}

// ─────────────────────────────────────────────
//  Text Animation Functions
// ─────────────────────────────────────────────
inline void typeEffect(const std::string& text, int delayMs = 25) {
    for (size_t i = 0; i < text.size(); i++) {
        std::cout << text[i] << std::flush;
        delay(delayMs);
    }
}

inline void typeEffectLn(const std::string& text, int delayMs = 25) {
    typeEffect(text, delayMs);
    std::cout << std::endl;
}

inline void slowPrint(const std::string& text, int delayMs = 40) {
    std::cout << text << std::flush;
    delay(delayMs);
}

inline void flashText(const std::string& text, const std::string& color, int times = 3) {
    for (int i = 0; i < times; i++) {
        std::cout << "\r" << color << Color::BOLD << text << Color::RESET << std::flush;
        delay(200);
        std::cout << "\r" << std::string(text.size(), ' ') << std::flush;
        delay(100);
    }
    std::cout << "\r" << color << Color::BOLD << text << Color::RESET << std::endl;
}

// ─────────────────────────────────────────────
//  UI Drawing Functions
// ─────────────────────────────────────────────
inline void printBorder(const std::string& color, int len = 62) {
    std::cout << color;
    for (int i = 0; i < len; i++) std::cout << "=";
    std::cout << Color::RESET << std::endl;
}

inline void printDoubleBorder(int len = 62) {
    std::cout << Color::CYAN << "+";
    for (int i = 0; i < len - 2; i++) std::cout << "=";
    std::cout << "+" << Color::RESET << std::endl;
}

inline void printDoubleBorderBottom(int len = 62) {
    std::cout << Color::CYAN << "+";
    for (int i = 0; i < len - 2; i++) std::cout << "=";
    std::cout << "+" << Color::RESET << std::endl;
}

inline void printDoubleBorderMid(int len = 62) {
    std::cout << Color::CYAN << "+";
    for (int i = 0; i < len - 2; i++) std::cout << "=";
    std::cout << "+" << Color::RESET << std::endl;
}

inline void printBoxLine(const std::string& text, int width = 62) {
    // Count visible length (skip ANSI escape sequences)
    int textLen = 0;
    bool inEscape = false;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '\033') inEscape = true;
        if (!inEscape) textLen++;
        if (inEscape && ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z' && text[i] != '\033' && i > 0 && text[i-1] != '\033'))) inEscape = false;
    }
    int padding = width - 4 - textLen;
    if (padding < 0) padding = 0;
    std::cout << Color::CYAN << "| " << Color::RESET
              << text << std::string(padding, ' ')
              << Color::CYAN << " |" << Color::RESET << std::endl;
}

inline void printHeader(const std::string& title) {
    printDoubleBorder();
    printBoxLine(Color::BOLD + Color::BRIGHT_CYAN + title + Color::RESET);
    printDoubleBorderBottom();
}

inline void printSectionHeader(const std::string& title) {
    std::cout << std::endl;
    std::cout << Color::CYAN << "  +-- " << Color::BOLD << Color::BRIGHT_CYAN
              << title << Color::RESET << Color::CYAN << " -";
    int remaining = 55 - (int)title.size();
    for (int i = 0; i < remaining && i < 60; i++) std::cout << "-";
    std::cout << "+" << Color::RESET << std::endl;
}

inline void printSectionFooter() {
    std::cout << Color::CYAN << "  +";
    for (int i = 0; i < 59; i++) std::cout << "-";
    std::cout << "+" << Color::RESET << std::endl;
}

// ─────────────────────────────────────────────
//  Progress Bar
// ─────────────────────────────────────────────
inline void printProgressBar(const std::string& label, int value, int maxVal,
                             const std::string& color, int barWidth = 20) {
    int filled = (maxVal > 0) ? (value * barWidth) / maxVal : 0;
    if (filled > barWidth) filled = barWidth;
    if (filled < 0) filled = 0;
    std::cout << "  " << Color::DIM << std::setw(12) << std::left << label
              << Color::RESET << " " << color;
    for (int i = 0; i < barWidth; i++) {
        std::cout << (i < filled ? "#" : ".");
    }
    std::cout << Color::RESET << " " << Color::BOLD << value << Color::RESET;
    if (maxVal > 0) std::cout << Color::DIM << "/" << maxVal << Color::RESET;
    std::cout << std::endl;
}

// ─────────────────────────────────────────────
//  Input Helpers
// ─────────────────────────────────────────────
inline void waitForEnter() {
    std::cout << std::endl << Color::DIM << "  Press [ENTER] to continue..."
              << Color::RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

inline int getMenuChoice(int minVal, int maxVal) {
    int choice = -1;
    std::cout << std::endl << Color::CYAN << "  > " << Color::BRIGHT_CYAN
              << "Enter choice: " << Color::RESET;
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return -1;
    }
    if (choice < minVal || choice > maxVal) return -1;
    return choice;
}

inline std::string getStringInput(const std::string& prompt) {
    std::string input;
    std::cout << Color::CYAN << "  > " << Color::BRIGHT_CYAN
              << prompt << Color::RESET;
    std::cin.ignore(10000, '\n');
    std::getline(std::cin, input);
    return input;
}

// ─────────────────────────────────────────────
//  Formatting Helpers
// ─────────────────────────────────────────────
inline std::string formatMoney(int amount) {
    std::string str = std::to_string(std::abs(amount));
    int insertPos = (int)str.length() - 3;
    while (insertPos > 0) {
        str.insert(insertPos, ",");
        insertPos -= 3;
    }
    return (amount < 0 ? "-$" : "$") + str;
}

inline std::string getRankColor(const std::string& rank) {
    if (rank == "Kingpin") return Color::BRIGHT_MAGENTA + Color::BOLD;
    if (rank == "Boss") return Color::BRIGHT_RED + Color::BOLD;
    if (rank == "Enforcer") return Color::BRIGHT_YELLOW;
    if (rank == "Hustler") return Color::BRIGHT_GREEN;
    return Color::DIM;
}

inline std::string getHeatColor(int heat) {
    if (heat >= 80) return Color::BRIGHT_RED + Color::BOLD;
    if (heat >= 60) return Color::RED;
    if (heat >= 40) return Color::YELLOW;
    if (heat >= 20) return Color::GREEN;
    return Color::BRIGHT_GREEN;
}

// ─────────────────────────────────────────────
//  Boot Sequence Item
// ─────────────────────────────────────────────
inline void bootItem(const std::string& message, int dotDelay = 80, int resultDelay = 300) {
    std::cout << Color::CYAN << "  [" << Color::BRIGHT_CYAN << "SYSTEM"
              << Color::CYAN << "] " << Color::RESET;
    typeEffect(message, 18);
    for (int i = 0; i < 3; i++) {
        std::cout << "." << std::flush;
        delay(dotDelay);
    }
    int pad = 48 - (int)message.size();
    if (pad < 1) pad = 1;
    std::cout << std::string(pad, ' ');
    delay(resultDelay);
    std::cout << Color::BOLD << Color::BRIGHT_GREEN << "[OK]" << Color::RESET << std::endl;
}

// ─────────────────────────────────────────────
//  System Alert Messages
// ─────────────────────────────────────────────
inline void printSuccess(const std::string& msg) {
    std::cout << Color::BRIGHT_GREEN << "  [+] " << msg << Color::RESET << std::endl;
}
inline void printError(const std::string& msg) {
    std::cout << Color::BRIGHT_RED << "  [!] " << msg << Color::RESET << std::endl;
}
inline void printWarning(const std::string& msg) {
    std::cout << Color::BRIGHT_YELLOW << "  [!] " << msg << Color::RESET << std::endl;
}
inline void printInfo(const std::string& msg) {
    std::cout << Color::BRIGHT_CYAN << "  [>] " << msg << Color::RESET << std::endl;
}

inline void printAlert(const std::string& title, const std::string& msg,
                       const std::string& color) {
    std::cout << std::endl;
    std::cout << color << Color::BOLD;
    std::cout << "  +======================================================+" << std::endl;
    int titlePad = 54 - (int)title.size();
    if (titlePad < 0) titlePad = 0;
    std::cout << "  |  " << title << std::string(titlePad, ' ') << "|" << std::endl;
    std::cout << "  +======================================================+" << std::endl;
    int lineLen = 52;
    for (size_t i = 0; i < msg.size(); i += lineLen) {
        std::string line = msg.substr(i, lineLen);
        int linePad = lineLen - (int)line.size();
        if (linePad < 0) linePad = 0;
        std::cout << "  |  " << Color::RESET << color << line
                  << std::string(linePad, ' ') << Color::BOLD << "|" << std::endl;
    }
    std::cout << "  +======================================================+"
              << Color::RESET << std::endl;
}
