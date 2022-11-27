#include "conditions.hpp"

// Object i has Prop k with value j
bdd propertyIs(const bdd p[M][N][N], const PropertyOfObject objectProp)
{
	return p[objectProp.propertyNumber_][objectProp.objectNumber_][objectProp.propertyValue_];
}

// (Object has Prop k1 with value j1) NAND (Object has Prop k2 with value j2)
bdd propertyImplic(const bdd p[M][N][N],
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

// (Current object i has Prop k with value j) NAND (Neighbor of NeighbourType nType ...)
bdd cond3(const bdd p[M][N][N], 
          const NeighbourType nType, 
          const PropertyOfObject currentProperty,
					PropertyOfObject neihbourProperty)
{
	bdd tree = bddtrue;
	switch (nType)
	{
		case UP:
		{
			for (int i = 0; i < N - ROW_LENGTH; ++i)
			{
        bdd a = p[currentProperty.propertyNumber_][i + ROW_LENGTH][currentProperty.propertyValue_];
        bdd b = p[neihbourProperty.propertyNumber_][i][neihbourProperty.propertyValue_];

				tree &= !(a ^ b);
			}
			break;
		}
		case DOWN:
		{
			for (int i = ROW_LENGTH; i < N; ++i)
			{
				tree &= (!p[currentProperty.propertyNumber_][i - ROW_LENGTH][currentProperty.propertyValue_] ^ p[neihbourProperty.propertyNumber_][i][neihbourProperty.propertyValue_]);
			}
			break;
		}
		case LEFT:
		{
			for (int i = 0; i < N; ++i)
			{
				if ((i + 1) % ROW_LENGTH != 0)
				{
					tree &= !(p[currentProperty.propertyNumber_][i + 1][currentProperty.propertyValue_] ^ p[neihbourProperty.propertyNumber_][i][neihbourProperty.propertyValue_]);
				}
			}
			break;
		}
		case RIGHT:
		{
			for (int i = 0; i < N; ++i)
			{
				if (i % ROW_LENGTH != 0)
				{
					tree &= !(p[currentProperty.propertyNumber_][i - 1][currentProperty.propertyValue_] ^ p[neihbourProperty.propertyNumber_][i][neihbourProperty.propertyValue_]);
				}
			}
			break;
		}
	}
	return tree;
}

bdd cond4(const bdd p[M][N][N], 
          const PropertyOfObject currentProperty, 
          const PropertyOfObject neihbourProperty)
{
	bdd treeTmp = bddfalse;
	std::vector<NeighbourType> neighbours = {UP, DOWN, RIGHT, LEFT};
	for (NeighbourType neighbour: neighbours)
	{
		treeTmp |= cond3(p, neighbour, currentProperty, neihbourProperty);
	}
	return treeTmp;
}

void cond5(bdd &tree, const bdd p[M][N][N])
{

	for (unsigned j = 0; j < N; j++)
	{
		for (unsigned i = 0; i < N - 1; i++)
		{
			for (unsigned k = i + 1; k < N; k++)
			{
				for (unsigned m = 0; m < M; m++)
				{
					tree &= p[m][i][j] >> !p[m][k][j];
				}
			}
		}
	}
}

bdd cond6(const bdd p[M][N][N])
{
	bdd tree = bddtrue;
	for (int i = 0; i < N; i++)
	{
		for (int k = 0; k < M; ++k)
		{
			bdd temp0 = bddfalse;
			for (int j = 0; j < N; j++)
			{
				temp0 |= p[k][i][j];
			}
			tree &= temp0;
		}
	}
	return tree;
}

bdd cond7(const bdd p[M][N][N])
{
	bdd tree = bddtrue;
	for (int i = 1; i < N; i += 2)
	{
		bdd temp = bddfalse;
		bdd temp1 = bddfalse;
		for (int j1 = 0; j1 < N && j1 <= K; j1++)
		{
			temp1 |= p[0][i][j1];
			bdd temp2 = bddfalse;
			for (int j2 = 0; j2 < N && (j1 + j2) <= K; j2++)
			{
				temp2 |= p[1][i][j2];
				bdd temp3 = bddfalse;
				for (int j3 = 0; j3 < N && (j1 + j2 + j3) <= K; j3++)
				{
					temp3 |= p[2][i][j3];
					bdd temp4 = bddfalse;
					for (int j4 = 0; j4 < N && (j1 + j2 + j3 + j4) <= K; j4++)
					{
						temp4 |= p[3][i][j4];
					}
					temp |= temp1 & temp2 & temp3 & temp4;
				}
			}
		}
		tree &= temp;
	}
	return tree;
}