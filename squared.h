#ifndef SQUARED_H
#define SQUARED_H

#include "C:\TX\TXLib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//! Подключение цветов 
#include "colors.h"

#define MYDEBUG

#ifdef MYDEBUG          //     __func__ __PRETTY_FUNCTION__ - назв функции  TXProcess error
#define ASSERT(right_instr) do { \ 
    if (!right_instr) { \
        fprintf(stderr, "\nAssertion failed: (%s), file <%s>, line: %d\n\n", #right_instr, __FILE__, __LINE__); \
        printf(RED "Link to the line:\n" RESET); \
        printf("C:/squared/" __FILE__ ":%d:1:\n\n", __LINE__); \
        printf(RED "--------------------------------------------------" RESET); \
        abort(); \
    } \
} while (0)
#else
#define ASSERT(right_instr) do { \
        } while(0)
#endif

struct TestCase {
    double a, b, c;
    int roots;
    double x1_exp, x2_exp;
};

//! Имена файлов, откуда берутся коэффициенты
#define STANDART_COEFF_FILENAME "coeffs.txt"
#define RANDOM_COEFF_FILENAME "random_tests.txt"
#define DEBUG_FILENAME "debug_tests.txt"

#define MAXLINE 1000
#define BUFSIZE 1000

//! Результаты успеха/провала выполнения функций
#define PASS 1
#define END 0

//! Точность сравнения для вещественных чисел
#define EPS 1e-3

//! Максимальное значение коэффициентов в случайном тест-кейсе
#define MAX_RANDOM_COEFF 1000

// мб загнать в структуру
const char UTESTS_MODE[] = "--test";
const char HELP_MODE[]   = "--help";
const char USER_MODE[]   = "--user";

//! Флажок на включение/выключение режима ИИ: 1 - вкл, 0 - выкл
const int AI_MOD = 0;

const int N_RANDOM_TESTS = 100;    //!< Количество случайных тестов
const int N_USER_INPUTS =  5;      //!< Количество пользовательских вводов


//! Виды корней квадратного уравнения
enum 
    {
    NOROOTS =  0,   //!< Нет корней    
    ONEROOT =  1,   //!< Один корень (или два совпадающих)
    TWOROOTS = 2,   //!< Два различных корня, a не равно 0
    INF = -1,       //!< Бесконечное число корней
    };

int SolveSquare(double a, double b, double c, double *x1, double *x2);     //! Функция, решающая квадратное 
                                                                            //! уравнение с коэффициентами a, b, c
//! Функция запуска тестов для проверки решения
int RunTests();

//! Функция сравнения вещественных чисел double
int IsEqual(double n, double m, double epsilon);

int IsBigger(double p, double q, double epsilon);

//! Сортирует корни по возрастанию (x_1 < x_2), чтобы сравнивать в определённом порядке
//! Если один из корней NAN, то NAN помещается в x_1 (если оба x_1 и x_2 - NAN)
int SortX(double *x_1, double *x_2);

//! Вывод справки в пользовательском режиме
void HelpInfo();

//! Вывод справочной информации для консольного режима
void HelpConsoleInfo();
#endif