#include "Grammar.h"
#include "Error_handling.h"
#include "Token_stream.h"
#include "GBLS.h"
#include "Calculator.cpp"
#include "Functions.h"

/*
Function: primary()

Description: This is the last step in the grammar chain,
it checks for primaries, which include {, (, and numbers.
it will then either return the number to factorial() or evaluate
another expression and then return a number to term()
*/

double Grammar::primary()
{
    Token t = ts.get();
    switch (t.kind)
    {
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

double Grammar::factorial()
{
    double left = primary();
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
        case '!':
        {
            left = Functions::calculate_factorial(left);
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

double Grammar::term()
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

double Grammar::expression()
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
