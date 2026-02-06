#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
    virtual ~Entity() = default;
    
    // 纯虚函数，子类必须实现
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    const sf::Vector2f& getPosition() const { return position_; }
    void setPosition(const sf::Vector2f& pos) { position_ = pos; }

protected:
    sf::Vector2f position_;
    bool active_ = true;
};