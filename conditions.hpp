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
bdd propertyImplic(const bdd p[M][N][N], 
					const PropertyOfObject propertyOfObject1, 
					const PropertyOfObject propertyOfObject2);
bdd cond3(const bdd p[M][N][N], const NeighbourType neighbour, const PropertyOfObject currentProperty,
					PropertyOfObject neihbourProperty);
bdd cond4(const bdd p[M][N][N], const PropertyOfObject currentProperty, const PropertyOfObject neihbourProperty);
void cond5(bdd &tree, const bdd p[M][N][N]);
bdd cond6(const bdd p[M][N][N]);
bdd cond7(const bdd p[M][N][N]);

#endif