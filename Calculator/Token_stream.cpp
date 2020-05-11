#include "Errors.h"
#include "Token_stream.h"
#include "GConsts.h"
#include <iostream>

using std::cin;

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