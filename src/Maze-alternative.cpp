#include "../include/Maze.hpp"
#include <stack>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <cmath>

Maze::Maze(int rows, int cols, int cellSize)
    : rows(rows), cols(cols), cellSize(cellSize), grid(rows, std::vector<int>(cols, 1))
{
    std::srand(std::time(nullptr));

    // Step 1: Pre-carve main path through the center
    carveMainPathThroughCenter();

    // Step 2: Generate rest of maze with DFS and center bias
    generateMaze(1, 1);

    // Step 3: Add loops to make it natural
    addExtraPaths(0.3f);
}

// -------------------------------------------------------------
// Generate Maze with DFS and bias toward center
// -------------------------------------------------------------
void Maze::generateMaze(int r, int c) {
    std::stack<std::pair<int, int>> stack;
    stack.push({r, c});
    grid[r][c] = 0;

    int dr[] = {-2, 2, 0, 0};
    int dc[] = {0, 0, -2, 2};

    while (!stack.empty()) {
        int row = stack.top().first;
        int col = stack.top().second;


        // 4 directions
        std::vector<int> dirs = {0, 1, 2, 3};

        // Apply center bias → prioritize moves toward middle
        int midR = rows / 2;
        int midC = cols / 2;
        auto centerBias = [&](int dir) {
            int nr = row + 2 * dr[dir] / 2;
            int nc = col + 2 * dc[dir] / 2;
            return std::abs(nr - midR) + std::abs(nc - midC);
        };
        std::sort(dirs.begin(), dirs.end(), [&](int a, int b) {
            return centerBias(a) < centerBias(b);
        });

        bool moved = false;
        for (int i : dirs) {
            int nr = row + dr[i];
            int nc = col + dc[i];

            if (nr > 0 && nc > 0 && nr < rows - 1 && nc < cols - 1 && grid[nr][nc] == 1) {
                grid[nr][nc] = 0;
                grid[row + dr[i] / 2][col + dc[i] / 2] = 0;
                stack.push({nr, nc});
                moved = true;
                break;
            }
        }

        if (!moved) stack.pop();
    }
}

// -------------------------------------------------------------
// Carve main path from start → center → end
// -------------------------------------------------------------
void Maze::carveMainPathThroughCenter() {
    int x = 1, y = 1;
    int midX = rows / 2;
    int midY = cols / 2;
    int endX = rows - 2;
    int endY = cols - 2;

    auto carvePath = [&](int x1, int y1, int x2, int y2) {
        while (x1 != x2 || y1 != y2) {
            grid[x1][y1] = 0;
            if (x1 < x2) x1++;
            else if (x1 > x2) x1--;
            if (y1 < y2) y1++;
            else if (y1 > y2) y1--;
        }
        grid[x2][y2] = 0;
    };

    carvePath(x, y, midX, midY);
    carvePath(midX, midY, endX, endY);
}

// -------------------------------------------------------------
// Add loops (braid factor logic remains same)
// -------------------------------------------------------------
void Maze::addExtraPaths(float braidFactor) {
    for (int r = 1; r < rows - 1; r++) {
        for (int c = 1; c < cols - 1; c++) {
            if (grid[r][c] == 0) continue;

            int openNeighbors = 0;
            if (grid[r - 1][c] == 0) openNeighbors++;
            if (grid[r + 1][c] == 0) openNeighbors++;
            if (grid[r][c - 1] == 0) openNeighbors++;
            if (grid[r][c + 1] == 0) openNeighbors++;

            if (openNeighbors == 1 && ((float)std::rand() / RAND_MAX) < braidFactor) {
                std::vector<std::pair<int, int>> walls;
                if (grid[r - 1][c] == 1) walls.push_back({r - 1, c});
                if (grid[r + 1][c] == 1) walls.push_back({r + 1, c});
                if (grid[r][c - 1] == 1) walls.push_back({r, c - 1});
                if (grid[r][c + 1] == 1) walls.push_back({r, c + 1});

                if (!walls.empty()) {
                    auto [wr, wc] = walls[std::rand() % walls.size()];
                    grid[wr][wc] = 0;
                }
            }
        }
    }
}

// -------------------------------------------------------------
// Draw maze (no change here)
// -------------------------------------------------------------
void Maze::draw(sf::RenderWindow& window) {
    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (r == 0 && c == 0)
                cell.setFillColor(sf::Color::Green);
            else if (r == rows - 1 && c == cols - 1)
                cell.setFillColor(sf::Color::Red);
            else if (grid[r][c] == 1)
                cell.setFillColor(sf::Color::Black);
            else
                cell.setFillColor(sf::Color::White);

            cell.setPosition(c * cellSize, r * cellSize);
            window.draw(cell);
        }
    }
}
