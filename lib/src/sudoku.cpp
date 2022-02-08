sudoku::Sudoku::Sudoku(std::string file_path, size_t psize, size_t pwindow_size) :
        scaling(static_cast<double>(pwindow_size) / size),
        window_size(pwindow_size),
        size(psize) {
    if (!font.loadFromFile("arial.ttf")) cout << "Could not load font" << endl;
    std::ifstream input_file(file_path);
    if (input_file.is_open()) {
        number_field = vector < vector < int > > (size, vector<int>(size));
        text_field = vector < vector < sf::Text > > (size, vector<sf::Text>(size));
        candidate_field = vector < vector < sudoku::candidates > > (size, vector<sudoku::candidates>(size));
        for (size_t i = 0; i < size; i++)
            for (size_t j = 0; j < size; j++) {
                size_t number;
                input_file >> number;
                insertNumber(number, i, j, true);
            }

        input_file.close();
    } else cout << "File could not be opened" << endl;
}


void sudoku::Sudoku::draw(sf::RenderWindow &my_window) {
    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++) {
            if (candidate_field[i][j].c[6] == false) color_square(my_window, size, window_size, j, i);
            if (number_field[i][j] != 0) my_window.draw(text_field[i][j]);

        }
}


void sudoku::Sudoku::insertNumber(size_t number, size_t xpos, size_t ypos, bool constructor) {
    number_field[xpos][ypos] = number;

    text_field[xpos][ypos] = sf::Text(std::to_string(number_field[xpos][ypos]), font, font_size);
    text_field[xpos][ypos].setPosition(scaling * ypos + font_size / 2, scaling * xpos);

    if (constructor) { text_field[xpos][ypos].setFillColor(sf::Color::Black); }
    else { text_field[xpos][ypos].setFillColor(sf::Color::Red); }

    text_field[xpos][ypos].setStyle(sf::Text::Bold);

    if (number != 0) {
        candidate_field[xpos][ypos].solved = true;
        candidate_field[xpos][ypos].c.reset();
    }
    eliminateNumber(number, xpos, ypos);
}


void sudoku::Sudoku::eliminateNumber(size_t number, size_t xpos, size_t ypos) {
    eliminateRow(number, ypos);
    eliminateCollumn(number, xpos);
    eliminateBox(number, xpos, ypos);
}


void sudoku::Sudoku::eliminateRow(size_t number, size_t ypos) {
    for (size_t i = 0; i < size; i++) candidate_field[i][ypos].c[number - 1] = false;
}


void sudoku::Sudoku::eliminateCollumn(int number, size_t xpos) {
    for (size_t i = 0; i < size; i++) candidate_field[xpos][i].c[number - 1] = false;
}


void sudoku::Sudoku::eliminateBox(int number, size_t xpos, size_t ypos) {
    size_t box_pos_X = xpos / box_size;
    size_t box_pos_Y = ypos / box_size;

    for (size_t i = 0; i < box_size; i++)
        for (size_t j = 0; j < box_size; j++)
            candidate_field[i + box_pos_X * box_size][j + box_pos_Y * box_size].c[number - 1] = false;
}


bool sudoku::Sudoku::check_unique_field(size_t xpos, size_t ypos) {
    if (candidate_field[xpos][ypos].c.count() == 1)
        for (size_t k = 0; k < size; k++)
            if (candidate_field[xpos][ypos].c[k] == 1) {
                insertNumber(k + 1, xpos, ypos, false);
                return true;
            }
    return false;
}


void sudoku::Sudoku::eliminateForImplicitDoubles() {

    for (size_t h = 0; h < size; h++)
        for (size_t i = 0; i < box_size; i++)
            for (size_t j = 0; j < box_size; j++) {
                auto vec = get_box_candidates(i, j, h + 1);
                if (vec.size() == 2) {
                    if (vec[0].first == vec[1].first) {
                        eliminateCollumn(h + 1, vec[0].first);
                        candidate_field[vec[0].first][vec[0].second].c[h] = 1;
                        candidate_field[vec[1].first][vec[1].second].c[h] = 1;
                    }
                    if (vec[0].second == vec[1].second) {
                        eliminateRow(h + 1, vec[0].second);
                        candidate_field[vec[0].first][vec[0].second].c[h] = 1;
                        candidate_field[vec[1].first][vec[1].second].c[h] = 1;
                    }
                }
            }
}


size_t sudoku::Sudoku::get_box_count(size_t xpos, size_t ypos, size_t number) {
    size_t sum = 0;
    for (size_t i = 0; i < box_size; i++)
        for (size_t j = 0; j < box_size; j++)
            sum += candidate_field[xpos * box_size + i][ypos * box_size + j].c[number - 1];
    return sum;
}


std::vector<std::pair<size_t, size_t> > sudoku::Sudoku::get_box_candidates(size_t xpos, size_t ypos, size_t number) {
    std::vector<std::pair<size_t, size_t> > box_candidates;
    for (size_t i = 0; i < box_size; i++)
        for (size_t j = 0; j < box_size; j++)
            if (candidate_field[xpos * box_size + i][ypos * box_size + j].c[number - 1])
                box_candidates.push_back(std::pair<size_t, size_t>(xpos * box_size + i, ypos * box_size + j));
    return box_candidates;
}


bool sudoku::Sudoku::check_unique_in_box(size_t xpos, size_t ypos, size_t number) {
    size_t box_pos_X = xpos / box_size;
    size_t box_pos_Y = ypos / box_size;
    bool changed = false;

    if (get_box_count(box_pos_X, box_pos_Y, number) == 1) {
        for (size_t i = 0; i < box_size; i++)
            for (size_t j = 0; j < box_size; j++)
                if (candidate_field[i + box_pos_X * box_size][j + box_pos_Y * box_size].c[number - 1]) {
                    insertNumber(number, i + box_pos_X * box_size, j + box_pos_Y * box_size, false);
                    return true;
                }
    }
    return false;
}

bool sudoku::Sudoku::check_unique_in_row(size_t ypos, size_t number) {
    size_t sum = 0;
    for (size_t i = 0; i < size; i++)
        sum += candidate_field[i][ypos].c[number - 1];
    if (sum == 1) {
        for (size_t i = 0; i < size; i++)
            if (candidate_field[i][ypos].c[number - 1]) insertNumber(number, i, ypos, false);
        return true;
    } else {
        return false;
    }

}

bool sudoku::Sudoku::check_unique_in_collumn(size_t xpos, size_t number) {
    size_t sum = 0;
    for (size_t i = 0; i < size; i++)
        sum += candidate_field[xpos][i].c[number - 1];
    if (sum == 1) {
        for (size_t i = 0; i < size; i++)
            if (candidate_field[xpos][i].c[number - 1]) insertNumber(number, xpos, i, false);
        return true;
    } else {
        return false;
    }
}

bool sudoku::Sudoku::fill_uniques() {
    bool changed = false;
    eliminateForImplicitDoubles();
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            if (!candidate_field[i][j].solved) {
                changed = changed || check_unique_field(i, j);
                for (size_t k = 0; k < size; k++) {
                    changed = changed || check_unique_in_row(j, k + 1);
                    changed = changed || check_unique_in_collumn(i, k + 1);
                    changed = changed || check_unique_in_box(i, j, k + 1);
                }
            }
        }
    }
    return changed;
}