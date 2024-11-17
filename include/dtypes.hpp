#ifndef DTYPES_HPP
#define DTYPES_HPP

#include <variant>

using _cdfVal = std::variant<std::string, int, double>;

namespace cdf {
std::vector<std::string> dTypeWithRank = {"int", "double", "string"};

}  // namespace cdf

#endif