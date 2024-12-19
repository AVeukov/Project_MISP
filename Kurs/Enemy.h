#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include "Player.h"

class Player;

class Enemy {
public:

    Enemy(const sf::Texture& texture, sf::Vector2f startPosition, float speed, int type);

    void update(float deltaTime, sf::Vector2f playerPosition, const Player& player, const std::vector<Enemy>& enemies, int type);
    void draw(sf::RenderWindow& window);                      
    sf::Vector2f getPosition() const;                        
    void takeDamage(int amount);                             
    bool isAlive() const;                                    
    sf::Sprite getSprite() const { return sprite_; }

    void dealDamage(Player& player, int damageAmount);
    void resetDamageFlag();
    bool isAttacking() const; 
    void resolveCollisionWithOtherEnemies(std::vector<Enemy>& enemies);
    bool checkCollisionWithOtherEnemies(const Enemy& other) const;

    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position);
    int getType() const { return type; }
    sf::FloatRect getCollisionBox() const;

private:
    enum class State {
        Idle,
        Running,
        Attacking
    };

    sf::Texture texture_;   
    sf::Sprite sprite_;     
    float speed_;         
    int health_;            
    int maxHealth_;         
    int type;
    float originalSpeed_;
    const std::vector<sf::IntRect>* currentAnimation_;
    sf::IntRect frameRect_;   
    float animationTimer_;    
    float animationSpeed_;    
    int currentFrame_;    
    
    float attackCooldown_; // Время перезарядки между атаками
    float timeSinceLastAttack_; // Время, прошедшее с последней атаки

    bool alive_;

    std::vector<sf::IntRect> idleFramesRight_; 
    std::vector<sf::IntRect> runFramesRight_;  
    std::vector<sf::IntRect> attackFrames_;    
    
    
    State state_;             
    bool facingRight_;       
    bool damageDealt_;
   
    sf::RectangleShape healthBar_; 
    void updateHealthBar();        

    sf::Vector2f calculateDirection(sf::Vector2f targetPosition) const; 

    void updateAnimation(float deltaTime);

    void initializeFrames(int type);
};

#endif  ENEMY_H
