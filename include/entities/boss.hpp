#pragma once
#include "entities/entity.hpp"
#include "entities/spellcard.hpp"
#include <queue>
#include <memory>

class Boss : public Entity {
public:
    Boss();
    
    void update(float dt, BulletManager& bullet_manager, const sf::Vector2f& player_pos);
    void update(float dt) override {} 
    void render(sf::RenderWindow& window) override;

    // 当被玩家子弹击中时调用
    void takeDamage(float damage);

    const sf::Vector2f& getPosition() const { return position_; }
    // 用于跳过当前符卡（调试用）
    void skipCurrentPhase();

    bool hasActiveSpell() const { return current_spell_ != nullptr; }
    float getCurrentSpellHealthPercent() const { 
        return current_spell_ ? current_spell_->getHealthPercent() : 0.0f; 
    }
    std::string getCurrentSpellName() const { 
        return current_spell_ ? current_spell_->getName() : ""; 
    }
    float getCurrentSpellTimer() const {
        return current_spell_ ? current_spell_->getCurrentTimer() : 0.0f;
    }
private:
    // 初始化所有符卡
    void initSpellCards();

    sf::CircleShape shape_;
    
    // 符卡队列：存放接下来要释放的符卡
    std::queue<std::unique_ptr<SpellCard>> spell_queue_;
    
    // 当前正在释放的符卡
    std::unique_ptr<SpellCard> current_spell_;
};