#include <SFML/Graphics.hpp>
#include <vector>

class Map {
private:
    sf::Texture backgroundTexture_;        // Текстура для фона
    sf::Sprite backgroundSprite_;          // Спрайт для фона
    std::vector<sf::Vector2f> enemyPositions_; // Позиции врагов

public:
    // Конструктор
    Map(const std::string& backgroundPath, const std::vector<sf::Vector2f>& enemyPositions) {
        if (!backgroundTexture_.loadFromFile(backgroundPath)) {
            throw std::runtime_error("Failed to load background texture!");
        }
        backgroundSprite_.setTexture(backgroundTexture_);
        enemyPositions_ = enemyPositions;
    }

    // Отрисовка фона
    void drawBackground(sf::RenderWindow& window) {
        window.draw(backgroundSprite_);
    }

    // Получение позиций врагов
    const std::vector<sf::Vector2f>& getEnemyPositions() const {
        return enemyPositions_;
    }
};
