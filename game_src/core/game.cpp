#include "core/game.hpp"
#include "utils/constants.hpp"
#include <optional>

Game::Game() : window_(sf::VideoMode({Config::WindowWidth, Config::WindowHeight}), "Touhou Project Refactored") {
    if (!ui_manager_.loadFont("assets/fonts/homo.ttf")) {
        printf("Failed to load font!\n");
    }
    window_.setFramerateLimit(60);
}

void Game::run() {
    printf("Game initialized...\n");
    sf::Clock clock;
    while (window_.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window_.close();
        }

        else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (state_ == GameState::StartScreen) {
                if (keyEvent->code == sf::Keyboard::Key::Enter) {
                    printf("game start! \n");
                    state_ = GameState::Playing;
                }
            }
            else if (state_ == GameState::Playing) {
                if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    printf("game paused! \n");
                    state_ = GameState::Paused;
                }
                if (keyEvent->code == sf::Keyboard::Key::F2) {
                    printf("next phase! \n");
                    boss_.skipCurrentPhase();
                }
            }
            else if (state_ == GameState::Paused) {
                if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    printf("next continue! \n");
                    state_ = GameState::Playing;
                }
            }
        }
    }
}

void Game::update(float dt) {
    if (state_ != GameState::Playing) return;

    player_.update(dt, bullet_manager_);
    boss_.update(dt, bullet_manager_, player_.getPosition());
    bullet_manager_.update(dt);
    checkCollisions();
}


void Game::checkCollisions() {
    sf::Vector2f p_pos = player_.getPosition();
    float p_radius = player_.getHitboxRadius();

    const auto& bullets = bullet_manager_.getBullets();
    
    for (const auto& bullet : bullets) {
        if (bullet->getType() == EntityType::BossBullet) {
            sf::Vector2f b_pos = bullet->getPosition();
            float dx = p_pos.x - b_pos.x;
            float dy = p_pos.y - b_pos.y;
            float dist_sq = dx * dx + dy * dy;
            float radius_sum = p_radius + bullet->getHitboxRadius() * 0.5; // 缩小一点判定半径，后续可设置成常数

            if (dist_sq < radius_sum * radius_sum) {
                if(player_.takeDamage()){
                    player_.set_invincible(2000); // 2秒无敌
                    bullet_manager_.clear(); // 清空所有子弹
                };
                if(player_.isDead()) {
                    resetGame();
                }
                break;
            }
        }
        // 玩家子弹判定
        if (bullet->isActive() && bullet->getType() == EntityType::PlayerBullet) {
            sf::Vector2f b_pos = bullet->getPosition();
            sf::Vector2f boss_pos = boss_.getPosition();
            
            float dx = boss_pos.x - b_pos.x;
            float dy = boss_pos.y - b_pos.y;
            if (dx*dx + dy*dy < 34*34) { 
                bullet->deactivate(); // 销毁子弹
                boss_.takeDamage(10.0f); // Boss 扣血
            }
        }
    }
}

void Game::resetGame() {
    bullet_manager_.clear();
    player_ = Player();
    boss_ = Boss();
}

void Game::render() {
    window_.clear(sf::Color(20, 20, 40));

    // 绘制游戏物体
    boss_.render(window_);
    player_.render(window_);
    bullet_manager_.render(window_);

    // 绘制 UI 层
    if (state_ == GameState::Playing) {
        ui_manager_.renderPlayingUI(window_, player_, boss_);
    }
    else if (state_ == GameState::StartScreen) {
        ui_manager_.renderStartScreen(window_);
    }
    else if (state_ == GameState::Paused) {
        ui_manager_.renderPlayingUI(window_, player_, boss_);
        ui_manager_.renderPauseScreen(window_);
    }

    window_.display();
}
