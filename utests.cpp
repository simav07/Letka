#include "squared.h"

#define MIN_RANDOM_NUM 0.1

//! Функция записывает значения коэффициентов a, b, c из строки buf
int get_coeffs(char buf[], double *a, double *b, double *c, int *roots_exp, double *x1_exp, double *x2_exp);

//! Функция решает уравнение с помощью функции squared,
//! проверяет правильность вычислений и возвращает 0 или 1 (неверно/верно)
int RunOneTest(TestCase test, int test_count);

//! Функция копирования неверно решенного теста в отдельный файл
int WriteIncorrectTest(TestCase test, int test_count, double x1, double x2, char filename[]);
//! Генерация случайных коэффициентов для квадратного уравнения
void MakeCoeffs(double *a, double *b, double *c, int *roots, double *rand_x1, double *rand_x2);

//! Функция записи в файл
int WriteRandomEq(char filename[]);

//! Функция печати структуры
int WriteStruct(TestCase test, char buffer[], int size_of_buffer);

//! Функция запуска юнит-тестов из файла filename
int StartCase(char filename[]);

int RunTests() {

    char buf[BUFSIZE] = "";

    //! Генерация и запись тестовых (случайных) коэффициентов в отдельный файл
    snprintf(buf, BUFSIZE, "%s", RANDOM_COEFF_FILENAME);
    WriteRandomEq(buf);

    //! Запуск решения уравнений в общем виде
    printf(YELLOW "\nПротокол по решению случайно подобранных случаев:\n" RESET);
    StartCase(buf);

    memset(buf, 0, BUFSIZE); // Очистка буфера

    //! Запуск решения частных случаев
    snprintf(buf, BUFSIZE, "%s", STANDART_COEFF_FILENAME);
    printf(YELLOW "\nПротокол по решению частых случаев:\n" RESET);
    StartCase(buf);

    return PASS;
    
}

int StartCase(char filename[]) {

    FILE *file_p = fopen(filename, "r");
    
    if (!file_p) {
        printf("Ошибка чтения файла: <%s>\n", filename);
        return END;
    }

    char buffer[BUFSIZE] = "";  //!< буфер для хранения i-ой строки с информацией

    int success_tests = 0;      //!< счетчик удачных тестов
    int all_tests = 0;          //!< подсчет количества тестов

    while (fgets(buffer, BUFSIZE, file_p) != NULL) {

        all_tests++;

        TestCase test = {0};
        // double a = 0, b = 0, c = 0;
        // double x1_exp = 0, x2_exp = 0;
        // int roots_exp = 0;

        if (get_coeffs(buffer, &test.a, &test.b, &test.c, &test.roots, &test.x1_exp, &test.x2_exp) != PASS) {

            all_tests--;  //! Скипаем пустую строку, учитываем что это был не тест
            continue;
        
        }

        //! Выполняем тестирование с этими числами
        success_tests += RunOneTest(test, all_tests); //!< Если тест удачный, то увеличиваем счетчик на 1

    }

    printf("\nAll tests: %d\n" GREEN "Success: %d\n" RESET RED "Incorrect: %d\n" RESET,
            all_tests, success_tests, all_tests-success_tests);
    
    fclose(file_p);
    return PASS;
}

int RunOneTest(TestCase test, int test_count) {

    double x1 = NAN, x2 = NAN;
    int roots = solve_square(test.a, test.b, test.c, &x1, &x2);

    //! сортируем (x1,x2) и (x1_exp, x2_exp) по возрастанию + NAN записывается в x1
    sort_x(&x1, &x2);

    double x1_exp = test.x1_exp;
    double x2_exp = test.x2_exp;

    sort_x(&x1_exp, &x2_exp);

    if (roots == test.roots) {

        if (isnan(x2) && isnan(x2_exp)) { //! Если все корни nan
            
            return 1;  //! Успех, 0 корней
        
        }
        else if (!isnan(x2) && !isnan(x2_exp)) {  //!< x2 и x2_exp не NAN

            if (isEqual(x2, x2_exp, EPS)) {

                if (!isnan(x1) && !isnan(x1_exp)) {

                    if (isEqual(x1, x1_exp, EPS))
                        return 1;  //! Успех - 2 корня
                    
                }

                else if (isnan(x1) && isnan(x1_exp)) {  //! x1 и x1_exp - NAN, x2 и x2_exp не NAN

                    return 1;   //! Успех - 1 корень x2
                
                }

            }

        }

    }

    char filename[BUFSIZE] = "";
    snprintf(filename, BUFSIZE, "%s", DEBUG_FILENAME);

    WriteIncorrectTest(test, test_count, x1, x2, filename);
    
    return 0;

}

int WriteIncorrectTest(TestCase test, int test_count, double x1, double x2, char filename[]) {

    FILE *file_p = fopen(filename, "a");
    
    if (!file_p) {
        printf("Ошибка чтения файла: <%s>\n", filename);
        return END;
    }    

    fprintf(file_p, "Date: %s, time: %s\n"
                    "Test №%d: \nIncorrect: x1 = %lg, x2 = %lg\n"
                                    "Expected: x1_exp = %lg, x2_exp = %lg\n\n",
                                    __DATE__, __TIME__,
                                    test_count, x1, x2, test.x1_exp, test.x2_exp);

    fclose(file_p);
    return PASS;
}

int get_coeffs(char buf[], double *a, double *b, double *c, int *roots_exp, double *x1_exp, double *x2_exp) {

    ASSERT(a);
    ASSERT(b);
    ASSERT(c);
    ASSERT(roots_exp);
    ASSERT(x1_exp);
    ASSERT(x2_exp);

    if (sscanf(buf, "%lf %lf %lf %d %lf %lf", a, b, c, roots_exp, x1_exp, x2_exp) == 6) {  //! Если записалось удачно

        return PASS;
    }

    return END;
}

int sort_x(double *x_1, double *x_2) {

    ASSERT(x_1);
    ASSERT(x_2);

    double dop_x = 0;

    if (isnan(*x_1) || isnan(*x_2)) {

        if (isnan(*x_2)) {

            *x_2 = *x_1;
            *x_1 = NAN;
            //! Теперь NAN (если встретилось) всегда будет в x_1
            return PASS;
        }

    }

    else if (isBigger(*x_1, *x_2, EPS)) {

        dop_x = *x_2;
        *x_2 = *x_1;
        *x_1 = dop_x;

    }
    
    return END;    
}

int WriteRandomEq(char filename[]) {

    FILE *file_p = fopen(filename, "w");
    
    if (!file_p) {
        printf("Ошибка чтения файла: <%s>\n", filename);
        return END;
    }

    ASSERT(file_p);

    for (unsigned int i = 0; i < N_RANDOM_TESTS; i++) {

        TestCase test = {0};
        //! Генерим коэффициенты
        MakeCoeffs(&test.a, &test.b, &test.c, &test.roots, &test.x1_exp, &test.x2_exp);

        //! Записываем структуру с коэффициентами в строку
        char buffer[BUFSIZE] = "";

        int n_fields = WriteStruct(test, buffer, sizeof(buffer));

        if (n_fields < 0) {
            printf("Ошибка записи структуры\n");
            return END;
        }

        fwrite(buffer, sizeof(char), n_fields, file_p);

    }

    fclose(file_p);

    return 0;

}

void MakeCoeffs(double *a, double *b, double *c, int *roots, double *rand_x1, double *rand_x2) {

    ASSERT(a);
    ASSERT(b);
    ASSERT(c);
    ASSERT(roots);
    ASSERT(rand_x1);
    ASSERT(rand_x2);

    *rand_x1 = MIN_RANDOM_NUM + double(rand() % MAX_RANDOM_COEFF);
    *rand_x2 = MIN_RANDOM_NUM + double(rand() % MAX_RANDOM_COEFF);

    *roots = TWOROOTS;
    *a = MIN_RANDOM_NUM + double(rand() % MAX_RANDOM_COEFF);
    *c = (*rand_x1) * (*rand_x2) * (*a);
    *b = -((*rand_x1) + (*rand_x2)) * (*a);
}

int WriteStruct(TestCase test, char buffer[], int size_of_buffer) {

    ASSERT (buffer);

    int n_fields = snprintf(buffer, size_of_buffer, "%10.2lf %15.2lf %15.2lf %3d %15.2lf %15.2lf\n",                  // define
                                                    test.a, test.b, test.c, test.roots, test.x1_exp, test.x2_exp);

    return n_fields;
}