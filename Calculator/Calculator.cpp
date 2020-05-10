#include "Facilities.h"
#include "Token.h"

const char number = 'n';
const char quit = 'q';
const char print = ';';
const string prompt = "> ";
const string result = "= ";

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
Class: Token_stream

Description: This class will be used as a buffer to hold a token.
It comes equipped with two public functions, one to get a token
from user input or the buffer. the other to put a token in the
buffer. two private variables will be needed by for member functions
to carry out their task. One is a Token that is the actual buffer,
the other is a bool that keeps track of whether or not the buffer
is full
*/

class Token_stream {
public:
    Token_stream();
    Token get();
    void putback(Token t);
    void ignore(char c);
private:
    bool full;
    Token buffer;
};

/*
Token_stream constructor: initializes buffer to empty
*/

Token_stream::Token_stream()
    :full(false), buffer(0)
{
}

/*
Token_stream member function get(): gets a token from the buffer or a 
character from the user input stream and assigns it to the appropriate 
token
*/

Token Token_stream::get()
{
    if (full) {
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch;

    switch (ch) {
    case quit:
    case print:
    case '!': 
    case '{': 
    case '}': 
    case '(': 
    case ')': 
    case '+': 
    case '-': 
    case '*': 
    case '/': 
    case '%':
        return Token(ch);
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
        cin.putback(ch);
        double val;
        cin >> val;
        return Token(number, val);
    }
    default:
        error("Bad token");
    }
}

/*
Token_stream member function putback(): puts a token in the buffer
*/

void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;
    full = true;
}

/*
Token_stream member function ignore(): clears input stream and buffer.
*/

void Token_stream::ignore(char c)
{
    if (full && c == buffer.kind)
    {
        full = false;
        return;
    }
    full = false;
    char ch = 0;
    while (cin >> ch)
        if (ch == c)return;
}

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

Token_stream ts;

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
    keep_window_open("~~");
    return 1;
}
catch (...) {
    cerr << "exception \n";
    return 2;
}

