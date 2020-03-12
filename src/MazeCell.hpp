#pragma once

#include <SFML/Graphics.hpp>

namespace mg {

    enum Direction {
        DOWN = 0,
        UP = 1,
        LEFT = 2,
        RIGHT = 3
    };

    class MazeCell {
    private:
        bool mWallUp, mWallDown, mWallLeft, mWallRight;

        int mRow, mCol;

        sf::RectangleShape mWalls[4];
        sf::RectangleShape background;

    public:
        MazeCell(int row, int col, sf::Color wall_color, 
                 sf::Vector2f size, sf::Vector2f position, sf::Vector2f wall_width);
        ~MazeCell() {}

        void draw(sf::RenderWindow& window);

        inline int getRow() { return mRow; }
        inline int getCol() { return mCol; }

        inline bool getUpWall() { return mWallUp; }
        inline bool getDownWall() { return mWallDown; }
        inline bool getLeftWall() { return mWallLeft; }
        inline bool getRightWall() { return mWallRight; }

        void removeWall(int n);
        void setColor(sf::Color color) { background.setFillColor(color); }

        bool visited = false;
        bool visitedSolution = false;
        bool isCurrent = false;
    };

}