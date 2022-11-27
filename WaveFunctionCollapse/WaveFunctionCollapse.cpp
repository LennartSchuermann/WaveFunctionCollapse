#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Cell.cpp"
#include "Constants.h"

int main()
{
    if (SCREENSIZE % CELLSIZE != 0) {
        std::cout << "Error, unvalid Screen-Cell-Size correlation!" << std::endl;
        return 0;
    }

    sf::RenderWindow window(sf::VideoMode(SCREENSIZE, SCREENSIZE), "Wave Function Collapse by Lennart S.", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);

    //Game clock
    sf::Clock clock;

    //Draw grid of cells
    std::vector<Cell> cells;
    for (size_t y = 0; y < SCREENSIZE; y += CELLSIZE)
    {
        for (size_t x = 0; x < SCREENSIZE; x += CELLSIZE)
        {
            Cell newCell(sf::Vector2f((float)x, (float)y));
            cells.push_back(newCell);
        }
    }

    //sorted cells by entropy
    std::vector<Cell*> cellsEntropySorted;
    for (size_t i = 0; i < cells.size(); i++)
    {
        cellsEntropySorted.push_back(&cells[i]);
    }

    int rcSize = SCREENSIZE / CELLSIZE;     //Row-Column-Size

    //set neighbours
    for (int i = 0; i < cells.size(); i++)
    {
        //save neighbour to the right (if exist)
        if ((i + 1) % rcSize > 0 && i + 1 < cells.size()) {
            cells[i].neighbours.push_back(&cells[i + 1]);
        }

        //save neighbour to the left (if exist) 
        if ((i - 1) % rcSize != rcSize - 1 && i - 1 > 0) {
            cells[i].neighbours.push_back(&cells[i - 1]);
        }

        //save neighbour below (if exist)
        if ((i + rcSize) < cells.size()) {
            cells[i].neighbours.push_back(&cells[i + rcSize]);
        }

        //save neighbour on top (if exist)
        if ((i - rcSize) >= 0) {
            cells[i].neighbours.push_back(&cells[i - rcSize]);
        }
    }

    cells[5].collapse(true);

    //Run the program as long as the window is open
    while (window.isOpen() && !cellsEntropySorted.empty())
    {
        float time = clock.restart().asSeconds();
        float fps = 1.0f / time;

        //Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //Update stuff...
        cellsEntropySorted.clear();
        for (size_t i = 0; i < cells.size(); i++)
        {
            if (cells[i].entropy != -1)
                cellsEntropySorted.push_back(&cells[i]);
        }

        std::sort(cellsEntropySorted.begin(), cellsEntropySorted.end(), lessThan());

        if(!cellsEntropySorted.empty())
            cellsEntropySorted[0]->collapse(false);

        //update cells

        //Clear the window with black color
        window.clear(sf::Color(61, 61, 61));

        //Draw stuff...
        for (const auto& cell : cells) {
            window.draw(cell.shape);
        }

        //End the current frame
        window.display();
    }

    std::cin.get();

    return 0;
}