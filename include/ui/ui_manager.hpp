#pragma once
#include <SFML/Graphics.hpp>
#include "entities/player.hpp"
#include "entities/boss.hpp"

class UIManager {
public:
    UIManager();

    // 初始化字体
    bool loadFont(const std::string& fontPath);

    // 渲染不同状态的 UI
    void renderPlayingUI(sf::RenderWindow& window, const Player& player, const Boss& boss);
    void renderStartScreen(sf::RenderWindow& window);
    void renderPauseScreen(sf::RenderWindow& window);
    void renderGameOverScreen(sf::RenderWindow& window, bool win);

private:
    sf::Font font_;
    
    // 缓存一些常用的 UI 元素，避免每帧重复创建
    sf::Text text_common_; 
    sf::RectangleShape boss_hp_bar_bg_;
    sf::RectangleShape boss_hp_bar_fill_;
};