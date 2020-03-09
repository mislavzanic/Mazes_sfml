#include "MazeCell.hpp"

mg::MazeCell::MazeCell(int row, int col, sf::Color wall_color,
                       sf::Vector2f size, sf::Vector2f position, sf::Vector2f wall_width)
: mRow(row), mCol(col) {

    background.setPosition(position);
    background.setSize(size);

    for (int i = 0; i < 4; ++i) {
        mWalls[i].setFillColor(wall_color);
    }

    //DOWN
    mWalls[DOWN].setPosition(sf::Vector2f(position.x, position.y + (size.y - wall_width.y)));
    mWalls[DOWN].setSize(sf::Vector2f(size.x, wall_width.y));

    //UP
    mWalls[UP].setPosition(position);
    mWalls[UP].setSize(sf::Vector2f(size.x, wall_width.y));

    //LEFT
    mWalls[LEFT].setPosition(position);
    mWalls[LEFT].setSize(sf::Vector2f(wall_width.x, size.y));

    //RIGHT
    mWalls[RIGHT].setPosition(sf::Vector2f(position.x + (size.x - wall_width.x), position.y));
    mWalls[RIGHT].setSize(sf::Vector2f(wall_width.x, size.y));

    mWallDown = mWallLeft = mWallRight = mWallUp = true;
}

void mg::MazeCell::draw(sf::RenderWindow& window) {
    
    if (visitedSolution){
        window.draw(background);
    }

    if (mWallDown)
        window.draw(mWalls[DOWN]);
    if (mWallUp)
        window.draw(mWalls[UP]);
    if (mWallLeft)
        window.draw(mWalls[LEFT]);
    if (mWallRight)
        window.draw(mWalls[RIGHT]);
}

void mg::MazeCell::removeWall(int n) {
    if (n == DOWN) mWallDown = false;
    else if (n == UP) mWallUp = false;
    else if (n == LEFT) mWallLeft = false;
    else mWallRight = false;
}