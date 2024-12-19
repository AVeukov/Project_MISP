#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class LocationMenu {
public:
    LocationMenu(sf::RenderWindow& window, sf::Font& font);
    void draw(sf::RenderWindow& window);
    int handleEvent(sf::Event event, sf::RenderWindow& window);
    int selectedLocationIndex_;
private:

    sf::Texture backgroundTexture_;
    sf::Sprite backgroundSprite_;

    std::vector<sf::RectangleShape> buttons_;
    std::vector<sf::Text> buttonTexts_;
    sf::Font& font_;
    sf::RectangleShape background_;
    

    
};
