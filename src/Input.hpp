#pragma once

#include <vector>
#include <string>

namespace PolyhedralLibrary {
        std::string Trim(const std::string &s);
        std::vector<std::string> Split(const std::string &input);
        bool isNumeric(const std::string &s);
        std::vector<int> Input();
	int Identify(int p, int q);
}
