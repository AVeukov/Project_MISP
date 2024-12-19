#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <fstream>


class Menu {
public:
    Menu(sf::RenderWindow& window, sf::Font& font);
    void reset();
    void draw(sf::RenderWindow& window);
    void handleEvent(sf::Event event, sf::RenderWindow& window);
    void drawSettings(sf::RenderWindow& window);

    bool isGameRunning() const { return isGameRunning_; }
    int getSelectedLocationIndex() const;  // ��������� ������� ��������� �������
    void setSelectedLocationIndex(int index);  // ��������� ������� ������� �������
    bool isSettingsMenuOpen() const { return isSettingsMenuOpen_; }
    float getPlayerSpeed() const { return speed_; }
    float getPlayerStamina() const { return stamina_; }

    int getSelectedLocationForNewGame() const;
    bool isNewGameSelected() { return isNewGameSelected_; }
    int GetPlayerHealth() const { return playerHealth_; }
    int playerHealth_ = 100;

private:
    std::vector<sf::RectangleShape> buttons_;
    std::vector<sf::Text> buttonTexts_;
    sf::Font& font_;
    sf::Texture backgroundTexture_;
    sf::Sprite backgroundSprite_;
    sf::SoundBuffer hoverSoundBuffer_;
    sf::Sound hoverSound_;
    sf::SoundBuffer clickSoundBuffer_;
    sf::Sound clickSound_;
    sf::Music backgroundMusic_; // ���������� ��� ������� ������

    bool isNewGameSelected_ = false;
    int selectedItemIndex_;
    bool isGameRunning_ = false;  // ���� ��������� ����
    bool isSettingsMenuOpen_ = false; // ���� �������� ���� ��������
    int selectedLocationIndex_ = 1;  // ������ ��������� ������� (1 �� ���������)

    int enemyHealth_ = 100;
    
    float speed_ = 100.0f;
    float stamina_ = 100.0f;


};
