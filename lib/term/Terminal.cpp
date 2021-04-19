#include "Terminal.hpp"
#include <unistd.h>

constexpr int CTRL_L_KEY = 12;
constexpr int ENTER_KEY = 10;

void Term::guarded_call(const char* cmd, char** argv)
{
    reset_shell_mode();
    execvp(cmd, argv);
    reset_prog_mode();
}

Term::Terminal::Terminal()
    : m_win(initscr())
{
    clear();
    cbreak();
    noecho();
    keypad(stdscr, true);
}

void Term::Terminal::print(const std::string& msg)
{
    addstr(msg.c_str());
}

void Term::Terminal::println(const std::string& msg)
{
    addstr(msg.c_str());
    waddch(m_win, '\n');
}

Term::Input Term::Terminal::get_input()
{
    char* input_buf = (char*)calloc(MAX_INPUT_LEN, sizeof(char));
    bool collecting_input = true;
    Keys pressed_key = NONE;
    int x, y;
    getyx(m_win, y, x);
    noecho();
    refresh();
    while (collecting_input) {
        int c = getch();
        switch (c) {
        case KEY_UP: {
            //                std::string h_elem = m_history.next();
            //                mvaddstr(y, x, h_elem.c_str());
            pressed_key = KEY_ARROW_UP;
        } break;
        case KEY_LEFT: {
            int cx, cy;
            getyx(m_win, cy, cx);
            cx--;
            move(cy, cx);
        } break;
        case KEY_RIGHT: {
            int cx, cy;
            getyx(m_win, cy, cx);
            cx++;
            move(cy, cx);
        } break;
        case KEY_ENTER:
        case ENTER_KEY:
            mvwinstr(m_win, y, x, input_buf);
            collecting_input = false;
            break;
        case KEY_DC: {
            int cx, cy;
            getyx(m_win, cy, cx);
            mvdelch(cy, cx);
        } break;
        case CTRL_L_KEY: {
            collecting_input = false;
            input_buf = (char*)calloc(MAX_INPUT_LEN, sizeof(char));
            clear();
        } break;
        case KEY_BACKSPACE:
        case 127:
            int cx, cy;
            getyx(m_win, cy, cx);
            mvdelch(cy, cx - 1);
            break;
        default:
            echochar(c);
            break;
        }
    }
    echo();
    std::string input(input_buf);
    free(input_buf);
    addstr(input_buf);
    return { .input = input, .key = pressed_key };
}

Term::Terminal::~Terminal()
{
    endwin();
}

void Term::Terminal::print_next_line(const std::string& msg)
{
    int y, x;
    getyx(m_win, y, x);
    y++;
    mvaddstr(y, 0, msg.c_str());
}

void Term::Terminal::display_prompt(const std::string& msg)
{
    int y, x;
    getyx(m_win, y, x);
    y++;
    echo();
    start_color();
    mvaddstr(y, 0, msg.c_str());
    noecho();
}

void Term::Terminal::register_observer(const std::shared_ptr<Term::TerminalObserver>& observer)
{
    m_observers.push_back(observer);
}
