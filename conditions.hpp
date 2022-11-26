#ifndef CONDITIONS_HPP
#define CONDITIONS_HPP

#include <vector>
#include <bdd.h>
//#include "constants.hpp"
#include "property.hpp"
#include "neighbour.hpp"
#include "constants.hpp"
// const std::size_t N = 9;
// const std::size_t M = 4;
// const std::size_t ROW_LENGTH = 3;
// const std::size_t K = 8;
// const int LOG_N = static_cast<int>(std::ceil(std::log2(N)));;

// const std::size_t VAR_NUM = 144;
// char var[VAR_NUM];

bdd cond1(const bdd p[M][N][N], const PropertyOfObject propertyOfObject);
bdd cond2(const bdd p[M][N][N], const PropertyOfObject propertyOfObject1, const PropertyOfObject propertyOfObject2);
bdd cond3(const bdd p[M][N][N], const Neighbour neighbour, const PropertyOfObject currentProperty,
					PropertyOfObject neihbourProperty);
bdd cond4(const bdd p[M][N][N], const PropertyOfObject currentProperty, const PropertyOfObject neihbourProperty);
void cond5(bdd &tree, const bdd p[M][N][N]);
bdd cond6(const bdd p[M][N][N]);
bdd cond7(const bdd p[M][N][N]);

#endif