#include "Button.h"

Button::Button(const sf::Vector2f& position, const std::string& text, sf::Font& font, sf::SoundBuffer& hoverSoundBuffer, sf::SoundBuffer& clickSoundBuffer)
    : hovered_(false), clicked_(false) {

    shape_.setSize(sf::Vector2f(200.f, 50.f));
    shape_.setPosition(position);
    shape_.setFillColor(sf::Color::Blue);

    text_.setFont(font);
    text_.setString(text);
    text_.setCharacterSize(24);
    text_.setFillColor(sf::Color::White);
    text_.setPosition(
        position.x + (shape_.getSize().x - text_.getLocalBounds().width) / 2,
        position.y + (shape_.getSize().y - text_.getLocalBounds().height) / 2
    );

    hoverSound_.setBuffer(hoverSoundBuffer);
    clickSound_.setBuffer(clickSoundBuffer);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape_);
    window.draw(text_);
}

void Button::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
        bool isHovered = shape_.getGlobalBounds().contains(mousePos);

        if (isHovered && !hovered_) {
            hoverSound_.play();
            shape_.setFillColor(sf::Color::Red);
        }
        else if (!isHovered && hovered_) {
            shape_.setFillColor(sf::Color::Blue);
        }

        hovered_ = isHovered;
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (hovered_) {
            clickSound_.play();
            clicked_ = true;
        }
    }
}

bool Button::isClicked() const {
    return clicked_;
}
