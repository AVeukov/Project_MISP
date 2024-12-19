#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

class Button {
public:
    Button(const sf::Vector2f& position, const std::string& text, sf::Font& font, sf::SoundBuffer& hoverSoundBuffer, sf::SoundBuffer& clickSoundBuffer);

    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);
    bool isClicked() const;

private:
    sf::RectangleShape shape_;
    sf::Text text_;
    sf::Sound hoverSound_;
    sf::Sound clickSound_;
    bool hovered_;
    bool clicked_;
};

#endif
