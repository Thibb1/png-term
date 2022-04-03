#include "PngTerm.hpp"
int main(int ac, char *av[]) {
    if (ac < 2) {
        std::cout << "Usage: " << av[0] << " path_to_img.png" << std::endl;
    }
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    use_default_colors();
    noecho();
    curs_set(0);
    for (int i = 0; i < COLORS; i++)
        init_extended_pair(i + 1, i, -1);
    for (int i = 1; i < ac; i++) {
        wclear(stdscr);
        if (std::filesystem::exists(av[i])) {
            Converter converter(av[i]);
            converter.processImage({0, 0});
            wrefresh(stdscr);
            getch();
        }
    }
    endwin();
}