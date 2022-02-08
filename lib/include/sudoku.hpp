#include <bitset>
#include <cmath>
#include "sudoku_utils.hpp"

namespace sudoku {
    using std::cout; using std::endl; using std::vector;


    struct candidates {
        bool solved = false;
        std::bitset<9> c;

        candidates() { c.set(); };
    };

    class Sudoku {

        const size_t size;
        const size_t window_size;
        const size_t box_size = static_cast<size_t>(round(sqrt(size)));
        const size_t font_size = 50;
        const double scaling;
        vector<vector<sf::Text> > text_field;
        vector<vector<candidates> > candidate_field;
        vector<vector<int> > number_field;
        sf::Font font;

    private:
        void insertNumber(size_t number, size_t xpos, size_t ypos, bool constructor);

        void eliminateNumber(size_t number, size_t xpos, size_t ypos);

        void eliminateRow(size_t number, size_t ypos);

        void eliminateCollumn(int number, size_t xpos);

        void eliminateBox(int number, size_t xpos, size_t ypos);


        void eliminateForImplicitDoubles();

        size_t get_box_count(size_t xpos, size_t ypos, size_t number);

        std::vector<std::pair<size_t, size_t> > get_box_candidates(size_t xpos, size_t ypos, size_t number);

        bool check_unique_field(size_t xpos, size_t ypos);

        bool check_unique_in_box(size_t xpos, size_t ypos, size_t number);

        bool check_unique_in_row(size_t ypos, size_t number);

        bool check_unique_in_collumn(size_t xpos, size_t number);


    public:
        Sudoku(std::string file_path, size_t size, size_t window_size);

        void draw(sf::RenderWindow &my_window);

        bool fill_uniques();

    };


}

#include "../src/sudoku.cpp"