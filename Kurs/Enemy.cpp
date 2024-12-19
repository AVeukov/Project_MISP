#include "Enemy.h"
#include <cmath>
#include <stdexcept>
#include <iostream>



Enemy::Enemy(const sf::Texture& texture, sf::Vector2f startPosition, float speed, int type)
    : speed_(speed), originalSpeed_(speed), health_(100), maxHealth_(100),
    animationTimer_(0.0f), animationSpeed_(0.2f), currentFrame_(0), state_(State::Idle),
    facingRight_(true), type(type), attackCooldown_(1.0f), timeSinceLastAttack_(0.0f) // Инициализация поля type
{
    sprite_.setTexture(texture);
    sprite_.setPosition(startPosition);

    healthBar_.setSize(sf::Vector2f(50.f, 5.f));
    healthBar_.setFillColor(sf::Color::Red);
    healthBar_.setPosition(sprite_.getPosition());

    initializeFrames(type); // Инициализация анимаций по типу
}



void Enemy::initializeFrames(int type) {
    if (type == 1) {
        idleFramesRight_ = {
            {23, 0, 44, 86}, {90, 0, 44, 86},
            {157, 0, 44, 86}, {224, 0, 44, 86}
        };
        runFramesRight_ = {
            {23, 86, 47, 86}, {89, 86, 47, 86},
            {158, 86, 47, 86}, {232, 86, 47, 86},
            {305, 86, 47, 86}, {357, 86, 47, 86},
            {426, 86, 47, 86}, {495, 86, 47, 86}
        };
        attackFrames_ = {
            {23, 172, 44, 86}, {89, 172, 60, 86},
            {171, 172, 65, 86}, {338, 172, 86, 86}
        };
    }
    else if (type == 2) {
        idleFramesRight_ = {
            {0, 0, 84, 71}, {85, 0, 84, 71},
            {170, 0,  84,71}, {255, 0 ,84 , 71}
        };
        runFramesRight_ = {
            {0, 71,84, 71}, {88, 71, 84, 71},
            {176, 71, 84, 71}, {264, 71,84, 71},
            {352, 71, 84, 71}, {440, 71, 84, 71},
            {548, 71, 84, 71}, {556, 71, 84, 71}
        };
        attackFrames_ = {
            {0, 144,84, 71}, {88, 144, 84, 71},
            {176, 144, 84, 71}, {264, 144,84, 71},
            {352, 144, 84, 71}, {440, 144, 95, 71},
        };
    }
    else if (type == 3) {
        idleFramesRight_ = {
            {0, 0, 80, 69}, {80, 0, 80, 69},
            {160, 0, 80, 69}, {240, 0, 80, 69}
        };
        runFramesRight_ = {
            {0, 0, 80, 69}, {80, 0, 80, 69},
            {160, 0, 80, 69}, {240, 0, 80, 69}
        };
        attackFrames_ = {
            {0, 207,79, 69}, {79, 207, 79, 69},
            {158, 207, 79, 69}, {237, 207,79, 69},
            {316, 207, 79, 69}, {395, 207, 79, 69},
            {474, 207, 79, 69}, {553, 207, 79, 69}
        };
    }

    currentAnimation_ = &idleFramesRight_; // Начальная анимация — Idle
}


void Enemy::update(float deltaTime, sf::Vector2f playerPosition, const Player& player, const std::vector<Enemy>& enemies, int type) {
    sf::Vector2f direction = calculateDirection(playerPosition);
    sf::Vector2f newPosition = sprite_.getPosition() + direction * speed_ * deltaTime;
    sf::Vector2f enemyCenter = newPosition;

    // Учет смещения для текущего врага
    if (type == 1) {
        enemyCenter += sf::Vector2f(22.f, 43.f);
    }
    else if (type == 2) {
        enemyCenter += sf::Vector2f(42.f, 35.f);
    }

    sf::Vector2f playerCenter = player.getPosition() + sf::Vector2f(35.f, 56.f); // Центр персонажа

    // Обновляем таймер атаки
    timeSinceLastAttack_ += deltaTime;

    // Проверка коллизии с игроком
    float distanceToPlayer = std::sqrt(std::pow(playerCenter.x - enemyCenter.x, 2) +
        std::pow(playerCenter.y - enemyCenter.y, 2));
    if (distanceToPlayer < 30.f) {
        // Если враг в радиусе атаки
        state_ = State::Attacking;
        speed_ = 0; // Останавливаем врага

        // Наносим урон, если прошло достаточно времени с момента последнего удара
        if (timeSinceLastAttack_ >= attackCooldown_) {
            const_cast<Player&>(player).takeDamage(10); // Например, 10 единиц урона
            timeSinceLastAttack_ = 0.0f; // Сбрасываем таймер атаки
        }
    }
    else {
        state_ = State::Running;
        speed_ = originalSpeed_;
    }

    // Проверка коллизий с другими врагами
    for (const Enemy& other : enemies) {
        if (&other != this && other.isAlive()) {
            sf::Vector2f otherCenter = other.getSprite().getPosition();

            // Учет смещения для другого врага
            if (other.getType() == 1) {
                otherCenter += sf::Vector2f(22.f, 43.f);
            }
            else if (other.getType() == 2) {
                otherCenter += sf::Vector2f(42.f, 35.f);
            }

            float distanceToOther = std::sqrt(std::pow(otherCenter.x - enemyCenter.x, 2) +
                std::pow(otherCenter.y - enemyCenter.y, 2));
            if (distanceToOther < 30.f) {
                sf::Vector2f pushBackDirection = enemyCenter - otherCenter;
                float pushBackLength = 30.f - distanceToOther; // Длина отката для выхода из коллизии
                pushBackDirection /= std::sqrt(pushBackDirection.x * pushBackDirection.x + pushBackDirection.y * pushBackDirection.y);
                newPosition += pushBackDirection * pushBackLength;
            }
        }
    }

    // Обновление позиции врага
    sprite_.setPosition(newPosition);

    // Логика смены анимации в зависимости от состояния
    switch (state_) {
    case State::Idle:
        currentAnimation_ = &idleFramesRight_;
        break;
    case State::Running:
        currentAnimation_ = &runFramesRight_;
        break;
    case State::Attacking:
        currentAnimation_ = &attackFrames_;
        break;
    }

    // Логика изменения направления спрайта
    if (direction.x > 0) {
        facingRight_ = true;
        sprite_.setScale(1.0f, 1.0f);
    }
    else if (direction.x < 0) {
        facingRight_ = false;
        sprite_.setScale(-1.0f, 1.0f);
    }

    // Обновление анимации
    updateAnimation(deltaTime);
    updateHealthBar();
}





void Enemy::updateAnimation(float deltaTime) {
    animationTimer_ += deltaTime;

    if (animationTimer_ >= animationSpeed_) {
        animationTimer_ -= animationSpeed_;

        switch (state_) {
        case State::Running:
            currentFrame_ = (currentFrame_ + 1) % runFramesRight_.size();
            sprite_.setTextureRect(runFramesRight_[currentFrame_]);
            break;
        case State::Attacking:
            currentFrame_ = (currentFrame_ + 1) % attackFrames_.size();
            sprite_.setTextureRect(attackFrames_[currentFrame_]);
            break;
        case State::Idle:
        default:
            currentFrame_ = (currentFrame_ + 1) % idleFramesRight_.size();
            sprite_.setTextureRect(idleFramesRight_[currentFrame_]);
            break;
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite_);
    window.draw(healthBar_);
}

sf::Vector2f Enemy::getPosition() const {
    return sprite_.getPosition();
}

void Enemy::takeDamage(int amount) {
    health_ -= amount;
    if (health_ < 0) { 
        health_ = 0; 
        alive_ = false;
    }
    updateHealthBar();
}

bool Enemy::isAlive() const {
    return health_ > 0;
}

sf::Vector2f Enemy::calculateDirection(sf::Vector2f targetPosition) const {
    sf::Vector2f direction = targetPosition - sprite_.getPosition();
    float length = std::hypot(direction.x, direction.y);
    if (length > 0) {
        direction /= length; 
    }
    return direction;
}

void Enemy::updateHealthBar() {
    sf::FloatRect bounds = sprite_.getGlobalBounds(); // Получаем размеры спрайта
    sf::Vector2f offset = facingRight_ ? sf::Vector2f(0.f, -10.f) : sf::Vector2f(-bounds.width, -10.f);
    healthBar_.setPosition(sprite_.getPosition() + offset);

    // Обновляем размер полоски 
    float healthPercentage = static_cast<float>(health_) / maxHealth_;
    healthBar_.setSize(sf::Vector2f(50.f * healthPercentage, 5.f));
}

void Enemy::dealDamage(Player& player, int damageAmount) {

    if (damageDealt_) return;

    sf::FloatRect enemyBounds = sprite_.getGlobalBounds();
    sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();

    // Проверяем пересечение границ
    if (enemyBounds.intersects(playerBounds)) {
        player.takeDamage(damageAmount); // Урон игроку
        damageDealt_ = true;            // Урон нанесён
    }
}

void Enemy::resetDamageFlag() {
    damageDealt_ = false;
}

bool Enemy::isAttacking() const {
    return state_ == State::Attacking; // Проверяем, что активна анимация атаки
}


bool Enemy::checkCollisionWithOtherEnemies(const Enemy& other) const {
    sf::FloatRect thisBounds = sprite_.getGlobalBounds();
    sf::FloatRect otherBounds = other.getSprite().getGlobalBounds();

    return thisBounds.intersects(otherBounds);
}

// Функция для корректировки позиции врага
void Enemy::resolveCollisionWithOtherEnemies(std::vector<Enemy>& enemies) {
    for (Enemy& other : enemies) {
        if (&other != this && checkCollisionWithOtherEnemies(other)) {
            // Если враги пересекаются, сдвигаем их
            sf::Vector2f offset = sprite_.getPosition() - other.getSprite().getPosition();
            float distance = std::sqrt(offset.x * offset.x + offset.y * offset.y);

            // Если расстояние меньше минимального, сдвигаем врага
            float minDistance = 25.0f;  // Минимальное расстояние между врагами

            if (distance < minDistance) {
                float overlap = minDistance - distance;
                sf::Vector2f correction = offset / distance * overlap;

                // Сдвигаем врага, чтобы избежать перекрытия
                sprite_.move(correction);
            }
        }
    }
}

void Enemy::setPosition(float x, float y) {
    sprite_.setPosition(x, y); // sprite_ — спрайт игрока
}

void Enemy::setPosition(const sf::Vector2f& position) {
    sprite_.setPosition(position);
}
