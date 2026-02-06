#pragma once
#include "entities/entity.hpp"
#include <functional>


// 定义子弹的视觉样式（方便复用）
struct BulletStyle {
    sf::Color color = sf::Color::White;
    float radius = 4.0f;            // 视觉半径
    sf::Vector2f scale = {1.0f, 1.0f}; // 缩放 (用于做椭圆)
    std::size_t point_count = 30;   // 边数 (3=三角形, 30+=圆形)
    float hitbox_radius = 2.0f;
    sf::Angle rotation = sf::degrees(0.f);
};


enum class EntityType {
    Player,
    Boss,
    PlayerBullet,
    BossBullet
};

class Bullet : public Entity {
public:
    // 使用 std::function 定义行为函数类型
    // 参数：Bullet引用自身，dt 时间增量
    using UpdateCallback = std::function<void(Bullet&, float)>;
    Bullet(float x, float y, sf::Angle angle,  float speed, EntityType type);
    // Bullet(float x, float y, sf::Angle angle,  float speed, EntityType type, BulletStyle style);

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    // === 核心属性设置 ===
    
    // 设置加速度 (像素/秒^2)
    void setAcceleration(float accel) { acceleration_ = accel; }
    
    // 设置角速度 (度/秒) - 用于转弯
    void setAngularVelocity(float ang_vel_deg) { angular_velocity_ = ang_vel_deg; }
    
    // 设置自定义行为函数 (例如：正弦波运动)
    void setBehavior(UpdateCallback callback) { behavior_func_ = callback; }

    // 设置最大/最小速度限制 (防止减速到倒着飞)
    void setSpeedLimit(float min_s, float max_s) { min_speed_ = min_s; max_speed_ = max_s; }

    // === Getters & Setters (供行为函数调用) ===
    float getSpeed() const { return current_speed_; }
    void setSpeed(float s) { current_speed_ = s; }
    
    sf::Angle getAngle() const { return current_angle_; }
    void setAngle(sf::Angle a) { current_angle_ = a; }
    
    // 改变样式
    void setStyle(const BulletStyle& style);
    bool isActive() const { return active_; }
    EntityType getType() const { return type_; }
    void deactivate() { active_ = false; }
    float getHitboxRadius() const { return hitbox_radius_; }
    const sf::CircleShape& getShape() const { return shape_; } // 公开 shape 以便渲染时访问
private:
    // 物理属性
    float current_speed_;       // 当前速率 (标量)
    sf::Angle current_angle_;   // 当前角度
    float acceleration_;        // 加速度
    float angular_velocity_;    // 角速度 (用于产生弧线)

    float min_speed_ = -9999.0f;
    float max_speed_ = 9999.0f;

    // 存活时间 (用于某些基于时间的函数)
    float live_time_ = 0.0f;

    // 视觉
    sf::CircleShape shape_; // 如果你还没有换成 Sprite，暂时用 Shape
    
    // 高级行为回调
    UpdateCallback behavior_func_ = nullptr;
    float hitbox_radius_;
    EntityType type_;          // 子弹类型
};