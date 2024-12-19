#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include "Animation.h"
#include "Enemy.h"
#include <sstream>
#include <math.h>
#include "Menu.h"

enum class AnimationState {
    IdleRight, IdleLeft, RunRight, RunLeft,
    AttackRight, AttackLeft,
    SuperAttackRight, SuperAttackLeft 
};

class Enemy;


class Player {
public:
    Player(const std::string& texturePath);

    void update(float deltaTime, const std::vector<Enemy>& enemies);
    void move(float deltaTime, const std::vector<Enemy>& enemies);
    void handleInput();
    void draw(sf::RenderWindow& window);

    void takeDamage(int amount);
    void useStamina(int amount);
    void recoverStamina(float deltaTime);

    int getHealth() const { return health_; }
    int getStamina() const { return stamina_; }
    void drawHUD(sf::RenderWindow& window);  

    bool isAttacking() const;
    void HealthMax();

    
    //int health_ = Menu::GetPlayerHealth();

    sf::Vector2f getDerection() { return direction_; }
    sf::Sprite getSprite() const { return sprite_; }

    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position);

    void dealDamage(std::vector<Enemy>& enemies, int damageAmount);
    void resetDamageFlag();

    sf::Vector2f getPosition() const;  
    bool isDestroyed() const { return destroyed_; }

    sf::FloatRect getCollisionBox() const;

    void updateParameters(float speed, float maxStamina);


protected:
    sf::Texture texture_;
    sf::Sprite sprite_;
    sf::Vector2f direction_;
    


    float speed_;
    int health_ = 100;
    float stamina_ = 100.0f;
    float staminaRecoveryRate_ = 100.0f;
    float maxStamina_ = 100.0f;

    AnimationState currentState_;
    bool movingLeft_;
    bool isAttacking_;
    bool destroyed_ = false;
    bool damageDealt_;


    Animation idleRight_;
    Animation runRight_;
    Animation attackRight_;
    Animation superAttackRight_;
    Animation superAttackLeft_;

    sf::RectangleShape healthBar_;
    sf::RectangleShape staminaBar_; 

    void updateAnimation(float deltaTime);
    void move(float deltaTime);
    void updateHUD();
};
