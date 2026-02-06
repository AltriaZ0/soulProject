#include "entities/bullet.hpp"
#include "utils/constants.hpp"
#include <cmath>
#include <algorithm> // for std::clamp

// Bullet::Bullet(float x, float y, sf::Angle angle, float speed, EntityType type, BulletStyle style)
//     : type_(type), 
//       current_speed_(speed), 
//       current_angle_(angle), 
//       acceleration_(0.0f), 
//       angular_velocity_(0.0f) {

//     position_ = {x, y};
//     setStyle(style);
//     shape_.setPosition(position_);
// }

Bullet::Bullet(float x, float y, sf::Angle angle, float speed, EntityType type)
    : type_(type), 
      current_speed_(speed), 
      current_angle_(angle), 
      acceleration_(0.0f), 
      angular_velocity_(0.0f) {

    position_ = {x, y};
    shape_.setPosition(position_);
}

// Bullet::Bullet(float x, float y, sf::Angle angle, float speed, EntityType type)
//     : type_(type), 
//       current_speed_(speed), 
//       current_angle_(angle), 
//       acceleration_(0.0f), 
//       angular_velocity_(0.0f) {
      
//     position_ = {x, y};

//     // 默认样式设置
//     BulletStyle style;
//     if (type_ == EntityType::PlayerBullet) {
//         style.radius = 8.0f;           // 视觉半径 8
//         style.point_count = 4;        // 方形

//         style.color = sf::Color(255, 200, 200);
//         // 初始旋转 (让子弹头朝向飞行方向) +90度是因为矩形默认竖直还是水平的问题
//         shape_.setRotation(current_angle_ + sf::degrees(90.f)); 
//     } else {
//         style.radius = 8.0f;           // 视觉半径 8
//         style.point_count = 4;        // 方形
//         style.color = sf::Color(100, 100, 255);
//         shape_.setRotation(current_angle_ + sf::degrees(90.f));
//     }
//     setStyle(style);
    
//     shape_.setPosition(position_);
// }

void Bullet::setStyle(const BulletStyle& style) {
    shape_.setRadius(style.radius);
    shape_.setPointCount(style.point_count); // 关键：控制形状
    shape_.setScale(style.scale);            // 关键：控制椭圆
    shape_.setFillColor(style.color);
    shape_.setRotation( style.rotation);
    // 2. 设置圆心 (Origin)
    // 对于 CircleShape, Origin 设为半径长度即为中心
    shape_.setOrigin({style.radius, style.radius}); 

    // 3. 设置判定半径
    hitbox_radius_ = style.hitbox_radius;
}

void Bullet::update(float dt) {
    live_time_ += dt;

    // 1. 如果有自定义行为函数，先执行它
    // (例如：不仅改变速度，还改变颜色，或者到了某时间突然转向)
    if (behavior_func_) {
        behavior_func_(*this, dt);
    }

    // 2. 应用基础物理 (加速度 & 角速度)
    if (acceleration_ != 0.0f) {
        current_speed_ += acceleration_ * dt;
        // 限制速度范围
        current_speed_ = std::clamp(current_speed_, min_speed_, max_speed_);
    }

    if (angular_velocity_ != 0.0f) {
        current_angle_ += sf::degrees(angular_velocity_ * dt);
        // 同步旋转贴图
        shape_.setRotation(current_angle_ + sf::degrees(90.f));
    }

    // 3. 极坐标转笛卡尔坐标：计算位移
    // v_x = speed * cos(angle)
    // v_y = speed * sin(angle)
    float rad = current_angle_.asRadians();
    sf::Vector2f velocity = {
        std::cos(rad) * current_speed_, 
        std::sin(rad) * current_speed_
    };

    position_ += velocity * dt;
    shape_.setPosition(position_);

    // 4. 边界检查 (World Bounds)
    const float buffer = 50.0f;
    if (position_.x < -buffer || position_.x > Config::WindowWidth + buffer ||
        position_.y < -buffer || position_.y > Config::WindowHeight + buffer) {
        active_ = false;
    }
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(shape_);
}