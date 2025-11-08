// main_sfml.cpp — SFML 3.0.x UI wired to your existing game logic
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "master.h"

// Helpers: check if either side still has hands
static bool playerHasHands() { return (User.getLeft() != 0) || (User.getRight() != 0); }
static bool cpuHasHands()    { return (CPU.getLeft()  != 0) || (CPU.getRight()  != 0); }

// Apply eliminations with wrap/rollover rule already handled in your set* methods.
// We only eliminate when exactly == maxHandValue (same as your console loop).
static void applyEliminations() {
    if (User.getLeft() == maxHandValue)  { std::cout << "Your Left Hand Has Been Eliminated!\n";  User.setLeft(0); }
    if (User.getRight() == maxHandValue) { std::cout << "Your Right Hand Has Been Eliminated!\n"; User.setRight(0); }
    if (CPU.getLeft() == maxHandValue)   { std::cout << "Enemy Left Hand Eliminated!\n";          CPU.setLeft(0); }
    if (CPU.getRight() == maxHandValue)  { std::cout << "Enemy Right Hand Eliminated!\n";         CPU.setRight(0); }
}

int main() {
    // Seed RNG once
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Window
    sf::RenderWindow window(sf::VideoMode({1000u, 700u}), "Finger Game (SFML 3)");

    // Font (put a TTF at assets/DejaVuSans.ttf next to your exe)
    sf::Font font;
    if (!font.openFromFile("assets/DejaVuSans.ttf")) {
        std::cerr << "Failed to load font (assets/DejaVuSans.ttf)\n";
        return 1;
    }

    // Title
    sf::Text title(font);
    title.setString("Finger Game");
    title.setCharacterSize(36);

    // Controls
    sf::Text hint(font);
    hint.setCharacterSize(18);
    hint.setString(
        "Controls:\n"
        "  A = choose Your Left    | D = choose Your Right\n"
        "  J = choose Enemy Left   | L = choose Enemy Right\n"
        "  Enter = Confirm your move (then CPU plays)\n"
        "  Esc = Quit"
    );

    auto centerHoriz = [](sf::Text& t, float windowWidth, float y) {
        sf::FloatRect b = t.getLocalBounds();
        float x = (windowWidth - b.size.x) * 0.5f - b.position.x;
        t.setPosition({x, y});
    };

    centerHoriz(title, static_cast<float>(window.getSize().x), 24.f);
    centerHoriz(hint,  static_cast<float>(window.getSize().x), 80.f);

    // HUD that shows hands and current selections
    sf::Text hud(font);
    hud.setCharacterSize(28);

    // Selection state
    enum class MySel { None, L, R };
    enum class EnSel { None, L, R };
    MySel mySel = MySel::None;
    EnSel enSel = EnSel::None;

    auto mySelStr = [&]() {
        switch (mySel) { case MySel::L: return "Your Left"; case MySel::R: return "Your Right"; default: return "(choose A/D)"; }
    };
    auto enSelStr = [&]() {
        switch (enSel) { case EnSel::L: return "Enemy Left"; case EnSel::R: return "Enemy Right"; default: return "(choose J/L)"; }
    };

    auto updateHud = [&] {
        std::string s;
        s += "=== Current Hands ===\n";
        s += "You  -> L:" + std::to_string(User.getLeft()) + " R:" + std::to_string(User.getRight()) + "\n";
        s += "CPU  -> L:" + std::to_string(CPU.getLeft())  + " R:" + std::to_string(CPU.getRight())  + "\n";
        s += "=====================\n\n";
        s += "Selected: " + std::string(mySelStr()) + "  ->  " + std::string(enSelStr()) + "\n";
        s += "(Enter to confirm)\n";
        hud.setString(s);
        centerHoriz(hud, static_cast<float>(window.getSize().x), 180.f);
    };
    updateHud();

    // Text to show status/winner
    sf::Text status(font);
    status.setCharacterSize(22);
    status.setFillColor(sf::Color(200, 220, 255));
    auto setStatus = [&](const std::string& s) {
        status.setString(s);
        centerHoriz(status, static_cast<float>(window.getSize().x), 420.f);
    };
    setStatus("Make a selection and press Enter.");

    // Simple function that executes one player move based on selection
    auto doPlayerMove = [&]() -> bool {
        if (!playerHasHands()) return false;
        if (mySel == MySel::None || enSel == EnSel::None) return false;

        // Disallow using eliminated hands
        if (mySel == MySel::L && User.getLeft() == 0)  { setStatus("Your Left is eliminated. Pick a different hand."); return false; }
        if (mySel == MySel::R && User.getRight() == 0) { setStatus("Your Right is eliminated. Pick a different hand."); return false; }
        if (enSel == EnSel::L && CPU.getLeft() == 0)   { setStatus("Enemy Left is eliminated. Pick a different target."); return false; }
        if (enSel == EnSel::R && CPU.getRight() == 0)  { setStatus("Enemy Right is eliminated. Pick a different target."); return false; }

        // Call your existing Player methods to keep logic identical
        if (mySel == MySel::L && enSel == EnSel::L)      User.playerl2l();
        else if (mySel == MySel::L && enSel == EnSel::R) User.playerl2r();
        else if (mySel == MySel::R && enSel == EnSel::R) User.playerr2r();
        else if (mySel == MySel::R && enSel == EnSel::L) User.playerr2l();

        return true;
    };

    // Main loop
    while (window.isOpen()) {
        while (auto ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) window.close();

            if (auto key = ev->getIf<sf::Event::KeyPressed>()) {
                using Sc = sf::Keyboard::Scancode;
                switch (key->scancode) {
                    case Sc::Escape: window.close(); break;

                    // Choose player hand
                    case Sc::A: mySel = MySel::L; setStatus("Selected: Your Left. Now pick Enemy hand (J/L), then Enter."); updateHud(); break;
                    case Sc::D: mySel = MySel::R; setStatus("Selected: Your Right. Now pick Enemy hand (J/L), then Enter."); updateHud(); break;

                    // Choose enemy hand
                    case Sc::J: enSel = EnSel::L; setStatus("Target: Enemy Left. Press Enter to confirm."); updateHud(); break;
                    case Sc::L: enSel = EnSel::R; setStatus("Target: Enemy Right. Press Enter to confirm."); updateHud(); break;

                    // Confirm turn
                    case Sc::Enter: {
                        // Player move
                        bool moved = doPlayerMove();
                        if (!moved) break;

                        // Eliminate any 5s created by player's move
                        applyEliminations();
                        updateHud();

                        // Win check after player's move
                        if (!cpuHasHands()) {
                            setStatus("You Win!");
                            updateHud();
                            break;
                        }

                        // CPU move (this was the missing step)
                        CPU.cpuTurn();

                        // Eliminate any 5s created by CPU move
                        applyEliminations();
                        updateHud();

                        // Win check after CPU
                        if (!playerHasHands()) {
                            setStatus("Computer Wins!");
                        } else {
                            setStatus("Your turn: pick A/D, then J/L, Enter.");
                        }

                        // Reset selection for next turn
                        mySel = MySel::None;
                        enSel = EnSel::None;
                        updateHud();
                        break;
                    }

                    default: break;
                }
            }
        }

        window.clear();
        window.draw(title);
        window.draw(hint);
        window.draw(hud);
        window.draw(status);
        window.display();
    }

    return 0;
}
