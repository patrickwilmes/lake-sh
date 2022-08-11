#ifndef LAKE_SH_TEXT_RENDERING_H
#define LAKE_SH_TEXT_RENDERING_H

#include <string>

inline std::string spaced(const std::string& text) {
	return " " + text + " ";
}

inline std::string red(const std::string& text) {
	return "\u001b[31m" + text + "\u001b[0m";
}

#endif