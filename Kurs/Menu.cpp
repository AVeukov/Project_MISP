#include "Menu.h"
#include <iostream>

Menu::Menu(sf::RenderWindow& window, sf::Font& font)
    : font_(font), selectedItemIndex_(0), isGameRunning_(false), selectedLocationIndex_(0)
{
    // �������� �������� ����
    if (!backgroundTexture_.loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/images/background.jpg")) {
        std::cerr << "������: �� ������� ��������� ���!" << std::endl;
    }
    backgroundSprite_.setTexture(backgroundTexture_);

    // ������������� ������� ����, ����� �� �������� ���� �����
    backgroundSprite_.setScale(
        window.getSize().x / backgroundSprite_.getLocalBounds().width,
        window.getSize().y / backgroundSprite_.getLocalBounds().height
    );

    // ������������� ������
    std::vector<std::string> buttonNames = { "Continue", "New game", "Settings", "Exit" };
    buttons_.resize(buttonNames.size());
    buttonTexts_.resize(buttonNames.size());

    float buttonWidth = 200.f;
    float buttonHeight = 50.f;
    float buttonSpacing = 20.f;
    float startY = (window.getSize().y - (buttonNames.size() * (buttonHeight + buttonSpacing))) / 2;

    for (size_t i = 0; i < buttons_.size(); ++i) {
        buttons_[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
        buttons_[i].setFillColor(sf::Color(128, 128, 128));
        buttons_[i].setPosition(
            (window.getSize().x - buttonWidth) / 2,
            startY + i * (buttonHeight + buttonSpacing)
        );

        buttonTexts_[i].setFont(font_);
        buttonTexts_[i].setString(buttonNames[i]);
        buttonTexts_[i].setCharacterSize(24);
        buttonTexts_[i].setFillColor(sf::Color::White);
        buttonTexts_[i].setPosition(
            buttons_[i].getPosition().x + (buttonWidth - buttonTexts_[i].getLocalBounds().width) / 2,
            buttons_[i].getPosition().y + (buttonHeight - buttonTexts_[i].getLocalBounds().height) / 2 - 5
        );
    }

    // �������� ������
    if (!hoverSoundBuffer_.loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/audio/fcec4eee249d72c.wav")) {
        std::cerr << "������: �� ������� ��������� ���� ���������!" << std::endl;
    }
    hoverSound_.setBuffer(hoverSoundBuffer_);

    if (!clickSoundBuffer_.loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/audio/453ec44415174e0.wav")) {
        std::cerr << "������: �� ������� ��������� ���� �����!" << std::endl;
    }
    clickSound_.setBuffer(clickSoundBuffer_);

    // �������� ������� ������
    if (!backgroundMusic_.openFromFile("C:/Users/User/source/repos/Kurs/Kurs/audio/Berserk_-_Gatsu.wav")) {
        std::cerr << "������: �� ������� ��������� ������!" << std::endl;
    }
    else {
        backgroundMusic_.setLoop(true);
        backgroundMusic_.play();
    }
}

// ��������� ����
void Menu::draw(sf::RenderWindow& window) {
    if (isSettingsMenuOpen_) {
        drawSettings(window);
    }
    else {
        window.draw(backgroundSprite_);
        for (size_t i = 0; i < buttons_.size(); ++i) {
            window.draw(buttons_[i]);
            window.draw(buttonTexts_[i]);
        }
    }
}


// ��������� ������� ����
void Menu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (isSettingsMenuOpen_) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                isSettingsMenuOpen_ = false; // ������������ � ������� ����
            }
            else if (event.key.code == sf::Keyboard::M) {
                // ���������/���������� ������
                if (backgroundMusic_.getStatus() == sf::Music::Playing) {
                    backgroundMusic_.pause();
                }
                else {
                    backgroundMusic_.play();
                }
            }
            else if (event.key.code == sf::Keyboard::Up) {
                enemyHealth_ += 10;
            }
            else if (event.key.code == sf::Keyboard::Down) {
                enemyHealth_ = std::max(10, enemyHealth_ - 10);
            }
            else if (event.key.code == sf::Keyboard::W) {
                playerHealth_ += 10;
            }
            else if (event.key.code == sf::Keyboard::S) {
                playerHealth_ = std::max(10, enemyHealth_ - 10);
            }
            else if (event.key.code == sf::Keyboard::J) {
                speed_ = std::min(500.0f, speed_ + 10.0f); // ����������� �������� (����. 500)
            }
            else if (event.key.code == sf::Keyboard::H) {
                speed_ = std::max(50.0f, speed_ - 10.0f); // ��������� �������� (���. 50)
            }
            else if (event.key.code == sf::Keyboard::K) {
                stamina_ = std::min(300.0f, stamina_ + 10.0f); // ����������� ������� (����. 300)
            }
            else if (event.key.code == sf::Keyboard::L) {
                stamina_ = std::max(50.0f, stamina_ - 10.0f); // ��������� ������� (���. 50)
            }
            else if (event.key.code == sf::Keyboard::Left) {
                selectedLocationIndex_ = std::max(0, selectedLocationIndex_ - 1);
            }
            else if (event.key.code == sf::Keyboard::Right) {
                selectedLocationIndex_ = std::min(2, selectedLocationIndex_ + 1);
            }
        }
    }
    else {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            for (size_t i = 0; i < buttons_.size(); ++i) {
                if (buttons_[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    clickSound_.play();
                    switch (i) {
                    case 0: // Continue
                        isGameRunning_ = true;
                        break;
                    case 1: // New game
                        isNewGameSelected_ = true;
                        isGameRunning_ = true;
                        selectedLocationIndex_ = 0; // �������� � ������� 1
                        break;
                    case 2: // Settings
                        isSettingsMenuOpen_ = true;
                        break;
                    case 3: // Exit
                        window.close();
                        break;
                    }
                }
            }
        }
    }
}



// ���������� ������ reset
void Menu::reset() {
    isGameRunning_ = false;
    isNewGameSelected_ = false; // ����� ������ ����� ����
    selectedLocationIndex_ = 0; // �����������: ����� ������ �������
    backgroundMusic_.play();  // �������� ������� ������, ���� �����
}


int Menu::getSelectedLocationIndex() const {
    return selectedLocationIndex_;
}

void Menu::setSelectedLocationIndex(int index) {
    selectedLocationIndex_ = index;
}

void Menu::drawSettings(sf::RenderWindow& window) {
    window.draw(backgroundSprite_); // ���������� ��� �� �������� ����

    sf::Text title("Settings", font_, 30);
    title.setFillColor(sf::Color::White);
    title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 50);
    window.draw(title);

    std::vector<std::string> settingsOptions = {
    "Music: " + std::string(backgroundMusic_.getStatus() == sf::Music::Playing ? "On" : "Off"),
    "Enemy Health: " + std::to_string(enemyHealth_),
    "Player Health: " + std::to_string(playerHealth_),
    "Speed (H/J): " + std::to_string(speed_),
    "Stamina (K/L): " + std::to_string(stamina_),
    "Location: " + std::to_string(selectedLocationIndex_ + 1)
    };


    float startY = 150.0f;
    float optionSpacing = 50.0f;

    for (size_t i = 0; i < settingsOptions.size(); ++i) {
        sf::Text optionText(settingsOptions[i], font_, 24);
        optionText.setFillColor(sf::Color::White);
        optionText.setPosition(100, startY + i * optionSpacing);
        window.draw(optionText);
    }

    sf::Text backText("Press ESC to return", font_, 20);
    backText.setFillColor(sf::Color::White);
    backText.setPosition(window.getSize().x / 2 - backText.getLocalBounds().width / 2, window.getSize().y - 100);
    window.draw(backText);
}

int Menu::getSelectedLocationForNewGame() const {
    return selectedLocationIndex_ < 0 ? 0 : selectedLocationIndex_; 
}
