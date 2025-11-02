#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Player
{
private:
    int row;
    int col;
    int cellSize;
    int offsetX;
    int offsetY;
    sf::RectangleShape shape;

public:
    Player(int startRow, int startCol, int cellSize, int margin);

    void handleInput(const sf::Event &event, const std::vector<std::vector<int>> &grid);
    void draw(sf::RenderWindow &window);
    bool reachedEnd(int endRow, int endCol) const;
    sf::Vector2i getPosition() const;
};
