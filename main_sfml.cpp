// main_sfml.cpp — SFML 3 UI with hand sprites, elimination, auto CPU turns, win banner, and right-click unselect.
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <array>
#include <string>
#include <vector>
#include <random>
#include <iostream>
#include "master.h" // for maxHandValue

// ---------- helpers ----------
static void center_text_h(sf::Text& t, float winWidth, float y) {
    sf::FloatRect b = t.getLocalBounds();
    float x = (winWidth - b.size.x) * 0.5f - b.position.x;
    t.setPosition({x, y});
}

static int add_with_rules(int dst, int src) {
    if (dst == 0 || src == 0) return dst;
    int sum = dst + src;
    if (sum == maxHandValue) return 0;
    if (sum > maxHandValue) return sum - maxHandValue;
    return sum;
}

// ---------- textures ----------
struct TextureSet {
    std::array<sf::Texture,5> left;
    std::array<sf::Texture,5> right;
    sf::Texture elimL;
    sf::Texture elimR;
};

// ---------- one hand visual ----------
struct HandVis {
    bool isLeft = true;
    float targetHeight = 200.f;
    const TextureSet* tex = nullptr;
    sf::Sprite sprite;

    HandVis(const TextureSet& set, bool left, int startVal, float targetH)
        : isLeft(left), targetHeight(targetH), tex(&set),
          sprite(getTextureFor(set, left, startVal)) {
        setValue(startVal);
    }

    static const sf::Texture& getTextureFor(const TextureSet& set, bool isLeft, int value) {
        if (value <= 0)
            return isLeft ? set.elimL : set.elimR;
        return isLeft ? set.left[value - 1] : set.right[value - 1];
    }

    void applyScaleFor(const sf::Texture& t) {
        sf::Vector2u sz = t.getSize();
        if (sz.y == 0) return;
        float s = targetHeight / static_cast<float>(sz.y);
        sprite.setScale({s, s});
    }

    void setValue(int v) {
        const sf::Texture& t = getTextureFor(*tex, isLeft, v);
        sprite.setTexture(t);
        applyScaleFor(t);
    }

    sf::FloatRect bounds() const { return sprite.getGlobalBounds(); }
};

// ---------- simple button ----------
struct Button {
    sf::RectangleShape box;
    sf::Text label;
    Button(sf::Font& f, const std::string& text, sf::Vector2f pos, sf::Vector2f size)
        : box(size), label(f) {
        box.setPosition(pos);
        box.setFillColor(sf::Color(50, 60, 75));
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color::White);
        label.setString(text);
        label.setCharacterSize(20);
        auto b = label.getLocalBounds();
        float x = pos.x + (size.x - b.size.x) * 0.5f - b.position.x;
        float y = pos.y + (size.y - b.size.y) * 0.5f - b.position.y;
        label.setPosition({x, y});
    }
    bool hit(sf::Vector2f p) const { return box.getGlobalBounds().contains(p); }
    void draw(sf::RenderWindow& w) const { w.draw(box); w.draw(label); }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({1100u, 700u}), "Finger Game (SFML 3)");

    // font
    sf::Font font;
    if (!font.openFromFile("assets/DejaVuSans.ttf")) {
        std::cerr << "Missing font: assets/DejaVuSans.ttf\n";
        return 1;
    }

    // load textures
    TextureSet tex;
    auto load_or_warn = [](sf::Texture& t, const std::string& path) {
        if (!t.loadFromFile(path)) std::cerr << "Missing sprite: " << path << "\n";
        t.setSmooth(true);
    };
    for (int i = 1; i <= 5; ++i) {
        load_or_warn(tex.left[i - 1], "assets/sprites/" + std::to_string(i) + "L.png");
        load_or_warn(tex.right[i - 1], "assets/sprites/" + std::to_string(i) + "R.png");
    }
    load_or_warn(tex.elimL, "assets/sprites/ElimL.png");
    load_or_warn(tex.elimR, "assets/sprites/ElimR.png");

    // UI text
    sf::Text title(font);
    title.setString("Finger Game");
    title.setCharacterSize(36);
    center_text_h(title, static_cast<float>(window.getSize().x), 18.f);

    sf::Text hint(font);
    hint.setString("Click YOUR hand, then click CPU hand. Turns alternate automatically.");
    hint.setCharacterSize(18);
    center_text_h(hint, static_cast<float>(window.getSize().x), 58.f);

    sf::Text banner(font);
    banner.setCharacterSize(26);
    banner.setFillColor(sf::Color(220, 220, 220));
    banner.setString("");
    center_text_h(banner, static_cast<float>(window.getSize().x),
                  static_cast<float>(window.getSize().y) - 36.f);

    // state values
    int userL = 1, userR = 1, cpuL = 1, cpuR = 1;
    bool playersTurn = true;
    bool gameOver = false;

    // visuals
    HandVis visUserL(tex, true, userL, 230.f);
    HandVis visUserR(tex, false, userR, 230.f);
    HandVis visCpuL(tex, true, cpuL, 200.f);
    HandVis visCpuR(tex, false, cpuR, 200.f);

    auto layout = [&] {
        float W = static_cast<float>(window.getSize().x);
        float H = static_cast<float>(window.getSize().y);
        float gap = 40.f;

        float totalUser = visUserL.bounds().size.x + gap + visUserR.bounds().size.x;
        float startX = (W - totalUser) / 2.f;
        float yUser = H - visUserL.bounds().size.y - 50.f;
        visUserL.sprite.setPosition({startX, yUser});
        visUserR.sprite.setPosition({startX + visUserL.bounds().size.x + gap, yUser});

        float totalCpu = visCpuL.bounds().size.x + gap + visCpuR.bounds().size.x;
        float startXC = (W - totalCpu) / 2.f;
        float yCpu = 120.f;
        visCpuL.sprite.setPosition({startXC, yCpu});
        visCpuR.sprite.setPosition({startXC + visCpuL.bounds().size.x + gap, yCpu});

        center_text_h(banner, W, H - 34.f);
    };

    auto refreshVisuals = [&] {
        visUserL.setValue(userL);
        visUserR.setValue(userR);
        visCpuL.setValue(cpuL);
        visCpuR.setValue(cpuR);
        layout();
    };
    refreshVisuals();

    Button playAgain(font, "Play Again",
                     {20.f, static_cast<float>(window.getSize().y) - 56.f},
                     {160.f, 40.f});

    std::mt19937 rng{std::random_device{}()};

    enum class Sel { None, UL, UR };
    Sel selected = Sel::None;

    auto userAlive = [&] { return userL > 0 || userR > 0; };
    auto cpuAlive = [&] { return cpuL > 0 || cpuR > 0; };

    auto setGameOver = [&](const std::string& msg) {
        gameOver = true;
        banner.setString(msg + "  Click 'Play Again' to restart.");
        center_text_h(banner, static_cast<float>(window.getSize().x),
                      static_cast<float>(window.getSize().y) - 34.f);
    };

    auto checkGameOver = [&] {
        if (!cpuAlive()) setGameOver("You Win!");
        if (!userAlive()) setGameOver("Computer Wins!");
    };

    auto cpuTurn = [&] {
        if (gameOver || !cpuAlive() || !userAlive()) return;

        char c = 'L', u = 'L';
        auto tryFinish = [&](int sL, int sR, int tL, int tR, char& outC, char& outU) -> bool {
            if (sL > 0 && tL > 0 && sL + tL == maxHandValue) { outC = 'L'; outU = 'L'; return true; }
            if (sL > 0 && tR > 0 && sL + tR == maxHandValue) { outC = 'L'; outU = 'R'; return true; }
            if (sR > 0 && tL > 0 && sR + tL == maxHandValue) { outC = 'R'; outU = 'L'; return true; }
            if (sR > 0 && tR > 0 && sR + tR == maxHandValue) { outC = 'R'; outU = 'R'; return true; }
            return false;
        };

        if (!tryFinish(cpuL, cpuR, userL, userR, c, u)) {
            std::vector<std::pair<char, char>> opts;
            if (cpuL > 0 && userL > 0) opts.emplace_back('L', 'L');
            if (cpuL > 0 && userR > 0) opts.emplace_back('L', 'R');
            if (cpuR > 0 && userL > 0) opts.emplace_back('R', 'L');
            if (cpuR > 0 && userR > 0) opts.emplace_back('R', 'R');

            if (!opts.empty()) {
                std::uniform_int_distribution<int> d(0, opts.size() - 1);
                auto pick = opts[d(rng)];
                c = pick.first;
                u = pick.second;
            }
        }

        if (c == 'L' && u == 'L') userL = add_with_rules(userL, cpuL);
        if (c == 'L' && u == 'R') userR = add_with_rules(userR, cpuL);
        if (c == 'R' && u == 'L') userL = add_with_rules(userL, cpuR);
        if (c == 'R' && u == 'R') userR = add_with_rules(userR, cpuR);

        refreshVisuals();
        checkGameOver();
    };

    auto resetGame = [&] {
        userL = userR = cpuL = cpuR = 1;
        playersTurn = true;
        gameOver = false;
        banner.setString("");
        selected = Sel::None;
        refreshVisuals();
    };

    // -------------- MAIN LOOP --------------
    while (window.isOpen()) {
        while (auto ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) window.close();

            // Mouse button press
            if (auto m = ev->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f p = (sf::Vector2f)sf::Mouse::getPosition(window);

                // ✅ RIGHT-CLICK to cancel selected hand
                if (m->button == sf::Mouse::Button::Right) {
                    if (selected != Sel::None) {
                        selected = Sel::None;
                    }
                    continue;
                }

                // Only handle LEFT clicks from now on:
                if (m->button != sf::Mouse::Button::Left)
                    continue;

                // "Play Again"
                if (playAgain.hit(p)) { resetGame(); continue; }

                if (gameOver) continue;
                if (!playersTurn || !cpuAlive() || !userAlive()) continue;

                // Select your hand
                if (visUserL.bounds().contains(p) && userL > 0) { selected = Sel::UL; continue; }
                if (visUserR.bounds().contains(p) && userR > 0) { selected = Sel::UR; continue; }

                // Attack CPU hand
                if (selected != Sel::None) {
                    bool applied = false;

                    if (visCpuL.bounds().contains(p) && cpuL > 0) {
                        cpuL = add_with_rules(cpuL, (selected == Sel::UL ? userL : userR));
                        applied = true;
                    }
                    else if (visCpuR.bounds().contains(p) && cpuR > 0) {
                        cpuR = add_with_rules(cpuR, (selected == Sel::UL ? userL : userR));
                        applied = true;
                    }

                    if (applied) {
                        selected = Sel::None;
                        refreshVisuals();
                        checkGameOver();

                        if (!gameOver) {
                            playersTurn = false;
                            cpuTurn();
                            playersTurn = true;
                        }
                    }
                }
            }
        }

        // drawing
        window.clear(sf::Color(20, 22, 30));
        window.draw(title);
        window.draw(hint);
        window.draw(visCpuL.sprite);
        window.draw(visCpuR.sprite);
        window.draw(visUserL.sprite);
        window.draw(visUserR.sprite);
        playAgain.draw(window);
        if (gameOver) window.draw(banner);
        window.display();
    }

    return 0;
}
