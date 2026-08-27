#include "squared.h"
#include <math.h>
#include <time.h>

#define AI_SLEEP_TIME 4

//! Отслеживание режима работы
#define START_TESTS    1  //!< Режим начала работы юнит-тестов
#define KEYBOARD_INPUT 2  //!< Режим ручного ввода
#define WORK_OFF       0  //!< Выход из программы
#define WORK_ON       10  //!< Работа пользовательского режима

// CamelCase 
// pascalCase 
// snake_case 
// CAPS_CASE

//! Возможные варианты решения уравнения
int GetLine(char s[], int lim);  //!< Функция читающая ввод пользователя

int GetInput(TestCase *test);    //!< Функция, получающая коэффициенты
void GetOutput(int nroots, double x1, double x2); //!< Функция, выводящая ответ пользователю

int Greet();  //!< AI - приветствие

int DeepThink(int is_aiMode);  //!< Генерация сообщения о мыслительном процессе

int IsZero(double numbr, double epsilon);  //!< Проверка double на ноль

int SolveStandartCase(double a, double b, double c, double *x1, double *x2); //!< Решение при НЕнулевых коэффициентах
int SolveSpecialCase(double a, double b, double c, double *x1, double *x2);  //!< Решение для частных случаев

void RunMultipleLaunch(); //!< запуск всей программы

int RunConsoleMode(int indx, char *argv[]); //!< Режим работы в определенном режиме
int RunStandartMode();                      //!< Режим работы в обычном пользовательском режиме

int main(int argc, char *argv[]) {

    ASSERT(argv);

    //! Если введены какие либо флаги
    if (argc > 1)
        if (argc == 2)
            RunConsoleMode(argc-1, argv);
        else
            printf(RED "\nToo many arguments\n" RESET);
    else {
        RunStandartMode();
    
    }
    return 0;
}

int RunConsoleMode(int indx, char *argv[]) {

    ASSERT(argv);

    if (!strcmp(argv[indx], UTESTS_MODE)) {   //! Режим юнит тестов
        RunTests();
        printf("Тесты выполнены.\n");
    }

    else if (!strcmp(argv[indx], USER_MODE)) {
        RunStandartMode();
    }

    else if (!strcmp(argv[indx], HELP_MODE)) //! Вывод справочной информации
        HelpConsoleInfo();
    
    else {
        printf("Flag scanning error\n");
    }

    return PASS;
}

int RunStandartMode() {

    printf("Привет! Меня зовут КотИИк. О чем хотели бы пообщаться?\n");
    HelpInfo();
    RunMultipleLaunch();

    return 0;
}

int Greet() {

    char ignore_line[MAXLINE] = "";
    char mode[MAXLINE] = "";

    GetLine(ignore_line, sizeof(ignore_line));

    int nLastSymb = 0;

    if (sscanf(ignore_line, "%s %n", mode, &nLastSymb) == 1) {

        if (ignore_line[nLastSymb-1] == '\n') {
            if (!strcmp(mode, "tests"))
                return START_TESTS; //!< Старт юнит-тестов
            
            if (!strcmp(mode, "end"))
            
                return WORK_OFF;    //!< Прекращение работы программы
            
            if (!strcmp(mode, "start")) {
                DeepThink(AI_MOD);
                printf("Какая прекрасная тема для разговора! Для начала можем порешать квадратное уравнение.\n");
                return KEYBOARD_INPUT;
            }
        }
        printf(RED "Input error\n" RESET);
        return WORK_ON;
    }

    printf("Ошибка чтения ввода. Программа завершена\n");
    return WORK_OFF;
}

void RunMultipleLaunch() {

    int work = WORK_ON;

    do {
        int mode = Greet();
        TestCase test = {0};
        double x1 = 0, x2 = 0;

        if (mode == KEYBOARD_INPUT) {  //! Режим ручного ввода
            if (GetInput(&test) == 1)  {   //! Если ввод удачный
        
                int nroots = SolveSquare(test.a, test.b, test.c, &x1, &x2);

                GetOutput(nroots, x1, x2);
                printf("\nПоздравляю! Вы решили квадратное уравнение. Хотели бы продолжить?\n");
                HelpInfo();

                continue;                
            }

            work = WORK_OFF; //! Если все попытки ввода неудачны - заканчиваем
        }
        if (mode == START_TESTS) {

            RunTests();
            printf("Тесты выполнены. Хотели бы продолжить?\n");
            HelpInfo();

            continue;
        }
        if (mode == WORK_ON) {
            printf("\nПопробуйте еще раз:\n");
            continue;
        }
        else
            work = WORK_OFF;  
      
    } while(work != WORK_OFF);
}

int DeepThink(int is_aiMode) {

    srand((int)time(NULL));
    
    int question = rand() % 5 + 1;  // Переменная, отвечающая за выбор рандомного вопроса
    
    //! Если режим ИИ включен
    if (is_aiMode)
    {
        switch (question) {
            case (1):
                printf(YELLOW "\nДумает...\n" RESET);
                sleep(AI_SLEEP_TIME);
                break;
            case (2):
                printf(YELLOW "\nМурчит и размышляет...\n" RESET);
                sleep(AI_SLEEP_TIME);
                break;
            case (3):
                printf(YELLOW "\nАнализирует...\n" RESET);
                sleep(AI_SLEEP_TIME);
                break;
            case (4):
                printf(YELLOW "\nПрикидывает, что к чему...\n" RESET);
                sleep(AI_SLEEP_TIME);
                break;
            case (5):
                printf(YELLOW "\nИдет процесс мышления...\n" RESET);
                sleep(AI_SLEEP_TIME);
                break;
       
            default: 
                break;
        }
    }
    return 0;
}

/// Функция получения удачного разультата - коэффициентов a, b, c
/// Если с 5 раза не получается, программа завершает работу
int GetInput(TestCase *test) {

    ASSERT(test);

    for (int i = 0; i < N_USER_INPUTS; i++)
    {
        char line[MAXLINE] = "";
        printf("Введите коэффициенты a, b, c: ");
        
        GetLine(line, sizeof(line));
        DeepThink(AI_MOD);
        int nLastSymb = 0;
        if (sscanf(line, "%lf %lf %lf %n", &((*test).a), &((*test).b), &((*test).c), &nLastSymb) == 3) {
            if (line[nLastSymb-1] == '\n')
                return 1;   //!< Ввод удался, выводим 1
        }
        
        printf(RED "Хм, не могу понять ввод.\nПожалуйста, введите 3 числа: a, b, c.\n" RESET);
    }
    /// Если ввод неудачный - выводим 0
    DeepThink(AI_MOD);
    printf("Мда, кажется нам не удастся поговорить :`(((((");
    return 0;
}

void GetOutput(int nroots, double x1, double x2) {

    switch (nroots)
        {
        case (ONEROOT):
            printf(GREEN "\nКорень вашего уравнения: x = %lg\n" RESET, x1);
            break;
        case (TWOROOTS):
            printf(GREEN "Ваши корни:\n x_1 = %lg, x_2 = %lg\n" RESET, x1, x2);
            break;
        case (NOROOTS):
            printf(GREEN "К сожалению, ваше уравнение не имеет действительных корней.\n" RESET);
            break;
        
        case (INF):
            printf(YELLOW "Бесконечное число корней\n" RESET);
            break;
        default:
            printf(RED "Switch error\n" RESET);
            break;
        }
    return;
}
int IsZero(double numbr, double epsilon) {

    ASSERT(isfinite (numbr));
    ASSERT(isfinite (epsilon));

    if (fabs(numbr) < epsilon) {
        return true;
    }
    else {
        return false;
    }
}
int SolveSquare(double a, double b, double c, double *x1, double *x2) {

    ASSERT(isfinite (a));
    ASSERT(isfinite (b));
    ASSERT(isfinite (c));
    ASSERT(x1);
    ASSERT(x2);

    //! Когда один из коэффициентов равен нулю
    if (IsZero(a, EPS) || IsZero(b, EPS) || IsZero(c, EPS)) {
    //! То особый случай, аля-улю
        return SolveSpecialCase(a, b, c, x1, x2);
    
    }
    else {  //! Решение в общем случае, a, b, c != 0
        return SolveStandartCase(a, b, c, x1, x2);
    }

    return 0;
}
int SolveStandartCase(double a, double b, double c, double *x1, double *x2) {

    double dis = b * b - (4 * a * c);

        DeepThink(AI_MOD);

        //! Дискриминант равен нулю
        if (IsZero(dis, EPS)) {
            *x1 = -b / (2 * a);
            *x2 = *x1;
            return ONEROOT;
        }

        else if (dis > 0) { //! Дискриминант больше нуля
            double sqrt_dis = sqrt(dis);

            *x1 = (-b + sqrt_dis) / (2 * a);
            *x2 = (-b - sqrt_dis) / (2 * a);
            return TWOROOTS;   //! указатель на то, что решение в действительных числах
        }
        else {             //! Нет действительных корней
            return NOROOTS;
        }

    return 0;
}

int SolveSpecialCase(double a, double b, double c, double *x1, double *x2) {

    bool isNull_a = IsZero(a, EPS);
    bool isNull_b = IsZero(b, EPS);
    bool isNull_c = IsZero(c, EPS);

    if (isNull_a) {   //! a = 0
        if (isNull_b) {
            if (isNull_c)   //! Бесконечное число корней
                return INF;
            else
                return NOROOTS; //! Нет корней
        }
        //! Линейное уравнение
        *x1 = (-c) / b;
        return ONEROOT;
    }
    else if (isNull_b) {  //! b = 0; a != 0;
        if (isNull_c){
            *x1 = 0;
            return ONEROOT;
        }
        else if (((-c) / a) >= 0) {
            *x1 = sqrt((-c) / a);
            *x2 = (-1) * (*x1);
            return TWOROOTS;
        }
        else
            return NOROOTS;
    }
    else if (isNull_c) {  //! c = 0; b != 0; a != 0
        *x2 = 0;
        SolveSquare(0, a, b, x1, x2);
        return TWOROOTS;
    }
    return 0;
}

int IsBigger(double p, double q, double epsilon) {

    ASSERT(isfinite (p));
    ASSERT(isfinite(q));
    ASSERT(isfinite (epsilon));

    if ((p - q) > epsilon)
        return true;
    return false;
}

int GetLine(char s[], int lim) {

    char chr = 0;
    int i = 0;

    while (((chr = (char) getchar()) != '\n') && (chr != EOF) && (i < lim)) {
        s[i] = chr;
        i++;
    }

    s[i] = '\n';
    return i;
}

int IsEqual(double n, double m, double epsilon) {

    ASSERT(isfinite (n));
    ASSERT(isfinite (m));
    ASSERT(isfinite (epsilon));

    if (((n - epsilon) <= m) && ((n + epsilon) >= m)) {
        return PASS;
    }
    return END;

}

void HelpInfo() {
    printf(GREEN "/---------------------------------------------------------------/\n\n"
                 "< end >   - завершить работу программы\n"
                 "< start > - ввести коэффициенты квадратного уравнения\n"
                 "< tests > - запустить юнит-тесты\n\n"
                 "/---------------------------------------------------------------/\n" RESET);
}

void HelpConsoleInfo() {
    printf(YELLOW "/---------------------------------------------------------------/\n\n"
                 "Console commands:\n"
                 "< --help > - вывести справочную информацию\n"
                 "< --user > - пользовательский режим\n"
                 "< --test > - запустить юнит-тесты\n\n"
                 "/---------------------------------------------------------------/\n" RESET);
}
