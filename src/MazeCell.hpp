#ifndef MAZECELL_HPP_INCLUDED
#define MAZECELL_HPP_INCLUDED

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

        sf::RectangleShape mBackground;
        sf::RectangleShape mWalls[4];

    public:
        MazeCell(int row, int col, sf::Color background_color, sf::Color wall_color, 
                 sf::Vector2f size, sf::Vector2f position, sf::Vector2f wall_width);
        ~MazeCell() {}

        void draw(sf::RenderWindow& window);

        inline int getRow() { return mRow; }
        inline int getCol() { return mCol; }

        void removeWall(int n);

        bool visited = false;
    };

}
#endif