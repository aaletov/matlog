# matlog

## `BuDDy`

В курсовой работе используются следующие функции, объекты и типы из библиотеки `BuDDy`:

- `int bdd_init(int nodenum, int cachesize)` - аллоцирует указанное количество памяти для библиотеки. 
- `int bdd_setvarnum(int N)` - устанавливает число булевых переменных. 
- `bdd_ithvar(int i)` - возвращает значение i-ой переменной.
- `bdd_nithvar(int i)` - возвращает НЕ значения i-ой переменной.
- `double bdd_satcount(const bdd &r)` - возвращает количество всех интерпретаций, на которых функция истинна.
- `void bdd_allsat(const bdd &r, bddallsathandler handler)` - применяет некоторую функцию ко всех наборам, на которых функция истинна (?), обработчик получает значения переменных (0, 1, -1) и размер массива.

  Сигнатура обработчика `void fun(char * varset ,int size);`
- `bdd_done()` - завершение работы с библиотекой.