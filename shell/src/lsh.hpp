#ifndef LAKE_SH_LSH_H
#define LAKE_SH_LSH_H

#include <ncurses.h>

namespace lsh {
    class lake_shell final {
    public:
	lake_shell();
	~lake_shell();
	void run();
    private:
	void display_prompt();
    private:
	WINDOW *win = nullptr;
	bool running = true;
    };
}

#endif//LAKE_SH_LSH_H
