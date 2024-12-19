#include "Player.h"
#include <iostream>
#include <cmath>

using namespace std;

Player::Player(const std::string& texturePath)
    : speed_(200.0f), currentState_(AnimationState::IdleRight), movingLeft_(false), isAttacking_(false), direction_(0.f, 0.f), damageDealt_(false) {
 
    if (!texture_.loadFromFile(texturePath)) {
        std::cerr << "Ошибка: не удалось загрузить текстуру!" << std::endl;
    }
    sprite_.setTexture(texture_);
    sprite_.setPosition(400, 1000);

    healthBar_.setSize(sf::Vector2f(100, 20)); 
    healthBar_.setFillColor(sf::Color::Red);   
    healthBar_.setPosition(700, 10);          

    staminaBar_.setSize(sf::Vector2f(100, 20));
    staminaBar_.setFillColor(sf::Color::Green);  
    staminaBar_.setPosition(700, 40);           

    staminaRecoveryRate_ = 10;  
    maxStamina_ = 150;

    // Кадры для различных анимаций
    std::vector<sf::IntRect> idleFramesRight = {
        {100 - 89, 0, 89, 1024 - 912}, {193 - 89, 0, 89, 1024 - 912},
        {289 - 89, 0, 89, 1024 - 912}, {382 - 89, 0, 89, 1024 - 912}
    };

    std::vector<sf::IntRect> runFramesRight = {
        {345 - 70, 112, 70, 112}, {176 - 70, 112, 70, 112},
        {258 - 70, 112, 70, 112}, {345 - 70, 112, 70, 112},
        {433 - 70, 112, 70, 112}, {518 - 70, 112, 70, 112},
        {598 - 70, 112, 70, 112}, {678 - 70, 112, 70, 112}
    };

    std::vector<sf::IntRect> attackFramesRight = {
        {98 - 75, 232, 75, 112}, {182 - 74, 232, 89, 112},
        {329 - 105, 232, 105, 112}, {461 - 98, 232, 100, 112}
    };
    std::vector<sf::IntRect> superAttackFramesRight = {
        {0, 344, 90, 112}, {91, 344, 100, 112},
        {192, 344, 117, 112}, {333, 344, 138, 112},
        {483, 344, 142, 112}, {637, 344, 145, 112},
        {799, 344, 944-799, 112}
    };

    superAttackRight_ = Animation(texture_, superAttackFramesRight, 0.2f, false);
    superAttackLeft_ = Animation(texture_, superAttackFramesRight, 0.2f, false); // Используем те же кадры, только зеркально


    idleRight_ = Animation(texture_, idleFramesRight, 0.2f);
    runRight_ = Animation(texture_, runFramesRight, 0.1f);
    attackRight_ = Animation(texture_, attackFramesRight, 0.15f, false); 

    sprite_.setTexture(texture_);
    sprite_.setPosition(400, 500);
}

void Player::recoverStamina(float deltaTime) {
    stamina_ = std::min(stamina_ + staminaRecoveryRate_ * deltaTime, maxStamina_);
    updateHUD();
}

void Player::handleInput() {
    if (isAttacking_) return; // Блокируем ввод во время атаки

    direction_ = sf::Vector2f(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction_.x = -1.f;
        movingLeft_ = true;
        currentState_ = AnimationState::RunLeft;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction_.x = 1.f;
        movingLeft_ = false;
        currentState_ = AnimationState::RunRight;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction_.y = -1.f;
        currentState_ = movingLeft_ ? AnimationState::RunLeft : AnimationState::RunRight;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction_.y = 1.f;
        currentState_ = movingLeft_ ? AnimationState::RunLeft : AnimationState::RunRight;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && stamina_ >= 10) {
        isAttacking_ = true;
        damageDealt_ = false; // Сбрасываем флаг для новой атаки
        currentState_ = movingLeft_ ? AnimationState::AttackLeft : AnimationState::AttackRight;
        attackRight_.reset();
        useStamina(10);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && !isAttacking_ && stamina_ >= 30) {
        isAttacking_ = true;
        damageDealt_ = false; // Сбрасываем флаг для новой атаки
        currentState_ = movingLeft_ ? AnimationState::SuperAttackLeft : AnimationState::SuperAttackRight;
        superAttackRight_.reset(); // Сбрасываем анимацию суперудара
        superAttackLeft_.reset();  // Сбрасываем анимацию для левого удара
        useStamina(30); // Суперудар стоит больше стамины
    }


    if (direction_.x == 0 && direction_.y == 0 && !isAttacking_) {
        currentState_ = movingLeft_ ? AnimationState::IdleLeft : AnimationState::IdleRight;
    }
}


void Player::update(float deltaTime, const std::vector<Enemy>& enemies) {
    handleInput();
    move(deltaTime, enemies);
    updateAnimation(deltaTime);
    if (!isAttacking_) {
        recoverStamina(deltaTime);
    }
}

void Player::move(float deltaTime, const std::vector<Enemy>& enemies) {
    if (isAttacking_) return;

    sf::Vector2f newPosition = sprite_.getPosition() + direction_ * speed_ * deltaTime;
    sf::Vector2f playerCenter = newPosition + sf::Vector2f(35.f, 56.f); // Центр персонажа с учетом смещения

    for (const Enemy& enemy : enemies) {
        if (enemy.isAlive()) { // Проверяем только живых врагов
            sf::Vector2f enemyCenter = enemy.getSprite().getPosition();

            // Учет смещения для разных типов врагов
            if (enemy.getType() == 1) {
                enemyCenter += sf::Vector2f(22.f, 43.f);
            }
            else if (enemy.getType() == 2) {
                enemyCenter += sf::Vector2f(42.f, 35.f);
            }

            float distance = std::sqrt(std::pow(playerCenter.x - enemyCenter.x, 2) +
                std::pow(playerCenter.y - enemyCenter.y, 2));
            if (distance < 30.f) { // Радиус коллизии, например, 30 пикселей
                sf::Vector2f pushBackDirection = playerCenter - enemyCenter;
                float pushBackLength = 30.f - distance; // Длина отката для выхода из коллизии
                pushBackDirection /= std::sqrt(pushBackDirection.x * pushBackDirection.x + pushBackDirection.y * pushBackDirection.y);
                sprite_.move(pushBackDirection * pushBackLength);
                return; // Остановить движение после выхода из коллизии
            }
        }
    }

    sprite_.move(direction_ * speed_ * deltaTime);
}


void Player::updateAnimation(float deltaTime) {
    switch (currentState_) {
    case AnimationState::IdleRight:
        idleRight_.update(deltaTime);
        idleRight_.applyToSprite(sprite_);
        sprite_.setScale(1.f, 1.f);
        break;
    case AnimationState::IdleLeft:
        idleRight_.update(deltaTime);
        idleRight_.applyToSprite(sprite_);
        sprite_.setScale(-1.f, 1.f);
        break;
    case AnimationState::RunRight:
        runRight_.update(deltaTime);
        runRight_.applyToSprite(sprite_);
        sprite_.setScale(1.f, 1.f);
        break;
    case AnimationState::RunLeft:
        runRight_.update(deltaTime);
        runRight_.applyToSprite(sprite_);
        sprite_.setScale(-1.f, 1.f);
        break;
    case AnimationState::AttackRight:
        if (attackRight_.isFinished()) {
            isAttacking_ = false;
            currentState_ = AnimationState::IdleRight;
            damageDealt_ = false;  // Сбрасываем флаг после завершения атаки
        }
        else {
            attackRight_.update(deltaTime);
            attackRight_.applyToSprite(sprite_);
        }
        sprite_.setScale(1.f, 1.f);
        break;
    case AnimationState::AttackLeft:
        if (attackRight_.isFinished()) {
            isAttacking_ = false;
            currentState_ = AnimationState::IdleLeft;
            damageDealt_ = false;  // Сбрасываем флаг после завершения атаки
        }
        else {
            attackRight_.update(deltaTime);
            attackRight_.applyToSprite(sprite_);
        }
        sprite_.setScale(-1.f, 1.f);
        break;
    case AnimationState::SuperAttackRight:
        if (superAttackRight_.isFinished()) {
            isAttacking_ = false;
            damageDealt_ = false; // Сбрасываем флаг
            currentState_ = AnimationState::IdleRight;
        }
        else {
            superAttackRight_.update(deltaTime);
            superAttackRight_.applyToSprite(sprite_);
        }
        sprite_.setScale(1.f, 1.f);
        break;

    case AnimationState::SuperAttackLeft:
        if (superAttackLeft_.isFinished()) {
            isAttacking_ = false;
            damageDealt_ = false; // Сбрасываем флаг
            currentState_ = AnimationState::IdleLeft;
        }
        else {
            superAttackLeft_.update(deltaTime);
            superAttackLeft_.applyToSprite(sprite_);
        }
        sprite_.setScale(-1.f, 1.f);
        break;





    }

}


void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite_);
}


void Player::useStamina(int amount) {
    stamina_ = std::max(0.0f, stamina_ - amount); 
    updateHUD(); 
}

void Player::updateHUD() {
    healthBar_.setSize(sf::Vector2f(health_, 10.f));  
    staminaBar_.setSize(sf::Vector2f(stamina_, 10.f)); 
}


void Player::drawHUD(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/font/SouvenircItalic.otf");
    sf::View currentView = window.getView();
    window.setView(window.getDefaultView());


    sf::Text test_text;
    test_text.setFont(font);
    test_text.setPosition(300, 200);
    test_text.setCharacterSize(24);
    test_text.setFillColor(sf::Color::Yellow);
    std::ostringstream p;
    p << getPosition().x << " ," << getPosition().y;
    test_text.setString(p.str());
    
    
    healthBar_.setSize(sf::Vector2f(health_, 20));
    staminaBar_.setSize(sf::Vector2f(stamina_, 20));
    window.draw(healthBar_);
    window.draw(staminaBar_);
    
    
    window.draw(test_text);

    window.setView(currentView);
}

void Player::dealDamage(std::vector<Enemy>& enemies, int damageAmount) {
    if (damageDealt_) return;

    sf::FloatRect playerBounds = sprite_.getGlobalBounds();

    for (Enemy& enemy : enemies) {
        if (enemy.isAlive()) {
            sf::FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();


            if (playerBounds.intersects(enemyBounds)) {

                if (isAttacking_ && enemyBounds.left > playerBounds.left) {
                    enemy.takeDamage(damageAmount);
                }
                else if (isAttacking_ && enemyBounds.left + enemyBounds.width < playerBounds.left + playerBounds.width) {
                    enemy.takeDamage(damageAmount);
                }
                
            }

        }
    }

    damageDealt_ = true;
}



void Player::resetDamageFlag() {
    damageDealt_ = false;
}

bool Player::isAttacking() const {
    return currentState_ == AnimationState::AttackRight || currentState_ == AnimationState::AttackLeft;
}





sf::Vector2f Player::getPosition() const {
    return sprite_.getPosition();
}

void Player::takeDamage(int amount) {
    health_ = std::max(0, health_ - amount);  
    updateHUD();
}

void Player::setPosition(float x, float y) {
    sprite_.setPosition(x, y); 
}

void Player::setPosition(const sf::Vector2f& position) {
    sprite_.setPosition(position);
}

void Player::HealthMax() {
    health_ = 100;
}

sf::FloatRect Player::getCollisionBox() const {
    sf::Vector2f center = sprite_.getPosition();
    return sf::FloatRect(center.x - 15.f, center.y - 15.f, 30.f, 30.f);
}

void Player::updateParameters(float speed, float maxStamina) {
    speed_ = speed;
    maxStamina_ = maxStamina;
    stamina_ = std::min(stamina_, maxStamina_); // Ограничиваем текущую стамину максимальной
}
