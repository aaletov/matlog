#include <iostream>
#include <cmath>
#include <vector>

#include <bdd.h>

const std::size_t N = 9;
const std::size_t M = 4;
const std::size_t N_SQRT = static_cast<size_t>(sqrt(N));
const std::size_t K = 21;
const int LOG_N = static_cast<int>(std::ceil(std::log2(N)));;

const std::size_t N_VAR = 144; // N * M * LOG_N
char var[N_VAR];

enum NeighbourRelations
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

void init(bdd p[M][N][N]);
bdd limitation_1(const bdd p[M][N][N], const int propertyNumber, const int objectNumber, const int propertyValue);
bdd limitation_2(const bdd p[M][N][N], const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2);
bdd limitation_3(const bdd p[M][N][N], const NeighbourRelations neighbour, const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2);
bdd limitation_4(const bdd p[M][N][N], const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2);
void limitation_5(bdd &tree, const bdd p[M][N][N]);
bdd limitation_6(const bdd p[M][N][N]);
bdd limitation_7(const bdd p[M][N][N]);

void fun(char *varset, int size);

int main()
{
  bdd_init(50000000, 1000000);
  bdd_setcacheratio(100);
  bdd_setvarnum(N_VAR);
  bdd p[M][N][N];

  init(p);

  bdd task = bddtrue;

  //variant 1: n1 = 3, n2 = 2, n3 = 5, n4 = 5

  //3 type-1 limitations
  task &= limitation_1(p, 1, 4, 8);
  task &= limitation_1(p, 2, 0, 5);
  task &= limitation_1(p, 3, 2, 5);

  //additional type-1 limitations
  task &= limitation_1(p, 0, 7, 1);
  task &= limitation_1(p, 2, 3, 1);
  task &= limitation_1(p, 0, 2, 6);
  task &= limitation_1(p, 0, 1, 5);
  task &= limitation_1(p, 2, 8, 6);
  task &= limitation_1(p, 1, 5, 1);
  task &= limitation_1(p, 3, 6, 6);
  task &= limitation_1(p, 1, 7, 3);
  task &= limitation_1(p, 1, 0, 0);
  task &= limitation_1(p, 0, 5, 3);
  task &= limitation_1(p, 3, 1, 0);
  task &= limitation_1(p, 0, 0, 7);
  task &= limitation_1(p, 1, 1, 4);
  task &= limitation_1(p, 2, 1, 0);

  //2 type-2 limitations
  task &= limitation_2(p, 0, 0, 2, 3);
  task &= limitation_2(p, 1, 5, 3, 3);

  //additional type-2 limitations
  task &= limitation_2(p, 0, 6, 2, 8);

  //5 type-3 limitations
  task &= limitation_3(p, NeighbourRelations::LEFT, 0, 7, 1, 2);
  task &= limitation_3(p, NeighbourRelations::RIGHT, 1, 8, 3, 2);
  task &= limitation_3(p, NeighbourRelations::UP, 3, 7, 0, 5);
  task &= limitation_3(p, NeighbourRelations::DOWN, 2, 4, 0, 1);
  task &= limitation_3(p, NeighbourRelations::RIGHT, 2, 7, 1, 0);

  //additional type-3 limitations 
 /* task &= limitation_3(p, NeighbourRelations::RIGHT, 1, 6, 2, 0);
  task &= limitation_3(p, NeighbourRelations::DOWN, 0, 2, 0, 4);
*/
  //5 type-4 limitations 
  task &= limitation_4(p, 0, 4, 1, 0);
  task &= limitation_4(p, 0, 8, 1, 2);
  task &= limitation_4(p, 2, 4, 3, 4);
  task &= limitation_4(p, 2, 6, 1, 1);
  task &= limitation_4(p, 3, 0, 0, 8);

  task &= limitation_6(p);
  task &= limitation_7(p);
  limitation_5(task, p);

  size_t satcount = static_cast<size_t>(bdd_satcount(task));
  std::cout << satcount << " solutions:\n" << std::endl;
  if (satcount) bdd_allsat(task, fun);

  bdd_done();
  system("pause");
}


void init(bdd p[M][N][N])
{
  size_t I = 0;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      for (size_t z = 0; z < M; ++z) {
        p[z][i][j] = bddtrue;
        for (size_t k = 0; k < LOG_N; ++k) {
          p[z][i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * z + k) : bdd_nithvar(I + LOG_N * z + k);
        }
      }
    }
    I += LOG_N * M;
  }
}

bdd limitation_1(const bdd p[M][N][N], const int propertyNumber, const int objectNumber, const int propertyValue)
{
  return p[propertyNumber][objectNumber][propertyValue];
}

bdd limitation_2(const bdd p[M][N][N], const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2)
{
  bdd tree = bddtrue;

  for (size_t i = 0; i < N; ++i) {
    tree &= !(p[propertyNumber1][i][propertyValue1] ^ p[propertyNumber2][i][propertyValue2]);
  }
  return tree;
}

bdd limitation_3(const bdd p[M][N][N], const NeighbourRelations neighbour, const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2)
{
  bdd tree = bddtrue;

  switch (neighbour) {
  case UP:
    for (size_t i = N_SQRT; i < N; ++i) {
      tree &= !(p[propertyNumber1][i][propertyValue1] ^ p[propertyNumber2][i - N_SQRT][propertyValue2]);
    }
    break;

  case DOWN:
    for (size_t i = 0; i < N - N_SQRT; ++i) {
      tree &= (!p[propertyNumber1][i][propertyValue1] ^ p[propertyNumber2][i + N_SQRT][propertyValue2]);
    }
    break;

  case LEFT:
    for (size_t i = 0; i < N; ++i) {
      if (i % N_SQRT != 0) {
        tree &= !(p[propertyNumber1][i][propertyValue1] ^ p[propertyNumber2][i - 1][propertyValue2]);
      }
    }
    break;

  case RIGHT:
    for (size_t i = 0; i < N; ++i) {
      if ((i + 1) % N_SQRT != 0) {
        tree &= !(p[propertyNumber1][i][propertyValue1] ^ p[propertyNumber2][i + 1][propertyValue2]);
      }
    }
    break;
  }
  return tree;
}

bdd limitation_4(const bdd p[M][N][N], const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2)
{
  return limitation_3(p, NeighbourRelations::UP, propertyNumber1, propertyValue1, propertyNumber2, propertyValue2) | 
    limitation_3(p, NeighbourRelations::DOWN, propertyNumber1, propertyValue1, propertyNumber2, propertyValue2) |
    limitation_3(p, NeighbourRelations::LEFT, propertyNumber1, propertyValue1, propertyNumber2, propertyValue2) |
    limitation_3(p, NeighbourRelations::RIGHT, propertyNumber1, propertyValue1, propertyNumber2, propertyValue2);
}

void limitation_5(bdd &tree, const bdd p[M][N][N])
{
  for (size_t m = 0; m < M; ++m) {
    for (size_t i = 0; i < N - 1; ++i) {
      for (size_t j = i + 1; j < N; ++j) {
        for (size_t k = 0; k < N; ++k) {
          tree &= p[m][i][k] >> !p[m][j][k];
        }
      }
    }
  }
}

bdd limitation_6(const bdd p[M][N][N])
{
  bdd tree = bddtrue;

  for (size_t i = 0; i < N; ++i) {
    for (size_t k = 0; k < M; ++k) {
      bdd temp = bddfalse;

      for (size_t j = 0; j < N; ++j) {
        temp |= p[k][i][j];
      }

      tree &= temp;
    }
  }

  return tree;
}

bdd limitation_7(const bdd p[M][N][N])
{
  bdd task = bddtrue;
  
  for (int i = 0; i < N; i++) {
    bdd temp = bddfalse;
    for (int j1 = 0; j1 < N; j1++) {
      for (int j2 = 0; j2 < N; j2++) {
        for (int j3 = 0; j3 < N; j3++) {
          for (int j4 = 0; j4 < N; j4++) {
            if (j1 + j2 + j3 + j4 <= K) {
              temp |= (p[0][i][j1] & p[1][i][j2] & p[2][i][j3] & p[3][i][j4]);
            }
          }
        }
      }
    }
    task &= temp;
  }

  return task;
}

void print()
{
  for (size_t i = 0; i < N; i++) {
    std::cout << i << ": ";
    for (size_t j = 0; j < M; j++) {
      int J = i * M * LOG_N + j * LOG_N;
      int num = 0;
      for (size_t k = 0; k < LOG_N; k++) num += static_cast<size_t>(var[J + k] << k);
      std::cout << num << ' ';
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

void build(char *varset, unsigned n, unsigned I)
{
  if (I == n - 1) {
    if (varset[I] >= 0) {
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

  if (varset[I] >= 0) {
    var[I] = varset[I];
    build(varset, n, I + 1);
    return;
  }

  var[I] = 0;
  build(varset, n, I + 1);
  var[I] = 1;
  build(varset, n, I + 1);
}

void fun(char *varset, int size)
{
  build(varset, size, 0);
}
