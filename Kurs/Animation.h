#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
public:
    Animation() = default;
    Animation(sf::Texture& texture, const std::vector<sf::IntRect>& frames, float speed, bool looped = true);

    void update(float deltaTime);
    void applyToSprite(sf::Sprite& sprite);
    bool isFinished() const;
    void reset();

private:
    std::vector<sf::IntRect> frames_;
    float speed_;
    int currentFrame_;
    float time_;
    sf::Texture* texture_;
    bool looped_;  // Новое поле для контроля зацикливания
};

#endif
