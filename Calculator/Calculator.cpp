#include "Errors.h"
#include "GConsts.h"
#include <iostream>
#include <vector>
#include "Functions.h"

using std::cin;
using std::cout;
using std::cerr;
using std::exception;
using std::vector;

/*
Calculator program:

This program reads simple mathematical expressions entered
by the user and calculates and displays the results

The grammar for input is:

Statement:
    Expression
    Print
    Quit

Print:
    ;

Quit:
    q

Expression:
    Term
    Expression + Term
    Expression - Term
Term:
    Primary
    Term * Primary
    Term / Primary
    Term % Primary
Primary:
    Number
    (Expression)
    -Primary
    +Primary
Number:
    floating-point-literal

Input comes from cin through the Token_stream called ts.
*/

/*
Function: main()

Description: main function of program. displays instructions on load.
Continuously runs program while input is valid. checks for quit characters
and print characters.
*/

int main()
try {
    define_name("pi", 3.1415926535);
    define_name("e", 2.7182818284);

    print_intro();
    calculate();
    return 0;


}

catch (runtime_error& e) {
    cerr << e.what() << '\n';
    return 1;
}
catch (...) {
    cerr << "exception \n";
    return 2;
}

