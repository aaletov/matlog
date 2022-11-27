#include <iostream>
#include <fstream>
#include "conditions.hpp"

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

	tree &= propertyIs(p, PropertyOfObject(1, 4, 8)); //+
	tree &= propertyIs(p, PropertyOfObject(2, 0, 5)); //+
	tree &= propertyIs(p, PropertyOfObject(3, 2, 5)); //+

	// Дополнительные условия типа 1
	tree &= propertyIs(p, PropertyOfObject(0, 7, 1)); //+
	tree &= propertyIs(p, PropertyOfObject(2, 3, 1)); //+
	tree &= propertyIs(p, PropertyOfObject(0, 2, 6)); //+
	tree &= propertyIs(p, PropertyOfObject(0, 1, 5)); //+
	tree &= propertyIs(p, PropertyOfObject(2, 8, 6)); //+
	tree &= propertyIs(p, PropertyOfObject(1, 5, 1)); //+
	tree &= propertyIs(p, PropertyOfObject(0, 0, 8)); //+
	tree &= propertyIs(p, PropertyOfObject(3, 6, 6)); //+

	tree &= propertyNAND(p, PropertyOfObject(0, -1, 0), PropertyOfObject(2, -1, 3)); //+
	tree &= propertyNAND(p, PropertyOfObject(1, -1, 5), PropertyOfObject(3, -1, 3)); //+

	// Дополнительные условия типа 2
	tree &= tree &= propertyNAND(p, PropertyOfObject(0, -1, 6), PropertyOfObject(2, -1, 8)); //+
	tree &= tree &= propertyNAND(p, PropertyOfObject(0, -1, 5), PropertyOfObject(2, -1, 0)); //+

	tree &= cond3(p, NeighbourType::LEFT, PropertyOfObject(0, -1, 7), PropertyOfObject(1, -1, 2)); //+
	tree &= cond3(p, NeighbourType::RIGHT, PropertyOfObject(1, -1, 8), PropertyOfObject(3, -1, 2)); //+
	tree &= cond3(p, NeighbourType::UP, PropertyOfObject(3, -1, 7), PropertyOfObject(0, -1, 5)); //+
	tree &= cond3(p, NeighbourType::DOWN, PropertyOfObject(2, -1, 4), PropertyOfObject(0, -1, 1)); //+
	tree &= cond3(p, NeighbourType::RIGHT, PropertyOfObject(2, -1, 7), PropertyOfObject(1, -1, 0)); //+

	// Дополнительные условия типа 3
	tree &= cond3(p, NeighbourType::RIGHT, PropertyOfObject(1, -1, 6), PropertyOfObject(2, -1, 0)); //+
	tree &= cond3(p, NeighbourType::DOWN, PropertyOfObject(0, -1, 2), PropertyOfObject(0, -1, 4)); //+
	tree &= cond3(p, NeighbourType::DOWN, PropertyOfObject(1, -1, 7), PropertyOfObject(3, -1, 2)); //+

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
