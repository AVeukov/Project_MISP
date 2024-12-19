#include "LocationMenu.h"
#include <iostream>

LocationMenu::LocationMenu(sf::RenderWindow& window, sf::Font& font)
    : font_(font), selectedLocationIndex_(-1) {
    
    if (!backgroundTexture_.loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/images/location_map.jpg")) {
        std::cerr << "Ошибка: не удалось загрузить фон!" << std::endl;
    }
    backgroundSprite_.setTexture(backgroundTexture_);

    // Масштабируем фон, чтобы он заполнил всё окно
    /*sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture_.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    backgroundSprite_.setScale(scaleX, scaleY);*/



    // Имена локаций
    std::vector<std::string> locationNames = { "Castle", "Forest", "Hell" };
    buttons_.resize(locationNames.size());
    buttonTexts_.resize(locationNames.size());

    float buttonWidth = 100.f;
    float buttonHeight = 50.f;
    float buttonSpacing = 15.f;
    float startY = (window.getSize().y - (locationNames.size() * (buttonHeight + buttonSpacing))) / 2;

    for (size_t i = 0; i < buttons_.size(); ++i) {
        // Кнопки
        buttons_[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
        buttons_[i].setFillColor(sf::Color(128, 128, 128));
        buttons_[i].setPosition(
            (window.getSize().x - buttonWidth) / 2,
            startY + i * (buttonHeight + buttonSpacing)
        );

        // Тексты кнопок
        buttonTexts_[i].setFont(font_);
        buttonTexts_[i].setString(locationNames[i]);
        buttonTexts_[i].setCharacterSize(24);
        buttonTexts_[i].setFillColor(sf::Color::White);
        buttonTexts_[i].setPosition(
            buttons_[i].getPosition().x + (buttonWidth - buttonTexts_[i].getLocalBounds().width) / 2,
            buttons_[i].getPosition().y + (buttonHeight - buttonTexts_[i].getLocalBounds().height) / 2 - 5
        );
    }
}

void LocationMenu::draw(sf::RenderWindow& window) {
    window.draw(background_);
    for (size_t i = 0; i < buttons_.size(); ++i) {
        window.draw(buttons_[i]);
        window.draw(buttonTexts_[i]);
    }
}

int LocationMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseMoved) {
        for (size_t i = 0; i < buttons_.size(); ++i) {
            if (buttons_[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                buttons_[i].setFillColor(sf::Color::White);
            }
            else {
                buttons_[i].setFillColor(sf::Color(128, 128, 128));
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        for (size_t i = 0; i < buttons_.size(); ++i) {
            if (buttons_[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                return i; // Возврат индекса выбранной локации
            }
        }
    }

    return -1; // Ничего не выбрано
}

