#include "Grid.h"

int main() {
    int rows = 20;
    int cols = 40;
    float obstacleDensity = 0.15f;

    Grid grid(rows, cols, obstacleDensity);
    grid.run();

    return 0;
}