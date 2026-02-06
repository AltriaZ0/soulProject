#pragma once
#include <SFML/Graphics.hpp>
#include "entities/player.hpp"
#include "entities/boss.hpp"
#include "core/bullet_manager.hpp"
#include "ui/ui_manager.hpp" // 包含 UI 管理器
enum class GameState {
    StartScreen, // 标题画面
    Playing,     // 游戏中
    Paused,      // 暂停
    GameOver     // 游戏结束
};

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void checkCollisions();
    void resetGame();

    sf::RenderWindow window_;
    Player player_;
    Boss boss_;
    BulletManager bullet_manager_;
    GameState state_ = GameState::StartScreen;
    UIManager ui_manager_;
    
};