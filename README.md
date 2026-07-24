# int2025_t — целочисленный тип фиксированной длины

Пользовательский тип для представления знакового целого числа фиксированной разрядности, реализованный без использования стандартных контейнеров и `std::bitset`.

- Размер типа: 254 байта (`uint8_t data[254]`), что подтверждается `static_assert(sizeof(int2025_t) <= 254)`.
- Диапазон значений: $[-2^{2024},\ 2^{2024} - 1]$.
- Переполнение обрабатывается по правилу `wrap around` (`MAX + 1 == MIN`, `MIN - 1 == MAX`), а не как UB.

## Возможности

Тип и операции над ним объявлены в `lib/number.h` в виде свободных функций и перегруженных операторов (у `int2025_t` нет пользовательских конструкторов):

- `int2025_t from_int(int32_t i)` — построение значения из `int32_t`.
- `int2025_t from_string(const char* buff)` — построение значения из строки с десятичным представлением числа (со знаком `-` для отрицательных).
- Арифметические операторы: `operator+`, `operator-`, `operator*`, `operator/`.
- Операторы сравнения: `operator==`, `operator!=`.
- Вывод значения в поток: `operator<<` — печатает число в десятичной системе счисления.

## Структура проекта

```
.
├── bin/     — консольное приложение для ручной проверки (main.cpp)
├── lib/     — реализация типа (number.h, number.cpp)
└── tests/   — набор тестов на GoogleTest
```

Структура и состав файлов фиксированы и не подлежат изменению.

Тесты в `tests/` разбиты по файлам согласно проверяемой функциональности:

- `conversion_tests.cpp` — построение значений через `from_int`/`from_string`.
- `arithmetic_tests.cpp` — сложение, вычитание, умножение, деление.
- `comparison_tests.cpp` — `operator==` / `operator!=`.
- `output_tests.cpp` — вывод в поток через `operator<<`.
- `edge_cases_tests.cpp` — граничные значения (`INT32_MAX`/`INT32_MIN`, степени двойки и т. п.).
- `compatibility_tests.cpp` — согласованность результатов `from_int` и `from_string`.

Все тестовые исполняемые файлы линкуются в один бинарник `number_tests` (см. `tests/CMakeLists.txt`), библиотека GoogleTest 1.16.0 подключается автоматически через `FetchContent`.

## Сборка и тесты

Сборка проекта:

```
cmake -S . -B build
cmake --build build
```

Запуск тестов:

```
cmake --build build --target number_tests && build/tests/number_tests
```

либо через `ctest`:

```
cd build && ctest --output-on-failure
```

Запуск консольного приложения (исполняемый файл называется `int2025`, как задано именем проекта в корневом `CMakeLists.txt`):

```
cmake --build build --target int2025 && build/bin/int2025
```

## Пример использования

```cpp
#include "number.h"

int2025_t a = from_int(42);
int2025_t b = from_string("123456789");

int2025_t sum = a + b;
int2025_t diff = a - b;
int2025_t prod = a * b;
int2025_t quot = b / a;

if (a == b) {
    // ...
}

std::cout << sum << std::endl;
```