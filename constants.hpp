#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cmath>

// N is the number of objects, N is the number of propertie's values
const inline std::size_t N = 9;

// M is the number of properties
const inline std::size_t M = 4;

// ROW_LENGTH is the length of one row
const inline std::size_t ROW_LENGTH = 3;

// ROW_COUNT is the count of rows
const inline std::size_t ROW_COUNT = N / ROW_LENGTH;

// Constant for 7'th condition. Sum of neighbour-object's properties MUST be more than K
const inline std::size_t K = 8;

// Number of variables for encoding
const inline int LOG_N = static_cast<int>(std::ceil(std::log2(N)));;

// ??
const inline std::size_t VAR_NUM = 144;
char inline var[N];

#endif