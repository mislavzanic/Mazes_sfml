#include "MazeCell.hpp"

mg::MazeCell::MazeCell(int row, int col, sf::Vector2f size, sf::Vector2f position, sf::Vector2f wall_width)
: mRow(row), mCol(col) , mPosition(position), mWall_width(wall_width) {

    mWallDown = mWallLeft = mWallRight = mWallUp = true;
}

void mg::MazeCell::removeWall(int n) {
    if (n == DOWN) mWallDown = false;
    else if (n == UP) mWallUp = false;
    else if (n == LEFT) mWallLeft = false;
    else mWallRight = false;
}
