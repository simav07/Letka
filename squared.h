#ifndef SQUARED_H
#define SQUARED_H

#include "C:\TX\TXLib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//! ���������� � ������� 
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

//! ��� �����, �� �������� ������ ������������
#define STANDART_COEFF_FILENAME "coeffs.txt"
#define RANDOM_COEFF_FILENAME "random_tests.txt"
#define DEBUG_FILENAME "debug_tests.txt"

#define MAXLINE 1000
#define BUFSIZE 1000

//! ������� ��������/���������� ���������� �������
#define PASS 1
#define END 0

//! ������� ����������� ��� ����� �����
#define EPS 1e-3

//! ����������� ������� ����������� � ����������� ��������� � ����-������
#define MAX_RANDOM_COEFF 1000

enum
    {
    N_RANDOM_TESTS = 100,
    N_USER_INPUTS = 5,
    };

//! �������� ������� ���������
enum 
    {
    NOROOTS = 0,    //!< ��� ������    
    ONEROOT = 1,   //!< ���� ������ (��� ��� �����������)
    TWOROOTS = 2,   //!< ������������ ������ ����, a �� ����� 0
    INF = -1,   //!< ����������� ����� ������
    };

int solve_square(double a, double b, double c, double *x1, double *x2);     //! �������, �������� ���������� 
                                                                            //! ��������� � ��������� �������������� a, b, c

//! ������� ���������� ��� ������ � ������
int RunTests();

//! ������� �������� ��������� ��� double'����� �����
int isEqual(double n, double m, double epsilon);

int isBigger(double p, double q, double epsilon);

//! ���������� ���������� ������ �� ����������� (x_1 < x_2), ����� ����� ��� ���������� � ����������� �������
//! ���� ����������� NAN, �� NAN ������ ����� � x_1 (��� ��� x_1 � x_2 - NAN)
int sort_x(double *x_1, double *x_2);

//! ����� � ������� ���������� � ��������
void HelpInfo();
#endif