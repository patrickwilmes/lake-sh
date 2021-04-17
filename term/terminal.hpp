#ifndef LAKE_SH_TERMINAL_HPP
#define LAKE_SH_TERMINAL_HPP

#include <ncurses.h>
#include <string>

namespace term {
    enum keys {
        NONE,
        KEY_ARROW_UP,
        KEY_CTRL_L
    };

    typedef struct {
        std::string input;
        keys key;
    } input_t;

    class terminal final {
    public:
        terminal();
        ~terminal();
        void print(const std::string &msg);
        void println(const std::string &msg);
        input_t get_input();
    private:
        static constexpr uint32_t MAX_INPUT_LEN = 2048;
        WINDOW *m_win;
    };
}// namespace term

#endif//LAKE_SH_TERMINAL_HPP
