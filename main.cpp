#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include "sudoku_utils.hpp"
#include "sudoku.hpp"

using std::cout; using std::endl; using std::vector;

const size_t window_size = 600;
const size_t size = 9;
vector<sf::RectangleShape> grid((size - 1) * 2);

int main() {

    sf::RenderWindow window(sf::VideoMode(window_size, window_size), "Sudoku-Solver");
    window.setFramerateLimit(60);
    sf::Time sleep_time = sf::seconds(1);

    create_grid(window, grid, size, window_size);
    sudoku::Sudoku test_obj("test.sdk", size, window_size);

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        cout << test_obj.fill_uniques() << endl;
        window.clear(sf::Color::White);
        test_obj.draw(window);
        draw_grid(window, grid);
        window.display();
        sf::sleep(sleep_time);
    }

    return 0;
}