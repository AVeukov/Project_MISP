#include <SFML/Graphics.hpp>
#include <vector>

class Map {
private:
    sf::Texture backgroundTexture_;        // �������� ��� ����
    sf::Sprite backgroundSprite_;          // ������ ��� ����
    std::vector<sf::Vector2f> enemyPositions_; // ������� ������

public:
    // �����������
    Map(const std::string& backgroundPath, const std::vector<sf::Vector2f>& enemyPositions) {
        if (!backgroundTexture_.loadFromFile(backgroundPath)) {
            throw std::runtime_error("Failed to load background texture!");
        }
        backgroundSprite_.setTexture(backgroundTexture_);
        enemyPositions_ = enemyPositions;
    }

    // ��������� ����
    void drawBackground(sf::RenderWindow& window) {
        window.draw(backgroundSprite_);
    }

    // ��������� ������� ������
    const std::vector<sf::Vector2f>& getEnemyPositions() const {
        return enemyPositions_;
    }
};
