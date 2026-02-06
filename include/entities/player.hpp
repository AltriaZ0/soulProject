#pragma once
#include "entities/entity.hpp"
#include "entities/characters/character.hpp"
// 前置声明，解决循环依赖
class BulletManager;

// 枚举机体
enum class CharacterType { Reimu, Marisa };

class Player : public Entity {
public:
    Player();
    // 切换机体
    void switchCharacter(CharacterType type);

    // 注意这里传入了 BulletManager&
    void update(float dt, BulletManager& bullet_manager);
    void update(float dt) override; // 为了满足 Entity 接口，但不实际使用
    
    void render(sf::RenderWindow& window) override;

    float getHitboxRadius() const;
    bool takeDamage();
    bool isDead() const;

    // 设置无敌和查询无敌状态
    bool set_invincible(int duration_ms = 2000); // 给予无敌状态
    bool isInvincible() const;

private:
    void handleInput(float dt, BulletManager& bullet_manager);

    sf::RectangleShape sprite_;
    sf::CircleShape hitbox_;
    float shoot_timer_;
    int life_;
    float invincible_timer_ = 0.0f;
    // 机体
    std::unique_ptr<Character> character_;

    // 缓存当前机体的属性（避免每帧都去调用 getStats）
    CharacterStats current_stats_;

};