#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>
#include <thread>

class Grid {
public:
    Grid(int rows, int cols, float obstacleDensity);

    void run();

private:
    int rows;
    int cols;
    float obstacleDensity;
    int startRow;
    int startCol;
    int endRow;
    int endCol;
    int cellSize;
    sf::RenderWindow window;
    std::vector<std::vector<bool>> grid;
    std::vector<int> path;

    void initializeGrid();
    void generateObstacles();
    void initializeWindow();
    void handleMouseClick(sf::Mouse::Button button, int x, int y);
    bool isValidCoordinate(int row, int col);
    void setStartPoint(int row, int col);
    void setEndPoint(int row, int col);
    void findPath();
    int findMinDistance(const std::vector<int>& distances, const std::vector<bool>& visited);
    void relaxNeighbors(int u, std::vector<int>& distances, std::vector<bool>& visited, std::vector<int>& parents);
    void reconstructPath(const std::vector<int>& parents, int currentIdx);
    void drawGrid();
    void drawPath();
};