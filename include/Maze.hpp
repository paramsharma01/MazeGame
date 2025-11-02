#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Maze {
private:
    int rows, cols;
    int cellSize;
    int offsetX; // margin from left
    int offsetY; // margin from top

    std::vector<std::vector<int>> grid;
    void generateMaze(int r, int c);
    void addExtraPaths(float braidFactor);
    void carveMainPathThroughCenter();


public:
    Maze(int rows, int cols, int cellSize, int offsetX = 0, int offsetY = 0);

    void draw(sf::RenderWindow& window);
    std::vector<sf::Vector2i> findPath();

    const std::vector<std::vector<int>>& getGrid() const { return grid; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }


};


