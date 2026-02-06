#include "core/bullet_manager.hpp"
#include <algorithm> // for remove_if

Bullet& BulletManager::spawnBullet(float x, float y, sf::Angle angle, float speed, 
                                   EntityType type) {
    return spawnBullet(x, y, angle, speed, type, BulletSkin::None);
}

Bullet& BulletManager::spawnBullet(float x, float y, sf::Angle angle, float speed, 
                                   EntityType type, BulletSkin skin) {
    Bullet bullet(x, y, angle, speed, type);
    BulletStyle style = BulletData::getStyle(skin);
    bullet.setStyle(style);
    bullets_.push_back(std::make_unique<Bullet>(bullet));
    return *bullets_.back();
}


void BulletManager::update(float dt) {
    for (auto& bullet : bullets_) {
        bullet->update(dt);
    }

    // 移除不活跃的子弹
    bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
        [](const std::unique_ptr<Bullet>& b) {
            return !b->isActive();
        }),
        bullets_.end());
}

void BulletManager::render(sf::RenderWindow& window) {
    for (const auto& bullet : bullets_) {
        bullet->render(window);
    }
}

void BulletManager::clear() {
    bullets_.clear();
}

const std::vector<std::unique_ptr<Bullet>>& BulletManager::getBullets() const {
    return bullets_;
}