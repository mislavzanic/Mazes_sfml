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
        sf::Vector2f mPosition, mWall_width;

    public:
        MazeCell(int row, int col, sf::Vector2f size, sf::Vector2f position, sf::Vector2f wall_width);
        ~MazeCell() {}

        inline int getRow() { return mRow; }
        inline int getCol() { return mCol; }

        inline sf::Vector2f getPositon() { return mPosition; }
        inline sf::Vector2f Wall_width() { return mWall_width; }

        inline bool getUpWall() { return mWallUp; }
        inline bool getDownWall() { return mWallDown; }
        inline bool getLeftWall() { return mWallLeft; }
        inline bool getRightWall() { return mWallRight; }

        void removeWall(int n);
        void setColor(sf::Color color) { this->mColor = color; }

        bool visited = false;
        bool visitedSolution = false;
        bool isCurrent = false;

        sf::Color mColor = sf::Color::Magenta;
    };

}