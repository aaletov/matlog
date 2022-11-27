#ifndef CONDITIONS_HPP
#define CONDITIONS_HPP

#include <vector>
#include <bdd.h>
#include "property.hpp"
#include "neighbour.hpp"
#include "constants.hpp"

namespace firstTypeCond 
{
}

namespace secTypeCond 
{
}

namespace thirdTypeCond
{
}

namespace fourthTypeCond
{
}

bdd propertyIs(const bdd p[M][N][N], const PropertyOfObject propertyOfObject);
bdd propertyEq(const bdd p[M][N][N], 
					const PropertyOfObject propertyOfObject1, 
					const PropertyOfObject propertyOfObject2);
bdd neighbourEq(const bdd p[M][N][N], const NeighbourType neighbour, const PropertyOfObject currentProperty,
					PropertyOfObject neihbourProperty);
bdd anyNeighboursEq(const bdd p[M][N][N], const PropertyOfObject currentProperty, const PropertyOfObject neihbourProperty);
void allPropValuesDistinct(bdd &tree, const bdd p[M][N][N]);
bdd allPropValuesValid(const bdd p[M][N][N]);

#endif