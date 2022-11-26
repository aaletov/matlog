#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cmath>

const inline std::size_t N = 9;
const inline std::size_t M = 4;
const inline std::size_t ROW_LENGTH = 3;
const inline std::size_t K = 8;
const inline int LOG_N = static_cast<int>(std::ceil(std::log2(N)));;

const inline std::size_t VAR_NUM = 144;
char inline var[VAR_NUM];

#endif