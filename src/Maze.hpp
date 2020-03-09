#ifndef MAZE_HPP_INCLUDED
#define MAZE_HPP_INCLUDED

#include "MazeCell.hpp"

#include <vector>

namespace mg {
    
    class Maze {
    private:
        std::vector<MazeCell *> mMaze;
        sf::Vector2i mDim;

    public:
        Maze(sf::Vector2i dim, sf::Vector2f window_size);
        ~Maze();

        void draw(sf::RenderWindow& window);
        
        void generateRecursive(sf::RenderWindow& window);
        void generatePrim();

        void solveBFS(sf::RenderWindow& window);
        void solveDFS(sf::RenderWindow &window);

        MazeCell *freeNeighbour(MazeCell *cell);
        void removeWalls(MazeCell *cell1, MazeCell *cell2);

        bool isGenerated = false;
        bool isSolved = false;

        int START = 0, END = mDim.x * mDim.y - 1;
    };
}

#endif