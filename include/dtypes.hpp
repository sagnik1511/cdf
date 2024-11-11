#ifndef DTYPES_HPP
#define DTYPES_HPP

#include <variant>
#include<string>


using _value = std::variant<int, char, double, std::string>;


// Helper template to check if T is a variant type
template <typename T>
struct is_variant : std::false_type {};

// Specialization for std::variant
template <typename... _value>
struct is_variant<std::variant<_value...>> : std::true_type {};

#endif