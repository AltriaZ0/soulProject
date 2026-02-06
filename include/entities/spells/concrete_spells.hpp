#pragma once
#include "entities/spellcard.hpp"

// 前置声明
class Boss;
class BulletManager;

// === 符卡 1: 简单的环形攻击 (非符) ===
class NormalAttack1 : public SpellCard {
public:
    NormalAttack1();
    void update(float dt, Boss& boss, BulletManager& mgr, const sf::Vector2f& player_pos) override;
private:
    float shoot_timer_;
};

// === 符卡 2: 螺旋 + 自机狙 (符卡) ===
class SpellCardSpiral : public SpellCard {
public:
    SpellCardSpiral();
    void update(float dt, Boss& boss, BulletManager& mgr, const sf::Vector2f& player_pos) override;
private:
    float shoot_timer_;
};

class SpellCardSpread : public SpellCard {
public:
    SpellCardSpread();
    void update(float dt, Boss& boss, BulletManager& mgr, const sf::Vector2f& player_pos) override;
private:
    float shoot_timer_;
    int wave_count_; // 记录发射了第几波，用来做微小的位移
};

// === 符卡 4: 樱花绽放 (Flower Shape) ===
// 几何弹幕：利用正弦波控制子弹速度，使扩散出去的子弹形成花瓣形状
class SpellCardFlower : public SpellCard {
public:
    SpellCardFlower();
    void update(float dt, Boss& boss, BulletManager& mgr, const sf::Vector2f& player_pos) override;
private:
    float shoot_timer_;
    float base_rotation_; // 让整朵花旋转起来
};

// === 符卡 5: 弹幕雨 (Random Rain) ===
// 覆盖弹幕：子弹不从 Boss 发出，而是从屏幕顶端随机落下，限制玩家走位
class SpellCardRain : public SpellCard {
public:
    SpellCardRain();
    void update(float dt, Boss& boss, BulletManager& mgr, const sf::Vector2f& player_pos) override;
private:
    float shoot_timer_;
};