#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <bdd.h>

const std::size_t N = 9;
const std::size_t M = 4;
const std::size_t ROW_LENGTH = 3;
const std::size_t K = 8;
const int LOG_N = static_cast<int>(std::ceil(std::log2(N)));;

const std::size_t VAR_NUM = 144;
char var[VAR_NUM];
enum Neighbour {UP, DOWN, LEFT, RIGHT};

struct PropertyOfObject
{
	PropertyOfObject(int propertyNumber, int objectNumber, int propertyValue):
		propertyNumber_(propertyNumber), objectNumber_(objectNumber), propertyValue_(propertyValue) {}

	int propertyNumber_;
	int objectNumber_;
	int propertyValue_;
};

void init(bdd p[M][N][N]) // Заполнение от 0 до 2^logN
{
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			for (int k = 0; k < N; ++k)
			{
				p[i][j][k] = bddtrue;
				for (int l = 0; l < LOG_N; ++l)
				{
					p[i][j][k] &= ((k >> l) & 1) ? bdd_ithvar((j * LOG_N * M) + l + LOG_N * i) : bdd_nithvar((j * LOG_N * M) + l + LOG_N * i);
				}
			}
		}
	}
}

bdd cond1(const bdd p[M][N][N], const PropertyOfObject propertyOfObject)
{
	return p[propertyOfObject.propertyNumber_][propertyOfObject.objectNumber_][propertyOfObject.propertyValue_];
}

bdd cond2(const bdd p[M][N][N], const PropertyOfObject propertyOfObject1, const PropertyOfObject propertyOfObject2)
{
	bdd tree = bddtrue;
	for (int i = 0; i < N; ++i)
	{
		tree &= !(p[propertyOfObject1.propertyNumber_][i][propertyOfObject1.propertyValue_] ^ p[propertyOfObject2.propertyNumber_][i][propertyOfObject2.propertyValue_]);
	}
	return tree;
}

bdd cond3(const bdd p[M][N][N], const Neighbour neighbour, const PropertyOfObject currentProperty,
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

bdd cond4(const bdd p[M][N][N], const PropertyOfObject currentProperty, const PropertyOfObject neihbourProperty)
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

void print()
{
	for (unsigned i = 0; i < N; i++)
	{
		std::cout << i << ": ";
		for (unsigned j = 0; j < M; j++)
		{
			int J = i * M * LOG_N + j * LOG_N;
			int num = 0;
			for (unsigned k = 0; k < LOG_N; k++) num += (unsigned) (var[J + k] << k);
			std::cout << num << ' ';
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void build(char *varset, unsigned n, unsigned I)
{
	if (I == n - 1) {
		if (varset[I] >= 0)
		{
			var[I] = varset[I];
			print();
			return;
		}
		var[I] = 0;
		print();
		var[I] = 1;
		print();
		return;
	}
	if (varset[I] >= 0)
	{
		var[I] = varset[I];
		build(varset, n, I + 1);
		return;
	}
	var[I] = 0;
	build(varset, n, I + 1);
	var[I] = 1;
	build(varset, n, I + 1);
}

// Обработчик для bdd
void fun(char *varset, int size)
{
	build(varset, size, 0);
}

void program()
{

	bdd_init(50000000, 7000000);
	bdd_setvarnum(VAR_NUM);
	bdd p[M][N][N];
	init(p);
	bdd tree = bddtrue;

	tree &= cond6(p);

	tree &= cond1(p, PropertyOfObject(1, 4, 8)); //+
	tree &= cond1(p, PropertyOfObject(2, 0, 5)); //+
	tree &= cond1(p, PropertyOfObject(3, 2, 5)); //+

	// Дополнительные условия типа 1
	tree &= cond1(p, PropertyOfObject(0, 7, 1)); //+
	tree &= cond1(p, PropertyOfObject(2, 3, 1)); //+
	tree &= cond1(p, PropertyOfObject(0, 2, 6)); //+
	tree &= cond1(p, PropertyOfObject(0, 1, 5)); //+
	tree &= cond1(p, PropertyOfObject(2, 8, 6)); //+
	tree &= cond1(p, PropertyOfObject(1, 5, 1)); //+
	tree &= cond1(p, PropertyOfObject(0, 0, 8)); //+
	tree &= cond1(p, PropertyOfObject(3, 6, 6)); //+

	tree &= cond2(p, PropertyOfObject(0, -1, 0), PropertyOfObject(2, -1, 3)); //+
	tree &= cond2(p, PropertyOfObject(1, -1, 5), PropertyOfObject(3, -1, 3)); //+

	// Дополнительные условия типа 2
	tree &= tree &= cond2(p, PropertyOfObject(0, -1, 6), PropertyOfObject(2, -1, 8)); //+
	tree &= tree &= cond2(p, PropertyOfObject(0, -1, 5), PropertyOfObject(2, -1, 0)); //+

	tree &= cond3(p, Neighbour::LEFT, PropertyOfObject(0, -1, 7), PropertyOfObject(1, -1, 2)); //+
	tree &= cond3(p, Neighbour::RIGHT, PropertyOfObject(1, -1, 8), PropertyOfObject(3, -1, 2)); //+
	tree &= cond3(p, Neighbour::UP, PropertyOfObject(3, -1, 7), PropertyOfObject(0, -1, 5)); //+
	tree &= cond3(p, Neighbour::DOWN, PropertyOfObject(2, -1, 4), PropertyOfObject(0, -1, 1)); //+
	tree &= cond3(p, Neighbour::RIGHT, PropertyOfObject(2, -1, 7), PropertyOfObject(1, -1, 0)); //+

	// Дополнительные условия типа 3
	tree &= cond3(p, Neighbour::RIGHT, PropertyOfObject(1, -1, 6), PropertyOfObject(2, -1, 0)); //+
	tree &= cond3(p, Neighbour::DOWN, PropertyOfObject(0, -1, 2), PropertyOfObject(0, -1, 4)); //+
	tree &= cond3(p, Neighbour::DOWN, PropertyOfObject(1, -1, 7), PropertyOfObject(3, -1, 2)); //+

	tree &= cond7(p);

	tree &= cond4(p, PropertyOfObject(0, -1, 4), PropertyOfObject(1, -1, 0)); //+
	tree &= cond4(p, PropertyOfObject(0, -1, 8), PropertyOfObject(1, -1, 2)); //+
	tree &= cond4(p, PropertyOfObject(2, -1, 4), PropertyOfObject(3, -1, 4)); //+
	tree &= cond4(p, PropertyOfObject(2, -1, 6), PropertyOfObject(1, -1, 1)); //+
	tree &= cond4(p, PropertyOfObject(3, -1, 0), PropertyOfObject(0, -1, 8)); //+

	// Дополнительные условия типа 4
	tree &= cond4(p, PropertyOfObject(2, -1, 5), PropertyOfObject(3, -1, 1)); //+

	cond5(tree, p);

	std::cout << bdd_satcount(tree) << " solution(s):\n";
	bdd_allsat(tree, fun);
	// std::cout << tree << "\n";
	bdd_done();
}

int main()
{
	if (N % ROW_LENGTH != 0)
	{
		std::cerr << "Invalid arguments\n";
		return 1;
	}
	program();
}
