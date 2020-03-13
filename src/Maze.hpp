#pragma once

#include "MazeCell.hpp"

#include <vector>

namespace mg {
    
    class Maze {
    private:
        std::vector<MazeCell *> mMaze;
        sf::Vector2i mDim;

        std::vector<sf::Vertex> quads;
        sf::Vector2f mSize;

        void __queueCells(std::vector<MazeCell *>& V, MazeCell *cell);

    public:
        Maze(sf::Vector2i dim, sf::Vector2f window_size);
        ~Maze();

        void draw(sf::RenderWindow& window);

        inline sf::Vector2f getSize() { return mSize; }
        
        void generateRecursive(sf::RenderWindow& window);
        void generatePrim(sf::RenderWindow& window);
        void generateWilson(sf::RenderWindow& window);

        void buildGraph();

        void solveBFS(sf::RenderWindow& window);
        void solveDFS(sf::RenderWindow &window);

        MazeCell *freeNeighbour(MazeCell *cell);
        void removeWalls(MazeCell *cell1, MazeCell *cell2);

        bool isGenerated = false;
        bool isSolved = false;

        int START = 0, END = mDim.x * mDim.y - 1;
    };
}