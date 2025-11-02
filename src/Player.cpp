#include "../include/Player.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player(int startRow, int startCol, int cellSize, int margin)
    : row(startRow), col(startCol), cellSize(cellSize),
      offsetX(margin), offsetY(margin)
{
    shape.setSize(sf::Vector2f(cellSize - 2, cellSize - 2));
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(offsetX + col * cellSize + 1, offsetY + row * cellSize + 1);
}


void Player::handleInput(const sf::Event& event, const std::vector<std::vector<int>>& grid)
{
    if (event.type != sf::Event::KeyPressed)
        return;

    int newRow = row;
    int newCol = col;

    if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
        newRow--;
    else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
        newRow++;
    else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left)
        newCol--;
    else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right)
        newCol++;

    // Check boundaries and walls
    if (newRow >= 0 && newRow < (int)grid.size() &&
        newCol >= 0 && newCol < (int)grid[0].size() &&
        grid[newRow][newCol] == 0)
    {
        row = newRow;
        col = newCol;

        // ✅ Apply the offset here so player moves inside maze, not in margin
        shape.setPosition(offsetX + col * cellSize + 1, offsetY + row * cellSize + 1);
    }
}


void Player::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

bool Player::reachedEnd(int endRow, int endCol) const
{
    return row == endRow && col == endCol;
}

sf::Vector2i Player::getPosition() const {
    return sf::Vector2i(this->col, this->row); // x = column, y = row
}