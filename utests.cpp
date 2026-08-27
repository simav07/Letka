#include "squared.h"

#define MIN_RANDOM_NUM 0.1

//! Function to extract coefficients a, b, c from string buf
int GetCoeffs(char buf[], double *a, double *b, double *c, int *roots_exp, double *x1_exp, double *x2_exp);

//! Function to run a single test and output the result of squared,
//! returns 0 or 1 (fail/success)
int RunOneTest(TestCase test, int test_count);

//! Function writes an incorrect test to the log file
int WriteIncorrectTest(TestCase test, int test_count, double x1, double x2, char filename[]);
//! Generates coefficients for a random equation
void MakeCoeffs(double *a, double *b, double *c, int *roots, double *rand_x1, double *rand_x2);

//! Function to write to file
int WriteRandomEq(char filename[]);

//! Function to write structure
int FillBufferFromStruct(TestCase test, char buffer[], int size_of_buffer);

//! Function to run tests from the file filename
int StartCase(char filename[]);

int RunTests() {

    char buf[BUFSIZE] = "";

    //! Generate random coefficients and write them to a file
    snprintf(buf, BUFSIZE, "%s", RANDOM_COEFF_FILENAME);
    WriteRandomEq(buf);

    //! Run tests from this file
    printf(YELLOW "\nRunning tests with random coefficients:\n" RESET);
    StartCase(buf);

    memset(buf, 0, BUFSIZE); // clear buffer

    //! Run tests from the standard file
    snprintf(buf, BUFSIZE, "%s", STANDART_COEFF_FILENAME);
    printf(YELLOW "\nRunning tests from the standard file:\n" RESET);
    StartCase(buf);

    return PASS;
    
}

int StartCase(char filename[]) {

    ASSERT(filename);

    FILE *file_p = fopen(filename, "r");
    
    if (!file_p) {
        printf("Error opening file: <%s>\n", filename);
        return END;
    }

    char buffer[BUFSIZE] = "";  //!< buffer for reading the i-th line from the file

    int success_tests = 0;      //!< number of successful tests
    int all_tests = 0;          //!< total number of tests

    while (fgets(buffer, BUFSIZE, file_p) != NULL) {

        all_tests++;

        TestCase test = {0};

        if (GetCoeffs(buffer, &test.a, &test.b, &test.c, &test.roots, &test.x1_exp, &test.x2_exp) != PASS) {

            all_tests--;  //! remove the incorrect line since it is not a test
            continue;
        
        }

        //! Check coefficients and run the test
        success_tests += RunOneTest(test, all_tests); //!< if the test passes, increment the counter by 1

    }

    printf("\nAll tests: %d\n" GREEN "Success: %d\n" RESET RED "Incorrect: %d\n" RESET,
            all_tests, success_tests, all_tests-success_tests);
    
    fclose(file_p);
    return PASS;
}

int RunOneTest(TestCase test, int test_count) {

    double x1 = NAN, x2 = NAN;
    int roots = SolveSquare(test.a, test.b, test.c, &x1, &x2);

    //! Sort (x1,x2) in ascending order + place NAN in x1
    SortX(&x1, &x2);

    double x1_exp = test.x1_exp;
    double x2_exp = test.x2_exp;

    SortX(&x1_exp, &x2_exp);

    if (roots == test.roots) {

        if (isnan(x2) && isnan(x2_exp)) { //! if both roots are nan
            
            return 1;  //! success, 0 roots
        
        }
        else if (!isnan(x2) && !isnan(x2_exp)) {  //!< x2 and x2_exp are not NAN

            if (IsEqual(x2, x2_exp, EPS)) {

                if (!isnan(x1) && !isnan(x1_exp)) {

                    if (IsEqual(x1, x1_exp, EPS))
                        return 1;  //! success - 2 roots
                    
                }

                else if (isnan(x1) && isnan(x1_exp)) {  //! x1 and x1_exp are NAN, x2 and x2_exp are not NAN

                    return 1;   //! success - 1 root (x2)
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

    ASSERT(filename);

    FILE *file_p = fopen(filename, "a");
    
    if (!file_p) {
        printf("Error opening file: <%s>\n", filename);
        return END;
    }    

    fprintf(file_p, "Date: %s, time: %s\n"
                    "Test #%d: \nIncorrect: x1 = %lg, x2 = %lg\n"
                                    "Expected: x1_exp = %lg, x2_exp = %lg\n\n",
                                    __DATE__, __TIME__,
                                    test_count, x1, x2, test.x1_exp, test.x2_exp);

    fclose(file_p);
    return PASS;
}

int GetCoeffs(char buf[], double *a, double *b, double *c, int *roots_exp, double *x1_exp, double *x2_exp) {

    ASSERT(a);
    ASSERT(b);
    ASSERT(c);
    ASSERT(roots_exp);
    ASSERT(x1_exp);
    ASSERT(x2_exp);

    ASSERT((a != b));
    ASSERT((b != c));
    ASSERT((a != c));
    ASSERT((a != x1_exp));
    ASSERT((x1_exp != x2_exp));

    if (sscanf(buf, "%lf %lf %lf %d %lf %lf", a, b, c, roots_exp, x1_exp, x2_exp) == 6) {

        return PASS;
    }

    return END;
}

int SortX(double *x_1, double *x_2) {

    ASSERT(x_1);
    ASSERT(x_2);
    ASSERT((x_1 != x_2));

    double dop_x = 0;

    if (isnan(*x_1) || isnan(*x_2)) {

        if (isnan(*x_2)) {

            *x_2 = *x_1;
            *x_1 = NAN;
            //! place NAN (if present) always in x_1
            return PASS;
        }

    }

    else if (IsBigger(*x_1, *x_2, EPS)) {

        dop_x = *x_2;
        *x_2 = *x_1;
        *x_1 = dop_x;

    }
    
    return END;    
}

int WriteRandomEq(char filename[]) {

    ASSERT(filename);

    FILE *file_p = fopen(filename, "w");
    
    if (!file_p) {
        printf("Error opening file: <%s>\n", filename); //----------------------------errno strerror
        return END;
    }

    for (unsigned int i = 0; i < N_RANDOM_TESTS; i++) {

        TestCase test = {0};
        //! generate coefficients
        MakeCoeffs(&test.a, &test.b, &test.c, &test.roots, &test.x1_exp, &test.x2_exp);

        //! write the equation to the buffer and output to the file
        char buffer[BUFSIZE] = "";

        int n_fields = FillBufferFromStruct(test, buffer, sizeof(buffer));

        if (n_fields < 0) {
            printf("Error writing structure\n");
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

    ASSERT((a != rand_x1));
    ASSERT((a != rand_x2));
    ASSERT((a != b));
    ASSERT((a != c));
    ASSERT((b != c));
    ASSERT((rand_x1 != rand_x2));

    *rand_x1 = MIN_RANDOM_NUM + double(rand() % MAX_RANDOM_COEFF);
    *rand_x2 = MIN_RANDOM_NUM + double(rand() % MAX_RANDOM_COEFF);

    *roots = TWOROOTS;
    *a = MIN_RANDOM_NUM + double(rand() % MAX_RANDOM_COEFF);
    *c = (*rand_x1) * (*rand_x2) * (*a);
    *b = -((*rand_x1) + (*rand_x2)) * (*a);
}

int FillBufferFromStruct(TestCase test, char buffer[], int size_of_buffer) {

    ASSERT (buffer);

    int n_fields = snprintf(buffer, size_of_buffer, "%10.2lf %15.2lf %15.2lf %3d %15.2lf %15.2lf\n",                 
                                                    test.a, test.b, test.c, test.roots, test.x1_exp, test.x2_exp);

    return n_fields;
}