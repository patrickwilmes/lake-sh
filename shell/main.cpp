#include "lsh.hpp"
#include <cstdlib>
#include <iostream>
#include <ncurses.h>

int main() {
    lsh::lake_shell shell;
    shell.run();
    //    lsh::run();
    /*
    int i = 0;
    char *inbuf = (char *) malloc(sizeof(char) * 2048);
    bool run = true;
    WINDOW *m_win;
    m_win = initscr();
    clear();
    cbreak();
    noecho();
    keypad(stdscr, true);
    while (run) {
	int c = getch();
	switch(c) {
	    case KEY_DOWN:
		mvwinstr(m_win, 0, 0, inbuf);
		run = false;
		break;
	    case KEY_UP:
		std::cout << "key up" << std::endl;
		break;
	    case KEY_BACKSPACE:
	    case KEY_DC:
	    case 127:
		int x, y;
		getyx(m_win, y, x);
		mvdelch(y, x-1);
		break;
	    default:
		{
		    echochar(c);
		    //inbuf[i] = (char) c;
		    //++i;
		}
		break;
	}
    }
    endwin();
    std::cout << inbuf << std::endl;
    free(inbuf);
    */
    return EXIT_SUCCESS;
}
