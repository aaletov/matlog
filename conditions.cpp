#include "conditions.hpp"

bdd cond1(const bdd p[M][N][N], const PropertyOfObject propertyOfObject)
{
	return p[propertyOfObject.propertyNumber_][propertyOfObject.objectNumber_][propertyOfObject.propertyValue_];
}

bdd cond2(const bdd p[M][N][N],
          const PropertyOfObject propertyOfObject1, 
          const PropertyOfObject propertyOfObject2)
{
	bdd tree = bddtrue;
	for (int i = 0; i < N; ++i)
	{
		tree &= !(p[propertyOfObject1.propertyNumber_][i][propertyOfObject1.propertyValue_] ^ p[propertyOfObject2.propertyNumber_][i][propertyOfObject2.propertyValue_]);
	}
	return tree;
}

bdd cond3(const bdd p[M][N][N], 
          const Neighbour neighbour, 
          const PropertyOfObject currentProperty,
					PropertyOfObject neihbourProperty)
{
	bdd tree = bddtrue;
	switch (neighbour)
	{
		case UP:
		{
			for (int i = 0; i < N - ROW_LENGTH; ++i)
			{
				tree &= !(p[currentProperty.propertyNumber_][i + ROW_LENGTH][currentProperty.propertyValue_] ^ p[neihbourProperty.propertyNumber_][i][neihbourProperty.propertyValue_]);
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
	std::vector<Neighbour> neighbours = {UP, DOWN, RIGHT, LEFT};
	for (Neighbour neighbour: neighbours)
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