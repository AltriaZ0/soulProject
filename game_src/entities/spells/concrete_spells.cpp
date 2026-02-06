#include "entities/spells/concrete_spells.hpp"
#include "entities/boss.hpp" // 因为用到了 boss.getPosition()
#include "utils/constants.hpp" // 引入 Config::WindowWidth
#include <cmath>

// === NormalAttack1 实现 ===
NormalAttack1::NormalAttack1() 
    : SpellCard("Non-Spell 1", 1500.0f, 25.0f), shoot_timer_(0.0f) {}

void NormalAttack1::update(float dt, Boss& boss, BulletManager& mgr, const sf::Vector2f& player_pos) {
    timer_ += dt;
    shoot_timer_ += dt;

    if (shoot_timer_ >= 0.1f) {
        sf::Vector2f pos = boss.getPosition();
        for (int i = 0; i < 24; ++i) {
            mgr.spawnBullet(pos.x, pos.y, sf::degrees(i * 30.0f + timer_ * 20.0f), 150.0f, EntityType::BossBullet, BulletSkin::Rice_Blue);
            mgr.spawnBullet(pos.x, pos.y, sf::degrees(i * 30.0f - timer_ * 20.0f), 150.0f, EntityType::BossBullet, BulletSkin::Rice_Red);
        }
        shoot_timer_ = 0.0f;
    }
}

SpellCardSpiral::SpellCardSpiral() 
    : SpellCard("Non-Spell 2", 1500.0f, 45.0f), shoot_timer_(0.0f) {}

void SpellCardSpiral::update(float dt, Boss& boss, BulletManager& mgr, const sf::Vector2f& player_pos) {
    timer_ += dt;
    shoot_timer_ += dt;

    // 螺旋弹
    if (shoot_timer_ >= 0.04f) {
        sf::Vector2f pos = boss.getPosition();
        mgr.spawnBullet(pos.x, pos.y, sf::degrees(timer_ * 200.0f), 90.0f, EntityType::BossBullet, BulletSkin::Orb_Blue);
        mgr.spawnBullet(pos.x, pos.y, sf::degrees(-timer_ * 200.0f), 90.0f, EntityType::BossBullet, BulletSkin::Orb_Red);
        shoot_timer_ = 0.0f;
    }
    
    // 自机狙
    if (std::fmod(timer_, 0.1f) < dt) {
         sf::Vector2f pos = boss.getPosition();
         float angle = std::atan2(player_pos.y - pos.y, player_pos.x - pos.x);
         for(int i = 0; i < 20; i++){
            mgr.spawnBullet(pos.x, pos.y, sf::radians(angle - 10.0f + i * 1.0f), 200.0f, EntityType::BossBullet, BulletSkin::Knife_Green);
         }

    }
}

// 符卡 3: 多路散射
// 核心逻辑：计算玩家角度 -> 偏移角度 -> 发射
SpellCardSpread::SpellCardSpread() 
    : SpellCard("Fire Sign: 7-Way Spread", 800.0f, 30.0f), shoot_timer_(0.0f), wave_count_(0) {}

void SpellCardSpread::update(float dt, Boss& boss, BulletManager& mgr, const sf::Vector2f& player_pos) {
    timer_ += dt;
    shoot_timer_ += dt;

    // 射击频率：0.4秒一次
    if (shoot_timer_ >= 0.7f) {
        sf::Vector2f pos = boss.getPosition();
        
        // 1. 计算指向玩家的基础角度 (atan2 返回弧度)
        float dx = player_pos.x - pos.x;
        float dy = player_pos.y - pos.y;
        float aim_angle_rad = std::atan2(dy, dx);
        
        // 2. 设定扇形参数
        int ways = 10;             // 发射 7 路
        float spread_angle = 45.0f; // 总张角 60 度
        float angle_step = spread_angle / (ways - 1); // 每路间隔 10 度
        float start_angle = -spread_angle / 2.0f;     // 起始偏移 -30 度

        // 3. 循环发射
        for (int i = 0; i < ways; ++i) {
            // 将弧度转为度数进行加减，再转回 sf::Angle
            float current_deg = sf::radians(aim_angle_rad).asDegrees() + start_angle + (i * angle_step);
            
            // 每一波稍微加一点速度变化，增加层次感
            float speed = 250.0f + (wave_count_ % 2) * 50.0f; 
            
            mgr.spawnBullet(pos.x, pos.y, sf::degrees(current_deg), speed, EntityType::BossBullet, BulletSkin::Dot_Yellow);
        }

        wave_count_++;
        shoot_timer_ = 0.0f;
    }
}

// 符卡 4: 樱花绽放
// 核心逻辑：所有子弹角度均匀分布，但速度 v = base + sin(k * angle)
SpellCardFlower::SpellCardFlower() 
    : SpellCard("Flower Sign: Blooming", 1200.0f, 45.0f), shoot_timer_(0.0f), base_rotation_(0.0f) {}

void SpellCardFlower::update(float dt, Boss& boss, BulletManager& mgr, const sf::Vector2f& player_pos) {
    timer_ += dt;
    shoot_timer_ += dt;
    base_rotation_ += dt * 30.0f; // 让花朵整体缓慢旋转

    // 射击频率：0.25秒一次，高频
    if (shoot_timer_ >= 0.25f) {
        sf::Vector2f pos = boss.getPosition();
        
        int count = 60; // 一圈发射 60 发
        int petals = 5; // 5 个花瓣

        for (int i = 0; i < count; ++i) {
            float angle_deg = i * (360.0f / count) + base_rotation_;
            
            // 【核心算法】速度调制
            // 将角度放入 sin 函数中。如果是 5 个花瓣，就乘以 5。
            // 这里的 math::sin 需要弧度，所以先转换
            float angle_rad = sf::degrees(angle_deg).asRadians();
            
            // 速度公式：基础速度 + 波动幅度 * sin(花瓣数 * 角度)
            float speed = 200.0f + 100.0f * std::sin(petals * angle_rad);

            mgr.spawnBullet(pos.x, pos.y, sf::degrees(angle_deg), speed, EntityType::BossBullet, BulletSkin::Sakura);
        }
        shoot_timer_ = 0.0f;
    }
}

// 符卡 5: 弹幕雨 (Random Position)
// 核心逻辑：X轴随机，Y轴固定在顶部，垂直向下发射
#include <cstdlib> // for rand()

SpellCardRain::SpellCardRain() 
    : SpellCard("Water Sign: Heavy Rain", 1600.0f, 35.0f), shoot_timer_(0.0f) {}

void SpellCardRain::update(float dt, Boss& boss, BulletManager& mgr, const sf::Vector2f& player_pos) {
    timer_ += dt;
    shoot_timer_ += dt;

    // 极高频率：0.02秒一次（像下雨一样连绵不绝）
    if (shoot_timer_ >= 0.015f) {
        // 1. 在屏幕宽度内随机生成 X 坐标
        // 假设边框预留 20 像素
        int min_x = 20;
        int max_x = Config::WindowWidth - 20;
        float rand_x = static_cast<float>(min_x + std::rand() % (max_x - min_x));
        
        // 2. Y 坐标固定在屏幕顶端 (或者 Boss 上方)
        float start_y = -10.0f; 

        // 3. 垂直向下 (90度)
        // 稍微加一点随机角度 (-5 到 +5 度)，模拟风吹
        float rand_angle_offset = (std::rand() % 10) - 5.0f; 
        
        // 4. 随机速度，制造参差感
        float speed = 300.0f + (std::rand() % 100);

        mgr.spawnBullet(rand_x, start_y, sf::degrees(90.0f + rand_angle_offset), speed, EntityType::BossBullet, BulletSkin::Strike_Green);

        shoot_timer_ = 0.0f;
    }
}