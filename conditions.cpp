#include "conditions.hpp"

// Condition 1
// Object i has Prop k with value j
bdd propertyIs(const bdd p[M][N][N], const PropertyOfObject objectProp)
{
	return p[objectProp.propertyNumber_][objectProp.objectNumber_][objectProp.propertyValue_];
}

// Condition 2
// (Object i has Prop k1 with value j1) XNOR (Object i has Prop k2 with value j2)
// for every i
bdd propertyEq(const bdd p[M][N][N],
          const PropertyOfObject lObjectProp, 
          const PropertyOfObject rObjectProp)
{
	bdd tree = bddtrue;
	for (int i = 0; i < N; ++i)
	{
    bdd a = p[lObjectProp.propertyNumber_][i][lObjectProp.propertyValue_];
    bdd b = p[rObjectProp.propertyNumber_][i][rObjectProp.propertyValue_];

    tree &= !(a ^ b);
	}

	return tree;
}

// Condition 3
// (Object i has Prop k with value j) XNOR (Neighbor of NeighbourType nType has Prop k with value j)
// for every i
bdd neighbourEq(const bdd p[M][N][N], 
          const NeighbourType nType, 
          const PropertyOfObject currentProperty,
					PropertyOfObject neighbourProperty)
{
	bdd tree = bddtrue;
	switch (nType)
	{
		case UPDIAG:
		{
			for (int i = 1; i < N; i++)
			{
				if (i % ROW_LENGTH == 0) {
					continue;
				}

				bdd a = p[currentProperty.propertyNumber_][i][currentProperty.propertyValue_];
				int upDiagIndex{};
				if (i < ROW_LENGTH) 
				{
					int lastRowIndex = ROW_LENGTH * (ROW_COUNT - 1);
					upDiagIndex = lastRowIndex + ((i % ROW_LENGTH) - 1);
				} else 
				{
					upDiagIndex = i - ROW_LENGTH - 1;
				}

        bdd b = p[neighbourProperty.propertyNumber_][upDiagIndex][neighbourProperty.propertyValue_];
				tree &= !(a ^ b);
			}
			break;
		}
		case LEFT:
		{
			for (int i = 1; i < N; i++)
			{
				if (i % ROW_LENGTH == 0) {
					continue;
				}

				bdd a = p[currentProperty.propertyNumber_][i][currentProperty.propertyValue_];
				bdd b = p[neighbourProperty.propertyNumber_][i - 1][neighbourProperty.propertyValue_];
				tree &= !(a ^ b);
			}
			break;
		}
	}
	return tree;
}

// Condition 4
// (Object i has Prop k with value j) XNOR (Neighbor with NeighbourType ANY has Prop k with value j)
// for every i
bdd anyNeighboursEq(const bdd p[M][N][N], 
          const PropertyOfObject currentProperty, 
          const PropertyOfObject neighbourProperty)
{
	bdd treeTmp = bddfalse;
	std::vector<NeighbourType> neighbours = {UPDIAG, LEFT};
	for (NeighbourType neighbour: neighbours)
	{
		treeTmp |= neighbourEq(p, neighbour, currentProperty, neighbourProperty);
	}
	return treeTmp;
}

// Condition 5
// (Object i1 has Prop k with value j) NIMPL (Object i2 has Prop k with value j) 
// for every i1 != i2
void allPropValuesDistinct(bdd &tree, const bdd p[M][N][N])
{
	for (int j = 0; j < N; j++)
	{
		for (int i = 0; i < N - 1; i++)
		{
			for (int k = i + 1; k < N; k++)
			{
				for (int m = 0; m < M; m++)
				{
					tree &= p[m][i][j] >> !p[m][k][j];
				}
			}
		}
	}
}

// Condition 6
// (Object has Prop)
bdd allPropValuesValid(const bdd p[M][N][N])
{
	bdd tree = bddtrue;
	for (int i = 0; i < N; i++)
	{
		for (int k = 0; k < M; k++)
		{
			bdd temp = bddfalse;
			for (int j = 0; j < N; j++)
			{
				temp |= p[k][i][j];
			}
			tree &= temp;
		}
	}
	return tree;
}
