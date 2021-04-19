#ifndef LAKE_SH_TERMINALOBSERVER_HPP
#define LAKE_SH_TERMINALOBSERVER_HPP

#include <string>

namespace Term {
    class TerminalObserver {
    public:
        virtual void notify(std::string &msg) = 0;
    };
}

#endif //LAKE_SH_TERMINALOBSERVER_HPP
