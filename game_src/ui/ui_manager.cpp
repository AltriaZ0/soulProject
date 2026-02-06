#include "ui/ui_manager.hpp"
#include "utils/constants.hpp" // Config::WindowWidth
#include <iomanip> // for string stream
#include <sstream>

UIManager::UIManager() : text_common_(font_) {
    printf("UIManager initializing...\n");
    // 初始化 Boss 血条样式
    boss_hp_bar_bg_.setSize({600.0f, 10.0f});
    boss_hp_bar_bg_.setFillColor(sf::Color(50, 50, 50)); 
    boss_hp_bar_bg_.setPosition({100.0f, 50.0f});        
    boss_hp_bar_bg_.setOutlineColor(sf::Color::White);
    boss_hp_bar_bg_.setOutlineThickness(2.0f);

    boss_hp_bar_fill_.setSize({600.0f, 10.0f}); 
    boss_hp_bar_fill_.setPosition({100.0f, 50.0f});
    boss_hp_bar_fill_.setFillColor(sf::Color::Red);
    printf("UIManager initialized.\n");
}

bool UIManager::loadFont(const std::string& fontPath) {
    printf("Font initializing...\n");
    if (!font_.openFromFile(fontPath)) {
        printf("Failed to load font!\n");
        return false;
    }
    printf("Font loaded from %s\n", fontPath.c_str());
    text_common_.setCharacterSize(24);
    text_common_.setFillColor(sf::Color::White);
    printf("Font initialized...\n");
    return true;
}

void UIManager::renderPlayingUI(sf::RenderWindow& window, const Player& player, const Boss& boss) {
    printf("UI initializing...\n");
    // 绘制残机
    std::stringstream ss;
    ss << "Player: ";
    int lives = 3; 
    for(int i=0; i<lives; ++i) ss << "* "; 
    
    text_common_.setString(ss.str());
    text_common_.setPosition({10.0f, static_cast<float>(Config::WindowHeight) - 40.0f}); 
    window.draw(text_common_);

    // 绘制 Boss 血条
    if (boss.hasActiveSpell()) { 
        float pct = boss.getCurrentSpellHealthPercent(); 
        
        window.draw(boss_hp_bar_bg_);
        
        boss_hp_bar_fill_.setSize({600.0f * pct, 10.0f});
        window.draw(boss_hp_bar_fill_);
        // 绘制符卡名字
        sf::Text spellName(font_); 
        spellName.setCharacterSize(24);
        spellName.setFillColor(sf::Color::White);
        spellName.setString(boss.getCurrentSpellName());
        spellName.setPosition({600.0f, 20.0f}); 

        sf::FloatRect bounds = spellName.getLocalBounds();
        spellName.setOrigin({bounds.size.x, 0.0f}); 
        window.draw(spellName);

        // 绘制倒计时
        sf::Text timerText(font_); // 同样需要构造时传 font
        timerText.setCharacterSize(24);
        timerText.setFillColor(sf::Color::White);
        
        std::stringstream ss_time;
        ss_time << std::fixed << std::setprecision(1) << boss.getCurrentSpellTimer();
        timerText.setString(ss_time.str());
        timerText.setPosition({720.0f, 50.0f});
        window.draw(timerText);
        printf("UI initialized...\n");
    }
}

void UIManager::renderStartScreen(sf::RenderWindow& window) {

    sf::RectangleShape overlay(sf::Vector2f(Config::WindowWidth, Config::WindowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    sf::Text title(font_); // 构造传入 font
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setString("TOUHOU DEMO");
    
    sf::FloatRect bounds = title.getLocalBounds();
    title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    title.setPosition({Config::WindowWidth / 2.0f, Config::WindowHeight / 3.0f});
    window.draw(title);

    sf::Text sub(font_); // 构造传入 font
    sub.setCharacterSize(24);
    sub.setFillColor(sf::Color::White);
    sub.setString("Press ENTER to Start");
    
    bounds = sub.getLocalBounds();
    sub.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    sub.setPosition({Config::WindowWidth / 2.0f, Config::WindowHeight / 2.0f});
    window.draw(sub);
}

void UIManager::renderPauseScreen(sf::RenderWindow& window) {
    sf::RectangleShape overlay(sf::Vector2f(Config::WindowWidth, Config::WindowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(overlay);

    sf::Text text(font_); // 构造传入 font
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);
    text.setString("PAUSED");

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    text.setPosition({Config::WindowWidth / 2.0f, Config::WindowHeight / 2.0f});
    window.draw(text);
}