@echo off
REM ============================================================
REM  Build Script — Underworld Empire Management System
REM  Compiles all .cpp files using g++ (MinGW)
REM ============================================================

echo.
echo  ╔══════════════════════════════════════════════╗
echo  ║   UNDERWORLD EMPIRE — BUILD SYSTEM          ║
echo  ╚══════════════════════════════════════════════╝
echo.

REM Create save directory
if not exist save mkdir save

echo  [BUILD] Compiling all source files...
echo.

g++ -std=c++14 -o UnderworldEmpire.exe ^
    main.cpp ^
    member.cpp ^
    territory.cpp ^
    mission.cpp ^
    police.cpp ^
    rivalgang.cpp ^
    blackmarket.cpp ^
    news.cpp ^
    player.cpp ^
    game.cpp ^
    game_menus.cpp ^
    -static -O2

if %errorlevel% neq 0 (
    echo.
    echo  [ERROR] Build failed! Check errors above.
    pause
    exit /b 1
)

echo.
echo  [OK] Build successful! Output: UnderworldEmpire.exe
echo.
echo  Run with: UnderworldEmpire.exe
echo.
pause
