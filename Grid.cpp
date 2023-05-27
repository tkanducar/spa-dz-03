#include "Grid.h"

Grid::Grid(int rows, int cols, float obstacleDensity) :
    rows(rows),
    cols(cols),
    obstacleDensity(obstacleDensity),
    startRow(-1),
    startCol(-1),
    endRow(-1),
    endCol(-1),
    cellSize(20),
    window(sf::VideoMode(cols* cellSize, rows* cellSize), "SpaDomacaZadaca03") {
    initializeGrid();
    generateObstacles();
    initializeWindow();
}

void Grid::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                handleMouseClick(event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
            }
        }

        window.clear(sf::Color::White);
        drawGrid();
        drawPath();
        window.display();
    }
}

void Grid::initializeGrid() {
    grid.resize(rows, std::vector<bool>(cols, false));
}

void Grid::generateObstacles() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (dis(gen) < obstacleDensity) {
                grid[row][col] = true;
            }
        }
    }
}

void Grid::initializeWindow() {
    window.setFramerateLimit(60);
}

void Grid::handleMouseClick(sf::Mouse::Button button, int x, int y) {
    if (button == sf::Mouse::Left) {
        int row = y / cellSize;
        int col = x / cellSize;

        if (isValidCoordinate(row, col)) {
            if (startRow == -1 && startCol == -1) {
                setStartPoint(row, col);
            }
            else if (endRow == -1 && endCol == -1) {
                setEndPoint(row, col);
            }
        }
    }
}

bool Grid::isValidCoordinate(int row, int col) {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void Grid::setStartPoint(int row, int col) {
    startRow = row;
    startCol = col;
    findPath();
}

void Grid::setEndPoint(int row, int col) {
    endRow = row;
    endCol = col;
    findPath();
}

void Grid::findPath() {
    if (startRow == -1 || startCol == -1 || endRow == -1 || endCol == -1) {
        return;
    }

    std::vector<int> distances(rows * cols, std::numeric_limits<int>::max());
    std::vector<bool> visited(rows * cols, false);
    std::vector<int> parents(rows * cols, -1);

    int startIdx = startRow * cols + startCol;
    int endIdx = endRow * cols + endCol;

    distances[startIdx] = 0;

    for (int i = 0; i < rows * cols; ++i) {
        int u = findMinDistance(distances, visited);
        visited[u] = true;

        if (u == endIdx) {
            reconstructPath(parents, endIdx);
            return;
        }

        relaxNeighbors(u, distances, visited, parents);
    }
}

int Grid::findMinDistance(const std::vector<int>& distances, const std::vector<bool>& visited) {
    int minDistance = std::numeric_limits<int>::max();
    int minIdx = -1;

    for (int v = 0; v < rows * cols; ++v) {
        if (!visited[v] && distances[v] <= minDistance) {
            minDistance = distances[v];
            minIdx = v;
        }
    }

    return minIdx;
}

void Grid::relaxNeighbors(int u, std::vector<int>& distances, std::vector<bool>& visited, std::vector<int>& parents) {
    int row = u / cols;
    int col = u % cols;

    std::vector<std::pair<int, int>> neighbors;
    if (row > 0) neighbors.emplace_back(row - 1, col);
    if (row < rows - 1) neighbors.emplace_back(row + 1, col);
    if (col > 0) neighbors.emplace_back(row, col - 1);
    if (col < cols - 1) neighbors.emplace_back(row, col + 1);

    for (const auto& neighbor : neighbors) {
        int neighborIdx = neighbor.first * cols + neighbor.second;

        if (!visited[neighborIdx] && !grid[neighbor.first][neighbor.second]) {
            int newDistance = distances[u] + 1;

            if (newDistance < distances[neighborIdx]) {
                distances[neighborIdx] = newDistance;
                parents[neighborIdx] = u;
            }
        }
    }
}

void Grid::reconstructPath(const std::vector<int>& parents, int currentIdx) {
    path.clear();

    while (currentIdx != -1) {
        path.push_back(currentIdx);
        currentIdx = parents[currentIdx];
    }

    std::reverse(path.begin(), path.end());
}

void Grid::drawGrid() {
    sf::RectangleShape rect(sf::Vector2f(cellSize, cellSize));
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color::Black);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            rect.setPosition(col * cellSize, row * cellSize);

            if (row == startRow && col == startCol) {
                rect.setFillColor(sf::Color::Green);
            }
            else if (row == endRow && col == endCol) {
                rect.setFillColor(sf::Color::Magenta);
            }
            else if (grid[row][col]) {
                rect.setFillColor(sf::Color::Black);
            }
            else {
                rect.setFillColor(sf::Color::White);
            }

            window.draw(rect);
        }
    }
}

void Grid::drawPath() {
    sf::RectangleShape rectangle(sf::Vector2f(cellSize, cellSize));
    rectangle.setOutlineThickness(1);
    rectangle.setFillColor(sf::Color::Cyan);

    for (const auto& idx : path) {
        int row = idx / cols;
        int col = idx % cols;

        rectangle.setPosition(col * cellSize, row * cellSize);
        window.draw(rectangle);

        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}