#include "Errors.h"
#include "Token_stream2.h"
#include "GConsts.h"
#include <iostream>

using std::cin;

Token_stream2::Token_stream2()
    :full(false), buffer(0)
{
}

Token2 Token_stream2::get()
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
        return Token2(ch);
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
        cin.putback(ch);
        double val;
        cin >> val;
        return Token2(number, val);
    }
    default:
        error("Bad token");
    }
}

/*
Token_stream member function putback(): puts a token in the buffer
*/

void Token_stream2::putback(Token2 t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;
    full = true;
}

/*
Token_stream member function ignore(): clears input stream and buffer.
*/

void Token_stream2::ignore(char c)
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