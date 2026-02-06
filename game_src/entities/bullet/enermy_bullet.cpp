#include "entities/bullet.hpp" // 需要 BulletStyle 的定义
#include "entities/bullet/enermy_bullet.hpp"

namespace BulletData {
    BulletStyle getStyle(BulletSkin skin) {
        BulletStyle style;

        // 设置默认值
        style.radius = 4.0f;
        style.point_count = 10;
        style.color = sf::Color::White;
        style.hitbox_radius = 2.0f;

        switch (skin) {
        case BulletSkin::None:
            style.radius = 6.0f;
            style.point_count = 4; // 方形
            style.color = sf::Color(200, 200, 200);
            style.hitbox_radius = 0.0f; 
            style.rotation = sf::degrees(45.f); 
            break;

        case BulletSkin::Dot_Yellow:
            style.radius = 12.0f;
            style.point_count = 30;
            style.color = sf::Color::Yellow;
            style.hitbox_radius = 3.0f;
            break;
        case BulletSkin::Rice_Red:
            style.radius = 6.0f;
            style.point_count = 30; // 圆形
            style.scale = {1.0f, 1.0f};
            style.color = sf::Color::Red;
            style.hitbox_radius = 6.0f; 
            break;

        case BulletSkin::Rice_Blue:
            style.radius = 8.0f;
            style.point_count = 30;
            style.scale = {1.0f, 1.0f};
            style.color = sf::Color::Blue;
            style.hitbox_radius = 10.0f;
            break;


        case BulletSkin::Orb_Red:
            style.radius = 12.0f;
            style.point_count = 30;
            style.color = sf::Color::Red;
            style.hitbox_radius = 3.0f;
            break;
            
        case BulletSkin::Orb_Blue:
            style.radius = 16.0f;
            style.point_count = 30;
            style.color = sf::Color::Blue;
            style.hitbox_radius = 3.0f;
            break;

        case BulletSkin::Knife_Green:
            style.radius = 12.0f;
            style.point_count = 3;
            style.scale = {0.2f, 1.5f};
            style.color = sf::Color::Green;
            style.hitbox_radius = 3.0f;
            style.rotation = sf::degrees(180.f); 
            break;
        case BulletSkin::Sakura:
            style.radius = 8.0f;
            style.point_count = 5;
            style.color = sf::Color(255, 200, 200);
            style.hitbox_radius = 3.0f;
            break;
        case BulletSkin::Strike_Green:
            style.radius = 6.0f;
            style.point_count = 4;
            style.scale = {0.5f, 1.0f};
            style.color = sf::Color::Green;
            style.hitbox_radius = 3.0f;
            break;
        }

        return style;
    }

}