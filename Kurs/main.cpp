#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Menu.h"
#include "Player.h"
#include "Enemy.h"
#include "LocationMenu.h"
#include "Map.h"
#include <fstream>
#include <iostream>

int readNumberFromFile() {
    std::string filename = "location.txt";
    std::ifstream inputFile(filename);
    int number;

    if (inputFile.is_open()) {
        inputFile >> number;
        inputFile.close();
        return number;
    }
    else {
        std::cerr << "Не удалось открыть файл!" << std::endl;
    }
}

void changeNumberInFile( int newNumber) {
    std::string filename = "location.txt";
    std::ofstream outputFile(filename);

    if (outputFile.is_open()) {
        outputFile << newNumber;
        outputFile.close();
    }
    else {
        std::cerr << "Не удалось открыть файл для записи!" << std::endl;
    }
}


//const std::string locationFilePath = "C:/Users/User/source/repos/Kurs/Kurs/location.txt";
//
//int readLocationFromFile(const std::string& filePath) {
//    std::ifstream file(filePath);
//    if (!file.is_open()) {
//        std::cerr << "Ошибка: не удалось открыть файл для чтения: " << filePath << std::endl;
//        return 0; // Если файл не найден, возвращаем локацию 0
//    }
//
//    int location;
//    file >> location;
//    file.close();
//    return location;
//}
//
//// Запись номера локации в файл
//void writeLocationToFile(const std::string& filePath, int location) {
//    std::ofstream file(filePath);
//    if (!file.is_open()) {
//        std::cerr << "Ошибка: не удалось открыть файл для записи: " << filePath << std::endl;
//        return;
//    }
//
//    file << location;
//    file.close();
//}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game");

    sf::Font font;
    if (!font.loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/font/SouvenircItalic.otf")) {
        std::cerr << "Ошибка: не удалось загрузить шрифт!" << std::endl;
        return -1;
    }

    Menu menu(window, font);

    std::string texturePath = "C:/Users/User/source/repos/Kurs/Kurs/images/Guts-animations.png";
    Player player(texturePath);

    std::vector<sf::Texture> enemiesTextures(3);
    if (!enemiesTextures[0].loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/images/Enemy1_animations.png") ||
        !enemiesTextures[1].loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/images/Enemy2_animations.png") ||
        !enemiesTextures[2].loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/images/Enemy3_animations.png")) {
        std::cerr << "Ошибка: не удалось загрузить текстуры врагов!" << std::endl;
        return -1;
    }

    int currentLocation = 0;


    std::vector<sf::Texture> backgroundTextures(3);
    if (!backgroundTextures[0].loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/images/map1.jpg") ||
        !backgroundTextures[1].loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/images/map2.png") ||
        !backgroundTextures[2].loadFromFile("C:/Users/User/source/repos/Kurs/Kurs/images/map3.png")) {
        std::cerr << "Ошибка: не удалось загрузить текстуры фонов!" << std::endl;
        return -1;
    }

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTextures[currentLocation]);

    sf::Vector2u backgroundSize = backgroundTextures[currentLocation].getSize();

    std::vector<Enemy> enemies;

    // Функция для создания врагов
    auto spawnEnemies = [&](int location) {
        if (location < 0 || location >= static_cast<int>(enemiesTextures.size())) {
            std::cerr << "Ошибка: недопустимый индекс текстуры врага (" << location << ")" << std::endl;
            return;
        }

        enemies.clear();
        int enemyType = location + 1; // Тип врага соответствует текущей карте
        int enemyAmount = 10;

        for (int i = 0; i < enemyAmount / 2; ++i) {
            float x = -200.0f - i * 100.0f; // Слева от карты
            float y = 700.0f;
            enemies.emplace_back(enemiesTextures[location], sf::Vector2f(x, y), 100.0f, enemyType);
        }

        for (int i = 0; i < enemyAmount / 2; ++i) {
            float x = backgroundSize.x + 200.0f + i * 100.0f; // Справа от карты
            float y = 700.0f;
            enemies.emplace_back(enemiesTextures[location], sf::Vector2f(x, y), 100.0f, enemyType);
        }
        };

    sf::View camera;
    camera.setSize(800, 600);
    camera.setCenter(400, 300);

    bool isGameRunning = false;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!isGameRunning) {
                menu.handleEvent(event, window);


                if (menu.isGameRunning() && menu.isNewGameSelected()) {
                    isGameRunning = true;
                    currentLocation = menu.getSelectedLocationForNewGame();
                    backgroundSprite.setTexture(backgroundTextures[currentLocation]);
                    changeNumberInFile(currentLocation);
                    spawnEnemies(currentLocation);
                    player.setPosition(sf::Vector2f(400, 300));
                    //writeLocationToFile(locationFilePath, currentLocation); 
                }
                else if (menu.isGameRunning()) {
                    isGameRunning = true;
                    currentLocation = readNumberFromFile();
                    backgroundSprite.setTexture(backgroundTextures[currentLocation]);
                    spawnEnemies(currentLocation);
                    player.setPosition(sf::Vector2f(400, 300)); 
                    player.HealthMax(); 
                }
                
            }
            else {
                player.handleInput();
            }
        }

        

        float deltaTime = clock.restart().asSeconds();

        if (isGameRunning) {
            player.update(deltaTime, enemies);

            if (player.getHealth() <= 0) {
                camera.setCenter(400, 300);
                window.setView(camera);
                isGameRunning = false;
                player.setPosition(400, 300);
                menu.reset();
                continue;
            }

            bool allEnemiesDefeated = true;

            if (!enemies.empty()) {
                for (Enemy& enemy : enemies) {
                    if (enemy.isAlive()) {
                        allEnemiesDefeated = false;
                        enemy.update(deltaTime, player.getPosition(), player, enemies, currentLocation);
                        if (enemy.isAttacking()) {
                            int damageAmount = 0;
                            enemy.dealDamage(player, damageAmount);
                        }
                        else {
                            enemy.resetDamageFlag();
                        }
                        enemy.resolveCollisionWithOtherEnemies(enemies);
                        if (player.isAttacking()) {
                            int damageAmount = 40;
                            player.dealDamage(enemies, damageAmount);
                        }
                    }
                }
            }

            if (allEnemiesDefeated) {
                if (currentLocation < 2) { 
                    currentLocation++;
                    changeNumberInFile(currentLocation);
                    player.HealthMax();
                    backgroundSprite.setTexture(backgroundTextures[currentLocation]);
                    spawnEnemies(currentLocation);
                    player.setPosition(sf::Vector2f(400, 300));
                }
                else {
                    enemies.clear();
                }
            }

            sf::Vector2f playerPosition = player.getPosition();
            camera.setCenter(playerPosition);

            float halfWidth = camera.getSize().x / 2.f;
            float halfHeight = camera.getSize().y / 2.f;

            if (camera.getCenter().x - halfWidth < 0)
                camera.setCenter(halfWidth, camera.getCenter().y);
            if (camera.getCenter().y - halfHeight < 0)
                camera.setCenter(camera.getCenter().x, halfHeight);
            if (camera.getCenter().x + halfWidth > backgroundSize.x)
                camera.setCenter(backgroundSize.x - halfWidth, camera.getCenter().y);
            if (camera.getCenter().y + halfHeight > backgroundSize.y)
                camera.setCenter(camera.getCenter().x, backgroundSize.y - halfHeight);
        
            if (currentLocation == 0) {
                if (playerPosition.x < 50) {
                    player.setPosition(50, playerPosition.y);
                }
                else if (playerPosition.x > 1985) {
                    player.setPosition(1985, playerPosition.y);
                }
                if (playerPosition.y < 450) {
                    player.setPosition(playerPosition.x, 450);
                }
                else if (playerPosition.y > 650) {
                    player.setPosition(playerPosition.x, 650);
                }
            }
            else if (currentLocation == 1) {
                if (playerPosition.x < 55) {
                    player.setPosition(55, playerPosition.y);
                }
                else if (playerPosition.x > 1980) {
                    player.setPosition(1980, playerPosition.y);
                }
                if (playerPosition.y < 305) {
                    player.setPosition(playerPosition.x, 305);
                }
                else if (playerPosition.y > 820) {
                    player.setPosition(playerPosition.x, 820);
                }
            }
            else if (currentLocation == 1) {
                if (playerPosition.x < 55) {
                    player.setPosition(55, playerPosition.y);
                }
                else if (playerPosition.x > 1980) {
                    player.setPosition(1980, playerPosition.y);
                }
                if (playerPosition.y < 305) {
                    player.setPosition(playerPosition.x, 305);
                }
                else if (playerPosition.y > 820) {
                    player.setPosition(playerPosition.x, 820);
                }
            }
            else if (currentLocation == 2) {
                if (playerPosition.x < 33) {
                    player.setPosition(33, playerPosition.y);
                }
                else if (playerPosition.x > 600) {
                    player.setPosition(600, playerPosition.y);
                }
                if (playerPosition.y < 480) {
                    player.setPosition(playerPosition.x, 480);
                }
                else if (playerPosition.y > 850) {
                    player.setPosition(playerPosition.x, 850);
                }
            }
        
        }



        window.setView(camera);
        window.clear();
        window.draw(backgroundSprite);



        if (!isGameRunning) {
            menu.draw(window);
        }
        else {
            player.draw(window);
            player.drawHUD(window);

            for (Enemy& enemy : enemies) {
                if (enemy.isAlive()) {
                    enemy.draw(window);
                }
            }
        }

        window.display();
    }

    return 0;
}


