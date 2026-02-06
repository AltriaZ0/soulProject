#pragma once
#include "entities/characters/character.hpp"
#include "utils/constants.hpp"


class Reimu : public Character {
public:
    CharacterStats getStats() const override {
        return {
            400.0f,               // 基础速度：中等
            0.4f,                 // 低速比例：标准
            3,                    // 初始命：3
            3,                    // 初始B：3
            0.08f,                // 射速：极快
            sf::Color(200, 50, 50)// 红色
        };
    }

    void onShoot(const sf::Vector2f& pos, bool is_focused, BulletManager& mgr) override {
        float speed = 1000.0f;
        if (is_focused) {
            // 集中模式：前方直线
            mgr.spawnBullet(pos.x - 8, pos.y - 10, degrees(-90), speed, EntityType::PlayerBullet);
            mgr.spawnBullet(pos.x + 8, pos.y - 10, degrees(-90), speed, EntityType::PlayerBullet);
        } else {
            // 高速模式：扩散
            mgr.spawnBullet(pos.x, pos.y - 10, degrees(-90), speed, EntityType::PlayerBullet);
            mgr.spawnBullet(pos.x - 10, pos.y, degrees(-110), speed, EntityType::PlayerBullet);
            mgr.spawnBullet(pos.x + 10, pos.y, degrees(-70), speed, EntityType::PlayerBullet);
        }
    }

    void onSkill(const sf::Vector2f& pos, BulletManager& mgr) override {
        printf("Reimu used Skill: Ying-Yang Orb!\n");
    }

    void onBomb(const sf::Vector2f& pos, BulletManager& mgr) override {
        printf("Reimu used Bomb: Fantasy Seal!\n");
        mgr.clear();
    }
};


class Marisa : public Character {
public:
    CharacterStats getStats() const override {
        return {
            550.0f,               // 基础速度：非常快
            0.35f,                // 低速比例：比较慢（便于微操）
            2,                    // 初始命：2 (贫血)
            4,                    // 初始B：4 (炸弹多)
            0.05f,                // 射速：快
            sf::Color(50, 50, 50) // 黑色
        };
    }

    void onShoot(const sf::Vector2f& pos, bool is_focused, BulletManager& mgr) override {
        float speed = 1400.0f; // 子弹速度快
        // 无论是否低速，都是前方强力直线
        mgr.spawnBullet(pos.x - 5, pos.y - 10, degrees(-90), speed, EntityType::PlayerBullet);
        mgr.spawnBullet(pos.x + 5, pos.y - 10, degrees(-90), speed, EntityType::PlayerBullet);
        
        if (is_focused) {
            // 低速：更加密集的激光
             mgr.spawnBullet(pos.x, pos.y - 15, degrees(-90), speed, EntityType::PlayerBullet);
        } else {
            // 高速：稍微宽一点
             mgr.spawnBullet(pos.x - 15, pos.y - 5, degrees(-95), speed, EntityType::PlayerBullet);
             mgr.spawnBullet(pos.x + 15, pos.y - 5, degrees(-85), speed, EntityType::PlayerBullet);
        }
    }

    void onSkill(const sf::Vector2f& pos, BulletManager& mgr) override {
        printf("Marisa used Skill: Magic Missiles!\n");
    }

    void onBomb(const sf::Vector2f& pos, BulletManager& mgr) override {
        printf("Marisa used Bomb: Master Spark!\n");
    }
};