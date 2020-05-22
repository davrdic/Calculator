#include "Errors.h"
#include "Functions.h"
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

bool new_line(char& ch)
{
    if (cin.get() == '\n') {
        cin.unget();
        return true;
    }
    else {
        cin.unget();
        return false;
    }
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
    int i = 0;

    if (new_line && i > 0) cin.putback(';');
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
    case '=':
    case ',':
        return Token(ch);
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        cin.putback(ch);
        double val;
        cin >> val;
        return Token(number, val);
    default:
        if (isalpha(ch)) {
            string s;
            s += ch;
            while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
            cin.putback(ch);
            if (s == declkey) return Token(let);
            if (s == squarekey) return Token(square);
            if (s == powerkey)  return Token(power);
            if (is_declared(s)) return Token(number, get_value(s));
            return Token(name,s);
        }
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
    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;
    char ch = 0;
    while (cin >> ch) {
        new_line(ch);
        if (ch == c) {
            return;
        }
    }
}