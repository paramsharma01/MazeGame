#include "../include/Maze.hpp"
#include <stack>
#include <cstdlib>
#include <ctime>
#include <random>
#include <queue>

Maze::Maze(int rows, int cols, int cellSize, int offsetX, int offsetY)
    : rows(rows), cols(cols), cellSize(cellSize), offsetX(offsetX), offsetY(offsetY), grid(rows, std::vector<int>(cols, 1))
{
    std::srand(std::time(nullptr));
    generateMaze(0, 0);
    addExtraPaths(0.8f);
}


void Maze::generateMaze(int r, int c) {
    std::stack<std::pair<int, int>> stack;
    stack.push({r, c});
    grid[r][c] = 0;

    int dr[] = {-2, 2, 0, 0};
    int dc[] = {0, 0, -2, 2};

    while (!stack.empty()) {
        auto [row, col] = stack.top();
        std::vector<int> dirs = {0, 1, 2, 3};
        std::shuffle(dirs.begin(), dirs.end(), std::mt19937(std::random_device()()));

        bool moved = false;
        for (int i : dirs) {
            int nr = row + dr[i];
            int nc = col + dc[i];

            if (nr >= 0 && nc >= 0 && nr < rows && nc < cols && grid[nr][nc] == 1) {
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

void Maze::draw(sf::RenderWindow& window) {
    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // Color logic
            if (r == 0 && c == 0)
                cell.setFillColor(sf::Color::Green); // Start
            else if (r == rows - 1 && c == cols - 1)
                cell.setFillColor(sf::Color::Red);   // End
            else if (grid[r][c] == 1)
                cell.setFillColor(sf::Color::Black); // Wall
            else
                cell.setFillColor(sf::Color::White); // Path

            // Add light gray grid outline for better visibility
            cell.setOutlineColor(sf::Color(180, 180, 180));
            cell.setOutlineThickness(0.5f);

            cell.setPosition(offsetX +  c* cellSize, offsetY + r* cellSize);
            window.draw(cell);
        }
    }
}

void Maze::addExtraPaths(float braidFactor) {
    // braidFactor = 0.0 → no loops (perfect maze)
    // braidFactor = 0.5 → some loops
    // braidFactor = 1.0 → very loopy maze

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


std::vector<sf::Vector2i> Maze::findPath() {
    std::vector<sf::Vector2i> path;
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<sf::Vector2i>> parent(rows, std::vector<sf::Vector2i>(cols, {-1, -1}));

    std::queue<sf::Vector2i> q;
    q.push({0, 0});
    visited[0][0] = true;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    while (!q.empty()) {
        auto cur = q.front();
        q.pop();

        if (cur.x == cols - 1 && cur.y == rows - 1)
            break;

        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows && grid[ny][nx] == 0 && !visited[ny][nx]) {
                visited[ny][nx] = true;
                parent[ny][nx] = cur;
                q.push({nx, ny});
            }
        }
    }

    // reconstruct path
    sf::Vector2i cur = {cols - 1, rows - 1};
    while (cur.x != -1 && cur.y != -1) {
        path.push_back(cur);
        cur = parent[cur.y][cur.x];
    }
    std::reverse(path.begin(), path.end());
    return path;
}
