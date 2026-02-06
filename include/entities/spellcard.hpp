#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "core/bullet_manager.hpp"

// 前置声明
class Boss; 

class SpellCard {
public:
    // 构造函数：设定这张符卡的 总血量 和 持续时间
    SpellCard(std::string name, float max_hp, float duration)
        : name_(name), max_hp_(max_hp), current_hp_(max_hp), 
          duration_(duration), timer_(0.0f) {}
    
    virtual ~SpellCard() = default;

    // 核心接口：每帧调用，发射子弹
    // 我们把 boss 指针传进去，这样符卡可以控制 Boss 移动（如果需要）
    virtual void update(float dt, Boss& boss, BulletManager& mgr, const sf::Vector2f& player_pos) = 0;

    // 检查符卡是否结束（时间到 或 血量归零）
    bool isFinished() const {
        return timer_ >= duration_ || current_hp_ <= 0.0f;
    }

    // 受到伤害
    void takeDamage(float amount) {
        current_hp_ -= amount;
        if (current_hp_ < 0) current_hp_ = 0;
    }

    // Getters
    float getHealth() const { return current_hp_; }
    
    float getMaxHealth() const { return max_hp_; }
    float getTimeRemaining() const { return duration_ - timer_; }
    std::string getName() const { return name_; }
    
    float getHealthPercent() const { 
        return (max_hp_ > 0) ? (current_hp_ / max_hp_) : 0.0f; 
    }
    float getCurrentTimer() const { return duration_ - timer_; }
    

protected:
    std::string name_;
    float max_hp_;
    float current_hp_;
    float duration_; // 秒
    float timer_;    // 当前计时
};