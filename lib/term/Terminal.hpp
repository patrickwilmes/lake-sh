#ifndef LAKE_SH_TERMINAL_HPP
#define LAKE_SH_TERMINAL_HPP

#include <ncurses.h>
#include <string>

namespace Term {

    void guarded_call(const char * cmd, char ** argv);

    enum Keys {
        NONE,
        KEY_ARROW_UP,
        KEY_CTRL_L
    };

    typedef struct {
        std::string input;
        Keys key;
    } Input;

    class Terminal final {
    public:
        Terminal();
        ~Terminal();
        void print(const std::string &msg);
        void println(const std::string &msg);
        void print_next_line(const std::string &msg);
        void display_prompt(const std::string &msg);
        Input get_input();
    private:
        static constexpr uint32_t MAX_INPUT_LEN = 2048;
        WINDOW *m_win;
    };
}// namespace Term

#endif//LAKE_SH_TERMINAL_HPP
