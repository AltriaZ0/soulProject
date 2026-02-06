#pragma once
#include "entities/bullet.hpp"
#include "entities/bullet/enermy_bullet.hpp"
#include <vector>
#include <memory>

class BulletManager {
public:
    Bullet& spawnBullet(float x, float y, sf::Angle angle, float speed, 
                        EntityType type, BulletSkin skin);
    Bullet& spawnBullet(float x, float y, sf::Angle angle, float speed, 
                        EntityType type);
    void update(float dt);
    void render(sf::RenderWindow& window);
    void clear();

    // 暴露给 Game 类做碰撞检测用
    const std::vector<std::unique_ptr<Bullet>>& getBullets() const;

private:
    std::vector<std::unique_ptr<Bullet>> bullets_;
    void applySkin(Bullet& bullet, BulletSkin skin);
};