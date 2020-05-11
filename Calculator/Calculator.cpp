#include "Errors.h"
#include "Token_stream.h"
#include "GConsts.h"
#include<iostream>

using std::cin;
using std::cout;
using std::cerr;
using std::exception;



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
Class: Token

Description: This class will be used to hold user input as a
two part object. One for its kind, the other for its value. 
It comes equipped with two initializers, one for characters
that have no value, the other for ones that do
*/

class Variable {
public:
    string name;
    double value;
};

/*
Basic functions
*/

double primary();
double factorial();
double term();
double expression();

/*
utility functions
*/

double calculate_factorial(int val);

/*
Globals
*/

Token_stream ts2;

/*
Function: primary()

Description: This is the last step in the grammar chain,
it checks for primaries, which include {, (, and numbers.
it will then either return the number to factorial() or evaluate 
another expression and then return a number to term()
*/

double primary()
{
    Token t = ts2.get();
    switch (t.kind) 
    {
        case '{':
        {
            double d = expression();
            t = ts2.get();
            if (t.kind != '}') error("'}' expected");
            return d;
        }
        case '(':
        {
            double d = expression();
            t = ts2.get();
            if (t.kind != ')') error("')' expected");
            return d;
        }
        case number:
        {
            double d = t.value;
            return d;
        }
        case '-':
            return -primary();
        case '+':
            return primary();
        default:
            error("primary expected");
    }
}

/*
Function: factorial()

Description: This funcions is used to check if a '!' has been 
found after a number has been read in or an expression has 
been evaluated. if so, return the factorial of the number or
evaluated expression to term()
*/

double factorial()
{
    double left = primary();
    Token t = ts2.get();

    while (true)
    {
        switch (t.kind)
        {
        case '!':
        {
            left = calculate_factorial(left);
            return left;
        }
        default:
            ts2.putback(t);
            return left;
        }
    }
}

/*
Function: term()

Description: this function handles multiplication and division.
if a * or / are found, evaluate for factorial and primary and
multiply by the previous factorial or primary. return the result 
to expression()
*/

double term()
{
    double left = factorial();
    Token t = ts2.get();

    while (true) {
        switch (t.kind) {
            case '*':
            {
                left *= factorial();
                t = ts2.get();
                break;
            }
            case '/':
            {
                double d = factorial();
                if (d == 0) error("divide by zero");
                left /= d;
                t = ts2.get();
                break;
            }
            case '%':
            {
                int i1 = narrow_cast<int>(left);
                int i2 = narrow_cast<int>(factorial());
                if (i2 == 0) error("%: divide by zero");
                left = i1 % i2;
                t = ts2.get();
                break;
            }

            default:
            {
                ts2.putback(t);
                return left;
            }
        }
    }
}

/*
Function: expression()

Description: This function handles + and -. if one is found, it appropriately
combines the previous term() with the next term() and returns the result
*/

double expression()
{
    double left = term();
    Token t = ts2.get();
    while (true) {
        switch (t.kind) {
        case '+':
            left += term();
            t = ts2.get();
            break;
        case '-':
            left -= term();
            t = ts2.get();
            break;
        default:
            ts2.putback(t);
            return left;
        }
    }
}

/*
Function: calculate_factorial()

Description: This function takes a number, calculate its
factorial, and returns its number
*/

double calculate_factorial(int val)
{
    for (int i = val - 1; i > 0; i--)
        val *= i;
    return val;
}

/*
Clears input and buffer after bad input.
*/

void clean_up_mess()
{
    ts2.ignore(print);
}

void calculate()
{
    while (cin)
    try {
        cout << prompt;
        Token t = ts2.get();
        while (t.kind == print) t = ts2.get();
        if (t.kind == quit) return;
        ts2.putback(t);
        cout << result << expression() << '\n';
    }
    catch (exception& e) {
        cerr << e.what() << '\n';
        clean_up_mess();
    }
}

void print_intro()
{
    cout << "Welcome to our simple calculator. Please enter expressions using \n";
    cout << "floating-point numbers. enter ';' to calculate and display, 'q' to \n";
    cout << "quit. The following operators are supported : \n";
    cout << "   + - * / ( ) % \n";
}

/*
Function: main()

Description: main function of program. displays instructions on load.
Continuously runs program while input is valid. checks for quit characters
and print characters.
*/

int main()
try {
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

