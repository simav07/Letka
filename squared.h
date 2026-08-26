#ifndef SQUARED_H
#define SQUARED_H

#include "C:\TX\TXLib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//! Библиотека с цветами
#include "colors.h"

#define ASSERT(right_instr) do { \
    if (!right_instr) { \
        fprintf(stderr, "\nAssertion failed: (%s), file <%s>, line: %d\n\n", #right_instr, __FILE__, __LINE__); \
        printf(RED "Link to the line:\n" RESET); \
        printf("C:/squared/" __FILE__ ":%d:1:\n\n", __LINE__); \
        printf(RED "--------------------------------------------------" RESET); \
        abort(); \
    } \
} while (0)

struct TestCase {
    double a, b, c;
    int roots;
    double x1_exp, x2_exp;
};

//! Имя файла, из которого читаем коэффициенты
#define STANDART_COEFF_FILENAME "coeffs.txt"
#define RANDOM_COEFF_FILENAME "random_tests.txt"
#define DEBUG_FILENAME "debug_tests.txt"

#define MAXLINE 1000
#define BUFSIZE 1000

//! Условия удачного/неудачного выполнения условия
#define PASS 1
#define END 0

//! Эпсилон окрестность для малых чисел
#define EPS 1e-3

//! Максимально большой коэффициент у квадратного уравнения в юнит-тестах
#define MAX_RANDOM_COEFF 1000

enum
    {
    N_RANDOM_TESTS = 100,
    N_USER_INPUTS = 5,
    };

//! Варианты решения уравнения
enum 
    {
    NOROOTS = 0,    //!< нет корней    
    ONEROOT = 1,   //!< один корень (или два совпадающих)
    TWOROOTS = 2,   //!< дискриминант больше нуля, a не равно 0
    INF = -1,   //!< бесконечное число корней
    };

int solve_square(double a, double b, double c, double *x1, double *x2);     //! Функция, решающая квадратное 
                                                                            //! уравнение с заданными коэффициентами a, b, c

//! Функция записывает все строки в массив
int RunTests();

//! Функция проверки равенства для double'вских чисел
int isEqual(double n, double m, double epsilon);

int isBigger(double p, double q, double epsilon);

//! Сортировка полученных корней по возрастанию (x_1 < x_2), чтобы знать как сравнивать с полученными корнями
//! Если встретилось NAN, то NAN всегда будет в x_1 (или оба x_1 и x_2 - NAN)
int sort_x(double *x_1, double *x_2);

//! Вывод в консоль информации о командах
void HelpInfo();
#endif