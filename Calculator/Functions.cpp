#include <iostream>
#include <vector>
#include "Errors.h"
#include "GConsts.h"
#include "Functions.h"
#include "Token_stream.h"
#include "Variable.h"

using std::cin;
using std::cout;
using std::cerr;
using std::exception;
using std::vector;

Token_stream ts;
vector<Variable> var_table;

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
Function: primary()

Description: This is the last step in the grammar chain,
it checks for primaries, which include {, (, and numbers.
it will then either return the number to factorial() or evaluate
another expression and then return a number to term()
*/

double primary()
{
    Token t = ts.get();
    switch (t.kind)
    {
    case square:
    {
        t = ts.get();
        if (t.kind != '(') error("'(' expected");
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        return sqrt(d);
    }
    case power:
    {
        t = ts.get();
        if (t.kind != '(') error("'(' expected");
        double d1 = expression();
        t = ts.get();
        if (t.kind != ',') error("',' expected");
        double d2 = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        return pow(d1, d2);
    }
    case '{':
    {
        double d = expression();
        t = ts.get();
        if (t.kind != '}') error("'}' expected");
        return d;
    }
    case '(':
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        return d;
    }
    case number:
    {
        double d = t.value;
        return d;
    }
    case '-':
        t = ts.get();
        if (t.kind == '-')
        {
            ts.putback(t);
            error("poop");
        }
        else {
            ts.putback(t);
            return -primary();
        }
    case '+':
        t = ts.get();
        if (t.kind == '+')
        {
            ts.putback(t);
            error("poop");
        }
        else {
            ts.putback(t);
            return +primary();
        }
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
    Token t = ts.get();

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
            ts.putback(t);
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
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
        case '*':
        {
            left *= factorial();
            t = ts.get();
            break;
        }
        case '/':
        {
            double d = factorial();
            if (d == 0) error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        case '%':
        {
            int i1 = narrow_cast<int>(left);
            int i2 = narrow_cast<int>(factorial());
            if (i2 == 0) error("%: divide by zero");
            left = i1 % i2;
            t = ts.get();
            break;
        }

        default:
        {
            ts.putback(t);
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
    Token t = ts.get();
    while (true) {
        switch (t.kind) {
        case '+':
            left += term();
            t = ts.get();
            break;
        case '-':
            left -= term();
            t = ts.get();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}

double get_value(string s)
{
    for (const Variable& v : var_table)
        if (v.name == s) return v.value;
    error("get: undefined variable ", s);
}

void set_value(string s, double d)
{
    for (Variable& v : var_table)
        if (v.name == s) {
            v.value = d;
            return;
        }
    error("set: undefined variable ", s);
}

/*
Clears input and buffer after bad input.
*/

void clean_up_mess()
{
    ts.ignore(print);
}

void calculate()
{
    while (cin)
        try {
        cout << prompt;
        Token t = ts.get();
        while (t.kind == print) t = ts.get();
        if (t.kind == quit) return;
        ts.putback(t);
        cout << result << statement() << '\n';
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

double statement()
{
    Token t = ts.get();
    switch (t.kind) {
    case let:
        return declaration();
    default:
        ts.putback(t);
        return expression();
    }
}

bool is_declared(string var)
{
    for (const Variable& v : var_table)
        if (v.name == var) return true;
    return false;
}

double define_name(string var, double val)
{
    if (is_declared(var)) error(var, " declared twice");
    var_table.push_back(Variable{ var, val });
    return val;
}

double declaration()
{
    Token t = ts.get();
    if (t.kind != name) error("name expected in declaration");
    string var_name = t.name;

    Token t2 = ts.get();
    if (t2.kind != '=') error("= missin in declarion of ", var_name);

    double d = expression();
    define_name(var_name, d);
    set_value(var_name, d);
    return d;
}