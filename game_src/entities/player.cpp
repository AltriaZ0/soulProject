#include "entities/player.hpp"
#include "entities/characters/concrete_characters.hpp"
#include "core/bullet_manager.hpp" 
#include "utils/constants.hpp"
#include <SFML/Window/Keyboard.hpp>

namespace {
    const float kPlayerSpeed = 450.0f;
    const float kFocusSpeedFactor = 0.4f;
    const float kPlayerBulletSpeed = 1200.0f;
    const float kHitboxRadius = 3.0f;
}

Player::Player() : shoot_timer_(0.0f) {
    position_ = {400.0f, 800.0f};
    // 默认选择
    switchCharacter(CharacterType::Marisa);
    sprite_.setSize({32.0f, 48.0f});
    sprite_.setOrigin({16.0f, 24.0f});
    sprite_.setFillColor(sf::Color(200, 50, 50));
    sprite_.setPosition(position_);

    hitbox_.setRadius(kHitboxRadius);
    hitbox_.setOrigin({kHitboxRadius, kHitboxRadius});
    hitbox_.setFillColor(sf::Color::White);
    hitbox_.setOutlineColor(sf::Color::Red);
    hitbox_.setOutlineThickness(1.0f);
}

// 核心：切换机体时的初始化逻辑
void Player::switchCharacter(CharacterType type) {
    // 1. 创建具体的机体对象
    if (type == CharacterType::Reimu) {
        character_ = std::make_unique<Reimu>();
    } else {
        character_ = std::make_unique<Marisa>();
    }

    // 2. 获取并缓存该机体的属性
    current_stats_ = character_->getStats();

    // 3. 应用属性到 Player 身上
    // 应用生命值 (如果是游戏开始时)
    life_ = current_stats_.max_life; 
    
    // 应用外观
    sprite_.setSize({32.0f, 48.0f});
    sprite_.setOrigin({16.0f, 24.0f});
    sprite_.setFillColor(current_stats_.theme_color);
    sprite_.setPosition(position_);
    
    printf("Switched Character! Speed: %.1f, Lives: %d\n", 
           current_stats_.base_speed, current_stats_.max_life);
}

// 这是一个空实现，因为我们需要带 bullet_manager 的版本
void Player::update(float dt) {} 

void Player::update(float dt, BulletManager& bullet_manager) {
    handleInput(dt, bullet_manager);
    sprite_.setPosition(position_);
    hitbox_.setPosition(position_);
    if (invincible_timer_ > 0.0f) {
        invincible_timer_ -= dt * 1000.0f; // 转换为毫秒
        if (invincible_timer_ <= 0.0f) {
            printf("Player is no longer invincible!\n");
        }
    }
}

void Player::render(sf::RenderWindow& window) {
    window.draw(sprite_);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
        window.draw(hitbox_);
    }
}

bool Player::takeDamage() {
    if(isInvincible()){
        printf("Player is invincible, no damage taken! \n");
        return false;
    }
    if ( life_ > 0) {
        --life_;
        printf("Player took damage! Remaining life: %d\n", life_);
        return true;
    }
    return false;
}

bool Player::isDead() const {
    if(life_ <= 0) {
        printf("Player is dead!\n");
    }
    return life_ <= 0;
}

bool Player::isInvincible() const { return invincible_timer_ > 0.0f; }

bool Player::set_invincible(int duration_ms) {

    printf("Player is now invincible for %d ms!\n", duration_ms);
    invincible_timer_ = duration_ms;

    return true;
}

float Player::getHitboxRadius() const { return kHitboxRadius; }

void Player::handleInput(float dt, BulletManager& bullet_manager) {
    if (!character_) return; // 安全检查

    // 从 Character 获取速度数据
    float speed = current_stats_.base_speed;
    bool is_focused = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);

    if (is_focused) {
        speed *= current_stats_.focus_speed_ratio;
    }


    // 移动逻辑
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  position_.x -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) position_.x += speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    position_.y -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  position_.y += speed * dt;

    // 边界限制
    if (position_.x < 16.0f) position_.x = 16.0f;
    if (position_.x > Config::WindowWidth - 16.0f) position_.x = Config::WindowWidth - 16.0f;
    if (position_.y < 24.0f) position_.y = 24.0f;
    if (position_.y > Config::WindowHeight - 24.0f) position_.y = Config::WindowHeight - 24.0f;

    // 射击
    if (character_->canShoot(dt)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
            character_->onShoot(position_, is_focused, bullet_manager);
            character_->resetShootTimer(current_stats_.fire_rate);
        }
    }

    // 大招
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) { // 假设 X 是 Bomb
         character_->onBomb(position_, bullet_manager);
    }

    // 调试
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1)) {
        life_ = 9999;
    }


}