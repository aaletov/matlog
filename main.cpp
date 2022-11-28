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

	tree &= allPropValuesValid(p);

	// Условия типа 1
	tree &= propertyIs(p, PropertyOfObject(0, 7, 1)); //+
	tree &= propertyIs(p, PropertyOfObject(2, 3, 1)); //+
	tree &= propertyIs(p, PropertyOfObject(0, 2, 6)); //+
	tree &= propertyIs(p, PropertyOfObject(0, 1, 5)); //+
	tree &= propertyIs(p, PropertyOfObject(2, 8, 6)); //+
	tree &= propertyIs(p, PropertyOfObject(1, 5, 1)); //+
	tree &= propertyIs(p, PropertyOfObject(0, 5, 7));

	// Условия типа 2
	tree &= propertyEq(p, PropertyOfObject(0, -1, 5), PropertyOfObject(1, -1, 2)); // # 1
	tree &= propertyEq(p, PropertyOfObject(0, -1, 6), PropertyOfObject(2, -1, 8)); // # 2
	tree &= propertyEq(p, PropertyOfObject(0, -1, 5), PropertyOfObject(2, -1, 0)); // # 3
	tree &= propertyEq(p, PropertyOfObject(0, -1, 5), PropertyOfObject(3, -1, 4)); // # 4
	tree &= propertyEq(p, PropertyOfObject(2, -1, 1), PropertyOfObject(1, -1, 4)); // # 5
	tree &= propertyEq(p, PropertyOfObject(0, -1, 1), PropertyOfObject(2, -1, 5)); // # 6
	tree &= propertyEq(p, PropertyOfObject(2, -1, 6), PropertyOfObject(3, -1, 5)); // # 7
	tree &= propertyEq(p, PropertyOfObject(1, -1, 1), PropertyOfObject(3, -1, 3)); // # 8

	// Условия типа 3
	tree &= neighbourEq(p, NeighbourType::LEFT, PropertyOfObject(2, -1, 6), PropertyOfObject(3, -1, 2)); // # 9
	tree &= neighbourEq(p, NeighbourType::LEFT, PropertyOfObject(1, -1, 1), PropertyOfObject(3, -1, 1)); // # 10
	tree &= neighbourEq(p, NeighbourType::UPDIAG, PropertyOfObject(0, -1, 5), PropertyOfObject(0, -1, 2)); // # 11
	tree &= neighbourEq(p, NeighbourType::LEFT, PropertyOfObject(0, -1, 5), PropertyOfObject(0, -1, 0)); // # 12
	tree &= neighbourEq(p, NeighbourType::UPDIAG, PropertyOfObject(3, -1, 1), PropertyOfObject(1, -1, 0)); // # 13

	// Условия типа 4
	tree &= anyNeighboursEq(p, PropertyOfObject(2, -1, 5), PropertyOfObject(0, -1, 2)); //+
	tree &= anyNeighboursEq(p, PropertyOfObject(0, -1, 5), PropertyOfObject(0, -1, 2));
	tree &= anyNeighboursEq(p, PropertyOfObject(2, -1, 8), PropertyOfObject(2, -1, 5));

	// Доп. условия типа 2
	tree &= propertyEq(p, PropertyOfObject(0, -1, 0), PropertyOfObject(2, -1, 2));
	tree &= propertyEq(p, PropertyOfObject(1, -1, 0), PropertyOfObject(3, -1, 0));
	tree &= propertyEq(p, PropertyOfObject(3, -1, 1), PropertyOfObject(2, -1, 3));
	tree &= propertyEq(p, PropertyOfObject(0, -1, 1), PropertyOfObject(1, -1, 7));
	tree &= propertyEq(p, PropertyOfObject(3, -1, 3), PropertyOfObject(2, -1, 4));
	tree &= propertyEq(p, PropertyOfObject(2, -1, 8), PropertyOfObject(1, -1, 5));
	tree &= propertyEq(p, PropertyOfObject(2, -1, 1), PropertyOfObject(0, -1, 4));
	tree &= propertyEq(p, PropertyOfObject(2, -1, 8), PropertyOfObject(3, -1, 7));

	// Доп. условия типа 3
	tree &= neighbourEq(p, NeighbourType::LEFT, PropertyOfObject(2, -1, 5), PropertyOfObject(3, -1, 6)); // 
	tree &= neighbourEq(p, NeighbourType::UPDIAG, PropertyOfObject(2, -1, 6), PropertyOfObject(0, -1, 3)); //
	tree &= neighbourEq(p, NeighbourType::UPDIAG, PropertyOfObject(0, -1, 5), PropertyOfObject(1, -1, 3)); //
	tree &= neighbourEq(p, NeighbourType::UPDIAG, PropertyOfObject(3, -1, 5), PropertyOfObject(1, -1, 6)); //

	allPropValuesDistinct(tree, p);

	std::cout << bdd_satcount(tree) << " solution(s):\n";
	bdd_allsat(tree, fun);
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
