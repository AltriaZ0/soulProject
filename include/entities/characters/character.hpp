#pragma once
#include <SFML/Graphics.hpp>
#include "core/bullet_manager.hpp"
#include <string>

// 机体属性结构体 (方便一次性返回一堆数值)
struct CharacterStats {
    float base_speed;          // 基础移动速度
    float focus_speed_ratio;   // 低速模式减速比例
    int max_life;              // 初始残机
    int max_bomb;              // 初始符卡(B)数量
    float fire_rate;           // 射击冷却时间 (秒)
    sf::Color theme_color;     // 机体代表色 (暂时用颜色代替贴图)
};

class Character {
public:
    virtual ~Character() = default;

    // --- 纯虚函数：子类必须实现 ---

    // 1. 获取机体面板属性
    virtual CharacterStats getStats() const = 0;

    // 2. 普通射击 (Z键)
    // 返回 true 代表射击成功（用于重置冷却）
    virtual void onShoot(const sf::Vector2f& pos, bool is_focused, BulletManager& mgr) = 0;

    // 3. 小技能 (比如 X键，或者松开射击键触发)
    virtual void onSkill(const sf::Vector2f& pos, BulletManager& mgr) = 0;

    // 4. 大招/B (C键)
    virtual void onBomb(const sf::Vector2f& pos, BulletManager& mgr) = 0;

    // --- 辅助逻辑 ---
    // 处理冷却时间
    bool canShoot(float dt) {
        shoot_timer_ -= dt;
        return shoot_timer_ <= 0.0f;
    }
    void resetShootTimer(float cooldown) {
        shoot_timer_ = cooldown;
    }

protected:
    float shoot_timer_ = 0.0f;
};