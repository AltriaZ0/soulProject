#include "entities/boss.hpp"
#include "entities/spells/concrete_spells.hpp" 
#include <iostream>

Boss::Boss() {
    position_ = {400.0f, 150.0f};
    
    // 初始化外观
    shape_.setRadius(30.0f);
    shape_.setOrigin({30.0f, 30.0f});
    shape_.setFillColor(sf::Color(150, 50, 250));
    shape_.setPosition(position_);

    // 初始化符卡顺序
    initSpellCards();
}

void Boss::initSpellCards() {
    // 按照出场顺序压入队列
    spell_queue_.push(std::make_unique<NormalAttack1>());
    spell_queue_.push(std::make_unique<SpellCardSpread>());
    spell_queue_.push(std::make_unique<SpellCardFlower>());
    spell_queue_.push(std::make_unique<SpellCardRain>());
    spell_queue_.push(std::make_unique<SpellCardSpiral>());
    
    // 载入
    if (!spell_queue_.empty()) {
        current_spell_ = std::move(spell_queue_.front());
        spell_queue_.pop();
        std::cout << "Boss Phase Start: " << current_spell_->getName() << std::endl;
    }
}

void Boss::update(float dt, BulletManager& mgr, const sf::Vector2f& player_pos) {
    // boss结束状态
    if (!current_spell_) {
        shape_.setFillColor(sf::Color(50, 50, 50));
        return; 
    }

    current_spell_->update(dt, *this, mgr, player_pos);

    // 检查符卡是否结束
    if ( current_spell_->isFinished()) {
        std::cout << "Phase Finished!" << std::endl;
        
        // 切换下一张
        if ( !spell_queue_.empty()) {
            current_spell_ = std::move(spell_queue_.front());
            spell_queue_.pop();
            std::cout << "Next Phase: " << current_spell_->getName() << std::endl;
            
            mgr.clear(); 
        } else {
            // 没有下一张了，Boss 被击败
            current_spell_ = nullptr;
            std::cout << "BOSS DEFEATED!" << std::endl;
            mgr.clear();
        }
    }
}

void Boss::skipCurrentPhase() {
    if (current_spell_) {
        std::cout << "Debug: Skipping Phase!" << std::endl;

        if (!spell_queue_.empty()) {
            // current_spell_ = std::move(spell_queue_.front());
            // spell_queue_.pop();
            current_spell_->takeDamage(9999999.0f); 
            std::cout << "Next Phase: " << current_spell_->getName() << std::endl;
        } else {
            current_spell_ = nullptr;
            std::cout << "BOSS DEFEATED (Skipped)!" << std::endl;
        }
    }
}

void Boss::takeDamage(float damage) {
    if (current_spell_) {
        current_spell_->takeDamage(damage);
    }
}

void Boss::render(sf::RenderWindow& window) {
    if (current_spell_) {
        window.draw(shape_);
    }
}