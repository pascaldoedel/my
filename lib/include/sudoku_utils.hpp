#pragma once

void create_grid(sf::RenderWindow &my_window, std::vector<sf::RectangleShape> &grid, size_t size, size_t window_size) {

    for (size_t i = 0; i < size - 1; i++) {
        grid[i] = sf::RectangleShape(sf::Vector2f(600.0, 3.0));
        grid[i].setFillColor(sf::Color::Black);
        grid[i].setPosition(0.0, (i + 1) * static_cast<double>(window_size) / size);
        unsigned int root = static_cast<int>(round(sqrt(size)));
        if ((i + 1) % root == 0) grid[i].setScale(1.0, 2.5);
    }

    for (size_t i = 0; i < size - 1; i++) {
        grid[i + size - 1] = sf::RectangleShape(sf::Vector2f(3.0, 600.0));
        grid[i + size - 1].setFillColor(sf::Color::Black);
        grid[i + size - 1].setPosition((i + 1) * static_cast<double>(window_size) / size, 0);
        unsigned int root = static_cast<int>(round(sqrt(size)));
        if ((i + 1) % root == 0) grid[i + size - 1].setScale(2.5, 1.0);
    }
}

void draw_grid(sf::RenderWindow &my_window, std::vector<sf::RectangleShape> &grid) {
    for (size_t i = 0; i < grid.size(); i++) {
        my_window.draw(grid[i]);
    }
}

void color_square(sf::RenderWindow &my_window, size_t size, size_t window_size, size_t xpos, size_t ypos) {
    double sl = static_cast<double>(window_size) / size;
    sf::RectangleShape square(sf::Vector2f(sl, sl));
    square.setFillColor(sf::Color::Green);
    square.setPosition(xpos * sl, ypos * sl);
    my_window.draw(square);
}