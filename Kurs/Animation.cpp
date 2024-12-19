#include "Animation.h"

Animation::Animation(sf::Texture& texture, const std::vector<sf::IntRect>& frames, float speed, bool looped)
    : frames_(frames), speed_(speed), currentFrame_(0), time_(0), texture_(&texture), looped_(looped) {}

void Animation::update(float deltaTime) {
    time_ += deltaTime;
    if (time_ >= speed_) {
        time_ = 0;
        if (looped_) {
            currentFrame_ = (currentFrame_ + 1) % frames_.size();  // Зацикливаем анимацию
        }
        else {
            if (currentFrame_ < frames_.size() - 1) {
                ++currentFrame_;
            }
        }
    }
}

void Animation::applyToSprite(sf::Sprite& sprite) {
    sprite.setTexture(*texture_);
    sprite.setTextureRect(frames_[currentFrame_]);
}

bool Animation::isFinished() const {
    return !looped_ && currentFrame_ == frames_.size() - 1;
}

void Animation::reset() {
    currentFrame_ = 0;
    time_ = 0;
}
